
#include "Board.hpp"
#include "LegalMovesManager.hpp"

//Create the bord background sprite
void Board::CreateBoardSprite()
{
    //Resize the board sprite vertex array
    boardSprite.setPrimitiveType(sf::Quads);
    boardSprite.resize(BOARD::COLUMNS * BOARD::ROWS * 4);

    //Create every square of the board sprite vertex array
    for (unsigned int i = 0; i < BOARD::COLUMNS; i++)
    {
        for (unsigned int j = 0; j < BOARD::ROWS; j++)
        {
            //Find the tile index based on the iterator
            const unsigned int tileIndex = i * BOARD::ROWS + j;

            //Get the square pointer in the board sprite vertex array
            sf::Vertex* quad = &boardSprite[tileIndex * 4];

            //Set the 4 corners of the square
            quad[0].position = sf::Vector2f(BOARD::TILE::SIZE * i, BOARD::TILE::SIZE * j);
            quad[1].position = sf::Vector2f(BOARD::TILE::SIZE * (i + 1), BOARD::TILE::SIZE * j);
            quad[2].position = sf::Vector2f(BOARD::TILE::SIZE * (i + 1), BOARD::TILE::SIZE * (j + 1));
            quad[3].position = sf::Vector2f(BOARD::TILE::SIZE * i, BOARD::TILE::SIZE * (j + 1));

            //Define the color of the square
            const bool isLightSquare = (i + j) % 2 == 0;
            const sf::Color color = isLightSquare ? BOARD::TILE::WHITE_COLOR : BOARD::TILE::BLACK_COLOR;

            //Set the color of the square
            quad[0].color = quad[1].color = quad[2].color = quad[3].color = color;
        }
    }
}

Board::Tile* Board::GetTile(const sf::Vector2u coordinates)
{
    return &board[coordinates.x * BOARD::ROWS + coordinates.y];
}

//Update the pieces movement like dragging
void Board::UpdatePiecesMovement(const float& deltaTime)
{
    //Check if the player is promoting
    if (promoting)
    {
        //Stop the function
        return;
    }

    //Get the mouse input
    const bool justPressed = mouseManager.JustPressed(sf::Mouse::Left);
    const bool justReleased = mouseManager.JustReleased(sf::Mouse::Left);

    //Check if the mouse is on the window
    if (mouseTile.x >= 0 && mouseTile.x < BOARD::COLUMNS &&
        mouseTile.y >= 0 && mouseTile.y < BOARD::ROWS && !isDraggingPiece)
    {
        //Check if a piece is dragged
        Tile* tile = GetTile(mouseTile);

        if (tile->hasPiece)
        {
            //Get the tile piece
            std::shared_ptr<Piece> piece = tile->piece;

            //Check if a piece is just pressed
            if (piece->GetPosition() == mouseTile && justPressed)
            {
                //Set the drag piece variable to true and update the dragging piece pointer
                isDraggingPiece = true;
                draggingPiece = piece;

                //Check if it's the piece's turn to play
                //Get the dragging piece legal moves
                if (draggingPiece->GetColor() == turnColor)
                {
                    draggingPieceLegalMoves = legalMovesManager->GetLegalMoves(draggingPiece, false);
                }
                //If is not the piece's turn, clear the dragging piece legal moves vector
                else
                {
                    draggingPieceLegalMoves.clear();
                }
            }
        }
    }

    //Check if a piece stops being dragged
    if (justReleased && isDraggingPiece)
    {
        //Set the drag piece variable to false and update the stop dragging piece function
        isDraggingPiece = false;
        StopDraggingPiece();
    }

    //Check if the piece is been dragged
    if (isDraggingPiece)
    {
        //Drag the piece to the mouse position
        draggingPiece->Drag(mousePosition);
    }
}

void Board::StopDraggingPiece()
{
    //Set the target position to the dragging piece variable
    draggingPiece->SetTargetPosition(mouseTile);

    //Check if the target position is in the legal moves vector
    if (draggingPieceLegalMoves.end() != std::find_if(draggingPieceLegalMoves.begin(), draggingPieceLegalMoves.end(), [&](const sf::Vector2u& position)
        {
            return position == sf::Vector2u(draggingPiece->GetTargetPosition());
        }))
    {
        //Get the difference between the dragging piece's target position and current position x axis
        const int differenceX = int(draggingPiece->GetTargetPosition().x) - int(draggingPiece->GetPosition().x);

        //Check if the king just castled to move the rook as well
        if (draggingPiece->GetType() == PIECE::Type::King && abs(differenceX) > 1)
        {
            Castle(draggingPiece);
        }
        //Check if the dragging piece is a pawn
        else if (draggingPiece->GetType() == PIECE::Type::Pawn)
        {
            //Check if the pawn moved on the x axis, if so, the pawn just captured something
            if (abs(differenceX) > 0)
            {
                //Check if the pawn captured something in the target position
                if (!GetTile(draggingPiece->GetTargetPosition())->hasPiece)
                {
                    //The pawn didn't capture anything in the target position, that means the pawn just captured en passant
                    CaptureEnPassant(draggingPiece);
                }
            }

            //Get the last rank of the pawn based on the color
            const int lastRank = draggingPiece->GetColor() == PIECE::Color::White ? 0 : BOARD::ROWS - 1;

            //Check if the pawn is promoting
            if (draggingPiece->GetTargetPosition().y == lastRank)
            {
                Promote(draggingPiece);
            }
        }

        //Move the dragging piece to its target
        MovePiece(draggingPiece, draggingPiece->GetTargetPosition());

        //Update the piece's turn
        turnColor = turnColor == PIECE::Color::White ? PIECE::Color::Black : PIECE::Color::White;
    }
    else
    {
        //Return the dragging piece to its origin position
        draggingPiece->SetPosition(draggingPiece->GetPosition());
    }
}

//Constructor
Board::Board(sf::RenderWindow* window) : 
	window(window)
{
    //Create the board
	CreateBoardSprite();
    piecesAtlasTexture.loadFromFile("assets/images/pieces.png");

    //Create two promotion piece selectors: one white and another black
    for (unsigned int i = 0; i < promotionPieceSelectors.size(); i++)
    {
        //Create a reference of the current promotion piece selector based on the iterator
        auto& selector = promotionPieceSelectors[i];

        selector = new PromotionPieceSelector(window, PIECE::Color(i));
        selector->SetTexture(piecesAtlasTexture);
        selector->SetFirstPiecePosition(sf::Vector2f(BOARD::TILE::SIZE * 3, (BOARD::COLUMNS - 1) * BOARD::TILE::SIZE * i));
        selector->Hide();
    }
    
    //Update the mouse position
    mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    //Update the current hovering mouse tile position
    mouseTile = sf::Vector2u(
        floor(mousePosition.x / BOARD::TILE::SIZE),
        floor(mousePosition.y / BOARD::TILE::SIZE)
    );

    //Create the legal moves manager
    legalMovesManager = new LegalMovesManager(board, piecesPositions);
}

//Destructor
Board::~Board()
{

}

//Load a board based on the given FEN notation
void Board::LoadFEN(const std::string FEN)
{
    //The current position of the loop
    sf::Vector2u currentPosition = sf::Vector2u(0, 0);

    //The type of the piece based on the given character
    std::unordered_map<char, PIECE::Type> pieceTypeCharacter =
    {
        { 'k', PIECE::Type::King },
        { 'q', PIECE::Type::Queen },
        { 'b', PIECE::Type::Bishop },
        { 'n', PIECE::Type::Knight },
        { 'r', PIECE::Type::Rook },
        { 'p', PIECE::Type::Pawn }
    };

    //Check for every character in the FEN notation
    for (auto character : FEN)
    {
        //Check if the character wants to jump to another row
        if (character == '/')
        {
            //Jump to the next row
            currentPosition.x = 0;
            currentPosition.y++;
        }
        //Check if there are empty spaces
        else if (std::isdigit(character))
        {
            //Ignore the empty spaces
            const unsigned int integerCharacter = character - '0'; //Transform the character to an integer
            currentPosition.x += integerCharacter;
        }
        //Check if the character is a piece number
        else
        {
            //Create a new piece based on the current position
            std::shared_ptr<Piece> piece = std::make_shared<Piece>(window);
            piece->SetTexture(piecesAtlasTexture);
            piece->SetPosition(currentPosition);
            piece->SetType(pieceTypeCharacter[std::tolower(character)]);
            piece->SetColor(std::islower(character) ? PIECE::Color::Black : PIECE::Color::White); //Check if the character is in lower case (black) or in upper case (white)

            //Insert the piece in the board array
            Tile* tile = GetTile(currentPosition);
            tile->hasPiece = true;
            tile->piece = piece;

            piecesPositions.push_back(currentPosition);

            //Continue counting the current positions
            currentPosition.x++;
        }
    } 
}

//Update the board pieces
void Board::Update(const float& deltaTime)
{
    //Update the mouse position
    mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    //Update the current hovering mouse tile position
    mouseTile = sf::Vector2u(
        floor(mousePosition.x / BOARD::TILE::SIZE),
        floor(mousePosition.y / BOARD::TILE::SIZE)
    );

    //Update the promotion pieces selector
    for (auto& selector : promotionPieceSelectors)
    {
        selector->Update(deltaTime);
    }

    //Check if the selector just has been pressed
    if (promotionPieceSelectors[0]->IsVisible() == false && promotionPieceSelectors[1]->IsVisible() == false && promoting)
    {
        StopPromoting();
    }

    UpdatePiecesMovement(deltaTime);
}

//Draw the board on the window
void Board::Draw()
{
    window->draw(boardSprite);

    //Draw all the pieces
    for (auto& piecePosition : piecesPositions)
    {
        Tile* tile = GetTile(piecePosition);

        if (tile->piece != nullptr)
        {
            GetTile(piecePosition)->piece->Draw();
        }
    }

    //Make the dragging piece be priorized
    if (isDraggingPiece)
    {
        //Draw the legal moves as little dots
        for (auto& legalMove : draggingPieceLegalMoves)
        {
            //Create a dot in the legal move position and draw it
            sf::CircleShape dot(15, 35);

            //Check if the legal move is a capture
            if (GetTile(legalMove)->hasPiece)
            {
                dot.setRadius(35);
                dot.setFillColor(sf::Color::Transparent);
                dot.setOutlineThickness(10);
                dot.setOutlineColor(sf::Color(0, 0, 0, 80));
            }
            //Check if the legal move is not a capture
            else
            {
                dot.setFillColor(sf::Color(0, 0, 0, 80));
            }

            dot.setOrigin(sf::Vector2f(dot.getRadius(), dot.getRadius()));
            dot.setPosition(sf::Vector2f(
                legalMove.x * BOARD::TILE::SIZE + BOARD::TILE::SIZE / 2,
                legalMove.y * BOARD::TILE::SIZE + BOARD::TILE::SIZE / 2
            ));

            window->draw(dot);
        }

        draggingPiece->Draw();
    }

    //Draw the promotion piece selector
    for (auto& selector : promotionPieceSelectors)
    {
        selector->Draw();
    }
}

void Board::MovePiece(std::shared_ptr<Piece> piece, const sf::Vector2u target)
{
    //Get the tile of the current piece position and its target
    Tile* targetTile = GetTile(target);
    Tile* currentTile = GetTile(piece->GetPosition());

    //Check for "en passant"
    //Check if the piece is a pawn (only pawns can be captured on passant)
    if (piece->GetType() == PIECE::Type::Pawn)
    {
        //Check if it's the first piece's move
        if (piece->IsFirstMove())
        {
            //Get the difference between the target piece's position and the current piece's position in the y axis
            const int differenceY = int(piece->GetTargetPosition().y) - int(piece->GetPosition().y);

            //Check if the pawn moved more than 2 squares
            if (abs(differenceY) > 1)
            {
                piece->SetPassantCapturable(true);
            }
        }
        //Disable en passant
        else
        {
            piece->SetPassantCapturable(false);
        }
    }

    //Iterate through every piece position
    for (int i = 0; i < piecesPositions.size(); i++)
    {
        //Get the current position of the pieces positions vector based on the iterator
        auto& position = piecesPositions[i];

        //Get the piece of the current checking position
        auto& checkingPiece = GetTile(position)->piece;

        //Disable en passant if a move is done
        if (checkingPiece->GetType() == PIECE::Type::Pawn)
        {
            if (!checkingPiece->IsFirstMove())
            {
                checkingPiece->SetPassantCapturable(false);
            }
        }

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

    //Set the piece first move to false
    piece->SetFirstMove(false);
}

void Board::Castle(std::shared_ptr<Piece> king)
{
    //Get the difference between the king piece's target position and current position x axis
    const int differenceX = int(king->GetTargetPosition().x) - int(king->GetPosition().x);

    //Create the rook variable the king is castling with
    std::shared_ptr<Piece> rook;

    //Create a variable to check all the tiles of the left or right sides until it gets the rook variable
    sf::Vector2u currentPosition = king->GetTargetPosition();

    //Make a while loop until the rook variable is not a null variable anymore
    while (true)
    {
        //Update the current position variable
        currentPosition.x += Sign(differenceX);

        //Get the tile of the current position
        Tile* tile = GetTile(currentPosition);

        //Check if the current position tile has a piece
        if (tile->hasPiece)
        {
            //Update the rook variable to be the current position tile piece's variable
            rook = tile->piece;
            break;
        }
    }

    //Move the rook to the side of the king based on the castle side
    MovePiece(rook, king->GetTargetPosition() + sf::Vector2u(-Sign(differenceX), 0));
}

//Make a pawn capture en passant
void Board::CaptureEnPassant(std::shared_ptr<Piece> piece)
{
    //Get the position of the captured piece
    const sf::Vector2i capturedPiecePosition = sf::Vector2i(
        piece->GetTargetPosition().x, piece->GetPosition().y
    );

    //Get the tile of the piece and the captured piece
    Tile* tile = GetTile(piece->GetPosition());
    Tile* capturedPieceTile = GetTile(sf::Vector2u(capturedPiecePosition));

    //Check if the captured piece tile exists (normaly it's impossible, I do this to avoid errors)
    if (capturedPieceTile->hasPiece)
    {
        //Get the captured piece member of the pieces positions vector
        const auto& capturedPieceMember = std::find(piecesPositions.begin(), piecesPositions.end(), capturedPieceTile->piece->GetPosition());

        //Check if the captured piece member exists in the pieces positions vector to avoid errors
        if (piecesPositions.end() != capturedPieceMember)
        {
            //Delete the captured piece member from the pieces positions vector
            piecesPositions.erase(capturedPieceMember);
        }
    }

    //Delete the captured piece from the board
    capturedPieceTile->hasPiece = false;
    capturedPieceTile->piece.reset();
}

void Board::Promote(std::shared_ptr<Piece> piece)
{
    promoting = true;

    //Create a reference of the promotion piece selector based on the color
    auto& selector = promotionPieceSelectors[piece->GetColor()];

    //Set the selector's position to the promoted pawn and make the selector visible
    selector->Show();
    selector->SetFirstPiecePosition(piece->GetVisualTargetPosition());
}

void Board::StopPromoting()
{
    //Set the promoting variable to false
    promoting = false;

    //Get the promoted pawn member based on if the piece's position is on the last rank
    const auto& promotedPawnMember = std::find_if(piecesPositions.begin(), piecesPositions.end(), [&](const sf::Vector2u& position)
        {
            return (GetTile(position)->piece->GetColor() == PIECE::Color::White && position.y == 0) ||
                (GetTile(position)->piece->GetColor() == PIECE::Color::Black && position.y == BOARD::ROWS - 1);
        });

    //Check if the promoted pawn exists (to avoid errors)
    if (piecesPositions.end() != promotedPawnMember)
    {
        //Get the iterator of the promoted pawn in the pieces positions vector
        const unsigned int iterator = std::distance(piecesPositions.begin(), promotedPawnMember);

        //Get the piece variable of the promoted pawn
        auto& promotedPawn = GetTile(piecesPositions[iterator])->piece;

        //Set the promoted pawn to the selected piece selection
        promotedPawn->SetType(promotionPieceSelectors[promotedPawn->GetColor()]->GetLastPieceSelection());
    }
}

const bool Board::IsOnScreen(const sf::Vector2u position) const
{
    //Check if the given position is on screen
    return (position.x >= 0 && position.x < BOARD::COLUMNS &&
        position.y >= 0 && position.y < BOARD::ROWS);
}
