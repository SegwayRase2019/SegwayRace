#include "./Actor.h"
#include "../main/Game.h"
#include "../component/Component.h"
#include <algorithm>

Actor::Actor(Game *game)
    : mState(EActive), mPosition(Vector2::Zero), mScale(1.0f), mRotation(0.0f), mGame(game)
{
    mGame->AddActor(this);
}

Actor::~Actor()
{
    mGame->RemoveActor(this);
    // Need to delete components
    // Because ~Component calls RemoveComponent, need a different style loop
    while (!mComponents.empty())
    {
        delete mComponents.back();
    }
}

void Actor::Update(float deltaTime)
{
    if (mState == EActive || mState == EInactive)
    {
        ComputeWorldTransform();

        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);

        ComputeWorldTransform();
    }
}

void Actor::UpdateComponents(float deltaTime)
{
    for (auto comp : mComponents)
    {
        comp->Update(deltaTime);
    }
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ProcessInput(const uint8_t *command)
{
    if (mState == EActive)
    {
        // First process input for components
        for (auto comp : mComponents)
        {
            comp->ProcessInput(command);
        }

        ActorInput(command);
    }
}

void Actor::ActorInput(const uint8_t *command)
{
}

void Actor::ComputeWorldTransform()
{
    if (mRecomputeTransform)
    {
        mRecomputeTransform = false;
        // Scale, then rotate, then translate
        //mWorldTransform = Matrix3::CreateScale(mScale);
        mWorldTransform = Matrix3::CreateTranslation(Vector2(mPosition.x, mPosition.y));
        mWorldTransform *= Matrix3::CreateRotation(mRotation);

        // // Inform components world transform updated
        // for (auto comp : mComponents)
        // {
        //     comp->OnUpdateWorldTransform();
        // }
    }
}

void Actor::AddComponent(Component *component)
{
    // Find the insertion point in the sorted vector
    // (The first element with a order higher than me)
    int myOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();
    for (;
         iter != mComponents.end();
         ++iter)
    {
        if (myOrder < (*iter)->GetUpdateOrder())
        {
            break;
        }
    }

    // Inserts element before position of iterator
    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component *component)
{
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end())
    {
        mComponents.erase(iter);
    }
}
