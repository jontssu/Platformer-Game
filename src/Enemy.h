#pragma once

class Enemy {
	public:
		Enemy();
		virtual ~Enemy() {}

	private:
		sf::Sprite sprite;
		sf::Texture textureSheet;
};