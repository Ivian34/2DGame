#include "MenuScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>


void MenuScene::init(TextRenderer &tr1, TextRenderer &tr2)
{
    // Inicializar la matriz de proyección
    projection = glm::ortho(0.0f, float(SCENE_WIDTH), float(SCENE_HEIGHT), 0.0f);
	menuBackGround.loadFromFile("images/menu3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	currentTime = 0.0f;

	initShaders();
    glClearColor(0.4f, 0.7f, 7.0f, 1.0f);
	initSquare(glm::vec2(0.0f, SCENE_HEIGHT), glm::vec2(SCENE_WIDTH, SCENE_HEIGHT), glm::vec2(SCENE_WIDTH, 0.f), glm::vec2(0.f, 0.f)); //Fondo del menú

	textRenderer = &tr1;
	textRenderer2 = &tr2;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);

    // Inicializa shaders, texturas y otros recursos necesarios
}

void MenuScene::update(int deltaTime)
{
    currentTime += deltaTime;
    // Actualiza la lógica del menú, como animaciones o selección de opciones
}

ShaderProgram& MenuScene::getShaderProgram()
{
	return texProgram;
}


void MenuScene::initSquare(const glm::vec2& esquina_ii, const glm::vec2& esquina_id, const glm::vec2& esquina_sd, const glm::vec2& esquina_si) {
    // Definir los vértices del cuadrado
    std::vector<float> vertices = {
        esquina_ii.x, esquina_ii.y ,   0.0f, 1.0f,   // Esquina inferior izquierda
        esquina_id.x ,esquina_id.y,    1.0f, 1.0f,   // Esquina inferior derecha
        esquina_sd.x, esquina_sd.y,    1.0f, 0.0f,   // Esquina superior derecha
        esquina_si.x, esquina_si.y,    0.0f, 0.0f    // Esquina superior izquierda
    };
    verticesList.push_back(vertices); // Almacenar vértices del quad

    // Crear y configurar VAO y VBO para el cuadrado
    Quad quad;
    glGenVertexArrays(1, &quad.VAO);
    glGenBuffers(1, &quad.VBO);

    glBindVertexArray(quad.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, quad.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Almacena el nuevo quad
    quads.push_back(quad);
}

void MenuScene::renderQuads() {
    for (const auto& quad : quads) {
        glBindVertexArray(quad.VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    glBindVertexArray(0);
}


void MenuScene::render()
{

    texProgram.use();


    glm::mat4 modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);

    texProgram.setUniformMatrix4f("projection", projection);

    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

    texProgram.setUniform4f("color", 1.0f, 1.f, 1.0f, 1.f);


    glEnable(GL_TEXTURE_2D);
    menuBackGround.use();
    glBindVertexArray(quads[0].VAO);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisable(GL_TEXTURE_2D);

    int tot = 1800;
    int aux = int(currentTime) % tot;
    if (aux > 0 && aux < (tot/2)) {
        textRenderer->renderText("Press Enter to start", 0.25 * SCENE_WIDTH, 0.41 * SCENE_HEIGHT, 1.f, glm::vec4(1.f, 1.f, 1.f, 1.f));
    }

    textRenderer2->renderText("Press i anywhere", 0.34 * SCENE_WIDTH, 0.15 * SCENE_HEIGHT, 1.f, glm::vec4(.6, .6, 0.f, 1.f));
    textRenderer2->renderText("for instructions", 0.34 * SCENE_WIDTH, 0.10 * SCENE_HEIGHT, 1.f, glm::vec4(.6, .6, 0.f, 1.f));

    glBindVertexArray(0);
}



void MenuScene::initShaders()
{
    Shader vShader, fShader;

    // Shader para texturas (el existente)
    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    if (!vShader.isCompiled())
    {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    if (!fShader.isCompiled())
    {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if (!texProgram.isLinked())
    {
        cout << "Shader Linking Error" << endl;
        cout << "" << texProgram.log() << endl << endl;
    }
    texProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();

}
