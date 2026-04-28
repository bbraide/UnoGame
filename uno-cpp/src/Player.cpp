#include "Player.h"
#include "Colors.h"

Player::Player(string name) : m_name(name), m_next(nullptr), m_prev(nullptr), m_needsSorting(false), m_calledUno(false) {}

// destructor
Player::~Player() {
	// Player doesn't own the Card* objects, Game does, so just clear the vector without deleting the Card* pointers
	m_playerDeck.clear();
}

// name getter and setter
string Player::GetName() const { return m_name; }
void Player::SetName(string name) { m_name = name; }

// pronoun getters and setters
string Player::GetPronounSubject() const { return m_pronounSubject; }
string Player::GetPronounObject() const { return m_pronounObject; }
string Player::GetPronounPossessive() const { return m_pronounPossessive; }
string Player::GetPronounReflexive() const { return m_pronounReflexive; }
void Player::SetPronouns(string subject, string object, string possessive, string reflexive) {
	m_pronounSubject = subject;
	m_pronounObject = object;
	m_pronounPossessive = possessive;
	m_pronounReflexive = reflexive;
}

// DLL getters and setters
Player* Player::GetNext() const { return m_next; }
void Player::SetNext(Player* next) { m_next = next; }

Player* Player::GetPrev() const { return m_prev; }
void Player::SetPrev(Player* prev) { m_prev = prev; }


// sort flag
bool Player::GetNeedsSort() const { return m_needsSorting; }
void Player::SetNeedsSort(bool sort) { m_needsSorting = sort; }


// card management 
void Player::AddCard(Card* card) {
	m_playerDeck.push_back(card);
	m_needsSorting = true;  // set needsSorting to true whenever player gets a new card
	m_calledUno = false;	// got more cards, reset (or maintain) non-UNO status
}	

void Player::RemoveCard(Card* card) {
	for (int i = 0; i < (int)m_playerDeck.size(); i++) {
		if (m_playerDeck[i] == card) {
			m_playerDeck.erase(m_playerDeck.begin() + i);
			return;
		}
	}
	// if we get here, something went wrong because the card should have been in the player's deck, but just in case, we can print an error message
	cout << "Error: card not found in player's deck when trying to remove card." << endl;
}


// sorting cards - separates unplayed cards (front) from played cards (back)
void Player::SortCards() {
	vector<Card*> unplayedCards;
	vector<Card*> playedCards;

	for (int i = 0; i < (int)m_playerDeck.size(); i++){
		if (m_playerDeck[i]->GetHasBeenPlayed()) playedCards.push_back(m_playerDeck[i]);

		else unplayedCards.push_back(m_playerDeck[i]);
	}

	// rebuild m_playerDeck with unplayed cards first, then played cards
	m_playerDeck.clear();
	for (int i = 0; i < (int)unplayedCards.size(); i++) m_playerDeck.push_back(unplayedCards[i]);
	
	for (int i = 0; i < (int)playedCards.size(); i++) m_playerDeck.push_back(playedCards[i]);


	// after sorting, set needsSorting back to false
	m_needsSorting = false;	
}


// seach for a card in player's deck
Card* Player::GetCardAt(int index) const {
	if (index < 0 || index >= (int)m_playerDeck.size()) return nullptr;
	return m_playerDeck[index];
}



// DISPLAY
void Player::DisplayCards() const {
	cout << "\nYour current deck is:\n" << endl;
	cout << "	";

	// sort first if deck has been updated since last display
	if (m_needsSorting) {
		const_cast<Player*>(this)->SortCards();  // const_cast is needed because DisplayCards is a const function, but SortCards modifies the player's deck and needs to be called before displaying
	}

	// duplicate check variables
	bool firstPrinted = false;  // flag to track if we've printed any cards 
	bool anyPlayed = false;  // flag to track if there are any played cards, used for formatting with " | " separator between unplayed and played cards



	// where all da magic happens
	for (int i = 0; i < (int)m_playerDeck.size(); i++) {
		// skip if this card was already counted in a previous iteration of the loop (for duplicates)
		bool alreadyCounted = false;

		// check how many of the same cards there are before the current card (will skip on the first card)
		for (int j = 0; j < i; j++) {
			if (*m_playerDeck[j] == *m_playerDeck[i]) {
				alreadyCounted = true;
				break; // if we find a previous card with the same color and value, this means we've already printed this card with the correct count
					   // exit J-loop
			}
		}

		// if this card is a duplicate of a previous card, skip to next I-loop iteration
		if (alreadyCounted) continue;

		// if card not already counted, count how many duplicates of this card there are
		int duplicateCount = 0;
		for (int k = i; k < (int)m_playerDeck.size(); k++) {
			if (*m_playerDeck[k] == *m_playerDeck[i]) {
				duplicateCount++;
			}
		}

		// separator between unplayed and played cards
		if (firstPrinted) cout << " | ";
		firstPrinted = true;

		// print the card with the correct color and count
		string color = m_playerDeck[i]->GetColor();
		string val = m_playerDeck[i]->GetCardVal();
		bool isSpecial = (val == "skip") || (val == "reverse") || (val == "+2");  
		bool hasPlayed = m_playerDeck[i]->GetHasBeenPlayed();


		// PLAYED CARDS
		if (hasPlayed) {
			// grey out played cards regardless of the type
			anyPlayed = true;
			cout << PLAYED;
			// special cards 
			if (color == "TBD") cout << "*" << (val == "+4" ? "+4 Card" : "Wild Card") << "*";
			// number cards and action cards
			else cout << "*" << color << " " << val << "*";
			cout << RESET;
		}

		// WILD CARDS (WILD AND +4)
		else if (color == "TBD") {
			// wild and +4 cards will be printed in bold orange to make them stand out more from number cards
			cout << BOLD_WILD << (val == "+4" ? "+4 Card" : "Wild Card") 
				 << RESET;
		}

		// ACTION CARDS (+2, REVERSE, SKIP)
		else if (isSpecial) {
			// colored special cards (skip, reverse, +2) to make them stand out more from number cards
			cout << cardColor(color, true) << color << " " << val << RESET;
		}

		// NUMBER CARDS
		else {
			// number cards will be printed in their respective colors
			cout << cardColor(color) << color << " " << val << RESET;
		}

		if (duplicateCount > 1) cout << " (x" << duplicateCount << ")";  // print count if there are duplicates
	}

	cout << endl;
	if (anyPlayed) cout << PLAYED << "*grey = cards already played*" << RESET << endl;  // legend for what grey cards mean
	cout << endl;
}



// CARDS LEFT
int Player::PlayerCardsLeft() const {
	int count = 0;
	// cards left = number of unplayed cards
	for (int i = 0; i < (int)m_playerDeck.size(); i++) {
		if (!m_playerDeck[i]->GetHasBeenPlayed()) count++;
	}
	return count;
}



// UNO CALL HANDLING
bool Player::GetCalledUno() const {
	return m_calledUno;
}

void Player::SetCalledUno(bool unoStatus) { m_calledUno = unoStatus; }