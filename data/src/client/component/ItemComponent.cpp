#include "./ItemComponent.h"
#include "../actor/Actor.h"
#include "../main/Game.h"
#include "../ui/HUD.h"

ItemComponent::ItemComponent(Actor* owner)
    : Component(owner)
{
    mOwner->GetGame()->GetHUD()->AddItemComponent(this);
}

ItemComponent::~ItemComponent()
{
    mOwner->GetGame()->GetHUD()->RemoveItemComponent(this);
}