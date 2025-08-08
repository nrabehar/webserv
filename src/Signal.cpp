/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:17:41 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 14:22:16 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Signal.hpp"

bool Signal::_stopReq = false;

Signal::Signal() {}
Signal::~Signal() {}

void Signal::handle(int signum)
{
	(void)signum;
	_stopReq = true;
}

void Signal::setup()
{
	signal(SIGINT, handle);
	signal(SIGTERM, handle);
	signal(SIGQUIT, handle);
}

bool Signal::shouldStop() { return _stopReq; }
void Signal::reqStop() { _stopReq = true; }
