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
class Token;
class Tile;
class Game;
class Board;


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
	blue,
	invalid
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
	else if (t == "farm" || t == "farmer")
	{
		return Type::blue;
	}
	else if (t == "temple" || t == "priest")
	{
		return Type::red;
	}
	else if (t == "market" || t == "merchant")
	{
		return Type::green;
	}
	else return Type::invalid;
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

	bool hasTemple() { if (type == CellType::TEMPLE) return true; else return false; };

	CellType getType() { return type; };

	Position getPosition() { return pos; };

	void setToken(Token* t);

private:
	Position pos;
	CellType type;
	Token* token = nullptr;
};


class Board
{
public:
	//~Board() { delete[] cells; };
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

	bool placeTile(Tile* tile, std::vector<std::string> args);
};

bool Board::placeTile(Tile* tile, std::vector<std::string> args)
{
	//Check if position is correct within the board dimensions.
	Position p;
	p.x = std::stoi(args[2]);
	p.y = std::stoi(args[3]);
	if (p.x < 0 || p.x > BOARD_WIDTH || p.y < 0 || p.y > BOARD_LENGTH)
	{
		std::cout << "exception: invalid board space position \n";
		return false;
	}

	//Check if the tile is a farm. Farms can only be places on river tiles.
	Cell* cell = getCell(p);
	if ((cell->getType() == CellType::RIVER && args[1] != "farm") || (cell->getType() != CellType::RIVER && args[1] == "farm"))
	{
		std::cout << "exception: only farms can be placed in a river tile \n";
		cell = nullptr;
		return false;
	}

	//Check if that cell is alredy ocupied.
	if (!cell->isEmpty() && !cell->hasTemple())
	{
		std::cout << "exception: board space is already occupied \n";
		cell = nullptr;
		return false;
	}
	//tile->setCell(cell);
	cell->setToken((Token*)tile);
	return true;
}

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
	void setCell(Cell*c) { if (c != nullptr) cell = c; };

protected:
	Cell* cell;
	const Player* owner;
	Type type;
	Board* board;
};

void Cell::setToken(Token* t)
{
	token = t;
};

class Tile :  public Token
{
	using Token::Token;

public:

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

	bool placeTile(const std::vector<std::string>& args)
	{
		if (!existsType(args[1]))
		{
			std::cout << "exception: could not parse tyle type \n";
			return false;
		};

		for (int i = 0; i < tiles.size(); ++i)
		{
			bool found = false;
			if (tiles[i]->getType() == checkType(args[1]))
			{
				if (board->placeTile(tiles[i], args))
				{
					tiles.erase(tiles.begin() + i);
					found = true;
					return true;;
				}
				else
					return false;
			}
		};
		std::cout << "exception: could not find tile of specified tile \n";	
		return false;
	};

	void resetActions() { actions = 2; };
	void useAction() { --actions; };
	bool hasActionsLeft() {
		if (actions != 0) {
			return true;
		}
		else
		{
			std::cout << "exception: too many turn actions \n";
			return false;
		}
	};

private:
	Board* board;
	int actions = 2;
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


void Board::init()
{
	const Player* p = nullptr;
	int count = 0;
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			Cell* c = new Cell(i, j, init_board[count]);
			cells[j][i] = c;
			if (c->hasTemple())
			{
				c->setToken(new Tile(this, p, c, Type::red));
			}
			++count;
		}
	}
}

class Game
{
	enum Commands {
		invalid,
		tile,
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
		board->init();;
	};
	~Game()
	{
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

	std::vector<std::string> readInput() 
	{
		std::vector<std::string> input;
		std::string line;
		while (std::getline(std::cin, line))
		{
			if (line == "")
				return input;
			input.emplace_back(line);
		}
		return input;
	};

	void startGame(std::vector<std::string> input_lines)
	{
		board = new Board();
		board->init();

		for(int i = 0; i < input_lines.size(); ++i)
		{

			if (input_lines[i] == "----")
			{
				players[current_player]->resetActions();
				nextPlayer();
				continue;
			}


			size_t pos = 0;
			std::string delimiter = " ";
			std::vector<std::string> arguments;
			while ((pos = input_lines[i].find(delimiter)) < 20)
			{
				arguments.emplace_back(input_lines[i].substr(0, pos));
				input_lines[i].erase(0, pos + delimiter.length());
			}
			arguments.emplace_back(input_lines[i]);


			//call dictionary command, args;
			switch (resolveCommand(arguments[0]))
			{
			case tile:
				if (arguments.size() != 4)
				{
					std::cout << "invalid command \n";
				};
				if (players[current_player]->hasActionsLeft())
				{
					if(players[current_player]->placeTile(arguments))
						players[current_player]->useAction();
				}
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
				std::cout << "invalid command \n";
			}
		}
	};

	Commands resolveCommand(std::string input)
	{
		static const std::map<std::string, Commands> optionCommands
		{
			{ "tile", tile },
			{ "refresh", refresh },
			{ "treasure", treasure },
			{ "catastrophe", catastrophe },
			{ "revolt", revolt },
			{ "war", war },
			{ "monument", monument },

		};

		auto itr = optionCommands.find(input);
		if (itr != optionCommands.end()) {
			return itr->second;
		}
		return invalid;
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

	//Read all the Players initialization, untill "----"
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

	//we read and ignore the next "---" line
	std::getline(std::cin, command);

	std::vector<std::string> input_reading;
	input_reading = g.readInput();
	g.startGame(input_reading);

	return 0;
}
