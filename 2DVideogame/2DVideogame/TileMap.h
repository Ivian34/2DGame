#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class Object;

class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void update(int deltaTime);
	void free();

	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, bool *collision, int *posX, Object*& interactedObj) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, bool *collision, int *posX, Object*& interactedObj) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int object_height, int *posY, bool *collision, Object*& interactedObj) const;

	bool collisionStaticHorizontal(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionStaticUp(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionStaticDown(const glm::ivec2 &pos, const glm::ivec2 &size) const;

	void createItem(const glm::ivec2 & pos, const string & type, int itemSize, const glm::vec2 &spritesheetSize, const glm::vec2 &spritesheetDispl);

private:
	bool loadLevel(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao, backVao;
	GLuint vbo, backVbo;
	GLint posLocation, texCoordLocation, backPosLocation, backTexCoordLocation;
	int nTiles, backNTiles;
	glm::ivec2 position, mapSize, tilesheetSize, backTilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet, backTilesheet;
	glm::vec2 tileTexSize, backTileTexSize;
	int *map, *backMap;

	vector<Object*> objects, items;
	ShaderProgram *texProgram;

	//Level bools
	bool hasBackmap = false;
};


#endif // _TILE_MAP_INCLUDE

