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

class Server
{
    // Special functions
    public:
        Server(int port, std::string passwd);
        ~Server();

    // Public functions
    public:
        void connectionLoop(void);

    // Internal functions
    private:
        Server(); // no public default constructor
        void setConnection();
        int acceptNewConnection();
        std::string readMessage(int fd) const;
        void parse();
        /**
         *  Check in the connections vector the 
         * client that have the same socket_id as fd parameter and read the fd message
        */
        void inspectEvent(int fd); 

    // Data
    private:
        int _port;
        int _server_socket;
        std::string _passwd;
        struct sockaddr_in _socket_addr;
        std::vector<Client> _connections;
        fd_set  _connections_set;

};
