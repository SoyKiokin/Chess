
#include "Piece.hpp"

//Reload/load the sprite
void Piece::LoadSprite()
{
	//Check if the texture is null before loading the sprite
	if (sprite.getTexture() == nullptr)
	{
		return;
	}

	//Set the texture rectangle based on the color and type
	const sf::Vector2u textureSize = sprite.getTexture()->getSize();
	const sf::Vector2f pieceTextureSize = sf::Vector2f(textureSize.x / 6, textureSize.y / 2);

	//Set the texture rect based on the color and type
	sprite.setTextureRect(sf::IntRect(
		pieceTextureSize.x * type, pieceTextureSize.y * color, pieceTextureSize.x, pieceTextureSize.y
	));

	//Set the texture scale based on the board tile size
	sprite.setScale(sf::Vector2f(
		float(BOARD::TILE::SIZE) / pieceTextureSize.x, float(BOARD::TILE::SIZE) / pieceTextureSize.y
	));
}

//Constructor
Piece::Piece(sf::RenderWindow* window) : 
	window(window)
{

}

//Destructor
Piece::~Piece()
{

}

//Set an external texture to the sprite
void Piece::SetTexture(sf::Texture& texture)
{
	sprite.setTexture(texture);
	LoadSprite();
}

//Get the sprite texture
const sf::Texture* Piece::GetTexture() const
{
	return sprite.getTexture();
}

//Set the piece's position based on the board
void Piece::SetPosition(const sf::Vector2u position)
{
	this->position = position;

	//Set visual position of the piece
	sprite.setPosition(sf::Vector2f(
		position.x * BOARD::TILE::SIZE, position.y * BOARD::TILE::SIZE
	));
}

//Get the piece's position based on the board
const sf::Vector2u& Piece::GetPosition() const
{
	return position;
}

//Get the visual piece's position
const sf::Vector2f& Piece::GetVisualPosition() const
{
	return sf::Vector2f(position.x * BOARD::TILE::SIZE, position.y * BOARD::TILE::SIZE);
}

//Set the piece's target position
void Piece::SetTargetPosition(const sf::Vector2u position)
{
	targetPosition = position;
}

const sf::Vector2u& Piece::GetTargetPosition() const
{
	return targetPosition;
}

//Get the visual piece's target position
const sf::Vector2f& Piece::GetVisualTargetPosition() const
{
	return sf::Vector2f(targetPosition.x * BOARD::TILE::SIZE, targetPosition.y * BOARD::TILE::SIZE);
}

void Piece::SetFirstMove(const bool isFirstMove)
{
	firstMove = isFirstMove;
}

const bool& Piece::IsFirstMove() const
{
	return firstMove;
}

void Piece::Drag(const sf::Vector2f draggingPosition)
{
	//Create the offset of the dragging position to center the piece
	const sf::FloatRect rectangle = sprite.getGlobalBounds();
	const sf::Vector2f offset = sf::Vector2f(rectangle.width / 2, rectangle.height / 2);

	//Set the visual sprite position to the dragging position
	sprite.setPosition(draggingPosition - offset);
}

//Check if the piece can be captured en passant (only for pawns)
const bool& Piece::IsEnPassantCapturable() const
{
	return enPassantCapturable;
}

//Set a value for the en passant capture variable
void Piece::SetPassantCapturable(const bool value)
{
	enPassantCapturable = value;
}

//Set the type of the piece
void Piece::SetType(const PIECE::Type type)
{
	this->type = type;
	LoadSprite();
}

//Get the type of the piece
const PIECE::Type& Piece::GetType() const
{
	return type;
}

//Set the color of the piece (black or white)
void Piece::SetColor(const PIECE::Color color)
{
	this->color = color;
	LoadSprite();
}

//Get the color of the piece (black or white)
const PIECE::Color& Piece::GetColor() const
{
	return color;
}

//Get the opposite color of the piece
const PIECE::Color& Piece::GetOppositeColor() const
{
	return color == PIECE::Color::White ? PIECE::Color::Black : PIECE::Color::White;
}

void Piece::Draw()
{
	window->draw(sprite);
}
