/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:09:57 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 04:46:42 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "../tools/Checker.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

class Location
{
private:
	std::string _path;
	std::string _root;
	std::string _cgi;
	std::string _uploadPath;
	bool _autoIndex;
	bool _hasRedirect;
	size_t _maxBodySize;
	std::vector<std::string> _indexs;
	std::vector<std::string> _methods;
	std::pair<int, std::string> _redirect;

public:
	Location();
	~Location();
	Location(const Location &);
	Location &operator=(const Location &);

	const std::string &getPath() const;
	const std::string &getRoot() const;
	const std::vector<std::string> &getIndexs() const;
	const std::vector<std::string> &getMethods() const;
	bool getAutoIndex() const;
	bool hasRedirect() const;
	const std::pair<int, std::string> &getRedirect() const;
	const std::string &getCgi() const;
	const std::string &getUploadPath() const;
	size_t getMaxBodySize() const;

	void setPath(const std::string &path);
	void setRoot(const std::string &root);
	void setCgi(const std::string &cgi);
	void setAutoIndex(bool autoIndex);
	void setRedirect(int code, const std::string &uri);
	void setUploadPath(const std::string &uploadPath);
	void setMaxBodySize(size_t size);

	void setIndexs(std::vector<std::string> &index);
	void setMethods(std::vector<std::string> &method);

	bool isAllowedMethod(const std::string &method) const;
	void setProperty(const std::string &, const std::string &);
};

#endif