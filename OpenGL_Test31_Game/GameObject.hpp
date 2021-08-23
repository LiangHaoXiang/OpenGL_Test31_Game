//
//  GameObject.hpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/22.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include "Texture2D.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SpriteRenderer.hpp"

using namespace glm;

class GameObject
{
public:
    // Object state
    vec2   Position, Size, Velocity;
    vec3   Color;
    float  Rotation;
    bool   IsSolid;
    bool   Destroyed;
    // Render state
    Texture2D   Sprite;
    // Constructor(s)
    GameObject();
    GameObject(vec2 pos, vec2 size, Texture2D sprite, vec3 color = vec3(1.0f), vec2 velocity = vec2(0.0f, 0.0f));
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer);
};

#endif /* GameObject_hpp */
