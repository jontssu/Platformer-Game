#pragma once

#include <vector>
#include "Tile.h"

class Tilemap {
	public:
		Tilemap();
		Tilemap(unsigned width, unsigned height, sf::Texture* tile_sheet, unsigned tile_size, int current_level);
		virtual ~Tilemap();
		
		inline const int& getTileSize() const { return tileSize; }
		inline const std::vector< std::vector<Tile*> >& getTiles() const { return tiles; }

		//Functions
		bool addTile(unsigned x, unsigned y);
		bool removeTile(unsigned x, unsigned y, bool can_respawn, bool playerDelete); // Returns true if a tile was actually removed
		void saveToFile(); // Save current tilemap to file

		void update();
		void render(sf::RenderTarget& target);	

	private:
		std::vector< std::vector<Tile*> > tiles;
		sf::Texture *tileSheet;
		sf::Texture playerSheet;
		int tileSize;
		int currentLevel;
};