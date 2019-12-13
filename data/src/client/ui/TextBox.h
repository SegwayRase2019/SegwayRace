#pragma once

#include "./Canvas.h"

class TextBox : public Canvas
{
public:
    TextBox(class Game* game, const std::string& text, std::function<void()> onOK);
    ~TextBox();
};