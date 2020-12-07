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
class Board;
class Player;
class Token;
class Tile;
class Leader;
class Cell;
class Kingdom;
class Region;



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
	Token* getToken() { return token; };

private:
	Position pos;
	CellType type;
	Token* token = nullptr;
};

void Cell::setToken(Token* t)
{
	token = t;
};

class Kingdom
{
public:
	Kingdom(const std::vector<Tile*> &k_t, Leader* lead) : kingdom_tiles(k_t)
	{
		leaders.emplace_back(lead);
	}
public:
	std::vector<Tile*> kingdom_tiles;
	std::vector<Leader*> leaders;
};

class Region
{
public:
	Region(const std::vector<Tile*> &r_t) : region_tiles(r_t) {}
	Region(Tile* tile) 
	{
		region_tiles.emplace_back(tile);
	}

public:
	std::vector<Tile*> region_tiles;
};



class Board
{
public:
	//~Board() { delete[] cells; };
	void init();

	Cell* getCell(const Position& pos)
	{
		return cells[pos.x][pos.y];
	};

	bool hasCellTemple(const Position &pos);
	Region* getRegion(const Position &pos);
	bool hasCellRegion(const Position & pos);
	bool isCellEmpty(const Position &pos);
	bool placeToken(Token* tile, std::vector<std::string> args, bool isLeader);
	std::vector<Region*> getAdjacentRegions(const Position &p);
	void checkKingdoms(std::vector<Token*> adjacent_tokens, Token* token, bool isLeader);

	void newRegion(const std::vector<Tile*> &tiles);
	void newRegion(Tile* tile);

	std::vector<Token*> getAdjacentTokens(Token* token, const Position &pos);
	bool hasAdjacentTemple(const Position &pos);
	
public:
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


	std::vector<Region*> regions;
	std::vector<Kingdom*> kingdoms;

};


class Token
{

public:
	Token(Board* b, Player* p, Cell* c, Type t) : board(b), owner(p), cell(c), type(t) {}

	Token(Board* b, Player* p, Cell* c, std::string t)
	{
		board = b;
		owner = p;
		cell = c;
		type = checkType(t);
	};

	Type getType() { return type; };

	Position getPosition();
	void setCell(Cell*c);
	void setRegion(Region* r) { if (r != nullptr) region = r; };
	void setKingdom(Kingdom* k) { if (k != nullptr) kingdom = k; };
	Kingdom* getKingdom() { return kingdom; };
	Region* getRegion() { return region; };

	bool isRegion() { if (region == nullptr) return false; else return true; };
	bool isKingdom() { if (kingdom == nullptr) return false; else return true; };

public:
	Player* owner;

protected:
	Cell* cell;
	Type type;
	Board* board;
	Region* region = nullptr;
	Kingdom* kingdom = nullptr;
};


class Tile : public Token
{
	using Token::Token;

public:

	void checkAdjacentKingdomsOrRegions();

};



class Leader : public Token
{
public:
	using Token::Token;
};



class Player
{
public:
	
	Player(Board* b, std::string starting_tiles)
	{
		board = b;
		leaders.emplace_back(new Leader(board, this, nullptr, Type::black));
		leaders.emplace_back(new Leader(board, this, nullptr, Type::red));
		leaders.emplace_back(new Leader(board, this, nullptr, Type::green));
		leaders.emplace_back(new Leader(board, this, nullptr, Type::blue));

		setTiles(starting_tiles);

		victory_points.emplace_back(0);
		victory_points.emplace_back(0);
		victory_points.emplace_back(0);
		victory_points.emplace_back(0);
	};

	void setTiles(std::string token_line);

	bool placeTile(const std::vector<std::string>& args);

	bool placeLeader(const std::vector<std::string>& args);

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

	void refreshTiles(std::vector<std::string> args);

	std::vector<int> getVictoryPoints() { return victory_points; };

	std::vector<Tile*> tiles;
	std::vector<Leader*> leaders;

	int getTreasures() { return treasures; };

	void winPoint(const int &index);

private:
	Board* board;
	int actions = 2;
	std::vector<int> victory_points;
	int treasures = 0;
};


//----------------------------------------------------------------END OF THE HEADER----------------------------------------------------

void Player::winPoint(const int &index)
{
	if(index >= 0 && index < victory_points.size())
	++victory_points[index];
}

void Token::setCell(Cell*c)
{
	cell = c;
	
}


/*
void Leader::checkAdjacentKingdomsOrRegions()
{
	std::vector<Token*> adjacent_tokens = board->getAdjacentTokens(this, cell->getPosition());

	for(unsigned int i = 0; i < adjacent_tokens.size();++i)
	{
		if(adjacent_tokens[i]->isRegion())
	}
	
}
*/

void Tile::checkAdjacentKingdomsOrRegions()
{
	std::vector<Token*> adjacent_tokens = board->getAdjacentTokens(this, cell->getPosition());

	switch (adjacent_tokens.size())
	{
	case 0:
		//New Region
		board->newRegion(this);
		break;

	case 1:
		//Join Region or Kingdom
		if (adjacent_tokens[1]->isKingdom())
		{
			kingdom = adjacent_tokens[1]->getKingdom();
			kingdom->kingdom_tiles.emplace_back(this);

			//check if this action is rewarded with points
			for (unsigned int i = 0; i < kingdom->leaders.size(); ++i)
			{
				if (kingdom->leaders[i]->getType() == Type::black)
				{
					//Add one victory point for the owner of the king
					kingdom->leaders[i]->owner->winPoint((const int)Type::black);
				}
				else if (kingdom->leaders[i]->getType() == type)
				{
					owner->winPoint((const int)type);
				}
			}
		}
		else
		{
			region = adjacent_tokens[1]->getRegion();
			region->region_tiles.emplace_back(this);
		}
		break;

	case 2:
		if (adjacent_tokens[0]->getRegion() != adjacent_tokens[1]->getRegion())
		{
			//Disvolve into a bigger region
		}
		else //Join the kingdom
			adjacent_tokens[0]->getRegion()->region_tiles.emplace_back(this);
		break;

	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
	//if(adjacent_tokens.size()  1)
	for (unsigned int i = 0; i < adjacent_tokens.size(); ++i)
	{
		if (adjacent_tokens[i]->isRegion())
		{

		}
	}

}

void Board::newRegion(const std::vector<Tile*> &tiles)
{
	Region* region = new Region(tiles);
	regions.emplace_back(region);
}

void Board::newRegion(Tile* tile)
{
	Region* region = new Region(tile);
	regions.emplace_back(region);
}

std::vector<Token*> Board::getAdjacentTokens(Token* token, const Position &pos)
{
	std::vector<Token*> adjacent_tokens;
	Cell* c = getCell(pos);
	if (pos.x + 1 <= BOARD_WIDTH)
	{
		if (!isCellEmpty(pos))
			adjacent_tokens.emplace_back(c->getToken());

	}
	if (pos.x - 1 >= 0)
	{
		if (!isCellEmpty(pos))
			adjacent_tokens.emplace_back(c->getToken());

	}
	if (pos.y + 1 <= BOARD_LENGTH)
	{
		if (!isCellEmpty(pos))
			adjacent_tokens.emplace_back(c->getToken());
	}
	if (pos.y - 1 >= 0)
	{
		if (!isCellEmpty(pos))
			adjacent_tokens.emplace_back(c->getToken());
	}
	return adjacent_tokens;
}

bool Board::hasAdjacentTemple(const Position &pos)
{
	if (pos.x + 1 <= BOARD_WIDTH)
	{
		if (hasCellTemple(pos))
			return true;
	}
	if (pos.x - 1 >= 0)
	{
		if (hasCellTemple(pos))
			return true;
	}
	if (pos.y + 1 <= BOARD_LENGTH)
	{
		if (hasCellTemple(pos))
			return true;
	}
	if (pos.y - 1 >= 0)
	{
		if (hasCellTemple(pos))
			return true;
	}
	return false;

}

bool Board::isCellEmpty(const Position &pos)
{
	Cell* c = getCell(pos);
	return c->isEmpty();
}

bool Board::hasCellTemple(const Position &pos)
{
	Cell* c = getCell(pos);
	return c->hasTemple();
}

Region* Board::getRegion(const Position &pos)
{
	Region* r = nullptr;
	Cell* c = getCell(pos);
	if (!c->isEmpty())
	{
		Token* t = c->getToken();
		if (t->isRegion())
		{
			r = t->getRegion();
			return r;
		}
	}

		return r;
}

bool Board::hasCellRegion(const Position &pos)
{
	Cell* c = getCell(pos);
	Token* t = c->getToken();
	if (t->isRegion())
		return true;
	else
		return false;
}

void Board::checkKingdoms(std::vector<Token*> adjacent_tokens, Token* token, bool isLeader = false)
{

	std::vector<Kingdom*> kingdoms;
	for (unsigned int i = 0; i < adjacent_tokens.size(); ++i)
	{
		if (adjacent_tokens[i]->isKingdom())
		{
			Kingdom* k = adjacent_tokens[i]->getKingdom();
			kingdoms.emplace_back(k);
		}
	}

	switch (kingdoms.size())
	{
	case 0:
		if (isLeader)
		{
			//checkAdjacentRegions and join the region
			//create kingdom.
		}
		break;
	case 1:
		//add the tile to the kingdom;
		token->setKingdom(kingdoms[0]);
		if (isLeader)
		{
			kingdoms[0]->leaders.emplace_back((Leader*)token);
		}
		else
		{
			kingdoms[0]->kingdom_tiles.emplace_back((Tile*)token);
		}
		break;

	case 2:
		//war
		//conflict
		break;
	default:
		std::cout << "exception: cannot place a tile which could unite 3 or more kingdoms. \n";
		break;
	}

}


bool Board::placeToken(Token* token, std::vector<std::string> args, bool isLeader = false)
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
	if (!cell->isEmpty())
	{
		std::cout << "exception: board space is already occupied \n";
		cell = nullptr;
		return false;
	}

	//Leaders can only be placed adjacent to a temple
	if (isLeader)
	{
		if (!hasAdjacentTemple(p))
		{
			std::cout << "exception: cannot place a leader in a space without adjacent temple \n";
		}
		else //Leaders cannot be placed between two kingdoms
		{
			//checkkingdoms

			std::vector<Region*> regions = getAdjacentRegions(p);

			switch (regions.size())
			{
				//TODO
			}
		}
	}

	cell->setToken(token);
	token->setCell(cell);
	return true;
}

std::vector<Region*> Board::getAdjacentRegions(const Position &pos)
{
	Region* new_region;
	std::vector<Region*> regions;

	if (pos.x + 1 <= BOARD_WIDTH)
	{
		new_region = getRegion(pos);
		if (new_region != nullptr)
			regions.emplace_back(new_region);
	}
	if (pos.x - 1 >= 0)
	{
		new_region = getRegion(pos);
		if (new_region != nullptr)
			regions.emplace_back(new_region);
	}
	if (pos.y + 1 <= BOARD_LENGTH)
	{
		new_region = getRegion(pos);
		if (new_region != nullptr)
			regions.emplace_back(new_region);
	}
	if (pos.y - 1 >= 0)
	{
		new_region = getRegion(pos);
		if (new_region != nullptr)
			regions.emplace_back(new_region);
	}
	return regions;
}

void Player::setTiles(std::string token_line)
{
	size_t pos = 0;
	std::string tile_type;
	while ((pos = token_line.find(" ")) != std::string::npos) {
		tile_type = token_line.substr(0, pos);
		tiles.emplace_back(new Tile(board, this, nullptr, tile_type));
		token_line.erase(0, pos + 1);
	}
	tiles.emplace_back(new Tile(board, this, nullptr, tile_type));

};

bool Player::placeTile(const std::vector<std::string>& args)
{
	if (!existsType(args[1]))
	{
		std::cout << "exception: could not parse tyle type \n";
		return false;
	};

	for (unsigned int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i]->getType() == checkType(args[1]))
		{
			if (board->placeToken(tiles[i], args))
			{
				tiles.erase(tiles.begin() + i);
				return true;;
			}
			else
				return false;
		}
	};
	std::cout << "exception: could not find tile of specified tile \n";
	return false;
};

bool Player::placeLeader(const std::vector<std::string>& args)
{
	if (!existsType(args[1]))
	{
		std::cout << "exception: could not parse tyle type \n";
		return false;
	};

	//check if the tile is on the board or at player's hand.
	for (unsigned int i = 0; i < leaders.size(); ++i)
	{
		if (leaders[i]->getType() == checkType(args[1]))
		{
			Position pos = leaders[i]->getPosition();
			if (pos.x != -1)
			{
				//leader is on the hand
				//placeLeader
				if (board->placeToken(leaders[i], args, true))
				{
					return true;;
				}
				else
					return false;
			}
			else
			{
				//Leader is on the board
				//move Leader
			}
		}

	};
	std::cout << "exception: could not find tile of specified tile \n";
	return false;
};

void Player::refreshTiles(std::vector<std::string> args)
{
	for (unsigned int i = 2; i < args.size(); ++i)
	{
		if (checkType(args[i]) != Type::invalid)
			tiles.emplace_back(new Tile(board, this, nullptr, checkType(args[i])));
		else
			std::cout << "exception: invalid number or player tiles \n";
	}

}


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
	Player* p = nullptr;
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

	void nextPlayer();

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

		for(unsigned int i = 0; i < input_lines.size(); ++i)
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
				}
				else if (players[current_player]->hasActionsLeft())
				{
					if(players[current_player]->placeTile(arguments))
						players[current_player]->useAction();
				}
				break;

			case leader:
				if (arguments.size() != 4)
				{
					std::cout << "invalid command \n";
				}
				else if (players[current_player]->hasActionsLeft())
				{
					if (players[current_player]->placeLeader(arguments))
						players[current_player]->useAction();
				}
				break;

			case refresh:
				if (arguments.size() != 4)
				{
					std::cout << "invalid command \n";
				};

				if ((current_player == std::stoi(arguments[1])) && (6 - (players[current_player]->tiles.size()) < (arguments.size()-2)))
				{
					players[current_player]->refreshTiles(arguments);
				}
				else
					std::cout << "exception: invalid number or player tiles \n";
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

void Game::nextPlayer()
{

	if ((unsigned int)(current_player + 1) >= players.size())
	{
		current_player = 0;
		for (unsigned int i = 0; i < players.size(); ++i)
		{
			std::vector<int> points = players[i]->getVictoryPoints();
			std::cout << "victory " << players[i]->getTreasures() << " (" << points[0] << " " << points[1] << " " << points[2] << " " << points[3] << ")\n";
		}
	}
	else
		++current_player;
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

	//end Game
	std::cout << "----";

	return 0;
}
