#include <iostream>
#include "TTTGame.h"

using namespace std;

int main()
{
	//initialise game
	TTTgame tictactoe = TTTgame(2);

	//get game board
	char tictactoeboard[3][3];
	char* boardvia;
	boardvia = tictactoe.TTTupdate();
	for (size_t i = 0; i < 3; ++i)
		for (size_t i2 = 0; i2 < 3; ++i2)
			tictactoeboard[i][i2] = boardvia[i2 + 3 * i];

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