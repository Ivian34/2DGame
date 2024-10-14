#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Object
{

public:
	void init(const glm::vec2 &pos, const string &filename, ShaderProgram &shaderProgram, int tileSize, const glm::vec2 &spritesheetSize, const glm::vec2 &spritesheetDispl);
	void update(int deltaTime);
	void render();

	//void setTileMap(TileMap *tileMap);
	void setTexPosition(const glm::vec2 &texturePos);
	glm::ivec2 getPosition() const;
	int getSize() const;

private:
	glm::ivec2 posTile, spriteSheetSize, spriteDispl;
	Texture spritesheet;
	Sprite *sprite;
	int objSize;
	//TileMap *map;
};


#endif // _OBJECT_INCLUDE