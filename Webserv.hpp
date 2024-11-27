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

# include <iostream>

class Webserv
{
	public:
		Webserv();
		~Webserv();
		Webserv(Webserv const &);
		Webserv &	operator=(Webserv const &);
		static void	usage(void);
	private:

};

#endif
