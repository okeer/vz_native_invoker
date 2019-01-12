#include "DispatcherLoginHelper.h"

void DispatcherLoginHelper::initConnection()
{
	PRL_RESULT ret = PRL_ERR_UNINITIALIZED;

	ret = Login(_hServer, NULL, NULL, NULL);
	if (PRL_FAILED(ret))
	{
		throw std::invalid_argument("ERROR: failed to login to VZ dispatcher");
	}
}

PRL_HANDLE DispatcherLoginHelper::GetServerHandle()
{
	if (_hServer == PRL_INVALID_HANDLE)
		initConnection();

	return _hServer;
}
