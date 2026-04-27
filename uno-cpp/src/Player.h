// Player header file
// Boma Braide
// November 28, 2024

#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"  // for storing player's deck of cards

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Player {
    public:
        // constructor for Player
        Player(string name);

        // destructor!
        ~Player();

        // name getter and setter
        string GetName() const;
        void SetName(string name);

        // DLL getters and setters
        Player* GetNext() const;
        void SetNext(Player* next);

        Player* GetPrev() const;
        void SetPrev(Player* prev);

        // CARD SORT FLAG 
        // set to true whenever the player gets a new card (either at the start of game or draw mid-game)
        // DisplayCards() will call SortCards() if this flag is true, and then set it back to false after sorting
        bool GetNeedsSort() const;
        void SetNeedsSort(bool sort);


		// CARD MANAGEMENT
		// adds a card to the player's deck; used for both dealing at the start of the game and drawing mid-game
		void AddCard(Card* card);

		// removes a card from the player's deck; used for playing a card from the player's hand
		// Game will push that card onto m_cardStack after the player plays it, so we don't need to worry about that here
		void RemoveCard(Card* card);


        // SORTING CARDS
        // separates unplayed cards (front) from played cards (back)
		// played cards - hasBeenPlayed = true, shown in red and placed at the end of the player's deck
        void SortCards();

        // when human player or AI wants to play a card, game must search for card in player's stack to FIRST make sure 
        // it exists in their hand, then make sure it's a valid play
        Card* GetCardAt(int index) const;

        // Displays player's cards
        // First: sort cards based on if they've been drawn or not 
        // if there exists cards that have been drawn, place at end and make the text red
        // we only care about SortCards and DisplayCards for human player (obvi)
        // group duplicates appear as "Red 6 (x2)" using nested for loops
        void DisplayCards() const;

        // returns the number of cards the player has not drawn
        int PlayerCardsLeft() const;

        // returns if player has already called UNO or not
        bool GetCalledUno() const;

        // reset UNO status or change to true if player has called UNO once
        void SetCalledUno(bool unoStatus);

    private:
        string m_name;
        vector<Card*> m_playerDeck;
        Player* m_next;
        Player* m_prev;
        bool m_needsSorting; // when main player gets more cards, needsSorting check to true.
                             // when true, cards will be sorted when DisplayCards() is called
        bool m_calledUno;       // true once UNO has been announced for this player
        // set to true when PlayerCardsLeft() == 1
        // set to false when more cards gets added to player's deck (AddCard())
};

#endif