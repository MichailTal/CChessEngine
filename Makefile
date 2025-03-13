.ONESHELL:

build:
	gcc cchess_engine.c init.c bitboard.c hashkey.c data.c board.c attack.c io.c movegen.c validate.c makemove.c perft.c -o cchess_engine

debug:
	gcc cchess_engine.c init.c bitboard.c hashkey.c data.c board.c attack.c io.c movegen.c validate.c makemove.c perft.c -o cchess_engine -g

pre-commit:
	pre-commit run --all-files

.PHONY: build pre-commit debug
