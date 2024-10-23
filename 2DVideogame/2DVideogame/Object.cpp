#include <iostream>
#include "Object.h"
#include "TileMap.h"


#define THROW_ANGLE_STEP 4
#define THROW_STEP 4
#define INIT_THROW_ANGLE 70


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
	sprite = Sprite::createSprite(glm::ivec2(tileSize), glm::vec2(float(tileSize) / (spritesheetSize.x*tileSize), float(tileSize) / (spritesheetSize.y*tileSize)), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS); // BASE y DESTROY
	sprite->setPosition(glm::ivec2(posObj.x + spriteDispl.x, posObj.y + spriteDispl.y));
}


void Object::update(int deltaTime)
{
	//sprite->update(deltaTime);
	if (objState == ObjectStates::MOVING) {
		posObj = glm::ivec2(posObj.x + velObj.x, posObj.y);

		Object *collidedObj = nullptr;
		if ((velObj.x < 0) && map->collisionStaticHorizontal(posObj, glm::ivec2(objSize))) {
			objState = ObjectStates::INACTIVE;
		}

		if (bThrow) {
			throwAngle += THROW_ANGLE_STEP;
			if (throwAngle == 180) {
				bThrow = false;
			}
			else {
				posObj.y = int(startY - 96 * sin(3.14159f * throwAngle / 180.0f));
			}
		}
		else posObj.y += THROW_STEP;
		if (map->collisionStaticDown(posObj, glm::ivec2(objSize))) {
			objState = ObjectStates::INACTIVE;
		}

		sprite->setPosition(glm::ivec2(posObj.x + spriteDispl.x, posObj.y + spriteDispl.y));
	}
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
	sprite->addKeyframe(BASE, glm::vec2(texturePos.x / spriteSheetSize.x, texturePos.y / spriteSheetSize.y));
	sprite->changeAnimation(BASE);
}

void Object::setPos(const glm::vec2 &pos) {
	posObj = pos;
	sprite->setPosition(glm::ivec2(posObj.x + spriteDispl.x, posObj.y + spriteDispl.y));
}

void Object::setVelocity(const glm::vec2 & v)
{
	velObj = v;
}

glm::ivec2 Object::getPosition() const {
	return posObj;
}

int Object::getSize() const {
	return objSize;
}

bool Object::canBeMoved(int yPos) const
{
	if (isInteractible() && yPos != (posObj.y + objSize)) {

		return false;
	}
	return !map->collisionStaticUp(glm::vec2(posObj.x, posObj.y - 1), glm::ivec2(objSize));
}

void Object::throwObject(const glm::vec2 & v)
{
	setMoving();
	bThrow = true;
	setVelocity(v);
	throwAngle = INIT_THROW_ANGLE;
	startY = posObj.y + int(96 * sin(3.14159f * throwAngle / 180.0f));
}

bool Object::isActive() const
{
	return objState != ObjectStates::INACTIVE;
}

bool Object::isInteractible() const
{
	return objState == ObjectStates::INTERACTABLE;
}

bool Object::canCollide() const
{
	return objState == ObjectStates::INTERACTABLE || objState == ObjectStates::STATIC;
}

void Object::setInteractable()
{
	objState = ObjectStates::INTERACTABLE;
}

void Object::setHeld()
{
	objState = ObjectStates::HELD;
}

void Object::setMoving()
{
	objState = ObjectStates::MOVING;
}

