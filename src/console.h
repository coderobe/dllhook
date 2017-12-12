#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#pragma once

#include <iostream>
#include <Windows.h>

class Console 
{
	private:
		FILE* fPtr;
	public:
		Console(std::string defaultText = "", std::string windowTitle = "Console")
		{
			AllocConsole();
			freopen_s(&fPtr, "CONOUT$", "w", stdout);

			SetConsoleTitle(windowTitle.c_str());

			if (defaultText.empty() == false)
			{
				printf_s(defaultText.c_str());
			}
		}
		~Console()
		{
			fclose(fPtr);
			FreeConsole();
		}
};

#endif
