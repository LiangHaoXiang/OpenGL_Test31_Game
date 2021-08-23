//
//  Game.hpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/20.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include "GameLevel.hpp"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
public:
    //游戏状态
    GameState State;
    bool Keys[1024];
    int Width, Height;
    
    vector<GameLevel> Levels;
    unsigned int Level;

    // 构造函数/析构函数
    Game(int width, int height);
    ~Game();
    // 初始化游戏状态（加载所有的着色器/纹理/关卡）
    void Init();
    // 游戏循环
    void Update(float dt);
    void ProcessInput(float dt);
    void Render();
    //一堆碰撞检测 AABB - AABB collision
    void DoCollisions();
    // Reset
    void ResetLevel();
    void ResetPlayer();
};

#endif /* Game_hpp */
