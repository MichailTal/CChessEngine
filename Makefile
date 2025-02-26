.ONESHELL:

build:
	gcc cchess_engine.c init.c bitboard.c hashkey.c data.c board.c -o cchess_engine

pre-commit:
	pre-commit run --all-files

.PHONY: build pre-commit
