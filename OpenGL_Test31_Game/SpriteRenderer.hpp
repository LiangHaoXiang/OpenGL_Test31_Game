//
//  SpriteRenderer.hpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/20.
//

#ifndef SpriteRenderer_hpp
#define SpriteRenderer_hpp

#include <stdio.h>
#include "Shader.hpp"
#include "Texture2D.hpp"

using namespace glm;

class SpriteRenderer
{
public:
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();
    
    void DrawSprite(Texture2D &texture, vec2 position, vec2 size = vec2(10, 10), float rotate = 0.0f, vec3 color = vec3(1.0f));
private:
    Shader shader;
    unsigned quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif /* SpriteRenderer_hpp */
