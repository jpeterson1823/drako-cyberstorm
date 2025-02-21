// Idea: Create a maze in STDOUT
// gcc {filename}.c -o {outputname}
// ./{filename}

#ifndef __drako_level3
#define __drako_level3

#include <stdio.h>
#include <drako/drako.h>

#define MAZE_ROWS 25
#define MAZE_COLS 69

static inline void level3_greeting() {
    printf("As you navigate through the darkness, you notice a path into\n");
    printf("an otherwise impenetrable wall. You are certain what you seek lies\n");
    printf("beyond it. Try navigating the maze by typing the w,a,s,d keys.\n");
    printf("and then press enter.\n");
}

static inline void level3() {
    // display level
    printf("\n----- LEVEL 3 -----\n");
    level3_greeting();
}

static inline void clear(void) {
    while (getchar() != '\n');
}

bool level3_main();

#endif