#pragma once

#include "ActionBase.h"
#include "vz_helpers.h"

#include <boost/program_options/variables_map.hpp>

#include <iostream>
#include <exception>

using namespace std;

class CloneVMAction : public ActionBase
{
	PRL_STR _new_name;
	PRL_STR _template_name;

protected:
	virtual void isArgumentsValid(variables_map* vm) override;
public:
	CloneVMAction(DispatcherLoginHelper* loginHelper);

	virtual void Run(variables_map vm) override;
};