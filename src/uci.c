#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "string.h"
#include "stdio.h"

#define INPUTBUFFER 400 * 6

void ParseGo(char* line, S_SEARCHINFO *info, board_representation *pos) {};
void ParsePosition(char* lineIn, board_representation *pos) {};

void UCI_Loop() {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char line[INPUTBUFFER];

    // See: https://www.wbec-ridderkerk.nl/html/UCIProtocol.html
    printf("id name %s\n", NAME);
    printf("id author Nils\n");
    printf("uci ok\n");

    board_representation pos[1];
    S_SEARCHINFO info[1];
    InitPvTable(pos -> PvTable);

    while (TRUE) {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if (!fgets(line, INPUTBUFFER, stdin))
        continue;

        if(line[0] == '\n')
        continue;

        if(!strncmp(line, "isready", 7)) {
            printf("readyok\n");
            continue;
        } else if (!strncmp(line, "position", 8)) {
            ParsePosition(line, pos);
        } else if (!strncmp(line, "ucinewgame", 10)) {
            ParsePosition("position startpos\n", pos);
        } else if (!strncmp(line, "go", 2)) {
            ParseGo(line, info, pos);
        } else if (!strncmp(line, "quit", 4)) {
            info -> quit = TRUE;
            break;
        } else if (!strncmp(line, "uci", 3)) {
            printf("id name %s\n", NAME);
            printf("id author Nils\n");
            printf("uci ok\n");
        }
        if (info -> quit) break;
    }
}