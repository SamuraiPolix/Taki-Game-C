# TAKI Card Game
### Game programmed in C, as the final project in Introduction to Programming course @ MTA College
![Taki_Cards_Game_Logo](https://user-images.githubusercontent.com/104992892/188504607-fa0156de-c1f6-4205-bc23-e20fb591c0e3.png)
  
> The game was made using memory allocation, pointers, sorting algorythems and by using C random function.

# Game Rules:
Each player follows the preceding card, laid on the deck, with a card of the same color or figure.  
The object of the game is to discard all the cards in your hand.
  
  
    
    
## Special cards:
* **Stop** - The next player loses their turn.
* **Plus** - Forces the user to play again. If the player cannot play another card he must draw.
* **Switch Direction** - Reverses the direction of the play.
* **Switch Color** - Allows the user to determine the color to be played by the next player.
* **TAKI** – Allows a player to follow with all the cards of the same color as the TAKI card.
  
  
  
  
## How To Play:
At the start of the game, you will get the following screen.  
Enter the number of players that will play the game:  
<img width="682" alt="Screen Shot 2022-09-21 at 16 26 35" src="https://user-images.githubusercontent.com/104992892/191517127-3e28e769-d63a-409c-abb3-1196b1b317c4.png">

You will be asked to enter the name of each player.  
This will also set the order of the players' turns:  
<img width="682" alt="Screen Shot 2022-09-21 at 16 27 25" src="https://user-images.githubusercontent.com/104992892/191517462-b96ba236-2b9d-425a-a441-b99e68c27fb2.png">

Now the game will start.  
You will see the current card on top of the deck, and the cards of the player currently playing.  
You will get a prompt to choose which card to put on top of the deck, or to draw a new card:  
<img width="584" alt="Screen Shot 2022-09-21 at 16 27 43" src="https://user-images.githubusercontent.com/104992892/191517880-c9563c09-1f85-4649-845c-47c732a94219.png">

If a player choses to put a COLOR card, which he can put on any card in the deck,  
he will get to choose which color the card will take:  
<img width="584" alt="Screen Shot 2022-09-21 at 16 28 14" src="https://user-images.githubusercontent.com/104992892/191518229-7b71298e-a697-4b88-a5b4-46b15f917f4f.png">

The chosen color will now show at the top of the deck:  
<img width="584" alt="Screen Shot 2022-09-21 at 16 28 33" src="https://user-images.githubusercontent.com/104992892/191518357-c9ef29ad-d97c-4f5f-b40b-45763c13aa8a.png">

When a player drops all of his cards, the game will be finished.  
A winning message will be shown, followed by the statistics of the game.  
The statistics will show frequency of each card dropped in the game:  
<img width="584" alt="Screen Shot 2022-09-21 at 16 29 11" src="https://user-images.githubusercontent.com/104992892/191518676-d779dd15-d6f5-4f41-8e7a-429235819e95.png">
   
   
### Some Notes:
1.  The TAKI card will allow the play to drop any card that is the same color as the TAKI card,  
    until he choses to draw a new card from the deck, or he finished all of his cards and won the game.
    
2. The STOP card will skip the next player's turn,  
   the next turn will be of the player that is after that player.

3. The SWITCH DIRECTION card will switch the direction of the turns,  
   the player that player before the current player will get the next turn.
   
4. The PLUS card forces the user to play again.  
   If the player cannot play another card he must draw a new card.


Credit for the README file: [@Idan-sh](https://github.com/Idan-sh)
