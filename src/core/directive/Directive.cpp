#include  "webserv.hpp"


Directive::Directive()
	: _path(DEFAULT_DIRECTIVE_PATH), _file(NULL), _data(NULL), _loaded(false) {}

Directive::Directive(const std::string & path)
	: _path(path), _file(NULL), _data(NULL), _loaded(false) {}

Directive::~Directive()
{
	if (_file)
		delete _file;
	if (_data)
		delete _data;
}

const Node<Token> * Directive::get()
{
	if (!_loaded)
		load();
	return (_data);
}
void Directive::load()
{

	FileHandler	handler;
	FilePathBuilder path_builder(".", "");
	FileExistenceChecker f_exist;
	FilePermissionChecker p_check(R_OK);
	FileOpener f_open;

	handler.setNext(&path_builder)
				 ->setNext(&f_exist)
				 ->setNext(&p_check)
				 ->setNext(&f_open);
	
	_file = handler.handle(_path);

	if (!_file)
		throw std::runtime_error(handler.getReason());

	while (!_file->isComplete())
		_file->read();

	TokenU::setSymRef("#;{}");

	std::vector<Token> tokens = Lexer::tokenize(_file->getData());
	TokenStream ss(tokens);
	DirectiveParser parser(ss);

	_data = parser.parse();

	LOG("Directive rules loaded");

}
