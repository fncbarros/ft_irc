#include "includes/Client.hpp"

Client::Client(/* args */)
{
}

void    Client::wellcome_client()
{
    read_message();
}

void    Client::read_message()
{
    char buffer[BUFFER_SIZE] = {0};

    int bytesReceived = recv(_socket_id, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0)
    {
        std::cout << "Failed to read Client Socket" << std::endl;
    }
    std::cout << "Client message received" << std::endl;
    std::cout << "[ " << buffer << " ]" << std::endl;
}

Client::Client(socket_t socket_id) : _socket_id(socket_id)
{

    std::cout << "Client created" << std::endl;
    std::cout << "Client id " << _socket_id << std::endl; 
    wellcome_client();
}

Client::~Client()
{
    close(_socket_id);
    std::cout << "Client destroyed" << std::endl;
}