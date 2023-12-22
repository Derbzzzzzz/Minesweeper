#include <conio.h>
#include <iostream>
#include <stdlib.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define BEGINNER 1
#define INTERMEDIATE 2
#define ADVANCED 3
#define QUIT 4

#define NOQUIT 0

#define BOMB '*'

using namespace std;

class Board
{
protected:
	int m_size;
	char* array;

	int index(int x, int y) const { return x * m_size + y; };

public:
	Board(int size)
	{
		m_size = size;
		array = new char[size * size]();
	}

	void free()
	{
		delete [] array;
	}

	char getVal(int x, int y) const { return array[index(x, y)]; };

	void setVal(int x, int y, char val) { array[index(x, y)] = val; };

	void printBoard()
	{
		cout << "\n";
		cout << "    ";

		for (int i = 0; i < m_size; i++)
		{
			cout << i << " ";
			if (i < 10)
			{
				cout << " ";
			}
		}

		cout << "\n";
		cout << "   ";

		for (int i = 0; i < m_size; i++)
		{
			cout << "___";
		}

		cout << "\n";

		for (int i = 0; i < m_size; i++)
		{
			cout << i;
			if (i < 10) 
			{
				cout << " ";
			}
			cout << "| ";

			for (int j = 0; j < m_size; j++)
			{
				cout << getVal(i, j) << "  ";
			}
			cout << "\n";
		}

		cout << "\n";
	}
};

class actualBoard : public Board
{
private:
	void placeBombs(int bombs)
	{
		unsigned int time_ui = unsigned int(time(NULL));
		srand(time_ui);

		while (bombs > 0)
		{
			int x = rand() % m_size;
			int y = rand() % m_size;

			if (getVal(x, y) != BOMB)
			{
				setVal(x, y, BOMB);
				bombs--;
			}
		}
	}

	void setBoard()
	{
		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				setVal(i, j, '0');
			}
		}
	}

	void setBombCounts()
	{
		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				if (getVal(i, j) != BOMB)
				{
					setVal(i, j, getBombs(i, j) + '0');
				}
			}
		}
	}

public:
	actualBoard(int size, int bombs) : Board(size)
	{
		setBoard();
		placeBombs(bombs);
		setBombCounts();
	}

	int getBombs(int x, int y)
	{
		int closeBombs = 0;
		bool top = (x == 0);
		bool bottom = (x == m_size - 1);
		bool left = (y == 0);
		bool right = (y == m_size - 1);

		// Bottom Left
		if (!bottom and !left and getVal(x + 1, y - 1) == BOMB) { closeBombs++; };

		// Left
		if (!left and getVal(x, y - 1) == BOMB) { closeBombs++; };

		// Top Left
		if (!top and !left and getVal(x - 1, y - 1) == BOMB) { closeBombs++; };

		// Top
		if (!top and getVal(x - 1, y) == BOMB) { closeBombs++; };

		// Top Right
		if (!top and !right and getVal(x - 1, y + 1) == BOMB) { closeBombs++; };

		// Right
		if (!right and getVal(x, y + 1) == BOMB) { closeBombs++; };

		// Bottom Right
		if (!bottom and !right and getVal(x + 1, y + 1) == BOMB) { closeBombs++; };

		// Bottom
		if (!bottom and getVal(x + 1, y) == BOMB) { closeBombs++; };

		return closeBombs;

	}
};

class userBoard : public Board
{
private:
	int safeSquares;
	void setBoard()
	{
		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				Board::setVal(i, j, '-');
			}
		}
	}
public:
	userBoard(int size, int bombs) : Board(size)
	{
		setBoard();
		safeSquares = m_size * m_size - bombs;
	}
	
	void setVal(int x, int y, char val)
	{
		Board::setVal(x, y, val);
		safeSquares--;
	}

	bool hasWon()
	{
		return safeSquares == 0;
	}
};


int getDifficultyLevel()
{
	cout << "Choose Difficulty Level:\n";
	cout << "1. Beginner\n";
	cout << "2. Intermediate\n";
	cout << "3. Advanced\n";
	cout << "4. Quit Game\n";

	while (true)
	{
		int difficulty;
		scanf_s("%d", &difficulty);

		if (difficulty == BEGINNER || difficulty == INTERMEDIATE || difficulty == ADVANCED || difficulty == QUIT)
		{
			system("cls");
			return difficulty;
		}

		cout << "Please Enter Valid Difficulty Level (1, 2, 3, or 4 to quit)\n";
	}
}

void getGameValues(int difficulty, int* size, int* bombs)
{
	if (difficulty == BEGINNER)
	{
		*size = 10;
		*bombs = 10;
	}
	else if (difficulty == INTERMEDIATE)
	{
		*size = 13;
		*bombs = 20;
	}
	else if (difficulty == ADVANCED)
	{
		*size = 15;
		*bombs = 40;
	}
	else
	{
		throw std::invalid_argument("recieved invalid difficulty level");
	}
}

void recSearch(int x, int y, int size, actualBoard& actualBoard, userBoard& userBoard)
{
	if (x < 0 or y < 0 or x >= size or y >= size or userBoard.getVal(x, y) != '-')
	{
		return;
	}
	
	int val = actualBoard.getVal(x, y);
	if (val != '0')
	{
		userBoard.setVal(x, y, val);
		return;
	}


	userBoard.setVal(x, y, '0');

	recSearch(x + 1, y + 1, size, actualBoard, userBoard);
	recSearch(x + 1, y, size, actualBoard, userBoard);
	recSearch(x + 1, y - 1, size, actualBoard, userBoard);
	recSearch(x, y + 1, size, actualBoard, userBoard);
	recSearch(x, y - 1, size, actualBoard, userBoard);
	recSearch(x - 1, y + 1, size, actualBoard, userBoard);
	recSearch(x - 1, y, size, actualBoard, userBoard);
	recSearch(x - 1, y - 1, size, actualBoard, userBoard);
}

int minesweeper()
{
	system("cls");

	int difficulty = getDifficultyLevel();

	if (difficulty == QUIT) { return QUIT; };

	int size, bombs;

	getGameValues(difficulty, &size, &bombs);

	actualBoard actualBoard{ size, bombs };

	userBoard userBoard{ size, bombs };

	bool gameOver = false;

	while (!gameOver)
	{
		system("cls");
		userBoard.printBoard();

		int x, y;
		x = y = -1;

		while (x < 0 or x >= size or y < 0 or y >= size)
		{
			cout << "Enter valid coordinate (Ex: 7 5)\n";
			scanf_s("%d %d", &x, &y);
		}


		int val = actualBoard.getVal(x, y);

		if (val == BOMB)
		{
			system("cls");
			actualBoard.printBoard();
			cout << "Sorry, you have lost! D:\n";
			cout << "\nPress any key to continue.\n";
			gameOver = true;
			_getch();
		}
		else
		{
			recSearch(x, y, size, actualBoard, userBoard);

			if (userBoard.hasWon())
			{
				system("cls");
				actualBoard.printBoard();
				cout << "Congratulations!\n" << "You have won!\n";
				cout << "\nPress any key to continue!\n";
				gameOver = true;
				_getch();
			}
			
		}

	}

	actualBoard.free();
	userBoard.free();

	return NOQUIT;

}


int main(void)
{
	cout << "Welcome to Minesweeper!\n\n";

	while (minesweeper() != QUIT) {};

	_CrtDumpMemoryLeaks();

}