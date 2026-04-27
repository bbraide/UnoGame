# 🎴 C++ UNO
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
<table>
  <tr>
    <!-- image 2 -->
    <td align="center">
      <img src="screencaps/welcome_screen.png" alt="Game Setup" width="350>
      <i>Welcome + Player Setup</i>
    </td>
    <!-- image 3 -->
    <td align="center">
      <img src="screencaps/game_start_set_order.png" alt="Player Shuffling" width="350">
      <i>Inky shuffles - lavender highlight shows who goes first</i>
    </td>
  </tr>
</table>
