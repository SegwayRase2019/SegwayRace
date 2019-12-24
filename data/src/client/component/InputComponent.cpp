#include "./InputComponent.h"
#include "../actor/Actor.h"
#include "../main/Game.h"
#include "../../common/Common.h"
#include "../../common/math/Math.h"

InputComponent::InputComponent(class Actor *owner)
	: MoveComponent(owner), mForwardKey(0), mBackKey(0), mClockwiseKey(0), mCounterClockwiseKey(0)
{
}

void InputComponent::ProcessInput(const uint8_t *keyState)
{
	
	if(mOwner->Actor::GetGame()->wiifit_connect == false){
	// Calculate forward speed for MoveComponent
	float forwardSpeed = 0.0f;
	if (keyState[mForwardKey])
	{
		forwardSpeed += mMaxForwardSpeed;
	}
	if (keyState[mBackKey])
	{
		forwardSpeed -= mMaxForwardSpeed; 
	}
	SetForwardSpeed(forwardSpeed);


	// Calculate angular speed for MoveComponent
	float angularSpeed = 0.0f;
	if (keyState[mClockwiseKey])
	{
		angularSpeed += mMaxAngularSpeed;
	}
	if (keyState[mCounterClockwiseKey])
	{
		angularSpeed -= mMaxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
	}
	else{
		float forwardSpeed = 0.0f;
		//prs.bal_x
	    //boolのwiiの判定trueで使えるようにする
		//Prs prs = mOwner->Actor::GetGame()->prs
		Prs prs = mOwner->Actor::GetGame()->prs;

		if(sqrt((pow(prs.bal_x, 2) + pow(prs.bal_y, 2)))<= 0.25)
		{

		}
		else if(prs.bal_y - prs.bal_x >= 0 && prs.bal_y + prs.bal_x >= 0 && sqrt(pow(prs.bal_x, 2) + pow(prs.bal_y, 2)) > 0.25 || prs.bal_y > 0.25)
		{
			forwardSpeed += mMaxForwardSpeed * prs.bal_y * 2;
		}
		else if(prs.bal_y - prs.bal_x <= 0 && prs.bal_y + prs.bal_x <= 0 && sqrt(pow(prs.bal_x, 2) + pow(prs.bal_y, 2)) > 0.25)
		{
			forwardSpeed -= mMaxForwardSpeed;
		}
		else{	}
		SetForwardSpeed(forwardSpeed);

		//angle
		float angularSpeed = 0.0f;
		//前に進みながら回る
		//基準角度76度(x=0.25をyを無限大発散した角度)
		if(prs.bal_y - prs.bal_x >= 0 && prs.bal_y + prs.bal_x >= 0)
		{
			//右回り
			if(std::atan2(prs.bal_y,prs.bal_x) < Math::Pi / 2 && prs.bal_x >= 0.25){
				angularSpeed -= mMaxAngularSpeed;
			  	angularSpeed *= ((Math::Pi * 19 / 45) - std::atan2(prs.bal_y,prs.bal_x)) * 0.20;
			}
			//左回り
			else if(std::atan2(prs.bal_y,prs.bal_x) > Math::Pi / 2 && prs.bal_x <=  -0.25){
				angularSpeed += mMaxAngularSpeed;
			  	angularSpeed *= ((Math::Pi * 19 / 45) - std::atan2(prs.bal_y,prs.bal_x * -1)) * 0.20;
			}			
		}
		//右に回転する
		else if(prs.bal_y - prs.bal_x < 0 && prs.bal_y + prs.bal_x > 0 ) 
		{
			angularSpeed -= mMaxAngularSpeed;
			if(prs.bal_y >= 0){
				angularSpeed *= ((Math::Pi * 19 / 45) - std::atan2(prs.bal_y,prs.bal_x)) * 0.15;
		    }
			else if(std::atan2(prs.bal_y * -1,prs.bal_x) <= Math::Pi / 24 && prs.bal_y < 0){
				angularSpeed *= ((Math::Pi * 19 / 45) + std::atan2(prs.bal_y * -1,prs.bal_x)) * 0.15;
			}
			else{
				angularSpeed *= 0;
			}
		}
		//左に回転する
		else if(prs.bal_y - prs.bal_x > 0 && prs.bal_y + prs.bal_x < 0 )
		{
			angularSpeed += mMaxAngularSpeed;
			if(prs.bal_y >= 0){
				angularSpeed *= ((Math::Pi * 19 / 45)  - std::atan2(prs.bal_y,prs.bal_x * -1) ) * 0.15;
			}
			else if(std::atan2(prs.bal_y * -1, prs.bal_x * -1) <= Math::Pi / 24  && prs.bal_y < 0){
				angularSpeed *= ((Math::Pi * 19 / 45) + std::atan2(prs.bal_y * -1 ,prs.bal_x * -1))  * 0.15;
			}
			else{
				angularSpeed *= 0;
			}
		}
		else{}
		SetAngularSpeed(angularSpeed);
	}
}
