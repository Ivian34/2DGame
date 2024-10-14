#include <iostream>
#include "Object.h"
#include "TileMap.h"

enum TileAnims
{
	BASE, DESTROY, NUM_ANIMS
};

void Object::init(const glm::vec2 &pos, const string &filename, ShaderProgram &shaderProgram, int tileSize, const glm::vec2 &spritesheetSize, const glm::vec2 &spritesheetDispl)
{
	posObj = pos;
	objSize = tileSize;
	spriteSheetSize = spritesheetSize;
	spriteDispl = spritesheetDispl;

	spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(tileSize), glm::vec2(float(tileSize)/(spritesheetSize.x*tileSize), float(tileSize) / (spritesheetSize.y*tileSize)), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS); // BASE y DESTROY
	sprite->setPosition(glm::ivec2(posObj.x + spriteDispl.x, posObj.y + spriteDispl.y));
}


void Object::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Object::render() const
{
	sprite->render();
}

 void Object::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Object::setTexPosition(const glm::vec2 & texturePos)
{
	sprite->addKeyframe(BASE, glm::vec2(texturePos.x/spriteSheetSize.x, texturePos.y/spriteSheetSize.y));
	sprite->changeAnimation(BASE);
}

void Object::setPos(const glm::vec2 &pos) {
	posObj = pos;
	sprite->setPosition(glm::ivec2(posObj.x + spriteDispl.x, posObj.y + spriteDispl.y));
}

glm::ivec2 Object::getPosition() const {
	return posObj;
}

int Object::getSize() const {
	return objSize;
}

bool Object::canBeMoved(int yPos) const
{
	if (yPos != (posObj.y + objSize)) {
		
		return false;
	}
	return !map->collisionStaticUp(glm::vec2(posObj.x, posObj.y - 1), glm::ivec2(objSize));
}

