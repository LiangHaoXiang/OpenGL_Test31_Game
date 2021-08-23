//
//  BallObject.hpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/23.
//

#ifndef BallObject_hpp
#define BallObject_hpp

#include <stdio.h>
#include "GameObject.hpp"
class BallObject : public GameObject
{
public:
    // 球的状态
    float   Radius;
    // 球是否固定在挡板上，不是的话可以自由移动
    bool Stuck;

    BallObject();
    BallObject(vec2 pos, float radius, vec2 velocity, Texture2D sprite);

    vec2 Move(float dt, unsigned int window_width);
    void Reset(vec2 position, vec2 velocity);
};

#endif /* BallObject_hpp */
