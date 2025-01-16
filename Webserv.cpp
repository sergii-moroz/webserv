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
	std::cout << "INFO: Listening socket is readable" << std::endl;
	int	sd = accept(_master_sd, NULL, NULL);
	if (sd < 0)
		throw std::runtime_error("ERROR: accept() is failed");
	else
	{
		FD_SET(sd, &_master_set);
		if (sd > _max_sd)
			_max_sd = sd; // TODO: replace all of this with function updateMasterSet()

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

void	Webserv::receiveData(int sd)
{
	int	rc;
	char	buffer[250];

	std::cout << "INFO: Existing descriptor " << sd << " is readable" << std::endl;
	rc = recv(sd, buffer, sizeof(buffer), 0);
	if (rc < 0)
		throw std::runtime_error("ERROR: recv() failed");
	else if (rc == 0)
	{
		std::cout << "INFO: Connection was closed by client. Socket " << sd << std::endl;
		// TODO: replace with cleanup()
		close(sd);
		FD_CLR(sd, &_master_set);
		if (sd == _max_sd)
		{
			while (FD_ISSET(_max_sd, &_master_set) == false)
				_max_sd--;
		}
	}
	else
	{
		std::cout << rc << " bytes received" << std::endl;
		send(sd, "server: ", 8, 0);
		rc = send(sd, buffer, rc, 0);
		if (rc < 0)
			throw std::runtime_error("ERROR: send() failed");
	}
}


/*
| Runs the web server, continuously processing incoming connections and data.
|
| This function enters an infinite loop where it constantly checks for ready sockets,
| accepts new connections on the master socket, and receives data from all available
| sockets in the working set.
| It reads only `n` ready sockets and decrement the socket readiness count
| until there are no more ready sockets.
*/
void	Webserv::run(void)
{
	int	n;

	while (true)
	{
		try
		{
			n = getReadySocket();
			std::cout << "DEBUG: " << n << " sockets are ready to processign" << std::endl;

			for (int sd=0; sd <= _max_sd && n > 0; ++sd)
			{
				if (FD_ISSET(sd, &_working_set))
				{
					n--;
					if (sd == _master_sd)
						acceptConnection();
					else
						receiveData(sd);
				}
			}
		}
		catch (std::exception const & e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}
