SRCS = $(wildcard src/*.c)
NAME = CChessEngine

.ONESHELL:

build:
	gcc -Iinclude $(SRCS) -o $(NAME) -O2

debug:
	gcc -Iinclude $(SRCS) -o $(NAME) -Wall -Wextra -Werror -pedantic -std=c99 -g

pre-commit:
	pre-commit run --all-files

.PHONY: build pre-commit debug
