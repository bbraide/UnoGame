// ACTION CARD CLASS - covers special colored cards with effects (skip, reverse, +2)

#ifndef ACTIONCARD_H
#define ACTIONCARD_H

#include "Card.h"

class ActionCard : public Card {
public:
	// cardVal: "skip", "reverse", "+2"; color: red, yellow, green, blue
	ActionCard(string color, string cardVal, bool isInDeck, bool hasBeenDrawn);
	~ActionCard() = default;

	bool CanPlayOn(Card* topCard) const override;  // overrides pure virtual function in Card class
};

#endif
