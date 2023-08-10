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
#include "common.hpp"
#include <string>
#include <netinet/in.h>
#include <vector>

typedef struct sockaddr s_sockaddr;
typedef struct sockaddr_in s_sockaddr_in;
typedef int socket_t;

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
    std::vector<socket_t> _connections;

};
