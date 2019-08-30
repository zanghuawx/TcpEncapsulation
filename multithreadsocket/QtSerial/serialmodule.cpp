/*********************************************************************************
*Copyright(C):
*FileName:  serialmodule.cpp
*Author:    WuXiao
*Version:   1.0
*Date:      2019.08.30
*Description:  base class of serial port communicate
*Others:
*Function List:
**********************************************************************************/


#include <QtSerialPort/QSerialPort>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "serialmodule.h"


//#define MaxFrameBytes 256


SerialModule::SerialModule(QThread *thd, const QString &portName, const int &baud, QObject *parent)
    : thd_(thd), portName_(portName), baud_(baud), serial_(), parsePeriod_(50),
      mtx_(), readBufQueue_(), parseTimer_()
{
    init();
}

SerialModule::~SerialModule()
{

}

void SerialModule::init()
{
    qDebug() <<  "function SerialModule::init() attach on main thread id: " << QThread::currentThreadId();
    this->moveToThread(thd_);
    serial_.setPortName(portName_);
    serial_.setBaudRate(baud_);
    serial_.setFlowControl(QSerialPort::NoFlowControl);
    serial_.setDataBits(QSerialPort::Data8);
    serial_.setStopBits(QSerialPort::OneStop);

    connect(&parseTimer_, SIGNAL(timeout()), this, SLOT(popQueue()), Qt::QueuedConnection);
    connect(this, SIGNAL(writeSignal(const char*, const int&)), this, SLOT(writeData(const char*, const int&)), Qt::QueuedConnection);

    /*************************test code**********************/
    connect(&testProduceTimer, SIGNAL(timeout()), this, SLOT(testReadData()), Qt::QueuedConnection);
    connect(&testConsumeTimer, SIGNAL(timeout()), this, SLOT(popQueue()), Qt::QueuedConnection);
    connect(&testProduceTimer, SIGNAL(timeout()), this, SLOT(testWriteData()), Qt::QueuedConnection);
    testProduceTimer.start(2000);
    testConsumeTimer.start(3000);
    /*************************test end***********************/

}

void SerialModule::openPort()
{
    serial_.open(QIODevice::ReadWrite);
    parseTimer_.start(parsePeriod_);
    connect(&serial_, SIGNAL(readyRead()), this, SLOT(readData()), Qt::QueuedConnection);

}

void SerialModule::closePort()
{
    disconnect(&serial_, SIGNAL(readyRead()), this, SLOT(readData()));
    serial_.clear();
    serial_.close();
    parseTimer_.stop();
}

int SerialModule::extractData(char *data, const int &length)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (readBufQueue_.isEmpty()) {
        qDebug() << "serial queue empty named " << portName_;
        return -1;
    }
    return (int)readBufQueue_.read(data, length);
}

void SerialModule::packageMessageFrame(const int &type, const int &command, char *data, const int &dataLength)
{
    qDebug() << " SerialModule::packageMessageFrame---";

    char buf[MaxFrameBytes];
    buf[0] = Head;
    buf[1] = (char)dataLength;
    buf[2] = (char)type;
    buf[3] = (char)command;
    memcpy(&buf[4], data, dataLength);
    buf[dataLength + 4] = checkSum(type, dataLength, data);
    buf[dataLength + 5] = Rear;
    int frameLength = dataLength + 6;

//    std::string str(buf, frameLength);
//    std::cout << "\n\r SerialModule::packageMessageFrame---" << frameLength << " bytes " << str;
    emit writeSignal(buf, frameLength);
}

char SerialModule::checkSum(quint8 dataType, quint8 len, const char *data)
{
    char check = 0;
    check = dataType + len;

    for (quint8 i = 0; i < len; i++) {
        check += data[i];
    }

    check ^= 0xff;
    return check;
}

int SerialModule::writeData(const char *data, const int& length)
{
    std::string str(data, length);
    std::cout << " SerialModule::writeData---" << length << " bytes " << str;
    for (int i = 0; i < length; i++) {
        printf(" %x ", data[i]);
    }
    QByteArray ba;
    ba.append(data, length);
    int res;
    res = (int)serial_.write(data, length);

    if (res <= 0) {
        qDebug() << "write serial error named " << portName_;
        return -1;
    }
    qDebug() << "write serial named " << portName_ << " " << res << " bytes";
    return res;
}

void SerialModule::setParsePeriod(const int &per)
{
    parsePeriod_ = per;
}

void SerialModule::parseDataTask()
{
    qDebug() << "SerialModule::parseDataTask()--- " <<  " thread id: " << QThread::currentThreadId();
    std::lock_guard<std::mutex> lock(mtx_);
    int state = 0;
    while (!readBufQueue_.isEmpty()) {
        char data[MaxFrameBytes];
        memset(data, 0, sizeof(data));
        readBufQueue_.read(data, (qint64)sizeof(data));
        for (int i = 0; i < sizeof(data); ++i) {
            printf("%x", data[i]);
        }
    }
    qDebug() << "SerialModule::parseDataTask()--- block";
}

void SerialModule::popQueue()
{
    parseDataTask();
}

void SerialModule::readData()
{
    qDebug() << "SerialModule::readData()--- " << portName_ <<  " thread id: " << QThread::currentThreadId();
    if (serial_.bytesAvailable() <= 0) {
        return;
    }
    QByteArray ba = serial_.readAll();
    std::lock_guard<std::mutex> lock(mtx_);
    readBufQueue_.append(ba);

}

void SerialModule::testReadData()
{
    qDebug() << "SerialModule::testReadData()--- " << portName_ <<  " thread id: " << QThread::currentThreadId();
    char testdata[150];
    for (int i = 0; i < 150; ++i) {
        testdata[i] = i;
    }
    testdata[19] = 0xFF;
    testdata[20] = 0xBB;
    QByteArray ba(testdata, sizeof(testdata));
    std::lock_guard<std::mutex> lock(mtx_);
    readBufQueue_.append(ba);
    qDebug() << "SerialModule::testReadData()--- block";
}

void SerialModule::testWriteData()
{
    qDebug() << " SerialModule::testWriteData---";
    int type = 0x50;
    int command = 0x51;
    int dataLength = 10;
    char data[] = {97,98,99,100,101,102,103,104,105,106};
    packageMessageFrame(type, command, data, dataLength);
}

const int SerialModule::MaxFrameBytes = 256;
const char SerialModule::Head = 0x0A;
const char SerialModule::Rear = 0xBB;
