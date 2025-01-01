The source code is located in ./src

Compile the game manually with the use of the command while on the root directory of the project:

    gcc -o build/Sea-Duel.exe src/main.c src/game.c -Iinclude -Llibs -lraylib -lopengl32 -lgdi32 -lm -lwinmm

The executable file will be available in the build directory.

The game uses exclusively UTF-8 encoding.

This project was NOT developed in Codeblocks. Success in compiling in Codeblocks with the usual compiler options and flag is not guarenteed.

All licences can be found on the LICENCES directory.
