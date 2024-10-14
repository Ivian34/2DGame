#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include "Object.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 40


enum PlayerAnims
{
	STAND, MOVE, CROUCH, JUMP, FALL
};

enum PlayerCollisions
{
	OBJH, OBJD, ENEMY, NCOLLISIONS
};

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/MM.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), glm::vec2(float(PLAYER_WIDTH) / 488.f, float(PLAYER_HEIGHT) / 488.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5); // Solo STAND y MOVE

	// Animación STAND
	sprite->setAnimationSpeed(STAND, 8);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND, glm::vec2(32.f / 488.f, 0.f));

	// Animación MOVE
	sprite->setAnimationSpeed(MOVE, 8);
	sprite->addKeyframe(MOVE, glm::vec2(64.f / 488.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(96.f / 488.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(128.f / 488.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(160.f / 488.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(192.f / 488.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(224.f / 488.f, 0.f));

	// Animación CROUCH
	sprite->setAnimationSpeed(CROUCH, 8);
	sprite->addKeyframe(CROUCH, glm::vec2(0.f, 48.f / 488.f));
	sprite->addKeyframe(CROUCH, glm::vec2(33.f / 488.f, 48.f / 488.f));

	//Jump animation
	sprite->setAnimationSpeed(JUMP, 8);
	sprite->addKeyframe(JUMP, glm::vec2(64.f / 488.f, 48.f / 488.f));

	sprite->setAnimationSpeed(FALL, 8);
	sprite->addKeyframe(FALL, glm::vec2(96.f / 488.f, 48.f / 488.f));



	sprite->changeAnimation(STAND);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


void Player::update(int deltaTime)
{
	for (int i = 0; i < NCOLLISIONS; ++i) collisions[i] = false;
	lastInteractableObj = nullptr;

	sprite->update(deltaTime);
	if(Game::instance().getKey(GLFW_KEY_LEFT))
	{
		facingLeft = true;
		sprite->setScale(glm::vec2(-1.f, 1.f)); // Voltear horizontalmente
		if (sprite->animation() != MOVE)
			sprite->changeAnimation(MOVE);
		posPlayer.x -= 3;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &collisions[OBJH], lastInteractableObj))
		{
			posPlayer.x += 3;
			sprite->changeAnimation(STAND);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		facingLeft = false;
		sprite->setScale(glm::vec2(1.f, 1.f)); // Escala normal
		if (sprite->animation() != MOVE)
			sprite->changeAnimation(MOVE);
		posPlayer.x += 3;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &collisions[OBJH], lastInteractableObj))
		{
			posPlayer.x -= 3;
			sprite->changeAnimation(STAND);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_DOWN))
	{
		if (sprite->animation() != CROUCH)
			sprite->changeAnimation(CROUCH);

	}
	else
	{
		if (sprite->animation() != STAND)
			sprite->changeAnimation(STAND);
	}
	
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
			{ // Falling
				if (sprite->animation() != FALL)
					sprite->changeAnimation(FALL);
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y, &collisions[OBJD], lastInteractableObj);
			}
			else
			{
				if (sprite->animation() != JUMP)
					sprite->changeAnimation(JUMP);
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y, &collisions[OBJD], lastInteractableObj))
		{
			if(Game::instance().getKey(GLFW_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
			else if (Game::instance().getKey(GLFW_KEY_Z) && !carryObj) {
				if (lastInteractableObj != nullptr && collisions[OBJH] && lastInteractableObj->canBeMoved(posPlayer.y + PLAYER_HEIGHT)) {
					carryObj = true;
					currentCarryObj = lastInteractableObj;
					currentCarryObj->setPos(glm::vec2(posPlayer.x, posPlayer.y - currentCarryObj->getSize()));
					currentCarryObj->setHeld();
				}
			}
		}
	}

	if (carryObj) {
		currentCarryObj->setPos(glm::vec2(posPlayer.x, posPlayer.y - currentCarryObj->getSize()));
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));	
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}


