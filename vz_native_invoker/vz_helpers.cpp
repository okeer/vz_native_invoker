// Copyright(C) 1999 - 2016 Parallels IP Holdings GmbH

// Based on VZ doc examples 

#include "vz_helpers.h"

char * convert_to_carray(std::string str)
{
	char *templ = new char[str.length() + 1];
	strcpy(templ, str.c_str());
	return templ;
}

PRL_RESULT GetHDDDev(PRL_HANDLE &hVm, PRL_HANDLE &hVmDev)
{
	PRL_HANDLE hVmCfg = PRL_INVALID_HANDLE;

	PRL_RESULT ret = PRL_ERR_UNINITIALIZED;

	PRL_UINT32 nDevCount;
	PRL_DEVICE_TYPE devType;

	ret = PrlVm_GetConfig(hVm, &hVmCfg);
	ret = PrlVmCfg_GetHardDisksCount(hVmCfg, &nDevCount);

	for (int i = 0; i < nDevCount; ++i)
	{
		PRL_HANDLE hDevice;
		ret = PrlVmCfg_GetHardDisk(hVmCfg, i, &hDevice);

		hVmDev = hDevice;

		if (i + 1 != nDevCount)
			PrlHandle_Free(hDevice);
	}

	PrlHandle_Free(hVmCfg);

	return PRL_ERR_SUCCESS;
}

PRL_RESULT ResizeVmTo(PRL_HANDLE hServer, PRL_HANDLE &hVm, PRL_UINT32 size)
{
	PRL_HANDLE hJobBeginEdit = PRL_INVALID_HANDLE;
	PRL_HANDLE hJobCommit = PRL_INVALID_HANDLE;
	PRL_HANDLE hJobResize = PRL_INVALID_HANDLE;
	PRL_HANDLE hDevice = PRL_INVALID_HANDLE;

	PRL_RESULT nJobRetCode = PRL_INVALID_HANDLE;
	PRL_RESULT nJobReturnCode = PRL_ERR_UNINITIALIZED;

	PRL_RESULT ret = PRL_ERR_UNINITIALIZED;

	hJobBeginEdit = PrlVm_BeginEdit(hVm);
	ret = PrlJob_Wait(hJobBeginEdit, 10000);

	PrlJob_GetRetCode(hJobBeginEdit, &nJobRetCode);

	if (PRL_FAILED(nJobRetCode))
	{
		fprintf(stderr, "Error: %s\n", prl_result_to_string(nJobRetCode));
		PrlHandle_Free(hJobBeginEdit);
		return nJobRetCode;
	}

	ret = GetHDDDev(hVm, hDevice);

	hJobResize = PrlVmDev_ResizeImage(hDevice, size, PACF_NON_INTERACTIVE_MODE | PRIF_RESIZE_LAST_PARTITION);

	ret = PrlJob_Wait(hJobResize, 360000);
	if (PRL_FAILED(ret))
	{
		PrlHandle_Free(hJobResize);
		PrlHandle_Free(hDevice);
		fprintf(stderr, "Failed to resize VM with:\n%s\n", prl_result_to_string(ret));
		return ret;
	}

	ret = PrlJob_GetRetCode(hJobResize, &nJobReturnCode);
	if (PRL_FAILED(ret))
	{
		PrlHandle_Free(hJobResize);
		PrlHandle_Free(hDevice);
		fprintf(stderr, "PrlVmDev_ResizeImage failed\n");
		return ret;
	}

	PrlHandle_Free(hJobResize);
	PrlHandle_Free(hDevice);

	if (PRL_FAILED(nJobReturnCode))
	{

		fprintf(stderr, "PrlVmDev_ResizeImage job returned error:\n%s\n", prl_result_to_string(nJobReturnCode));
		return nJobReturnCode;
	}

	// Commit the changes. 
	hJobCommit = PrlVm_Commit(hVm);

	ret = PrlJob_Wait(hJobCommit, 10000);
	PrlJob_GetRetCode(hJobCommit, &nJobRetCode);
	if (PRL_FAILED(nJobRetCode))
	{
		fprintf(stderr, "Commit error: %s\n", prl_result_to_string(nJobRetCode));
		PrlHandle_Free(hJobCommit);
		return nJobRetCode;
	}

	PrlHandle_Free(hJobCommit);
	PrlHandle_Free(hJobBeginEdit);

	return PRL_ERR_SUCCESS;
}

PRL_RESULT CloneVmTo(PRL_HANDLE hServer, PRL_HANDLE &hVm, PRL_STR newVmName)
{
	PRL_HANDLE hJobCreated = PRL_INVALID_HANDLE;

	PRL_RESULT nJobReturnCode = PRL_ERR_UNINITIALIZED;
	PRL_RESULT ret = PRL_ERR_UNINITIALIZED;

	hJobCreated = PrlVm_CloneEx(hVm, newVmName, "", PRL_FALSE);

	ret = PrlJob_Wait(hJobCreated, 360000);
	if (PRL_FAILED(ret))
	{
		PrlHandle_Free(hJobCreated);
		fprintf(stderr, "Failed to clone %s VM with:\n%s", newVmName, prl_result_to_string(ret));
		return ret;
	}

	ret = PrlJob_GetRetCode(hJobCreated, &nJobReturnCode);
	if (PRL_FAILED(ret))
	{
		PrlHandle_Free(hJobCreated);
		fprintf(stderr, "PrlVm_CloneEx failed\n");
		return ret;
	}

	if (PRL_FAILED(nJobReturnCode))
	{
		fprintf(stderr, "PrlVm_CloneEx job returned error:\n%s\n", prl_result_to_string(nJobReturnCode));
		return nJobReturnCode;
	}

	return PRL_ERR_SUCCESS;
}

PRL_RESULT GetVmConfigByName(PRL_HANDLE hServer, PRL_STR sVmName, PRL_HANDLE &hVmConfig)
{
	PRL_HANDLE hJob = PRL_INVALID_HANDLE;
	PRL_RESULT nJobResult = PRL_INVALID_HANDLE;
	PRL_RESULT nJobReturnCode = PRL_INVALID_HANDLE;

	hJob = PrlSrv_GetVmConfig(hServer, sVmName, PGVC_SEARCH_BY_UUID | PGVC_SEARCH_BY_NAME);

	nJobResult = PrlJob_Wait(hJob, 360000);
	if (PRL_FAILED(nJobResult))
	{
		PrlHandle_Free(hJob);
		fprintf(stderr, "Failed to get %s VM config failed with:\n%s", sVmName, prl_result_to_string(nJobResult));
		return nJobResult;
	}

	nJobResult = PrlJob_GetRetCode(hJob, &nJobReturnCode);
	if (PRL_FAILED(nJobResult))
	{
		PrlHandle_Free(hJob);
		fprintf(stderr, "PrlVm_Comnfig failed\n");
		return nJobResult;
	}

	if (PRL_FAILED(nJobReturnCode))
	{
		PrlHandle_Free(hJob);
		fprintf(stderr, "Error:\n%s\n", prl_result_to_string(nJobReturnCode));
		return nJobReturnCode;
	}

	return PRL_ERR_SUCCESS;
}

PRL_RESULT GetVmByName(PRL_HANDLE hServer, PRL_STR sVmName, PRL_HANDLE &hVm)
{
	PRL_HANDLE hResult = PRL_INVALID_HANDLE;
	PRL_RESULT nJobResult = PRL_INVALID_HANDLE;

	PRL_HANDLE hJob = PrlSrv_GetVmList(hServer);
	PRL_RESULT ret = PrlJob_Wait(hJob, 10000);
	if (PRL_FAILED(ret))
	{

		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		return ret;
	}

	ret = PrlJob_GetRetCode(hJob, &nJobResult);
	if (PRL_FAILED(nJobResult))
	{

		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		return ret;
	}

	ret = PrlJob_GetResult(hJob, &hResult);
	if (PRL_FAILED(ret))
	{

		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		return ret;
	}
	PrlHandle_Free(hJob);

	PRL_UINT32 nParamsCount = 0;
	ret = PrlResult_GetParamsCount(hResult, &nParamsCount);
	for (PRL_UINT32 i = 0; i < nParamsCount; ++i)
	{

		PrlResult_GetParamByIndex(hResult, i, &hVm);

		char vm_name[1024];
		PRL_UINT32 nBufSize = sizeof(vm_name);
		ret = PrlVmCfg_GetName(hVm, vm_name, &nBufSize);
		if (PRL_FAILED(ret))
		{

			return PRL_ERR_FAILURE;
		}


		if (strcmp(sVmName, vm_name) == 0)
		{
			PrlHandle_Free(hResult);
			return PRL_ERR_SUCCESS;
		}

		PrlHandle_Free(hVm);
	}

	PrlHandle_Free(hResult);
	return PRL_ERR_NO_DATA;
}

// Intializes the SDK library and
// logs in to the Virtuozzo host locally.
// Obtains a handle of type PHT_SERVER identifying
// the Virtuozzo host.
PRL_RESULT Login(PRL_HANDLE &hServer, char* szServer, char* szUsername, char* szPassword)
{
	// Variables for handles.
	PRL_HANDLE hJob = PRL_INVALID_HANDLE; // job handle
	PRL_HANDLE hJobResult = PRL_INVALID_HANDLE; // job result

	// Variables for return codes.
	PRL_RESULT err = PRL_ERR_UNINITIALIZED;
	PRL_RESULT nJobReturnCode = PRL_ERR_UNINITIALIZED;

	// Use the correct dynamic library depending on the platform.
#ifdef _WIN_
#define SDK_LIB_NAME "prl_sdk.dll"
#elif defined(_LIN_)
#define SDK_LIB_NAME "libprl_sdk.so"
#endif

// Load SDK library.
	if (PRL_FAILED(SdkWrap_Load(SDK_LIB_NAME)) &&
		PRL_FAILED(SdkWrap_Load("./" SDK_LIB_NAME)))
	{
		fprintf(stderr, "Failed to load " SDK_LIB_NAME "\n");
		return -1;
	}

	// Initialize the API.
	err = PrlApi_InitEx(PARALLELS_API_VER, PAM_SERVER, 0, 0);

	if (PRL_FAILED(err))
	{
		fprintf(stderr, "PrlApi_InitEx returned with error: %s.\n",
			prl_result_to_string(err));
		PrlApi_Deinit();
		SdkWrap_Unload();
		return -1;
	}

	// Create a server handle (PHT_SERVER).
	err = PrlSrv_Create(&hServer);
	if (PRL_FAILED(err))
	{
		fprintf(stderr, "PrlSvr_Create failed, error: %s",
			prl_result_to_string(err));
		PrlApi_Deinit();
		SdkWrap_Unload();
		return -1;
	}

	// Log iVn (asynchronous call).

	if (szServer == NULL || szUsername == NULL || szPassword == NULL)
	{
		hJob = PrlSrv_LoginLocal(hServer, NULL, 0, PSL_HIGH_SECURITY);
	}
	else
	{
		hJob = PrlSrv_Login(
			hServer,            // PRL_HANDLE of type PHT_SERVER.
			szServer,           // Name and IP address of the host machine.
			szUsername,         // User name.
			szPassword,         // Password.
			0,                  // Previuos session ID (0 if not used).
			0,                  // Port number (0 for default port).
			0,                  // Timeout value (0 for default timeout).
			PSL_HIGH_SECURITY); // Security level.
	}

	// Wait for a maximum of 10 seconds for
	// the job to complete.
	err = PrlJob_Wait(hJob, 1000);

	if (PRL_FAILED(err))
	{
		fprintf(stderr,
			"PrlJob_Wait for PrlSrv_Login returned with error: %s\n",
			prl_result_to_string(err));
		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		PrlApi_Deinit();
		SdkWrap_Unload();
		return -1;
	}

	// Analyze the result of PrlSrv_Login.
	err = PrlJob_GetRetCode(hJob, &nJobReturnCode);

	// First, check PrlJob_GetRetCode success/failure.
	if (PRL_FAILED(err))
	{
		fprintf(stderr, "PrlJob_GetRetCode returned with error: %s\n",
			prl_result_to_string(err));
		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		PrlApi_Deinit();
		SdkWrap_Unload();
		return -1;
	}

	// Now check the Login operation success/failure.
	if (PRL_FAILED(nJobReturnCode))
	{
		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		printf("Login job returned with error: %s\n",
			prl_result_to_string(nJobReturnCode));
		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		PrlApi_Deinit();
		SdkWrap_Unload();
		return -1;
	}

	return 0;
}

// Log off and deinitializes the SDK library.
//
PRL_RESULT LogOff(PRL_HANDLE &hServer)
{
	PRL_HANDLE hJob = PRL_INVALID_HANDLE;
	PRL_HANDLE hJobResult = PRL_INVALID_HANDLE;

	PRL_RESULT err = PRL_ERR_UNINITIALIZED;
	PRL_RESULT nJobReturnCode = PRL_ERR_UNINITIALIZED;

	// Log off.
	hJob = PrlSrv_Logoff(hServer);
	err = PrlJob_Wait(hJob, 1000);
	if (PRL_FAILED(err))
	{
		fprintf(stderr, "PrlJob_Wait for PrlSrv_Logoff returned error: %s\n",
			prl_result_to_string(err));
		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		PrlApi_Deinit();
		SdkWrap_Unload();
		return -1;
	}

	// Get the Logoff operation return code.
	err = PrlJob_GetRetCode(hJob, &nJobReturnCode);

	// Check the PrlJob_GetRetCode success/failure.
	if (PRL_FAILED(err))
	{
		fprintf(stderr, "PrlJob_GetRetCode failed for PrlSrv_Logoff with error: %s\n",
			prl_result_to_string(err));
		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		PrlApi_Deinit();
		SdkWrap_Unload();
		return -1;
	}

	// Report success or failure of PrlSrv_Logoff.
	if (PRL_FAILED(nJobReturnCode))
	{
		fprintf(stderr, "PrlSrv_Logoff failed with error: %s\n",
			prl_result_to_string(nJobReturnCode));
		PrlHandle_Free(hJob);
		PrlHandle_Free(hServer);
		PrlApi_Deinit();
		SdkWrap_Unload();
		return -1;
	}

	// Free handles that are no longer required.
	PrlHandle_Free(hJob);
	PrlHandle_Free(hServer);

	// De-initialize the Virtuozzo API, and unload the SDK.
	PrlApi_Deinit();
	SdkWrap_Unload();

	return 0;
}
