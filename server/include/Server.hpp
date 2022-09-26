#ifndef MULTITHR_SERVER_HPP
#define MULTITHR_SERVER_HPP

#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include "../include/ThreadPool.hpp"

constexpr uint16_t kBufferSize = 3000;

typedef int sockdesc_t;

enum class ServerStatus {
    serverIsStarted = 0,
    serverIsStopped,
    sockInitError,
    sockBindError,
    sockKeepaliveError,
    sockListeningError,
    sockOptError
};

class Server {
public:
    Server(uint16_t port, uint8_t thread_count = std::thread::hardware_concurrency()) :
     port_(port), server_socket_(-1), server_status_(ServerStatus::serverIsStopped), conn_handle_thread_(), thread_pool_(thread_count) {}

    ~Server() {};

    ServerStatus Start();

    void ThreadPoolJoin();

    uint16_t GetPort();
private:
    ServerStatus server_status_;

    uint16_t port_;
    sockdesc_t server_socket_; 
    
    std::thread conn_handle_thread_;
    ThreadPool thread_pool_;

    static void ConnHandlerLoop(Server* p_inst);

    static void ClientSockProcessingLoop(sockdesc_t client_sock_desc);
};    

#endif /* MULTITHR_SERVER_HPP */