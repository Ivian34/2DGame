#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "Sprite.h"

class TileMap;
class Object
{

public:
	void init(const glm::vec2 &pos, const string &filename, ShaderProgram &shaderProgram, int tileSize, const glm::vec2 &spritesheetSize, const glm::vec2 &spritesheetDispl);
	void update(int deltaTime);
	void render() const;

	void setTileMap(TileMap *tileMap);
	void setTexPosition(const glm::vec2 &texturePos);
	void setPos(const glm::vec2 &pos);

	glm::ivec2 getPosition() const;
	int getSize() const;
	bool canBeMoved(int yPos) const;

private:
	glm::ivec2 posObj, spriteSheetSize, spriteDispl;
	Texture spritesheet;
	Sprite *sprite;
	int objSize;
	TileMap *map;
};


#endif // _OBJECT_INCLUDE