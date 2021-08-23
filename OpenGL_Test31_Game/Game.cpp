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
#include "BallObject.hpp"

using namespace glm;

SpriteRenderer* Renderer;
// 初始化挡板的大小
const vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const float PLAYER_VELOCITY(500.0f);

GameObject *Player;

// 初始化球的速度
const vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const float BALL_RADIUS = 12.5f;

BallObject *Ball;

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
    
    //初始化玩家挡板
    vec2 playerPos = vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    
    //初始化球
    vec2 ballPos = playerPos + vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
}

void Game::Update(float dt)
{
    // 更新对象
    Ball->Move(dt, this->Width);
    // 检测碰撞
    this->DoCollisions();
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
        if (this->Keys[GLFW_KEY_SPACE])
        {
            Ball->Stuck = false;
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
        // 绘制球
        Ball->Draw(*Renderer);
    }
}

//碰撞检测 AABB - AABB collision
//bool CheckCollision(GameObject &one, GameObject &two)
//{
//    // x轴方向碰撞？
//    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
//        two.Position.x + two.Size.x >= one.Position.x;
//    // y轴方向碰撞？
//    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
//        two.Position.y + two.Size.y >= one.Position.y;
//    // 只有两个轴向都有碰撞时才碰撞
//    return collisionX && collisionY;
//}
bool CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
    // 获取圆的中心
    vec2 center(one.Position + one.Radius);
    // 计算AABB的信息（中心、半边长）
    vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // 获取两个中心的差矢量
    vec2 difference = center - aabb_center;
    vec2 clamped = clamp(difference, -aabb_half_extents, aabb_half_extents);
    // AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
    vec2 closest = aabb_center + clamped;
    // 获得圆心center和最近点closest的矢量并判断是否 length <= radius
    difference = closest - center;
    return length(difference) < one.Radius;
}

void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level - 1].Bricks)
    {
        if (!box.Destroyed)
        {
            if (CheckCollision(*Ball, box))
            {
                if (!box.IsSolid)
                {
                    box.Destroyed = true;
                }
            }
        }
    }
}
