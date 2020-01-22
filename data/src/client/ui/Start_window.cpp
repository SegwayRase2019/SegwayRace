#include "./Start_window.h"
#include "../../common/math/Math.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include "../main/Client_func.h"
#include "./Font.h"
#include "./Wait_window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

Startwindow::Startwindow(Game *game)
    : Canvas(game)
{
    SetTitle("SegwayRace", Color::Black, 72);
    mBackGround = mGame->GetClient_window()->GetTexture("assets/images/ResultBG.png");
    mText = mFont->RenderText("Your charactor", Color::White, 48);
    mPushText = mFont->RenderText("Please push \"Enter\"!", Color::White, 30);

    switch (mGame->clientID)
    {
    case 0:
        mPlTex = mGame->GetClient_window()->GetTexture("assets/images/Player_red.png");
        break;
    case 1:
        mPlTex = mGame->GetClient_window()->GetTexture("assets/images/Player_blue.png");
        break;
    case 2:
        mPlTex = mGame->GetClient_window()->GetTexture("assets/images/Player_green.png");
        break;
    case 3:
        mPlTex = mGame->GetClient_window()->GetTexture("assets/images/Player_purple.png");
        break;
    }
}

Startwindow::~Startwindow()
{
}

void Startwindow::Draw(SDL_Renderer *renderer)
{
    Canvas::Draw(renderer);
    int w, h;
    float uiSize = (float)mWindowHeight / 800.0f;
    Vector2 uiPos = Vector2::Zero;
    SDL_QueryTexture(mText, NULL, NULL, &w, &h);
    uiPos.x = mWindowHeight / 2 - (float)w * uiSize / 2;
    uiPos.y = mWindowHeight / 2 - (float)h * uiSize;
    DrawTexture(renderer, mText, uiPos, uiSize);
    uiPos.y += (float)h * uiSize;
    SDL_QueryTexture(mPlTex, NULL, NULL, &w, &h);
    uiPos.x = mWindowHeight / 2 - (float)w * uiSize / 2;
    DrawTexture(renderer, mPlTex, uiPos, uiSize);
    uiPos.y += (float)h * uiSize;
    SDL_QueryTexture(mPushText, NULL, NULL, &w, &h);
    uiPos.x = mWindowHeight / 2 - (float)w * uiSize / 2;
    DrawTexture(renderer, mPushText, uiPos, uiSize);
}

void Startwindow::Update(float deltaTime)
{
    if (mGame->GetGameState() == Game::ERunning)
        Close();
}

void Startwindow::HandleKeyPress(int key)
{
    if (mGame->GetGameState() == Game::EStartwindow && key == SDLK_RETURN)
    {
        mGame->SetGameState(Game::EWaitPlayer);
        class Waitwindow *ww = new Waitwindow(mGame);
        mGame->GetClient_command()->SendWaitSignal();
    }
}