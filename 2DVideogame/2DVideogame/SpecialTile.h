#ifndef _SPECIAL_TILE_INCLUDE
#define _SPECIAL_TILE_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class SpecialTile
{

public:
	void init(const glm::vec2 &pos, const string &filename, ShaderProgram &shaderProgram, int tileSize, const glm::vec2 &spritesheetSize);
	void update(int deltaTime);
	void render();

	//void setTileMap(TileMap *tileMap);
	void setTexPosition(const glm::vec2 &texturePos);

private:
	glm::ivec2 posTile, spriteSheetSize;
	Texture spritesheet;
	Sprite *sprite;
	//TileMap *map;
};


#endif // _SPECIAL_TILE_INCLUDE