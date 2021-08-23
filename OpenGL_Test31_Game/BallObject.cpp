//
//  BallObject.cpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/23.
//

#include "BallObject.hpp"

BallObject::BallObject()
    : GameObject(), Radius(12.5f), Stuck(true)  { }

BallObject::BallObject(vec2 pos, float radius, vec2 velocity, Texture2D sprite)
    :  GameObject(pos, vec2(radius * 2, radius * 2), sprite, vec3(1.0f), velocity), Radius(radius), Stuck(true) { }

vec2 BallObject::Move(float dt, unsigned int window_width)
{
    // 如果没有被固定在挡板上
    if (!this->Stuck)
    {
        // 移动球
        this->Position += this->Velocity * dt;
        // 检查是否在窗口边界以外，如果是的话反转速度并恢复到正确的位置
        if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if (this->Position.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }

    }
    return this->Position;
}

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(vec2 position, vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
}
