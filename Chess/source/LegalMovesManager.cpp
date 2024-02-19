
#include "LegalMovesManager.hpp"

Board::Tile* LegalMovesManager::GetTile(const sf::Vector2u coordinates)
{
	return &board[coordinates.x * BOARD::ROWS + coordinates.y];
}

const std::vector<sf::Vector2u> LegalMovesManager::GetPawnLegalMoves(std::shared_ptr<Piece> piece, const bool attacking)
{
	//Create the output function variable
	std::vector<sf::Vector2u> output;

	//Create a value that can be -1 or 1, the value depends on the color of the piece
	const int colorMultiplier = piece->GetColor() == PIECE::Color::White ? -1 : 1;

	//Calculate the position in front of th pawn
	sf::Vector2u position = piece->GetPosition() + sf::Vector2u(0, colorMultiplier);

	//Check if the position is visualy on the screen
	if (IsOnScreen(position) && !attacking)
	{
		//Check if there's a piece in front of the pawn
		const Board::Tile* tile = GetTile(position);

		if (!tile->hasPiece)
		{
			//Check if there's a check after the move
			if (!CheckForChecksAfterMove(piece->GetColor(), piece, position))
			{
				//Push the legal move to the output vector
				output.push_back(position);
			}

			//Check if the pawn has moved yet
			if (piece->IsFirstMove())
			{
				//Get the position two tiles in front of the pawn
				position.y += colorMultiplier;

				//Check if the position is visualy on the screen
				if (IsOnScreen(position))
				{
					//Check if there's a piece two tiles in front of the pawn
					const Board::Tile* tile = GetTile(position);

					if (!tile->hasPiece)
					{
						//Check if there's a check after the move
						if (!CheckForChecksAfterMove(piece->GetColor(), piece, position))
						{
							//Push the legal move to the output vector
							output.push_back(position);
						}
					}
				}
			}
		}
	}

	//Make a rectangle of 1x3 to check if there's a piece in the pawn's attacking range
	for (int i = -1; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			//Check if the checked position is in front of the pawn
			if (i == 0)
			{
				continue;
			}

			//Get the position of the attacking range of the pawn based on the iterator
			position = piece->GetPosition() + sf::Vector2u(i, j * colorMultiplier);

			//Check if the position is visualy on the screen
			if (IsOnScreen(position))
			{
				//Get the tile based on the checking position
				const Board::Tile* tile = GetTile(position);

				//Set the attacking range
				if (attacking)
				{
					//Don't check for en passant captures
					if (position.y != piece->GetPosition().y)
					{
						output.push_back(position);

					}
					continue;
				}

				//Check if there's an enemy piece in the tile
				if (tile->hasPiece)
				{
					//Check if the tile piece is an opponent piece
					if (tile->piece->GetColor() != piece->GetColor())
					{
						//Check if there's a check after the move
						if (!CheckForChecksAfterMove(piece->GetColor(), piece, position))
						{
							//Check if the checking position is on the en passant capture range
							if (position.y == piece->GetPosition().y)
							{
								//Check if the checking piece can capture the tile's piece en passant
								if (tile->piece->IsEnPassantCapturable())
								{
									//Push the legal move to the output vector and modify the position
									//so the legal move is in front of the pawn
									output.push_back(position + sf::Vector2u(0, colorMultiplier));
								}
							}
							else
							{
								//Push the legal move to the output vector
								output.push_back(position);
							}
						}
					}
				}
			}
		}
	}

	return output;
}

const std::vector<sf::Vector2u> LegalMovesManager::GetKnightLegalMoves(std::shared_ptr<Piece> piece, const bool attacking)
{
	//Create the output function variable
	std::vector<sf::Vector2u> output;

	//Create a square of 4x4 around the knight a take only the available knight positions
	for (int i = -2; i < 3; i++)
	{
		for(int j = -2; j < 3; j++)
		{
			//Check if the some of the x and y axis are equals to 3, that means that it's a available knight position
			if (abs(i) + abs(j) == 3)
			{
				//Get the position based on the iterator
				const sf::Vector2u position = sf::Vector2u(sf::Vector2i(piece->GetPosition()) + sf::Vector2i(i, j));

				//Check if the calculated position based on the iterator is on screen
				if (IsOnScreen(position))
				{
					//Set the attacking range of the piece
					if (attacking)
					{
						output.push_back(position);
						continue;
					}

					//Check if there's a same color piece on the checked position, if so, don't push the checked position to the output vector
					const Board::Tile* tile = GetTile(position);

					if (tile->hasPiece)
					{
						if (tile->piece->GetColor() == piece->GetColor())
						{
							continue;
						}
					}

					//Check if there's a check after the move
					if (!CheckForChecksAfterMove(piece->GetColor(), piece, position))
					{
						//Push the legal move to the output vector
						output.push_back(position);
					}
				}
			}
		}
	}

	return output;
}

const std::vector<sf::Vector2u> LegalMovesManager::GetKingLegalMoves(std::shared_ptr<Piece> piece, const bool attacking)
{
	//Create the output function variable
	std::vector<sf::Vector2u> output;

	//Create a square of 3x3 around the king a take only the available knight positions
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			//Check if the iterators are pointing to the starting king position
			if(i != 0 || j != 0)
			{
				//Get the position based on the iterator
				const sf::Vector2u position = sf::Vector2u(sf::Vector2i(piece->GetPosition()) + sf::Vector2i(i, j));

				//Check if the calculated position based on the iterator is on screen
				if (IsOnScreen(position))
				{
					//Check if there's a same color piece on the checked position, if so, don't push the checked position to the output vector
					const Board::Tile* tile = GetTile(position);

					//Set the attacking range
					if (attacking)
					{
						output.push_back(position);
						continue;
					}

					//Check if the current position tile has a piece
					if (tile->hasPiece)
					{
						if (tile->piece->GetColor() == piece->GetColor())
						{
							continue;
						}
					}

					//Get all the enemy attacking tiles
					const std::array<bool, BOARD::ROWS* BOARD::COLUMNS> attackedTilesBoard = GetAllLegalMoves(piece->GetOppositeColor(), piece);

					//Check if the move is legal
					if (!attackedTilesBoard[position.x * BOARD::ROWS + position.y])
					{
						//Push the calculated position in the output vector
						output.push_back(position);
					}
				}
			}
		}
	}
	
	//Check for the castle
	//Check if the king moved before
	if (piece->IsFirstMove())
	{
		//Get the left and right direction to use them later in the loop
		const std::array<int, 2> directions = { -1, 1 };
		
		//Check the long and short castle directions
		for (auto& direction : directions)
		{
			//Get the current position of the direction
			sf::Vector2u currentPosition = piece->GetPosition();

			//Check for every tile based on the direction until is off screen or it hits a piece
			while (true)
			{
				//Get the current position and the direction in a variable
				const sf::Vector2u nextPosition = currentPosition + sf::Vector2u(direction, 0);

				//Check if the next position is on screen, if it's not, stop the loop
				if (!IsOnScreen(nextPosition))
				{
					break;
				}

				//Get the tile of the current next position
				Board::Tile* tile = GetTile(nextPosition);

				//Check if the next position tile has a piece, if it does, stop the loop
				if (tile->hasPiece)
				{
					//Check if the piece that the tile has is a rook and moved before
					if (tile->piece->GetType() == PIECE::Type::Rook && tile->piece->IsFirstMove())
					{
						//Get the castle position of the king based on the direction
						const sf::Vector2u castlePosition = piece->GetPosition() + sf::Vector2u(direction * 2, 0);

						//Get all the enemy attacking tiles
						const std::array<bool, BOARD::ROWS* BOARD::COLUMNS> attackedTilesBoard = GetAllLegalMoves(piece->GetOppositeColor(), piece);

						//Check if the move is legal
						if (!attackedTilesBoard[castlePosition.x * BOARD::ROWS + castlePosition.y] && 
							!attackedTilesBoard[(piece->GetPosition().x + direction) * BOARD::ROWS + castlePosition.y] &&
							!attackedTilesBoard[piece->GetPosition().x * BOARD::ROWS + castlePosition.y])
						{
							//Push the calculated position in the output vector
							output.push_back(castlePosition);
						}
					}

					break;
				}

				//Continue the loop setting the current position to the next position
				currentPosition = nextPosition;
			}
		}
	}

	return output;
}

const std::vector<sf::Vector2u> LegalMovesManager::GetSlidingPieceLegalMoves(std::shared_ptr<Piece> piece, const std::vector<sf::Vector2i> directions, const bool attacking)
{
	//Create the output function variable
	std::vector<sf::Vector2u> output;

	//Check on every direction all the legal moves
	for (auto& direction : directions)
	{
		//Create a vector that will store every checked position on the checked direction
		sf::Vector2u currentPosition = piece->GetPosition();

		//Check if the next checked position is on screen
		while (IsOnScreen(currentPosition + sf::Vector2u(direction)))
		{
			//Add the update the current position so it's the next current position
			currentPosition += sf::Vector2u(direction);

			//Set the attacking range of the sliding piece
			if(attacking)
			{
				output.push_back(currentPosition);
			}

			//Get the tile based on the current position
			Board::Tile* tile = GetTile(currentPosition);

			//Check if there's a piece in the current position
			if (tile->hasPiece)
			{
				if (tile->piece->GetColor() != piece->GetColor())
				{
					if (!attacking)
					{
						//Check if there's a check after the move
						if (!CheckForChecksAfterMove(piece->GetColor(), piece, currentPosition))
						{
							//Push the legal move to the output vector
							output.push_back(currentPosition);
						}
					}
				}
				//Set the attacking range of the sliding piece so a piece can be defended
				else if (attacking)
				{
					output.push_back(currentPosition);
				}

				//Stop the loop
				break;
			}

			//Check if the function is checking the attacking range
			if (!attacking)
			{
				//Check if there's a check after the move
				if (!CheckForChecksAfterMove(piece->GetColor(), piece, currentPosition))
				{
					//Push the legal move to the output vector
					output.push_back(currentPosition);
				}
			}
		}
	}

	return output;
}

const std::array<bool, BOARD::COLUMNS* BOARD::ROWS> LegalMovesManager::GetAllLegalMoves(const PIECE::Color color, std::shared_ptr<Piece> piece)
{
	//Create the output function variable
	std::array<bool, BOARD::COLUMNS* BOARD::ROWS> output = {};

	//Get all the positions the pieces are on
	for (auto& position : piecesPositions)
	{
		//Get the piece of the tile based on the current position
		auto& currentPiece = GetTile(position)->piece;

		//Check if the piece is the same color as the given color
		if (currentPiece->GetColor() == color && currentPiece != piece)
		{
			//Get all the legal moves of the piece of the tile
			for (auto& attackingPosition : GetLegalMoves(currentPiece, true))
			{
				//Set to the tile of the created board is attacked
				output[attackingPosition.x * BOARD::ROWS + attackingPosition.y] = true;
			}
		}
	}

	return output;
}

//Check if there's a check
const bool LegalMovesManager::CheckForChecks(const PIECE::Color color)
{
	//Get the king position member of the pieces positions vector with lambda
	const auto& kingPositionMember = std::find_if(piecesPositions.begin(), piecesPositions.end(), [&](const sf::Vector2u& position)
		{
			return GetTile(position)->piece->GetType() == PIECE::Type::King && GetTile(position)->piece->GetColor() == color;
		});

	//Check if there's a king in the pieces positions vector
	if (piecesPositions.end() != kingPositionMember)
	{
		//Get the king's piece based on the king position member
		auto& king = GetTile(piecesPositions[std::distance(piecesPositions.begin(), kingPositionMember)])->piece;
	
		//Get all the attacked tiles
		const std::array<bool, BOARD::ROWS* BOARD::COLUMNS> attackedTilesBoard = GetAllLegalMoves(king->GetOppositeColor(), king);

		return attackedTilesBoard[king->GetPosition().x * BOARD::ROWS + king->GetPosition().y];
	}

	return false;
}

const bool LegalMovesManager::CheckForChecksAfterMove(const PIECE::Color color, std::shared_ptr<Piece> piece, const sf::Vector2u target)
{
	//The output of the function
	bool isCheck = false;

	//Save the current position of the piece, the board and all the pieces positions
	const sf::Vector2u currentPiecePosition = piece->GetPosition();
	std::array<Board::Tile, BOARD::COLUMNS* BOARD::ROWS> currentBoard = board;
	std::vector<sf::Vector2u> currentPiecesPositions = piecesPositions;

	//Get the king position member of the pieces positions vector with lambda
	const auto& kingPositionMember = std::find_if(piecesPositions.begin(), piecesPositions.end(), [&](const sf::Vector2u& position)
		{
			return GetTile(position)->piece->GetType() == PIECE::Type::King && GetTile(position)->piece->GetColor() == color;
		});

	//Check if there's a king in the pieces positions vector
	if (piecesPositions.end() != kingPositionMember)
	{
		//Get the king's piece based on the king position member
		auto& king = GetTile(piecesPositions[std::distance(piecesPositions.begin(), kingPositionMember)])->piece;

		//Check if the piece wants to eat the king to stop the check
		if (target != king->GetPosition())
		{
			//Set the piece's position to its target temporarily
			MovePiece(piece, target);
		}

		//Get all the attacked tiles
		const std::array<bool, BOARD::ROWS* BOARD::COLUMNS> attackedTilesBoard = GetAllLegalMoves(king->GetOppositeColor(), king);

		isCheck = attackedTilesBoard[king->GetPosition().x * BOARD::ROWS + king->GetPosition().y];
	}

	//Set the piece's position back to its original position and the board back to how it was
	MovePiece(piece, currentPiecePosition);
	board = currentBoard;
	piecesPositions = currentPiecesPositions;

	return isCheck;
}

const bool LegalMovesManager::IsOnScreen(const sf::Vector2u position) const
{
	//Check if the given position is on screen
	return (position.x >= 0 && position.x < BOARD::COLUMNS &&
		position.y >= 0 && position.y < BOARD::ROWS);
}

void LegalMovesManager::MovePiece(std::shared_ptr<Piece> piece, const sf::Vector2u target)
{
	//Get the tile of the current piece position and its target
	Board::Tile* targetTile = GetTile(target);
	Board::Tile* currentTile = GetTile(piece->GetPosition());

	//Iterate through every piece position
	for (int i = 0; i < piecesPositions.size(); i++)
	{
		//Get the current position of the pieces positions vector based on the iterator
		auto& position = piecesPositions[i];

		//Check if the checked position is the same as the target position
		if (position == target)
		{
			//Delete the position in the pieces positions vector of the target
			if (targetTile->hasPiece)
			{
				piecesPositions.erase(piecesPositions.begin() + i);
			}
		}

		//Check if the position is the same as the piece's position
		if (position == piece->GetPosition())
		{
			//Move the piece's position in the vector of pieces positions to its target
			piecesPositions[i] = target;
		}
	}

	//Copy the piece to its target
	targetTile->hasPiece = true;
	targetTile->piece = piece;

	//Delete the original piece
	currentTile->hasPiece = false;
	currentTile->piece.reset();

	//Set the piece to the target position
	piece->SetPosition(target);
}

LegalMovesManager::LegalMovesManager(std::array<Board::Tile, BOARD::COLUMNS* BOARD::ROWS>& board, std::vector<sf::Vector2u>& piecesPositions) : 
	board(board),
	piecesPositions(piecesPositions)
{

}

LegalMovesManager::~LegalMovesManager()
{

}

const std::vector<sf::Vector2u> LegalMovesManager::GetLegalMoves(std::shared_ptr<Piece> piece, const bool attacking)
{
	switch (piece->GetType())
	{
	case PIECE::Type::Pawn:
		return GetPawnLegalMoves(piece, attacking);

	case PIECE::Type::Knight:
		return GetKnightLegalMoves(piece, attacking);

	case PIECE::Type::King:
		return GetKingLegalMoves(piece, attacking);

	case PIECE::Type::Bishop:
		return GetSlidingPieceLegalMoves(piece, { {-1, -1}, {1, -1}, {-1, 1}, {1, 1} }, attacking);

	case PIECE::Type::Rook:
		return GetSlidingPieceLegalMoves(piece, { {-1, 0}, {1, 0}, {0, -1}, {0, 1} }, attacking);

	case PIECE::Type::Queen:
		return GetSlidingPieceLegalMoves(piece, { {-1, -1}, {1, -1}, {-1, 1}, {1, 1}, {-1, 0}, {1, 0}, {0, -1}, {0, 1} }, attacking);
	}

	return { };
}
