/*
MIT License

Copyright (c) 2024 kadzicuh

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

#include "RC4.h"

VOID rc4_dec(LPBYTE data, DWORD data_len, LPBYTE key, INT key_len)
{
	HCRYPTPROV hProv;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0) ||
		CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET) ||
		CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		HCRYPTHASH hHash;
		if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		{
			if (CryptHashData(hHash, key, key_len, 0))
			{
				HCRYPTKEY hKey;
				if (CryptDeriveKey(hProv, CALG_RC4, hHash, 0, &hKey))
				{
					CryptDecrypt(hKey, (HCRYPTHASH)NULL, TRUE, 0, data, &data_len);

					CryptDestroyKey(hKey);
				}
			}
			CryptDestroyHash(hHash);
		}
		CryptReleaseContext(hProv, 0);
	}
}

VOID rc4_enc(LPBYTE data, DWORD data_len, LPBYTE key, INT key_len)
{
	HCRYPTPROV hProv;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0) ||
		CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET) ||
		CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		HCRYPTHASH hHash;
		if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		{
			if (CryptHashData(hHash, key, key_len, 0))
			{
				HCRYPTKEY hKey;
				if (CryptDeriveKey(hProv, CALG_RC4, hHash, 0, &hKey))
				{
					CryptEncrypt(hKey, (HCRYPTHASH)NULL, TRUE, 0, data, &data_len, data_len);

					CryptDestroyKey(hKey);
				}
			}
			CryptDestroyHash(hHash);
		}
		CryptReleaseContext(hProv, 0);
	}
}