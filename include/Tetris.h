#ifndef TETRIS_H
#define TETRIS_H

#include "GameBoard.h"

class Tetris
{
private:
	int windowWidth;
	int windowHeight;
	int boardWidth;
	int boardHeight;
	int gridSize;
	sf::RenderWindow window;
	GameBoard board;

public:
	Tetris() {}
	void StartGame(int windowWidth, int windowHeight,
		int boardWidth, int boardHeight, int gridSize);
	void Play();
	void Instructions();
};

#endif
