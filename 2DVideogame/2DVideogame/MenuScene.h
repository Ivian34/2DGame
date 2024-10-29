#pragma once
#ifndef _MENU_SCENE_INCLUDE
#define _MENU_SCENE_INCLUDE

#include "ShaderProgram.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>

#define SCENE_WIDTH 352
#define SCENE_HEIGHT 198

struct Quad {
    GLuint VAO;
    GLuint VBO;
};

class MenuScene
{
public:
    void init();
    void update(int deltaTime);
    void render();
	
private:
	void initShaders();
	void initSquare(const glm::vec2 &esquina_ii, const glm::vec2& esquina_id, const glm::vec2& esquina_sd, const glm::vec2& esquina_si);
    void renderQuads();
    glm::mat4 projection;
    ShaderProgram texProgram, simpleProgram;
    float currentTime;
	Texture menuBackGround, press;

    std::vector<Quad> quads;
    std::vector<std::vector<float>> verticesList; // Almacena los vértices de cada quad

};

#endif // _MENU_SCENE_INCLUDE
