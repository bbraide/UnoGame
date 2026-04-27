// Card(string color, string type, string cardVal, bool isInDeck, bool hasBeenDrawn)
#ifndef CARD_H
#define CARD_H

#include <string>
#include <iostream>

using namespace std;

// PROJECT TERMINOLOGY (for me mostly lol)
// "draw" - when a player takes a card from the draw pile and adds it to their hand
// "play" - when a player takes a card from their hand and places it on the discard pile, activating its effect 
// "deal" - when the game starts and each player is given 7 cards to start with, taken from the draw pile


// CARD LIFE CYCLE
// parsed from file -> isInDeck = false, hasBeenPlayed = false
// stored in m_deckOfCards -> isInDeck = true, hasBeenPlayed = false
// dealt to player -> isInDeck = false, hasBeenPlayed = false
// drawn mid-game -> isInDeck = false, hasBeenPlayed = false, just like when it's dealt at the start of the game, 
//															  but drawn mid-game is when a player takes a card from 
//															  the draw pile and adds it to their hand
// played by player -> isInDeck = false, hasBeenPlayed = true


// ====================================================================================================================================================================

// CARD CLASS - represents a single card in the game, with its color, type, value, quantity, and status (in deck or drawn)
// abstract/parent class for all card types (number, special (action, wild))
// Card describes itself but doesn't know how to interact with other cards or players, that will be handled by the Game class and its methods
class Card {
	public:
		Card(string color, string cardVal, bool isInDeck, bool hasBeenPlayed): 
			m_color(color), m_cardVal(cardVal), m_isInDeck(isInDeck), m_hasBeenPlayed(hasBeenPlayed){}

		virtual ~Card() = default;

		// pure virtual function to be implemented by derived classes (NumberCard, SpecialCard)
		virtual bool CanPlayOn(Card* topCard) const = 0;  // checks if this card can be played on top of the given card (topCard)

		// GETTERS
		string GetColor() const { return m_color; }
		string GetCardVal() const  { return m_cardVal; }
		int GetInDeck() const { return m_isInDeck;  }
		bool GetHasBeenPlayed() const { return m_hasBeenPlayed; }

		// SETTERS
		void SetColor(string color) { m_color = color; }
		void SetCardVal(string cardVal) { m_cardVal = cardVal; }
		void SetInDeck(bool isInDeck) { m_isInDeck = isInDeck; }
		void SetHasPlayed(bool hasBeenPlayed) { m_hasBeenPlayed = hasBeenPlayed; }

		// OVERLOADED == OPERATOR
		// cards are the same if they have the same COLOR AND VALUE (we don't worry about 
		// them being played or in the deck)
		bool operator==(const Card& other) const {
			return (m_color == other.m_color && m_cardVal == other.m_cardVal);
		}

	private:
		string m_color;  // red, yellow, green, blue, or TBD (cards like wild and +4)
		string m_cardVal;  // when loading cards, will be stoi'd if card type is "number"
		bool m_isInDeck;  // checks if card is a part of the main stack of 108 cards
		bool m_hasBeenPlayed;  // will be true if user plays the card from their hand 
		
};

#endif 