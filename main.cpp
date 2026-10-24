#ifdef __ANDROID__
#include <SDL.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

#include "LawnApp.h"
#include "Resources.h"
#include "Sexy.TodLib/TodStringFile.h"
using namespace Sexy;

#ifdef __3DS__
#include <3ds.h>
#include <malloc.h>
extern "C" {
	unsigned int __stacksize__ = 512 * 1024;
}
#endif

bool (*gAppCloseRequest)();				//[0x69E6A0]
bool (*gAppHasUsedCheatKeys)();			//[0x69E6A4]
SexyString (*gGetCurrentLevelName)();

//0x44E8F0
//int WINAPI WinMain(_In_ HINSTANCE /* hInstance */, _In_opt_ HINSTANCE /* hPrevInstance */, _In_ LPSTR /* lpCmdLine */, _In_ int /* nCmdShow */)
int main(int argc, char** argv)
{
#ifdef __3DS__
	osSetSpeedupEnable(true);
#endif

#ifdef __ANDROID__
	// Game data (main.pak + properties/) and saves live in Android/data/<package>/files/
	// Android 11+ hides Android/data from file managers, so the data lives in /sdcard/PvZ
	// (needs "All files access", requested by PermissionActivity). The app's own external dir is a fallback.
	const char* aSharedDir = "/sdcard/PvZ";
	const char* anAppDir = SDL_AndroidGetExternalStoragePath();
	mkdir(aSharedDir, 0777);

	struct stat aStat;
	bool aFound = false;
	const char* aCandidates[2] = { aSharedDir, anAppDir };
	for (int i = 0; i < 2 && !aFound; i++)
	{
		if (aCandidates[i] == NULL || chdir(aCandidates[i]) != 0)
			continue;
		aFound = stat("main.pak", &aStat) == 0 && stat("properties", &aStat) == 0;
	}

	if (!aFound)
	{
		chdir(aSharedDir);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game data not found",
			"Copy main.pak and the properties folder from your PvZ GOTY copy to:\n/sdcard/PvZ/\n(Internal storage > PvZ)", NULL);
		return 1;
	}
#endif

	TodStringListSetColors(gLawnStringFormats, gLawnStringFormatCount);
	gGetCurrentLevelName = LawnGetCurrentLevelName;
	gAppCloseRequest = LawnGetCloseRequest;
	gAppHasUsedCheatKeys = LawnHasUsedCheatKeys;
	gExtractResourcesByName = Sexy::ExtractResourcesByName;
	gLawnApp = new LawnApp();
	gLawnApp->Init();
	gLawnApp->Start();
	gLawnApp->Shutdown();
	if (gLawnApp)
		delete gLawnApp;

	return 0;
};
