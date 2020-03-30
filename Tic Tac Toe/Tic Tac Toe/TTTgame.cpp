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
	for (size_t i = 0; i < sizeof(*TTTboard) / sizeof(**TTTboard); ++i)
		for (size_t i2 = 0; i2 < sizeof(TTTboard) / sizeof(*TTTboard); ++i2)
		{
			if (TTTboard[i][i2] == TTTplace)
			{
				status = true;
				TTTboard[i][i2] = TTTplayer_current;
				TTTwinner = ((TTTboard[i][0] == TTTboard[i][1] && TTTboard[i][0] == TTTboard[i][2]) || (TTTboard[0][i2] == TTTboard[1][i2]) && TTTboard[0][i2] == TTTboard[2][i2]) || (TTTboard[0][0] == TTTboard[1][1] && TTTboard[0][0] == TTTboard[2][2]) || (TTTboard[0][2] == TTTboard[1][1] && TTTboard[0][2] == TTTboard[2][0]) ? TTTplayer_current : '0';
				if (TTTwinner == '0')
					TTTplayer_current = TTTplayer_current == 'X' ? 'O' : 'X';

			}
		}

	return status;
	//Still needs to check if there is a win and toggle current player
};

char* TTTgame::TTTupdate()
{
	return *TTTboard;
};

bool TTTgame::TTTwinstatus()
{
	bool status = false;
	if (TTTwinner != '0')
		status = true;
	return status;
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
	char tictactoeboard[3][3];
	char *boardvia;
	boardvia = tictactoe.TTTupdate();
	for (size_t i = 0; i < 3; ++i)
		for (size_t i2 = 0; i2 < 3; ++i2)
			tictactoeboard[i][i2] = boardvia[i2+3*i];

	std::cout << tictactoe.TTTplayer_current << "'s move first" << '\n' << '\n';
	for (size_t i = 0; i < sizeof(*tictactoeboard) / sizeof(**tictactoeboard); ++i)
	{
		for (size_t i2 = 0; i2 < sizeof(tictactoeboard) / sizeof(*tictactoeboard); ++i2)
			std::cout << tictactoeboard[i][i2] << "    ";
		std::cout << '\n';
	}

	//game loop
	while (!(tictactoe.TTTwinstatus()))
	{
		//make move
		char move;
		std::cin >> move;
		if (tictactoe.TTTmove(move))
		{
			boardvia = tictactoe.TTTupdate();
			for (size_t i = 0; i < 3; ++i)
			{
				for (size_t i2 = 0; i2 < 3; ++i2)
				{
					tictactoeboard[i][i2] = boardvia[i2 + 3 * i];
					std::cout << tictactoeboard[i][i2] << "    ";
				}
				std::cout << '\n';
			}
		}
		else
		{
			std::cout << "Bad Move, try again" << '\n' << '\n';
		}
		if (tictactoe.TTTwinstatus())
			std::cout << "Winner is " << tictactoe.TTTplayer_current << "!!!" << '\n' << '\n';
		else
			std::cout << "Next move for " << tictactoe.TTTplayer_current << '\n' << '\n';
	}
}
*/