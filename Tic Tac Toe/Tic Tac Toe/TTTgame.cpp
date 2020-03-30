#include "TTTgame.h"

TTTgame::TTTgame()
{
	players = 1;
};

TTTgame::TTTgame(int p)
{
	players = (p > 0 ? (p > 2 ? 2 : p) : 1);
};

bool TTTgame::TTTmove(char TTTplace)
{
	bool status = false;
	for(size_t i = 0; i < sizeof(*TTTboard) / sizeof(**TTTboard); ++i)
		for (size_t i2 = 0; i2 < sizeof(TTTboard) / sizeof(*TTTboard); ++i2)
		{
			if (TTTboard[i][i2] == TTTplace)
			{
				status = true;
				TTTboard[i][i2] = TTTplayer_current;
			}
		}
	return status;
	//Still needs to check if there is a win and toggle current player
};

const char* TTTgame::TTTupdate()
{
	return *TTTboard;
};

const char* TTTgame::TTTwinplayer()
{
	return &TTTwinner;
};

/*
#include <iostream>
#include "TTTgame.h"

using namespace std;

int main()
{
	//initialise game
	TTTgame tictactoe = TTTgame(2);

//get game board
char tictactoeboard[3][3] = tictactoe.TTTupdate();

const char* boardvia = *tictactoeboard;

std::cout << tictactoe.TTTplayer_current << "'s move first" << '\n';

//game loop
while (!(tictactoe.TTTwinstatus()))
{
	//make move
	char move;
	std::cin >> move;
	if (tictactoe.TTTmove(move))
	{
		boardvia = tictactoe.TTTupdate();
	}
	else
	{
		std::cout << "Bad Move, try again" << '\n';
	}
	std::cout << "Next move for" << tictactoe.TTTplayer_current << '\n';
}
}
*/