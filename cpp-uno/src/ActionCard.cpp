// ACTIONCARD.CPP

#include "ActionCard.h"

// constructor for ActionCard class, calls Card constructor to initialize the common attributes of all cards (color, cardVal, isInDeck, hasBeenDrawn)
ActionCard::ActionCard(string color, string cardVal, bool isInDeck, bool hasBeenDrawn) 
	: Card(color, cardVal, isInDeck, hasBeenDrawn) {}

bool ActionCard::CanPlayOn(Card* topCard) const {
	// match on color OR match on action type (ex. blue skip can go with blue skip; red skip can go with green skip)
	// wild and +4 cards are handled in the Game class, so we don't need to worry about them here since they can be played on any card
	return (GetColor() == topCard->GetColor() || GetCardVal() == topCard->GetCardVal());
}