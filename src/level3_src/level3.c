// Idea: Create a maze in STDOUT
// gcc {filename}.c -o {outputname}
// ./{filename}

#include <stdio.h>
#include <string.h>
#include <drako/drako.h>
#include <level3/level.h>

// setup user [row:2, col:33]
int userRow = 1;
int userCol = 32;

bool level3_main(char* cmd) {
    // get first token of command
    char *token, *saveptr;
    token = strtok_r(cmd, " ", &saveptr);

    // check if we reached the center
    if ((userRow == 11) && (userCol == 32)) {
        printf("\n"
            "You come across a fellow adventurer who looks beaten and rugged.\n"
            "Obviously he has been here for a while. \"I... I thought I was\n"
            "done for,\" the weary traveler stammers, their voice thich with\n"
            "relief as they clutch your arm. \"Days--maybe weeks--I have been\n"
            "wandering these cursed halls, and just when I had given up hope,\n"
            "you appear like a phantom of mercy.\" They gesture to the heavy\n"
            "chest beside them, its gilded edges glided edges glinting in the\n"
            "dim torchlight. \"Please, take it. Every coin, every gem--yours.\n"
            "I owe you my life\" As you pry open the lid, your eyes widen.\n"
            "Amidst the glittering wealth lies the true prize--the reason you\n"
            "ventured into this wretched labyrinth in the first place.\n"
            "Nestled atop a velvet cloth, shimmering with an otherwordly\n"
            "glow, rests the coveted GOOG TOKEN.\n"
            "\n----- LEVEL 3: COMPLETE -----\n\n"
            "\n----- YOU WIN! -----\n\n"
            "\nShow the GOOG TOKEN to your cyberstorm professor to earn credit.\n"
        );
        // increment level
        drako.currentLevel++;
        return true;
    }

    // check if command is "cast"
    if (strcmp(token, "cast") == 0) {
        // get second token (if it exists)
        token = strtok_r(NULL, " ", &saveptr);
        if (token == NULL) {
            printf("As you begin to cast your spell, you realize you never decided on what one to cast.\n");
            printf("Make sure to specify a spell to cast when calling \"cast\"!\n");
            return false;
        }
        else if (strcmp(token, "<insight>") == 0) {
            // hit for user to look at board
            printf(
                "Try to \"walk\" with the w,a,s,d keys.\n"
                "Try to reach the center of the maze.\n"
            );

            // level not completed, so return false
            return false;
        }
        else {
            printf("\n"
                "The air shimmers as a vortex of light begins to coalesce—a delicate swirl of opalescent hues\n"
                "that hints at a gateway into the unknown. Its intricate patterns, woven from threads of\n"
                "iridescent magic, begin to fracture and unravel.\n"
                "In the dark silence that follows, only you remain...\n"
            );
            return false;
        }
    }
    else if (strcmp(token,"pass") == 0) {
        // do nothing
        return false;
    }
    else if (strcmp(token, "w") == 0) {
        // up
        // printf("walk up\n");
        if (!((userRow == 1) || (MAZE[userRow-1][userCol] == '#'))) {
            userRow -= 1;
        }
        printMaze(userRow, userCol);
        return false;
    }
    else if (strcmp(token, "a") == 0) {
        // left
        // printf("walk left\n");
        if (!((userCol == 0) || (MAZE[userRow][userCol-1] == '#'))) {
            userCol -= 1;
        }
        printMaze(userRow, userCol);
        return false;
    }
    else if (strcmp(token, "s") == 0) {
        // down
        // printf("walk down\n");
        if (!((userRow >= (MAZE_ROWS - 1)) || (MAZE[userRow+1][userCol] == '#'))) {
            userRow += 1;
        }
        printMaze(userRow, userCol);
        return false;
    }
    else if (strcmp(token, "d") == 0) {
        // up
        // printf("walk up\n");
        if (!((userCol >= (MAZE_COLS - 1)) || (MAZE[userRow][userCol+1] == '#'))) {
            userCol += 1;
        }
        printMaze(userRow, userCol);
        return false;
    }
    else {
        printf("You do not know that spell.\n");
        return false;
    }
}