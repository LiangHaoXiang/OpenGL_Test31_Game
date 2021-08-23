//
//  ParticleGenerator.hpp
//  OpenGL_Test31_Game
//
//  Created by Haoxiang Liang on 2021/8/23.
//

#ifndef ParticleGenerator_hpp
#define ParticleGenerator_hpp

#include <stdio.h>
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "GameObject.hpp"
#include <vector>

using namespace std;
using namespace glm;

// Represents a single particle and its state
struct Particle {
    vec2 Position, Velocity;
    vec4 Color;
    float Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator
{
public:
    // Constructor
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    // Update all particles
    void Update(float dt, GameObject &object, unsigned int newParticles, vec2 offset = vec2(0.0f, 0.0f));
    // Render all particles
    void Draw();
private:
    // State
    vector<Particle> particles;
    unsigned int amount;
    // Render state
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    // Initializes buffer and vertex attributes
    void init();
    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // Respawns particle
    void respawnParticle(Particle &particle, GameObject &object, vec2 offset = vec2(0.0f, 0.0f));
};

#endif /* ParticleGenerator_hpp */
