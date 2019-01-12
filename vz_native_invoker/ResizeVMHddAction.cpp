#include "ResizeVMHddAction.h"

ResizeVMHddAction::ResizeVMHddAction(DispatcherLoginHelper* loginHelper) : ActionBase(loginHelper)
{
	actionDesc = boost::shared_ptr<boost::program_options::options_description>(
		new boost::program_options::options_description("Resize VM HDD options (PrlVmDev_ResizeImage)"));
	actionDesc->add_options()
		("name,n", value<string>(), "a name for the VM to resize")
		("size,s", value<int>(), "a new size for HDD")
		;
	mode = "resize";
}

void ResizeVMHddAction::isArgumentsValid(variables_map * vm)
{
	if (vm->count("size"))
		_size = (*vm)["size"].as<int>();

	if (vm->count("name"))
		_name = convert_to_carray((*vm)["name"].as<string>());

	if (_name == NULL || _size == 0)
		throw std::invalid_argument("ERROR: Name and/or size are not specified");
}

void ResizeVMHddAction::Run(variables_map vm)
{
	PRL_HANDLE hServer = PRL_INVALID_HANDLE;
	PRL_HANDLE hVm = PRL_INVALID_HANDLE;
	PRL_RESULT ret = PRL_ERR_UNINITIALIZED;

	isArgumentsValid(&vm);

	hServer = _loginHelper->GetServerHandle();

	ret = GetVmByName(hServer, _name, hVm);
	if (PRL_FAILED(ret))
	{
		PrlHandle_Free(hVm);
		throw std::invalid_argument("ERROR: Failed to get VM via API");
	}

	ret = ResizeVmTo(hServer, hVm, _size);
	if (PRL_FAILED(ret))
	{
		PrlHandle_Free(hVm);
		throw std::invalid_argument("ERROR: Failed to Resize via API");
	}

	PrlHandle_Free(hVm);

}
