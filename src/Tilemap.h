#pragma once

#include <vector>
#include "Tile.h"

class Tilemap {
	public:
		Tilemap();
		Tilemap(unsigned width, unsigned height, sf::Texture* tile_sheet, unsigned tile_size);
		virtual ~Tilemap();
		
		inline const int& getTileSize() const { return tileSize; }

		//Functions
		void addTile(unsigned x, unsigned y);
		void removeTile(unsigned x, unsigned y);

		void update();
		void render(sf::RenderTarget& target);	

	private:
		std::vector< std::vector<Tile*> > tiles;
		sf::Texture *tileSheet;
		int tileSize;
};