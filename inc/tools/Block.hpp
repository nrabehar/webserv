#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "../webserv.hpp"

class Block
{

	private:

		static bool _in_block;
		static int _brace_count;
		static std::string _block;
		static std::string _name;

	public:

		Block();
		~Block();
		static void skipLocation(std::istringstream &);
		static void skipComment(std::istringstream &, std::string &);
		static std::vector<std::string> extractServer(const std::string &);
		static std::vector<std::string> extractLocation(const std::string &);

	private:

		Block(const Block &);
		Block & operator=(const Block &);

		static void findBlock(std::istringstream &, std::vector<std::string> &, std::string &);
		static void processLine(std::string &);

};

#endif // BLOCK_HPP