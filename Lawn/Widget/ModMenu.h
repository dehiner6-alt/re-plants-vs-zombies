#ifndef __MODMENU_H__
#define __MODMENU_H__

#include "LawnDialog.h"

class Board;

// In-game "mod menu" (PvZ Toolkit style toggles). State is process-wide and is
// applied from Board::UpdateGame / SeedPacket::Update.
namespace ModCheats
{
	extern bool gInfiniteSun;
	extern bool gNoCooldown;
	extern bool gAutoCollect;

	void		ApplyToBoard(Board* theBoard);	// called once per game tick
	void		KillAllZombies(Board* theBoard);
	void		MarkCheatsUsed();				// flags the profile so cheated runs don't count as legit
}

class ModMenuDialog : public LawnDialog
{
public:
	enum
	{
		ModMenu_InfiniteSun = 100,
		ModMenu_NoCooldown,
		ModMenu_AutoCollect,
		ModMenu_KillZombies,
		ModMenu_ButtonCount = 4
	};

public:
	LawnStoneButton*	mButtons[ModMenu_ButtonCount];

public:
	ModMenuDialog(LawnApp* theApp);
	virtual ~ModMenuDialog();

	virtual int			GetPreferredHeight(int theWidth);
	virtual void		Resize(int theX, int theY, int theWidth, int theHeight);
	virtual void		AddedToManager(WidgetManager* theWidgetManager);
	virtual void		RemovedFromManager(WidgetManager* theWidgetManager);
	virtual void		ButtonDepress(int theId);

	void				RefreshLabels();
};

#endif
