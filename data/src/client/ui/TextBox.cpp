#include "./TextBox.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include <SDL2/SDL.h>

TextBox::TextBox(Game* game, const std::string& text, std::function<void()> onOK)
    :Canvas(game)
{
    mBGPos = Vector2(0.0f, 0.0f);
    mTitlePos = Vector2(100.0f, 10.0f);
    mNextButtonPos = Vector2(0.0f, 0.0f);

    mBackGround = mGame->GetClient_window()->GetTexture("assets/images/DialogBG.png");
    SetTitle(text, Vector3::Zero, 30);
    AddButton("OKButton", [onOK]() {
		onOK();
	});
    AddButton("CancelButton", [this]() { 
		Close();
	});
}

TextBox::~TextBox()
{
}