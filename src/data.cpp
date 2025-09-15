#include "webserv.hpp"

std::ostream & operator<<(std::ostream & o, const IOutput & obj) {
	obj.write(o);
	return (o);
}

std::istream & operator>>(std::istream & i, IInput & obj) {
	obj.read(i);
	return (i);
}
