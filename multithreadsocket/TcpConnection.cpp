
//TcpConnection类，表示客户端连接


#include "TcpConnection.h"
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>



#define BUFSIZE 4096


int TcpConnection::recvn(int fd, std::string & bufferin) {
	int nbyte = 0;
	char buf[BUFSIZE];
	int readsum = 0;
	for (;;) {
		nbyte = read(fd, buf, BUFSIZE);
		if (nbyte > 0) {
			bufferin.append(buf, nbyte);
			readsum += nbyte;
			//数据不够一块,最后一次读了
			if (nbyte < BUFSIZE) {
				return readsum;
			} else {
				continue;
			}
		} else if (nbyte < 0) {	//异常
			if (errno == EAGAIN) {//系统缓冲区未有数据，非阻塞返回
			
				//std::cout << "EAGAIN,系统缓冲区未有数据，非阻塞返回" << std::endl;
				return readsum;
			} else if (errno == EINTR) {
				std::cout << "errno == EINTR" << std::endl;
				continue;
			} else {
				//可能是RST
				perror("recv error");
				//std::cout << "recv error" << std::endl;
				return -1;
			}
		} else {	//返回0, 客户端关闭了socket，FIN(结束连线)
			return 0;
		}
	}
}


int TcpConnection::sendn(int fd, std::string & bufferout) {
	ssize_t nbyte = 0;
	size_t length = 0;
	int writesum = 0;
	length = bufferout.size();
	if (length >= BUFSIZE) {
		length = BUFSIZE;
	}
	for (;;) {
		nbyte = write(fd, bufferout.c_str(), length);
		if (nbyte > 0) {
			bufferout.erase(0, nbyte);
			writesum += nbyte;
			length = bufferout.size();
			if (length >= BUFSIZE) {
				length = BUFSIZE;
			}
			if (length == 0) {
				return writesum;
			}
		} else if (nbyte < 0) {
			if (errno == EAGAIN) {//系统缓冲区满，非阻塞返回
			
				std::cout << "write errno == EAGAIN,not finish!" << std::endl;
				return writesum;
			}
			else if (errno == EINTR) {
				std::cout << "write errno == EINTR" << std::endl;
				continue;
			}
			else if (errno == EPIPE) {
				//客户端已经close，并发了RST，继续wirte会报EPIPE，返回0，表示close
				perror("write error");
				std::cout << "write errno == client send RST" << std::endl;
				return -1;
			}
			else {
				perror("write error");//Connection reset by peer
				std::cout << "write error, unknow error" << std::endl;
				return -1;
			}
		} else {
			//应该不会返回0
			//std::cout << "client close the Socket!" << std::endl;
			return 0;
		}
	}
}


TcpConnection::TcpConnection(EventLoop * loop, int fd, const struct sockaddr_in & clientaddr) 
	: fd_(fd), loop_(loop), asyncProcess_(false), spChannel_(new Channel()), 
	clientAddr_(clientaddr), halfClose_(false), disConnected_(false), 
	bufferIn_(), bufferOut_() {
	
	//注册事件执行函数
	spChannel_->setFd(fd_);
	spChannel_->setReadHandle(std::bind(&TcpConnection::handleRead, this));
	spChannel_->setWriteHandle(std::bind(&TcpConnection::handleWrite, this));
	spChannel_->setCloseHandle(std::bind(&TcpConnection::handleClose, this));
	spChannel_->setErrorHandle(std::bind(&TcpConnection::handleError, this));
	spChannel_->setEvents(EPOLLIN | EPOLLET);
}

TcpConnection::~TcpConnection() {
	//多线程下，加入loop的任务队列？不用，因为已经在当前loop线程
	//移除事件,析构成员变量
	loop_->removeChannelFromPoller(spChannel_.get());
	close(fd_);
}

void TcpConnection::addChannelToLoop() {
	//std::cout << "TcpConnection line 127: addChannelToLoop(), point loop_:" << loop_ <<  std::endl;
	//将“事件添加到epool中”制作成任务,并添加到eventloop的任务列表,
	//executeTask时会执行“”,即执行loop_->addChannelToPoller(spChannel_.get())
	loop_->addTask(std::bind(&EventLoop::addChannelToPoller, loop_, spChannel_.get()));
}

void TcpConnection::sendString(const std :: string & str) {
	bufferOut_ += str;	//跨线程消息投递
	//判断当前线程是不是loop io线程 如果是当前线程调用sendString()则...
	if (loop_->getThreadId() == std::this_thread::get_id()) {
		sendInLoop();
	} else {
		//如果是其它线程(工作线程,用户线程)(非本eventLoop所在的线程,而eventloop就是一个io线程,只做费时处理)调用send...
		asyncProcess_ = false;	//异步调用结束
		//跨线程调用,加入IO线程的任务队列，唤醒I/O线程,eventloop->loop->pollWait()发生事件不再阻塞.
		loop_->addTask(std::bind(&TcpConnection::sendInLoop, shared_from_this()));	//shared_from_this: 在工作线程和io线程引用this对象,引用次数都会+1
	}
}

void TcpConnection::sendInLoop() {
	int ret = sendn(fd_, bufferOut_);
	if (ret > 0) {
		uint32_t events = spChannel_->getEvents();
		//缓冲区满了，数据没发完，就设置EPOLLOUT事件触发	
		if (bufferOut_.size() > 0) {
			spChannel_->setEvents(events | EPOLLOUT);
			loop_->updateChannelToPoller(spChannel_.get());
		} else {
			//数据已发完
			spChannel_->setEvents(events & (~EPOLLOUT));
			sendCompleteCallback_(shared_from_this());	//回调
			//发送完毕，如果是半关闭状态，则可以close了
			if (halfClose_) {
				handleClose();
			}
		}
	} else if (ret < 0) {
		handleError();
	} else {
		handleClose();
	}
}

void TcpConnection::shutDown() {
	if (loop_->getThreadId() == std::this_thread::get_id()) {
		shutDownInLoop();
	} else {
		//不是IO线程，则是跨线程调用,加入IO线程的任务队列，唤醒
		loop_->addTask(std::bind(&TcpConnection::shutDownInLoop, shared_from_this()));
	}
}

void TcpConnection::shutDownInLoop() {
	if (disConnected_) {
		return;
	}
	std::cout << "shutdown" << std::endl;
	closeCallback_(shared_from_this());	//连接关闭的回调
	loop_->addTask(std::bind(connectCleanup_, shared_from_this()));	//自己不能清理自己
	disConnected_ = true;
}


void TcpConnection::handleRead() {
	//接收数据写入缓冲区
	int ret = TcpConnection::recvn(fd_, bufferIn_);
	//业务回调,可以利用工作线程池处理，投递任务
	if (ret > 0) {
		messageCallback_(shared_from_this(), bufferIn_);
	} else if (ret == 0) {
		handleClose();		//没有读到数据,说明对端已经close或shutdown(关闭)了
	} else {
		handleError();
	}
}


void TcpConnection::handleWrite() {
	//将缓冲区内容发送出去
	int ret = TcpConnection::sendn(fd_, bufferOut_);
	//如果发送出去了
	//获取事件以备更新
	//如果buffer还没发完,就设置EPOLLOUT事件触发，然后更新
	//如果buffer发完了，则回调,清EPOLLOUT事件
	if (ret > 0) {
		uint32_t event = spChannel_->getEvents();
		if (bufferOut_.size() > 0) {
			spChannel_->setEvents(event | EPOLLOUT);
			loop_->updateChannelToPoller(spChannel_.get());
		} else {
			spChannel_->setEvents(event & (~EPOLLOUT));
			sendCompleteCallback_(shared_from_this());
			if (halfClose_) {
				handleClose();
			}
		}
	} else if (ret < 0) {
		handleError();
	} else {
		handleClose();
	}
}


//handle 一般都在工作线程中执行
void TcpConnection::handleError() {
	if (disConnected_) {
		return;		//未连接,就不执行错误回调了
	}
	errorCallback_(shared_from_this());
	//连接标记为清理,将清理动作添加到loop的任务列表中
	loop_->addTask(std::bind(connectCleanup_, shared_from_this()));	//自己不能清理自己，交给loop(IO线程)执行
}


//对端关闭连接,有两种，一种close，另一种是shutdown(半关闭)
//但服务器并不清楚是哪一种，只能按照最保险的方式来，即发完数据再close
void TcpConnection::handleClose() {
	if (disConnected_) {
		return;
	}
	//缓冲器还有数据发送或接收，或异步处理在运行
	//如果还有数据待发送,则先发完,设置半关闭标志位
	//如果刚刚收到数据,则调用消息回调
	if (bufferIn_.size() > 0 || bufferOut_.size() > 0 || asyncProcess_) {
		halfClose_ = true;
		if (bufferIn_.size() > 0) {
			messageCallback_(shared_from_this(), bufferIn_);
		}
	} else {
		//执行清除连接任务，当然自己不能清除自己,交给loop(io事件)来执行
		loop_->addTask(std::bind(connectCleanup_, shared_from_this()));
		closeCallback_(shared_from_this());
		disConnected_ = true;
	}
}






