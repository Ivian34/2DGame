#include "MenuScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>


struct LineSegment {
    glm::vec2 start;
    glm::vec2 end;
};

std::vector<LineSegment> generateLightning(glm::vec2 start, glm::vec2 end, int numSegments) {
    std::vector<LineSegment> lightning;
    glm::vec2 current = start;
    for (int i = 0; i < numSegments; ++i) {
        // Generar un pequeño desplazamiento aleatorio
        glm::vec2 offset(rand() % 20 - 10, rand() % 20 - 10);

        // Calcular el siguiente punto interpolado en la línea del rayo
        glm::vec2 next = glm::mix(current, end, (float)(i + 1) / numSegments) + offset;

        // Asegurarse de que el próximo punto esté dentro de la pantalla
        next.x = glm::clamp(next.x, 0.0f, float(SCENE_WIDTH));
        next.y = glm::clamp(next.y, 0.0f, float(SCENE_HEIGHT));

        // Agregar el segmento a la lista
        lightning.push_back({ current, next });
        current = next;
    }
    return lightning;
}


void MenuScene::init()
{
    // Inicializar la matriz de proyección
    projection = glm::ortho(0.0f, float(SCENE_WIDTH), float(SCENE_HEIGHT), 0.0f);
	menuBackGround.loadFromFile("images/menu3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	press.loadFromFile("images/press.png", TEXTURE_PIXEL_FORMAT_RGBA);
	currentTime = 0.0f;

	initShaders();
    glClearColor(0.4f, 0.7f, 7.0f, 1.0f);
	initSquare(glm::vec2(0.0f, SCENE_HEIGHT), glm::vec2(SCENE_WIDTH, SCENE_HEIGHT), glm::vec2(SCENE_WIDTH, 0.f), glm::vec2(0.f, 0.f)); //Fondo del menú
	initSquare(glm::vec2((1.f /4) * SCENE_WIDTH, (4.f/5) *  SCENE_HEIGHT ), glm::vec2(3.f /4 * SCENE_WIDTH, 4.f / 5 * SCENE_HEIGHT), glm::vec2(3.f /4 * SCENE_WIDTH, 1.f / 2 * SCENE_HEIGHT), glm::vec2(1.f / 4 * SCENE_WIDTH, 1.f / 2 * SCENE_HEIGHT)); //letras del menú

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.use();
    simpleProgram.setUniformMatrix4f("projection", projection);
    // Inicializa shaders, texturas y otros recursos necesarios
}

void MenuScene::update(int deltaTime)
{
    currentTime += deltaTime;
    // Actualiza la lógica del menú, como animaciones o selección de opciones
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

    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.f);


    glEnable(GL_TEXTURE_2D);
    menuBackGround.use();
    glBindVertexArray(quads[0].VAO);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisable(GL_TEXTURE_2D);

    int tot = 1800;
    int aux = int(currentTime) % tot;
    if (aux > 0 && aux < (tot/2)) {
        glEnable(GL_TEXTURE_2D);
        press.use();
        glBindVertexArray(quads[1].VAO);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisable(GL_TEXTURE_2D);
    }

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

    // Shader para la hitbox (nuevo)
    Shader simpleVShader, simpleFShader;

    simpleVShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
    if (!simpleVShader.isCompiled())
    {
        cout << "Hitbox Vertex Shader Error" << endl;
        cout << "" << simpleVShader.log() << endl << endl;
    }
    simpleFShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
    if (!simpleFShader.isCompiled())
    {
        cout << "Hitbox Fragment Shader Error" << endl;
        cout << "" << simpleFShader.log() << endl << endl;
    }
    simpleProgram.init();
    simpleProgram.addShader(simpleVShader);
    simpleProgram.addShader(simpleFShader);
    simpleProgram.link();
    if (!simpleProgram.isLinked())
    {
        cout << "Hitbox Shader Linking Error" << endl;
        cout << "" << simpleProgram.log() << endl << endl;
    }
    simpleProgram.bindFragmentOutput("outColor");
    simpleVShader.free();
    simpleFShader.free();
}
