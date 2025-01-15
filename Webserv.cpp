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

	memset(&_serveraddr, 0, sizeof(_serveraddr));
	_serveraddr.sin6_family = AF_INET6;
	_serveraddr.sin6_port = htons(_port);
	_serveraddr.sin6_addr = in6addr_any;

	FD_ZERO(&_master_set);
	_max_sd = _master_sd;
	FD_SET(_master_sd, &_master_set);

	_timeout.tv_sec = 15;
	_timeout.tv_usec = 0;

	char	str[INET6_ADDRSTRLEN];

	if (inet_ntop(AF_INET6, &_serveraddr.sin6_addr, str, sizeof(str)))
		std::cout << "INFO: Server " << str << ":" << _port << std::endl;
}

void	Webserv::initSocket(void)
{
	int	rc, on = 1;

	rc = setsockopt(_master_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
		throw std::runtime_error("ERROR: setsockopt(SO_REUSEADDR) is failed");

	rc = fcntl(_master_sd, F_SETFL, O_NONBLOCK);
	if (rc < 0)
		throw std::runtime_error("ERROR: set socket to be nonblocking is failed. fcntl(O_NONBLOCK)");
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
	// char	buffer[100];
	// memset(buffer, 0, sizeof(buffer));
	// int	size = read(sd, buffer, 100);
	// std::cout << "DEBUG: The message was recived: " << buffer << std::endl;
	// std::cout << "DEBUG: Message's size: " << size << std::endl;
	// close(sd);
}

void	Webserv::init(void)
{
	try
	{
		setup();
		initSocket();
		bindSocket();
		listenSocket();
		// acceptConnection();
	}
	catch (std::exception const & e)
	{
		std::cerr << e.what() << std::endl;
	}
}

int	Webserv::getReadySocket(void)
{
	int	rc;

	memcpy(&_working_set, &_master_set, sizeof(_master_set));
	std::cout << "Waiting on select()..." << std::endl;
	rc = select(_max_sd + 1, &_working_set, NULL, NULL, &_timeout);

	if (rc < 0)
		throw std::runtime_error("ERROR: select() failed");

	if (rc == 0)
		throw std::runtime_error("select() timed out. End program.");

	return (rc);
}

void	Webserv::run(void)
{
	int	rc;

	while (true)
	{
		rc = getReadySocket();
		std::cout << "DEBUG: " << rc << " sockets are ready to processign" << std::endl;
		if (rc > 0)
			break;
	}
}
