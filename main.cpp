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
	const char* aDataDir = SDL_AndroidGetExternalStoragePath();
	if (aDataDir)
		chdir(aDataDir);

	struct stat aStat;
	if (stat("main.pak", &aStat) != 0 || stat("properties", &aStat) != 0)
	{
		std::string aMsg = "Copy main.pak and the properties folder from your PvZ GOTY copy to:\n";
		aMsg += aDataDir ? aDataDir : "(storage unavailable)";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game data not found", aMsg.c_str(), NULL);
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
