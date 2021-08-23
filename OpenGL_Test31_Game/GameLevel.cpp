//
//  GameLevel.cpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/23.
//

#include "GameLevel.hpp"
#include <fstream>
#include <sstream>
//#include <iostream>
#include "ResourceManager.hpp"

void GameLevel::Load(string path, unsigned int levelWidth, unsigned int levelHeight)
{
    const char* file = path.c_str();
    // 清空过期数据
    this->Bricks.clear();
    // 从文件中加载
    unsigned int tileCode;
    GameLevel level;
    string line;
    ifstream fstream(file);
    vector<vector<unsigned int>> tileData;
    if(fstream)
    {
        // 读取关卡文件的每一行
        while (getline(fstream, line))
        {
            istringstream sstream(line);
            vector<unsigned int> row;
            // 读取被空格分隔的每个数字
            while (sstream >> tileCode) {
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }
        if(tileData.size() > 0)
        {
            this->init(tileData, levelWidth, levelHeight);
        }
    }
}

void GameLevel::init(vector<vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // 计算每个维度的大小
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / height;
    // 基于tileDataC初始化关卡
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // 检查砖块类型
            if (tileData[y][x] == 1)
            {
                vec2 pos(unit_width * x, unit_height * y);
                vec2 size(unit_width, unit_height);
                GameObject obj(pos, size,
                    ResourceManager::GetTexture("block_solid"),
                    vec3(0.8f, 0.8f, 0.7f)
                );
                obj.IsSolid = GL_TRUE;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)
            {
                vec3 color = vec3(1.0f); // 默认为白色
                if (tileData[y][x] == 2)
                    color = vec3(0.2f, 0.6f, 1.0f);//蓝色
                else if (tileData[y][x] == 3)
                    color = vec3(0.0f, 0.7f, 0.0f);//绿色
                else if (tileData[y][x] == 4)
                    color = vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = vec3(1.0f, 0.5f, 0.0f);//橙色

                vec2 pos(unit_width * x, unit_height * y);
                vec2 size(unit_width, unit_height);
                this->Bricks.push_back(
                    GameObject(pos, size, ResourceManager::GetTexture("block"), color)
                );
            }
        }
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Bricks)
    {
        if (!tile.Destroyed)
        {
            tile.Draw(renderer);
        }
    }
}

bool GameLevel::IsCompleted()
{
    for (GameObject &tile : this->Bricks)
    {
        if (!tile.IsSolid && !tile.Destroyed)
        {
            return false;
        }
    }
    return true;
}
