#include "PetProtocMessCodec.h"
#include "RandomProductPet.h"
#include "pet.pb.h"
#include "../Timer.h"
#include <time.h>
#include <unistd.h>
#include <memory>

petproto::PetProto pet;
bool cycleSendFlag = false;

void newPetRow() {
	RandomProductPet randPet;
	pet.set_name(randPet.get_name());
	pet.set_owner(randPet.get_owner());
	pet.set_species(randPet.get_species());
	pet.set_sex(randPet.get_sex());
	pet.set_birth(randPet.get_birth());
	pet.set_death(randPet.get_death());

	cycleSendFlag = true;
	
}

int main() {
	EventLoop* loop = new EventLoop();
	std::string ip("127.0.0.1");
	int port = 8032;
	PetProtocMessCodec client(loop, ip, port, "PetProtocolBufferClient");
	client.start();

	//Timer(EventLoop * loop, const int& ms, const TimerMode & mode = CYCLE);
	std::shared_ptr<Timer> producePetTimer(new Timer(loop, 3000));
	producePetTimer->addOrUpdateTimerCallback("producePetTimer", std::bind(&newPetRow));
	producePetTimer->start();


	const std::shared_ptr<TcpClient>& cc = boost::any_cast<const std::shared_ptr<TcpClient>&>(client.getHost());
	std::cout << "usleep(800000)" << std::endl;
	usleep(800000);
	while (cc->isConnect()) {
		//client.sendContactsByTcp(cc->getTcpConnectPtr());

		if (cycleSendFlag) {
			cycleSendFlag = false;
			google::protobuf::Message* mess = &pet;
			client.sendContactsByTcp(cc->getTcpConnectPtr(), *mess);
			std::cout << pet.name() << "/" << pet.owner() << "/" << pet.birth() << std::endl;
		}
		
		
	}
	


	usleep(3000000);
	
}
