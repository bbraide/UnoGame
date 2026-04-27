#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Game.h"
// this is a big one chat

// =============================================================================================
// CONSTRUCTOR + DESTRUCTOR
// =============================================================================================

// constructor
Game::Game(string filename)
	: m_numPlayers(0), m_filename(filename), m_mainPlayer(nullptr),
	  m_namesGiven(false), m_gameOver(false)
{
	PopulateCards(filename);
}

// destructor!!!!
Game::~Game() {
	m_lineup.Clear();  // unhook DLL pointers first

	// delete players
	for (int i = 0; i < (int)m_players.size(); i++) delete m_players[i];
	m_players.clear();

	// delete cards
	for (int i = 0; i < (int)m_deckOfCards.size(); i++) delete m_deckOfCards[i];
	m_deckOfCards.clear();
	m_drawPile.clear();
	m_cardStack.clear();
	m_mainPlayer = nullptr;
}



// =============================================================================================
// POPULATE CARDS
// =============================================================================================

// file loader
void Game::PopulateCards(string filename) {
	ifstream inFile(filename);
	if (!inFile.is_open()) {
		cout << "ERROR: could not open " << filename << endl;
		return;
	}

	// parse comma-separated fields: color,type,value,isInDeck,hasBeenPlayed
	// ex. red,number,7,false,false
	//	   TBD,special,wild,false,false
	// type is local only - used to pick the right subclass, never stored on the Card
	const char DELIM = ',';
	string color, type, val, isInDeckStr, hasBeenPlayedStr;

	while (getline(inFile, color, DELIM) &&
		getline(inFile, type, DELIM) &&
		getline(inFile, val, DELIM) &&
		getline(inFile, isInDeckStr, DELIM) &&
		getline(inFile, hasBeenPlayedStr, '\n')) {

		// construct the right subclass based on type and value
		Card* newCard = nullptr;
		if (type == "number") newCard = new NumberCard(color, val, true, false);
		else if (val == "wild" || val == "+4") newCard = new WildCard(val, true, false);
		else newCard = new ActionCard(color, val, true, false);

		m_deckOfCards.push_back(newCard);
	}

	inFile.close();
	cout << "Loaded " << (int)m_deckOfCards.size() << " cards." << endl;
}



// =============================================================================================
// WELCOME MENU
// =============================================================================================

void Game::WelcomeMenu() {
	// prompt for player name
	string name;
	cout << BOLD << "Welcome to C++ UNO!" << RESET
		 << "\nWhat's your name? ";
	getline(cin, name);

	// prompt for number of players (2-4)
	int numPlayers = 0;
	cout << "Nice to meet you, " << name << "! How many people (including you) are playing today? (2-4 players) ";
	do {
		cin >> numPlayers;
		cin.ignore();

		if (numPlayers > MAX_PLAYERS) cout << "Woah, too many players! Try a smaller number: ";
		else if (numPlayers == 1) cout << "Playing with yourself only? That's a little lonely. Try a bigger number: ";
		else if (numPlayers == 0) cout << "Zero?? How are you here? Try a bigger number: ";
		else if (numPlayers < 0) cout << "?????????? Number must be positive. Try a bigger number: ";

	} while (numPlayers < MIN_PLAYERS || numPlayers > MAX_PLAYERS);

	PopulatePlayers(numPlayers);
	m_mainPlayer->SetName(name);	// set human player's actual name

	//=========================================================================

	// build welcome message listing other players 
	// "Player 1 and Player 2 are waiting for you" or just "Player 1 is waiting for you"
	string waitingMsg = "";
	for (int i = 1; i < (int)m_players.size(); i++) {
		// more than human + another CPU
		if (i > 1) waitingMsg += (i == (int)m_players.size() - 1) ? " and " : ", ";
		waitingMsg += m_players[i]->GetName();
	}

	/////////////

	int choice = 0;
	string singular = " is ";
	string plural = " are ";
	bool gameStarted = false;

	while (!gameStarted) {
		cout << EVENT_SEPARATOR << endl;
		cout << "Hey " << name << "! " << waitingMsg 
			 << ((numPlayers == 2) ? singular : plural) 
			 << "waiting for you. What would you like to do?" << endl;
		cout << "1. Rules" << endl;
		cout << "2. Give Players Names" << endl;
		cout << "3. Start Game" << endl;
		cout << "4. I Don't Feel Like Playing UNO Anymore" << endl;
		cin >> choice;
		cin.ignore();

		switch (choice) {
		case 1:
			DisplayRules();
			break;
		case 2:
			if (m_namesGiven) cout << "Names already given to the players. Changing them again would be too much paperwork." << endl;
			else {
				SetPlayerNames(numPlayers);
				// rebuild waiting message with updated names
				waitingMsg = "";
				for (int i = 1; i < (int)m_players.size(); i++) {
					// more than human + another CPU
					if (i > 1) waitingMsg += (i == (int)m_players.size() - 1) ? " and " : ", ";
					waitingMsg += m_players[i]->GetName();
				}
			}
			break;
		case 3:
			gameStarted = true;
			break;
		case 4:
			cout << "Are you sure? I'll tell the others. Come again soon!" << endl;
			return;
		default:
			cout << "Invalid option." << endl;
		}
	}

	cout << "\nLet's begin!" << endl;
	ShuffleCards();
	MainMenu();
}



// =============================================================================================
// DISPLAY RULES
// =============================================================================================

void Game::DisplayRules() {
    cout << "\n//= RULES =//\n" << endl;
    cout << "Each player starts out with 7 cards. There will be a card pile to draw from" << endl;
    cout << "and another pile to place cards. When placing cards into the pile:" << endl;
    cout << "  * If drawing based on color, the color MUST match the card on top of the stack." << endl;
    cout << "    Number or action can be different." << endl;
    cout << "  * If drawing based on number, the number MUST match the card on top of the stack." << endl;
    cout << "    Color can be different." << endl;
    cout << "  * Number and color can both match. Nothing special happens." << endl;
    cout << "  * Players can place multiple cards of the same number. (COMING SOON)" << endl;
    cout << "    (e.g. Red 7 and Yellow 7 is valid, but Blue 9 and Blue Skip is not)" << endl;
    cout << "  * Players can place multiple cards of the same action, EXCEPT Wild. (COMING SOON)" << endl;
    cout << "    (e.g. 2 +4s or 3 Reverses are valid, but a Reverse and a Skip are not)" << endl;
    cout << "  * When you draw from the pile, you can place it immediately if it's valid." << endl;
    cout << "  * Whoever reaches 0 cards first wins!" << endl;
}



// =============================================================================================
// POPULATE PLAYERS
// =============================================================================================

void Game::PopulatePlayers(int numPlayers) {
	m_numPlayers = numPlayers;

	// first player (index 0) is always the human
	// AI players start with default names (Player 1, Player 2, etc.)
	// use "You" as placeholder for human player. will properly set name in WelcomeMenu later
	for (int i = 0; i < numPlayers; i++) {
		string name = (i == 0) ? "You" : "Player " + to_string(i);
		Player* p = new Player(name);
		m_players.push_back(p);
	}

	m_mainPlayer = m_players[0];
}



// =============================================================================================
// SET PLAYER NAMES
// =============================================================================================

void Game::SetPlayerNames(int numPlayers) {
	// already been here
	if (m_namesGiven) return;

	// only name the AI players (human named themselves at start)
	for (int i = 1; i < (int)m_players.size(); i++) {
		string name;
		cout << "Enter Player " << i << "'s name: ";
		getline(cin, name);
		m_players[i]->SetName(name);
	}
	cout << "Names have been updated." << endl;
	m_namesGiven = true;
}



// =============================================================================================
// SHUFFLE CARDS + DEAL; RESHUFFLING
// =============================================================================================
void Game::ShuffleCards() {
	// seed random engine
	default_random_engine rng((unsigned int)time(0));

	// randomly pick who shuffles (and therefore goes first)
	int shufflerIdx = rand() % m_numPlayers;
	Player* shuffler = m_players[shufflerIdx];
	cout << "\n-> " << shuffler->GetName() << " chose to shuffle the cards." << endl;

	// number of cards to pull for dealing = numPlayers*7
	int cardsToDeal = m_numPlayers * NUM_STARTING_HAND;
	cout << "-> " << shuffler->GetName() << " takes " << cardsToDeal
		<< " cards from the pile and shuffles them." << endl;

	// shuffle the entire deck first
	shuffle(m_deckOfCards.begin(), m_deckOfCards.end(), rng);

	// deal NUM_STARTING_HAND to each player (ACTUALLY)
	// deal in round-robin order, starting from the player AFTER the shuffler
	int dealtCount = 0;
	for (int round = 0; round < NUM_STARTING_HAND; round++) {
		for (int i = 0; i < m_numPlayers; i++) {
			// skips the dealer and starts with the next person
			int playerIdx = (shufflerIdx + 1 + i) % m_numPlayers;
			Card* card = m_deckOfCards[dealtCount++];
			card->SetInDeck(false);
			m_players[playerIdx]->AddCard(card);
		}
	}

	// print deal messages
	for (int i = 0; i < m_numPlayers; i++) {
		int playerIdx = (shufflerIdx + 1 + i) % m_numPlayers;
		Player* recipient = m_players[playerIdx];
		
		// human is the dealer
		if (recipient == m_mainPlayer && m_mainPlayer == shuffler) cout << "-> You give yourself " << recipient ->PlayerCardsLeft() << " cards." << endl;
		// dealer gives human cards
		else if (recipient == m_mainPlayer) cout << "-> " << shuffler->GetName() << " hands you " << recipient->PlayerCardsLeft() << " cards." << endl;
		// dealer gives themselves cards 
		else if (recipient == shuffler) cout << "-> " << shuffler->GetName() << " gives themselves " << recipient->PlayerCardsLeft() << " cards." << endl;
		// dealer giving other players cards
		else cout << "-> " << shuffler->GetName() << " hands " << recipient->GetName() << " " << recipient->PlayerCardsLeft() << " cards." << endl;
	}

	// remaining cards go into the draw pile (shuffle them as well!)
	for (int i = dealtCount; i < (int)m_deckOfCards.size(); i++) m_drawPile.push_back(m_deckOfCards[i]);

	shuffle(m_drawPile.begin(), m_drawPile.end(), rng);
	cout << "-> The rest of the cards have been shuffled and placed into the draw pile." << endl;

	// flip the first non-wild card from the draw pile as the starting card
	for (int i = 0; i < (int)m_drawPile.size(); i++) {
		if (m_drawPile[i]->GetColor() != "TBD") {
			m_cardStack.insert(m_cardStack.begin(), m_drawPile[i]);
			m_drawPile.erase(m_drawPile.begin() + i);
			break;
		}
	}
	cout << "-> A starting card has been placed." << endl;

	// build the lineup starting from the shuffler
	SetOrder(shuffler);

	// print turn order
	cout << "\nSince " << shuffler->GetName()
		<< " shuffled the cards, they go first. The order is:\n" << endl;
	cout << "    ";
	m_lineup.DisplayLineup(m_mainPlayer->GetName());
	cout << endl;
}

//////////////

void Game::ReshuffleDiscardPile() {
	if (m_cardStack.size() <= 1) return; // nothing to reshuffle

	cout << WARNING << "-> Draw pile empty! Reshuffling discard pile..."
		<< RESET << endl;
	Sleep(5000);	// 5 second delay for shuffling

	// keep the top card; it's still in play
	Card* topCard = m_cardStack.front();

	// move everything else from m_cardStack back to m_drawPile
	for (int i = 1; i < (int)m_cardStack.size(); i++) {
		m_cardStack[i]->SetHasPlayed(false);	// reset play status
		m_drawPile.push_back(m_cardStack[i]);
	}

	// clear the cardStack but keep the top card
	m_cardStack.clear();
	m_cardStack.push_back(topCard);

	// shuffle the repopulated draw pile
	default_random_engine rng((unsigned int)time(0));
	shuffle(m_drawPile.begin(), m_drawPile.end(), rng);

	cout << WARNING << "-> " << m_drawPile.size()
		<< " cards reshuffled into draw pile!" << RESET << endl;
}


// =============================================================================================
// SET ORDER
// =============================================================================================

void Game::SetOrder(Player* firstPlayer) {
	// add players to the lineup starting from firstPlayer (the one who dealt the cards)
	// should wrap around
	int startIdx = 0;
	// find the firstPlayer/shuffler in the vector and save their index position
	for (int i = 0; i < (int)m_players.size(); i++) {
		if (m_players[i] == firstPlayer) { startIdx = i; break; }
	}
	for (int i = 0; i < (int)m_players.size(); i++) {
		m_lineup.AddPlayer(m_players[(startIdx + i) % (int)m_players.size()]);
	}
}



// =============================================================================================
// DISPLAY TOP CARD
// =============================================================================================

void Game::DisplayTopCard() const {
	// gather all the top card intel and such
	Card* top = GetTopCard();
	string color = top->GetColor();
	string val = top->GetCardVal();
	bool isSpecial = (val == "skip" || val == "reverse" || val == "+2" || val == "wild" || val == "+4");

	cout << "The top card is ";
	if (val == "wild" || val == "+4") {
		// wild/+4 always has a chosen color by the time it's on the stack
		string label = (val == "+4") ? "+4 Card" : "Wild Card";
		cout << BOLD_WILD << label << RESET
			<< " (" << cardColor(color) << color << RESET << ")";
	}
	else {
		cout << cardColor(color, isSpecial)
			<< color << " " << val << RESET;
	}
	cout << "." << endl;
}



// =============================================================================================
// IS VALID PLAY
// =============================================================================================

bool Game::IsValidPlay(Card* playerCard) {
	// polymorphism: hold up, let me cook
	return playerCard->CanPlayOn(GetTopCard());
}



// =============================================================================================
// CHECK FOR WINNER
// =============================================================================================

bool Game::CheckForWinner() {
	// traverse through lineup
	Player* curr = m_lineup.GetHead();

	for (int i = 0; i < m_lineup.Size(); i++) {
		int cardsLeft = curr->PlayerCardsLeft();

		// somebody won the game oooooooohh
		if (cardsLeft == 0) {
			// YOU won?!
			if (curr == m_mainPlayer) {
				cout << BOLD_TURN << "\nYOU WON!!! You played all your cards. This one is for the history books!!" << RESET << endl;
			}
			// other player won </3
			else {
				cout << WARNING << "\n" << curr->GetName()
					<< " has no cards left. " << curr->GetName()
					<< " wins! Better luck next time." << RESET << endl;
			}
			m_gameOver = true;
			return true;
		}

		// someone will call UNO
		if (cardsLeft == 1 && !curr->GetCalledUno()) {
			HandleAiUno(curr); // handles both human and AI
			curr->SetCalledUno(true);
		}
		// walk to next node
		curr = curr->GetNext();
	}
	return false;
}



// =============================================================================================
// HANDLE UNO
// =============================================================================================

void Game::HandleAiUno(Player* player) {
	if (player == m_mainPlayer) HandleHumanUno();
	else {
		cout << WARNING << "\n" << player->GetName()
			<< " just called UNO! They feel a bunch of targets down their back."
			<< RESET << endl;
	}
}

///////////////////////////////////////

void Game::HandleHumanUno() {
	int choice = 0;
	do {
		cout << BOLD_TURN << "\nYou have one card left! This is your chance to call UNO!!" << RESET << endl;
		cout << "1. Call it!" << endl;
		cout << "2. Nah" << endl;

		cin >> choice;
		cin.ignore();

		switch (choice) {
		case 1:
			cout << BOLD_TURN << "You hollered \"UNO!!!\" You wonder why everyone is looking at you so cunningly." << RESET << endl;
			break;

		case 2:
			// caught not calling UNO, so another player calls it before human
			cout << WARNING << "Someone else called UNO before you. 2 cards added to your deck for your fumble!" << RESET << endl;

			for (int i = 0; i < 2; i++) {
				if (m_drawPile.empty()) ReshuffleDiscardPile();
				if (!m_drawPile.empty()) {
					m_mainPlayer->AddCard(m_drawPile.front());
					m_drawPile.erase(m_drawPile.begin());
				}
			}
			break;

		default:
			cout << "Invalid option! 1 or 2, this is a life or death situation." << endl;
		}
	} while (choice != 1 && choice != 2);
}



// =============================================================================================
// COLOR HELPERS
// =============================================================================================

string Game::ColorToString(CardColor color) const {
	switch (color) {
		case CardColor::Red:		return "red";
		case CardColor::Yellow:		return "yellow";
		case CardColor::Green:		return "green";
		case CardColor::Blue:		return "blue";
		default:					return "TBD";
	}
}

CardColor Game::StringToColor(const string& color) const {
	if (color == "red")    return CardColor::Red;
	if (color == "yellow") return CardColor::Yellow;
	if (color == "green")  return CardColor::Green;
	if (color == "blue")   return CardColor::Blue;
	return CardColor::TBD;
}



// =============================================================================================
// CHOOSE COLOR (WILD/+4)
// =============================================================================================

void Game::ChooseColor(WildCard* card) {
	cout << "Choose a color:" << endl;
	cout << RED << "1. Red" << RESET << endl;
	cout << YELLOW << "2. Yellow" << RESET << endl;
	cout << GREEN << "3. Green" << RESET << endl;
	cout << BLUE << "4. Blue" << RESET << endl;

	int choice = 0;
	CardColor chosen = CardColor::TBD;

	do {
		cin >> choice;
		cin.ignore();

		switch (choice) {
			case 1: chosen = CardColor::Red;	break;
			case 2: chosen = CardColor::Yellow;	break;
			case 3: chosen = CardColor::Green;	break;
			case 4: chosen = CardColor::Blue;	break;
			default: cout << "Invalid choice. Choose 1-4! ";
		}
	} while (chosen == CardColor::TBD);

	string colorStr = ColorToString(chosen);
	card->SetColor(colorStr);
	cout << "Color changed to "
		<< cardColor(colorStr) << colorStr << RESET << "!" << endl;

}



// =============================================================================================
// APPLY CARD EFFECT
// =============================================================================================
void Game::ApplyCardEffect(Card* card) {
	string val = card->GetCardVal();

	// if not a special value, nothing to do
	if (val != "skip" && val != "reverse" &&
		val != "+2" && val != "+4") return;

	// skip a player
	if (val == "skip") {
		Player* skippedP = m_lineup.PeekNextPlayer();
		cout << WARNING << "-> " << skippedP->GetName() << " is skipped!" << RESET << endl;
		m_lineup.Skip();
	}

	// reverse the flow
	else if (val == "reverse") {
		m_lineup.Reverse();
		cout << WARNING << "-> Play direction reversed!" << RESET << endl;
		// in a 2-player game, reverse acts like a skip
	}

	// +2 somebody
	else if (val == "+2") {
		Player* target = m_lineup.PeekNextPlayer();
		cout << (target == m_mainPlayer ? BOLD : "")
			<< WARNING << "-> " << target->GetName()
			<< " draws 2 cards! o_o" << RESET << endl;
		for (int i = 0; i < 2; i++) {
			// TODO v1.1: reshuffle m_cardStack into m_drawPile if it runs out mid-penalty
			if (m_drawPile.empty()) ReshuffleDiscardPile();
			if (!m_drawPile.empty()) {
				target->AddCard(m_drawPile.front());
				m_drawPile.erase(m_drawPile.begin());
			}
		}
		m_lineup.Skip();	// also skips the victim's turn
	}

	// +4 with her head
	else if (val == "+4") {
		Player* victim = m_lineup.PeekNextPlayer();
		cout << (victim == m_mainPlayer ? BOLD : "")
			<< WARNING << "-> " << victim->GetName()
			<< " draws 4 cards!! O_O" << RESET << endl;
		for (int i = 0; i < 4; i++) {
			// TODO v1.1: reshuffle m_cardStack into m_drawPile if it runs out mid-penalty
			if (m_drawPile.empty()) ReshuffleDiscardPile();
			if (!m_drawPile.empty()) {
				victim->AddCard(m_drawPile.front());
				m_drawPile.erase(m_drawPile.begin());
			}
		}
		m_lineup.Skip();	// also skips the victim's turn
	}
}


// =============================================================================================
// AI THINKING DELAY
// =============================================================================================
// Sleep() takes milliseconds
// so 3-7 seconds = 3000-7000 milliseconds
void Game::AiThinkingDelay() {
	int ms = (rand() % 5000) + 3000;  // random between 3000-7000ms
	Sleep(ms);
}



// =============================================================================================
// AI COLOR CHOOSER HELPER
// =============================================================================================
void Game::AiChooseColor(WildCard* wild, Player* player) {
	// if wild or +4, AI picks a random color
	CardColor colorOptions[] = { CardColor::Red, CardColor::Yellow, CardColor::Green, CardColor::Blue };
	string colorPick = ColorToString(colorOptions[rand() % NUM_COLORS]);
	wild->SetColor(colorPick);
	cout << WARNING << "-> " << player->GetName()
		<< " changes the color to " << cardColor(colorPick)
		<< colorPick << RESET << "!" << endl;
}



// =============================================================================================
// AI PLAYER TURN
// =============================================================================================

void Game::PlayerTurn(Player* currPlayer) {
	cout << EVENT_SEPARATOR << endl;
	cout << "It's " << LAVENDER << currPlayer->GetName() << RESET << "'s turn!" << endl;
	DisplayTopCard();
	cout << currPlayer->GetName() << " is thinking..." << endl;
	AiThinkingDelay();


	// simple algorithm: find the first valid card in AI's hand (if exists) and play it into pile
	Card* chosen = nullptr;
	for (int i = 0; i < currPlayer->PlayerCardsLeft(); i++) {
		Card* c = currPlayer->GetCardAt(i);
		if (c && IsValidPlay(c)) {
			chosen = c;
			break;
		}
	}

	if (chosen) {
		// AI plays the card
		chosen->SetHasPlayed(true);
		m_cardStack.insert(m_cardStack.begin(), chosen);
		currPlayer->RemoveCard(chosen);
		cout << "-> " << currPlayer->GetName() << " places their card into the pile!" << endl;
		DisplayTopCard();

		// if wild or +4, AI picks a random color
		WildCard* wild = dynamic_cast<WildCard*>(chosen);
		if (wild) AiChooseColor(wild, currPlayer);
		ApplyCardEffect(chosen);
	}

	else {
		// no valid card -> draw from pile
		if (m_drawPile.empty()) ReshuffleDiscardPile();
		if (!m_drawPile.empty()) {
			Card* drawn = m_drawPile.front();
			m_drawPile.erase(m_drawPile.begin());
			currPlayer->AddCard(drawn);
			cout << "-> " << currPlayer->GetName() << " takes a card from the draw pile." << endl;

			// check if drawn card can be played immediately
			if (IsValidPlay(drawn)) {
				drawn->SetHasPlayed(true);
				m_cardStack.insert(m_cardStack.begin(), drawn);
				currPlayer->RemoveCard(drawn);
				cout << "-> " << currPlayer->GetName() << " plays the drawn card!" << endl;
				DisplayTopCard();

				// if wild or +4, AI picks a random color
				WildCard* wild = dynamic_cast<WildCard*>(drawn);
				if (wild) AiChooseColor(wild, currPlayer);
				ApplyCardEffect(drawn);
			}
			else cout << "-> " << currPlayer->GetName() << " keeps their card." << endl;
		}
	}
}



// =============================================================================================
// HUMAN TURN
// =============================================================================================

void Game::HumanTurn() {
	cout << EVENT_SEPARATOR << endl;
	cout << BOLD_TURN << "It's your turn!" << RESET << endl;
	
	int choice = 0;
	bool turnOver = false;

	do {
		DisplayTopCard();
		// human player menu
		cout << "\nWhat's your move?" << endl;
		cout << "1. View my cards" << endl;
		cout << "2. See player order" << endl;
		cout << "3. Place cards" << endl;
		cout << "4. Draw from pile" << endl;
		cout << "5. Rules" << endl;
		cout << "6. Quit" << endl;
		cin >> choice;
		cin.ignore();

		switch (choice) {
			case 1:
				m_mainPlayer->DisplayCards();
				break;


			case 2:
				cout << "\nThe current order is:\n" << endl;
				cout << "	";
				m_lineup.DisplayLineup(m_mainPlayer->GetName());
				cout << endl;
				break;


			case 3: {
				// show hand and prompt for card to play
				m_mainPlayer->DisplayCards();
				DisplayTopCard();
				int cardType = 0;
				bool cardPlayed = false;

				do {
					cout << "\nWhat type of card are you playing?" << endl;
					cout << "1. Number card" << endl;
					cout << "2. Special card" << endl;
					cout << "3. Back" << endl;
					cin >> cardType;
					cin.ignore();

					switch (cardType) {
					case 1: {
						// number card
						string color; int val;
						cout << "Enter the color (red, yellow, green, blue): ";
						getline(cin, color);
						cout << "Enter the value (0-9, or -1 to go back): ";
						cin >> val;
						cin.ignore();
						if (val == -1) break;

						// search player's hand for the card they described
						Card* toPlay = nullptr;
						for (int i = 0; i < m_mainPlayer->PlayerCardsLeft(); i++) {
							Card* c = m_mainPlayer->GetCardAt(i);
							if (c && c->GetColor() == color && c->GetCardVal() == to_string(val)) {
								toPlay = c;
								break;
							}
						}

						if (!toPlay) {
							cout << "You don't have that card." << endl;
							break;
						}

						if (!IsValidPlay(toPlay)) {
							cout << "That card can't be played on the current top card." << endl;
							break;
						}

						// OTHERWISE.....
						toPlay->SetHasPlayed(true);
						m_cardStack.insert(m_cardStack.begin(), toPlay);
						m_mainPlayer->RemoveCard(toPlay);
						cout << "\n -> You placed your card into the pile!" << endl;
						DisplayTopCard();
						ApplyCardEffect(toPlay);
						turnOver = true;
						cardPlayed = true;
						break;
					}


					case 2: {
						// special card
						string color, val;
						cout << "Enter the color (red, yellow, green, blue, or TBD for wild): ";
						getline(cin, color);
						cout << "Enter the action (+2, reverse, skip, wild, +4): ";
						getline(cin, val);

						// search player's hand for the card they described
						Card* toPlay = nullptr;
						for (int i = 0; i < m_mainPlayer->PlayerCardsLeft(); i++) {
							Card* c = m_mainPlayer->GetCardAt(i);
							if (c && c->GetColor() == color && c->GetCardVal() == val) {
								toPlay = c;
								break;
							}
						}

						if (!toPlay) { cout << "You don't have that card." << endl; break; }
						if (!IsValidPlay(toPlay)) { cout << "That card can't be played." << endl; break; }

						// otherwiiiiseeeeee
						toPlay->SetHasPlayed(true);
						m_cardStack.insert(m_cardStack.begin(), toPlay);
						m_mainPlayer->RemoveCard(toPlay);

						// if wild or +4, prompt for color
						WildCard* wild = dynamic_cast<WildCard*>(toPlay);
						if (wild) ChooseColor(wild);

						cout << "\n-> You placed your card into the pile!" << endl;
						DisplayTopCard();
						ApplyCardEffect(toPlay);
						turnOver = true;
						cardPlayed = true;
						break;
					}

					case 3:
						cardPlayed = true; // exits while loop and takes you back to main menu
						break;
					default:
						cout << "Invalid choice." << endl;
					}
				} while (!cardPlayed);
				break;
			}

			case 4: {
				// draw from pile
				if (m_drawPile.empty()) ReshuffleDiscardPile();
				if (!m_drawPile.empty()) {
					Card* drawn = m_drawPile.front();
					m_drawPile.erase(m_drawPile.begin());
					m_mainPlayer->AddCard(drawn);

					cout << "\n-> You picked up a ";
					if (drawn->GetColor() == "TBD") {
						cout << BOLD_WILD
							<< (drawn->GetCardVal() == "+4" ? "+4 Card" : "Wild Card")
							<< RESET;
					}
					else {
						cout << cardColor(drawn->GetColor(),
							drawn->GetCardVal() == "skip" ||
							drawn->GetCardVal() == "reverse" ||
							drawn->GetCardVal() == "+2")
							<< drawn->GetColor() << " " << drawn->GetCardVal() << RESET;
					}
					cout << "." << endl;

					if (IsValidPlay(drawn)) {
						cout << "Place card into stack? (Y/N) ";
						char yn;
						cin >> yn;
						cin.ignore();
						if (yn == 'Y' || yn == 'y') {
							drawn->SetHasPlayed(true);
							m_cardStack.insert(m_cardStack.begin(), drawn);
							m_mainPlayer->RemoveCard(drawn);
							WildCard* wild = dynamic_cast<WildCard*>(drawn);
							if (wild) ChooseColor(wild);
							cout << "\n-> You placed your new card into the stack!" << endl;
							DisplayTopCard();
							ApplyCardEffect(drawn);
							turnOver = true;

						}
						else {
							cout << "-> You keep the card." << endl;
							turnOver = true;
						}

					}
					else {
						cout << "-> Card can't be played. Added to your hand." << endl;
						turnOver = true;
					}
					break;
				}
			}

			case 5:
				DisplayRules();
				cout << "\n====\n" << endl;
				break;

				
			case 6: {
				char yn;
				cout << "Are you sure? You haven't finished the game! (Y/N) ";
				cin >> yn;
				cin.ignore();

				if (yn == 'Y' || yn == 'y') {
					m_gameOver = true;
					turnOver = true;
				}
				break;
			}

			default:
				cout << "Invalid choice." << endl;
				

		}

	} while (!turnOver);
}
// whew


// =============================================================================================
// MAIN MENU
// =============================================================================================
void Game::MainMenu() {
	do {
		// check win condition at the end of each turn (after first turn is taken)
		Player* curr = m_lineup.GetCurrentPlayer();

		if (curr == m_mainPlayer) HumanTurn();
		else PlayerTurn(curr);

		// immediately check for winner after this turn
		if (CheckForWinner()) break;

		// advance to next player
		m_lineup.Advance();

	} while (!m_gameOver);

	cout << "\nThanks for playing C++ UNO!" << endl;
}