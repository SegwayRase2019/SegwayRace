#pragma once

#include "../../common/Common.h"
#include "./Canvas.h"
#include <vector>

class HUD : public Canvas
{
public:
    // 背景や他のアクターより前方に描画
    HUD(class Game* game);
    ~HUD();

    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;

    void AddRacer(class Racer* rc);
    void RemoveRacer(class Racer* rc);

protected:
    void UpdateRanking();
    int mWindowWidth;
    int mWindowHeight;

    SDL_Texture* mRankingUI[MAX_CLIENTS];
    SDL_Texture* mRacerUI[MAX_CLIENTS];
    SDL_Texture *mNoPlayerUI;

    int mRank[MAX_CLIENTS] = {3,2,1,0};

    std::vector<class Racer*> mRacers;
};