
/*******************************************************************/

#include <fstream>
#include <string>
#include <iostream>
#include <Windows.h>
#include <psapi.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>

#define MAX_ROWS_MD5 19

extern "C" {
extern int _fltused;
volatile int _AVOID_FLOATING_POINT_LIBRARY_BUG = _fltused;
};


/*******************************************************************/
// replace:

int Setup(LPSTR lpCmdLine); // Internal function forward

bool PackInitialize(const char * c_pszFolder)
{
	if (_access(c_pszFolder, 0) != 0)
		return true;

	std::string stFolder(c_pszFolder);
	stFolder += "/";

	std::string stFileName(stFolder);
	stFileName += "Index";

	CMappedFile file;
	LPCVOID pvData;

	if (!file.Create(stFileName.c_str(), &pvData, 0, 0))
	{
		LogBoxf("FATAL ERROR! File not exist: %s", stFileName.c_str());
		TraceError("FATAL ERROR! File not exist: %s", stFileName.c_str());
		return true;
	}

	CMemoryTextFileLoader TextLoader;
	TextLoader.Bind(file.Size(), pvData);

	bool bPackFirst = TRUE;

	const std::string& strPackType = TextLoader.GetLineString(0);

	if (strPackType.compare("FILE") && strPackType.compare("PACK"))
	{
		TraceError("Pack/Index has invalid syntax. First line must be 'PACK' or 'FILE'");
		return false;
	}

#ifdef NDEBUG // @warme601 _DISTRIBUTE -> NDEBUG
	Tracef("Note: PackFirst mode enabled. [pack]\n");
#else
	bPackFirst = FALSE;
	Tracef("Note: PackFirst mode not enabled. [file]\n");
#endif

	CTextFileLoader::SetCacheMode();
#if defined(USE_RELATIVE_PATH)
	CEterPackManager::Instance().SetRelativePathMode();
#endif
	CEterPackManager::Instance().SetCacheMode();
	CEterPackManager::Instance().SetSearchMode(bPackFirst);

	CSoundData::SetPackMode();

	std::string strPackName, strTexCachePackName;
	for (DWORD i = 1; i < TextLoader.GetLineCount() - 1; i += 2)
	{
		const std::string & c_rstFolder = TextLoader.GetLineString(i);
		const std::string & c_rstName = TextLoader.GetLineString(i + 1);

		strPackName = stFolder + c_rstName;
		strTexCachePackName = strPackName + "_texcache";

		CEterPackManager::Instance().RegisterPack(strPackName.c_str(), c_rstFolder.c_str());
		CEterPackManager::Instance().RegisterPack(strTexCachePackName.c_str(), c_rstFolder.c_str());
	}

	CEterPackManager::Instance().RegisterRootPack((stFolder + std::string("root")).c_str());
	return true;
}



/*******************************************************************/
// with:



bool PackInitialize(const char * c_pszFolder)
{
	if (_access(c_pszFolder, 0) != 0)
		return true;

	std::string stFolder(c_pszFolder);
	stFolder += "/";

#ifndef ENABLE_INDEX_AT_SOURCE
	std::string stFileName(stFolder);
	stFileName += "Index";

	CMappedFile file;
	LPCVOID pvData;

	if (!file.Create(stFileName.c_str(), &pvData, 0, 0))
	{
		LogBoxf("FATAL ERROR! File not exist: %s", stFileName.c_str());
		TraceError("FATAL ERROR! File not exist: %s", stFileName.c_str());
		return true;
	}

	CMemoryTextFileLoader TextLoader;
	TextLoader.Bind(file.Size(), pvData);
#endif

// Fix reading d:/ymir work, fps drops, slow loading time
	bool bPackFirst = TRUE;

#ifndef ENABLE_INDEX_AT_SOURCE
	const std::string& strPackType = TextLoader.GetLineString(0);

	if (strPackType.compare("FILE") && strPackType.compare("PACK"))
	{
		TraceError("Pack/Index has invalid syntax. First line must be 'PACK' or 'FILE'");
		return false;
	}
#endif

#ifdef NDEBUG // @warme601 _DISTRIBUTE -> NDEBUG
	Tracef("Note: PackFirst mode enabled. [pack]\n");
#else
// Fix reading d:/ymir work, fps drops, slow loading time
	bPackFirst = FALSE;
	Tracef("Note: PackFirst mode not enabled. [file]\n");
#endif

	CTextFileLoader::SetCacheMode();
#if defined(USE_RELATIVE_PATH)
	CEterPackManager::Instance().SetRelativePathMode();
#endif
	CEterPackManager::Instance().SetCacheMode();
	CEterPackManager::Instance().SetSearchMode(bPackFirst);

	CSoundData::SetPackMode();

#ifndef ENABLE_INDEX_AT_SOURCE
	std::string strPackName, strTexCachePackName;
	for (DWORD i = 1; i < TextLoader.GetLineCount() - 1; i += 2)
	{
		const std::string & c_rstFolder = TextLoader.GetLineString(i);
		const std::string & c_rstName = TextLoader.GetLineString(i + 1);

		strPackName = stFolder + c_rstName;
		strTexCachePackName = strPackName + "_texcache";

		CEterPackManager::Instance().RegisterPack(strPackName.c_str(), c_rstFolder.c_str());
		CEterPackManager::Instance().RegisterPack(strTexCachePackName.c_str(), c_rstFolder.c_str());
	}
#endif

#ifdef ENABLE_INDEX_AT_SOURCE
	CEterPackManager::Instance().RegisterPack("pack/bgm", "*");
	CEterPackManager::Instance().RegisterPack("pack/icon", "*");
	CEterPackManager::Instance().RegisterPack("pack/locale", "*");
	CEterPackManager::Instance().RegisterPack("pack/maps", "*");
	CEterPackManager::Instance().RegisterPack("pack/property", "*");
	CEterPackManager::Instance().RegisterPack("pack/season", "*");
	CEterPackManager::Instance().RegisterPack("pack/sound", "*");
	CEterPackManager::Instance().RegisterPack("pack/textureset", "*");
	CEterPackManager::Instance().RegisterPack("pack/uiscript", "*");
	CEterPackManager::Instance().RegisterPack("pack/effect", "*");
	CEterPackManager::Instance().RegisterPack("pack/guild", "*");
	CEterPackManager::Instance().RegisterPack("pack/item", "*");
	CEterPackManager::Instance().RegisterPack("pack/monster", "*");
	CEterPackManager::Instance().RegisterPack("pack/monster2", "*");
	CEterPackManager::Instance().RegisterPack("pack/npc", "*");
	CEterPackManager::Instance().RegisterPack("pack/npc2", "*");
	CEterPackManager::Instance().RegisterPack("pack/npc_mount", "*");
	CEterPackManager::Instance().RegisterPack("pack/npc_pet", "*");
	CEterPackManager::Instance().RegisterPack("pack/pc", "*");
	CEterPackManager::Instance().RegisterPack("pack/pc2", "*");
	CEterPackManager::Instance().RegisterPack("pack/terrain", "*");
	CEterPackManager::Instance().RegisterPack("pack/tree", "*");
	CEterPackManager::Instance().RegisterPack("pack/etc", "*");
	CEterPackManager::Instance().RegisterPack("pack/zone", "*");
#endif

	CEterPackManager::Instance().RegisterRootPack((stFolder + std::string("root")).c_str());
	return true;
}



/*******************************************************************/
//Note: Here you should put the names of the files from your server's pack folder or the folder you are using.

#ifdef ENABLE_INDEX_AT_SOURCE
	CEterPackManager::Instance().RegisterPack("pack/bgm", "*");
#endif
