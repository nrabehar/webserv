#include  "webserv.hpp"

Directive::Directive(IFile * file): _file(NULL),  _data(NULL) {}
Directive::~Directive() {}

const Node<std::string> * Directive::get() const { return (_data); }
void Directive::load() {}

