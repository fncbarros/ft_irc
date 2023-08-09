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
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>

static const short ADDR_FAM = AF_INET;
static const unsigned int SOCKLEN = sizeof(struct sockaddr_in);

typedef struct sockaddr_in s_sockaddr;
typedef int socket_t;

class Server
{
public:
    Server();
    Server(short port, std::string passwd);
    ~Server();

    void setPort(int port);
    void setPassword(std::string passwd);
    void setConnection();
    void connectionLoop();

private:
    int _port;
    std::string _passwd;
    int _socket;
    s_sockaddr _socket_addr;
    std::vector<socket_t> _connections;

};
