#include "../include/Client.hpp"

ClientStatus Client::SocketInit() {
    if((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket initialization error!" << std::endl;
        return ClientStatus::clientSocketInitErr;
    }

	return ClientStatus::clientOperatingOk;
}


void Client::sendThreadFunc() {
	std::string str;

	while(1)  {
		DataResolver::InputStrung(str, is_connected_);

		DataResolver::StringFormat(str);

		strBuffer.push(str);
	}
}

void Client::receiveThreadFunc() {
	std::string str;

	while(1) {
		str = strBuffer.pull();

		uint16_t sum = DataResolver::CountStrNumsSum(str);

		SendToHost(sum);		
	}
}

ClientStatus Client::SendToHost(uint16_t num_to_send) {
	std::string buf_to_send = std::to_string(num_to_send);

	if((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket initialization error!" << std::endl;
		return ClientStatus::clientSocketInitErr;
    }
	if((connect(sock_, (sockaddr *)&sock_addr_, sizeof(sock_addr_))) < 0) {
		std::cerr << "Socket connection to host error!" << std::endl;
		return ClientStatus::clientSocketConnErr;
	}
	is_connected_ = true;
	if((send(sock_, buf_to_send.c_str(), buf_to_send.size(), 0)) < 0) {
		std::cout << "Message was not sent to server!" << std::endl;
		return ClientStatus::clientMessageSendErr;
	}
	close(sock_);
	is_connected_ = false;
	return ClientStatus::clientOperatingOk;
}
