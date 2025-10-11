#include "webserv.hpp"

EventHandler::EventHandler(): _fd(-1) {}
EventHandler::EventHandler(int fd): _fd(fd) {}
EventHandler::~EventHandler() {}

int EventHandler::fd() const { return (_fd); }
