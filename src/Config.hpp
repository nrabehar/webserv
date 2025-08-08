/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:14:13 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 11:38:11 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "ServerConfig.hpp"
#include "WException.hpp"
#include <fstream>
#include <map>
#include <vector>

class Config
{
private:
	Config();
	std::string config_path;
	std::string config_contents;
	std::vector<ServerConfig> servers;
	void loadFile();
	void parseConfigFile();
	void validateServer(const ServerConfig &);
	void validateLocation(const Location &);
	void validateHostPort();
	ServerConfig parseServer(std::istringstream &);
	Location parseLocation(std::istringstream &);
	void printServerConfig(const ServerConfig &) const;
	void printLocationConfig(const Location &) const;

public:
	~Config();
	Config(const Config &);
	Config(const std::string &);
	Config &operator=(const Config &);
	void validate();
	void printConfig() const;
	const std::vector<ServerConfig> &getServers() const;
};

#endif