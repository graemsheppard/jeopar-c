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
#include "questions.h"

// Initializes the array of questions for the game
void initialize_game(void)
{
    // initialize each question struct and assign it to the question array
    FILE *fp = fopen("questions.dat", "r");

    // ensure the file can be opened
    if (fp == NULL) {
        printf("questions.dat not found or is inaccessible...\n");
        exit(0);
    }
    char cur_line[MAX_LEN];
    int question_idx = 0;
    int line_idx = 0;
    question cur_question;
    while(fgets(cur_line, MAX_LEN, fp) != NULL) {
        // ignore newlines
        if (cur_line[0] == '\n')
            continue;

        // allow comments beginning with '#'
        if (cur_line[0] == '#')
            continue;

        // find the start of a question
        if (cur_line[0] == '~') {
            line_idx = 0;
            continue;
        }
        int actual_length = strcspn(cur_line, "\n");
        cur_line[actual_length] = 0;

        switch (line_idx) {
            case 0: // category line
                // ensure the category is valid
                for (int i = 0; i < NUM_CATEGORIES; i++) {
                    if (strcmp(categories[i], cur_line) == 0) {
                        strcpy(cur_question.category, categories[i]);
                        break;
                    }
                }
                if (cur_question.category[0] == '\0')
                    printf("Unknown category: %s\n", cur_line);
                break;
            case 1:
                // technically this is the answer but im considering it as the question
                strcpy(cur_question.question, cur_line);
                break;
            case 2:
                strcpy(cur_question.answer, cur_line);
                break;
            case 3:
                cur_question.value = atoi(cur_line);
                if (cur_question.value == 0)
                    printf("Could not convert %s to integer\n", cur_line);
                break;
        }
        line_idx++;

        // ensure the question is complete and add to the array
        if (line_idx == 4 && validate_question(cur_question) == true) {
            questions[question_idx++] = cur_question;
        }

    }

    for (int i = 0; i < NUM_QUESTIONS; i++) {
        printf("CAT: %s\n", questions[i].category);
        printf("QUE: %s\n", questions[i].question);
        printf("ANS: %s\n", questions[i].answer);
        printf("VAL: %d\n\n", questions[i].value);
    }
    fclose(fp);
}

// Check if a question has all required fields
bool validate_question(question q) {
    if (q.category[0] != '\0' && q.question[0] != '\0' && q.answer[0] != '\0' && q.value != 0) {
        return true;
    }
    return false;
}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
    // print categories and dollar values for each unanswered question in questions array
}

// Displays the question for the category and dollar value
void display_question(char *category, int value)
{

}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
    // Look into string comparison functions
    return false;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
    // lookup the question and see if it's already been marked as answered
    return false;
}
