.ONESHELL:

build:
	gcc cchess_engine.c init.c bitboard.c hashkey.c data.c board.c attack.c io.c -o cchess_engine

pre-commit:
	pre-commit run --all-files

.PHONY: build pre-commit
