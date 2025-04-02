.ONESHELL:

build:
	gcc -Iinclude src/cchess_engine.c src/init.c src/bitboard.c src/hashkey.c src/data.c src/board.c src/attack.c src/io.c src/movegen.c src/validate.c src/makemove.c src/perft.c src/search.c src/misc.c src/pvtable.c src/evaluate.c src/uci.c src/xboard.c -o cchess_engine

debug:
	gcc -Iinclude src/cchess_engine.c src/init.c src/bitboard.c src/hashkey.c src/data.c src/board.c src/attack.c src/io.c src/movegen.c src/validate.c src/makemove.c src/perft.c src/search.c src/misc.c src/pvtable.c src/evaluate.c src/uci.c src/xboard.c -o cchess_engine -g

pre-commit:
	pre-commit run --all-files

.PHONY: build pre-commit debug
