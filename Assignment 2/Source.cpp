#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

/*Game Constants*/
const int gameBoardSize{ 10 };
enum class gamePieces { hiddenEmpty, revealedEmpty, hiddenMine, revealedMine };
struct gameSlot
{
	gamePieces piece{ gamePieces::hiddenEmpty };
	bool flagged{ false };
};


/*GUI Functions*/
void splashScreen();
void displayGameState(const  std::vector<gameSlot>& gameBoard, bool revealMines = false);
void displayGameDone(const  std::vector<gameSlot>& gameBoard);

/* Engine Functions*/

std::vector<gameSlot> boardSetup();
void changeGameState(std::vector<gameSlot>& gameBoard);
bool isGameDone(const  std::vector<gameSlot>& gameBoard);

int countMines(int row, int column, const  std::vector<gameSlot>& gameBoard);

int boardIndex(int row, int column);

int main()
{

	char playAgain{ 'y' };
	while (playAgain == 'y' || playAgain == 'Y')
	{
		std::vector<gameSlot>&& gameBoard{ boardSetup() };
		while (!isGameDone(gameBoard))
		{
			displayGameState(gameBoard);
			changeGameState(gameBoard);
		}
		displayGameDone(gameBoard);
		std::cout << "\nDo you wish to play another game? (y/n): ";
		std::cin >> playAgain;

	}
}
void splashScreen()
{
	std::cout << "Mine Sweeper!" << std::endl;
	std::cout << std::endl;
	std::cout << "Your name here (2019)" << std::endl;
	std::cout << "CPSC 2377, Game Programming, Quest 0" << std::endl;
	std::cout << "UALR, Computer Science Dept." << std::endl;
	std::cout << std::endl;
	std::cout << "INSTRUCTIONS:" << std::endl;
	std::cout << std::endl;
	std::cout << "Clear the minefield without hitting a mine!" << std::endl;
	system("PAUSE");
}

void displayGameState(const  std::vector<gameSlot>& gameBoard, bool revealMines)
{
	char rowName{ 'A' };
	int colName{ 1 };
	system("CLS");

	std::cout << "   ";
	for (int i{ 0 }; i < gameBoardSize; i++)
	{
		std::cout << colName << " ";
		colName++;
	}
	std::cout << std::endl;
	for (int row{ 0 }; row < gameBoardSize; row++)
	{

		std::cout << rowName << "| ";
		rowName++;
		for (int column{ 0 }; column < gameBoardSize; column++)
		{
			if (gameBoard[boardIndex(row, column)].flagged) {
				std::cout << "f ";
			}
			else {
				switch (gameBoard[boardIndex(row, column)].piece)
				{
				case gamePieces::hiddenEmpty:
					std::cout << "- ";
					break;
				case gamePieces::hiddenMine:
					if (revealMines) std::cout << "* ";
					else std::cout << "- ";
					break;
				case gamePieces::revealedMine:
					std::cout << "* ";
					break;
				case gamePieces::revealedEmpty:
				{
					int numNeighbors{ countMines(row, column, gameBoard) };

					if (numNeighbors == 0)
						std::cout << "  ";
					else
						std::cout << numNeighbors << " ";
					break;
				}
				}
			}
		}
		std::cout << "\n";
	}
}

void displayGameDone(const  std::vector<gameSlot>& gameBoard)
{
	displayGameState(gameBoard, true);
	std::vector<gameSlot>::iterator it = std::find_if(gameBoard.begin(), gameBoard.end(), it->piece == gamePieces::revealedMine);
	for (auto slot : gameBoard)
	{
		if (slot.piece == gamePieces::revealedMine)
		{
			std::cout << "The mine exploded.... You are dead!!!!\n";
			std::cout << "Better luck next time!";
			return;
		}
	}
	std::cout << "You have cleared the mine field!\n";
	std::cout << "You are loved by all! Use your power for good!";
}

std::vector<gameSlot> boardSetup()
{
	std::vector<gameSlot> gameBoard(gameBoardSize * gameBoardSize);

	static std::random_device seed;
	static std::default_random_engine e(seed());
	static std::bernoulli_distribution mined(.25);

	for (auto& slot : gameBoard)
	{
		if (mined(e))
		{
			slot.piece = gamePieces::hiddenMine;
			slot.flagged = false;
		}
		else
		{
			slot.piece = gamePieces::hiddenEmpty;
			slot.flagged = false;
		}
	}
	return gameBoard;
}

int boardIndex(int row, int column)
{
	return row * gameBoardSize + column;
}

void changeGameState(std::vector<gameSlot>& gameBoard)
{

	char row{ 'Z' };
	int column{ -1 };
	int numRow{ 0 };
	int flagged = -1;
	do
	{
		while (row < 'A' || row > 'A' + gameBoardSize)
		{
			std::cout << "Choose a row: ";
			std::cin >> row;
		}
		numRow = row - 'A';
		while (column < 0 || column > gameBoardSize)
		{
			std::cout << "Choose a column: ";
			std::cin >> column;
			column--;
		}
		while (flagged < 0 || flagged>1) {
			if (gameBoard[boardIndex(numRow, column)].piece != gamePieces::revealedEmpty) {
				std::cout << "Do you want to flag this mine?\n 1/yes 0/no";
				std::cin >> flagged;
			}
			else {
				std::cout << "You can't flag this spot ";
				flagged = 0;
			}
		}
		break;
	} while (gameBoard[boardIndex(numRow, column)].piece == gamePieces::revealedEmpty);
	if (flagged == 0) {
		switch (gameBoard[boardIndex(numRow, column)].piece)
		{
		case gamePieces::hiddenEmpty:
			gameBoard[boardIndex(numRow, column)].piece = gamePieces::revealedEmpty;
			break;
		case gamePieces::hiddenMine:
			gameBoard[boardIndex(numRow, column)].piece = gamePieces::revealedMine;
			break;
		}
	}
	else {
		gameBoard[boardIndex(numRow, column)].flagged = true;
	}
	
}

bool isGameDone(const  std::vector<gameSlot>& gameBoard)
{
	bool stillEmpty{ true };
	for (auto slot : gameBoard)
	{
		switch (slot.piece)
		{
		case gamePieces::hiddenEmpty:
			stillEmpty = false;
			break;
		case gamePieces::revealedMine:
			return true;
			break;
		}
	}
	return stillEmpty;
}

int countMines(int row, int column, const  std::vector<gameSlot>& gameBoard)
{
	int mineCount{ 0 };
	for (int neighborRow{ -1 }; neighborRow <= 1; neighborRow++)
	{
		for (int neighborColumn{ -1 }; neighborColumn <= 1; neighborColumn++)
		{
			if ((neighborColumn != 0 || neighborRow != 0)
				&& row + neighborRow >= 0 && row + neighborRow < gameBoardSize
				&& column + neighborColumn >= 0 && column + neighborColumn < gameBoardSize
				&& (gameBoard[boardIndex(row + neighborRow, column + neighborColumn)].piece == gamePieces::hiddenMine
					|| gameBoard[boardIndex(row + neighborRow, column + neighborColumn)].piece == gamePieces::revealedMine))
			{
				mineCount++;
			}
		}
	}
	return mineCount;
}
