//
//  PostProcessor.hpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/23.
//

#ifndef PostProcessor_hpp
#define PostProcessor_hpp

#include <stdio.h>
#include "Texture2D.hpp"
#include "SpriteRenderer.hpp"
#include "Shader.hpp"

class PostProcessor
{
public:
    // State
    Shader PostProcessingShader;
    Texture2D Texture;
    unsigned int Width, Height;
    // Options
    // Shake: 轻微晃动场景并附加一个微小的模糊效果
    // Confuse: 反转场景中的颜色并颠倒x轴和y轴
    // Chaos: 利用边缘检测卷积核创造有趣的视觉效果，并以圆形旋转动画的形式移动纹理图片，实现“混沌”特效
    bool Confuse, Chaos, Shake;
    // Constructor
    PostProcessor(Shader shader, unsigned int width, unsigned int height);
    // Prepares the postprocessor's framebuffer operations before rendering the game
    void BeginRender();
    // Should be called after rendering the game, so it stores all the rendered data into a texture object
    void EndRender();
    // Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void Render(float time);
private:
    // Render state
    unsigned int MSFBO, FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    unsigned int RBO; // RBO is used for multisampled color buffer
    unsigned int VAO;
    // Initialize quad for rendering postprocessing texture
    void initRenderData();
};

#endif /* PostProcessor_hpp */
