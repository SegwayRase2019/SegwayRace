#pragma once

// #include <SDL2/ttf.h>
// #include <vector>
using namespace std;

class Component
{
  public:
    //
    Component(class Actor *owner, int updateOrder = 100);
    virtual ~Component();
    //
    virtual void Update(float deltaTime);
    int GetUpdateOrder() const { return mUpdateOrder; }

  protected:
    //
    class Actor *mOwner;
    //
    int mUpdateOrder;
};