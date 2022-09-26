#include "../include/Server.hpp"

/* PUBLIC METHODS IMPLEMENTATIONS */

ServerStatus Server::Start() {
    sockaddr_in addr;

    if((server_socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket initialization failed!" << std::endl;
        return ServerStatus::sockInitError;
    }

    int reuse = 1;
    if ((setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse))) < 0) {
        std::cerr << "Setting socket option (SO_REUSEADDR) failed!" << std::endl;
        return ServerStatus::sockOptError;
    }

    if ((setsockopt(server_socket_, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse))) < 0) {
        std::cerr << "Setting socket option (SO_REUSEPORT) failed!" << std::endl;
        return ServerStatus::sockOptError;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);   
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((bind(server_socket_, (sockaddr *)&addr, sizeof(addr))) < 0)
    {
        std::cerr << "Socket binding error!" << std::endl;
        return ServerStatus::sockBindError;
    }

    if((listen(server_socket_, 1)) < 0)
    {
        std::cerr << "Socket listening preparing error!" << std::endl;
        return ServerStatus::sockListeningError;
    }

    thread_pool_.EnqueueJob(std::function<void(Server*)>(ConnHandlerLoop), this);

    return ServerStatus::serverIsStarted;
}

void Server::ThreadPoolJoin() {
    thread_pool_.JoinThreadPool();
}

uint16_t Server::GetPort() {
    return port_;
}

void Server::ConnHandlerLoop(Server* p_inst) {
    socklen_t client_addr_len = sizeof(sockaddr_in);
    sockaddr_in client_addr;
    sockdesc_t client_socket;

    if((client_socket = accept((p_inst->server_socket_), (sockaddr *)&client_addr, &client_addr_len)) < 0) {
        std::cerr << "New client acception error!" << std::endl;
    }
    else {
        std::cout << "New client connected on " << client_addr.sin_port << "!" << std::endl;

        (p_inst->thread_pool_).EnqueueJob(std::function<void(int)>(ClientSockProcessingLoop), client_socket);
    }

    (p_inst->thread_pool_).EnqueueJob(std::function<void(Server*)>(ConnHandlerLoop), p_inst);
};

void Server::ClientSockProcessingLoop(sockdesc_t client_sock_desc) {
    std::vector<char> buf(kBufferSize);
    size_t bytes_read = 0;

    while(1) {
        if((bytes_read = recv(client_sock_desc, buf.data(), buf.size(), 0)) > 0)
        {
            buf.emplace(std::find(buf.begin(), buf.end(), '\n'), '\0');

            if((strlen(buf.data()) > 2) 
              /*&& (std::find(buf.begin(), buf.end(), '\0') == std::find_if(buf.begin(), buf.end(), [](u_char ch)->bool { return (!std::isdigit(ch)); })) */
              && ((std::atoi(buf.data()) % 32) == 0)) {
                std::cout << "Received data from client: " << buf.data() << std::endl;
            } 
            /*else if (!(strcmp(buf.data(), "exit"))) { 
                std::cout << "Client has requested to close the connection!" << std::endl;
                break;
            }*/
            else {
                std::cout << "Received data does not match the conditions!" << std::endl;
            }
        } 
        else if (bytes_read < 0) {
            std::cerr << "Failed to read data from client!" << std::endl;
            break;
        }
        else {
            std::cout << "Client closed the connection!" << std::endl << std::endl;
            break;
        }
    }
    close(client_sock_desc);
};
