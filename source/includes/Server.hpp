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
    // Special functions
    public:
        Server(int port, std::string passwd);
        ~Server();

    // Public functions
    public:
        void setConnection();
        void connectionLoop(void);

    // Internal functions
    private:
        Server(); // no public default constructor
        void read_message(void) const;


    // Data
    private:
        int _port;
        int _socket;
        int _socket_process;
        std::string _passwd;
        struct sockaddr_in _socket_addr;
        std::vector<Client> _connections;

};
