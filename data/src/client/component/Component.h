#pragma once

#include <cstdint>

class Component
{
public:
  // コンストラクタ
  Component(class Actor *owner, int updateOrder = 100);
  // デストラクタ
  virtual ~Component();
  // アップデート
  virtual void Update(float deltaTime);
  // 入力処理
  virtual void ProcessInput(const uint8_t *keyState) {}
  //ワールド
  virtual void OnUpdateWorldTransform();

  // ゲッター
  class Actor *GetOwner() { return mOwner; }
  int GetUpdateOrder() const { return mUpdateOrder; }

protected:
  class Actor *mOwner;
  int mUpdateOrder;
};