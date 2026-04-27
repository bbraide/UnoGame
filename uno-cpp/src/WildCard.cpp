// WILDCARD.CPP

#include "WildCard.h"

// constructor for WildCard class, calls Card constructor to initialize the common attributes of all cards (color, cardVal, isInDeck, hasBeenDrawn)
WildCard::WildCard(string cardVal, bool isInDeck, bool hasBeenDrawn) 
	: Card("TBD", cardVal, isInDeck, hasBeenDrawn) {}

bool WildCard::CanPlayOn(Card* topCard) const {
	// wild cards can always be played on top of any card. game will prompt player to choose color after
	return true;
}