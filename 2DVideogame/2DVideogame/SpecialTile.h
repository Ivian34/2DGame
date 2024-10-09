#ifndef _SPECIAL_TILE_INCLUDE
#define _SPECIAL_TILE_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class SpecialTile
{

public:
	SpecialTile(const glm::vec2 &pos, const string &filename, ShaderProgram &shaderProgram, const glm::vec2 &tileSize, const glm::vec2 &sizeInSpritesheet);

	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setTexPosition(const glm::vec2 &texturePos);
	~SpecialTile();

private:
	glm::ivec2 posTile;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};


#endif // _SPECIAL_TILE_INCLUDE