#ifndef __drako_level1
#define __drako_level1

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <drako/drako.h>

#define _LEVEL1_DISP_DELAY 2000

static inline void level1_greeting() {
    printf("Entering this dark room, you notice there is not much present.\n");
    printf("The only thing in this room is a small desk, upon which rests a single piece of paper.\n");
    printf("The paper reads:\n");

    printf("\n"
        "Mimgbecn 7al,"
        "Ewl etg nvzlz lppcmpg alp sliaty M rd, altrr atio xst zgpca sq shqa hasyt hro hvqpiomyv\n"
        "hrnxlre. Ioi edygsapksi mptrrich hgcdzw hpspd rhvgtk atio wjbiswh ssyv mscvvxetu, eys\n"
        "zllsvad homqi pr hpfw ewhx odu'x bjpxp uliw chxfghp. T wlec ioi oxzxlca hcxw sq lhxpg, hro\n"
        "qlrppal ti, h pzl alfbw – ewbvwe apop p oilgafppa. Xst mpzdy fpcleew tc mdvxd xz wwxjo,\n"
        "ppjl dilt prostcn xzd ssfssc, lh pj ewl hfcnizc pxdtsj th smdilrtcn. M dwvyws yidi oict,\n"
        "iye hvqpiomyv aiwaz qp cvx ed smyvlv qdy xzd ssyv. Alth wplrl jptsw lapzp xu e hpf M npu'x\n"
        "jta yyslvdihro puh T rhryda wptt xz glqpbiic <ioihpfsfi>.\n"
    );
    printf("\nAttached to the bottom of the page is a spell: <insight>\n");
}

static inline void level1() {
    // display level
    printf("\n----- LEVEL 1 -----\n");
    level1_greeting();
}

static inline bool level1_cast(char* cmd) {
    // get first token of command
    char *token, *saveptr;
    token = strtok_r(cmd, " ", &saveptr);

    // check if command is "cast"
    if (strcmp(token, "cast") == 0) {
        // get second token (if it exists)
        token = strtok_r(NULL, " ", &saveptr);
        if (token == NULL) {
            printf("As you begin to cast your spell, you realize you never decided on what one to cast.\n");
            printf("Make sure to specify a spell to cast when calling \"cast\"!\n");
            return false;
        }
        else if (strcmp(token, "<thewayout>") == 0) {
            // display correct key message
            printf("\n"
                "The very fabric of reality trembles as a portal begins to unfold—a swirling,\n"
                "iridescent archway that ripples into existence. Its edges shimmer with hues of\n"
                "twilight blue and deep violet, bordered by intricate runes that pulse with\n"
                "otherworldly energy. The air vibrates with a humming energy as you pass through\n"
                "the event horizon. For an instant, it feels as if time itself has stopped.\n"
                "Then, as if waking from a dream, you find yourself before your next challenge.\n"
                "\n----- LEVEL 1: COMPLETE -----\n\n"
            );
            // increment level
            drako.currentLevel++;
            drako.currentLevel++;
            return true;
        }
        else if (strcmp(token, "<insight>") == 0) {
            // hit for user to look at board
            printf("\nDrako begins to change.");

            // select, clear, and enable display
            display_select(&drako.disp);
            display_clear(&drako.disp);
            display_show(&drako.disp);

            // display "HE" for 1 second
            display_write(&drako.disp, 0x7679);
            sleep_ms(_LEVEL1_DISP_DELAY);
            printf(".");

            // display "LP" for 1 second
            display_write(&drako.disp, 0x3873);
            sleep_ms(_LEVEL1_DISP_DELAY);
            printf(".\n");

            // hide the display
            display_hide(&drako.disp);

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
    else {
        printf("You do not know that spell.\n");
        return false;
    }
}

void level1_display();

#endif
