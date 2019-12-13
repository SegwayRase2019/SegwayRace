#pragma once

#include "./Component.h"

class ItemComponent : public Component
{
public:
  ItemComponent(class Actor* owner);
  ~ItemComponent();
};