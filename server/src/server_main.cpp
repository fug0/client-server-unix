#include "../include/Server.hpp"

int main(int argc, char* argv[])
{   
    if (argc != 2) {
        std::cerr << "You need to pass <port> value!" << std::endl;
        return 0;
    }
    
    Server MainServer(std::atoi(argv[1]), std::thread::hardware_concurrency());

    if(MainServer.Start() == ServerStatus::serverIsStarted) {
        std::cout << "Server successfully started on port " << MainServer.GetPort() << "!" << std::endl << std::endl;
        MainServer.ThreadPoolJoin();
    }

    std::cerr << "Server starting failed!" << std::endl;
    return 0;
}

