#include "webserv.hpp"

NodeChecker::~NodeChecker() {}

NodeChecker::Checker NodeChecker::check(const Node<Token>* node)
{
  return Checker(node);
}

NodeChecker::Checker::~Checker() {}

NodeChecker::Checker::Checker(const Node<Token>* n)
	: _node(n), _valid(true) {}

NodeChecker::Checker::Checker(const Checker &other)
	: _node(other._node), _valid(other._valid) {}

NodeChecker::Checker& NodeChecker::Checker::argCount(int min, int max)
{
  
	if (_valid)
	{

    size_t n = _node->getData().size();
    
		if ((min >= 0 && n < (size_t)min) || (max >= 0 && n > (size_t)max))
      _valid = false;
  
	}

	return (*this);

}

NodeChecker::Checker& NodeChecker::Checker::argType(const std::string& types)
{
 
	if (_valid && !types.empty())
	{
    
		std::vector<std::string> allowed = split(types, ' ');
    const std::vector<Token>& args = _node->getData();
    
		for (size_t i = 0; i < args.size(); ++i)
			
			if (!contains(allowed, TokenU::typeToString(args[i].type)))
			{
	
				_valid = false;
				break;

			}

    }

    return (*this);

}

NodeChecker::Checker& NodeChecker::Checker::acceptParent(const std::string& parents)
{
    
	if (_valid && !parents.empty() && _node->getParent())
	{
    
		std::vector<std::string> allowed = split(parents, ' ');
    
		if (!contains(allowed, _node->getParent()->getName()))
      _valid = false;
  }
  
	return (*this);

}

NodeChecker::Checker& NodeChecker::Checker::requireChild()
{

	if (_valid && _node->getChild().empty())
    _valid = false;
  
	return (*this);

}

bool NodeChecker::Checker::isValid() const { return _valid; }

std::vector<std::string> NodeChecker::Checker::split(const std::string& s, char delim)
{
    
	std::vector<std::string> elems;
    
	std::string::size_type start = 0, end = 0;
    
	while ((end = s.find(delim, start)) != std::string::npos)
	{
        
		if (end > start) elems.push_back(s.substr(start, end - start));
      start = end + 1;

	}
    
	if (start < s.size())
		elems.push_back(s.substr(start));
    
	return (elems);

}

bool NodeChecker::Checker::contains(const std::vector<std::string>& vec, const std::string& val)
{
    
	for (size_t i = 0; i < vec.size(); ++i)
	{

		if (vec[i] == val) return true;
	
	}

	return (false);

}
