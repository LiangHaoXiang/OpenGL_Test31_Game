//
//  Game.cpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/20.
//

#include "Game.hpp"
#include "SpriteRenderer.hpp"
#include "ResourceManager.hpp"
#include <GLFW/glfw3.h>

using namespace glm;

SpriteRenderer* Renderer;
// 初始化挡板的大小
const vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const float PLAYER_VELOCITY(500.0f);

GameObject *Player;

Game::Game(int width, int height): State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    
}

Game::~Game()
{
    
}

void Game::Init()
{
    string p1 = "/Users/haoxiangliang/Desktop/代码草稿/OpenGL/OpenGL_Test31_Game/OpenGL_Test31_Game/Shaders/sprite.vs";
    string p2 = "/Users/haoxiangliang/Desktop/代码草稿/OpenGL/OpenGL_Test31_Game/OpenGL_Test31_Game/Shaders/sprite.fs";
    Shader shader = ResourceManager::LoadShader(p1.c_str(), p2.c_str(), nullptr, "sprite");
    mat4 projection = ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    shader.Use();
    shader.SetInteger("image", 0);
    shader.SetMatrix4("projection", projection);
    //加载纹理
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/paddle.png", true, "paddle");
    Renderer = new SpriteRenderer(shader);
    
    //加载关卡
    GameLevel lv1, lv2, lv3, lv4;
    lv1.Load("/Users/haoxiangliang/Desktop/未命名文件夹/GameLevel/gameLv_1.txt", this->Width, this->Height * 0.5);
    lv2.Load("/Users/haoxiangliang/Desktop/未命名文件夹/GameLevel/gameLv_2.txt", this->Width, this->Height * 0.5);
    lv3.Load("/Users/haoxiangliang/Desktop/未命名文件夹/GameLevel/gameLv_3.txt", this->Width, this->Height * 0.5);
    lv4.Load("/Users/haoxiangliang/Desktop/未命名文件夹/GameLevel/gameLv_4.txt", this->Width, this->Height * 0.5);
    this->Levels.push_back(lv1);
    this->Levels.push_back(lv2);
    this->Levels.push_back(lv3);
    this->Levels.push_back(lv4);
    this->Level = 1;
    
    vec2 playerPos = vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt)
{
    
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // 移动挡板
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
    }
}

void Game::Render()
{
//    Texture2D faceTexture = ResourceManager::GetTexture("face");
//    Renderer->DrawSprite(faceTexture, vec2(200, 200), vec2(300, 400), 45.0f, vec3(0.0f, 1.0f, 0.0f));
    if (this->State == GAME_ACTIVE)
    {
        // 绘制背景
        Texture2D bgTexture = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(bgTexture, vec2(0, 0), vec2(this->Width, this->Height), 0.0f);
        // 绘制关卡
        this->Levels[this->Level - 1].Draw(*Renderer);
        // 绘制玩家挡板
        Player->Draw(*Renderer);
    }
}
