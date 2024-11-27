/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoroz <smoroz@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:56:38 by smoroz            #+#    #+#             */
/*   Updated: 2024/11/27 18:56:38 by smoroz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int argc, char **argv)
{
	if (argc == 1)
	{
		std::cout
			<< " Configuration file isn't provided." << std::endl
			<< " Loading default settings ..." << std::endl;
	}
	else if (argc == 2)
	{
		std::cout
			<< " Configuration file is provided." << std::endl
			<< " TODO: Read/Check config-file" << std::endl
			<< " TODO: Start server with specified configuration" << std::endl;
	}
	else
	{
		std::cout << argv[1] << std::endl;
		std::cout
			<< " To many parameters are specified. Need (1) "
			<< "got (" << argc - 1 << ")" << std::endl;
		Webserv::usage();
	}

	return (0);
}
