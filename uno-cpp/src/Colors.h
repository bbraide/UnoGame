// COLORS - make the game pretty!!! 
// we can use ANSI escape codes to print colored text in the terminal, 
// which will make it easier for players to identify card colors and make the game more visually appealing
// OTHER COLORS
//	purple: current player's name in the lineup (to indicate whose turn it is)
//	orange: special cards (action, wild) to make them stand out more from number cards

#ifndef COLORS_H
#define COLORS_H

#include <string>
using namespace std;

// ==============================================================================================
// RESET COLOR - always call after printing colored text to reset back to default color
// ==============================================================================================
const string RESET = "\033[0m";
const string BOLD = "\033[1m";


// ==============================================================================================
// UNO CARD COLORS
// will be used like cout << RED << "Red 5" << RESET; to print "Red 5" in red text
// ==============================================================================================
const string RED = "\033[38;2;255;99;99m";
const string YELLOW = "\033[38;2;255;217;50m";
const string GREEN = "\033[38;2;100;220;100m";
const string BLUE = "\033[38;2;100;180;255m";


// ==============================================================================================
// SPECIAL CARDS: COLOR + BOLD
// wild and +4 cards will be printed in orange to make them stand out more from number cards
// will be used like cout << BOLD_WILD << "Wild" << RESET; to print "Wild" in bold orange text
// ==============================================================================================
const string BOLD_RED = BOLD + RED;
const string BOLD_YELLOW = BOLD + YELLOW;
const string BOLD_GREEN = BOLD + GREEN;
const string BOLD_BLUE = BOLD + BLUE;
const string BOLD_WILD = BOLD + "\033[38;2;255;179;71m";  // bold orange color for wild and +4 cards 


// ==============================================================================================
// GAME TEXT/UI
// ==============================================================================================
const string LAVENDER = "\033[38;2;180;160;255m";			// current player highlight
const string BOLD_TURN = BOLD + "\033[38;2;155;127;255m";	// "It's your turn!"
const string WARNING = "\033[38;2;255;179;71m";				// special card effect messages
const string PLAYED = "\033[38;2;180;178;169m";				// cards already played (grey)


// ==============================================================================================
// CARDCOLOR HELPER - returns the right string for a card's color field
//  usage: cout << cardColor(card->getColor()) << card->getCardVal() << RESET;
// ==============================================================================================
inline string cardColor(const string& color, bool isSpecial = false) {
	string base; 
	if (color == "red")	base = isSpecial ? BOLD_RED : RED;
	else if (color == "yellow") base = isSpecial ? BOLD_YELLOW : YELLOW;
	else if (color == "green") base = isSpecial ? BOLD_GREEN : GREEN;
	else if (color == "blue") base = isSpecial ? BOLD_BLUE : BLUE;
	else base = BOLD_WILD;   // "TBD" — wild or +4
	return base;
}


#endif