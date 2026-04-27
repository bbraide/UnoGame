// WILD CARD CLASS - covers special colorless cards with effects (wild, +4)

#ifndef WILDCARD_H
#define WILDCARD_H

#include "Card.h"

// when played, game calls SetColor() setter function in Card class to set the color of the card to the color chosen by the player at play time
class WildCard : public Card {
public:
	// cardVal: "wild", "+4"; color: none until player chooses color when playing the card
	// color starts as "TBD" and is set at play time via SetColor() setter function in Card class
	WildCard(string cardVal, bool isInDeck, bool hasBeenDrawn);
	~WildCard() = default;

	// wilds can ALWAYS be played on top of any card, so we can just return true for this function
	bool CanPlayOn(Card* topCard) const override;  // overrides pure virtual function in Card class

	bool NeedsColorChoice() const { return GetColor() == "TBD"; }  // checks if the color of the card is still "TBD", which means the player hasn't chosen a color for the card yet
};

#endif