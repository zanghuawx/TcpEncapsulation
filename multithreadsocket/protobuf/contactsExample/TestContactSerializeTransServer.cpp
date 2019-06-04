#include "ContactSerializeTransfer.h"
#include <time.h>
#include <unistd.h>

int main() {
	EventLoop* loop = new EventLoop();
	int port = 8032;
	ContactSerializeTransfer server(loop, port, 2);
	server.start();
	loop->loop();

	

}

