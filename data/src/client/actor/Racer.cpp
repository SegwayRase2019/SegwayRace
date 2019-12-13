#include "./Racer.h"
#include "../component/SpriteComponent.h"
#include "../main/Game.h"
#include "../main/Client_window.h"

Racer::Racer(Game *game, int clientID)
	: Actor(game)
{
	// Create a sprite component
	SpriteComponent *sc = new SpriteComponent(this, 50);
	switch (clientID)
	{
	case 0:
		sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/Player_red.png"));
		break;
	case 1:
		sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/Player_blue.png"));
		break;
	case 2:
		sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/Player_green.png"));
		break;
	case 3:
		sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/Player_purple.png"));
		break;
	default:
		sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/Player_purple.png"));
		break;
	}
}

void Racer::UpdateActor(float deltaTime)
{
}

void Racer::ActorInput(const uint8_t *keyState)
{
}