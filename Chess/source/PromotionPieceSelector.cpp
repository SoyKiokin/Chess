
#include "PromotionPieceSelector.hpp"

//Update the shadow's position based on the color
void PromotionPieceSelector::UpdateShadowPosition()
{
	switch (color)
	{
	case PIECE::Color::White:
		shadow.setPosition(pieces[0].background.getPosition());
		break;

	default:
		shadow.setPosition(pieces[0].background.getPosition() - sf::Vector2f(0,
			BOARD::TILE::SIZE * (std::max<int>(pieces.size() - 1, 0))
		));
		break;
	}
}

const bool& PromotionPieceSelector::MouseJustReleased()
{
	//Check if the left mouse button is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//Set the mouse pressed previously variable to false
		mousePressedPreviously = true;
	}
	else
	{
		//Check if the mouse left button was pressed on the last frame
		if (mousePressedPreviously)
		{
			//Set the mouse pressed previously variable to false
			mousePressedPreviously = false;
			return true;
		}
	}

	return false;
}

PromotionPieceSelector::PromotionPieceSelector(sf::RenderWindow* window, const PIECE::Color color) :
	window(window),
	color(color)
{

}

PromotionPieceSelector::~PromotionPieceSelector()
{

}

void PromotionPieceSelector::SetTexture(const sf::Texture& texture)
{
	//Create a value that can be -1 or 1, the value depends on the color of the piece
	const int colorMultiplier = color == PIECE::Color::White ? 1 : -1;

	//Create all the pieces one by one
	for (unsigned int i = 0; i < pieces.size(); i++)
	{
		//Create a variable that will reference the current piece based on the iterator
		auto& piece = pieces[i];

		//Set the type of the piece
		piece.type = PIECE::PROMOTION_PIECES[i];

		//Create a rectangle white of the size
		piece.background.setSize(sf::Vector2f(BOARD::TILE::SIZE, BOARD::TILE::SIZE));
		piece.background.setFillColor(sf::Color::White);
		piece.background.setPosition(sf::Vector2f(0, BOARD::TILE::SIZE * colorMultiplier * i));

		//Set the texture of the piece sprite
		piece.sprite.setTexture(texture);

		//Set the texture rectangle based on the color and type
		const sf::Vector2u textureSize = texture.getSize();
		const sf::Vector2f pieceTextureSize = sf::Vector2f(textureSize.x / 6, textureSize.y / 2);

		//Set the texture rect based on the color and type
		piece.sprite.setTextureRect(sf::IntRect(
			pieceTextureSize.x * piece.type, pieceTextureSize.y * color, pieceTextureSize.x, pieceTextureSize.y
		));

		//Set the texture scale based on the board tile size
		piece.sprite.setScale(sf::Vector2f(
			float(BOARD::TILE::SIZE) / pieceTextureSize.x, float(BOARD::TILE::SIZE) / pieceTextureSize.y
		));

		piece.sprite.setPosition(piece.background.getPosition());
	}

	//Create the shadow of the background
	shadow.setSize(sf::Vector2f(BOARD::TILE::SIZE, BOARD::TILE::SIZE * pieces.size()));
	shadow.setFillColor(sf::Color(0, 0, 0, 0));
	shadow.setOutlineThickness(SHADOW_THICKNESS);
	shadow.setOutlineColor(sf::Color(0, 0, 0, 50));

	//Set the shadow's position
	UpdateShadowPosition();
}

const sf::Texture& PromotionPieceSelector::GetTexture() const
{
	return *pieces[0].sprite.getTexture();
}

void PromotionPieceSelector::SetFirstPiecePosition(const sf::Vector2f position)
{
	for (int i = 0; i < pieces.size(); i++)
	{
		//Create a variable that will reference the current piece based on the iterator
		auto& piece = pieces[i];

		//Create a value that can be -1 or 1, the value depends on the color of the piece
		const int colorMultiplier = color == PIECE::Color::White ? 1 : -1;

		//Set the pieces position and background postiion
		piece.background.setPosition(position + sf::Vector2f(0, (int(BOARD::TILE::SIZE) * colorMultiplier) * i));
		piece.sprite.setPosition(piece.background.getPosition());
	}

	//Set the shadow's position
	UpdateShadowPosition();
}

const sf::Vector2f& PromotionPieceSelector::GetFirstPiecePosition() const
{
	return pieces[0].sprite.getPosition();
}

//Show the selector on screen
void PromotionPieceSelector::Show()
{
	show = true;
}

//Hide the selector on screen
void PromotionPieceSelector::Hide()
{
	show = false;
}

//Check if the selector is visible
const bool& PromotionPieceSelector::IsVisible() const
{
	return show;
}

//Get the last piece selection
const PIECE::Type PromotionPieceSelector::GetLastPieceSelection() const
{
	return lastPieceSelection;
}

void PromotionPieceSelector::Update(const float& deltaTime)
{
	//Check if the selector is visible
	if (!show)
	{
		//Stop the function
		return;
	}

	//Get the view mouse position of the window
	const sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	//Check if a piece is hovered
	for (auto& piece : pieces)
	{
		//Set the color of the hovered by the mouse piece background based on the mouse position
		if (piece.background.getGlobalBounds().contains(mousePosition))
		{
			//Change the background color to gray
			piece.background.setFillColor(sf::Color(200, 200, 200));

			//Check if the mouse left button is just pressed
			if (MouseJustReleased())
			{
				//Hide the selector
				show = false;

				//Set the last piece selection
				lastPieceSelection = piece.type;
			}
		}
		else
		{
			//Change the background color to white
			piece.background.setFillColor(sf::Color(255, 255, 255));
		}
	}
}

void PromotionPieceSelector::Draw()
{
	//Don't show the sprites when the show variable is set to false
	if (!show)
	{
		return;
	}

	//Draw the shadow
	window->draw(shadow);

	//Draw the pieces and backgrounds
	for (auto& piece : pieces)
	{
		window->draw(piece.background);
		window->draw(piece.sprite);
	}
}
