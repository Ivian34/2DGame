#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "Sprite.h"

class TileMap;

enum class ObjectStates {INTERACTABLE, STATIC, HELD, MOVING, DESTROY, INACTIVE, NSTATES};

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

	//State checks
	bool isActive() const;
	bool isInteractible() const;
	bool canCollide() const;

	//State setters
	void setInteractable();
	void setHeld();

private:
	glm::ivec2 posObj, spriteSheetSize, spriteDispl;
	Texture spritesheet;
	Sprite *sprite;
	int objSize;
	TileMap *map;

	ObjectStates objState = ObjectStates::STATIC;
};


#endif // _OBJECT_INCLUDE