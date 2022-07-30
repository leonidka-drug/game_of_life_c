# Terminal version of The Game Of Life, written in C.

To compile this you need to use -lncurses flag with gcc like (gcc -Wall -Werror -Wextra -o game_of_life game_of_life.c -lncurses).

After compiling, you can use ./game_of_life as usual, or can redirect stdin stream to the program, for exemple (./game_of_life < file_2.txt)
and run the program with the prepared configurations.
