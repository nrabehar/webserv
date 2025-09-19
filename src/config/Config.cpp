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
	std::cout << "Configuration file loaded: " << _path << std::endl;

}

void	Config::parse()
{
	std::vector<Token> tokens = ConfigLexer::tokenize(_file->getData());
	_root = ConfigParser::parse(tokens);
	ConfigValidator validator;

	validator.add(new CHttpValidator());
	
	validator.validate(_root);
  _root->print(0);
}
