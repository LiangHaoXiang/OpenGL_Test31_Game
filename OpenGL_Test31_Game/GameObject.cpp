//
//  GameObject.cpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/22.
//

#include "GameObject.hpp"


GameObject::GameObject()
    : Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(vec2 pos, vec2 size, Texture2D sprite, vec3 color, vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) { }

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
