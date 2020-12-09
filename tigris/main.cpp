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
class Cell;
class Area;



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
	void setTypeCatastrophe() { type = CellType::CATASTROPHE; }

private:
	Position pos;
	CellType type;
	Token* token = nullptr;
};


class Area
{
public:
	Area(const std::vector<Token*> &a_t) : area_tokens(a_t) {}
	Area(Token* tok)
	{
		area_tokens.push_back(tok);
	}
	bool isKingdom();
	Token* getLeader();

public:
	std::vector<Token*> area_tokens;
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
	Area* getArea(const Position &pos);
	bool hasCellArea(const Position & pos);
	bool isCellEmpty(const Position &pos);
	bool placeToken(Token* tile, std::vector<std::string> args);

	void dissolveArea(const Position &pos);

	void findIsland(Token * current_token, std::vector<Token*> island_tokens, std::vector<Token*> pool);

	bool checkRevolt(Token* leader_token);

	bool checkWar(Token* tile_token);

	bool isPositionCorrect(const Position & pos);

	void newArea(std::vector<Token*> tokens);
	void newArea(Token* token);

	std::vector<Area*> getAdjacentAreas(const Position &p);
	std::vector<Token*> getAdjacentTokens(const Position &pos);
	bool hasAdjacentTemple(const Position &pos);
	void solveCatastrophe(const Position &pos);

	void fuzeAreas(std::vector<Area*> areas_to_fuze);

	
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


	Cell* cells[BOARD_WIDTH][BOARD_LENGTH];
	std::vector<Area*> areas;

};


class Token
{

public:
	Token(Board* b, Player* p, Cell* c, Type t, bool il = false) : board(b), owner(p), cell(c), type(t), isleader(il){}

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
	std::vector<Token*> getCollidingTokens(std::vector<Token*> tile_vector);
	bool collidesWith(Token * token);
	void setArea(Area* a) { if (a != nullptr) area = a; };
	Area* getArea() { return area; };
	bool isLeader() { return isleader; }

public:
	Player* owner;

protected:
	Cell* cell;
	Type type;
	Board* board;
	Area* area = nullptr;
	bool isleader;
};



class Player
{
public:
	
	Player(Board* b, std::string starting_tiles)
	{
		board = b;
		tokens.emplace_back(new Token(board, this, nullptr, Type::black, true));
		tokens.emplace_back(new Token(board, this, nullptr, Type::red, true));
		tokens.emplace_back(new Token(board, this, nullptr, Type::green, true));
		tokens.emplace_back(new Token(board, this, nullptr, Type::blue, true));

		setTokens(starting_tiles);

		victory_points.emplace_back(0);
		victory_points.emplace_back(0);
		victory_points.emplace_back(0);
		victory_points.emplace_back(0);
		resetActions();
	};

	void setTokens(std::string token_line);
	bool placeTile(const std::vector<std::string>& args);
	bool placeLeader(const std::vector<std::string>& args);
	void resetActions() { actions = 2; };
	void useAction() { --actions; };
	bool hasActionsLeft();
	void refreshTiles(std::vector<std::string> args);
	int getTreasures() { return treasures; };
	void winPoint(const int &index);
	void printPoints();

	std::vector<int> getVictoryPoints() { return victory_points; };
	std::vector<Token*> tokens;

private:
	Board* board;
	int actions = 2;
	std::vector<int> victory_points;
	int treasures = 0;
};


//----------------------------------------------------------------END OF THE HEADER----------------------------------------------------
void Board::dissolveArea(const Position &pos)
{
	std::vector<Token*> colliding_tokens = getAdjacentTokens(pos);

	for (unsigned int i = 0; i < colliding_tokens.size(); ++i)
	{
		std::vector<Token*> island_tokens, pool;
		pool = getAdjacentTokens(colliding_tokens[i]->getPosition());
		while (pool.size() != 0)
		{
			findIsland(pool[0], island_tokens, pool);
		}
		if (island_tokens.size() > 0)
		{
			newArea(island_tokens);
		}
		//update area to all tokens
		for (unsigned int k = 0; k < island_tokens.size(); ++k)
		{
			island_tokens[k]->setArea(areas[areas.size() - 1]);
		}
		
	}
}

void Board::findIsland(Token* current_token, std::vector<Token*> island_tokens, std::vector<Token*> pool)
{
	//send current_token to visited
	island_tokens.emplace_back(current_token);
	//remove current token from the pool
	pool.erase(std::remove(pool.begin(), pool.end(), current_token), pool.end());
	
	std::vector<Token*> colliding_tokens = getAdjacentTokens(current_token->getPosition());

	for (unsigned int i = 0; i < colliding_tokens.size(); ++i)
	{
		//Check if that node was alredy visited
		if (std::find(island_tokens.begin(), island_tokens.end(), colliding_tokens[i]) != island_tokens.end())
		{
			continue;
		}
		//check if that token is in the pool pending to be visited
		else if (std::find(pool.begin(), pool.end(), colliding_tokens[i]) != pool.end())
		{
			continue;
		}
		else
			//Otherwise just add it to the pool
			pool.emplace_back(colliding_tokens[i]);
	}
	
}



void Cell::setToken(Token* t)
{
	token = t;
};

bool Area::isKingdom()
{
	for (unsigned int i = 0; i < area_tokens.size(); ++i)
	{
		if (area_tokens[i]->isLeader())
		{
			return true;
		}
	}
	return false;
}

Token * Area::getLeader()
{
	Token* leader = nullptr;
	if(!isKingdom())
		return leader;
	else
	{
		for (unsigned int i = 0; i < area_tokens.size(); ++i)
		{
			if (area_tokens[i]->isLeader())
				leader = area_tokens[i];
		}
	}
	return leader;
}

bool Player::hasActionsLeft() 
{
	if (actions > 0) {
		return true;
	}
	else
	{
		std::cout << "exception: too many turn actions \n";
		return false;
	}
};


void Player::winPoint(const int &index)
{
	if((unsigned int)index > 0 && index <= victory_points.size())
	++victory_points[index - 1];
}

void Player::printPoints()
{
	std::cout << "victory " << getTreasures() << " (" << victory_points[0] << " " << victory_points[1] << " " << victory_points[2] << " " << victory_points[3] << ")\n";
}

void Token::setCell(Cell*c)
{
	cell = c;
	
}

bool Board::checkRevolt(Token* leader_token)
{
	std::vector<Area*> adjacent_areas = getAdjacentAreas(leader_token->getPosition());
	int coliding_kingdoms = 0;
	int last_index_kingdom;
	for (unsigned int i = 0; i < adjacent_areas.size(); ++i)
	{
		adjacent_areas[i]->isKingdom();
		++coliding_kingdoms;
		last_index_kingdom = i;
	}
	if (coliding_kingdoms > 1)
	{
		std::cout << "exception: cannot place a leader that would unite two kingdoms";
		return false;
	}
	if (coliding_kingdoms == 1)
	{
		for (unsigned int i = 0; i < adjacent_areas[last_index_kingdom]->area_tokens.size(); ++i)
		{
			if (adjacent_areas[last_index_kingdom]->area_tokens[i]->getType() == leader_token->getType())
				return true;
		}
		//Join kingdom
		adjacent_areas[last_index_kingdom]->area_tokens.emplace_back(leader_token);
		leader_token->setArea(adjacent_areas[last_index_kingdom]);
	}
		//unite regions to the kingdom if any
		fuzeAreas(adjacent_areas);
		return false;
}

bool Board::checkWar(Token* incoming_tile)
{
	std::vector<Area*> adjacent_areas = getAdjacentAreas(incoming_tile->getPosition());
	if (adjacent_areas.size() == 0)
	{
		newArea(incoming_tile);
		return false;
	}

	std::vector<Area*> adj_kingdoms;
	for (unsigned int i = 0; i < adjacent_areas.size(); ++i)
	{
		if(adjacent_areas[i]->isKingdom())
			adj_kingdoms.emplace_back(adjacent_areas[i]);
	}
	
	if (adj_kingdoms.size() > 2)
	{
		std::cout << "exception: invalid board space position";
		return false;
	}
	else if (adj_kingdoms.size() == 2)
	{
		if (adj_kingdoms[0]->getLeader()->getType() == adj_kingdoms[1]->getLeader()->getType())
		{
			//war conflict
			return true;
		}
		else
		{
			fuzeAreas(adj_kingdoms);
			return false;
		}
	}
	else if (adj_kingdoms.size() == 1)
	{
		//if there is 1 or less adjacent kingdoms, join kingdom and fuze rest of areas.

		if (adj_kingdoms[0]->getLeader()->getType() == Type::black)
		{
			//Add one victory point for the owner of the king
			adj_kingdoms[0]->getLeader()->owner->winPoint((const int)Type::black);
		}
		else if (adj_kingdoms[0]->getLeader()->getType() == incoming_tile->getType())
		{
			//Add one victory point to the owner of the leader of the same type as the new tile placed
			adj_kingdoms[0]->getLeader()->owner->winPoint((const int)incoming_tile->getType());
		}
		adj_kingdoms[0]->area_tokens.emplace_back(incoming_tile);
		incoming_tile->setArea(adj_kingdoms[0]);
		fuzeAreas(adjacent_areas);
	}
	return false;
}



bool Board::isPositionCorrect(const Position &pos)
{
	if (pos.x >= 0 && pos.x <= BOARD_WIDTH && pos.y >= 0 && pos.y <= BOARD_LENGTH)
		return true;
	else
		return false;
}

void Board::newArea(std::vector<Token*> tokens)
{
	Area* area = new Area(tokens);
	for (unsigned int i = 0; i < tokens.size(); ++i)
	{
		tokens[i]->setArea(area);
	}
	areas.emplace_back(area);
}

void Board::newArea(Token* token)
{
	Area* area = new Area(token);
	areas.emplace_back(area);
	token->setArea(area);
}


std::vector<Token*> Board::getAdjacentTokens(const Position &pos)
{
	std::vector<Token*> adjacent_tokens;
	Cell* c;
	if (pos.x + 1 <= BOARD_WIDTH)
	{
		Position temp_pos = pos;
		temp_pos.x += 1;
		c = cells[temp_pos.x][temp_pos.y];
		if (!isCellEmpty(temp_pos))
			adjacent_tokens.emplace_back(c->getToken());

	}
	if (pos.x - 1 >= 0)
	{
		Position temp_pos = pos;
		temp_pos.x -= 1;
		c = cells[temp_pos.x][temp_pos.y];
		if (!isCellEmpty(temp_pos))
			adjacent_tokens.emplace_back(c->getToken());

	}
	if (pos.y + 1 <= BOARD_LENGTH)
	{
		Position temp_pos = pos;
		temp_pos.y += 1;
		c = cells[temp_pos.x][temp_pos.y];
		if (!isCellEmpty(temp_pos))
			adjacent_tokens.emplace_back(c->getToken());
	}
	if (pos.y - 1 >= 0)
	{
		Position temp_pos = pos;
		temp_pos.y -= 1;
		c = cells[temp_pos.x][temp_pos.y];
		if (!isCellEmpty(temp_pos))
			adjacent_tokens.emplace_back(c->getToken());
	}
	return adjacent_tokens;
}

void Board::solveCatastrophe(const Position &pos)
{
	if (isPositionCorrect(pos))
	{
		Cell* cell = getCell(pos);
		if (!cell->isEmpty())
		{
			Token* token_to_destroy = cell->getToken();
			delete token_to_destroy;
		}
		else
			cell->setTypeCatastrophe();

		dissolveArea(pos);
	}
}

void Board::fuzeAreas(std::vector<Area*> areas_to_fuze)
{
	std::vector<Token*> tokens_to_fuze;
	for (unsigned int i = 0; i < areas_to_fuze.size(); ++i)
	{
		for (unsigned k = 0; k < areas_to_fuze[i]->area_tokens.size(); ++k)
		{
			tokens_to_fuze.emplace_back(areas_to_fuze[i]->area_tokens[k]);
		}
	}
	newArea(tokens_to_fuze);
}

std::vector<Token*> Token::getCollidingTokens(std::vector<Token*> tile_vector)
{
	std::vector<Token*> colliding_tiles;
	for (unsigned int i = 0; i < tile_vector.size(); ++i)
	{
		if (this->collidesWith(tile_vector[i]))
			colliding_tiles.emplace_back(tile_vector[i]);
	}
	return colliding_tiles;
}

bool Token::collidesWith(Token* token)
{
	if (abs(this->getPosition().x - token->getPosition().x) == 1 && this->getPosition().y == token->getPosition().y)
		return true;
	if (abs(this->getPosition().y - token->getPosition().y) == 1 && this->getPosition().x == token->getPosition().x)
		return true;
	return false;
}


bool Board::hasAdjacentTemple(const Position &pos)
{
	if (isPositionCorrect(pos))
	{
		std::vector<Token*> adjacent_tokens = getAdjacentTokens(pos);
		for (unsigned int i = 0; i < adjacent_tokens.size(); ++i)
		{
			if (!adjacent_tokens[i]->isLeader() && adjacent_tokens[i]->getType() == checkType("temple"))
			{
				return true;
			}
		}
		return false;
	}
	else
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

Area* Board::getArea(const Position &pos)
{
	Area* a = nullptr;
	Cell* c = getCell(pos);
	if (!c->isEmpty())
	{
		Token* t = c->getToken();
		a = t->getArea();
	}
	return a;
}

bool Board::hasCellArea(const Position &pos)
{
	Cell* c = getCell(pos);
	Token* t = c->getToken();
	if (t->getArea() != nullptr)
		return true;
	else
		return false;
}


bool Board::placeToken(Token* token, std::vector<std::string> args)
{
	//Check if position is correct within the board dimensions.
	Position p;
	p.x = std::stoi(args[2]);
	p.y = std::stoi(args[3]);
	if (!isPositionCorrect(p))
	{
		std::cout << "exception: invalid board space position \n";
		return false;
	}

	Cell* cell = getCell(p);

	//Check if that cell is alredy ocupied.
	if (!cell->isEmpty())
	{
		std::cout << "exception: board space is already occupied \n";
		cell = nullptr;
		return false;

	}
	//Check if the tile is a farm. Farms can only be places on river tiles.
	if ((cell->getType() == CellType::RIVER && args[1] != "farm") || (cell->getType() != CellType::RIVER && args[1] == "farm"))
	{
		if (token->isLeader())
		{
			std::cout << "exception: cannot place a leader in a river\n";
		}
		else
			std::cout << "exception: only farms can be placed in a river tile\n";
		cell = nullptr;
		return false;
	}

	//Leaders can only be placed adjacent to a temple
	if (!hasAdjacentTemple(p) && token->isLeader())
	{
		std::cout << "exception: cannot place a leader in a space without adjacent temple \n";
		return false;
	}

	cell->setToken(token);
	token->setCell(cell);
	if (token->isLeader())
	{
		checkRevolt(token);
	}
	else
		checkWar(token);

	return true;
}

std::vector<Area*> Board::getAdjacentAreas(const Position &pos)
{
	Area* new_area;
	std::vector<Area*> areas;

	if (pos.x + 1 <= BOARD_WIDTH)
	{
		Position temp_pos = pos;
		temp_pos.x += 1;
		new_area = getArea(temp_pos);
		if (new_area != nullptr)
			areas.emplace_back(new_area);
	}
	if (pos.x - 1 >= 0)
	{
		Position temp_pos = pos;
		temp_pos.x -= 1;
		new_area = getArea(temp_pos);
		if (new_area != nullptr)
			areas.emplace_back(new_area);
	}
	if (pos.y + 1 <= BOARD_LENGTH)
	{
		Position temp_pos = pos;
		temp_pos.y += 1;
		new_area = getArea(temp_pos);
		if (new_area != nullptr)
			areas.emplace_back(new_area);
	}
	if (pos.y - 1 >= 0)
	{
		Position temp_pos = pos;
		temp_pos.y -= 1;
		new_area = getArea(temp_pos);
		if (new_area != nullptr)
			areas.emplace_back(new_area);
	}
	return areas;
}

void Player::setTokens(std::string token_line)
{
	size_t pos = 0;
	std::string tile_type;
	while ((pos = token_line.find(" ")) != std::string::npos) {
		tile_type = token_line.substr(0, pos);
		tokens.emplace_back(new Token(board, this, nullptr, tile_type));
		token_line.erase(0, pos + 1);
	}
	tokens.emplace_back(new Token(board, this, nullptr, tile_type));

};

bool Player::placeTile(const std::vector<std::string>& args)
{
	if (!existsType(args[1]))
	{
		std::cout << "exception: could not parse tyle type \n";
		return false;
	};

	for (unsigned int i = 0; i < tokens.size(); ++i)
	{
		if (!tokens[i]->isLeader() && tokens[i]->getType() == checkType(args[1]))
		{
			if (board->placeToken(tokens[i], args))
			{
				tokens.erase(tokens.begin() + i);
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
		std::cout << "exception: could not parse leader type \n";
		return false;
	};

	//check if the tile is on the board or at player's hand.
	for (unsigned int i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i]->getType() == checkType(args[1]))
		{
			Position pos = tokens[i]->getPosition();
			if (pos.x != -1)
			{
				board->cells[tokens[i]->getPosition().x][tokens[i]->getPosition().y]->setToken(nullptr);
				tokens[i]->setCell(nullptr);
		
				board->dissolveArea(tokens[i]->getPosition());
			}
			//leader on the hand
			if (board->placeToken(tokens[i], args))
			{
				tokens.erase(tokens.begin(), tokens.begin() + i);
				return true;
			}
			else
				return false;

		}

	};
	std::cout << "exception: could not find tile of specified tile \n";
	return false;
};

void Player::refreshTiles(std::vector<std::string> args)
{
	//count missing tiles
	int tiles = 0;
	for (unsigned k = 0; k < tokens.size(); ++k)
	{
		if (!tokens[k]->isLeader())
			++tiles;
	}
	if ((6 - tiles) != (args.size() - 2))
	{
		std::cout << "exception: invalid number or player tiles \n";
		return;
	}

	//get missig tiles;
	for (unsigned int i = 2; i < args.size(); ++i)
	{
		if (checkType(args[i]) != Type::invalid)
			tokens.emplace_back(new Token(board, this, nullptr, checkType(args[i])));
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
			Cell* c = new Cell(j, i, init_board[count]);
			cells[j][i] = c;
			if (c->hasTemple())
			{
				c->setToken(new Token(this, p, c, Type::red));
				newArea(c->getToken());
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
					players[current_player]->refreshTiles(arguments);
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
			{ "leader", leader },
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
	for (unsigned int k = 0; k < players.size(); ++k)
	{
		players[k]->printPoints();
	}
	std::cout << "----\n";

	if ((unsigned int)(current_player + 1) >= players.size())
	{
		current_player = 0;
		
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

