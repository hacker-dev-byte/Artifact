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

VOID get_logical_drives(_TCHAR* arr, INT* count)
{
	DWORD bits = 0;
	if (!(bits = GetLogicalDrives())) 
		ExitProcess(0);

	for (INT i = 0; i < 26; i++)
	{
		INT n = ((bits >> i) & 0x00000001);
		if (n == 1)
		{
			UINT drive_type = GetDriveType((_TCHAR[3]) { 65 + i, 58, 0 });
			if ((drive_type != DRIVE_CDROM) && (drive_type != DRIVE_REMOTE))
			{
				if (arr && count)
				{
					_TCHAR logical_drive[] = { 65 + i, 0 };

					if (_tcscat_s(arr, 27, logical_drive))
						ExitProcess(0);

					(*count)++;
				}
			}
		}
	}
}