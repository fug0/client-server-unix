#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "SharedBuffer.hpp"
#include "DataResolver.hpp"

typedef int sockdesc_t;

constexpr char kDefaultIP[] = "127.0.0.1";
constexpr ushort kDefaultPort = 8080;

enum class ClientStatus {
	clientOperatingOk = 0,
	clientDisconnected,
	clientSocketInitErr,
	clientSocketConnErr,
	clientMessageSendErr
};

class Client {
public:
	Client(const char* ipv4 = kDefaultIP, uint16_t port = kDefaultPort) : is_connected_(false) {
		sock_addr_.sin_family = AF_INET;
		sock_addr_.sin_port = htons(port); 
		sock_addr_.sin_addr.s_addr = inet_addr(ipv4);
	};
	~Client() = default;

	ClientStatus SocketInit();

	void receiveThreadFunc();
	void sendThreadFunc();
private:
	sockdesc_t sock_;
	sockaddr_in sock_addr_;
	std::atomic<bool> is_connected_;
	sharedBuff<std::string> strBuffer;

	ClientStatus SendToHost(uint16_t num_to_send);
};

#endif /* CLIENT_HPP */