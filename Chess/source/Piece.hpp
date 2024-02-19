
#pragma once

#include "Global.hpp"

class Piece
{
private:
	//Window variables
	sf::RenderWindow* window;

	//Piece variables/functions
	sf::Sprite sprite;
	void LoadSprite();

	//Piece position variables
	sf::Vector2u position;
	sf::Vector2u targetPosition;
	bool firstMove = true;

	//En passant variables
	bool enPassantCapturable = false;

	//Type variables
	PIECE::Type type = PIECE::Type::Pawn;
	PIECE::Color color = PIECE::Color::White;

public:
	//Constructor/Destructor
	Piece(sf::RenderWindow* window);
	~Piece();

	//Texture functions
	void SetTexture(sf::Texture& texture);
	const sf::Texture* GetTexture() const;

	//Position functions
	void SetPosition(const sf::Vector2u position);
	const sf::Vector2u& GetPosition() const;

	const sf::Vector2f& GetVisualPosition() const;

	void SetTargetPosition(const sf::Vector2u position);
	const sf::Vector2u& GetTargetPosition() const;

	const sf::Vector2f& GetVisualTargetPosition() const;

	void SetFirstMove(const bool isFirstMove);
	const bool& IsFirstMove() const;

	void Drag(const sf::Vector2f draggingPosition);

	//Check if the piece can be captured en passant
	const bool& IsEnPassantCapturable() const;
	void SetPassantCapturable(const bool value);

	//Type functions
	void SetType(const PIECE::Type type);
	const PIECE::Type& GetType() const;

	void SetColor(const PIECE::Color color);
	const PIECE::Color& GetColor() const;
	const PIECE::Color& GetOppositeColor() const;

	//Draw piece
	void Draw();
};

