#pragma once

#include "ActionBase.h"
#include "vz_helpers.h"

#include <exception>

#include <boost/program_options/variables_map.hpp>

using namespace std;

class RefreshConfigAction : public ActionBase
{
	PRL_STR _name;

protected:
	virtual void isArgumentsValid(variables_map* vm);

public:
	RefreshConfigAction(DispatcherLoginHelper* loginHelper);

	virtual void Run(variables_map vm);
};
