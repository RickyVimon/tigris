#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#define BOARD_WIDTH 16
#define BOARD_LENGTH 11

//Forward declarations
class Player;
class Board;
class Cell;

struct Position
{
	int x;
	int y;
};

enum class Type
{
	black,
	red,
	green,
	blue
};

enum class CellType
{
	LAND,
	RIVER,
	TEMPLE,
	CATASTROPHE
};

Type checkType(std::string t)
{
	if (t == "settlement" || t == "king")
	{
		return Type::black;
	}
	if (t == "farm" || t == "farmer")
	{
		return Type::blue;
	}
	if (t == "temple" || t == "priest")
	{
		return Type::red;
	}
	if (t == "market" || t == "merchant")
	{
		return Type::green;
	}
};

bool existsType(std::string t)
{
	if (t == "settlement" || t == "king" || t == "farm" || t == "farmer" || t == "temple" || t == "priest" || t == "market" || t == "merchant")
	{
		return true;
	}
	else
		return false;
	
};


class Token 
{
	
public:
	Token(Board* b, const Player* &p, Cell* c, Type t) : board(b), owner(p), cell(c), type(t)	{}

	Token(Board* b, const Player* &p, Cell* c, std::string t)
	{
		board = b;
		owner = p;
		cell = c;
		type = checkType(t);
	};

	Type getType() { return type; };

	Position getPosition();
	virtual bool placeToken() { return false; };

protected:
	Cell* cell;
	const Player* owner;
	Type type;
	Board* board;
};

class Tile :  public Token
{
	using Token::Token;

public:
	 bool placeToken(std::vector<std::string> args, const Player* player)
	{
		//Check if position is correct within the board dimensions.
		Position p;
		p.x = stoi(args[1]);
		p.y = stoi(args[2]);
		if (p.x < 0 || p.x > BOARD_WIDTH || p.y < 0 || p.y > BOARD_LENGTH)
		{
			std::cout << "exception: invalid board space position";
			return false;
		}

		//Check if the tile is a farm. Farms can only be places on river tiles.
		cell = board->getCell(p);
		if (cell->getType() == CellType::RIVER && args[0] != "farm")
		{
			std::cout << "exception: only farms can be placed in a river tile";
			cell = nullptr;
			return false;
		}

		//Check if that cell is alredy ocupied.
		if (!cell->isEmpty())
		{
			std::cout << "exception: board space is already occupied";
			cell = nullptr;
			return false;
		}
		cell->setToken(this);
		return true;
	};
};

class Leader : public Token
{
public:

	using Token::Token;
	
	enum class Type
	{
		king,
		priest,
		merchant,
		farmer
	}; 

};


class Player
{
public:
	const Player* myself = this;
	Player(Board* b, std::string starting_tiles)
	{
		board = b;
		leaders.emplace_back(new Leader(board, myself, nullptr, Type::black));
		leaders.emplace_back(new Leader(board, myself, nullptr, Type::red));
		leaders.emplace_back(new Leader(board, myself, nullptr, Type::green));
		leaders.emplace_back(new Leader(board, myself, nullptr, Type::blue));

		setTiles(starting_tiles);
	};

	void setTiles(std::string token_line) 
	{
		size_t pos = 0;
		std::string tile_type;
		while ((pos = token_line.find(" ")) != std::string::npos) {
			tile_type = token_line.substr(0, pos);
			tiles.emplace_back(new Tile(board, myself, nullptr, tile_type));
			token_line.erase(0, pos + 1);
		}
		tiles.emplace_back(new Tile(board, myself, nullptr, tile_type));

	};

	std::vector<Tile*> tiles;
	std::vector<Leader*> leaders;

	void placeTyle(const std::vector<std::string>& args)
	{
		if (!existsType(args[0]))
		{
			std::cout << "exception: could not parse tyle type";
			return;
		};

		for (int i; i < tiles.size(); ++i)
		{
			if (tiles[i]->getType() == checkType(args[0]))
			{
				if(tiles[i]->placeToken(args, myself))
					tiles.erase(tiles.begin() + i);
			}
			else
				std::cout << "exception: could not find tile of specified tile";
		};
	
	};

private:
	Board* board;
};


class Cell
{
public:


	Cell(const int& x, const int& y, const int& t)
	{
		pos.x = x;
		pos.y = y;
		type = static_cast<CellType>(t);
	}

	bool isEmpty() { if (token == nullptr) return true; else return false; };

	CellType getType() { return type; };

	Position getPosition() { return pos; };

	void setToken(Token* t) { token = t;};

private:
	Position pos;
	CellType type;
	Token* token = nullptr;;
};

Position Token::getPosition() 
{
	Position p;
	p.x = -1;
	p.y = -1;
	if (cell != nullptr)
	{
		p = cell->getPosition();
	}
	return p;
};

static class Board
{
public:
	~Board() { delete[] cells; };
	std::vector<int> init_board
	{ 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 2, 0,
		0, 2, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1,
		1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
		0, 0, 0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
		0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2,
		0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 2, 0, 1, 0, 0, 0
	};
	Cell *cells[BOARD_WIDTH][BOARD_LENGTH];
	void init();
	
	Cell* getCell(const Position& pos) 
	{
		return cells[pos.x][pos.y];
	};
};

void Board::init()
{
	int count = 0;
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			Cell* c = new Cell(i, j, init_board[count]);
			cells[j][i] = c;
			++count;
		}
	}
}

class Game
{
	enum Commands {
		tyle,
		leader,
		refresh,
		treasure,
		catastrophe,
		revolt,
		war,
		monument
	};

public:

	Game() 
	{
		board = new Board();
		board->init();
	};
	~Game()
	{
		delete board;
	}

	void addNewPlayer(std::string initial_tiles) 
	{
		Player* p = new Player(board, initial_tiles);
		players.emplace_back(p);
	}

	void nextPlayer()
	{
		if (current_player + 1 >= players.size())
		{
			current_player = 0;
		}
		else
			++current_player;
	};

	void readCommand() 
	{
		std::string command;
		std::getline(std::cin, command);

		if (command == "----")
		{
			nextPlayer();
		}


		size_t pos = 0;
		std::string delimiter = " ";
		std::vector<std::string> arguments;
		while (pos = command.find(delimiter) != std::string::npos) {
			arguments.emplace_back(command.substr(0, pos));
			command.erase(0, pos + delimiter.length());
		}
		arguments.emplace_back(command);

		//call dictionary command, args;
		switch (resolveCommand(arguments[0]))
		{
			arguments.pop_back();
			case tyle:
				players[current_player]->placeTyle(arguments);
				break;

			case refresh:
				break;

			case treasure:
				break;

			case catastrophe:
				break;

			case revolt:
				break;

			case war:
				break;

			default:
				std::cout << "invalid command";
		}
	};

	Commands resolveCommand(std::string input) {
		static const std::map<std::string, Commands> optionCommands{
			{ "tyle", tyle },
			{ "refresh", refresh },
			{ "treasure", treasure },
			{ "catastrophe", catastrophe },
			{ "revolt", revolt },
			{ "war", war },
			{ "monument", monument },
		};
	}


private:

	//index to track the current player
	int current_player;
	std::vector<Player*> players;
	Board* board;
};

int main() 
{
	Game g;
	std::string command;
	/* Enter your code here. Read input from STDIN. Print output to STDOUT */

	char c_delimeter('-');
	std::getline(std::cin, command, c_delimeter);

	size_t pos = 0;
	std::string delimiter = "\n";
	std::string players_tiles;
	while ((pos = command.find(delimiter)) != std::string::npos) {
		players_tiles = command.substr(0, pos);
		g.addNewPlayer(players_tiles);
		command.erase(0, pos + delimiter.length());
	}

	return 0;
}