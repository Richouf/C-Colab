#pragma once

#ifndef TTTGAME_H_
#define TTTGAME_H_

class TTTgame
{
	int players;

public:
	TTTgame();
	TTTgame(int);
	bool TTTmove(char TTTplace);
	bool TTTwinstatus();
	char TTTplayer_current = 'X';
	char* TTTupdate();
	bool TTTendGame();

private:
	char TTTboard[3][3] = { {'1','2','3'},{'4','5','6'},{'7','8','9'} };
	char TTTwinner = '0';
};

#endif TTTGAME_H_