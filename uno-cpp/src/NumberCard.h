// NUMBERCARD - child class of Card. covers cards with numbers 0-9

#ifndef NUMBERCARD_H
#define NUMBERCARD_H

#include "Card.h"

class NumberCard : public Card {
public:
	// color: red, yellow, green, blue, cardVal: "0"-"9"
	NumberCard(string color, string cardVal, bool isInDeck, bool hasBeenDrawn);
	~NumberCard() = default;

	// checks if this card can be played on top of the given card (topCard)
	bool CanPlayOn(Card* topCard) const override;  // overrides pure virtual function in Card class
};

#endif