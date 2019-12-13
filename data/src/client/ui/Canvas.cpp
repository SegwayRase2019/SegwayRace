#include "./Canvas.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include "./Font.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

Canvas::Canvas(class Game *game)
    : mGame(game), mTitle(nullptr), mBackGround(nullptr), mTitlePos(200.0f, 10.0f), mNextButtonPos(300.0f, 400.0f), mBGPos(0.0f, 0.0f), mState(EActive)
{
    mGame->PushUI(this);
    mFont = mGame->GetFont("assets/fonts/Default.ttf");
    mButtonOn = mGame->GetClient_window()->GetTexture("assets/images/ButtonYellow.png");
    mButtonOn = mGame->GetClient_window()->GetTexture("assets/images/ButtonBlue.png");
}

Canvas::~Canvas()
{
    if (mTitle)
    {
        SDL_DestroyTexture(mTitle);
    }

    for (auto b : mButtons)
    {
        delete b;
    }
    mButtons.clear();
}

void Canvas::Update(float deltaTime)
{
}

void Canvas::Draw(SDL_Renderer *renderer)
{
    if (mBackGround)
    {
        DrawTexture(renderer, mBackGround, mBGPos);
    }
    // Draw title (if exists)
    if (mTitle)
    {
        DrawTexture(renderer, mTitle, mTitlePos);
    }
    // Draw buttons
    for (auto b : mButtons)
    {
        // Draw background of button
        SDL_Texture *tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
        DrawTexture(renderer, tex, b->GetPosition());
        // Draw text of button
        DrawTexture(renderer, b->GetNameTex(), b->GetPosition());
    }
}

void Canvas::ProcessInput(const uint8_t *keys)
{
    // Do we have buttons?
    if (!mButtons.empty())
    {
        // Get position of mouse
        int x, y;
        SDL_GetMouseState(&x, &y);
        // Convert to (0,0) center coordinates
        Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
        mousePos.x -= mGame->GetClient_window()->GetWidth() * 0.5f;
        mousePos.y = mGame->GetClient_window()->GetHeight() * 0.5f - mousePos.y;

        // Highlight any buttons
        for (auto b : mButtons)
        {
            if (b->ContainsPoint(mousePos))
            {
                b->SetHighlighted(true);
            }
            else
            {
                b->SetHighlighted(false);
            }
        }
    }
}

void Canvas::HandleKeyPress(int key)
{
    switch (key)
    {
    case SDL_BUTTON_LEFT:
        if (!mButtons.empty())
        {
            for (auto b : mButtons)
            {
                if (b->GetHighlighted())
                {
                    b->OnClick();
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
}

void Canvas::Close()
{
    mState = EClosing;
}

void Canvas::SetTitle(const std::string &text,
                      const Vector3 &color,
                      int pointSize)
{
    if (mTitle)
    {
        mTitle = nullptr;
    }
    mTitle = mFont->RenderText(text, color, pointSize);
}

void Canvas::AddButton(const std::string &name, std::function<void()> onClick)
{
    int w, h;
    SDL_QueryTexture(mButtonOn, nullptr, nullptr, &w, &h);
    Vector2 dims(static_cast<float>(w),
                 static_cast<float>(h));
    Button *b = new Button(name, mFont, onClick, mNextButtonPos, dims);
    mButtons.emplace_back(b);

    // Update position of next button
    // Move down by height of button plus padding
    SDL_QueryTexture(mButtonOff, nullptr, nullptr, &w, &h);
    mNextButtonPos.y -= h + 20.0f;
}

void Canvas::DrawTexture(SDL_Renderer *renderer, SDL_Texture *texture,
                         const Vector2 &offset, float scale)
{
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

    SDL_Rect r;
    r.w = static_cast<int>(w*scale);
    r.h = static_cast<int>(h*scale);

    r.x = static_cast<int>(offset.x);
    r.y = static_cast<int>(offset.y);

    SDL_RenderCopy(renderer, texture, nullptr, &r);
}