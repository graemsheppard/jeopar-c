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
// Returns number of tokens
int tokenize(char *buffer, char ***tokens_ptr)
{
    // Initialize empty token and counter
    char *token = NULL;
    char **tokens = NULL;
    int count = 0;

    // First pass: count spaces
    for (int i = 0; buffer[i] != '\0'; i++) {
        char c = buffer[i];
        if (c == ' ') count++;
    }

    // Allocate memory for tokens
    *tokens_ptr = calloc(count + 2, sizeof(char*));
    tokens = *tokens_ptr;

    // Second pass: create tokens
    token = strtok(buffer, " \n");
    for (int i=0; token != NULL; i++)
    {
        tokens[i] = token;
        token = strtok(NULL, " \n");
    }

    return ++count;
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


int main(int argc, char *argv[])
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
		player p;
        printf("Enter Player %d Name: ", i+1);
        scanf("%[^\n]%*c", p.name);
        p.score = 0;
		players[i] = p;
        printf("\n");
    }


    // Perform an infinite loop getting command input from users until game ends
    game_over = 0;
    char str[4][BUFFER_LEN] = {{0}};

    while (game_over == 0)
    {
    	display_categories();

    	char *token;
    	char *name = malloc(MAX_LEN * sizeof(char));
    	char *prompt;
    	char **tokens = NULL;
    	int count;

    	while (!player_exists(players, NUM_PLAYERS, name)) {
    		printf("Enter a valid player's name\n");
    		fgets(buffer, BUFFER_LEN, stdin);
    		count = tokenize(buffer, &tokens);
            if (tokens[0] == NULL) continue;
    		if (count >= 1) {
    			strcpy(name, tokens[0]);
    		}
    	}

    	count = 0;
    	char *category = malloc(MAX_LEN * sizeof(char));;
    	char *value = malloc(MAX_LEN * sizeof(char));;

    	// Loops until user enters a category and value of a non-answered question
    	while (!valid_input(category, value)) {
    		printf("%s\n", "Enter Category and Value");
    		fgets(buffer, BUFFER_LEN, stdin);
    		count = tokenize(buffer, &tokens);
    		if (count >= 2) {
    			strcpy(category, tokens[0]); // Copy the values from tokens
    			strcpy(value, tokens[1]);
    		}
    	}
    	display_question(category, atoi(value));
    	memset(buffer, 0, BUFFER_LEN);

    	count = 0;
      while (count == 0) {
                fgets(buffer, BUFFER_LEN, stdin);
                count = tokenize(buffer, &tokens);
                if ((strcmp(tokens[0],"what") == 0)&&(strcmp(tokens[1],"is") == 0))
                {
                        count=1;
                }
                else
                {
                        printf("%s\n"," You need to put what is before the answer");
                                count=0;
                }
        }

    	question_count--;

        bool is_valid = valid_answer(category, atoi(value), tokens[0]);
    	if (is_valid == true){
    		printf("%s\n", "Correct Answer!");
    		update_score(players, NUM_PLAYERS, name, atoi(value));
    	}else {
    		printf("%s\n", "Sorry incorrect answer");
    		display_answer(str[0], atoi(str[1]));
    	}

        free(name);
        free(category);
        free(value);

    	if (question_count <=0){
    		game_over =1;
    	}
    }
    show_results(players, NUM_PLAYERS);
    return EXIT_SUCCESS;
}
