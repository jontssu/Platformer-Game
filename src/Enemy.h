#pragma once

class Enemy {
	public:
		Enemy(float x, float y, sf::Texture* texture_sheet, bool vertical_movement = true, int minBound = 0, int maxBound = 0);
		virtual ~Enemy() {}

		// Getters
		const sf::Vector2f getPosition() const;
		const sf::FloatRect getGlobalBounds() const;
		bool isAlive() const { return alive; }

		// Functions
		void update();
		void render(sf::RenderTarget& target);
		void takeDamage();

	private:
		sf::Sprite sprite;
		sf::Texture* textureSheet;
		
		// Movement
		bool verticalMovement;
		int minBound, maxBound;
		sf::Vector2f velocity;
		float speed;
		int direction; // 1 for right, -1 for left
		float leftBound, rightBound, topBound, bottomBound;
		
		// State
		bool alive;
		
		void initSprite(float x, float y);
		void updateMovement();
};