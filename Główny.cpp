#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> 
#include <iostream>
#include <time.h> //For srand(time(NULL)))
#include <Windows.h> //For Sleep
#include <stdlib.h>
 

#define MAX_COLOR_SIZE 7
#define MAX_LINE_LENGTH 1000
#define NOBODY -1
#define DEFAULT_MEMORY_ALLOCATION_SIZE 1
#define GAME_FILE "output.txt"

using namespace std;



enum TrueofFalse {
    False,
    True,
};


enum colors{
    green,
    blue,
    red,
    violet,
    yellow,
    white,
    black,
};



typedef struct input { //Main game values
    int players_count;
    int cauldron_count;
    int green_count;
    int green_value;
    int other_cards_count;
    int *values;
}input_values;


typedef struct cards { //When Generating Deck in task 1 values are stored here
    int* green; 
    int* blue;
    int* red;
    int* violet;
    int* yellow;
    int* white;
    int* black;
} game_cards;


typedef struct card {
    int value;
    int color;
} card_type;


typedef struct player {
    int player_number;
    int hand_cards_count;
    int deck_cards_count;
    card_type* player_hand_cards;
    card_type* player_deck_cards;
    int max_hand_cards_count; //for dynamic arrays to know when to expand
    int max_deck_cards_count;
}player_x;


typedef struct piler {
    int cards_amount;
    card_type* cards;
    int max_card_count;
}pile;


typedef struct game_datas {
    int active_player;
    int pile_threshold;
}game_data;



card_type* EasyTwiceArrayEnlargement(card_type* all_cards, int max_card_count) { //Increases the array with cards twice
    card_type* new_array = (card_type*)malloc(2 * max_card_count * sizeof(card_type));
    if (new_array != NULL) {
        memcpy(new_array, all_cards, max_card_count * sizeof(card_type));
        free(all_cards);
    }
    else {
        cout << "Error allocating memory\n";
    }
    return new_array;
}


void UserInput(input_values* user_input, game_data * data) { //Reads data from stdin according to task 1 and 2 with additional pile_threshold
    //cin >> data->active_player; was used previously
    data->active_player = 0;
    cin >> user_input->players_count;
    cin >> user_input->cauldron_count;
    cin >> user_input->green_count;
    cin >> user_input->green_value;
    cin >> user_input->other_cards_count;
    cin >> data->pile_threshold;
    user_input->values = (int*)malloc(user_input->other_cards_count * sizeof(int));
    if (user_input->values != NULL) {
            for (int i = 0; i < user_input->other_cards_count; i++) {
                cin >> *(user_input->values+i);

            }
    }
}


int* GetCardArray(game_cards* all_cards, int i) { //Prints data for task 1
    switch (i) {
    case 1:
        return (all_cards->blue);
    case 2:
        return (all_cards->red);
    case 3:
        return (all_cards->violet);
    case 4:
        return (all_cards->yellow);
    case 5:
        return (all_cards->white);
    case 6:
        return (all_cards->black); 
    default:
        return (all_cards->green);

    }

}


player_x **InitializePlayers(int players_count) {
    player_x** players_table = (player_x**)malloc(players_count * sizeof(player_x*));
    if (players_table != NULL) {
        for (int i = 0; i < players_count; i++) {
            *(players_table + i) = (player_x*)malloc(sizeof(player_x));
            if (*(players_table + i) != NULL) {
                (*(players_table + i))->player_number = i;
                (*(players_table + i))->deck_cards_count = 0;
                (*(players_table + i))->hand_cards_count = 0;
                (*(players_table + i))->player_hand_cards = (card_type*)malloc(1 * sizeof(card_type));
                (*(players_table + i))->player_deck_cards = (card_type*)malloc(1 * sizeof(card_type));
                (*(players_table + i))->max_deck_cards_count = DEFAULT_MEMORY_ALLOCATION_SIZE;
                (*(players_table + i))->max_hand_cards_count = DEFAULT_MEMORY_ALLOCATION_SIZE;
            }
        }
    }
    
    return players_table;
}


void FreeMemoryAfterPlayers(player_x** players_table, const input_values user_input) {
    for (int i = 0; i < user_input.players_count; i++) {
        free((*(players_table + i))->player_deck_cards);
        free((*(players_table + i))->player_hand_cards);
        free(*(players_table + i));
    }
    free(players_table);
}


pile** InitializePiles(int cauldron_count) {
    pile** piles = (pile**)malloc(cauldron_count * sizeof(pile*));
    if (piles != NULL) {
        for (int i = 0; i < cauldron_count; i++) {
            *(piles + i) = (pile*)malloc(sizeof(pile));
            if (*(piles + i) != NULL) {
                (*(piles + i))->cards_amount = 0;
                (*(piles + i))->cards = (card_type*)malloc(1 * sizeof(card_type));
                (*(piles + i))->max_card_count = DEFAULT_MEMORY_ALLOCATION_SIZE;
            }
        }
    }
    return piles;
}


void FreeMemoryAfterPiles(piler** piles, const input_values user_input) {
    for (int i = 0; i < user_input.cauldron_count; i++) {
        free((*(piles + i))->cards);
    }
    free(piles);
}


card_type *CardsGenerate(const input_values user_input) {
    card_type *all_cards = (card_type*)malloc((user_input.green_count + user_input.other_cards_count * user_input.cauldron_count) * sizeof(card_type));
    if (all_cards == NULL) {
        cout << "Error allocating memory. Please try again later.";
        return all_cards;
    }
    int created_cards = 0; ///Counter for moving in the array of all_cards
    for (int i = 0; i < user_input.green_count; i++) {
        (*(all_cards + created_cards)).color = green;
        (*(all_cards + created_cards)).value = user_input.green_value;
        created_cards++;
    }
    for (int i = 1; i <= user_input.cauldron_count; i++) {
        for (int j = 0; j < user_input.other_cards_count; j++) {
            (*(all_cards + created_cards)).value = user_input.values[j];
            (*(all_cards + created_cards)).color = i;
            created_cards++;
        }
    }
    return all_cards;
}


void FreeMemoryAfterCards(card_type* all_cards) {
    free(all_cards);
}


const char* SayMyName(int color) { //Retuns color name
    switch (color) {
    case 0:
        return "green";
        break;
    case 1:
        return "blue";
        break;
    case 2:
        return "red";
        break;
    case 3:
        return "violet";
        break;
    case 4:
        return "yellow";
        break;
    case 5:
        return "white";
        break;
    case 6:
        return "black";
        break;
    }
    return "green";
}


void Print1(const input_values user_input, game_cards* all_cards) { //For task 1 and 2
    FILE* p = fopen(GAME_FILE, "w");
    int* wskaznik = all_cards->green;
    for (int i = 0; i < user_input.green_count; i++) {
        fprintf(p, "%d green ", *wskaznik);
        wskaznik++;
    }
    char kolor[7][7] = { "green", "blue", "red", "violet", "yellow", "white", "black" };
    for (int i = 1; i <= user_input.cauldron_count; i++) {

        for (int j = 0; j < user_input.other_cards_count; j++)
            fprintf(p, "%d %s ", GetCardArray(all_cards, i)[j], kolor[i]);
    }
    fclose(p);
}


void Print2(player_x** players_table, input_values user_input, game_data current_game_data, pile** piles) { //For general usage to write to file
    FILE* p = fopen(GAME_FILE, "w");
    //cout << "active player = " << (current_game_data.active_player) + 1 << endl;
    fprintf(p, "active player = %d\n", (current_game_data.active_player) + 1);
    //cout << "players number = " << user_input.players_count << endl;
    fprintf(p, "players number = %d\n", user_input.players_count);
    //cout << "explosion threshold = " << current_game_data.pile_threshold << endl;
    fprintf(p, "explosion threshold = %d\n", current_game_data.pile_threshold);
    for (int i = 0; i < user_input.players_count; i++) {
        //cout << i+1 << " player hand cards:";
        fprintf(p, "%d player hand cards:", i + 1);
        for (int j = 0; j < (*(players_table + i))->hand_cards_count; j++) {
            //cout << " " << (*(players_table + i))->player_hand_cards[j].value << " ";
            //SayMyName((*(players_table + i))->player_hand_cards[j].color);
            fprintf(p, " %d ", (*(players_table + i))->player_hand_cards[j].value);
            fprintf(p, SayMyName((*(players_table + i))->player_hand_cards[j].color));
        }
        //cout << endl << i+1 << " player deck cards:";
        fprintf(p, "\n");
        fprintf(p, "%d player deck cards:", i + 1);
        for (int j = 0; j < (*(players_table + i))->deck_cards_count; j++) {
            //cout << " " << (*(players_table + i))->player_deck_cards[j].value << " "; 
            //SayMyName((*(players_table + i))->player_deck_cards[j].color);
            fprintf(p, " %d ", (*(players_table + i))->player_deck_cards[j].value);
            fprintf(p, SayMyName((*(players_table + i))->player_deck_cards[j].color));
            
        }
        fprintf(p, "\n");
        //cout << endl;
    }
    for (int i = 0; i < user_input.cauldron_count; i++) {
        //cout << i + 1 << " pile cards:";
        fprintf(p, "%d pile cards:", i + 1);
        for (int j = 0; j < (*(piles + i))->cards_amount; j++) {
            //cout << " " << (*(piles + i))->cards[j].value << " "; 
            //SayMyName((*(piles + i))->cards[j].color);
            fprintf(p, " %d ", (*(piles + i))->cards[j].value);
            fprintf(p, SayMyName((*(piles + i))->cards[j].color));
        }
        //cout << endl;
        fprintf(p, "\n");
    }
    fclose(p);
}


void Print3(player_x** players_table, input_values user_input, pile** piles) { //For task 3a and 3b
    FILE *p = fopen("", "w");
    for (int i = 0; i < user_input.players_count; i++) {
        fprintf(p, "%d players has %d on hand\n", i + 1, (*(players_table + i))->hand_cards_count);
        fprintf(p, "%d players has %d in front of him\n", i + 1, (*(players_table + i))->deck_cards_count);
    }
    for (int i = 0; i < user_input.cauldron_count; i++) {
        fprintf(p, "there are %d cards on %d pile\n", (*(piles + i))->cards_amount, i+1);
    }
    fclose(p);
}


void CardDealer(player_x **players_table, input_values user_input, card_type* all_cards) {
    int given_cards = 0;
    int amount = user_input.green_count + user_input.cauldron_count * user_input.other_cards_count; //how many cards are there to be given to players
    for (int i = 0; i < amount; i++) {
        if ((*(players_table + given_cards % user_input.players_count))->hand_cards_count >= (*(players_table + given_cards % user_input.players_count))->max_hand_cards_count) {
            (*(players_table + given_cards % user_input.players_count))->player_hand_cards = EasyTwiceArrayEnlargement((*(players_table + given_cards % user_input.players_count))->player_hand_cards, (*(players_table + given_cards % user_input.players_count))->max_hand_cards_count);
            (*(players_table + given_cards % user_input.players_count))->max_hand_cards_count *= 2;
        }
        (*(players_table + given_cards % user_input.players_count))->player_hand_cards[(*(players_table + given_cards % user_input.players_count))->hand_cards_count].value = (*(all_cards + given_cards)).value;
        (*(players_table + given_cards % user_input.players_count))->player_hand_cards[(*(players_table + given_cards % user_input.players_count))->hand_cards_count].color = (*(all_cards + given_cards)).color;
        (*(players_table + given_cards % user_input.players_count))->hand_cards_count += 1; //we give cards to player on position how many cards does the player have, % allows to go to next player
        given_cards++;
    }
}


void SortAscending(int tablica[], int size) {
    int temp;
    for (int i = 1; i < size; i++) {
        for (int j = 1; j < size; j++) {
            if (tablica[j - 1] > tablica[j]) {
                temp = tablica[j];
                tablica[j] = tablica[j - 1];
                tablica[j - 1] = temp;
            }
        }
    }
}


int CountPiles(int card_count[]) {
    int cauldron_count = 0;
    for (int i = 1; i < MAX_COLOR_SIZE; i++) {
        if (card_count[i] > 0) {
            cauldron_count++;
        }
        else
            break;
    }
    return cauldron_count;
}


int CardColorChar(char color[]) { 
    if (color[0] == 'b' && color[1] == 'l' && color[2] == 'a') { //Keeping in mind color name length is longer than 3 chars
        return black;
    }
    else {
        switch (color[0]) {
        case 'g':
            return green;
        case 'b':
            return blue;
        case 'r':
            return red;
        case 'y':
            return yellow;
        case 'w':
            return white;
        case 'v':
            return violet;
        }
    }
    return green;
}


int ReadTillNumber(char* buffor) {
    int number=0;
    char* temp;
    temp = strtok(buffor, " ");
    do {
        temp = strtok(NULL, " ");
        if (isdigit(temp[0])) {
            number = atoi(temp);
            break;
        }
    } while (temp != NULL);
    return number;
}


void ReadGameValues(input_values user_input, game_data *current_game_data, player_x*** players_table_ptr, pile*** piles_ptr ) {
    FILE* p = fopen(GAME_FILE, "r");
    char buffor[MAX_LINE_LENGTH]; //reads lines to this array before further stripping
    char whitespace[] = " "; //for strtok (we divide buffor by whitespaces)
    char* card_value; //for card_numbers while str_toking
    char* card_color; //for card_
    int* card_values = (int*)malloc(DEFAULT_MEMORY_ALLOCATION_SIZE * sizeof(int));
    int* temp_alloc;
    int max_card_values_count = DEFAULT_MEMORY_ALLOCATION_SIZE;
    int green_value=-1;
    int cards_count[MAX_COLOR_SIZE] = { 0,0,0,0,0,0,0 }; //green blue red violet yellow white black
    fgets(buffor, MAX_LINE_LENGTH, p); //active player line
    current_game_data->active_player = ReadTillNumber(buffor)-1;
    fgets(buffor, MAX_LINE_LENGTH, p); //players_count line
    user_input.players_count = ReadTillNumber(buffor);
    fgets(buffor, MAX_LINE_LENGTH, p); //threshold line
    current_game_data->pile_threshold = ReadTillNumber(buffor);
    player_x **players_table = InitializePlayers(user_input.players_count);
    //CARDS ON PLAYERS
    for (int i = 0; i < 2 * user_input.players_count; i++) {
        fgets(buffor, MAX_LINE_LENGTH, p);
        card_value = strtok(buffor, whitespace);
        for (int j = 0; j < 3; j++) { //because we need to reach to cards, function ReadTillNumber ignores first element
            card_value = strtok(NULL, whitespace);
        }
        while (card_value != NULL) {
            card_value = strtok(NULL, whitespace);
            card_color = strtok(NULL, whitespace);
            if (card_color == NULL)
                break;
            if (atoi(card_color) == green) //we need green_value for user_input
                green_value = atoi(card_color);
            if (CardColorChar(card_color) == blue) { //special condition for blue cards, because we save values of all cards to user_input->values later
                if (cards_count[blue] + 1 >= max_card_values_count) { //i don't use EasyTwiceArrayEnlargement because this is int, not card_type
                    temp_alloc = (int*)malloc(2*max_card_values_count * sizeof(int));
                    if (temp_alloc != NULL) {
                        memcpy(temp_alloc, card_values, max_card_values_count * sizeof(int));
                        free(card_values);
                        card_values = temp_alloc;
                        max_card_values_count *= 2;
                    }
                }
                *(card_values + cards_count[blue]) = atoi(card_value);
            }
            if (i % 2) { //if i%2 == 1 then current input line presents player deck cards
                if ((*(players_table + i / 2))->deck_cards_count >= (*(players_table + i / 2))->max_deck_cards_count) {
                    (*(players_table + i / 2))->player_deck_cards = EasyTwiceArrayEnlargement((*(players_table + i / 2))->player_deck_cards, (*(players_table + i / 2))->max_deck_cards_count);
                    (*(players_table + i / 2))->max_deck_cards_count *= 2;
                }
                (*(players_table + i / 2))->player_deck_cards[(*(players_table + i / 2))->deck_cards_count].value = atoi(card_value);
                (*(players_table + i / 2))->player_deck_cards[(*(players_table + i / 2))->deck_cards_count].color = CardColorChar(card_color);
                (*(players_table + i / 2))->deck_cards_count += 1;
            }
            else { // i%2 == 0 means hand cards
                if ((*(players_table + i / 2))->hand_cards_count >= (*(players_table + i / 2))->max_hand_cards_count) {
                    (*(players_table + i / 2))->player_hand_cards = EasyTwiceArrayEnlargement((*(players_table + i / 2))->player_hand_cards, (*(players_table + i / 2))->max_hand_cards_count);
                    (*(players_table + i / 2))->max_hand_cards_count *= 2;
                }
                (*(players_table + i / 2))->player_hand_cards[(*(players_table + i / 2))->hand_cards_count].value = atoi(card_value);
                (*(players_table + i / 2))->player_hand_cards[(*(players_table + i / 2))->hand_cards_count].color = CardColorChar(card_color);
                (*(players_table + i / 2))->hand_cards_count += 1;
            }
            cards_count[CardColorChar(card_color)]++;
        }
    }
    
    user_input.cauldron_count = CountPiles(cards_count);
    pile **piles = InitializePiles(user_input.cauldron_count);
    //CARDS ON PILES
    for (int i = 0; i < user_input.cauldron_count; i++) {
        fgets(buffor, MAX_LINE_LENGTH, p);
        card_value = strtok(buffor, whitespace);
        for (int j = 0; j < 2; j++) {
            card_value = strtok(NULL, whitespace);
        }
        while (card_value != NULL) {
            card_value = strtok(NULL, whitespace);
            card_color = strtok(NULL, whitespace);
            if (card_color == NULL)
                break;
            if (CardColorChar(card_color) == blue) {
                if (cards_count[blue] + 1 >= max_card_values_count) {
                    temp_alloc = (int*)malloc(2 * max_card_values_count * sizeof(int));
                    if (temp_alloc != NULL) {
                        memcpy(temp_alloc, card_values, max_card_values_count * sizeof(int));
                        free(card_values);
                        card_values = temp_alloc;
                        max_card_values_count *= 2;
                    }
                }
                *(card_values + cards_count[blue]) = atoi(card_value);
            }
            if ((*(piles + i))->cards_amount >= (*(piles + i))->max_card_count) {
                (*(piles + i))->cards = EasyTwiceArrayEnlargement((*(piles + i))->cards, (*(piles + i))->max_card_count);
                (*(piles + i))->max_card_count *= 2;
            }
            //Adding cards to piles
            (*(piles + i))->cards[(*(piles + i))->cards_amount].value = atoi(card_value);
            (*(piles + i))->cards[(*(piles + i))->cards_amount].color = CardColorChar(card_color);
            (*(piles + i))->cards_amount += 1;
        }
    }
    SortAscending(card_values, cards_count[blue]);
    user_input.green_count = cards_count[green];
    user_input.other_cards_count = cards_count[blue];
    user_input.green_value = green_value;//card_values[green][0];
    FreeMemoryAfterPiles(*piles_ptr, user_input);
    FreeMemoryAfterPlayers(*players_table_ptr, user_input);
    *piles_ptr = piles; //changing the pointer given to function
    *players_table_ptr = players_table;
    user_input.values = (int*)malloc(cards_count[blue] * sizeof(int));
    memcpy(user_input.values, card_values, cards_count[blue] * sizeof(int));
    free(card_values);
}


int CheckPlayers(const input_values* user_input, player_x** players_table) {
    int check_with_first = (*(players_table))->hand_cards_count;
    for (int i = 1; i < user_input->players_count; i++) {
        if (abs(check_with_first - (*(players_table + i))->hand_cards_count) > 2) {
            cout << "The number of players cards on hand is wrong\n";
            return True;
        }
    }
    return False;
}


int CheckCauldrons(pile** piles, const game_data* current_game_data, const input_values* user_input) {
    int sum_pile = 0;
    int flag_different_colors = 0;
    int error = 0;
    for (int i = 0; i < user_input->cauldron_count; i++) {
        for (int j = 1; j < (*(piles + i))->cards_amount; j++) {  // V if to colors next to each other are different and none of them is green then it is a failure
            if ((*(piles + i))->cards[j - 1].color != (*(piles + i))->cards[j].color && (*(piles + i))->cards[j - 1].color != green && (*(piles + i))->cards[j].color != green && flag_different_colors==0) {
                cout << "Two different colors were found on the " << i + 1 << " pile\n";
                flag_different_colors = 1;
                error = 1;
            }
        }
        flag_different_colors = 0;
    }
    for (int i = 0; i < user_input->cauldron_count; i++) {
        for (int j = 0; j < (*(piles + i))->cards_amount; j++) {
            sum_pile += (*(piles + i))->cards[j].value;
        }
        if (sum_pile > current_game_data->pile_threshold) {
            cout << "Pile number " << i + 1 << " should explode earlier\n";
            error = 1;
        }
        sum_pile = 0;
    }
    return error;
}


int CheckGameStatus(const input_values* user_input, player_x** players_table, const game_data* current_game_data, pile** piles) {
    int error_count = 0;
    error_count += CheckPlayers(user_input, players_table);
    error_count += CheckCauldrons(piles, current_game_data, user_input);
    if (error_count == 0) {
        //cout << "Current state of the game is ok\n";
        return False;
    }
    return True;
}


int WhichCauldronPossible(pile** piles, int color, const input_values* user_input) {
    if (color == green) { //color green means first cauldron
        return 0;
    }
    for (int i = 0; i < user_input->cauldron_count; i++) {
        for (int j=0; j < (*(piles + i))->cards_amount; j++) { //First scanning looking for exact color pile
            if ((*(piles + i))->cards[j].color == color)
                return i;
        }
    }
    int only_green = 1;
    for (int i = 0; i < user_input->cauldron_count; i++) {
        if ((*(piles + i))->cards_amount == 0) {
            return i;
        }
        for (int j = 0; j < (*(piles + i))->cards_amount; j++) {
            if ((*(piles + i))->cards[j].color != green)
                only_green = 0;
        }
        if (only_green == 1)
            return i;
    }
    return 0;
}


void DeleteFirstPlayerCard(player_x* player) {
    player->hand_cards_count--;
    for (int i = 0; i < player->hand_cards_count; i++)
        player->player_hand_cards[i] = player->player_hand_cards[i + 1];
}


int EasyMove(const input_values* user_input, player_x** players_table, game_data* current_game_data, pile** piles) {
    int card_value = (*(players_table + current_game_data->active_player))->player_hand_cards[0].value;
    int card_color = (*(players_table + current_game_data->active_player))->player_hand_cards[0].color;
    int cauldron_put = WhichCauldronPossible(piles, card_color, user_input);
    DeleteFirstPlayerCard(players_table[current_game_data->active_player]);
    if ((*(piles + cauldron_put))->cards_amount+1 >= (*(piles + cauldron_put))->max_card_count) {
        (*(piles + cauldron_put))->cards = EasyTwiceArrayEnlargement((*(piles + cauldron_put))->cards, (*(piles + cauldron_put))->max_card_count);
        (*(piles + cauldron_put))->max_card_count *= 2;
    }
    (*(piles + cauldron_put))->cards[(*(piles + cauldron_put))->cards_amount].value = card_value;
    (*(piles + cauldron_put))->cards[(*(piles + cauldron_put))->cards_amount].color = card_color;
    (*(piles + cauldron_put))->cards_amount++;
    current_game_data->active_player = (current_game_data->active_player + 1) % (user_input->players_count);
    return cauldron_put;
}


int LastPlayer(const input_values* user_input, game_data* current_game_data) {
    if (current_game_data->active_player == 0) {
        return user_input->players_count - 1;
    }
    return current_game_data->active_player - 1;
}


int CountCauldronValue(pile* current_pile) {
    int sum = 0;
    for (int i = 0; i < current_pile->cards_amount; i++) {
        sum += current_pile->cards[i].value;
    }
    
    return sum;
}


int IfCauldronExplosion(game_data* current_game_data, pile** piles, int last_cauldron) {
    if (CountCauldronValue(piles[last_cauldron]) > current_game_data->pile_threshold) {
        return True;
    }
    return False;
}


void CauldronToPlayer(const input_values* user_input, game_data* current_game_data, pile** piles, player_x** players_table, int last_cauldron) {
    int last_player = LastPlayer(user_input, current_game_data);
    for (int i = 0; i < piles[last_cauldron]->cards_amount; i++) {
        if (players_table[last_player]->deck_cards_count >= players_table[last_player]->max_deck_cards_count) {
            players_table[last_player]->player_deck_cards = EasyTwiceArrayEnlargement(players_table[last_player]->player_deck_cards, players_table[last_player]->max_deck_cards_count);
            players_table[last_player]->max_deck_cards_count *= 2;
        }
        players_table[last_player]->player_deck_cards[players_table[last_player]->deck_cards_count].color = piles[last_cauldron]->cards[i].color;
        players_table[last_player]->player_deck_cards[players_table[last_player]->deck_cards_count].value = piles[last_cauldron]->cards[i].value;
        players_table[last_player]->deck_cards_count++;
    }
    piles[last_cauldron]->cards_amount = 0;
}


int CheckGreens(const input_values* user_input, pile** piles, player_x** players_table) {
    int green_value = -1;
    int green_count = 0;
    for (int i = 0; i < user_input->players_count; i++) {
        for (int j = 0; j < (*(players_table + i))->hand_cards_count; j++) {
            if ((*(players_table + i))->player_hand_cards[j].color == green) {
                if (green_value < 0)
                    green_value = (*(players_table + i))->player_hand_cards[j].value;
                green_count++;
                if (green_value != (*(players_table + i))->player_hand_cards[j].value) {
                    cout << "Different green cards values occured\n";
                    return True;
                }
            }
        }
        for (int j = 0; j < (*(players_table + i))->deck_cards_count; j++) {
            if ((*(players_table + i))->player_deck_cards[j].color == green) {
                if (green_value < 0)
                    green_value = (*(players_table + i))->player_deck_cards[j].value;
                green_count++;
                if (green_value != (*(players_table + i))->player_deck_cards[j].value) {
                    cout << "Different green cards values occured\n";
                    return True;
                }
            }
        }
    }
    for (int i = 0; i < user_input->cauldron_count; i++) {
        for (int j = 0; j < (*(piles + i))->cards_amount; j++) {
            if ((*(piles + i))->cards[j].color == green) {
                if (green_value < 0)
                    green_value = (*(piles + i))->cards[j].value;
                green_count++;
                if (green_value != (*(piles + i))->cards[j].value) {
                    cout << "Different green cards values occured\n";
                    return True;
                }
            }
        }
    }
    if (green_count) {
        //cout << "Found " << green_count << " green cards all with " << green_value << " value\n";
        return False;
    }
    else
        cout << "Green cards does not exist\n";
    return True;
}


void FreeMemoryForCheck(int** card_values_colors, int* card_values_positions, input_values* user_input) {
    free(card_values_positions);
    for (int i = 0; i < user_input->cauldron_count + 1; i++)
        free(*(card_values_colors + i));
    free(card_values_colors);
}


int CheckColorValidity(player_x** players_table, input_values* user_input, pile** piles) {
    int** card_values_colors = (int**)malloc((user_input->cauldron_count+1) * sizeof(int*));
    int* card_values_positions = (int*)malloc((user_input->cauldron_count+1) * sizeof(int));
    if (card_values_positions != NULL) {
        for (int i = 0; i < user_input->cauldron_count+1; i++)
            *(card_values_positions + i) = 0;
    }
    if (card_values_colors != NULL && card_values_positions != NULL) {
        for (int i = 0; i < user_input->cauldron_count+1; i++) {
            *(card_values_colors+i) = (int*)malloc(MAX_LINE_LENGTH * sizeof(int));
            if (*(card_values_colors+i) == NULL)
                return True;
        }
    }
    else
        return True;
    int card_color, card_value;    //these are to simplify for end-reader
    for (int i = 0; i < user_input->players_count; i++) {
        for (int j = 0; j < (*(players_table + i))->hand_cards_count; j++) {
            card_color = (*(players_table + i))->player_hand_cards[j].color;
            card_value = (*(players_table + i))->player_hand_cards[j].value;
            card_values_colors[card_color][card_values_positions[card_color]] = card_value;
            card_values_positions[card_color]++;
        }
        for (int j = 0; j < (*(players_table + i))->deck_cards_count; j++) {
            card_color = (*(players_table + i))->player_deck_cards[j].color;
            card_value = (*(players_table + i))->player_deck_cards[j].value;
            card_values_colors[card_color][card_values_positions[card_color]] = card_value;
            card_values_positions[card_color]++;
        }
    }
    for (int i = 0; i < user_input->cauldron_count; i++) {
        for (int j = 0; j < piles[i]->cards_amount; j++) {
            card_color = (*(piles + i))->cards[j].color;
            card_value = (*(piles + i))->cards[j].value;
            card_values_colors[card_color][card_values_positions[card_color]] = card_value;
            card_values_positions[card_color]++;
        }
    }
    for (int i = 0; i < user_input->cauldron_count + 1; i++) {
        SortAscending(*(card_values_colors+i), *(card_values_positions+i));
    }
    for (int i = 2; i < user_input->cauldron_count+1; i++) {
        if (memcmp(*(card_values_colors+i-1), *(card_values_colors + i), (user_input->other_cards_count) * sizeof(int)) != 0) {
            cout << "The values of cards of all colors are not identical:\n";
            for (int j = 1; j < user_input->cauldron_count + 1; j++) {
                cout << SayMyName(j);
                cout << " cards values: ";
                for (int k = 0; k < *(card_values_positions+j); k++) {
                    cout << *((*(card_values_colors+j))+k) << " ";
                }
                cout << "\n";
            }
            free(card_values_positions);
            for (int z = 0; z < user_input->cauldron_count; z++)
                free(*(card_values_colors + z));
            free(card_values_colors);
            return True;
        }
    }
    
    //cout << "The values of cards of all colors are identical:\n";
    //for (int k = 0; k < *(card_values_positions+blue); k++) {
    //    cout << card_values_colors[blue][k] << " ";
    //}
    //cout << "\n";
    FreeMemoryForCheck(card_values_colors, card_values_positions, user_input);
    return False;
}


int ShouldEndRound(player_x** players_table, const input_values* user_input) {
    for (int i = 0; i < user_input->players_count; i++) {
        if ((*(players_table + i))->hand_cards_count != 0)
            return False;
    }
    return True;
}


void CountColorDeckCards(player_x* player, const input_values* user_input, int* colors_cards_counts) {
    for (int i = 0; i < user_input->cauldron_count + 1; i++)
        *(colors_cards_counts + i) = 0;
    for (int i = 0; i < player->deck_cards_count; i++) {
        (*(colors_cards_counts + player->player_deck_cards[i].color))++;
    }
}


int MaxInArray(int* array, int length) {
    int max = *array;
    for (int i = 0; i < length; i++) {
        if ((*(array + i)) > max && i!=green)
            max = (*(array + i));
    }
    return max;
}


int ScoreOfPlayer(player_x *player, int player_number, int* resistance) {
    int score = 0;
    for (int i = 0; i < player->deck_cards_count; i++) {
        if (player->player_deck_cards[i].color == green) {
            score -= 2;
        }
        else if (resistance[player->player_deck_cards[i].color] == player_number) {
            continue;
        }
        else {
            score -= 1;
        }
    }
    return score;
}


int* WhoisResistant(player_x** players_table, const input_values* user_input) {
    int* max_colors_counts = (int*)malloc((user_input->cauldron_count + 1) * sizeof(int));
    for (int i = 0; i < user_input->cauldron_count + 1; i++) {
        *(max_colors_counts + i) = 0;
    }
    int* resistance = (int*)malloc((user_input->cauldron_count + 1) * sizeof(int));
    int* colors_cards_count = (int*)malloc((user_input->cauldron_count + 1) * sizeof(int));
    int* memory = (int*)malloc((user_input->cauldron_count + 1) * sizeof(int));
    for (int i = 0; i < user_input->cauldron_count + 1; i++) {
        *(resistance + i) = NOBODY;
        *(memory + i) = NOBODY;
    }
    for (int i = 0; i < user_input->players_count; i++) {
        CountColorDeckCards(*(players_table+i), user_input, colors_cards_count);
        for (int j = 0; j < user_input->cauldron_count + 1; j++) {
            if (*(colors_cards_count+j) > *(max_colors_counts+j) && *(colors_cards_count+j) > *(memory+j)) {
                *(max_colors_counts+j) = *(colors_cards_count+j);
                *(resistance+j) = i;
            }
            else if (*(colors_cards_count+j) == *(max_colors_counts+j) && *(max_colors_counts+j) > 0) {
                *(memory+j) = *(max_colors_counts+j);
                *(max_colors_counts+j) = NOBODY;
                *(resistance+j) = NOBODY;
            }
        }
    }
    free(memory);
    free(max_colors_counts);
    free(colors_cards_count);
    return resistance;
}


void EndOfTurn(player_x** players_table, const input_values* user_input, int* all_scores) {
    int *scores = (int*)malloc(user_input->players_count * sizeof(int));
    int* resistance = WhoisResistant(players_table, user_input);
    if (scores != NULL) {
        for (int i = 0; i < user_input->players_count; i++) {
            (*(scores + i)) = abs(ScoreOfPlayer(*(players_table + i), i, resistance));
        }
        for (int i = 0; i < user_input->cauldron_count + 1; i++) {
            if (*(resistance + i) > NOBODY && i != green) {
                cout << "Na kolor ";
                cout << SayMyName(i);
                cout << " odporny jest gracz " << *(resistance + i) + 1 << endl;
            }
        }
        for (int i = 0; i < user_input->players_count; i++) {
            cout << "Wynik gracza " << i + 1 << " = " << (*(scores + i)) << endl;
            *(all_scores + i) += (*(scores + i));
        }
    }
    free(resistance);
    free(scores);
}


int CheckAllCardsCount(player_x** players_table, const input_values* user_input, pile** piles_table) {
    int* colors_counts = (int*)malloc((user_input->cauldron_count + 1) * sizeof(int));
    for (int i = 0; i < user_input->cauldron_count + 1; i++) {
        *(colors_counts+i) = 0;
    }
    for (int i = 0; i < user_input->players_count; i++) {
        for (int j = 0; j < (*(players_table + i))->deck_cards_count; j++)
            *(colors_counts + (*(players_table + i))->player_deck_cards[j].color) += 1;
        for (int j = 0; j < (*(players_table + i))->hand_cards_count; j++)
            *(colors_counts + (*(players_table + i))->player_hand_cards[j].color) += 1;
    }
    for (int i = 0; i < user_input->cauldron_count; i++) {
        for (int j=0; j< (*(piles_table+i))->cards_amount;j++)
            *(colors_counts + (*(piles_table + i))->cards[j].color) += 1;
    }
    for (int i = 2; i < user_input->cauldron_count + 1; i++) {
        if (*(colors_counts + i - 1) != *(colors_counts + i)) {
            cout << "At least two colors with different number of cards were found:\n";
            for (int j = 1; j < user_input->cauldron_count + 1; j++) {
                cout << SayMyName(j) << " cards are " << *(colors_counts + j) << endl;
            }
            return True;
        }
    }
    // cout << "The number cards of all colors is equal: " << *(colors_counts + 1) << endl;
    return False;
}


void ShuffleCards(card_type* all_cards, input_values user_input) {
    card_type temp_card;
    int with, amount = user_input.green_count + user_input.cauldron_count * user_input.other_cards_count;
    for (int j = 0; j < amount; j++) {
        with = rand() % amount;
        temp_card = (*(all_cards + j));
        (*(all_cards + j)) = (*(all_cards + with));
        (*(all_cards + with)) = temp_card;
    }
}


int LowestCauldronPossible(pile** piles, int color, const input_values* user_input) {
    int* cauldron_values = (int*)malloc(user_input->cauldron_count * sizeof(int));
    if (cauldron_values != NULL) {
        for (int i = 0; i < user_input->cauldron_count; i++) {
            *(cauldron_values + i) = CountCauldronValue(*(piles + i));
        }
        int min = *cauldron_values, min_index = 0;
        for (int i = 0; i < user_input->cauldron_count; i++) {
            if (*(cauldron_values + i) < min) {
                min = *(cauldron_values + i);
                min_index = i;
            }
        }
        return (min_index);
    }
    cout << "An error has occured";
    return False; 
}


int LowestMove(const input_values* user_input, player_x** players_table, game_data* current_game_data, pile** piles) {
    int card_value = (*(players_table + current_game_data->active_player))->player_hand_cards[0].value;
    int card_color = (*(players_table + current_game_data->active_player))->player_hand_cards[0].color; 
    int flag = 0, stop = False, card_number = 0;
    for (int i = 0; i < user_input->other_cards_count && stop == False; i++) {
        card_value = user_input->values[i];
        if (card_value > user_input->green_value && flag == False) {
            card_value = user_input->green_value;
            flag = True;
            i--;
        }
        for (int j = 0; j < (*(players_table + current_game_data->active_player))->hand_cards_count && stop == False; j++) {
            if ((*(players_table + current_game_data->active_player))->player_hand_cards[j].value == card_value) {
                card_color = (*(players_table + current_game_data->active_player))->player_hand_cards[j].color;
                stop = True;
                card_number = j;
            }
        }
    }
    int cauldron_put = WhichCauldronPossible(piles, card_color, user_input);
    for (int i = card_number; i < (*(players_table + current_game_data->active_player))->hand_cards_count - 1; i++)
        (*(players_table + current_game_data->active_player))->player_hand_cards[i] = (*(players_table + current_game_data->active_player))->player_hand_cards[i + 1];
    if ((*(piles + cauldron_put))->cards_amount + 1 >= (*(piles + cauldron_put))->max_card_count) {
        (*(piles + cauldron_put))->cards = EasyTwiceArrayEnlargement((*(piles + cauldron_put))->cards, (*(piles + cauldron_put))->max_card_count);
        (*(piles + cauldron_put))->max_card_count *= 2;
    }
    (*(piles + cauldron_put))->cards[(*(piles + cauldron_put))->cards_amount].value = card_value;
    (*(piles + cauldron_put))->cards[(*(piles + cauldron_put))->cards_amount].color = card_color;
    (*(piles + cauldron_put))->cards_amount++;
    (*(players_table + current_game_data->active_player))->hand_cards_count--;
    current_game_data->active_player = (current_game_data->active_player + 1) % (user_input->players_count);
    return cauldron_put;
}
void PrintScores(int* scores, const input_values user_input) {
    for (int i = 0; i < user_input.players_count; i++) {
        cout << "Wynik calkowity gracza " << i + 1 << " = " << (*(scores + i)) << endl;
    }
}

void EasyOrNotGame() {
    input_values user_input;
    game_data current_game_data;
    UserInput(&user_input, &current_game_data);
    card_type* all_cards;
    player_x** players_table;
    pile** piles_table;
    int round_end = False, last_cauldron_number;
    int error_count = 0;
    int* all_results = (int*)malloc(user_input.players_count * sizeof(int));
    if (all_results == NULL) {
        cout << "An error allocating memmory has occured. Please try again on a different platform.";
        return;
    } 
    for (int i = 0; i < user_input.players_count; i++) {
        *(all_results + i) = 0;
    }
    for (int player = 0;player < user_input.players_count ;player++){ //start different player, rounds count equal to players_number, starts one on the left to one that shuffled cards;
        current_game_data.active_player = (player+1)%user_input.players_count;
        all_cards = CardsGenerate(user_input);
        players_table = InitializePlayers(user_input.players_count);
        ShuffleCards(all_cards, user_input);
        CardDealer(players_table, user_input, all_cards);
        piles_table = InitializePiles(user_input.cauldron_count);
        round_end = False;
        error_count = 0;
        Print2(players_table, user_input, current_game_data, piles_table);
        while (round_end != True) {
            cout << "Now playing player number " << current_game_data.active_player + 1 << endl;
            ReadGameValues(user_input, &current_game_data, &players_table, &piles_table);
            error_count += CheckGreens(&user_input, piles_table, players_table);
            error_count += CheckAllCardsCount(players_table, &user_input, piles_table);
            error_count += CheckColorValidity(players_table, &user_input, piles_table);
            error_count += CheckGameStatus(&user_input, players_table, &current_game_data, piles_table);
            if (error_count) {
                cout << "An error has occurred. Please contact the programmer.";
                return;
            }
            last_cauldron_number = LowestMove(&user_input, players_table, &current_game_data, piles_table);
            if (IfCauldronExplosion(&current_game_data, piles_table, last_cauldron_number)) {
                CauldronToPlayer(&user_input, &current_game_data, piles_table, players_table, last_cauldron_number);
            }
            round_end = ShouldEndRound(players_table, &user_input);
            Print2(players_table, user_input, current_game_data, piles_table);
            if (round_end == True) {
                cout << "Runda numer " << player + 1 << " zostala zakonczona! Oto jej wyniki:\n";
                EndOfTurn(players_table, &user_input, all_results);
                FreeMemoryAfterPlayers(players_table, user_input);
                FreeMemoryAfterPiles(piles_table, user_input);
                FreeMemoryAfterCards(all_cards);
            }
            current_game_data.active_player %= user_input.players_count;
        }
    }
    PrintScores(all_results, user_input);
    free(user_input.values);
}


int main() {
    srand(time(NULL));
    EasyOrNotGame();
    return 0;
}
/* 
PROGRAM REQUIRES INPUT //PLAYERS_COUNT PILE_COUNT GREEN_CARDS_COUNT GREEN_CARDS_VALUE OTHER_CARDS_COUNT PILE_THRESHOLD
3 6 15 9 25 80 
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 22 23 21 24 25 //OTHER_CARDS_VALUES
*/