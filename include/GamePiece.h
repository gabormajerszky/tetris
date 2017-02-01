#ifndef GAMEPIECE_H
#define GAMEPIECE_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace Direction
{
	enum specifier {LEFT = -1, RIGHT = 1};
}

namespace Piece
{
	enum letter {I, K, L_LEFT, L_RIGHT, N_LEFT, N_RIGHT, O, NUM_PIECES};
}

class GamePiece
{
private:
	std::vector<sf::Vector2i> blocks;
	Piece::letter pieceType;

public:
	GamePiece() {}
	void InitPiece(Piece::letter letter, int midCol);

	Piece::letter GetType() const;
	std::vector<sf::Vector2i> GetBlocks() const;
	std::vector<sf::Vector2i> RotatedBlocks(Direction::specifier direction, int center = 1);

	void SetBlocks(std::vector<sf::Vector2i> blocks);

	void MoveBlocks(Direction::specifier direction);
	void MoveBlocksDown();

	friend std::ostream& operator<<(std::ostream &out, const GamePiece &piece);
};

#endif
