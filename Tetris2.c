//Tetris
//Joseph Dykstra
//Port started 07-15-2013
//Finished ??-??-2013
#include "SDL/SDL.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define DEBUG
#define MODE	0
//#define ONE_FLOOR_KICK
#define SHOW_NEXT_PIECES
#define SHOW_GHOST_PIECE
#define SHOW_CURR_PIECE


#include "DefinitionsAndData.c"
#include "SmallFunctions.c"
#include "InitAndQuit.c"
#include "TetrominoManip.c"
#include "InputOutputAndDraw.c"
#ifdef DEBUG
	#include "Debug.c"
	#ifdef MODE
		#undef MODE
	#endif
	#define MODE	0
//	#define DEBUG_STREAM		//turns on most debug stream messages
//	#define DEBUG_BOARD			//turns on board drawing in debug stream
//	#define DEBUG_INFO			//turns on debug stream info about game's state
//	#define DRAW_TET_DETAILS	//turns on details of tetromino drawing positions
#endif


/* ************************************ *\
|*		Modes:							*|
|* 0	Normal							*|
|* 1	Fast							*|
|* 2	Debug clearFullRows()			*|
|* 3	Dubug rotation					*|
|* 4	Debug peiceSetinto()			*|
|* 5	Debug peiceCreate()				*|
|* 6	Debug gravity in inputOutput()	*|
\* ************************************ */


int main( int argc, char *argv[] )
	{
	initReset();
	srand(time(0));
	if (MODE==0||MODE==1)
		{
		while (playagain)
			{
			ClearScreen();
			selectGame(1); //selectGame(MODE); //don't have way to select game types yet.
			gameResetVars();
			alive=true;
			while (alive)
				{
				pieceCreate();
				alive = move(dwn,tetUD);	//If a piece was made where it...
				piecemoving = alive;			//can't move down, end the game.
				for (int i=1; piecemoving; i++) //while the piece is moving
					{
					i%=waitGravity;
					drawBoard(true); //replace [false] with [i==0]
#ifdef DEBUG_INFO
					debugInfo(i);
#endif
					SDL_Delay(delaytime/waitGravity); // was 10
					piecemoving=inputOutput(true,true,i);
					}
				//delaytime*=speedup; //needs float typecast?
				pieceSetInto(currentPiece);
				clearFullRows();
				}
			playagain=gameEnd();
			}
		}
#ifdef DEBUG
	else
		{
		debug(MODE);
		}
#endif
	SDL_Quit();
	return 0;
	}

	
	
	
	
/*
BUGS

FIX!

FIXED?
pieces rotate into others (off by 2 because of hidden 2 rows?)
hard drop acts like firm drop
	
	
TODO
+ use the Random Generator
+ make a hold piece
+ add bonus points for combos
+ add piece, line, score, and level counters.
+ rotate the nextPiece so that WYSIWYG
+ create pause functionality
+ clean up the code...
	- put all [printf] functions between [#ifdef DEBUG] statements
	- take out unused functions
	- name all functions with camelCase
	- name all variables with camelCase
	- name all #defines in ALL_CAPS
	- change all [UD] and [LR] to [y] and [x]
	- replace clearfullrows() [if (INallowLog)] with [#ifdef DEBUG] statements
	- make some variables local?
	- deside which checks for the end of game:
		[piecemoving=move(dwn,tetUD);]	// Tetris3.c			r:55	c:17
		[if (blocksInRow>0 && i<2)]		// TetrominoManip.c		r:171	c:9

DONE
+ clean up the debug stream; too much un-needed junk in it




Process Order:
-Rotation
-Move
-Drop
-Gravity

Soft Drop					DONE
Hard Drop					DONE
Clockwise Rotation			DONE
Right Side Bias				DONE
Delayed Auto Shift			DONE
Wall kicks					DONE

Allow only 1 floor kick									DELETED (Don't Like)
Allow I wall kicking always.							DONE
Allow I floor kicking even when not touching ground		DONE

Adhere to SRS guidelines, some ARS guidelines
	http://tetris.wikia.com/wiki/SRS
	http://tetris.wikia.com/wiki/ARS

*/
