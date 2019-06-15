#include "ActionParser.h"

ActionParser::ActionParser(int new_ac, char* new_av[])
{
	const std::string disclaimer =
		"\nThis tool invokes Virtuozzo native C++ API.\n\nUsage:\n\n./vz_native_invoker --mode <specific mode> [<mode specific options/arguments>]\n\nOptions";

	visible_desc_ptr = boost::shared_ptr<boost::program_options::options_description>(new boost::program_options::options_description(disclaimer));
	work_desc_ptr = boost::shared_ptr<boost::program_options::options_description>(new boost::program_options::options_description());

	ac = new_ac;
	av = new_av;
}

void ActionParser::RegisterAction(ActionBase& action)
{
	actions.push_back(&action);
	visible_desc_ptr->add(*work_desc_ptr);
	visible_desc_ptr->add(*action.actionDesc);
}

void ActionParser::InitOptionMap()
{
	std::string s = "Select mode:";
	for (std::vector<ActionBase*>::iterator it = actions.begin(); it != actions.end(); ++it)
		s += " " + (*it)->mode;

	visible_desc_ptr->add_options()
		("help,h", "Show this message")
		("mode,m", value<std::string>(&mode), s.c_str())
		;

	work_desc_ptr->add_options()
		("help,h", "Show this message")
		("mode,m", value<std::string>(&mode), s.c_str())
		;

	parsed_options parsed = command_line_parser(ac, av).options(*work_desc_ptr).allow_unregistered().run();
	store(parsed, vm);
	notify(vm);
}

void ActionParser::Parse()
{
	bool fired = false;
	InitOptionMap();

	for (std::vector<ActionBase*>::iterator it = actions.begin(); it != actions.end(); ++it)
	{
		if (mode == (*it)->mode)
		{
			fired = true;
			work_desc_ptr->add(*((*it)->actionDesc));
			store(parse_command_line(ac, av, *work_desc_ptr), vm);
			(*it)->Run(vm);
			cout << "Action " << (*it)->mode << " completed with success" << endl;
		}
	}

	if (!fired || vm.count("help"))
		cout << *visible_desc_ptr << endl;
}
