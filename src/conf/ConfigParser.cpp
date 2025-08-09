/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:42:08 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 14:45:02 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

void Config::parseConfigFile()
{
	std::istringstream stream(config_contents);
	std::string token;
	while (stream >> token)
	{
		if (token == "server")
			servers.push_back(parseServer(stream));
		else if (token.empty() || token[0] == '#')
		{
			std::string line;
			std::getline(stream, line);
		}
		else
			throw ParseException("Unexcepted token: " + token);
	}
}

Server Config::parseServer(std::istringstream &stream)
{
	Server server;
	std::string token;

	if (!(stream >> token) || token != "{")
	{
		stream.clear();
		throw ParseException("Expected '{' after server");
	}
	while (stream >> token && token != "}")
	{
		if (token == "location")
			server.addLocation(parseLocation(stream));
		else if (token.empty() || token[0] == '#')
		{
			std::string line;
			std::getline(stream, line);
		}
		else
		{
			std::string value;
			std::getline(stream, value);
			value = trimStr(value);
			if (!value.empty() && value[value.length() - 1] == ';')
				value = trimStr(value.substr(0, value.length() - 1));
			server.setProperty(token, value);
		}
	}
	server.fixLocationData();
	return (server);
}

Location Config::parseLocation(std::istringstream &stream)
{
	Location location;
	std::string path;
	std::string token;

	if (!(stream >> path))
		throw ParseException("Excpected path after location");
	location.setPath(path);
	if (!(stream >> token) || token != "{")
	{
		if (path == "=")
			location.setPath(token);
		else
			throw ParseException("Expected '{' after location path: " + token);
	}
	int brace_count = 1;
	while (brace_count > 0 && stream >> token)
	{
		std::string value;

		if (token == "{")
		{
			brace_count++;
			continue;
		}
		else if (token == "}")
		{
			brace_count--;
			if (!brace_count)
				break;
			continue;
		}
		std::getline(stream, value);
		value = trimStr(value);
		if (token.empty() || token[0] == '#')
			continue;
		if (value[value.length() - 1] == ';')
			value = trimStr(value.substr(0, value.length() - 1));
		location.setProperty(token, value);
		value.clear();
	}
	return (location);
}