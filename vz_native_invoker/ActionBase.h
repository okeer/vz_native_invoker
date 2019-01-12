#pragma once

#include "Parallels.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <iostream>
#include <string>

#include "DispatcherLoginHelper.h"

using namespace boost::program_options;

class ActionBase
{
protected:
	DispatcherLoginHelper* _loginHelper;

	virtual void isArgumentsValid(variables_map* vm) = 0;

public:
	ActionBase(DispatcherLoginHelper* loginHelper) { _loginHelper = loginHelper; } ;

	std::string mode;
	boost::shared_ptr<options_description> actionDesc;

	virtual void Run(variables_map vm) = 0;
};