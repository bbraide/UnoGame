#ifndef _GAME_H_
#define _GAME_H_

// created classes
#include "Card.h"
#include "NumberCard.h"
#include "ActionCard.h"
#include "WildCard.h"
#include "Player.h"
#include "Lineup.h"
#include "Colors.h"

// other libraries
#include <iostream>
#include <ctime>		// srand(time(0)) for shuffling
#include <fstream>	
#include <string>
#include <vector>
#include <algorithm>	// std::shuffle
#include <random>		// std::default_random_engine

using namespace std;

const int NUM_CARDS = 108;			// size of a stundard UNO deck
const int NUM_STARTING_HAND = 7;	// number of cards each player starts out with
const int NUM_COLORS = 4;			// red, yellow, green, blue
const int MIN_PLAYERS = 2;			// minimum # of players including human
const int MAX_PLAYERS = 4;			// maximum # of players including human
inline string EVENT_SEPARATOR = "\n~*-*~\n";


// scoped enum for the four UNO colors + TBD for unplayed wilds
// using enum class to avoid naming conflicts with Colors.h constants
// ex. CardColor::Red, CardColor::Blue
enum class CardColor { Red, Yellow, Green, Blue, TBD };

class Game
{
	public:
		Game(string filename);  // constructor - loads file + sets up game state
		~Game();  // destructor - free all Card* and Player* objects

		// read in cards from uno_deck.txt and populate m_deckOfCards
		// NumberCard for numbers, ActionCard for skip/reverse/+2, WildCard for wild/+4
		void PopulateCards(string filename);

		// ENTRY POINT
		// when user first enters program, is prompted for name and number of players (2-4 only)
		// user has the option of giving players names
		/* Welcome, [user]! [numPlayers-1] other players are waiting for you. What do you want to do?
		*   1. Rules
			2. Name Players
			3. Start Game
			4. I Don't Feel Like Playing UNO Anymore
		*/
		void WelcomeMenu();

		// prints the rules of UNO
		void DisplayRules();
		
		// depending on the number of players user selects, will create Player objects and store them into m_players
		// who shuffles is randomly selected, and the player who shuffles goes first 
		void PopulatePlayers(int numPlayers);

		// if user chooses to do so, they can give the other players names
		// goes through vector and updates the names
		// CAN ONLY BE CALLED ONCE! m_namesGiven prevents double naming
		void SetPlayerNames(int numPlayers);

		// prompts user to select their pronouns for the game; updates
		// the human player's pronoun fields accordingly
		void SetPlayerPronouns();


		// ====================================================================
		// STARTING THE GAME!!
		// ====================================================================

		// will first shuffle [numPlayers*7] cards from m_deckOfCards, then give out 7 to each player
		// will then shuffle the remaining cards and place into m_drawPile
		// places one starting card face-up onto m_cardStack
		void ShuffleCards();

		// once m_drawPile is empty (cards not in players' hands are all in m_drawStack) reshuffle and repopulate m_drawPile
		void ReshuffleDiscardPile();
		
		// builds m_lineup (DLL) from m_players with firstPlayer at the front
		void SetOrder(Player* firstPlayer);

		// main menu of game; runs till a player wins or user quits
		void MainMenu();

		// after selecting "Place cards" user chooses what card they'll draw, checks if they can make the draw
		// color MUST match (if going based on color), number MUST match (if going based on number)
		// color and number can match 
		// is just playerCard->CanPlayOn(GetTopCard())
		bool IsValidPlay(Card* playerCard);

		// picks a random color for AI wild/+4 cards
		void AiChooseColor(WildCard* wild, Player* player);

		// adds a random 3-7 second delay to simulate AI "thinking"
		void AiThinkingDelay();

		// handles a single player's turn (CPUs play automatically)
		void PlayerTurn(Player *currPlayer);

		// handles the human player's turn (shows menu, takes input)
		/* The top card is a [card type]. (if wild or +4: The color is [color].)
			What's your move?
			1. View my cards
			2. See Player order
			3. Place cards
			4. Draw from pile
			5. Quit
		*/
		void HumanTurn();

		// (NEW v1.2) plays multiple cards in one sequence, applying each effect (if applicable) as they go
		// called after human confirms their full set of cards to play
		void MultipleCardPlay(vector<Card*>& playedCards);

		// applies the effect of a special card after it's been played
		// skip -> Skip(), reverse -> Reverse(), +2/+4 -> deal penalty cards
		void ApplyCardEffect(Card* card);

		// prompts human player to choose color after playing wild or +4
		// calls card->SetColor() with the chosen color
		void ChooseColor(WildCard* card);

		// converts CardColor enum to lowercase string for SetColor() and display
		string ColorToString(CardColor color) const;

		// converts string stored on Card back to CardColor enum for comparisons
		CardColor StringToColor(const string& color) const;

		// returns the currently on top of the m_cardStack
		Card* GetTopCard() const { return m_cardStack.front(); }

		// prints the top card in its own color
		// "The top card is [color] [value]."
		void DisplayTopCard() const;

		/* loops through m_lineup and checks every player's card count
			 - 1 card left -> print UNO call message
					-> if human: pause and prompt to call UNO or suffer the consequences
					-> if CPU: automatically calls UNO
			 - 0 cards -> that player wins, m_gameOver = true
			 * returns true if game is over, false otherwise
			 * called at the end of each player turn	*/
		bool CheckForWinner();

		// prompts human to call UNO when they have one card left
		// if they choose "Nah", two cards are added to their hand
		void HandleHumanUno();

		// called when an AI player hits one card - prints the taunt message
		// "[player] just called UNO! they feel a bunch of targets down their back."
		void HandleAiUno(Player* player);


	private:
		int m_numPlayers;
		string m_filename;

		// all 108 UNO cards (Game owns these)
		vector<Card*> m_deckOfCards;	
		
		// remaining cards that haven't been handed out to; available to be drawn from mid-game
		vector<Card*> m_drawPile;

		// discard pile; top card is m_cardStack.front()
		vector<Card*> m_cardStack;		

		// stores players until order has been established for the game; used before Lineup is built
		vector<Player*> m_players;		

		// DLL of players in active turn order
		Lineup m_lineup;				

		// human player
		Player* m_mainPlayer;  
		bool m_namesGiven;	   // true once SetPlayerNames() has been called
		bool m_gameOver;	   // true when a player reaches 0 cards or user quits

};

#endif