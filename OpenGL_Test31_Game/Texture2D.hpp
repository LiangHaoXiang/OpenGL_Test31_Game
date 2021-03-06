//
//  Texture2D.hpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/20.
//

#ifndef Texture2D_hpp
#define Texture2D_hpp

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D
{
public:
    // Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    unsigned int ID;
    // Texture image dimensions
    unsigned int Width, Height; // Width and height of loaded image in pixels
    // Texture Format
    unsigned int Internal_Format; // Format of texture object
    unsigned int Image_Format; // Format of loaded image
    // Texture configuration
    unsigned int Wrap_S; // Wrapping mode on S axis
    unsigned int Wrap_T; // Wrapping mode on T axis
    unsigned int Filter_Min; // Filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // Filtering mode if texture pixels > screen pixels
    // Constructor (sets default texture modes)
    Texture2D();
    // Generates texture from image data
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};
#endif /* Texture2D_hpp */
