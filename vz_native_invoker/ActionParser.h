#pragma once

#include "ActionBase.h"
#include "Parallels.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <vector>

using namespace std;
using namespace boost::program_options;

class ActionParser
{
	variables_map vm;

	std::string mode;

	boost::shared_ptr<options_description> visible_desc_ptr;
	boost::shared_ptr<options_description> work_desc_ptr;

	std::vector<ActionBase*> actions;

	int ac;
	char** av;

	void InitOptionMap();
public:
	ActionParser(int ac, char* av[]);

	void RegisterAction(ActionBase& action);
	void Parse();
};