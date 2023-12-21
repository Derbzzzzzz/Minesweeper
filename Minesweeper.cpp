#include <iostream>
#include <vector>

#define BEGINNER 1
#define INTERMEDIATE 2
#define ADVANCED 3

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

	char getVal(int x, int y) const { return array[index(x, y)]; };

	void setVal(int x, int y, char val) { array[index(x, y)] = val; };

	void printBoard()
	{
		cout << "\n";

		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				cout << getVal(i, j) << " ";
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
	void setBoard()
	{
		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				setVal(i, j, '-');
			}
		}
	}
public:
	userBoard(int size, int bombs) : Board(size)
	{
		setBoard();
	}
};


int getDifficultyLevel()
{
	cout << "Choose Difficulty Level:\n";
	cout << "1. Beginner\n";
	cout << "2. Intermediate\n";
	cout << "3. Advanced\n";

	while (true)
	{
		int difficulty;
		scanf_s("%d", &difficulty);

		if (difficulty == BEGINNER || difficulty == INTERMEDIATE || difficulty == ADVANCED)
		{
			return difficulty;
		}
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

void minesweeper()
{
	int difficulty = getDifficultyLevel();

	int size, bombs;

	getGameValues(difficulty, &size, &bombs);

	actualBoard actualBoard{ size, bombs };

	userBoard userBoard{ size, bombs };

	bool gameOver = false;

	actualBoard.printBoard();

	while (!gameOver)
	{
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
			gameOver = true;
		}
		else
		{
			// must set userBoard with number of mines
			userBoard.setVal(x, y, val);
			// recursive depth first search for 0s

		}

	}

}

int main(void)
{
	cout << "Welcome to Minesweeper!";


	minesweeper();

}