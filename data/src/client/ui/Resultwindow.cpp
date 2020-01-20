#include "Resultwindow.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include "../main/Client_func.h"
#include "./TextBox.h"
#include <SDL2/SDL.h>

Resultwindow::Resultwindow(Game *game)
    : Canvas(game)
{
    SetTitle("リザルト画面");
    mBackGround = mGame->GetClient_window()->GetTexture("assets/images/ResultBG.png");
    for (int i = 0; i < mClientNum; i++)
    {
        switch (i)
        {
        case 0:
            mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankOne.png");
            mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_red.png");
            break;
        case 1:
            mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankTwo.png");
            mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_blue.png");
            break;
        case 2:
            mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankThree.png");
            mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_green.png");
            break;
        case 3:
            mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankFour.png");
            mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_purple.png");
            break;
        }
        mRank[mGame->GetClient_command()->PlayerPos[i].rank - 1] = mGame->GetClient_command()->PlayerPos[i].Client_id;
    }
}

Resultwindow::~Resultwindow()
{
}

void Resultwindow::Draw(SDL_Renderer *renderer)
{
    Canvas::Draw(renderer);

    // ランキングの描画
    int w, h;
    Vector2 uiPos = Vector2::Zero;
    Vector2 uiPos2 = Vector2::Zero;
    float uiSize = (float)mWindowHeight / 800;
    SDL_QueryTexture(mRankingUI[0], NULL, NULL, &w, &h);
    for (int i = 0; i < mClientNum; i++)
    {
        uiPos.x = (float)w * uiSize * 1.5f;
        uiPos.y += (float)h * uiSize * 1.5f;
        DrawTexture(renderer, mRankingUI[i], uiPos, uiSize);
    }
    for (int i = 0; i < mClientNum; i++)
    {
        uiPos.x = (float)w * uiSize * 3.0f;
        uiPos.y = (float)h * uiSize * 1.5f * (i + 1);
        DrawTexture(renderer, mRacerUI[mRank[i]], uiPos, uiSize);
    }
}

void Resultwindow::HandleKeyPress(int key)
{
    Canvas::HandleKeyPress(key);

    if (key == SDLK_ESCAPE)
    {
        Close();
    }
}