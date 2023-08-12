/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:12:25 by fbarros           #+#    #+#             */
/*   Updated: 2023/08/09 19:13:22 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

static const unsigned int SOCKLEN = sizeof(s_sockaddr_in);

class Server
{
public:
    Server();
    Server(int port, std::string passwd);
    ~Server();

    void setPort(int port);
    void setPassword(std::string passwd);
    void setConnection();
    void connectionLoop();

private:
    int _port;
    std::string _passwd;
    int _socket;
    struct sockaddr_in _socket_addr;
    std::vector<Client> _connections;

};
