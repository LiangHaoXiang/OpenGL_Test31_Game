//
//  ResourceManager.hpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/20.
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include <stdio.h>
#include <iostream>
#include <map>
#include "Texture2D.hpp"
#include "Shader.hpp"

using namespace std;
// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager
{
public:
    // Resource storage
    static map<string, Shader>    Shaders;
    static map<string, Texture2D> Textures;
    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader   LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    // Retrieves a stored sader
    static Shader   GetShader(string name);
    // Loads (and generates) a texture from file
    static Texture2D LoadTexture(string path, bool alpha, string name);
    // Retrieves a stored texture
    static Texture2D GetTexture(string name);
    // Properly de-allocates all loaded resources
    static void      Clear();
private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // Loads and generates a shader from file
    static Shader    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    // Loads a single texture from file
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif /* ResourceManager_hpp */
