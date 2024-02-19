
#pragma once

#include "Global.hpp"
#include "Piece.hpp"
#include "Board.hpp"

#include <array>

class LegalMovesManager
{
private:
	//Board variables/functions
	std::array<Board::Tile, BOARD::COLUMNS* BOARD::ROWS>& board;
	std::vector<sf::Vector2u>& piecesPositions;
	Board::Tile* GetTile(const sf::Vector2u coordinates);

	//Get legal pieces move functions
	const std::vector<sf::Vector2u> GetPawnLegalMoves(std::shared_ptr<Piece> piece, const bool attacking);
	const std::vector<sf::Vector2u> GetKnightLegalMoves(std::shared_ptr<Piece> piece, const bool attacking);
	const std::vector<sf::Vector2u> GetKingLegalMoves(std::shared_ptr<Piece> piece, const bool attacking);
	const std::vector<sf::Vector2u> GetSlidingPieceLegalMoves(std::shared_ptr<Piece> piece, const std::vector<sf::Vector2i> directions, const bool attacking);
	const const std::array<bool, BOARD::COLUMNS* BOARD::ROWS> GetAllLegalMoves(const PIECE::Color color, std::shared_ptr<Piece> piece);
	
	//Move a piece on a non existing board
	void MovePiece(std::shared_ptr<Piece> piece, const sf::Vector2u target);

	//Checks functions
	const bool CheckForChecks(const PIECE::Color color);
	const bool CheckForChecksAfterMove(const PIECE::Color color, std::shared_ptr<Piece> piece, const sf::Vector2u target);

	//Check if a position is offscreen
	const bool IsOnScreen(const sf::Vector2u position) const;

public:
	//Constructor/Destructor
	LegalMovesManager(std::array<Board::Tile, BOARD::COLUMNS* BOARD::ROWS>& board, std::vector<sf::Vector2u>& piecesPositions);
	~LegalMovesManager();

	//Moves functions
	const std::vector<sf::Vector2u> GetLegalMoves(std::shared_ptr<Piece> piece, const bool attacking);
};

