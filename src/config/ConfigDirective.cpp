#include "webserv.hpp"

void	DirectiveSpec::clean()
{
	name = "";
	is_block = false;
	allowed_in.clear();
	min_arg = 0;
	max_arg = 0;
	arg_type.clear();
}

ConfigDirectiveRegistry::~ConfigDirectiveRegistry() {}

ConfigDirectiveRegistry::ConfigDirectiveRegistry()
{
	DirectiveSpec spec;
	// http
	spec.name = "base";
	spec.is_block = true;
	spec.allowed_in.push_back("base");
	spec.min_arg = 0;
	spec.max_arg = 0;
	_specs.push_back(spec);
	spec.clean();
	// http
	spec.name = "http";
	spec.is_block = true;
	spec.allowed_in.push_back("base");
	spec.min_arg = 0;
	spec.max_arg = 0;
	_specs.push_back(spec);
	spec.clean();

	// server
	spec.name = "server";
	spec.is_block = true;
	spec.allowed_in.push_back("http");
	spec.allowed_in.push_back("base");
	spec.min_arg = 0;
	spec.max_arg = 0;
	_specs.push_back(spec);
	spec.clean();

	// location
	spec.name = "location";
	spec.is_block = true;
	spec.allowed_in.push_back("server");
	spec.min_arg = 1;
	spec.max_arg = 1;
	spec.arg_type.push_back(STRING);
	_specs.push_back(spec);
	spec.clean();

	// listen
	spec.name = "listen";
	spec.is_block = false;
	spec.allowed_in.push_back("server");
	spec.min_arg = 1;
	spec.max_arg = 1;
	spec.arg_type.push_back(NUMBER);
	_specs.push_back(spec);
	spec.clean();

	// root
	spec.name = "root";
	spec.is_block = false;
	spec.allowed_in.push_back("http");
	spec.allowed_in.push_back("server");
	spec.allowed_in.push_back("location");
	spec.min_arg = 1;
	spec.max_arg = 1;
	spec.arg_type.push_back(STRING);
	_specs.push_back(spec);
	spec.clean();

	// index
	spec.name = "index";
	spec.is_block = false;
	spec.allowed_in.push_back("server");
	spec.allowed_in.push_back("location");
	spec.min_arg = 1;
	spec.max_arg = -1; // -1 = infini
	spec.arg_type.push_back(ARRAY);
	_specs.push_back(spec);
	spec.clean();

	// autoindex
	spec.name = "autoindex";
	spec.is_block = false;
	spec.allowed_in.push_back("server");
	spec.allowed_in.push_back("location");
	spec.min_arg = 1;
	spec.max_arg = 1;
	spec.arg_type.push_back(BOOL);
	_specs.push_back(spec);
	spec.clean();
}

const DirectiveSpec*	ConfigDirectiveRegistry::get(const std::string &name) const
{
	for (size_t i = 0; i < _specs.size(); ++i)
	{
		if (_specs[i].name == name)
			return (&_specs[i]);
	}
	return (NULL);
}
