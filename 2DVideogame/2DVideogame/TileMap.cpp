#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Object.h"

using namespace std;

enum PlayerCollisions
{
	OBJH, OBJD, ENEMY, NCOLLISIONS
};

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile, minCoords, program);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);

	for (int i = 0; i < int(objects.size()); ++i) {
		objects[i]->render();
	}
}

void TileMap::update(int deltaTime)
{
	for (int i = 0; i < int(objects.size()); ++i) {
		objects[i]->update(deltaTime);
	}
	auto it = objects.cbegin();
	while (it != objects.cend()) {
		if (!(*it)->isActive()) {
			it = objects.erase(it);
		}
		else ++it;
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j*mapSize.x + i] = 0;
			else
				map[j*mapSize.x + i] = (tile - int('0'))*10;
			fin.get(tile);
			if (tile != ' ')
				map[j*mapSize.x + i] += tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	getline(fin, line);
	if (line.compare(0, 7, "OBJECTS") != 0)
		return false;
	getline(fin, line);
	while (line.compare(0, 3, "END") != 0) {
		int objN, objSize;
		glm::ivec2 sheetSize, tilePos;
		getline(fin, line);
		sstream.str(line);
		sstream >> objN >> objSize;
		getline(fin, line);
		sstream.str(line);
		sstream >> tilesheetFile;
		getline(fin, line);
		sstream.str(line);
		sstream >> sheetSize.x >> sheetSize.y;
		getline(fin, line);
		sstream.str(line);
		sstream >> tilePos.x >> tilePos.y;
		for (int i = 0; i < objN; ++i) {
			glm::ivec2 objPos;
			getline(fin, line);
			sstream.str(line);
			sstream >> objPos.x >> objPos.y;
			Object *newObj = new Object();
			newObj->init(glm::vec2(objPos.x * tileSize, objPos.y * tileSize), tilesheetFile, program, objSize, sheetSize, minCoords);
			newObj->setTexPosition(tilePos);
			newObj->setTileMap(this);
			newObj->setInteractable();
			objects.push_back(newObj);
		}
		getline(fin, line);
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, bool *collision, Object*& interactedObj) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] != 0)
			return true;
	}

	x = pos.x;
	y0 = pos.y;
	y1 = (pos.y + size.y - 1);
	for (int i = 0; i < int(objects.size()); ++i) {
		if (objects[i]->canCollide()) {
			glm::ivec2 objPos = objects[i]->getPosition();
			int objSize = objects[i]->getSize();
			if ((x > objPos.x && x <= (objPos.x + objSize)) &&
				(y0 < (objPos.y + objSize) && objPos.y < y1)) {
				*collision = true;
				interactedObj = objects[i];
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, bool *collision, Object*& interactedObj) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] != 0)
			return true;
	}

	x = (pos.x + size.x - 1);
	y0 = pos.y;
	y1 = (pos.y + size.y - 1);
	for (int i = 0; i < int(objects.size()); ++i) {
		if (objects[i]->canCollide()) {
			glm::ivec2 objPos = objects[i]->getPosition();
			int objSize = objects[i]->getSize();
			if ((x >= objPos.x && x < (objPos.x + objSize)) &&
				(y0 < (objPos.y + objSize) && objPos.y < y1)) {
				*collision = true;
				interactedObj = objects[i];
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool *collision, Object*& interactedObj) const
{
	int x0, x1, y, y2;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	x0 = pos.x;
	x1 = (pos.x + size.x - 1);
	y2 = (pos.y + size.y - 1);
	for (int i = 0; i < int(objects.size()); ++i)
	{
		if (objects[i]->canCollide()) {
			glm::ivec2 objPos = objects[i]->getPosition();
			int objSize = objects[i]->getSize();

			if ((y2 >= objPos.y && y2 < (objPos.y + objSize)) &&
				(x0 < (objPos.x + objSize) && objPos.x < x1))
			{
				if (*posY - tileSize * y + size.y <= 5)
				{
					*posY = tileSize * y - size.y;
					*collision = true;
					return true;
				}
			}
		}
	}

	return false;
}

bool TileMap::collisionStaticUp(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y, y2;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0)
		{
			return true;
		}
	}

	x0 = pos.x;
	x1 = (pos.x + size.x - 1);
	y2 = pos.y;
	for (int i = 0; i < int(objects.size()); ++i)
	{
		if (objects[i]->canCollide()) {
			glm::ivec2 objPos = objects[i]->getPosition();
			int objSize = objects[i]->getSize();

			if ((y2 > objPos.y && y2 <= (objPos.y + objSize)) &&
				(x0 < (objPos.x + objSize) && objPos.x < x1))
			{
				return true;
			}
		}
	}

	return false;
}

