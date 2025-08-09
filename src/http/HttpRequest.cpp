/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:30:49 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 08:05:19 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {}
HttpRequest::~HttpRequest() {}

const std::string &HttpRequest::getMethod() const { return _method; }
const std::string &HttpRequest::getUri() const { return _uri; }
const std::string &HttpRequest::getVersion() const { return _version; }
const std::string &HttpRequest::getBody() const { return _body; }
const std::string &HttpRequest::getHeader(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find(key);
	if (it != _headers.end())
		return it->second;
	return key;
}
