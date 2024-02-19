
#pragma once

#include "Global.hpp"
#include <array>

class PromotionPieceSelector
{
private:
	//Default variables
	sf::RenderWindow* window;
	const PIECE::Color color;

	//Piece square of the list structure
	struct Piece
	{
		sf::RectangleShape background;
		sf::Sprite sprite;
		PIECE::Type type;
	};

	//Pieces variables
	std::array<Piece, PIECE::PROMOTION_PIECES.size()> pieces;

	//Shadow variables
	sf::RectangleShape shadow;
	const unsigned int SHADOW_THICKNESS = 5;
	void UpdateShadowPosition();

	//Show or hide variables
	bool show = true;

	//Piece selection variables
	PIECE::Type lastPieceSelection = PIECE::Queen;

	//Mouse input variables/functions
	bool mousePressedPreviously = false;
	const bool& MouseJustReleased();

public:
	//Constructor/Destructor
	PromotionPieceSelector(sf::RenderWindow* window, const PIECE::Color color);
	~PromotionPieceSelector();

	//Set the texture of the pieces
	void SetTexture(const sf::Texture& texture);
	const sf::Texture& GetTexture() const;

	//Set the position of the first piece's position
	void SetFirstPiecePosition(const sf::Vector2f position);
	const sf::Vector2f& GetFirstPiecePosition() const;

	//Show or hide functions
	void Show();
	void Hide();
	const bool& IsVisible() const;

	//Piece selection functions
	const PIECE::Type GetLastPieceSelection() const;

	//Default functions
	void Update(const float& deltaTime);
	void Draw();
};

