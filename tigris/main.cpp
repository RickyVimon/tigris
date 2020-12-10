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
	else if (t == "market" || t == "trader")
	{
		return Type::green;
	}
	else return Type::invalid;
};

bool existsType(std::string t)
{
	if (t == "settlement" || t == "king" || t == "farm" || t == "farmer" || t == "temple" || t == "priest" || t == "market" || t == "trader")
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

	bool isEmpty() { if (token != nullptr) return false; else return true; };

	bool hasTemple() { if (type == CellType::TEMPLE) return true; else return false; };

	CellType getType() { return type; };

	Position getPosition() { return pos; };

	void setToken(Token* t);
	Token* getToken() { return token; };
	void setTypeCatastrophe() { type = CellType::CATASTROPHE; };
	bool isCatastrophe() { if (type == CellType::CATASTROPHE) return true; else return false; };

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
	void removeTokenFromArea(Token* token);

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

	void findIsland(Token * current_token, std::vector<Token*> &island_tokens, std::vector<Token*> &pool);

	bool checkRevolt(Token* leader_token, Cell* c);

	void solveRevolt(std::vector<std::string> args);

	bool solveWar(std::vector<std::string> args);

	bool buildMonument(std::vector<std::string> args);

	bool checkWar(Token* tile_token, Cell* c);

	bool isPositionCorrect(const Position & pos);

	void newArea(std::vector<Token*> tokens);
	void newArea(Token* token);

	std::vector<Area*> getAdjacentAreas(const Position &p);
	std::vector<Token*> getAdjacentTokens(const Position &pos);
	bool hasAdjacentTemple(const Position &pos);
	bool solveCatastrophe(const std::vector<std::string> &args);

	void takeTreasure(const Position & pos);

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
	bool flag_war = false;
	bool flag_revolt = false;
	Token* conflict_token = nullptr;

};


class Token
{

public:
	Token(Board* b, Player* p, Cell* c, Type t, bool il = false, bool tr = false, bool m = false) : board(b), owner(p), cell(c), type(t), isleader(il), istreasure(tr), ismonument(m){}

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
	bool takeTreasure() { bool out = istreasure; if (istreasure) istreasure = false; return out; };
	bool isTreasure() { return istreasure; };
	bool isMonument() { return ismonument; };
	void setMonument(bool s) { ismonument = s; };
	void Destroy();
	void backToHand();
	void changeType(Type t) { type = t; };

public:
	Player* owner;
	Area* area = nullptr;

private:
	Cell* cell;
	Type type;
	Board* board;
	bool isleader;
	bool istreasure;
	bool ismonument;
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
	void takeTreasures(const std::vector<std::string>& args);
	void addTreasures() { ++treasures; };

	std::vector<int> getVictoryPoints() { return victory_points; };
	std::vector<Token*> tokens;

public:
	int catastrophe_tiles = 2;

private:
	Board* board;
	int actions = 2;
	std::vector<int> victory_points;
	int treasures = 0;
};

class Game
{
public:

	Game()
	{
		board = new Board();
		board->init();;
	};
	~Game()
	{
	}

	void addNewPlayer(std::string initial_tiles);
	void nextPlayer();
	std::vector<std::string> readInput();
	void startGame(std::vector<std::string> input_lines);
	Commands resolveCommand(std::string input);

private:
	//index to track the current player
	int current_player;
	std::vector<Player*> players;
	Board* board;
};


//----------------------------------------------------------------END OF THE HEADER----------------------------------------------------
void Board::dissolveArea(const Position &pos)
{
	std::vector<Token*> colliding_tokens = getAdjacentTokens(pos);

	for (unsigned int i = 0; i < colliding_tokens.size(); ++i)
	{
		std::vector<Token*> island_tokens, pool;
		Position p = colliding_tokens[i]->getPosition();
		if (isPositionCorrect(p))
		{
			pool = getAdjacentTokens(p);
		}
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

void Board::findIsland(Token* current_token, std::vector<Token*> &island_tokens, std::vector<Token*> &pool)
{
	//send current_token to visited
	island_tokens.emplace_back(current_token);
	//remove current token from the pool
	pool.erase(std::remove(pool.begin(), pool.end(), current_token), pool.end());
	
	std::vector<Token*> colliding_tokens;
	Position p = current_token->getPosition();
	if (isPositionCorrect(p))
	{
		colliding_tokens = getAdjacentTokens(p);
	}


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
		{
			//Otherwise just add it to the pool
			pool.insert(pool.begin(), colliding_tokens[i]);
		}
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

void Area::removeTokenFromArea(Token * token)
{
	area_tokens.erase(std::remove(area_tokens.begin(), area_tokens.end(), token), area_tokens.end());

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
	if(index > 0 && (unsigned int)index <= victory_points.size())
	++victory_points[index - 1];
}

void Player::printPoints()
{
	std::cout << "victory " << getTreasures() << " (" << victory_points[0] << " " << victory_points[1] << " " << victory_points[2] << " " << victory_points[3] << ")\n";
}

void Player::takeTreasures(const std::vector<std::string>& args)
{
	//Check if position is correct within the board dimensions.
	Position p;
	p.x = stoi(args[1]);
	p.y = stoi(args[2]);
	if (!board->isPositionCorrect(p))
	{
		std::cout << "exception: invalid board space position \n";
		return;
	}
	board->takeTreasure(p);
}

void Token::setCell(Cell*c)
{
	cell = c;
	
}

bool Board::checkRevolt(Token* leader_token, Cell* cell)
{
	std::vector<Area*> adjacent_areas = getAdjacentAreas(cell->getPosition());
	int coliding_kingdoms = 0;
	int last_index_kingdom;
	for (unsigned int i = 0; i < adjacent_areas.size(); ++i)
	{
		if (adjacent_areas[i]->isKingdom())
		{
			++coliding_kingdoms;
			last_index_kingdom = i;
		}
	}
	if (coliding_kingdoms > 1)
	{
		std::cout << "exception: cannot place a leader that would unite two kingdoms\n";
		return false;
	}
	if (coliding_kingdoms == 1)
	{
		for (unsigned int i = 0; i < adjacent_areas[last_index_kingdom]->area_tokens.size(); ++i)
		{
			if (adjacent_areas[last_index_kingdom]->area_tokens[i]->isLeader() && adjacent_areas[last_index_kingdom]->area_tokens[i]->getType() == leader_token->getType())
			{
				//call for revolt;
				flag_revolt = true;				
				conflict_token = leader_token; 
				return true;
			}
		}
		//Join kingdom
		adjacent_areas[last_index_kingdom]->area_tokens.emplace_back(leader_token);
		leader_token->setArea(adjacent_areas[last_index_kingdom]);
	}
	else
	{
		//create first kingdom
		newArea(leader_token);
		adjacent_areas.emplace_back(leader_token->area);
	}
	//unite regions to the kingdom if any
	fuzeAreas(adjacent_areas);
	leader_token->setCell(cell);
	cell->setToken(leader_token);
	return true;
}

void Board::solveRevolt(std::vector<std::string> args)
{
	if (conflict_token == nullptr)
	{
		std::cout << "exception: no leader found on that revolt";
		return;
	}
	Position p;
	p.x = stoi(args[1]);
	p.y = stoi(args[2]);
	if (!isPositionCorrect(p))
	{
		std::cout << "exception: invalid board space position \n";
		return;
	}

	Token* defender = nullptr;
	for (unsigned int i = 0; i < conflict_token->area->area_tokens.size(); ++i)
	{
		if (conflict_token->area->area_tokens[i]->getType() == conflict_token->getType() && conflict_token->area->area_tokens[i]!=conflict_token && conflict_token->area->area_tokens[i]->isLeader())
		{
			defender = conflict_token->area->area_tokens[i];
		}
	}
	if (defender == nullptr)
		return;
	
	std::vector<Token*> attacker_tokens, defender_tokens;
	attacker_tokens = getAdjacentTokens(conflict_token->getPosition());
	defender_tokens = getAdjacentTokens(defender->getPosition());

	//Count adjacent temples
	int attacker_temples = 0, defender_temples = 0;
	for (unsigned  i = 0; i < attacker_tokens.size(); ++i)
	{
		if (!attacker_tokens[i]->isLeader() && (attacker_tokens[i]->getType() == Type::red))
			attacker_temples++;
	}
	for (unsigned int i = 0; i < defender_tokens.size(); ++i)
	{
		if (!defender_tokens[i]->isLeader() && (defender_tokens[i]->getType() == Type::red))
			defender_temples++;
	}

	attacker_temples += stoi(args[1]);
	attacker_temples += stoi(args[2]);

	if (attacker_temples < defender_temples)
	{
		//attaker wins
		Position p = defender->getPosition();
		defender->backToHand();
		dissolveArea(p);
	}
	else
	{
		//defender wins
		Position p = conflict_token->getPosition();
		conflict_token->backToHand();
		dissolveArea(p);
	}


}

bool Board::solveWar(std::vector<std::string> args)
{
	Position p;
	p.x = stoi(args[2]);
	p.y = stoi(args[3]);
	if (!isPositionCorrect(p))
	{
		std::cout << "exception: invalid board space position \n";
		return false;
	}
	//TODO: Check number of tiles of the same color of the original areas.

	return false;
}

bool Board::buildMonument(std::vector<std::string> args)
{
	Position p;
	p.x = stoi(args[3]);
	p.y = stoi(args[4]);
	{
		std::cout << "exception: invalid board space position \n";
		return false;
	}
	Cell* cell1 = cells[p.x][p.y];
	Cell* cell2 = cells[cell1->getPosition().x][cell1->getPosition().y + 1];
	Cell* cell3 = cells[cell1->getPosition().x + 1][cell1->getPosition().y];
	Cell* cell4 = cells[cell1->getPosition().x + 1][cell1->getPosition().y + 1];
	Token* cell1_tok = cell1->getToken();
	Token* cell2_tok = cell2->getToken();
	Token* cell3_tok = cell3->getToken();
	Token* cell4_tok = cell4->getToken();
	

	if (cell1_tok->getType() == cell2_tok->getType() && cell3_tok->getType() == cell4_tok->getType() && cell1_tok->getType() == cell3_tok->getType())
	{
		if (!existsType(args[2]))
		{
			return false;
		}
	
		//we can build the monument
		cell1_tok->setMonument(true);
		cell2_tok->setMonument(true);
		cell3_tok->setMonument(true);
		cell4_tok->setMonument(true);

		cell1_tok->changeType(checkType(args[2]));
		cell2_tok->changeType(checkType(args[2]));
		cell3_tok->changeType(checkType(args[2]));
		cell4_tok->changeType(checkType(args[2]));

		return true;
	}
	else
		return false;

}

bool Board::checkWar(Token* incoming_tile, Cell* cell)
{
	std::vector<Area*> adjacent_areas = getAdjacentAreas(cell->getPosition());
	if (adjacent_areas.size() == 0)
	{
		newArea(incoming_tile);
		incoming_tile->setCell(cell);
		cell->setToken(incoming_tile);
		return true;
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
			flag_war = true;
			return true;
		}
		else
		{
			fuzeAreas(adj_kingdoms);
			incoming_tile->setCell(cell);
			cell->setToken(incoming_tile);
			return true;
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
	incoming_tile->setCell(cell);
	cell->setToken(incoming_tile);
	return true;
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
	adjacent_tokens.clear();
	Cell* c;
	if (pos.x + 1 < BOARD_WIDTH)
	{
		Position temp_pos = pos;
		temp_pos.x += 1;
		c = cells[temp_pos.x][temp_pos.y];
		if (!isCellEmpty(temp_pos) && !c->isCatastrophe() && (c->getToken()->getType() != Type::invalid))
			adjacent_tokens.emplace_back(c->getToken());

	}
	if (pos.x - 1 > 0)
	{
		Position temp_pos1 = pos;
		temp_pos1.x -= 1;
		c = cells[temp_pos1.x][temp_pos1.y];
		if (!isCellEmpty(temp_pos1) && !c->isCatastrophe() && (c->getToken()->getType() != Type::invalid))
			adjacent_tokens.emplace_back(c->getToken());

	}
	if (pos.y + 1 < BOARD_LENGTH)
	{
		Position temp_pos2 = pos;
		temp_pos2.y += 1;
		c = cells[temp_pos2.x][temp_pos2.y];
		if (!isCellEmpty(temp_pos2) && !c->isCatastrophe() && (c->getToken()->getType() != Type::invalid))
			adjacent_tokens.emplace_back(c->getToken());
	}
	if (pos.y - 1 > 0)
	{
		Position temp_pos3 = pos;
		temp_pos3.y -= 1;
		c = cells[temp_pos3.x][temp_pos3.y];
		if (!isCellEmpty(temp_pos3) && !c->isCatastrophe() && (c->getToken()->getType() != Type::invalid))
			adjacent_tokens.emplace_back(c->getToken());
	}
	return adjacent_tokens;
}

bool Board::solveCatastrophe(const std::vector<std::string> &args)
{
	//Check if position is correct within the board dimensions.
	Position pos;
	pos.x = stoi(args[1]);
	pos.y = stoi(args[2]);
	if (!isPositionCorrect(pos))
	{
		std::cout << "exception: invalid board space position \n";
		return false;
	}
	
	Cell* cell = getCell(pos);
	if (!cell->isEmpty())
	{
		Token* token_to_destroy = cell->getToken();
		if (token_to_destroy->isMonument())
		{
			std::cout << "exception: cannot place catastrophe if space is bearing a monument\n";
			return false;
		}
		else if (token_to_destroy->isLeader())
		{
			std::cout << "exception: cannot place catastrophe if space alredy contains a leader\n";
			return false;
		}
		else if (token_to_destroy->isTreasure())
		{
			std::cout << "exception: cannot place catastrophe if space is bearing a treasure\n";
			return false;
		}
		else
		{
			if (token_to_destroy->area != nullptr)
			{
				Token* leader = token_to_destroy->area->getLeader();
				if (leader != nullptr)
				{
					token_to_destroy->Destroy();
					if (!hasAdjacentTemple(leader->getPosition()))
					{
						leader->backToHand();
					}
				}
			}
			else
				token_to_destroy->Destroy();

			dissolveArea(pos);
		}

	}
	else
		cell->setTypeCatastrophe();


	return true;

}

void Board::takeTreasure(const Position &pos)
{
	Cell* c = getCell(pos);
	if (c->isEmpty())
	{
		std::cout << "exception: trying to take treasure where there is none\n";
	}
	else
	{
		Token* leader = c->getToken()->area->getLeader();
		if(leader != nullptr)
			if (leader->getType() == Type::green)
			{
				if (c->getToken()->takeTreasure())
				{
					leader->owner->addTreasures();
				}
				else
					std::cout << "exception: trying to take treasure where there is none\n";
			}
	}
}

void Board::fuzeAreas(std::vector<Area*> areas_to_fuze)
{
	std::vector<Token*> tokens_to_fuze;
	for (unsigned int i = 0; i < areas_to_fuze.size(); ++i)
	{
		for (unsigned int k = 0; k < areas_to_fuze[i]->area_tokens.size(); ++k)
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

void Token::Destroy()
{
	this->area = nullptr;
	this->cell = nullptr;
	this->owner = nullptr;
	this->type = Type::invalid;
}

void Token::backToHand()
{
	cell->setToken(nullptr);
	cell = nullptr;
	area->removeTokenFromArea(this);
	area = nullptr;
}


bool Board::hasAdjacentTemple(const Position &pos)
{
	if (isPositionCorrect(pos))
	{
		std::vector<Token*> adjacent_tokens = getAdjacentTokens(pos);
		for (unsigned  int i = 0; i < adjacent_tokens.size(); ++i)
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
	p.x = stoi(args[2]);
	p.y = stoi(args[3]);
	if (!isPositionCorrect(p))
	{
		std::cout << "exception: invalid board space position \n";
		return false;
	}

	Cell* cell = getCell(p);
	if (cell->isCatastrophe())
	{
		std::cout << "exception: board space is already occupied \n";
		return false;
	}

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


	if (token->isLeader())
	{
		return checkRevolt(token, cell);
	}
	else
		return checkWar(token, cell);
}

std::vector<Area*> Board::getAdjacentAreas(const Position &pos)
{
	Area* new_area;
	std::vector<Area*> areas;

	if ((pos.x + 1) < BOARD_WIDTH)
	{
		Position temp_pos = pos;
		temp_pos.x += 1;
		new_area = getArea(temp_pos);
		if (new_area != nullptr)
			areas.emplace_back(new_area);
	}
	if ((pos.x - 1) >= 0)
	{
		Position temp_pos = pos;
		temp_pos.x -= 1;
		new_area = getArea(temp_pos);
		if (new_area != nullptr)
			areas.emplace_back(new_area);
	}
	if ((pos.y + 1) < BOARD_LENGTH)
	{
		Position temp_pos = pos;
		temp_pos.y += 1;
		new_area = getArea(temp_pos);
		if (new_area != nullptr)
			areas.emplace_back(new_area);
	}
	if ((pos.y - 1) >= 0)
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

	for (unsigned int i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i]->getType() == checkType(args[1]))
		{
			Position pos = tokens[i]->getPosition();
			if (pos.x != -1)
			{		
				board->cells[pos.x][pos.y]->setToken(nullptr);

				tokens[i]->setCell(nullptr);
				if (tokens[i]->getArea() != nullptr)
				{
					tokens[i]->getArea()->removeTokenFromArea(tokens[i]);
					tokens[i]->setArea(nullptr);
				}
				board->dissolveArea(pos);
			}
			//leader on the hand
			if (board->placeToken(tokens[i], args))
			{
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
	for (unsigned int k = 0; k < tokens.size(); ++k)
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
	for (unsigned int i = 0; i < BOARD_LENGTH; ++i)
	{
		for (unsigned int j = 0; j < BOARD_WIDTH; ++j)
		{
			Cell* c = new Cell(j, i, init_board[count]);
			cells[j][i] = c;
			if (c->hasTemple())
			{
				c->setToken(new Token(this, p, c, Type::red, false, true));
				newArea(c->getToken());
			}
			++count;
		}
	}
}


std::vector<std::string> Game::readInput()
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

void Game::addNewPlayer(std::string initial_tiles)
{
	Player* p = new Player(board, initial_tiles);
	players.emplace_back(p);
}

Commands Game::resolveCommand(std::string input)
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

void Game::startGame(std::vector<std::string> input_lines)
{

	for (unsigned int i = 0; i < input_lines.size(); ++i)
	{

		if (input_lines[i] == "----")
		{
			//count missing tiles
			int tiles = 0;
			for (unsigned int k = 0; k < players[current_player]->tokens.size(); ++k)
			{
				if (!players[current_player]->tokens[k]->isLeader())
					++tiles;
			}
			if ((6 - tiles) != 0)
			{
				std::cout << "exception: invalid number or player tiles\n";
				//next line

				continue;
			}
			else
			{
				players[current_player]->resetActions();
				nextPlayer();
			}
			++i;
			//endgame
			if (i > input_lines.size() - 1)
				break;
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
			if (board->flag_revolt || board->flag_war)
			{
				std::cout << "board has unresolved conflicts\n";
				continue;
			}
			if (arguments.size() != 4)
			{
				std::cout << "invalid command \n";
			}
			else if (players[current_player]->hasActionsLeft())
			{
				if (players[current_player]->placeTile(arguments))
					players[current_player]->useAction();
			}
			break;

		case leader:
			if (board->flag_revolt || board->flag_war)
			{
				std::cout << "board has unresolved conflicts\n";
				continue;
			}
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
			if (board->flag_revolt || board->flag_war)
			{
				std::cout << "board has unresolved conflicts\n";
				continue;
			}
			players[current_player]->refreshTiles(arguments);
			break;

		case treasure:
			if (board->flag_revolt || board->flag_war)
			{
				std::cout << "board has unresolved conflicts\n";
				continue;
			}
			if (arguments.size() != 3)
				std::cout << "exception: taking incorrect number of treasures\n";
			else
			{
				players[current_player]->takeTreasures(arguments);
			}
			break;

		case catastrophe:
			if (board->flag_revolt || board->flag_war)
			{
				std::cout << "exception: board has unresolved conflicts\n";
				continue;
			}
			if (arguments.size() != 3)
			{
				std::cout << "invalid command \n";
				continue;
			}
			else
			{
				if (players[current_player]->catastrophe_tiles <= 0)
				{
					std::cout << "exception: invalid number or player tiles \n";
				}
				else if (board->solveCatastrophe(arguments))
				{
					players[current_player]->useAction();
					--players[current_player]->catastrophe_tiles;
				}
			}

			break;

		case revolt:
			if (!board->flag_revolt)
			{
				std::cout << "exception: could not find conflict attacker or deffender\n";
				continue;
			}
			if (arguments.size() != 3)
			{
				std::cout << "invalid command \n";
				continue;
			}
			board->solveRevolt(arguments);
			board->flag_revolt = false;
			break;

		case war:
			if (!board->flag_war)
			{
				std::cout << "could not find conflict attacker or deffender\n";
				continue;
			}
			board->flag_war = false;
			break;

		case monument:
			if (board->flag_revolt || board->flag_war)
			{
				std::cout << "board has unresolved conflicts\n";
				continue;
			}
			board->buildMonument(arguments);
			break;


		default:
			std::cout << "invalid command \n";
		}
	}
};

void Game::nextPlayer()
{
	for (unsigned int k = 0; k < players.size() ; k++)
	{
		players[k]->printPoints();
	}
	std::cout << "----\n";
	if ((unsigned int)((current_player + 1)) >= players.size())
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
	

	return 0;
}

