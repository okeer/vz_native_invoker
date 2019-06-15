#pragma once

#include "vz_helpers.h"

#include <exception>
#include <stdexcept>

using namespace std;

class DispatcherLoginHelper
{
	PRL_HANDLE _hServer;

	void initConnection();
public:
        DispatcherLoginHelper()
	{
        _hServer = PRL_INVALID_HANDLE;
	}

	~DispatcherLoginHelper()
	{
		if (_hServer != PRL_INVALID_HANDLE)
			LogOff(_hServer);
	}

	PRL_HANDLE GetServerHandle();
};
