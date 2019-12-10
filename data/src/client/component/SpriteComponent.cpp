#include "./SpriteComponent.h"
#include "../actor/Actor.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include "../../common/math/Math.h"
#include "../actor/Player.h"

SpriteComponent::SpriteComponent(Actor *owner, int drawOrder)
    : Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0)
{
    mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
    mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer *renderer)
{
    Vector2 plPos = mOwner->GetGame()->mPlayer->GetPosition();
    Vector2 winCenter(mOwner->GetGame()->GetClient_window()->GetWidth() / 2, mOwner->GetGame()->GetClient_window()->GetHeight() / 2);
    winCenter -= plPos;
    Vector2 acPos = mOwner->GetPosition();
    plPos += winCenter;
    acPos += winCenter;
    Vector2 delPl = acPos - plPos;
    delPl.y *= -1;
    //winCenter -= plPos;
    //float playerRot = mOwner->GetGame()->mPlayer->GetRotation();
    Matrix3 deltaPlayer = Matrix3::CreateTranslation(delPl);
    float rot = mOwner->GetRotation();
    float playerRot = mOwner->GetGame()->mPlayer->GetRotation();
    float delRot = rot - playerRot;
    Matrix3 deltaRot = Matrix3::CreateRotation(-playerRot);

    Matrix3 view = deltaPlayer * deltaRot;
    Vector2 viewPos(view.mat[2][0], -view.mat[2][1]);
    viewPos = viewPos + plPos;

    if (mTexture)
    {
        SDL_Rect r;
        r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
        r.h = static_cast<int>(mTexHeight * mOwner->GetScale());

        r.x = static_cast<int>(viewPos.x - r.w / 2);
        r.y = static_cast<int>(viewPos.y - r.h / 2);

        SDL_RenderCopyEx(renderer, mTexture, nullptr, &r, -Math::ToDegrees(delRot), nullptr, SDL_FLIP_NONE);
    }
}

void SpriteComponent::SetTexture(SDL_Texture *texture)
{
    mTexture = texture;
    SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}