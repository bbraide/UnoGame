// NUMBERCARD.CPP

#include "NumberCard.h"

// constructor for NumberCard class, calls Card constructor to initialize the common attributes of all cards (color, cardVal, isInDeck, hasBeenDrawn)
NumberCard::NumberCard(string color, string cardVal, bool isInDeck, bool hasBeenDrawn) 
	: Card(color, cardVal, isInDeck, hasBeenDrawn) {}

bool NumberCard::CanPlayOn(Card* topCard) const {
	// a number card can be played on top of another card if it matches either the color or the number of the top card
	return (GetColor() == topCard->GetColor() || GetCardVal() == topCard->GetCardVal());
}