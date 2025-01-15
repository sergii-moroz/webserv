/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoroz <smoroz@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:58:12 by smoroz            #+#    #+#             */
/*   Updated: 2024/11/27 18:58:12 by smoroz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <iostream>
# include <fcntl.h>

# define PORT		12345
# define BACKLOGS	10

class Webserv
{
	public:
		Webserv();
		~Webserv();
		Webserv(Webserv const &);
		Webserv &	operator=(Webserv const &);
		static void	usage(void);

	private:
		void	acceptConnection(void);
		void	listenSocket(void);
		void	initSocket(void);
		void	bindSocket(void);
		void	setup(void);
		void	init(void);

		int				_port;
		int				_backlogs;
		int				_master_sd;
		sockaddr_in6	_serveraddr;

		fd_set			_master_set;
		fd_set			_working_set;
		int				_max_sd;
		timeval			_timeout;
};

#endif
