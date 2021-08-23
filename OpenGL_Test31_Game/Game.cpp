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
#include "ParticleGenerator.hpp"
#include "PostProcessor.hpp"

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
ParticleGenerator *Particles;
PostProcessor *Effects;

float ShakeTime = 0.0f;

//球体碰撞砖块的方向
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef tuple<bool, Direction, vec2> Collision;
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(vec2 target);

Game::Game(int width, int height): State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    
}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
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
    
    string p3 = "/Users/haoxiangliang/Desktop/代码草稿/OpenGL/OpenGL_Test31_Game/OpenGL_Test31_Game/Shaders/particle.vs";
    string p4 = "/Users/haoxiangliang/Desktop/代码草稿/OpenGL/OpenGL_Test31_Game/OpenGL_Test31_Game/Shaders/particle.fs";
    Shader particleShader = ResourceManager::LoadShader(p3.c_str(), p4.c_str(), nullptr, "particle");
    particleShader.Use();
    particleShader.SetMatrix4("projection", projection);
    
    string p5 = "/Users/haoxiangliang/Desktop/代码草稿/OpenGL/OpenGL_Test31_Game/OpenGL_Test31_Game/Shaders/post_processing.vs";
    string p6 = "/Users/haoxiangliang/Desktop/代码草稿/OpenGL/OpenGL_Test31_Game/OpenGL_Test31_Game/Shaders/post_processing.fs";
    Shader processingShader = ResourceManager::LoadShader(p5.c_str(), p6.c_str(), nullptr, "postprocessing");
    
    //加载纹理
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/paddle.png", true, "paddle");
    Renderer = new SpriteRenderer(shader);
    
    ResourceManager::LoadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/particle.png", true, "particle");
    Particles = new ParticleGenerator(particleShader, ResourceManager::GetTexture("particle"), 500);
    
    Effects = new PostProcessor(processingShader, this->Width * 2, this->Height * 2);
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
    this->Level = 3;
    
    //初始化玩家挡板
    vec2 playerPos = vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    
    //初始化球
    vec2 ballPos = playerPos + vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
}

void Game::ResetLevel()
{
    this->Levels[this->Level - 1].Load("/Users/haoxiangliang/Desktop/未命名文件夹/GameLevel/gameLv_" + to_string(this->Level) + ".txt", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Position = vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}

void Game::Update(float dt)
{
    // 更新对象
    Ball->Move(dt, this->Width);
    // Update particles
    Particles->Update(dt, *Ball, 2, vec2(Ball->Radius / 2));
    // 检测碰撞
    this->DoCollisions();
    // 球是否接触底部边界？
    if (Ball->Position.y >= this->Height)
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
    
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
        {
            Effects->Shake = false;
        }
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // 移动挡板
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0)
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                {
                    Ball->Position.x -= velocity;
                }
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                {
                    Ball->Position.x += velocity;
                }
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
        {
            Ball->Stuck = false;
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        Effects->BeginRender();
        // 绘制背景
        Texture2D bgTexture = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(bgTexture, vec2(0, 0), vec2(this->Width, this->Height), 0.0f);
        // 绘制关卡
        this->Levels[this->Level - 1].Draw(*Renderer);
        // 绘制玩家挡板
        Player->Draw(*Renderer);
        // 绘制粒子特效
        Particles->Draw();
        // 绘制球
        Ball->Draw(*Renderer);
        Effects->EndRender();
        Effects->Render(glfwGetTime());
    }
}

void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level - 1].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // 如果collision 是 true
            {
                // 如果砖块不是实心就销毁砖块
                if (!box.IsSolid)
                {
                    box.Destroyed = true;
                }
                else
                {
                    // 如果是实心的砖块则激活shake特效
                    ShakeTime = 0.05f;
                    Effects->Shake = true;
                }
                // 碰撞处理
                Direction dir = std::get<1>(collision);
                vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) // 水平方向碰撞
                {
                    Ball->Velocity.x = -Ball->Velocity.x; // 反转水平速度
                    // 重定位
                    float penetration = Ball->Radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        Ball->Position.x += penetration; // 将球右移
                    else
                        Ball->Position.x -= penetration; // 将球左移
                }
                else // 垂直方向碰撞
                {
                    Ball->Velocity.y = -Ball->Velocity.y; // 反转垂直速度
                    // 重定位
                    float penetration = Ball->Radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        Ball->Position.y -= penetration; // 将球上移
                    else
                        Ball->Position.y += penetration; // 将球下移
                }
            }
        }
    }
    
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
        float centerBoard = Player->Position.x + Player->Size.x / 2;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->Size.x / 2);
        // 依据结果移动
        float strength = 2.0f;
        vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->Velocity.y = -1 * std::abs(Ball->Velocity.y);
        Ball->Velocity = normalize(Ball->Velocity) * length(oldVelocity);
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
Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
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
    if (length(difference) <= one.Radius)
    {
        return make_tuple(true, VectorDirection(difference), difference);
    }
    else
    {
        return make_tuple(false, UP, vec2(0.0f));
    }
}

Direction VectorDirection(vec2 target)
{
    vec2 compass[] = {
        vec2(0.0f, 1.0f),  // 上
        vec2(1.0f, 0.0f),  // 右
        vec2(0.0f, -1.0f), // 下
        vec2(-1.0f, 0.0f)  // 左
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = dot(normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}
