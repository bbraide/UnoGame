# 🎴 C++ UNO (v1.0)
> A fully playable 2-4 player UNO game in C++ featuring polymorphism, a circular doubly linked list, and a colorful ANSI terminal UI experience!

---

# ⋆｡°✩ ★ FEATURES
- **2-4 player support** - 1 human player vs up to 3 AI opponents
- **Full UNO ruleset** - number cards, action cards (Skip, Reverse, +2), and wild cards (+4, Wild)
- **Rainbow ANSI terminal UI** - every program needs a little color. Every card prints in its own color, special cards in bold, current player highlighted in lavender
- **Circular doubly linked list** for player turn management. Supports Reverse, Skip, and wrap-around naturally
- **AI thinking delay** - 3-7 second randomized pause per AI turn (so the immersion feels real)
- **UNO detection** - calls out players who have 1 card left, and human prompted to either call it or skip it (punished greatly for the latter)
- **Duplicate card grouping** - displays ```Red 8 (x2)``` instead of listing the same card twice
- **Played card stacking** - cards already played appear greyed out in your hand
- **Reshuffle logic** - automatically reshuffles the discard pile into the draw pile if it runs out mid-game

---

# ⋆｡°✩ ★ SCREENSHOTS

<!-- IMAGE 1 -->
<div align="center">
  <p><sub><i>(User input is in pink or yellow!)</i></sub></p>
  
  <img src="screencaps/gameplay_1.png" alt="Gameplay Screenshot" width="400">
  <p><i>Colorful hand display. Special cards are emboldened!</i></p>

</div>

<!-- IMAGE 2 AND 3 -->
<table align="center">
  <tr>
    <!-- image 2 -->
    <td align="center">
      <img src="screencaps/welcome_screen.png" alt="Game Setup" width="450">
      <p><i>Welcome + Player Setup</i></p>
    </td>
    <!-- image 3 -->
    <td align="center">
      <img src="screencaps/game_start_set_order.png" alt="Player Shuffling" width="450">
      <p><i>Inky shuffles - lavender highlight shows who goes first</i></p>
    </td>
  </tr>
</table>

<!-- IMAGE 4 AND 5 -->
<table align="center">
  <tr>
    <!-- image 4 -->
    <td align="center">
      <img src="screencaps/gameplay_2_plus4.png" alt="Player gets +4'ed by Another player" width="450">
      <p><i>Binky hitting Pinky with a +4</i></p>
    </td>
    <!-- image 5 -->
    <td align="center">
      <img src="screencaps/drawing_card_plus4.png" alt="Playing a +4 Card" width="400">
      <p><i>Drawing a +4 from the pile and unleashing it :3</i></p>
    </td>
  </tr>
</table>

<!-- IMAGE 6 AND 7 -->
<table align="center">
  <tr>
    <!-- image 6 -->
    <td align="center">
      <img src="screencaps/cpu_gets_uno.png" alt="AI Player calls UNO!" width="450">
      <p><i>Time to lock in.</i></p>
    </td>
    <!-- image 7 -->
    <td align="center">
      <img src="screencaps/human_player_gets_uno.png" alt="Human player calls UNO!" width="400">
      <p><i>UNO FOR CLYDE!</i></p>
    </td>
  </tr>
</table>

<!-- IMAGE 8 AND 9 -->
<table align="center">
  <tr>
    <!-- image 8 -->
    <td align="center">
      <img src="screencaps/cpu_wins_game.png" alt="AI Player wins the game" width="450">
      <p><i>We can't all be winners...</i></p>
    </td>
    <!-- image 9 -->
    <td align="center">
      <img src="screencaps/human_player_wins.png" alt="Human player wins the game!" width="400">
      <p><i>NEVERMIND!!</i></p>
    </td>
  </tr>
</table>

<!-- IMAGE 10 -->
<div align="center">
  <img src="screencaps/display_rules.png" alt="Rule Display" width="400">
  <p><i>Full rules display accessible at any time during the game</i></p>
</div>

---

<hr>

# ⋆｡°✩ ★ TECH STACK AND SKILLS DEMONSTRATED

| Concept | Implementation |
|---|---|
|**Abstract classes + polymorphism**|`Card` is an abstract base class; `NumberCard`, `ActionCard`, and `WildCard` each override `CanPlayOn()`|
|**Circular doubly linked list**|`Lineup` manages player turn order with wrap-around, direction reversal, and skip logic|
|**Dynamic memory management**|`Game` owns all `Card*` and `Player*` objects; safe destructor chain with no memory leaks|
|**File I/O parsing**|`uno_deck.txt` parsed with `getline()` and delimiter splitting to construct the right subclass per card|
|**Operator overloading**|`Card::operator==` compares cards by color and value for duplicate detection|
|`enum class`|`CardColor` scoped enum for type-safe color selection in wild card logic|
|**ANSI terminal UI**|RGB codes via `Colors.h` - fully themeable card display system|
|**Separation of concerns**|`Card` describes itself, `Player` manages a hand, `Lineup` manages order, `Game` runs everything|

---

<hr>

# ⋆｡°✩ ★ CARD HIERARCHY

```
Card  (abstract base — CanPlayOn() is pure virtual)
├── NumberCard    (colors: red/yellow/green/blue, values: 0-9)
├── ActionCard    (Skip, Reverse, +2 — colored)
└── WildCard      (+4, Wild — color chosen at play time)
```

---

<hr>

# ⋆｡°✩ ★ HOW TO BUILD AND RUN

## Requirements
- Windows (uses `windows.h` for ANSI color support)
- Visual Studio 2022 or later
- C++ 20 or later


## Steps 
### 1. **Clone the repo**
``` bash
git clone https://github.com/bbraide/UnoGame.git
cd UnoGame
```

### 2. **Open in Visual Studio**
- Open `uno-cpp/UnoGame/UnoGame.sln` in Visual Studio 2022 (or later)

### 3. **Check the deck file location**
- Visual Studio looks for files relative to the project folder. Ensure `uno_deck.txt` is inside:
  `uno-cpp/UnoGame/UnoGame/uno_deck.txt`
- *Note: If it's missing, copy the one from the root folder into that directory!*

### 4. **Set C++ Standard to C++20**
- Right-click project → **Properties** → **Configuration Properties** → **General** → **C++ Language Standard** → `ISO C++20 Standard (/std:c++20)`

### 5. **Build & Run**
- Hit `Ctrl+F5` and end some friendships!

> [!NOTE] 
> ANSI colors require Windows 10 or later. The game automatically enables virtual terminal processing on startup.


---

<hr>

# ⋆｡°✩ ★ GAMEPLAY

```
Welcome to C++ UNO! What's your name? Clyde
Nice to meet you, Clyde! How many people (including you) are playing today? (2-4 players) 4

~*-*~

Hey Clyde! Player 1, Player 2 and Player 3 are waiting for you. What would you like to do?
1. Rules
2. Give Players Names
3. Start Game
4. I Don't Feel Like Playing UNO Anymore
```

---

<hr>

# ⋆｡°✩ ★ YOUR TURN MENU

```
What's your move?
1. View my cards
2. See player order
3. Place cards
4. Draw from pile
5. Rules
6. Quit
```

***Choosing "Nah" when you have one card left will cost you two cards, so choose carefully 👀***

---

<hr>

# ⋆｡°✩ ★ HOW TO PLAY

The game is fully interactive and will prompt you whenever input is needed.
- **Place Cards** (option ***3***): Enter the __<ins>card type</ins>__, __<ins>color</ins>__, and __<ins>number</ins>__ (in three separate steps) corresponding to the card you want to play.
- **Draw from pile** (option ***4***): If you have no playable cards, you can choose to draw a card.
- **Wilds**: Follow the on-screen prompt to choose a new color.

> [!WARNING]
> **Input Stability**: Currently, the game expects the correct data types (integers for selection, etc.). Entering letters when a number is expected may cause the game to crash. Input validation is a work-in-progress for v1.1!

---

<hr>

<details>
  <summary>⋆｡°✩ ★ FULL UNO RULES (click to expand)</summary>
  <br>
  
- Match the top card by **color** OR **number/action**
- **Skip** — next player loses their turn
- **Reverse** — play direction flips (acts as Skip in 2-player)
- **+2** — next player draws 2 cards and loses their turn
- **Wild** — play anytime, choose any color
- **+4** — play anytime, next player draws 4 cards and loses their turn, then choose any color
- When you draw from the pile, you may immediately play it if it's valid
- If the draw pile runs out, the discard pile is automatically reshuffled
- First player to **0 cards** wins!
  
</details>

---

<hr>

# ⋆｡°✩ ★ PROJECT STRUCTURE

```
UnoGame/ (Repo Root)
├── screencaps/             # README screenshots
├── uno_deck.txt            # Deck source file
├── uno_sample.txt          # Sample game output
├── README.md               # YOU ARE HERE
└── uno-cpp/                # Main project container
    ├── src/                # All source and header files
    │   ├── ActionCard.cpp
    │   ├── ActionCard.h
    │   ├── Card.h
    │   ├── Colors.h
    │   ├── Game.cpp
    │   ├── Game.h
    │   ├── Lineup.cpp
    │   ├── Lineup.h
    │   ├── main.cpp
    │   ├── NumberCard.cpp
    │   ├── NumberCard.h
    │   ├── Player.cpp
    │   ├── Player.h
    │   ├── WildCard.cpp
    │   └── WildCard.h
    └── UnoGame/            # Visual Studio solution folder
        ├── UnoGame.sln     # VS Solution file
        └── UnoGame/        # Project folder
            ├── UnoGame.vcxproj
            └── uno_deck.txt
```

---

<hr>

# ⋆｡°✩ ★ ROADMAP

- **v1.1** — inclusive pronoun system for all players (comma-delimited input: `she,her,her` / `they,them,their` / custom neopronouns fully supported), playable card highlighting
- **v1.2** — +2/+4 stacking rules, smarter AI (prefers special cards)
- **v1.3** — tournament mode (first to win 3 rounds), score tracking across games
- **v2.0** — cross-platform port (Linux/Mac), GUI with raylib or SDL

---

<hr>

# ⋆｡°✩ ★ ABOUT 
Started this as a mini passion project in my junior year of college (fall semester) to demonstrate core C++ concepts beyond the classroom. Here we are a year and a half later!

**Connect:**
[GitHub](https://github.com/bbraide) · [@bberetd on Instagram](https://instagram.com/bberetd)
