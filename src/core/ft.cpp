#include "core/ft.hpp"

bool ft::_simulate_failure = false;

NOTHROW
void ft::failure(bool value)
{
	ft::_simulate_failure = value;
}
