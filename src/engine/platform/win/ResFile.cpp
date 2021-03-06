/**
\author		Korotkov Andrey aka DRON
\date		22.01.2010 (c)Korotkov Andrey

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#include "ResFile.h"

CResFile::CResFile(uint	uiInstIdx, WORD resId):
CInstancedObj(uiInstIdx),
_hRes(NULL),
_pData(NULL),
_ui32Pos(0),
_ui32Size(0)
{	
	strcpy(_acName,IntToStr(resId).c_str());
	HRSRC res = FindResource(hModule, MAKEINTRESOURCE(resId), "BIN");
	
	if (res)
		_hRes = LoadResource(hModule,res);
	
	_bOpened = _hRes!=NULL;
	
	if (_bOpened)
	{
		_pData	= LockResource(_hRes);
		_ui32Size = SizeofResource(hModule, res);
	}
}

CResFile::~CResFile()
{
	if (_bOpened)
	{
		UnlockResource(_hRes);
		FreeResource(_hRes);
	}
}

HRESULT CResFile::Read(void *pBuffer, uint uiCount, uint &uiRead)
{
	memcpy(pBuffer, (uint8 *)_pData + _ui32Pos, uiCount);

	_ui32Pos += uiCount;

	uiRead = uiCount;

	return S_OK;
}

HRESULT CResFile::Write(const void *pBuffer, uint uiCount, uint &uiWritten)
{
	LOG("Writing to Windows module resources is not supported.", LT_WARNING);
	return E_FAIL;
}

HRESULT CResFile::Seek(uint32 ui32Offset, E_FILE_SYSTEM_SEEK_FLAG eWay, uint32 &ui32Position)
{
	switch(eWay)
	{
	case FSSF_BEGIN:
		_ui32Pos = ui32Offset;
		break;
	case FSSF_END:
		_ui32Pos = _ui32Size - ui32Offset;
		break;
	case FSSF_CURRENT:
		_ui32Pos += ui32Offset;
		break;
	}
	
	ui32Position = _ui32Pos;

	return S_OK;
}

HRESULT CResFile::GetSize(uint32 &ui32Size)
{
	ui32Size = _ui32Size;
	return S_OK;
}

HRESULT CResFile::IsOpen(bool &bOpened)
{
	bOpened = _bOpened;
	return S_OK;
}

HRESULT CResFile::GetName(char *pcName, uint uiCharsCount)
{
	if (uiCharsCount < strlen(_acName))
	{
		LOG("Too small \"pcName\" buffer size.", LT_ERROR);
		return E_INVALIDARG;
	}

	strcpy(pcName, _acName);

	return S_OK;
}

HRESULT CALLBACK CResFile::GetPath(char *pcPath, uint uiCharsCount)
{
	strcpy(pcPath, "");
	return S_OK;
}

HRESULT CALLBACK CResFile::Free()
{
	delete this;
	return S_OK;
}