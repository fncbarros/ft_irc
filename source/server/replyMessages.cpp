#include "../includes/Server.hpp"

int Server::replyPassMissMatch(Client& client)
{
    std::cout << "Incorrect pass" << std::endl;
    Utils::writeTo(":IRC42 " + PASSMISMATCH + " * :Password incorrect", client.getId());
    return false;
}
