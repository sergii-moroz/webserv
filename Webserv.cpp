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

Webserv::Webserv()
{
	std::cout << "Webserv: Default constructor called" << std::endl;
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
	std::cout << "Webbserv: Destructor called" << std::endl;
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
