#include "GamePiece.h"


void GamePiece::InitPiece(Piece::letter letter, int midCol)
{
	// every piece consists of four blocks
	blocks.resize(4);
	pieceType = letter;

	switch (letter)
	{
	case Piece::I:
		blocks[0] = sf::Vector2i(0, midCol - 2);
		blocks[1] = sf::Vector2i(0, midCol - 1);
		blocks[2] = sf::Vector2i(0, midCol);
		blocks[3] = sf::Vector2i(0, midCol + 1);
		break;
	case Piece::K:
		blocks[0] = sf::Vector2i(0, midCol - 1);
		blocks[1] = sf::Vector2i(0, midCol);
		blocks[2] = sf::Vector2i(0, midCol + 1);
		blocks[3] = sf::Vector2i(1, midCol);
		break;
	case Piece::L_LEFT:
		blocks[0] = sf::Vector2i(0, midCol - 1);
		blocks[1] = sf::Vector2i(0, midCol);
		blocks[2] = sf::Vector2i(0, midCol + 1);
		blocks[3] = sf::Vector2i(1, midCol - 1);
		break;
	case Piece::L_RIGHT:
		blocks[0] = sf::Vector2i(0, midCol - 1);
		blocks[1] = sf::Vector2i(0, midCol);
		blocks[2] = sf::Vector2i(0, midCol + 1);
		blocks[3] = sf::Vector2i(1, midCol + 1);
		break;
	case Piece::N_LEFT:
		blocks[0] = sf::Vector2i(0, midCol - 1);
		blocks[1] = sf::Vector2i(0, midCol);
		blocks[2] = sf::Vector2i(1, midCol);
		blocks[3] = sf::Vector2i(1, midCol + 1);
		break;
	case Piece::N_RIGHT:
		blocks[0] = sf::Vector2i(0, midCol + 1);
		blocks[1] = sf::Vector2i(0, midCol);
		blocks[2] = sf::Vector2i(1, midCol - 1);
		blocks[3] = sf::Vector2i(1, midCol);
		break;
	case Piece::O:
		blocks[0] = sf::Vector2i(0, midCol - 1);
		blocks[1] = sf::Vector2i(0, midCol);
		blocks[2] = sf::Vector2i(1, midCol - 1);
		blocks[3] = sf::Vector2i(1, midCol);
		break;
	}
}


Piece::letter GamePiece::GetType() const
{
	return pieceType;
}


std::vector<sf::Vector2i> GamePiece::GetBlocks() const
{
	return blocks;
}


std::vector<sf::Vector2i> GamePiece::RotatedBlocks(
	Direction::specifier direction, int center)
{
	// return a copy of blocks after the rotation
	std::vector<sf::Vector2i> rotatedBlocks = blocks;
	for (size_t i = 0; i < rotatedBlocks.size(); ++i)
	{
		// the key idea to the rotation is that
		// the difference in rows after the rotation
		// will become the difference in columns before the rotation

		int rowDifference = direction * (rotatedBlocks[i].y - rotatedBlocks[center].y);
		int columnDifference = direction * (rotatedBlocks[center].x - rotatedBlocks[i].x);

		rotatedBlocks[i].x = rotatedBlocks[center].x + rowDifference;
		rotatedBlocks[i].y = rotatedBlocks[center].y + columnDifference;
	}
	return rotatedBlocks;
}


void GamePiece::SetBlocks(std::vector<sf::Vector2i> blocks)
{
	this -> blocks = blocks;
}


void GamePiece::MoveBlocks(Direction::specifier direction)
{
	for (size_t i = 0; i < blocks.size(); ++i)
		blocks[i] = sf::Vector2i(blocks[i].x, blocks[i].y + direction);
}


void GamePiece::MoveBlocksDown()
{
	for (size_t i = 0; i < blocks.size(); ++i)
		blocks[i] = sf::Vector2i(blocks[i].x + 1, blocks[i].y);
}


std::ostream& operator<<(std::ostream &out, const GamePiece &piece)
{
	out << "[ ";
	for (size_t i = 0; i < piece.blocks.size(); ++i)
		out << "(" << piece.blocks[i].x << ", " << piece.blocks[i].y << ") ";
	out << "]" << "    Type : " << piece.pieceType;
	return out;
}
