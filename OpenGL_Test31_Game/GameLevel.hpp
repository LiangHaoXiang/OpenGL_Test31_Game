//
//  GameLevel.hpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/23.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include <stdio.h>
#include <vector>
#include "GameObject.hpp"

using namespace std;

class GameLevel
{
public:
    vector<GameObject> Bricks;
    
    GameLevel(){ }
    // 从文件中加载关卡
    void Load(string path, unsigned int levelWidth, unsigned int Height);
    // 渲染关卡
    void Draw(SpriteRenderer &renderer);
    // 检查一个关卡是否已完成 (所有非坚硬的瓷砖均被摧毁)
    bool IsCompleted();
private:
    // 由砖块数据初始化关卡
    void init(vector<vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif /* GameLevel_hpp */
