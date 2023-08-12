#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "common.hpp"

class Client
{
    private:
        socket_t _socket_id;
        std::string _nickname;
        std::string _username;
        std::string _password;
        void    wellcome_client();
        void    read_message();

    public:
        Client(/* args */);
        Client(socket_t socket_id);
        ~Client();
};

#endif