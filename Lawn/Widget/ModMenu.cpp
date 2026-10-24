#include "ModMenu.h"
#include "../Board.h"
#include "../Coin.h"
#include "../Zombie.h"
#include "GameButton.h"
#include "../System/PlayerInfo.h"
#include "../../LawnApp.h"
#include "../../Resources.h"
#include "../../ConstEnums.h"
#include "../../Sexy.TodLib/TodStringFile.h"

using namespace Sexy;

namespace ModCheats
{
	bool gInfiniteSun = false;
	bool gNoCooldown = false;
	bool gAutoCollect = false;

	void MarkCheatsUsed()
	{
		if (gLawnApp && gLawnApp->mPlayerInfo)
			gLawnApp->mPlayerInfo->mHasUsedCheatKeys = true;
	}

	void ApplyToBoard(Board* theBoard)
	{
		if (theBoard == nullptr)
			return;

		if (gInfiniteSun)
			theBoard->mSunMoney = 9990;

		if (gAutoCollect)
		{
			Coin* aCoin = nullptr;
			while (theBoard->IterateCoins(aCoin))
			{
				// only sun and money: grabbing trophies / presents automatically would skip level events
				if (!aCoin->mDead && !aCoin->mIsBeingCollected && (aCoin->IsSun() || aCoin->IsMoney()))
					aCoin->Collect();
			}
		}
	}

	void KillAllZombies(Board* theBoard)
	{
		if (theBoard == nullptr)
			return;

		Zombie* aZombie = nullptr;
		while (theBoard->IterateZombies(aZombie))
			aZombie->DieNoLoot();
	}
}

//////////////////////////////////////////////////////////////////////////

ModMenuDialog::ModMenuDialog(LawnApp* theApp) : LawnDialog(
	theApp,
	Dialogs::DIALOG_MODMENU,
	true,
	__S("MOD MENU"),
	__S(""),
	__S("[DIALOG_BUTTON_OK]"),
	Dialog::BUTTONS_FOOTER)
{
	mButtons[0] = MakeButton(ModMenu_InfiniteSun, this, __S(""));
	mButtons[1] = MakeButton(ModMenu_NoCooldown, this, __S(""));
	mButtons[2] = MakeButton(ModMenu_AutoCollect, this, __S(""));
	mButtons[3] = MakeButton(ModMenu_KillZombies, this, __S(""));

	RefreshLabels();

	mTallBottom = true;
	CalcSize(10, 60 + 4 * 46);
}

ModMenuDialog::~ModMenuDialog()
{
	for (int i = 0; i < ModMenu_ButtonCount; i++)
		delete mButtons[i];
}

void ModMenuDialog::RefreshLabels()
{
	mButtons[0]->mLabel = ModCheats::gInfiniteSun ? __S("Sol infinito: SI") : __S("Sol infinito: NO");
	mButtons[1]->mLabel = ModCheats::gNoCooldown ? __S("Sin cooldown: SI") : __S("Sin cooldown: NO");
	mButtons[2]->mLabel = ModCheats::gAutoCollect ? __S("Recoger auto: SI") : __S("Recoger auto: NO");
	mButtons[3]->mLabel = __S("Matar zombis");

	for (int i = 0; i < ModMenu_ButtonCount; i++)
		mButtons[i]->MarkDirty();
}

int ModMenuDialog::GetPreferredHeight(int theWidth)
{
	return LawnDialog::GetPreferredHeight(theWidth) + 40;
}

void ModMenuDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	LawnDialog::Resize(theX, theY, theWidth, theHeight);

	int aBtnWidth = IMAGE_BUTTON_LEFT->mWidth + IMAGE_BUTTON_MIDDLE->mWidth * 3 + IMAGE_BUTTON_RIGHT->mWidth;
	int aBtnHeight = mLawnYesButton->mHeight;
	int aX = theWidth / 2 - aBtnWidth / 2;

	// stack the toggles right above the footer (OK) button
	int aY = mLawnYesButton->mY - aBtnHeight * ModMenu_ButtonCount;
	for (int i = 0; i < ModMenu_ButtonCount; i++)
		mButtons[i]->Resize(aX, aY + i * aBtnHeight, aBtnWidth, aBtnHeight);
}

void ModMenuDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	LawnDialog::AddedToManager(theWidgetManager);
	for (int i = 0; i < ModMenu_ButtonCount; i++)
		AddWidget(mButtons[i]);
}

void ModMenuDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	LawnDialog::RemovedFromManager(theWidgetManager);
	for (int i = 0; i < ModMenu_ButtonCount; i++)
		RemoveWidget(mButtons[i]);
}

void ModMenuDialog::ButtonDepress(int theId)
{
	LawnDialog::ButtonDepress(theId);

	switch (theId)
	{
	case ModMenu_InfiniteSun:
		ModCheats::gInfiniteSun = !ModCheats::gInfiniteSun;
		if (ModCheats::gInfiniteSun) ModCheats::MarkCheatsUsed();
		RefreshLabels();
		break;

	case ModMenu_NoCooldown:
		ModCheats::gNoCooldown = !ModCheats::gNoCooldown;
		if (ModCheats::gNoCooldown) ModCheats::MarkCheatsUsed();
		RefreshLabels();
		break;

	case ModMenu_AutoCollect:
		ModCheats::gAutoCollect = !ModCheats::gAutoCollect;
		if (ModCheats::gAutoCollect) ModCheats::MarkCheatsUsed();
		RefreshLabels();
		break;

	case ModMenu_KillZombies:
		ModCheats::MarkCheatsUsed();
		ModCheats::KillAllZombies(mApp->mBoard);
		break;
	}
}
