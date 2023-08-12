#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "common.hpp"

class Client
{
    // Special functions
    public:
        Client(const std::string &nick, const std::string &user, const std::string &pass);
        ~Client();
    
    // Public functions
    public:
        const std::string get_nickname(void) const;
        const std::string get_username(void) const;
        const std::string get_password(void) const;
        bool        auth(const std::string &nick, const std::string &pass) const;

    // Internal functions
    private:
        Client();


    // Data
    private:
        std::string _nickname;
        std::string _username;
        std::string _password;
};

#endif