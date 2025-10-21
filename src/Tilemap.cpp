#include "stdafx.h"
#include "Tilemap.h"

Tilemap::Tilemap()
{
	//Initialize tilemap
	tileSheet = NULL;
	tileSize = 0;
}

Tilemap::Tilemap(unsigned width, unsigned height, sf::Texture *tile_sheet, unsigned tile_size)
{
	tileSheet = tile_sheet;
	tileSize = tile_size;

	tiles.resize(width);
	for (int i = 0; i < tiles.size(); i++)
	{
		tiles[i].resize(height, nullptr);
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

void Tilemap::addTile(unsigned x, unsigned y)
{
	if (x < tiles.size() && x >= 0)
	{
		if (y < tiles[x].size() && y >= 0)
		{
			if (tiles[x][y] == nullptr)
			{
				tiles[x][y] = new Tile(x, y, tileSize, tileSheet, sf::IntRect({0, 0}, {tileSize, tileSize}), false);
			}
		}
	}
}

void Tilemap::removeTile(unsigned x, unsigned y)
{
	if (x < tiles.size())
	{
		if (y > tiles[x].size())
		{
			if (tiles[x][y] != nullptr)
			{
				delete tiles[x][y];
				tiles[x][y] = nullptr;
			}
		}
	}
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
}

void Tilemap::render(sf::RenderTarget &target)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[i].size(); j++)
		{
			if (tiles[i][j] != nullptr)
			{
				tiles[i][j]->render(target);
			}
		}
	}
}