// LINEUP CLASS - doubly linked list of Player objects; manages turn order, reversals, and skips

#ifndef LINEUP_H
#define LINEUP_H

#include "Player.h"

class Lineup {
public:
	Lineup();
	~Lineup();

	void Clear(); // unhooks all pointers without deleting players

	Player* GetHead() const;

	// adds a player to the tail of the lineup
	void AddPlayer(Player* newPlayer);

	// returns the player whose turn it is currently
	Player* GetCurrentPlayer() const;

	// returns the next player in the lineup (used for determining who to skip and who goes after current player)
	// will be useful for printing "-> Boma is skipped!" and "-> Boma goes next!" in the Game class when a skip or normal turn happens
	Player* PeekNextPlayer() const;

	// moves the current player pointer to the next player in the lineup (used for normal turn progression and for skipping players)
	void Advance();

	// reverses the order of the lineup (used for reverse cards)
	// in 2-player game, reverse card just acts like a skip, so we can just call Advance() to skip the other player and keep the same current player
	void Reverse();

	// skips the next player (advance twice)
	// used by skip cards and +2/+4 cards after the penalty is applied to the next player
	void Skip();

	int Size() const;  // returns the number of players in the lineup

	// prints the names of the players in the lineup in order, starting from the current player and going through the lineup once
	// ex. if lineup is A -> B -> C -> D and current player is B, will print "B -> C -> D -> A"
	// mainPlayerName used to append " (you)" to the main player's name when printing the lineup, so if main player is C, will print "B -> C (you) -> D -> A"
	void DisplayLineup(const string& mainPlayerName) const;

private:
	Player* m_head;  // pointer to the first player in the lineup (not necessarily who goes first, since current player can be anywhere in the lineup)
	Player* m_tail;  // pointer to the last player in the lineup
	Player* m_currentPlayer;  // pointer to the player whose turn it is currently
	bool m_forward;  // true if lineup is in forward order, false if lineup is in reverse order
	int m_size;  // number of players in the lineup
};

#endif

