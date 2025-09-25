#include "webserv.hpp"

Config::Config(const std::string &path)
	: _file(NULL), _path(path), _root(NULL) {}
Config::~Config()
{
	if (_file)
		delete _file;
	if (_root)
		delete _root;
}

const std::vector<ServerConfig> &	Config::servers() const { return (_servers); }

void	Config::load()
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
	_file->read();
	parse();
	std::cout << "Configuration file loaded: " << _path << std::endl;

}

void	Config::parse()
{

	TokenU::setSymRef("#;{}");

	std::string content = Sanitizer::sanitize(_file->getData());
	std::vector<Token> tokens = Lexer::tokenize(content);
	TokenStream ss(tokens);
	DirectiveParser parser(ss);
	_root = parser.parse();
	_root->print();
	Validator::validate(_root);

	std::vector<Node<Token>* > & childs = _root->getChild();

	for (size_t i = 0; i < childs.size(); ++i)
		Merger::merge(_root, childs[i]);

	_servers = Transformer::transform(_root);

}
