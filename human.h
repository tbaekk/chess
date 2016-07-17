#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "player.h"
#include <string>

class Human: public Player {
	std::string colour;
	bool win;
	bool stalemate;
	bool checkmate;
 public:
	Huamn(std::string colour, bool win, bool stalemate, bool checkmate);
	~Human();
	std::string getColour();
	bool isWin();
	bool isStalemate();
	bool isCheckmate();
};

#endif