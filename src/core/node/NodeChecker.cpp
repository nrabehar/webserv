#include "webserv.hpp"

bool DirectiveChecker::_valid = true;

DirectiveChecker::~DirectiveChecker() {}

void DirectiveChecker::argCount(const Directive & d)
{
    if (!_valid || !d.node())
        return;
    
    size_t n = d.node()->getData().size();
    int min = d.min_arg();
    int max = d.max_arg();
    
    if ((min >= 0 && n < (size_t)min) ||
        (max >= 0 && n > (size_t)max))
    {
        std::ostringstream oss;
        oss << "Directive `" << d.name() << "` expects ";
        if (min == max)
            oss << min << " argument(s)";
        else if (max >= 0)
            oss << "between " << min << " and " << max << " arguments";
        else
            oss << "at least " << min << " arguments";
        oss << ", but got " << n << " at line " << d.node()->getData()[0].line;
        throw std::runtime_error(oss.str());
        _valid = false;
    }
}

void DirectiveChecker::argType(const Directive & d)
{
    const Node<Token> * node = d.node();
    if (!_valid || !node)
        return;
    
    const std::vector<std::string> & types = d.arg_type();
    const std::vector<Token>& args = node->getData();
    
    for (size_t i = 0; i < args.size(); ++i)
    {
        std::string typeStr = TokenU::typeToString(args[i].type);
        if (!contains(types, typeStr))
        {
            std::ostringstream oss;
            oss << "Directive `" << d.name() << "` has invalid argument `"
                << args[i].value << "` at position " << i
                << ". Allowed types: ";
            for (size_t j = 0; j < types.size(); ++j)
            {
                if (j > 0) oss << ", ";
                oss << types[j];
            }
            throw std::runtime_error(oss.str());
            _valid = false;
            break;
        }
    }
}

void DirectiveChecker::acceptParent(const Directive & d)
{
    const Node<Token> * node = d.node();
    if (!_valid || !node || !node->getParent())
        return;
    
    std::string parent_name = node->getParent()->getName();
    _valid = d.isAllowedIn(parent_name);

    if (!_valid)
        ERR("Directive `" + d.name() + "` is not allowed in `" + parent_name + "`");
}

void DirectiveChecker::requireChild(const Directive & d)
{
    const Node<Token> * node = d.node();
    if (!_valid || !node)
        return;

    size_t n = node->getChild().size();
    if (d.req_child() && n == 0)
    {
        ERR("Directive `" + d.name() + "` requires at least one child block");
        _valid = false;
    }
}

bool DirectiveChecker::check(const Directive & d)
{

	_valid = true;

	argCount(d);
	argType(d);
	acceptParent(d);
	requireChild(d);

	if (_valid)
		DirectiveRegistry::save(d);

	return (_valid);

}

bool DirectiveChecker::contains(const std::vector<std::string>& vec, const std::string& val)
{
    
	for (size_t i = 0; i < vec.size(); ++i)
	{

		if (vec[i] == val) return true;
	
	}

	return (false);

}
