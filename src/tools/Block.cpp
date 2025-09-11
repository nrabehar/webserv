#include "webserv.hpp"

bool Block::_in_block = false;
int Block::_brace_count = 0;
std::string Block::_block = "";
std::string Block::_name = "";

Block::Block() {}
Block::~Block() {}

std::vector<std::string> Block::extractServer(const std::string &content)
{
	std::istringstream stream(content);
	std::string token;
	std::vector<std::string> blocks;

	_name = "server";
	while (stream >> token)
	{
		if (token.empty() || token[0] == '#') {
			skipComment(stream, token);
			continue;
		}
		if (token == "server") {
			findBlock(stream, blocks, token);
		} else {
			throw std::runtime_error("ConfigParser: unexpected token outside 'server' block: " + token);
		}
	}
	_block.clear();
	_in_block = false;
	_brace_count = 0;
	_name = "";
	return blocks;
}

std::vector<std::string> Block::extractLocation(const std::string &content)
{
	std::istringstream stream(content);
	std::string token;
	std::vector<std::string> blocks;

	_name = "location";
	while (stream >> token)
	{
		if (token.empty() || token[0] == '#') {
			skipComment(stream, token);
			continue;
		}
		if (token == "location") {
			stream >> token;
			if (token == "{")
				throw std::runtime_error("ConfigParser: 'location' block must be preceded by a path");
			_block.append(token + "\n");
			findBlock(stream, blocks, token);
		} else {
			if (!_in_block)
				std::getline(stream, token);
		}
	}

	_block.clear();
	_in_block = false;
	_brace_count = 0;
	_name = "";
	return blocks;
}

void Block::skipLocation(std::istringstream &stream)
{
	std::string token;
	std::vector<std::string> __blocks;
	stream >> token;
	findBlock(stream, __blocks, token);
}

void Block::skipComment(std::istringstream &stream, std::string &token)
{
	if (token[0] == '#') {
		std::getline(stream, token);
	}
}

void Block::findBlock(std::istringstream &stream, std::vector<std::string> &blocks, std::string &token)
{
	stream >> token;
	if (token != "{") {
		if (_name == "server")
			throw std::runtime_error("ConfigParser: '" + _name + "' block must be followed by '{'");
		else if (_name == "location")
			throw std::runtime_error("ConfigParser: '" + _name + "' block must be preceded by a path and followed by '{'");
	}

	_in_block = true;
	_brace_count = 1;
	_block.append(token);

	while (std::getline(stream, token)) {
		processLine(token);
		if (_brace_count == 0) {
			_in_block = false;
			_block = String::trim(_block);
			if (_name == "server")
			{
				_block = _block.substr(1, _block.size() - 2);
				_block = String::trim(_block);
			}
			blocks.push_back(_block);
			_block.clear();
			break;
		}
	}
	if (_brace_count < 0) {
		throw std::runtime_error("ConfigParser: unmatched '}' in '" + _name + "' block");
	}
	if (_in_block) {
		throw std::runtime_error("ConfigParser: unmatched '{' in '" + _name + "' block");
	}
}

void Block::processLine(std::string &line)
{
	if (line.empty())
		return;
	size_t pos = line.find('#');
	if (pos != std::string::npos) {
		line = line.substr(0, pos);
	}
	_block.append(line + "\n");
	for (size_t i = 0; i < line.size(); i++) {
		if (line[i] == '{') {
			_brace_count++;
		} else if (line[i] == '}') {
			_brace_count--;
		}
	}
}