//
//  Game.cpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/20.
//

#include "Game.hpp"
#include "SpriteRenderer.hpp"
#include "ResourceManager.hpp"

using namespace glm;

SpriteRenderer* Renderer;

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
    string p3 = "/Users/haoxiangliang/Desktop/未命名文件夹/awesomeface.png";
    ResourceManager::LoadTexture(p3.c_str(), GL_TRUE, "face");
    Renderer = new SpriteRenderer(shader);
}

void Game::Update(float dt)
{
    
}

void Game::ProcessInput(float dt)
{
    
}

void Game::Render()
{
    Texture2D faceTexture = ResourceManager::GetTexture("face");
    Renderer->DrawSprite(faceTexture, vec2(200, 200), vec2(300, 400), 45.0f, vec3(0.0f, 1.0f, 0.0f));
}
