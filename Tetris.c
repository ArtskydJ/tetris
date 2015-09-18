//Tetris
//Joseph Dykstra
//Port started 07-15-2013
//Finished xx-xx-201x
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
//#include <string.h>
#include "including/EasySDL.h"


#define DEBUG
#define MODE	0


#include "DefinitionsAndData.c"
#include "InitAndQuit.c"
#include "TetrominoManip.c"
#include "InputAndOutput.c"
#include "Draw.c"
#ifdef DEBUG
	#include "Debug.c"
	#ifdef MODE
		#undef MODE
	#endif
	#define MODE	0
	#define DEBUG_STREAM		//turns on most debug stream messages
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
	if (MODE==0 || MODE==1)
		{
		while (playagain)
			{
			ClearScreen();
			selectGame(1); //selectGame(MODE); //don't have way to select game types yet.
			gameResetVars();
			alive=true;
			while (alive)
				{
				pieceCreate(COMING);
				alive = move(dwn,tetUD);	//If a piece was made where it...
				piecemoving = alive;			//can't move down, end the game.
				for (int i=1; piecemoving; i++) //while the piece is moving
					{
					i%=waitGravity;
					drawBoard();
#ifdef DEBUG_INFO
					debugInfo(i);
#endif
					SDL_Delay(delaytime); // change to 10
					piecemoving=inputOutput(i);
					}
				//delaytime=((float)delaytime*speedup); //needs float typecast?
				if (alive)
					pieceSetInto(currentPiece);
				drawBoard();
				blinkyFullRows();
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
	quit();
	return 0;
	}

	
	
	
	
/*

TTF...
DefinitionsAndData  -  line 240
InitAndQuit  -  line 22


BUGS

FIX!
the icon is invisible
level counter doesn't count up
piece never speeds up

FIXED?
	
	
TODO
+ add bonus points for combos
+ add background music
+ change BLINK_TIME into a variable? (or divide by delaytime?)
+ clean up the code...
	- take out unused functions
	- name all functions with camelCase
	- name all variables with camelCase
	- name all #defines in ALL_CAPS
	- change all [UD] and [LR] to [y] and [x]
	- make some variables local?
	- deside which checks for the end of game:
		[piecemoving=move(dwn,tetUD);]	// Tetris3.c			r:55	c:17 //Official correct way, I think
		[if (blocksInRow>0 && i<2)]		// TetrominoManip.c		r:171	c:9

DONE
+ use 'The Random Generator'
+ line clears should blink
+ show "paused" if paused.
+ clean up the debug stream; too much un-needed junk in it
+ make a hold piece
+ rotate the nextPiece so that WYSIWYG
+ create pause functionality
+ clean up the code...
	- put all [printf] functions between [#ifdef DEBUG] statements
	- replace clearfullrows() [if (INallowLog)] with [#ifdef DEBUG] statements




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
