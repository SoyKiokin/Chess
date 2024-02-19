
#pragma once

#include "Global.hpp"
#include "Piece.hpp"
#include "MouseManager.hpp"
class LegalMovesManager;
#include "PromotionPieceSelector.hpp"

#include <unordered_map>
#include <array>

class Board
{
public:
	//Constructor/Destructor
	Board(sf::RenderWindow* window);
	~Board();

	//Load board functions
	void LoadFEN(const std::string FEN);

	//Update functions
	void Update(const float& deltaTime);

	//Draw functions
	void Draw();

	//Create a structure containing a piece and a boolean that says if the tile has a piece
	struct Tile
	{
		std::shared_ptr<Piece> piece;
		bool hasPiece = false;
	};

private:
	//Window variables
	sf::RenderWindow* window;

	//Mouse variables
	MouseManager mouseManager;
	sf::Vector2f mousePosition;
	sf::Vector2u mouseTile;

	//Board variables/functions
	sf::VertexArray boardSprite;
	void CreateBoardSprite();

	//Board map variables/functions
	Tile* GetTile(const sf::Vector2u coordinates);
	std::array<Tile, BOARD::COLUMNS* BOARD::ROWS> board;
	std::vector<sf::Vector2u> piecesPositions;

	//Piece variables
	sf::Texture piecesAtlasTexture;

	//Pieces movement variables
	bool isDraggingPiece = false;
	std::shared_ptr<Piece> draggingPiece;
	std::vector<sf::Vector2u> draggingPieceLegalMoves;
	LegalMovesManager* legalMovesManager;

	void UpdatePiecesMovement(const float& deltaTime);
	void StopDraggingPiece();

	//Move piece functions
	void MovePiece(std::shared_ptr<Piece> piece, const sf::Vector2u target);
	void Castle(std::shared_ptr<Piece> king);

	//En passant functions
	void CaptureEnPassant(std::shared_ptr<Piece> piece);

	//Promotion functions/variables
	void Promote(std::shared_ptr<Piece> piece);
	void StopPromoting();
	bool promoting = false;
	std::array<PromotionPieceSelector*, 2> promotionPieceSelectors;

	//Turn variables
	PIECE::Color turnColor = PIECE::Color::White;

	//Check if a position is offscreen
	const bool IsOnScreen(const sf::Vector2u position) const;
};

