#include "Common.h"
#include "Settings_Menu.h"
#include "System_Monitor.h"

extern "C"
{
	int module_start()
	{
		klog("!! Hello World !!\n");

		Mono::Init();

		//UI::Utilities::SetVersionString("5.05 OSM's Cool Firmware");
		System_Monitor::Init();
		Settings_Menu::Init();
		//Title_Menu::Init();	

		Notify(ORBIS_TOOLBOX_NOTIFY);

		return 0;
	}

	int module_stop()
	{
		klog("!! BYE !!\n");

		Notify("Orbis Toolbox: Unloading...");

		Settings_Menu::Term();
		System_Monitor::Term();
		//Title_Menu::Term();

		return 0;
	}

	void _start()
	{

	}
}