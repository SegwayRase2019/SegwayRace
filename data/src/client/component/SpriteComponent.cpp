#include "./SpriteComponent.h"
#include "../actor/Actor.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include "../actor/Player.h"

SpriteComponent::SpriteComponent(Actor *owner, int drawOrder)
    : Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0)
{
    mOwner->GetGame()->AddSprite(this);
    winCenter.x = mOwner->GetGame()->GetClient_window()->GetWidth() / 2;
    winCenter.y = mOwner->GetGame()->GetClient_window()->GetHeight() / 2;
}

SpriteComponent::~SpriteComponent()
{
    mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer *renderer)
{

    if (mTexture)
    {
        Vector2 viewPos = PosDelta();
        float delRot = RotDelta();
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

float SpriteComponent::RotDelta()
{
    float acRot = mOwner->GetRotation();
    float playerRot = mOwner->GetGame()->mPlayer->GetRotation();
    float delRot = acRot - playerRot;

    return delRot;
}

Vector2 SpriteComponent::PosDelta()
{
    Vector2 plPos = mOwner->GetGame()->mPlayer->GetPosition();
    Vector2 acPos = mOwner->GetPosition();
    Vector2 delPl = acPos - plPos;

    delPl.y *= -1;

    float acRot = mOwner->GetRotation();
    float playerRot = mOwner->GetGame()->mPlayer->GetRotation();
    float delRot = acRot - playerRot;

    //Matrix3 rocal = mOwner->GetGame()->mPlayer->GetRocalTransform();
    //Matrix3 world = mOwner->GetWorldTransform();
    Matrix3 deltaPos = Matrix3::CreateTranslation(delPl);
    Matrix3 deltaRot = Matrix3::CreateRotation(-playerRot);
    Matrix3 view = deltaPos * deltaRot;
    //Vector2 viewPos;
    // viewPos.x = world.mat[2][0] - rocal.mat[2][0];
    // viewPos.y = world.mat[2][1] - rocal.mat[2][1];
    // printf("%f\n%f\n", world.mat[2][1], rocal.mat[2][1]);

    Vector2 viewPos(view.mat[2][0], -view.mat[2][1]);
    viewPos += winCenter;

    return viewPos;
}
