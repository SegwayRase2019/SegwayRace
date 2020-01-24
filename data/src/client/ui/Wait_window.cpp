#include "./Wait_window.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include "../main/Client_func.h"
#include "../../common/math/Math.h"
#include "./TextBox.h"
#include <SDL2/SDL.h>

Waitwindow::Waitwindow(Game *game)
    : Canvas(game)
{
    mBackGround = mGame->GetClient_window()->GetTexture("assets/images/Waitwindow.png");
    mWaitMsg = mFont->RenderText("Waiting other players ...  ", Color::White, 32);
}

Waitwindow::~Waitwindow()
{
}

void Waitwindow::Draw(SDL_Renderer *renderer)
{
    Canvas::Draw(renderer);
    int w, h;
    float uiSize = 1.0f;
    Vector2 uiPos = Vector2::Zero;
    SDL_QueryTexture(mWaitMsg, NULL, NULL, &w, &h);
    uiPos.x = mWindowWidth - (float)w * uiSize;
    uiPos.y = mWindowHeight - (float)h * uiSize;
    DrawTexture(renderer, mWaitMsg, uiPos, uiSize);
}

void Waitwindow::Update(float deltaTime)
{
    if (mGame->GetGameState() == Game::ERunning)
        Close();
}