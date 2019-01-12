#include "CloneVMAction.h"

CloneVMAction::CloneVMAction(DispatcherLoginHelper* loginHelper) : ActionBase(loginHelper)
{
	actionDesc = boost::shared_ptr<boost::program_options::options_description>(
		new boost::program_options::options_description("Clone VM mode options (PrlVm_CloneEx)"));
	actionDesc->add_options()
		("name,n", value<string>(), "a name for the new VM")
		("template,t", value<string>(), "a template from which VM will be cloned")
		;
	mode = "clone";
}

void CloneVMAction::isArgumentsValid(variables_map* vm)
{
	if (vm->count("template"))
		_template_name = convert_to_carray((*vm)["template"].as<std::string>());

	if (vm->count("name"))
		_new_name = convert_to_carray((*vm)["name"].as<string>());

	if (_new_name == NULL || _template_name == NULL)
		throw std::invalid_argument("ERROR: Name and/or template are not specified");
}


void CloneVMAction::Run(variables_map vm)
{
	PRL_HANDLE hServer = PRL_INVALID_HANDLE;
	PRL_HANDLE hVm = PRL_INVALID_HANDLE;
	PRL_RESULT ret = PRL_ERR_UNINITIALIZED;

	isArgumentsValid(&vm);

	hServer = _loginHelper->GetServerHandle();

	ret = GetVmByName(hServer, _template_name, hVm);
	if (PRL_FAILED(ret))
	{
		PrlHandle_Free(hVm);
		throw std::invalid_argument(prl_result_to_string(ret));
	}

	ret = CloneVmTo(hServer, hVm, _new_name);
	if (PRL_FAILED(ret))
	{
		PrlHandle_Free(hVm);
		throw std::invalid_argument(prl_result_to_string(ret));
	}
}