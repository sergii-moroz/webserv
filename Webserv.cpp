/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoroz <smoroz@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 19:00:26 by smoroz            #+#    #+#             */
/*   Updated: 2024/11/27 19:00:26 by smoroz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

// ==========================================
// Constructors
// ==========================================

Webserv::Webserv() : _port(PORT), _backlogs(BACKLOGS)
{
	std::cout << "Webserv: Default constructor called" << std::endl;
	init();
}

Webserv::Webserv(Webserv const & copy)
{
	std::cout << "Webserv: Copy constructor called" << std::endl;
	*this = copy;
}
// ==========================================
// Destructor
// ==========================================

Webserv::~Webserv()
{
	std::cout << "Webserv: Destructor called" << std::endl;
	close(_master_sd);
}

// ==========================================
// Assignment operator(=)
// ==========================================

Webserv &	Webserv::operator=(Webserv const & rhs)
{
	std::cout << "Webserv: Assignment operator called" << std::endl;
	if (this != &rhs)
	{
		// here copy
	}
	return (*this);
}

// ==========================================
// Methods
// ==========================================

void	Webserv::usage(void)
{
	std::cout << " example: ./webserv [configFile]" << std::endl;
}

void	Webserv::setup(void)
{
	std::cout << "INFO: creating a socket..." << std::endl;
	_master_sd = socket(AF_INET6, SOCK_STREAM, 0);
	if (_master_sd < 0)
		throw std::runtime_error("ERROR: Failed to create socket");

	std::cout	<< "socket id: " << _master_sd
				<< ", _port: " << _port
				<< ", backlogs: " << _backlogs << std::endl;

	memset(&_serveraddr, 0, sizeof(_serveraddr));
	_serveraddr.sin6_family = AF_INET6;
	_serveraddr.sin6_port = htons(_port);
	_serveraddr.sin6_addr = in6addr_any;
	// _serveraddr.sin_addr.s_addr = INADDR_ANY;
}

void	Webserv::initSocket(void)
{
	int	rc, on = 1;

	rc = setsockopt(_master_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
		throw std::runtime_error("ERROR: setsockopt(SO_REUSEADDR) is failed");
}

void	Webserv::bindSocket(void)
{
	int	rc;

	rc = bind(_master_sd, (struct sockaddr *)&_serveraddr, sizeof(_serveraddr));
	if (rc < 0)
	{
		close(_master_sd);
		throw std::runtime_error("ERROR: bind() is failed");
	}
}

void	Webserv::listenSocket(void)
{
	if (listen(_master_sd, _backlogs) < 0)
		throw std::runtime_error("ERROR: Failed to listen on socket");

	std::cout << "INFO: Ready for client connections ..." << std::endl;
}

void	Webserv::acceptConnection(void)
{
	int	sd = accept(_master_sd, NULL, NULL);
	if (sd < 0)
		throw std::runtime_error("ERROR: accept() is failed");
	else
	{
		struct sockaddr_in6	clientaddr;
		socklen_t	addrlen=sizeof(clientaddr);
		char str[INET6_ADDRSTRLEN];

		getpeername(sd, (struct sockaddr *)&clientaddr, &addrlen);
		if (inet_ntop(AF_INET6, &clientaddr.sin6_addr, str, sizeof(str)))
			std::cout << "INFO: Client: " << str << ":" << ntohs(clientaddr.sin6_port) << std::endl;
	}
	char	buffer[100];
	memset(buffer, 0, sizeof(buffer));
	int	size = read(sd, buffer, 100);
	std::cout << "DEBUG: The message was recived: " << buffer << std::endl;
	std::cout << "DEBUG: Message's size: " << size << std::endl;
	close(sd);
}

void	Webserv::init(void)
{
	try
	{
		setup();
		initSocket();
		bindSocket();
		listenSocket();
		acceptConnection();
	}
	catch (std::exception const & e)
	{
		std::cerr << e.what() << std::endl;
	}
}
