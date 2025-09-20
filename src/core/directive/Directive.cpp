#include  "webserv.hpp"

Directive::Directive(IFile * file): _file(file),  _data(NULL), _loaded(false) {}
Directive::~Directive() {}

const Node<Token> * Directive::get()
{
	if (!_loaded)
		load();
	return (_data);
}
void Directive::load()
{
	const std::string & file_content = _file->getData();
	std::vector<Token> tokens = Lexer::tokenize(file_content);
	TokenStream ss(tokens);
	DirectiveParser parser(ss);
	_data = parser.parse();
	_loaded = true;
}
