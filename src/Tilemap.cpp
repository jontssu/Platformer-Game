#include "stdafx.h"
#include "Tilemap.h"
#include <filesystem>

Tilemap::Tilemap()
{
	//Initialize tilemap
	tileSheet = NULL;
	tileSize = 0;
}

Tilemap::Tilemap(unsigned width, unsigned height, sf::Texture *tile_sheet, unsigned tile_size, int current_level)
: tileSheet(tile_sheet), tileSize(tile_size), currentLevel(current_level)
{
	tiles.resize(width);
	for (int i = 0; i < tiles.size(); i++)
		tiles[i].resize(height, nullptr);

	std::ifstream inFile("./levels/map" + std::to_string(currentLevel) + ".txt");
	std::cout << "Loading level from: " << "levels/map" + std::to_string(currentLevel) + ".txt" << "\n";
	if (inFile.is_open())
	{
		char ch;
		int x = 0, y = 0;

		while (inFile.get(ch) && y < height) {
			if (ch == '1') {
				if (x < width && y < height) {
					tiles[x][y] = new Tile(x, y, tileSize, tileSheet, sf::IntRect({32, 36}, {tileSize, tileSize}), false, TILE_NORMAL);
				}
				x++;
			} else if (ch == '2') {
				if (x < width && y < height) {
					tiles[x][y] = new Tile(x, y, tileSize, tileSheet, sf::IntRect({32, 36}, {tileSize, tileSize}), false, TILE_POWERUP_EXTRA_BLOCK);
				}
				x++;
			} else if (ch == '3') {
				if (x < width && y < height) {
					tiles[x][y] = new Tile(x, y, tileSize, tileSheet, sf::IntRect({32, 36}, {tileSize, tileSize}), false, TILE_POWERUP_JUMP_BOOST);
				}
				x++;
			} else if (ch == '4') {
				if (x < width && y < height) {
					tiles[x][y] = new Tile(x, y, tileSize, tileSheet, sf::IntRect({32, 36}, {tileSize, tileSize}), false, TILE_PLACEABLE);
				}
				x++;	
			} else if (ch == '5') {
				if (x < width && y < height) {
					tiles[x][y] = new Tile(x, y, tileSize, tileSheet, sf::IntRect({32, 36}, {tileSize, tileSize}), false, TILE_DEADLY);
				}
				x++;
			} else if (ch == '6') {
				if (x < width && y < height) {
					tiles[x][y] = new Tile(x, y, tileSize, tileSheet, sf::IntRect({32, 36}, {tileSize, tileSize}), false, TILE_WIN);
				}
				x++;	
			} else if (ch == '0') {
				x++;
			} else if (ch == '\n') {
				x = 0;
				y++;
			} else if (ch == ' ') {
				continue;
			}
		}
		inFile.close();
	}
}


Tilemap::~Tilemap()
{
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[i].size(); j++)
		{
			delete tiles[i][j];
			tiles[i][j] = nullptr;
		}
	}
}

bool Tilemap::addTile(unsigned x, unsigned y)
{
	if (x < tiles.size() && x >= 0)
	{
		if (y < tiles[x].size() && y >= 0)
		{
			if (tiles[x][y] == nullptr)
			{
				tiles[x][y] = new Tile(x, y, tileSize, tileSheet, sf::IntRect({32, 36}, {tileSize, tileSize}), false, TILE_PLACEABLE);
				return (true);
			}
		}
	}
	return false;
}

bool Tilemap::removeTile(unsigned x, unsigned y, bool can_respawn, bool playerDelete)
{
	if (x < tiles.size())
	{
		if (y < tiles[x].size())
		{
			if (tiles[x][y] != nullptr)
			{
				if (tiles[x][y]->getType() == TILE_POWERUP_JUMP_BOOST && can_respawn) {
					tiles[x][y]->setDespawn();
					return true; // Tile was "removed" by despawning
				}
				else if (playerDelete == false && (tiles[x][y]->getType() == TILE_PLACEABLE || tiles[x][y]->getType() == TILE_POWERUP_EXTRA_BLOCK)) {
					delete tiles[x][y];
					tiles[x][y] = nullptr;
					return true; // Tile was successfully removed
				}
				else if (playerDelete == true && tiles[x][y]->getType() == TILE_PLACEABLE) {
					delete tiles[x][y];
					tiles[x][y] = nullptr;
					return true; // Tile was successfully removed
				}
			}
		}
	}
	return false; // No tile was removed
}

void Tilemap::update()
{
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[i].size(); j++)
		{
			if (tiles[i][j] != nullptr)
			{
				tiles[i][j]->update();
			}
		}
	}

	//Save every 2 sec
	static int frameCount = 0;
	frameCount++;
	if (frameCount % 120 == 0) {
		saveToFile();
	}
}

void Tilemap::saveToFile()
{
	std::ofstream outFile("./levels/copy.map" + std::to_string(currentLevel) + ".txt", std::ios::out | std::ios::trunc);
	if (outFile.is_open())
	{
		
		for (int j = 0; j < tiles[0].size(); j++)
		{
			for (int i = 0; i < tiles.size(); i++)
			{
				if (tiles[i][j] == nullptr)
					outFile << "0 ";
				else if (tiles[i][j]->getType() == TILE_NORMAL)
					outFile << "1 ";
				else if (tiles[i][j]->getType() == TILE_POWERUP_EXTRA_BLOCK)
					outFile << "2 ";
				else if (tiles[i][j]->getType() == TILE_POWERUP_JUMP_BOOST)
				{
					if (tiles[i][j]->getIsSpawned() == false)
						outFile << "0 ";
					else
						outFile << "3 ";
				}
				else
				{
					outFile << "0 ";
				}
			}
			outFile << "\n";
		}
		outFile.flush();
		outFile.close();
	}
	else
	{
		std::cout << "ERROR: Could not open file for writing!" << std::endl;
	}
}

void Tilemap::render(sf::RenderTarget& target)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[i].size(); j++)
		{
			if (tiles[i][j] != nullptr)
				tiles[i][j]->render(target);
		}
	}
}