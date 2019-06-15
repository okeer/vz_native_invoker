#include "RefreshConfigAction.h"

RefreshConfigAction::RefreshConfigAction(DispatcherLoginHelper* loginHelper) : ActionBase(loginHelper)
{
	actionDesc = boost::shared_ptr<boost::program_options::options_description>(
		new boost::program_options::options_description("Refresh VM options (PrlVm_RefreshConfig)"));
	actionDesc->add_options()
		("name,n", value<string>(), "a name for the VM to refresh config")
		;
	mode = "refresh";
}

void RefreshConfigAction::isArgumentsValid(variables_map * vm)
{
	if (vm->count("name"))
		_name = convert_to_carray((*vm)["name"].as<string>());

	if (_name == NULL)
		throw std::invalid_argument("ERROR: Name is not specified");
}

void RefreshConfigAction::Run(variables_map vm)
{
	PRL_HANDLE hServer = PRL_INVALID_HANDLE;
	PRL_HANDLE hVm = PRL_INVALID_HANDLE;
	PRL_HANDLE hJobRefresh = PRL_INVALID_HANDLE;
	PRL_RESULT ret = PRL_ERR_UNINITIALIZED;
        PRL_RESULT nJobRetCode = PRL_INVALID_HANDLE;
        PRL_RESULT nJobReturnCode = PRL_ERR_UNINITIALIZED;

	isArgumentsValid(&vm);

	hServer = _loginHelper->GetServerHandle();

	ret = GetVmByName(hServer, _name, hVm);
	if (PRL_FAILED(ret))
	{
		PrlHandle_Free(hVm);
		throw std::invalid_argument("ERROR: Failed to get VM via API");
	}

	hJobRefresh = PrlVm_RefreshConfig(hVm);

	ret = PrlJob_Wait(hJobRefresh, 360000);
        if (PRL_FAILED(ret))
        {
		 PrlHandle_Free(hVm);
                PrlHandle_Free(hJobRefresh);
		throw std::invalid_argument("ERROR: Failed to refresh VM config via API");
        }

	ret = PrlJob_GetRetCode(hJobRefresh, &nJobReturnCode);
        
	if (PRL_FAILED(ret))
        {
		 PrlHandle_Free(hVm);
		throw std::invalid_argument("ERROR: Failed to refresh VM config via API");
        }

        PrlHandle_Free(hJobRefresh);

        if (PRL_FAILED(nJobReturnCode))
        {
		 PrlHandle_Free(hVm);
		throw std::invalid_argument("ERROR: PrlVmDev_ResizeImage job returned error");
        }


	PrlHandle_Free(hVm);
}
