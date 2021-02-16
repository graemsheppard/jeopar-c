/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define NUM_PLAYERS 4

// Put global environment variables here
int game_over;
int question_count = NUM_QUESTIONS;


// Processes the answer from the user containing what is or who is and tokenizes it to retrieve the answer.
void tokenize(char *input, char **tokens){
	char *ans = strrchr (input, ' ');
	if (ans && *(ans+1)){
		return &ans+1;
	}
}

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(player *players, int num_players){
	player tmp; 
	for(int i=0; i<num_players; i++){
		for(int j=0; j<num_players; j++){
			if (players[j].score > players[i].score){
				tmp = players[j];
				players[i] = players[j];
				players[j] = tmp;
			}
		}
	}

	for (int i=0; i<num_players; i++){
		printf("%s: $%d\n", players[i].name, players[i].score);		
	}
}


int main()
{
    // An array of 4 players, may need to be a pointer if you want it set dynamically
    player players[NUM_PLAYERS];
    
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };

    // Display the game introduction and initialize the questions
    initialize_game();

    // Prompt for players names
    // initialize each of the players in the array
     for(int i=0; i< NUM_PLAYERS; i++){
        printf("%s %d %s: " "Enter Player", i+1, " name");
        scanf("%[^\n]%*c", players[i].name);
        players[i].score =0;
        printf("\n");
    }


    // Perform an infinite loop getting command input from users until game ends
    game_over = 0;
    char str[4][BUFFER_LEN] = {{0}};

    while (game_over ==0)
    {
	char *token;
	char *name;
	char *prompt;

	display_categories();

	printf("%s\n", "Enter Players Name");
	fgets(buffer, BUFFER_LEN, stdin);
	name = strtok(buffer, "\n");

	if (player_exists(players, NUM_PLAYERS, name) == true){
	
		printf("%s\n", "Enter Category and Value");
		fgets(buffer, BUFFER_LEN, stdin);

		token = strtok(buffer, "\n");
		prompt = strtok(token, " ");
		strcpy(str[0], prompt);
		prompt = strtok(NULL, " ");
		strcpy(str[1], prompt);
		token = NULL;

		display_question(str[0], atoi(str[1]));

		memset(buffer, 0, BUFFER_LEN);
		fgets(buffer, BUFFER_LEN, stdin);

		token = strtok(buffer, "\n");
                prompt = strtok(NULL, "");
                strcpy(str[2], prompt);
                token = NULL;
	
		question_count--; 

		if (valid_answer(str[0], atoi(str[1]), str[2]) == true){
			printf("%s\n", "Correct Answer!");
			update_score(players, NUM_PLAYERS, name, atoi(str[1]));
		}else {
			printf("%s\n", "Sorry incorrect answer");
			display_answer(str[0], atoi(str[1]));
		}

		
	}else{
		printf("%s\n", "Name not valid Enter Players name");
	}
	
	if (question_count <=0){
		game_over =1;
	}
    }
    show_results(players, NUM_PLAYERS);
    return EXIT_SUCCESS;
}
