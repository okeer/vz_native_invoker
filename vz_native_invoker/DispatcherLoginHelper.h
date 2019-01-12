#pragma once

#include "vz_helpers.h"

#include <exception>
#include <stdexcept>

using namespace std;

class DispatcherLoginHelper
{
	PRL_HANDLE _hServer = PRL_INVALID_HANDLE;

	void initConnection();
public:
	~DispatcherLoginHelper()
	{
		if (_hServer != PRL_INVALID_HANDLE)
			LogOff(_hServer);
	}

	PRL_HANDLE GetServerHandle();
};