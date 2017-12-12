#include "main.h"
#include "console.h"

//Console instance
Console* console;

//Forward declarations
void hookHelloWorld(byte*);

/*
Entry Point, starts main thread on injection
@return TRUE
*/
BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//Initialize instance of the the console
		std::string info = "Simple DLL hooking example\n";
		console = new Console(info, std::string("dllhook"));

		//Start main thread
		DWORD dwThreadID = 0;
		CreateThread(NULL, NULL, &dwMain, NULL, NULL, &dwThreadID);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//Free console
		delete console;
	}
	return TRUE;
}

/*
Main Thread
@return [h4x success]
*/
DWORD WINAPI dwMain(LPVOID lpArg)
{
	byte* HookProcessBaseAddress = (byte*)GetModuleHandle(NULL);
	printf_s("HookProcessBaseAddress: %p\n", HookProcessBaseAddress);

	hookHelloWorld(HookProcessBaseAddress);

	return TRUE;
}

void hookHelloWorld(byte* HookProcessBaseAddress)
{
	char* hookAddress = NULL;

	const char* locate = "Hello";
	hookAddress = GetAddressByData(locate, strlen(locate));

	if (hookAddress == NULL)
	{
		MessageBoxA(0, "Failed to find hook address", 0, 0);
		return;
	}

	printf_s("hookAddress: %p\n", hookAddress);

	printf_s("Got '%s'\n", (char*)hookAddress);
}
