#pragma once
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <drako/drako.h>

static const char* level0_greeting = "Heyo, Im Drako your personal adventuring companion.\n\n"
                                     "Alright, listen closely adventurer, because this is how it works. The dungeon is split into\n"
                                     "three levels, each more treacherous than the last. But there’s a method to it, a way out.\n"
                                     "To move on from one level to the next, you’ll need to retrieve the current levels key.\n\n"
                                     "It’s not hidden in some chest or tucked away in a secret room; it’s embedded in the very\n"
                                     "fabric of the dungeon’s riddles. You’ll find the key in the form of a block of text\n"
                                     "wrapped in those < > symbols. You’ll need to read carefully, because the answer to enter\n"
                                     "the dungeon and unlock the path ahead is <firststeps>. Remember, this isn’t just about\n"
                                     "brute force—it’s about wit, patience, and learning how the dungeon thinks. Get the key, and\n"
                                     "you'll move forward. Fail, and you’ll be stuck here forever.\n";
static inline void level0() {
    // make sure they dont call this level after entering the dungeon
    if (drako.currentLevel != 0) {
        printf("Your adventure has already begun! There's no turning back now.\n");
        return;
    }
    // display level
    printf("\n----- LEVEL 0 -----\n");

    // display greeting
    printf("\n%s\n", level0_greeting);
}

static inline bool level0_cast(char* cmd) {
    // get first token of command
    char *token, *saveptr;
    token = strtok_r(cmd, " ", &saveptr);

    // check if command is "cast"
    if (strcmp(token, "cast") == 0) {
        // get second token (if it exists)
        token = strtok_r(NULL, " ", &saveptr);
        if (token == NULL) {
            printf("As you begin to cast your spell, you realize you never decided on which one to cast.\n");
            printf("Make sure to specify a spell to cast when calling \"cast\"!\n");
            return false;
        }
        else if (strcmp(token, "<firststeps>") == 0) {
            // display correct key message
            printf("\nThe door, etched with time-worn runes and steeped in forgotten enchantments, shudders as\nshimmering threads of magic twist around it. Then, sudden silence.\n");
            printf("You hear the distinctive \"click\" of a lock as the door slowly opens.\n");
            printf("Realizing that the first of many challenges has been completed...\nyou walk through the now open door.\n");
            printf("\n----- LEVEL 0: COMPLETE -----\n\n");
            // increment level
            drako.currentLevel++;
            return true;
        }
        else {
            printf("You begin to weave the fibers of the arcane around you.\nAs you complete your spell, it fizzles.\nNothing else happens...\n");
            return false;
        }
    }
    else {
        printf("You do not know that spell.\n");
        return false;
    }
}

