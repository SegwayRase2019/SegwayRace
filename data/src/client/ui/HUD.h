#pragma once

#include "../../common/Common.h"
#include "../component/ItemComponent.h"
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

    void AddItemComponent(class ItemComponent *item);
    void RemoveItemComponent(class ItemComponent *item);

  protected:
    void UpdateRanking();
    void PlayerPosUpdate();

    SDL_Texture* mRankingUI[MAX_CLIENTS];
    SDL_Texture* mRacerUI[MAX_CLIENTS];
    SDL_Texture *mPlayerPosUI[MAX_CLIENTS];
    SDL_Texture *mNoPlayerUI;
    SDL_Texture *mMapUI;
    SDL_Texture *mMapBaseUI;

    std::vector<class ItemComponent *> mItems;

    int mRank[MAX_CLIENTS] = {3,2,1,0};

    Vector2 mPlPos[MAX_CLIENTS];

    std::vector<class Racer*> mRacers;
};