#include <iostream>
#include <string>
#include <algorithm>
#include <thread>

#include "../include/Client.hpp"

int main(int argc, char* argv[]) {
	if (argc != 3)
	{
		std::cerr << "Usage: ./main <host's ip> <host's port>" << std::endl;
		return 1;
	}

	Client client(argv[1], std::atoi(argv[2]));
	if(client.SocketInit() != ClientStatus::clientOperatingOk) {
		std::cerr << "Socket initialization error!" << std::endl;
	}
	
	std::thread sendThread([&client]() {client.sendThreadFunc();});
	std::thread receiveThread([&client]() {client.receiveThreadFunc();});

	if (sendThread.joinable())    sendThread.join();
	if (receiveThread.joinable()) receiveThread.join();

	return 0;
}