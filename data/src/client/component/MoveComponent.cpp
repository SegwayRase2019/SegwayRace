#include "./MoveComponent.h"
#include "../actor/Actor.h"
#include "../main/Music.h"

MoveComponent::MoveComponent(class Actor *owner, int updateOrder)
    : Component(owner, updateOrder), mAngularSpeed(0.0f)
{
}

float MoveComponent::mForwardSpeed = 0.0f;

void MoveComponent::Update(float deltaTime)
{

    if (!Math::NearZero(mAngularSpeed))
    {
        float rot = mOwner->GetRotation();
        rot += mAngularSpeed * deltaTime;

        Sound::Curve_Sound();

        mOwner->SetRotation(rot);
    }

    if (!Math::NearZero(mForwardSpeed))
    {
        Vector2 pos = mOwner->GetPosition();
        pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
        Sound::Engine_Sound();

        mOwner->SetPosition(pos);
    }
}