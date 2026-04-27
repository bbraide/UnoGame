// LINEUP.CPP

#include "Lineup.h"
#include "Colors.h"

Lineup::Lineup() : m_head(nullptr), m_tail(nullptr), m_currentPlayer(nullptr), m_forward(true), m_size(0) {}

Lineup::~Lineup() {}

void Lineup::Clear() {
	// walk exactly m_size steps and null everything out
	Player* curr = m_head;
	for (int i = 0; i < m_size; i++) {
		Player* next = curr->GetNext();
		curr->SetNext(nullptr);
		curr->SetPrev(nullptr);
		curr = next;
	}
	m_head = nullptr;
	m_tail = nullptr;
	m_currentPlayer = nullptr;
	m_size = 0;
}

Player* Lineup::GetHead() const { return m_head; }

void Lineup::AddPlayer(Player* newPlayer) {
	// empty lineup case: new player becomes head, tail, and current player
	if (m_head == nullptr) {
		m_head = newPlayer;
		m_tail = newPlayer;
		newPlayer->SetNext(nullptr);
		newPlayer->SetPrev(nullptr);
	} else {
		newPlayer->SetPrev(m_tail);
		newPlayer->SetNext(nullptr);
		m_tail->SetNext(newPlayer);
		m_tail = newPlayer;
	}
	// make list circular by connecting head and tail so Advance() can wrap around without needing to check for nullptr
	m_tail->SetNext(m_head);
	m_head->SetPrev(m_tail);
	
	if (m_currentPlayer == nullptr) m_currentPlayer = m_head;  // if this is the first player added, set current player to head

	m_size++;
}

Player* Lineup::GetCurrentPlayer() const {
	return m_currentPlayer;
}

Player* Lineup::PeekNextPlayer() const {
	// if lineup is empty, return nullptr
	if (m_currentPlayer == nullptr) return nullptr;

	// if lineup is in forward order, next player is current player's next; if lineup is in reverse order, next player is current player's prev
	if (m_forward) {
		return m_currentPlayer->GetNext();
	} else {
		return m_currentPlayer->GetPrev();
	}
}

void Lineup::Advance() {
	// if lineup is empty, do nothing (can never happen because game won't start if there are no players, but just in case)
	if (m_currentPlayer == nullptr) return;

	// if lineup is in forward order, current player becomes current player's next; if lineup is in reverse order, current player becomes current player's prev
	if (m_forward) {
		m_currentPlayer = m_currentPlayer->GetNext();
	} else {
		m_currentPlayer = m_currentPlayer->GetPrev();
	}
}

// reverses the order of the lineup; simply toggle m_forward boolean
void Lineup::Reverse() {
	m_forward = !m_forward;
}

// skips the next player by advancing twice
void Lineup::Skip() {
	Advance();
}

int Lineup::Size() const {
	return m_size;
}

void Lineup::DisplayLineup(const string& mainPlayerName) const {
	if (m_currentPlayer == nullptr) return;

	// always display in clock order regardless of direction
	// but use arrow to indicate current player and direction of play
	Player* curr = m_head;
	for (int i = 0; i < m_size; i++) {
		// highlight current player in lavender and append " (you)" to main player's name
		bool isCurrent = (curr == m_currentPlayer);
		bool isMain = (curr->GetName() == mainPlayerName);

		if (isCurrent) cout << LAVENDER << BOLD;  // highlight current player in lavender

		if (isMain) cout << curr->GetName() << " (you)";  // append " (you)" to main player's name
		else cout << curr->GetName();

		if (isCurrent) cout << RESET;  // reset color after printing current player's name

		if (i < m_size - 1) {
			if (m_forward) cout << " -> ";  // arrow indicates direction of play
			else cout << " <- ";
		}
		curr = curr->GetNext();
	}
	cout << endl;
}
