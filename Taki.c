// Samuel Lazareanu 53036281
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // in order to use dynamic allocation
#include <time.h>	// in order to use "time" function

// MAX values defining
#define MAX_PLAYER_NAME_LENGTH 21		// 20 + 1 for the '\0'
#define MAX_CARD_SYMBOL_NAME 5		// 4 ("TAKI") + 1 for the '\0'

#define NUM_OF_CARD_TYPES 14		// 1-14

// Color card value defining
#define NO_COLOR_CARD_VALUE 0
#define YELLOW_CARD_COLOR_VALUE 1
#define RED_CARD_COLOR_VALUE 2
#define BLUE_CARD_COLOR_VALUE 3
#define GREEN_CARD_COLOR_VALUE 4

// Special card value defining
#define PLUS_CARD_VALUE 10
#define STOP_CARD_VALUE 11
#define CHANGE_DIRECTION_CARD_VALUE 12
#define TAKI_CARD_VALUE 13
#define COLOR_CARD_VALUE 14

// Starting values defining
#define STARTING_AMOUT_OF_CARDS 4

// Structs
typedef struct Card {
	int cardValue;		// Card value ranging from 1 to 14 (10 to 14 are special cards)
	int cardColor;		// Yellow - 1, Red - 2, Blue - 3, Green - 4
} CARD, Card;

typedef struct CardStats {
	int cardValue;
	int times;
} CARD_STATS, CardStats;

typedef struct Player {
	char name[MAX_PLAYER_NAME_LENGTH];		// Players name
	int physicalCards;		// Physical space for cards in hand
	int logicalCards;		// Actual num of cards in hand
	Card* hand;			// The address of the players hand
} PLAYER, Player;

typedef struct Game {
	Player *players;		// The address of the players database
	int numOfPlayers;		// The number of players in the game
	Card* deck;				// The deck of cards in the mid of the table
	int physicalCards;		// Physical space for cards in deck
	int logicalCards;		// Actual num of cards in deck
	int winner;				// Set to the index of the winner in *players, or -1 if a winner wasn't found
	int turn;				// Index of the player whos turn is now
	int direction;			// 1 is clockwise, -1 is counterclockwise
	bool takiMode;			// T/F if TAKI mode is ON (A taki card was placed and the turn wasn't finished)
} GAME, Game;

// Declaring functions
Card* cardRealloc(Card* cards, int size, int newSize);
Player* setPlayers(int numOfPlayers);
void setPlayersName(Player* players, int numOfPlayers);
Card getRandomCard();
Card getRandomNormalCard();
void startGame(Game *game);
void gameHandler(Game* game);
void cardPlacementHandler(Game* game, int cardIndex);
void drawCardsToPlayer(Player* player, int numOfCards);
void drawCardToMid(Game* game, Card card);
bool canPlaceCard(Game* game, Card* card);
void placeCard(Game* game, int cardIndex);
void printCard(Card card);
void printHand(Player* player);
void yellowText();
void redText();
void blueText();
void greenText();
void resetTextColor();
void freePlayersHands(Game* game);
void sortCardStats(CardStats cardStats[], int size);
CardStats* merge(CardStats data1[], int size1, CardStats data2[], int size2);
void copyArr(CardStats data1[], CardStats* data2, int size);
void printGameStatistics(Game* game);

// MAIN
void main() {
	//Player* players = NULL;		// A database of the players in the game
	//int numOfPlayers;		// The number of players in the game
	Game* game;

	srand(time(NULL));

	game = (Game*)malloc(sizeof(Game));

	// check if memory allocation was successful
	if (game == NULL) {
		printf("Memory allocation failed !!! \n");
		exit(1);
	}

	printf("************  Welcome to TAKI game !!! *********** \n");
	printf("Please enter the number of players: \n");
	scanf("%d", &game->numOfPlayers);

	game->players = setPlayers(game->numOfPlayers);

	// set starting values, etc.
	startGame(game);

	// playing the game - a func that handles the turns and card drawing/placing
	gameHandler(game);

	// a winner was found beacuse gameHandler finished its work
	printf("\nThe winner is... %s! Congratulations!\n\n", (game->players + game->winner)->name);

	// Game statistics
	printGameStatistics(game);
	printf("\n");

	// free all allocated memory
	freePlayersHands(game);		// free players hands
	free(game->players);		// free players
	free(game->deck);		// free the deck of the game
	free(game);				// free the game
}

// Realloc function for Card*
// Receives address of cards database, its size and a desired new size
// returns the address of a new database with the desired new size and the old databases data
Card* cardRealloc(Card* cards, int size, int newSize) {
	int i;
	Card* newCards = (Card*)malloc(newSize * sizeof(Card));

	// Check if memory allocation was successful
	if (newCards == NULL) {
		printf("Memory re-allocation failed !!! \n");
		exit(1);
	}

	for (i = 0; i < size && i < newSize; i++)
		*(newCards + i) = *(cards + i);

	free(cards);		// Copied data to newCards - can free cards now

	return newCards;
}

// this func receives the number of players in the game and creates a database for them with their names
// and returns the address of the players database
Player* setPlayers (int numOfPlayers){
	Player* players = NULL;

	// allocate memory to players
	players = (Player*)malloc(numOfPlayers * sizeof(Player));

	// check if memory allocation was successful
	if (players == NULL) {
		printf("Memory allocation failed !!! \n");
		exit(1);
	}

	// set player names
	setPlayersName(players, numOfPlayers);

	return players;
}

// this func receives the address of the players database and the number of players
// gets their names from the user and sets them in the database
void setPlayersName(Player* players, int numOfPlayers) {
	int i;

	for (i = 0; i < numOfPlayers; i++) {
		printf("Please enter the first name of player #%d: \n", (i + 1));
		scanf(" %s", (players+i)->name);
		printf("\n");
	}
}

// A func that runs at the beginning of the game
// It receives a game and initializes its values - setting up the game
void startGame(Game *game) {
	int playerInd,
		cardInd;

	for (playerInd = 0; playerInd < game->numOfPlayers; playerInd++) {
		// Create a hand for each player
		(game->players + playerInd)->hand = NULL;	// Reset value to NULL to check if allocation was successfull
		(game->players + playerInd)->hand = (Card*)malloc(STARTING_AMOUT_OF_CARDS * sizeof(Card));

		// Check if memory allocation was successful
		if ((game->players + playerInd)->hand == NULL) {
			printf("Memory allocation failed !!! \n");
			exit(1);
		}

		// Reset num of cards values
		(game->players + playerInd)->logicalCards = STARTING_AMOUT_OF_CARDS;
		(game->players + playerInd)->physicalCards = 0;

		// Give each player 4 rand cards
		drawCardsToPlayer(game->players + playerInd, STARTING_AMOUT_OF_CARDS);
	}

	// Create a "deck" - the cards that face upwards in the mid of the table
	game->deck = NULL;	// Reset value to NULL to check if allocation was successfull
	game->deck = (Card*)malloc(sizeof(Card));
	// Check if memory allocation was successful
	if (game->deck == NULL) {
		printf("Memory allocation failed !!! \n");
		exit(1);
	}

	// Reset num of cards values
	game->logicalCards = 1;
	game->physicalCards = 0;

	// Set a rand normal card in the mid
	drawCardToMid(game, getRandomNormalCard());

	// Set winner value to -1 (a winner wasnt found)
	game->winner = -1;

	// Current turn index - set to 0 - the first player
	game->turn = 0;

	// Set default direction of playing
	game->direction = 1;		// clockwise

	// Set taki mode to F, no card was placed yet
	game->takiMode = false;
}

// Handles the game - giving turns, etc. until a winner is found
void gameHandler(Game* game) {
	Player* currPlayer;			// The user whos turn is now
	Card* chosenCardToPut;		// If the user chose to put a card down - the address to the card
	int chosenAction;		// The action chosen by the user - take or put card
	bool isChoiceOk = false;

	// The game keeps running until a winner is found
	while (game->winner == -1) {
		// Reset chosen action
		chosenAction = -1;
		isChoiceOk = false;

		currPlayer = game->players + game->turn;
		// Print the upper card in the mid of the table
		printf("Upper card:\n");
		printCard(*(game->deck + game->physicalCards - 1));

		// Print the player whos turn is now
		printf("%s's turn:\n\n", currPlayer->name);
		printHand(currPlayer);

		// The player whos turn is now chooses action
		// Keeps running until the choice is valid
		while (!isChoiceOk) {
			if (!game->takiMode)		// If taki mode isnt on
				printf("Please enter 0 if you want to take a card from the deck\n");
			else
				printf("Please enter 0 if you want to take finish your turn\n");
			printf("or %d-%d if you want to put one of your cards in the middle:\n", 1, currPlayer->physicalCards);	// The player must have at least 1 card otherwise he is the winner
			scanf("%d", &chosenAction);

			if (chosenAction > currPlayer->physicalCards || chosenAction < 0)
				printf("Invalid choice! Try again.\n");
			else if (chosenAction >= 1 && chosenAction <= currPlayer->physicalCards && !canPlaceCard(game, currPlayer->hand + chosenAction - 1))
				printf("Invalid card! Try again.\n");
			else
				isChoiceOk = true;		// The choice is OK - move on
		}

		if (chosenAction == 0 && !game->takiMode) {
			drawCardsToPlayer(currPlayer, 1);
			game->turn += 1 * game->direction;
		}
		else if (chosenAction == 0 && game->takiMode) {		// user finished round after Taki card was put
			game->takiMode = false;		// Reset taki mode

			// Check if last card put is special - if so - act accordingly
			switch ((game->deck + game->physicalCards - 1)->cardValue)
			{
			case PLUS_CARD_VALUE:
				// If the player has no more cards - draw him one
				if (currPlayer->physicalCards == 0)
					drawCardsToPlayer(currPlayer, 1);
				// else - the turn remains his and he needs to put another card
				break;
			case STOP_CARD_VALUE:
				game->turn += 1 * (game->direction);		// Skips one player (turn) (we skip another at the end of the IF)
				break;
			case CHANGE_DIRECTION_CARD_VALUE:
				game->direction *= -1;		// change directions: 1 to (-1), (-1) to 1
				break;
			case TAKI_CARD_VALUE:
				break;
			case COLOR_CARD_VALUE:
				// Choose color - finished taki mode with Color card on top
				printf("Please enter your color choice:\n");
				printf("%d - Yellow\n", YELLOW_CARD_COLOR_VALUE);
				printf("%d - Red\n", RED_CARD_COLOR_VALUE);
				printf("%d - Blue\n", BLUE_CARD_COLOR_VALUE);
				printf("%d - Green\n", GREEN_CARD_COLOR_VALUE);
				scanf("%d", &(game->deck + game->physicalCards - 1)->cardColor);	// Update mid card color
				break;
			default:	// Normal card (1-9)
				break;
			}
			game->turn += 1 * game->direction;
		}
		else if (chosenAction >= 1 && chosenAction <= currPlayer->physicalCards) {
			chosenCardToPut = currPlayer->hand + chosenAction - 1;
			// if the card can be placed (based on the rules), placing it accordingly
			cardPlacementHandler(game, chosenAction - 1);
		}

		// keeps the turns between the players - handles if the turn exceeds the amount of players (turn < 0 OR turn >= numOfPlayers)
		if (game->turn >= game->numOfPlayers)		// exceed clockwise
			game->turn %= game->numOfPlayers;		// %numOfPlayers to reset
		else if (game->turn < 0)					// exceed counterclockwise
			game->turn += game->numOfPlayers;		// Add the num of players 

		// Checks if winner is found - handles is last placed card was special which needs a draw from the pack
		game->winner = searchWinner(game);
	}
}

// Receives a game and the card wanted to be placed in the mid
// Returns T/F if the card can be placed in the mid (by the rules of the game)
bool canPlaceCard(Game* game, Card* card) {
	Card *upperCard = game->deck + game->physicalCards - 1;
	// the COLOR card can be placed on every card
	if (card->cardValue == COLOR_CARD_VALUE)
		return true;
	// Same card color
	if (card->cardColor == upperCard->cardColor)
		return true;
	// Same card value / symbol (including specials)
	if (card->cardValue == upperCard->cardValue)
		return true;

	// If none of the above is T
	return false;
}

// Receives a game and the index of the card needed to be placed in the mid (player and card can be found from the game struct)
// Places the card according to its type
void cardPlacementHandler(Game* game, int cardIndex) {
	Player* currPlayer = game->players + game->turn;			// The user whos turn is now
	Card* chosenCardToPut = currPlayer->hand + cardIndex;		// The card that needs to be placed
	switch (chosenCardToPut->cardValue)
	{
	case PLUS_CARD_VALUE:
		placeCard(game, cardIndex);
		// If the player has no more cards - draw him one
		if (!game->takiMode) {
			if (currPlayer->physicalCards == 0) {
				drawCardsToPlayer(currPlayer, 1);
				game->turn += 1 * (game->direction);
			}
			// else - the turn remains his and he needs to put another card
		}
		break;
	case STOP_CARD_VALUE:
		placeCard(game, cardIndex);
		if (!game->takiMode)
			game->turn += 2 * (game->direction);		// Skips one player (turn)
		break;
	case CHANGE_DIRECTION_CARD_VALUE:
		placeCard(game, cardIndex);
		if (!game->takiMode) {
			game->direction *= -1;		// change directions: 1 to (-1), (-1) to 1
			game->turn += 1 * (game->direction);
		}
		break;
	case TAKI_CARD_VALUE:
		placeCard(game, cardIndex);
		if (!game->takiMode)
			game->takiMode = true;
		break;
	case COLOR_CARD_VALUE:
		placeCard(game, cardIndex);
		// Choose color - if not in taki mode
		if (!game->takiMode) {
			printf("Please enter your color choice:\n");
			printf("%d - Yellow\n", YELLOW_CARD_COLOR_VALUE);
			printf("%d - Red\n", RED_CARD_COLOR_VALUE);
			printf("%d - Blue\n", BLUE_CARD_COLOR_VALUE);
			printf("%d - Green\n", GREEN_CARD_COLOR_VALUE);
			scanf("%d", &(game->deck + game->physicalCards - 1)->cardColor);	// Update mid card color
			game->turn += 1 * game->direction;
		}
		else {
			// We are in TAKI mode and COLOR was placed
			// We set its color to the color of the TAKI card, if the user ends the round here, he will have the option to change it
			// NOTE: I wanted to see how you did it but there was a bug in your executable file regarding this situation.
			(game->deck + game->physicalCards - 1)->cardColor = (game->deck + game->physicalCards - 2)->cardColor;
		}
		break;
	default:	// Normal card (1-9)
		placeCard(game, cardIndex);
		if (!game->takiMode)
			game->turn += 1 * game->direction;
		break;
	}
}

// Receives a game and card index to place down
// (relavant player is known because of game->turn)
// The func places the selected card based on its type and rearranges the hand of the player
void placeCard(Game* game, int cardIndex) {
	Player* currPlayer = game->players + game->turn;	// the player whos turn is noe
	Card* cardToPlace = currPlayer->hand + cardIndex;	// the chosen card to put down
	int readIndex, writeIndex;		// read and write index to help rearrange hand of cards after placement

	// Put card in the mid
	drawCardToMid(game, *cardToPlace);
	currPlayer->physicalCards--;

	// rearranging hand of cards - removing the placed card
	writeIndex = cardIndex;
	readIndex = writeIndex + 1;
	while (readIndex <= currPlayer->physicalCards) {
		*(currPlayer->hand + writeIndex) = *(currPlayer->hand + readIndex);
		writeIndex++;
		readIndex++;
	}
}

// Returns a random card
Card getRandomCard() {
	Card card;

	card.cardValue = 1 + rand() % 14;		// Random card value (1 - 14, including special)
	if (card.cardValue != 14)		// If the card randomized is "COLOR" it has no color at the beginning
		card.cardColor = 1 + rand() % 4;		// Random color value (1 - 4)
	else
		card.cardColor = 0;
	return card;
}

// Returns a random NORMAL card (no special cards, meaning card value is 1 - 9)
Card getRandomNormalCard() {
	Card card;

	card.cardColor = 1 + rand() % 5;		// Random color value (1 - 4)
	card.cardValue = 1 + rand() % 10;		// Random card value (1 - 9, without special)

	return card;
}

// Receives a player and the number of cards needed to be drawn to his hand
// draws this number of cards to hand
void drawCardsToPlayer(Player* player, int numOfCards) {
	int counter;
	Card* newHand;
	
	// Check there is enough space for cards in hand
	while (player->physicalCards + numOfCards > player->logicalCards) {
		newHand = cardRealloc(player->hand, player->logicalCards, player->logicalCards * 2);

		// Check if memory re-allocation was successful
		if (newHand == NULL) {
			printf("Memory re-allocation failed !!! \n");
			exit(1);
		}
		// else - memory reallocation was successful - set new player's hand
		player->hand = newHand;
		player->logicalCards *= 2;
	}

	for (counter = 0; counter < numOfCards; counter++) {
		*(player->hand + player->physicalCards) = getRandomCard();
		player->physicalCards++;
	}
		
}

// Receives a game and a card to put in the mid of the table - puts it in
void drawCardToMid(Game* game, Card card) {
	int cardInd;
	Card* newDeck;

	// Check there is enough space for cards in hand
	while (game->physicalCards >= game->logicalCards) {
		newDeck = cardRealloc(game->deck, game->logicalCards, game->logicalCards * 2);

		// Check if memory re-allocation was successful
		if (newDeck == NULL) {
			printf("Memory re-allocation failed !!! \n");
			exit(1);
		}
		// else - memory reallocation was successful - set new deck
		game->deck = newDeck;
		game->logicalCards *= 2;
	}

	// After checking its possible - draw card to mid
	*(game->deck + game->physicalCards) = card;
	game->physicalCards++;

}

// Receives a card to print
// Prints the card in the form required
void printCard(Card card) {
	char cardColor;

	// check and set card color
	switch (card.cardColor)
	{
	case NO_COLOR_CARD_VALUE:
		cardColor = ' ';	// If the card has no color
		break;
	case YELLOW_CARD_COLOR_VALUE:
		cardColor = 'Y';
		yellowText();		// sets the text to be yellow
		break;
	case RED_CARD_COLOR_VALUE:
		cardColor = 'R';
		redText();		// sets the text to be red
		break;
	case BLUE_CARD_COLOR_VALUE:
		cardColor = 'B';
		blueText();		// sets the text to be blue
		break;
	case GREEN_CARD_COLOR_VALUE:
		cardColor = 'G';
		greenText();		// sets the text to be green
		break;
	default:
		break;
	}

	printf("*********\n");
	printf("*       *\n");
	// Card number
	switch (card.cardValue)
	{
	case PLUS_CARD_VALUE:
		printf("*   +   *\n");
		break;
	case STOP_CARD_VALUE:
		printf("*  STOP *\n");
		break;
	case CHANGE_DIRECTION_CARD_VALUE:
		printf("*  <->  *\n");
		break;
	case TAKI_CARD_VALUE:
		printf("*  TAKI *\n");
		break;
	case COLOR_CARD_VALUE:
		printf("* COLOR *\n");
		break;
	default:
		printf("*   %d   *\n", card.cardValue);
		break;
	}
	printf("*   %c   *\n", cardColor);
	printf("*       *\n");
	printf("*********\n\n");

	resetTextColor();		// resets text color back to normal
}

// Receives an address to a player
// Prints its hand of cards
void printHand(Player* player) {
	int i;

	for (i = 0; i < player->physicalCards; i++) {
		printf("Card #%d: \n", i + 1);
		printCard(*(player->hand+i));
	}
}

// Sets the color of the text to yellow
void yellowText() {
	printf("\033[1;33m");
}

// Sets the color of the text to red
void redText() {
	printf("\033[1;31m");
}

// Sets the color of the text to blue
void blueText() {
	printf("\033[0;34m");
}

// Sets the color of the text to green
void greenText() {
	printf("\033[0;32m");
}

// Resets the color of the text to default
void resetTextColor() {
	printf("\033[0;37m");
}

// This func sets free the dynamicly allocated memory of each players hand
void freePlayersHands(Game* game) {
	int i;
	Card* currPlayerHand;

	for (i = 0; i < game->numOfPlayers; i++) {
		currPlayerHand = (game->players + i)->hand;
		free(currPlayerHand);
	}
}

// This func checks if a winner was found
// Returns -1 if not found, the index of the winner if found
int searchWinner(Game* game) {
	int i;
	// Checks if there is a player with 0 cards
	// (if he puts + and has no cards, another func deals him a card automatically so he wont have 0)
	for (i = 0; i < game->numOfPlayers; i++) {
		if ((game->players + i)->physicalCards == 0)
			return i;
	}
	return -1;
}

void printGameStatistics(Game* game) {
	CardStats cardStats[NUM_OF_CARD_TYPES];
	int i;		// used for loops


	// Reset values
	for (i = 0; i < NUM_OF_CARD_TYPES; i++) {
		cardStats[i].cardValue = i + 1;
		cardStats[i].times = 0;
	}

	// Inset values from the deck in the mid of the table
	for (i = 0; i < game->physicalCards; i++) {
		cardStats[(game->deck + i)->cardValue - 1].times++;
	}

	sortCardStats(cardStats, NUM_OF_CARD_TYPES);

	// Printing the stats
	printf("************ Game Statistics ************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n");

	for (i = 0; i < NUM_OF_CARD_TYPES; i++) {
		switch (cardStats[i].cardValue)
		{
		case PLUS_CARD_VALUE:
			printf("   +   |%5d\n", cardStats[i].times);
			break;
		case STOP_CARD_VALUE:
			printf(" STOP  |%5d\n", cardStats[i].times);
			break;
		case CHANGE_DIRECTION_CARD_VALUE:
			printf("  <->  |%5d\n", cardStats[i].times);
			break;
		case TAKI_CARD_VALUE:
			printf(" TAKI  |%5d\n", cardStats[i].times);
			break;
		case COLOR_CARD_VALUE:
			printf(" COLOR |%5d\n", cardStats[i].times);
			break;
		default: // 1-9
			printf("   %d   |%5d\n", cardStats[i].cardValue, cardStats[i].times);
			break;
		}
	}
}

// Sort the statistics of the cards from most frequent to least frequent
void sortCardStats(CardStats cardStats[], int size) {
	Card* tmpArr = NULL;
	if (size <= 1)
		return;

	sortCardStats(cardStats, size / 2);
	sortCardStats(cardStats + size / 2, size - size / 2);
	tmpArr = merge(cardStats, size / 2, cardStats + size / 2, size - size / 2);
	if (tmpArr) {
		copyArr(cardStats, tmpArr, size);
		free(tmpArr);
	}
	else {
		printf("Memory allocation failed !!!");
		exit(1);
	}

}

// Merges 2 sorted arrays into 1 sorted array
// used as a helper for sortCardStats
CardStats* merge(CardStats data1[], int size1, CardStats data2[], int size2) {
	int ind1, ind2, resInd;
	CardStats* res = (CardStats*)malloc((size1 + size2) * sizeof(CardStats));

	if (res) {
		ind1 = ind2 = resInd = 0;

		while (ind1 < size1 && ind2 < size2) {
			if (data1[ind1].times >= data2[ind2].times) {
				res[resInd] = data1[ind1];
				ind1++;
			}
			else {
				res[resInd] = data2[ind2];
				ind2++;
			}
			resInd++;
		}

		while (ind1 < size1) {
			res[resInd] = data1[ind1];
			ind1++;
			resInd++;
		}

		while (ind2 < size2) {
			res[resInd] = data2[ind2];
			ind2++;
			resInd++;
		}
	}
	return res;
}

// Copy one array to another
void copyArr(CardStats data1[], CardStats* data2, int size) {
	int i;

	for (i = 0; i < size; i++) {
		data1[i] = *(data2 + i);
	}
}
