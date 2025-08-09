/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:14:20 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 14:20:17 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <csignal>

class Signal
{
private:
	static bool _stopReq;
	static void handle(int signum);

public:
	Signal();
	~Signal();

	static void setup();
	static bool shouldStop();
	static void reqStop();
};

#endif
