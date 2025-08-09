/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 07:52:09 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 08:37:40 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestManager.hpp"

RequestManager::RequestManager() {}
RequestManager::~RequestManager() { _reqs.clear(); }

bool RequestManager::isReqComplete(int fd) const
{
	std::map<int, ReqState>::const_iterator it = _reqs.find(fd);
	if (it == _reqs.end())
		return false;
	std::string buf = it->second._req_buf;
	size_t end_pos = buf.find("\r\n\r\n");
	return (end_pos != std::string::npos);
}

void RequestManager::addReqData(int fd, const char *data, size_t size)
{
	if (_reqs.find(fd) == _reqs.end())
	{
		ReqState state;
		state._is_complete = false;
		state._req_buf = "";
		_reqs[fd] = state;
	}
	_reqs[fd]._req_buf.append(data, size);
}
