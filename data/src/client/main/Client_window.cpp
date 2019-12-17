#include "./Client_window.h"
#include "../component/Component.h"
#include "../component/SpriteComponent.h"
#include "../actor/Actor.h"
#include "../ui/Canvas.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <algorithm>

static SDL_Rect gButtonRect[MAX_CLIENTS + 2];

Client_window::Client_window(Game *game)
	: mWindow(nullptr), mRenderer(nullptr)
{
	mGame = game;
	mWindowWidth = 560;
	mWindowHeight = 560;
}

bool Client_window::InitWindows(int clientID, int num, char name[][MAX_NAME_SIZE])
{
	mClient_command = mGame->GetClient_command();

	char *s, title[20];

	/* 引き数チェック */
	assert(0 < num && num <= MAX_CLIENTS);

	/* SDLの初期化 */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("failed to initialize SDL.\n");
		return false;
	}

	sprintf(title, "SegwayRace(ID:%d)", clientID);
	/* メインのウインドウを作成する */
	mWindow = SDL_CreateWindow(
		title, // Window title
		100,		  // Top left x-coordinate of window
		100,		  // Top left y-coordinate of window
		mWindowWidth,		  // Width of window
		mWindowHeight,		  // Height of window
		0			  // Flags (0 for no flags Set)
	);
	if (!mWindow)
	{
		SDL_Log("Failed to criate window:%s\n", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	/* image初期化 */
	if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG))
	{
		SDL_Log("failed to initialize SDL_Image:%s", SDL_GetError());
		return false;
	}

	/* 背景を白にする */
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
	SDL_RenderClear(mRenderer);

	SDL_RenderPresent(mRenderer);

	return true;
}

void Client_window::DestroyWindow(void)
{
	// Destory texture
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();

	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

SDL_Texture *Client_window::GetTexture(const std::string &filename)
{
	SDL_Texture *tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(filename);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface *surf = IMG_Load(filename.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", filename.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", filename.c_str());
			return nullptr;
		}

		mTextures.emplace(filename.c_str(), tex);
	}
	return tex;
}

/*SurfaceからTextureを作成する関数*/
/*　　　　文字の描画に使用　　　　*/
SDL_Texture *Client_window::CreateFromSurface(SDL_Surface *surf)
{
	SDL_Texture *tex = nullptr;
	if (!surf)
	{
		SDL_Log("Failed to load surfacse for fonts");
		return nullptr;
	}

	tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		SDL_Log("Failed to convert surface to texture for font");
		return nullptr;
	}
	return tex;
}

void Client_window::AddSprite(SpriteComponent *sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		 iter != mSprites.end();
		 ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSprites.insert(iter, sprite);
}

void Client_window::RemoveSprite(SpriteComponent *sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Client_window::Draw()
{
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
	SDL_RenderClear(mRenderer);

	// Draw all sprite components
	for (auto sprite : mSprites)
	{
		if(sprite->Component::GetOwner()->GetState()==Actor::EActive)
			sprite->Draw(mRenderer);
	}

	//UIの描画
	for (auto ui : mGame->GetUIStack())
	{
		ui->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}
