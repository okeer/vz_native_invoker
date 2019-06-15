#include "Parallels.h"
#include "vz_helpers.h"

#include "ActionBase.h"
#include "ActionParser.h"
#include "CloneVMAction.h"
#include "DispatcherLoginHelper.h"
#include "ResizeVMHddAction.h"
#include "RefreshConfigAction.h"

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
		RefreshConfigAction refreshAction = RefreshConfigAction(&loginHelper);

		newActionParser.RegisterAction(cloneAction);
		newActionParser.RegisterAction(resizeAction);
		newActionParser.RegisterAction(refreshAction);

		newActionParser.Parse();
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
	}

}
