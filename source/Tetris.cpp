#include "Tetris.h"


void Tetris::StartGame(int windowWidth, int windowHeight,
		int boardWidth, int boardHeight, int gridSize)
{
	this -> windowWidth = windowWidth;
	this -> windowHeight = windowHeight;
	this -> boardWidth = boardWidth;
	this -> boardHeight = boardHeight;
	this -> gridSize = gridSize;

	// constructor of the class which creates a menu
	window.create(sf::VideoMode(windowWidth, windowHeight), "Tetris",
		sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));
	window.setKeyRepeatEnabled(false);

	// load textures from files
	sf::Texture inactivePlay;
	inactivePlay.loadFromFile("res/play_button_inactive.png");
	sf::Texture activePlay;
	activePlay.loadFromFile("res/play_button_active.png");

	sf::Texture inactiveInstruction;
	inactiveInstruction.loadFromFile("res/instructions_button_inactive.png");
	sf::Texture activeInstruction;
	activeInstruction.loadFromFile("res/instructions_button_active.png");

	sf::Texture inactiveExit;
	inactiveExit.loadFromFile("res/exit_button_inactive.png");
	sf::Texture activeExit;
	activeExit.loadFromFile("res/exit_button_active.png");

	// create sprites from textures
	sf::Sprite playButton;
	playButton.setTexture(inactivePlay);
	playButton.setPosition(205, 200);

	sf::Sprite instructionButton;
	instructionButton.setTexture(inactiveInstruction);
	instructionButton.setPosition(35, 300);

	sf::Sprite exitButton;
	exitButton.setTexture(inactiveExit);
	exitButton.setPosition(220, 400);

	// create coordinates of the sprites
	sf::FloatRect playButtonCoords = playButton.getGlobalBounds();
	float playButtonLeft = playButtonCoords.left;
	float playButtonRight = playButtonCoords.left + playButtonCoords.width;
	float playButtonTop = playButtonCoords.top;
	float playButtonBottom = playButtonCoords.top + playButtonCoords.height;

	sf::FloatRect instructionButtonCoords = instructionButton.getGlobalBounds();
	float instructionButtonLeft = instructionButtonCoords.left;
	float instructionButtonRight = instructionButtonCoords.left + instructionButtonCoords.width;
	float instructionButtonTop = instructionButtonCoords.top;
	float instructionButtonBottom = instructionButtonCoords.top + instructionButtonCoords.height;

	sf::FloatRect exitButtonCoords = exitButton.getGlobalBounds();
	float exitButtonLeft = exitButtonCoords.left;
	float exitButtonRight = exitButtonCoords.left + exitButtonCoords.width;
	float exitButtonTop = exitButtonCoords.top;
	float exitButtonBottom = exitButtonCoords.top + exitButtonCoords.height;

	// start the main loop of the menu

	while (window.isOpen())
	{
		// handle events
		sf::Event menuEvent;
		while (window.pollEvent(menuEvent))
		{
			if (menuEvent.type == sf::Event::Closed)
				window.close();

			// handle mouse movement over menubuttons
			if (menuEvent.type == sf::Event::MouseMoved)
			{
				int x = menuEvent.mouseMove.x;
				int y = menuEvent.mouseMove.y;

				if (x > playButtonLeft && x < playButtonRight &&
					y > playButtonTop && y < playButtonBottom)
					playButton.setTexture(activePlay);

				else if (x > instructionButtonLeft && x < instructionButtonRight &&
					y > instructionButtonTop && y < instructionButtonBottom)
					instructionButton.setTexture(activeInstruction);

				else if (x > exitButtonLeft && x < exitButtonRight &&
					y > exitButtonTop && y < exitButtonBottom)
					exitButton.setTexture(activeExit);

				else
				{
					playButton.setTexture(inactivePlay);
					instructionButton.setTexture(inactiveInstruction);
					exitButton.setTexture(inactiveExit);
				}
			}

			// handle clicks on menubuttons
			if (menuEvent.type == sf::Event::MouseButtonPressed)
			{
				// the user pressed the left mouse button
				if (menuEvent.mouseButton.button == sf::Mouse::Left)
				{
					int x = menuEvent.mouseButton.x;
					int y = menuEvent.mouseButton.y;

					// the user clicked on play
					if (x > playButtonLeft && x < playButtonRight &&
						y > playButtonTop && y < playButtonBottom)
						Play();

					// the user clicked on the instructions
					if (x > instructionButtonLeft && x < instructionButtonRight &&
						y > instructionButtonTop && y < instructionButtonBottom)
						Instructions();

					// the user clicked on exit
					if (x > exitButtonLeft && x < exitButtonRight &&
						y > exitButtonTop && y < exitButtonBottom)
						window.close();
				}
			}
		}

		// refresh the screen
		window.clear();
		window.draw(playButton);
		window.draw(instructionButton);
		window.draw(exitButton);
		window.display();
	}
}


void Tetris::Play()
{
	// create the gameboard
	board.Create(boardWidth, boardHeight, gridSize);

	// repeatdelay is how often an event gets repeated,
	// when a movement key is being pressed continuously

	sf::Time moveDownDelay(sf::milliseconds(500));
	sf::Time repeatDelay(sf::milliseconds(75));

	sf::Clock gameClock;
	sf::Clock inputClock;
	
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				// if the user pressed esc, go back to the menu
				if (event.key.code == sf::Keyboard::Escape)
					return;

				if (event.key.code == sf::Keyboard::Left)
				{
					board.MoveLeft();
					inputClock.restart();
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					board.MoveRight();
					inputClock.restart();
				}

				if (event.key.code == sf::Keyboard::Down)
					moveDownDelay = sf::Time(sf::milliseconds(50));
				if (event.key.code == sf::Keyboard::Space)
					board.AutoRotate();
				if (event.key.code == sf::Keyboard::Q)
					board.Rotate(Direction::LEFT);
				if (event.key.code == sf::Keyboard::E)
					board.Rotate(Direction::RIGHT);
			}

			if (event.type == sf::Event::KeyReleased)
				if (event.key.code == sf::Keyboard::Down)
					moveDownDelay = sf::Time(sf::milliseconds(500));
		}

		if (inputClock.getElapsedTime() > repeatDelay)
		{
			// inputclock is restarted if it goes above a limit,
			// then the left-right keys will be checked
			// the consequence of this is that the left-right movement
			// is continuous when the user keeps a key pressed

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				board.MoveLeft();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				board.MoveRight();

			inputClock.restart();
		}

		// make the piece move and reset the internal timer
		if (gameClock.getElapsedTime() > moveDownDelay)
		{
			board.MoveDown();
			gameClock.restart();
		}

		// refresh the screen
		window.clear();
		board.DrawAll(window);
		window.display();

		// check the gameover state
		if (board.IsGameOver())
			return;
	}
}


void Tetris::Instructions()
{

    sf::Texture instructions_texture;
    instructions_texture.loadFromFile("res/instructions.png");
    sf::Sprite instructions(instructions_texture);

	window.clear();
    window.draw(instructions);
	window.display();

	while (window.isOpen())
	{
		sf::Event instructionEvent;
		while (window.pollEvent(instructionEvent))
		{
			if (instructionEvent.type == sf::Event::KeyPressed ||
				instructionEvent.key.code == sf::Keyboard::Escape)
				return;
		}
	}
}
