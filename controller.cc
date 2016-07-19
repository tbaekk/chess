#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include "controller.h"

Controller::Controller(): in{&cin}, board{this}, customized{false}, currPlayer{"white"} {}
Controller::~Controller() {}

void Controller::notify(int r, int c, int destr, int destc) {
	if (!board.canMove(board.checkState(r,c), destr, destc, currPlayer)) throw iv;
	board.checkState(r,c)->move(destr,destc);
	td->notify(&board);
}

void Controller::setNextPlayer() {
	if (currPlayer == "white") currPlayer = "black";
	else currPlayer = "white";
}

void Controller::init() {
	try {
		string w, b, d;
		*in >> w >> b >> d;
		 //Player init
		if (!iv.isPlayer(w,b)) throw iv;
		if (w == "human") wp = make_shared<Human>("white");
		else wp = make_shared<Computer>("white");
		if (b == "human") bp = make_shared<Human>("black");
		else bp = make_shared<Computer>("black");

		if (!customized) {
			board.init(wp->getColour(), bp->getColour()); //Board init
		}
		if (!customized && d == "td") {
			td = make_shared<TextDisplay>();
			td->notify(&board);
		}
		else {
			cout << "Initialize graphic display" << endl;
		}
	} catch (InputValidation e) {
		throw e;
	}
}

void Controller::setup() {
	customized = true;
	iv.setupMessage();
	td = make_shared<TextDisplay>();
	board.setup();
	try {
		string cmd;
		while (*in >> cmd) {
			try {
				char p, r, c;
				string colour;
				if (cmd == "+") {
					*in >> p >> c >> r;
					if (iv.isValid(r,c,p)) {
						board.setup_add(p, r-'0'-1, c-'a');
						td->notify(&board);
					}
					else throw iv;
				} else if (cmd == "-") {
					*in >> c >> r;
					if (iv.isValid(r,c)) {
						board.setup_delete(r-'0'-1, c-'a');
						td->notify(&board);
					}
					else throw iv;
				} else if (cmd == "=") {
					*in >> colour;
					currPlayer = colour;
				} else if (cmd == "done") {
					break;
					//check condtion
					// if true exit
					// else back to loop
				} else throw iv;
			} catch (InputValidation e) {
				e.errorMessage();
			}
		}	
	} catch(ios::failure) {}
}

void Controller::game() {
	try {
		init();
		iv.gameMessage(); //Start new game
		while (1) {
			cout << *td;
			iv.currPlayerMessage(currPlayer);
			try {
				string cmd;
				*in >> cmd;
				if (cmd == "move") {
					string move;
					getline(*in,move);
					vector<string> cord;
					if (move == "") cout << "Call move Computer" << endl;
					else {
						istringstream iss{move};
						string tmp;
						while(iss >> tmp) cord.emplace_back(tmp);

						if (cord.size() != 3 && cord.size() != 2) throw iv;
						else if (cord.size() == 2) {
							if (!iv.isValid(cord[0][1],cord[0][0]) || !iv.isValid(cord[1][1],cord[1][0])) throw iv;
							int r,c,destr,destc;
							r = cord[0][1]-'0'-1;
							c = cord[0][0]-'a';	
							destr = cord[1][1]-'0'-1;
							destc = cord[1][0]-'a';
							notify(r,c,destr,destc);
						} else {
							if(!iv.isValid(cord[0][1],cord[0][0],cord[2][0]) || !iv.isValid(cord[1][1],cord[1][0],cord[2][0])) throw iv;
							cout << "//Call pawn promotion" << endl;
						}
					}
				} else if (cmd == "resign") {

				} else throw iv;

				// Display state of the game
				// if checkmate: out checkmate -> break
				// if stalemate: out stalemate -> break
				// if check: out check

				setNextPlayer();
			} catch (InputValidation e) {
				e.errorMessage();
			}
		}
		if(customized) customized = false;
	} catch (InputValidation e) {
		throw e;
	}
}

void Controller::play() {
	in->exceptions(ios::failbit|ios::eofbit);
	iv.menuMessage();
	try {
		string cmd;
		while (*in >> cmd) {
			try {
				if (cmd == "game") game();
				else if (cmd == "setup") setup();
				else if (cmd == "quit") break;
				else throw iv;
			} catch (InputValidation e) {
				e.errorMessage();
			} //input failure for menu
		}
	} catch (ios::failure) {} //eof
}