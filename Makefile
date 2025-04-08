.ONESHELL:

build:
	gcc -Iinclude src/*.c -o cchess_engine

debug:
	gcc -Iinclude src/*.c -o cchess_engine -Wall -Wextra -Werror -pedantic -std=c99 -g

pre-commit:
	pre-commit run --all-files

.PHONY: build pre-commit debug
