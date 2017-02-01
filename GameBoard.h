#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "GamePiece.h"

class GameBoard
{
private:
	bool gameOver;
	int score;

	int columns;
	int rows;
	int gridSize;
	float blockSize;

	sf::RectangleShape **grid;
	GamePiece piece;

	bool CheckGameOver();

public:
	GameBoard() : columns(0), rows(0) {}
	void Create(int width, int height, int gridSize);
	~GameBoard();

	Piece::letter GetRandomPiece();

	void ClearPiece();
	void PlacePiece();
	void DrawAll(sf::RenderWindow &window);

	// having separate moving functions is good
	// because detecting collisions become easier
	void MoveLeft();
	void MoveDown();
	void MoveRight();
	
	// this function also returns whether the rotation was succsessful,
	// so it can be reused in AutoRotate
	bool Rotate(Direction::specifier direction);
	void AutoRotate();

	void BuildPiece();
	void NextPiece();
	bool IsRowFull(int row);
	void ClearRow(int row);
	void PushDown(int row);
	void ClearFullRows();
	
	int GetScore() { return score; }
	bool IsGameOver() { return gameOver; }
};

#endif
