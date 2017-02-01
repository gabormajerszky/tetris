#include "GameBoard.h"


bool GameBoard::CheckGameOver()
{
	// this function needs to be called when the new piece
	// has been created, but hasn't been drawn yet

	std::vector<sf::Vector2i> blocks = piece.GetBlocks();
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		if (grid[blocks[i].x][blocks[i].y].getFillColor() == sf::Color::Blue)
			return true;
	}
	return false;
}


void GameBoard::Create(int width, int height, int gridSize)
{
	gameOver = false;
	score = 0;

	this -> gridSize = gridSize;
	float blockSize = 0.98f * gridSize;
	columns = width / gridSize;
	rows = height / gridSize;

	// create a 2d-array of "blocks"
	grid = new sf::RectangleShape*[rows];
	for (int i = 0; i < rows; ++i)
		grid[i] = new sf::RectangleShape[columns];

	// initialize the array
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
		{
			grid[i][j] = sf::RectangleShape(sf::Vector2f(blockSize, blockSize));
			grid[i][j].setPosition(static_cast<float>(j * gridSize), static_cast<float>(i * gridSize));
			grid[i][j].setFillColor(sf::Color::Black);
		}

	// start a seed for the PRNG
	unsigned int seed = static_cast<unsigned int>(time(0));
	srand(seed);
	
	piece.InitPiece(GetRandomPiece(), columns / 2);
	PlacePiece();
}


GameBoard::~GameBoard()
{

    if (rows == 0)
        return;

	// clean up the 2d-array
	for (int i = 0; i < rows; ++i)
		delete[] grid[i];
	delete[] grid;

}


void GameBoard::ClearPiece()
{
	std::vector<sf::Vector2i> blocks = piece.GetBlocks();
	for (size_t i = 0; i < blocks.size(); ++i)
		grid[blocks[i].x][blocks[i].y].setFillColor(sf::Color::Black);
}


Piece::letter GameBoard::GetRandomPiece()
{
	return static_cast<Piece::letter>(rand() % Piece::NUM_PIECES);
}


void GameBoard::PlacePiece()
{
	// place a piece in the 2d-array of tiles
	std::vector<sf::Vector2i> blocks = piece.GetBlocks();
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		grid[blocks[i].x][blocks[i].y].setFillColor(sf::Color::Red);
	}
}


void GameBoard::DrawAll(sf::RenderWindow &window)
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			window.draw(grid[i][j]);
}


void GameBoard::MoveLeft()
{
	std::vector<sf::Vector2i> blocks = piece.GetBlocks();
	// return if any of the blocks are at the left side of the screen
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		if (blocks[i].y == 0 ||
			grid[blocks[i].x][blocks[i].y - 1].getFillColor() == sf::Color::Blue)
			return;
	}
	ClearPiece();
	piece.MoveBlocks(Direction::LEFT);
	PlacePiece();
}


void GameBoard::MoveDown()
{
	std::vector<sf::Vector2i> blocks = piece.GetBlocks();
	// return if any of the blocks are at the bottom of the screen
	for (size_t i = 0; i < blocks.size(); ++i)
		if (blocks[i].x == rows - 1 ||
			grid[blocks[i].x + 1][blocks[i].y].getFillColor() == sf::Color::Blue)
		{
			BuildPiece();
			ClearFullRows();
			NextPiece();
			return;
		}
	ClearPiece();
	piece.MoveBlocksDown();
	PlacePiece();
}


void GameBoard::MoveRight()
{
	std::vector<sf::Vector2i> blocks = piece.GetBlocks();
	// return if any of the blocks are at the left side of the screen
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		if (blocks[i].y == columns - 1 ||
			grid[blocks[i].x][blocks[i].y + 1].getFillColor() == sf::Color::Blue)
			return;
	}
	ClearPiece();
	piece.MoveBlocks(Direction::RIGHT);
	PlacePiece();
}


bool GameBoard::Rotate(Direction::specifier direction)
{
	// there is no point in rotating the square :)
	if (piece.GetType() == Piece::O)
		// although the rotation didn't occour,
		// we can treat this situation as if it did
		return true;

	// create a copy of the rotated blocks
	std::vector<sf::Vector2i> rotatedBlocks = piece.RotatedBlocks(direction);

	// check to see if we would hit a wall or another piece,
	// if we actually rotated the piece
	for (size_t i = 0; i < rotatedBlocks.size(); ++i)
	{
		if (rotatedBlocks[i].x < 0 || rotatedBlocks[i].x > rows - 1 ||
			rotatedBlocks[i].y < 0 || rotatedBlocks[i].y > columns -1 ||
			grid[rotatedBlocks[i].x][rotatedBlocks[i].y].getFillColor() == sf::Color::Blue)
			// couldn't rotate the piece without colliding
			return false;
	}

	ClearPiece();
	piece.SetBlocks(rotatedBlocks);
	PlacePiece();
	return true;
}


void GameBoard::AutoRotate()
{
	if (!Rotate(Direction::RIGHT))
		Rotate(Direction::LEFT);
}


void GameBoard::BuildPiece()
{
	std::vector<sf::Vector2i> blocks = piece.GetBlocks();
	for (size_t i = 0; i < blocks.size(); ++i)
		grid[blocks[i].x][blocks[i].y].setFillColor(sf::Color::Blue);
}


void GameBoard::NextPiece()
{
	piece = GamePiece();
	piece.InitPiece(GetRandomPiece(), columns / 2);

	// game over needs to be checked here,
	// because the new piece is created but not drawn yet
	// (and checking is based on the colors of the tiles)
	gameOver = CheckGameOver();

	PlacePiece();
}


bool GameBoard::IsRowFull(int row)
{
	for (int j = 0; j < columns; ++j)
		if (grid[row][j].getFillColor() != sf::Color::Blue)
			return false;
	score += 100;
	return true;
}


void GameBoard::ClearRow(int row)
{
	for (int j = 0; j < columns; ++j)
		grid[row][j].setFillColor(sf::Color::Black);
}


void GameBoard::PushDown(int row)
{
	// push down every block above an empty row

	for (int i = row; i > 0; --i)
		for (int j = 0; j < columns; ++j)
		{
			grid[i][j].setFillColor(grid[i - 1][j].getFillColor());
			grid[i - 1][j].setFillColor(sf::Color::Black);
		}
}


void GameBoard::ClearFullRows()
{
	if (IsRowFull(0))
		ClearRow(0);

	for (int i = 1; i < rows; ++i)
		if (IsRowFull(i))
		{
			ClearRow(i);
			PushDown(i);
		}
}
