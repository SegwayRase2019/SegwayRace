<<<<<<< HEAD
#include "./Client_window.h"
#include "../component/SpriteComponent.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <algorithm>
=======
#include "Client_func.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
>>>>>>> 98288e8... ネットワークのマルチスレッド化

static SDL_Rect gButtonRect[MAX_CLIENTS + 2];

Client_window::Client_window(Game *game)
	: mWindow(nullptr), mRenderer(nullptr)
{
	mGame = game;
}

bool Client_window::InitWindows(int clientID, int num, char name[][MAX_NAME_SIZE])
{
<<<<<<< HEAD
	mClient_command = mGame->GetClient_command();
=======
	mClient_command = mGame->getClient_command();
>>>>>>> 98288e8... ネットワークのマルチスレッド化

	int i;
	SDL_Surface *image;
	char clientButton[4][30] = {"./assets/images/0.jpg", "./assets/images/1.jpg", "./assets/images/2.jpg", "./assets/images/3.jpg"};
	char endButton[] = "./assets/images/END.jpg";
	char allButton[] = "./assets/images/ALL.jpg";
	char *s, title[10];

	/* 引き数チェック */
	assert(0 < num && num <= MAX_CLIENTS);

	/* SDLの初期化 */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("failed to initialize SDL.\n");
		return false;
	}

	/* メインのウインドウを作成する */
	mWindow = SDL_CreateWindow(
		"SegwayRace", // Window title
		100,		  // Top left x-coordinate of window
		100,		  // Top left y-coordinate of window
		560,		  // Width of window
		560,		  // Height of window
<<<<<<< HEAD
		0			  // Flags (0 for no flags Set)
=======
		0			  // Flags (0 for no flags set)
>>>>>>> 98288e8... ネットワークのマルチスレッド化
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

	/* ボタンの作成 */
	SDL_Texture *tex = nullptr;
	for (i = 0; i < num + 2; i++)
	{
		gButtonRect[i].x = 20 + 80 * i;
		gButtonRect[i].y = 10;
		gButtonRect[i].w = 70;
		gButtonRect[i].h = 20;

		if (i == num)
		{
			s = allButton;
<<<<<<< HEAD
			tex = GetTexture(allButton);
		}
		else if (i == num + 1)
		{
			// s = endButton;
			tex = GetTexture(endButton);
		}
		else
		{
			// s = clientButton[i];
			tex = GetTexture(clientButton[i]);
		}
		// image = IMG_Load(s);
		// tex = SDL_CreateTextureFromSurface(mRenderer, image);
		// SDL_FreeSurface(image);
=======
			tex = getTexture(allButton);
			//SDL_QueryTexture(tex, nullptr, nullptr, &gButtonRect[i].w, &gButtonRect[i].h);
		}
		else if (i == num + 1)
		{
			s = endButton;
			tex = getTexture(endButton);
			//SDL_QueryTexture(tex, nullptr, nullptr, &gButtonRect[i].w, &gButtonRect[i].h);
		}
		else
		{
			s = clientButton[i];
			tex = getTexture(clientButton[i]);
			//SDL_QueryTexture(tex, nullptr, nullptr, &gButtonRect[i].w, &gButtonRect[i].h);
		}
		image = IMG_Load(s);
		tex = SDL_CreateTextureFromSurface(mRenderer, image);
		SDL_FreeSurface(image);
>>>>>>> 98288e8... ネットワークのマルチスレッド化

		SDL_RenderCopy(mRenderer, tex, NULL, &gButtonRect[i]);
	}
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

<<<<<<< HEAD
SDL_Texture *Client_window::GetTexture(const std::string &filename)
=======
SDL_Texture *Client_window::getTexture(const std::string &filename)
>>>>>>> 98288e8... ネットワークのマルチスレッド化
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

<<<<<<< HEAD
void Client_window::AddSprite(SpriteComponent *sprite)
{
	int myDrawOrder = sprite -> GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		 iter != mSprites.end();
		 ++iter)
	{
		if (myDrawOrder < (*iter) -> GetDrawOrder())
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
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}
=======
void Client_window::WindowEvent(int num)
{
	SDL_Event event;
	SDL_MouseButtonEvent *mouse;
	int buttonNO;

	/* 背景を白にする */
	/*
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
	SDL_RenderClear(mRenderer);

	SDL_RenderPresent(mRenderer);
	*/

	/* 引き数チェック */
	assert(0 < num && num <= MAX_CLIENTS);

	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mClient_command->SendEndCommand();
			break;
		case SDL_MOUSEBUTTONUP:
			mouse = (SDL_MouseButtonEvent *)&event;
			if (mouse->button == SDL_BUTTON_LEFT)
			{
				buttonNO = CheckButtonNO(mouse->x, mouse->y, num);
#ifndef NDEBUG
				printf("#####\n");
				printf("WindowEvent()\n");
				printf("Button %d is pressed\n", buttonNO);
#endif
				if (0 <= buttonNO && buttonNO < num)
				{
					/* 名前の書かれたボタンが押された */
				}
				else if (buttonNO == num)
				{
					/* 「All」と書かれたボタンが押された */
				}
				else if (buttonNO == num + 1)
				{
					/* 「End」と書かれたボタンが押された */
					SDL_Log("Push EndButton\n");
					mClient_command->SendEndCommand();
				}
			}
			break;
		}
	}
}

void Client_window::DrawRectangle(int x, int y, int width, int height)
{
#ifndef NDEBUG
	printf("#####\n");
	printf("DrawRectangle()\n");
	printf("x=%d,y=%d,width=%d,height=%d\n", x, y, width, height);
#endif

	//rectangleColor(mWindow,x,y,x+width,y+height,0x000000ff);
	SDL_RenderPresent(mRenderer);
}

void Client_window::DrawCircle(int x, int y, int r)
{
#ifndef NDEBUG
	printf("#####\n");
	printf("DrawCircle()\n");
	printf("x=%d,y=%d,tyokkei=%d\n", x, y, r);
#endif

	//circleColor(mWindow,x,y,r,0x000000ff);
	SDL_RenderPresent(mRenderer);
}

void Client_window::DrawDiamond(int x, int y, int height)
{
	Sint16 vx[5], vy[5];
	int i;

#ifndef NDEBUG
	printf("#####\n");
	printf("DrawDiamond()\n");
	printf("x=%d,y=%d,height=%d\n", x, y, height);
#endif

	for (i = 0; i < 4; i++)
	{
		vx[i] = x + height * ((1 - i) % 2) / 2;
		vy[i] = y + height * ((2 - i) % 2);
	}
	vx[4] = vx[0];
	vy[4] = vy[0];

	SDL_RenderPresent(mRenderer);
}

int Client_window::CheckButtonNO(int x, int y, int num)
{
	int i;

	for (i = 0; i < num + 2; i++)
	{
		if (gButtonRect[i].x < x &&
			gButtonRect[i].y < y &&
			gButtonRect[i].x + gButtonRect[i].w > x &&
			gButtonRect[i].y + gButtonRect[i].h > y)
		{
			return i;
		}
	}
	return -1;
}
>>>>>>> 98288e8... ネットワークのマルチスレッド化
