#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "Sprite.h"

class TileMap;

enum class ObjectStates { INTERACTABLE, STATIC, HELD, MOVING, DESTROY, INACTIVE, NSTATES };

class Object
{

public:
	void init(const glm::vec2 &pos, const string &filename, ShaderProgram &shaderProgram, int tileSize, const glm::vec2 &spritesheetSize, const glm::vec2 &spritesheetDispl);
	void update(int deltaTime);
	void render() const;

	void setTileMap(TileMap *tileMap);
	void setTexPosition(const glm::vec2 &texturePos);
	void setPos(const glm::vec2 &pos);
	void setVelocity(const glm::vec2 &v);

	glm::ivec2 getPosition() const;
	int getSize() const;
	bool canBeMoved(int yPos) const;

	void throwObject(const glm::vec2 &v);

	//State checks
	bool isActive() const;
	bool isInteractible() const;
	bool canCollide() const;
	bool hasItem();

	//State setters
	void setInteractable();
	void setHeld();
	void setMoving();
	void setDestroy();
	void setContainItem(const string &i);

private:
	glm::ivec2 posObj, spriteSheetSize, spriteDispl;
	glm::vec2 velObj;
	Texture spritesheet;
	Sprite *sprite;
	int objSize, throwAngle, startY;
	TileMap *map;

	string item;

	ObjectStates objState = ObjectStates::STATIC;

	bool bThrow = false;
	bool bItem = false;
};


#endif // _OBJECT_INCLUDE