#include "Parallels.h"
#include "vz_helpers.h"

#include "ActionBase.h"
#include "ActionParser.h"
#include "CloneVMAction.h"
#include "DispatcherLoginHelper.h"
#include "ResizeVMHddAction.h"

using namespace std;
using namespace boost;
using namespace boost::program_options;

int main(int ac, char* av[])
{
	PRL_HANDLE hServer = PRL_INVALID_HANDLE;

	try
	{
		DispatcherLoginHelper loginHelper = DispatcherLoginHelper();
		ActionParser newActionParser = ActionParser(ac, av);

		CloneVMAction cloneAction = CloneVMAction(&loginHelper);
		ResizeVMHddAction resizeAction = ResizeVMHddAction(&loginHelper);

		newActionParser.RegisterAction(cloneAction);
		newActionParser.RegisterAction(resizeAction);

		newActionParser.Parse();
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
	}

}