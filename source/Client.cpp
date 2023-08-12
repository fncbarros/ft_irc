#include "includes/Client.hpp"

Client::Client(/* args */)
{
}

Client::Client(const std::string &nick, const std::string &user, const std::string &pass)
: _nickname(nick)
, _username(user)
, _password(pass)
{
    std::cout << "Client created: " << _nickname << " " << _username << " " << _password << std::endl;
}

const std::string Client::get_nickname(void) const
{
    return (_nickname);
}

const std::string Client::get_username(void) const
{
    return (_username);
}

const std::string Client::get_password(void) const
{
    return (_password);
}

bool        Client::auth(const std::string &nick, const std::string &pass) const
{
    return (nick == _nickname) && (pass == _password);
}

Client::~Client()
{
    std::cout << "Client destroyed" << std::endl;
}