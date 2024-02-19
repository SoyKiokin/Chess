
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>

using std::cout;

namespace PIECE
{
	enum Type { King, Queen, Bishop, Knight, Rook, Pawn };
	enum Color { White, Black };

	const std::array<Type, 4> PROMOTION_PIECES = { Type::Queen, Type::Bishop, Type::Knight, Type::Rook };
}

namespace BOARD
{
	constexpr unsigned int ROWS = 8;
	constexpr unsigned int COLUMNS = 8;

	namespace TILE
	{
		constexpr unsigned int SIZE = 96;
		const sf::Color BLACK_COLOR = sf::Color(153, 88, 55);
		const sf::Color WHITE_COLOR = sf::Color(224, 185, 144);
	}

	const std::string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
}

namespace WINDOW
{
	const sf::Vector2u SIZE = sf::Vector2u(BOARD::COLUMNS * BOARD::TILE::SIZE, BOARD::ROWS * BOARD::TILE::SIZE);
	const std::string TITLE = "Chess!";
	constexpr sf::Uint8 STYLE = sf::Style::Close;
}

template<typename T>
const T Sign(const T value)
{
	return (value > 0) - (value < 0);
}
