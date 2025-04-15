# CChessEngine
Simple Chess Engine in C.

It currently has the following features:

- Alpha beta search
- Iterative deepening
- Quiescence search
- Transposition table
  - Always replace
- Polyglot opening books
- MVV/LVA move ordering
- Basic evaluation

The engine is based around the old implementation of VICE, an easy engine for demonstration.
You can check the repo [here](https://github.com/bluefeversoft/vice/tree/main)

For Multithreading the Move Search, this engine utilizes [tinycthread](https://github.com/tinycthread/tinycthread).

## Lichess

This engine is powering a BOT Account on Lichess, that you can check out [here](https://lichess.org/@/Nilstron)


## Build process

You can run this engine on both POSIX and Windows Systems as well as Docker Images.

- Clone the code from the main branch of this repo

- Make sure that your system is able to execute make commands. MacOS and Linux usually are per default. If you want to build on Win, [install](https://community.chocolatey.org/packages/make) Make or copy & paste the gcc commands from the Makefile in the terminal

- Make sure that your compiler is [gcc](https://gcc.gnu.org/releases.html)

- run:
 ```bash
     make build
 ```

- the binary is created within the folder of the Makefile

## Author

You can find the Author of the engine [here](https://www.linkedin.com/in/nils-k%C3%B6hler97/)
