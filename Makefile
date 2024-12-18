main:
	gcc src/main.c -o build/game.exe -Ilib/ -Llib/ -Wall -O2 -std=c99 -Wall -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm
	build/game.exe