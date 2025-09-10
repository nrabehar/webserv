#include "webserv.hpp"

ConfigParser::ConfigParser(const AFile & file):
	_file(file),
	_server(),
	_server_block() {
		try
		{
			parseServerBlock();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}

}

ConfigParser::~ConfigParser() {
}

void ConfigParser::parse(const std::string &) {
}

EErrorCode ConfigParser::check() const {
	return (ST_OK);
}

void ConfigParser::reportError(EErrorCode) {
}

void ConfigParser::parseServerBlock() {

	std::string content = _file.getContent();
	std::istringstream stream(content);
	std::string token;
	int 	 brace_count = 0;
	bool   in_server_block = false;
	std::string block;

	while (stream >> token)
	{
		if (token.empty())
			continue;
		if (token[0] == '#') {
			std::getline(stream, token);
			continue;
		}
		if (!in_server_block && token == "server") {
			stream >> token;
			if (token == "{") {
				in_server_block = true;
				brace_count = 1;
				block.append("server {\n");
			}
			else
				throw std::runtime_error("ConfigParser: 'server' block must be followed by '{'");
		}
		if (in_server_block) {
			while (std::getline(stream, token))
			{
				size_t pos = token.find('#');
				if (pos != std::string::npos)
					token = token.substr(0, pos);
				block.append(token + "\n");
				for (size_t i = 0; i < token.size(); i++)
				{
					if (token[i] == '{')
						brace_count++;
					else if (token[i] == '}')
						brace_count--;
				}
				if (brace_count == 0)
					break;
			}
			if (brace_count == 0)
			{
				in_server_block = false;
				_server_block.push_back(block);
				block.clear();
			}
			if (brace_count < 0)
				throw std::runtime_error("ConfigParser: unmatched '}' in 'server' block");
		}
		else
			throw std::runtime_error("ConfigParser: unexpected token outside 'server' block: " + token);
	}
	if (in_server_block)
		throw std::runtime_error("ConfigParser: unmatched '{' in 'server' block");

}

const std::vector<Server> & ConfigParser::getServer() const { return (_server); }
const std::vector<std::string> & ConfigParser::getServerBlock() const { return (_server_block); }