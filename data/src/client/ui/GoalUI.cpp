#include "./GoalUI.h"
#include "../main/Game.h"
#include "../main/Client_window.h"

GoalUI::GoalUI(Game *game)
    : Canvas(game), mCountTime(0.0f), mMaxCount(2.0f), isBigger(true)
{
    mGoalUIa = mFont->RenderText("GOAL", Color::Black, 36);
    mGoalUIb = mFont->RenderText("GOAL", Color::White, 30);
}

GoalUI::~GoalUI()
{
}

void GoalUI::Update(float deltaTime)
{
    // GoalUIが徐々に大きくなっていく時間を更新
    if (isBigger == true)
    {
        mCountTime += deltaTime;
        if (mCountTime > mMaxCount + 1)
            isBigger = false;
    }
    else
    {
        mCountTime -= deltaTime;
        if (mCountTime < mMaxCount - 1)
            isBigger = true;
    }

    if (mGame->GetGameState() == Game::EFinished)
        Close();
}

void GoalUI::Draw(SDL_Renderer *renderer)
{
    // GoalUIの描画
    Vector2 uiPos = Vector2::Zero;
    float uiSize;
    uiSize = (float)mWindowHeight / 200.0f;
    uiSize *= mCountTime / mMaxCount;
    int w, h;
    SDL_QueryTexture(mGoalUIa, NULL, NULL, &w, &h);
    int w2, h2;
    SDL_QueryTexture(mGoalUIb, NULL, NULL, &w2, &h2);
    uiPos.x = ((float)mWindowWidth - (float)w * uiSize) / 2.0f;
    uiPos.y = ((float)mWindowHeight - (float)h * uiSize) / 2.0f;
    DrawTexture(renderer, mGoalUIa, uiPos, uiSize);

    uiPos.x += ((float)w - (float)w2) / 2;
    uiPos.y += ((float)h - (float)h2) / 2;
    DrawTexture(renderer, mGoalUIb, uiPos, uiSize);
}