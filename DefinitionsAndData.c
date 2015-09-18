//Definitions and Data
//Created for use with Tetris
//Written by Joseph Dykstra

//FLAGS
//#define ONE_FLOOR_KICK
#define SHOW_NEXT_PIECES
#define SHOW_GHOST_PIECE
#define SHOW_CURR_PIECE
#define SHOW_HOLD_PIECE
#define SHOW_LINES
#define SHOW_TEXT

//DIMENSIONS
#define BLOCK_SIZE	32
#define AREA_WIDTH	10	//blocks
#define AREA_HEIGHT	20	//blocks
#define TEXT_EXTRA	150	//pixels

#define SCR_WIDTH  (((AREA_WIDTH+6)*BLOCK_SIZE)+TEXT_EXTRA)
#define SCR_HEIGHT (AREA_HEIGHT*BLOCK_SIZE)
#define SCR_BPP    32

#define TEXT_SIZE	18
#define TEXT_OFFSET	10	//Offset of text from preview dividing line and surrounding lines

#define PREVIEW_PIECE_SPACING	3
#define PREVIEW_PIECES   		(AREA_HEIGHT/PREVIEW_PIECE_SPACING)
#define PREVIEW_PIECE_CALC		14	//

//ERRORS
#define SDL_INIT_ERROR		-1
#define	VIDEO_INIT_ERROR	-2
#define TTF_INIT_ERROR		-3
#define OPEN_FONT_ERROR		-4
#define ICON_SURFACE_ERROR	-5
#define FAKE_TET_ERROR		-6

//TIMING
#define waitGravity     5
#define waitDisp        18
#define waitSlowMove    3
#define waitFastMove    0
#define waitRotateAgain 3
#define waitHardDrop    3
#define waitLock        2
#define fastRepeat      5
#define BLINK_TIME		200 //MS, change to var sometime?

//Buttons (Arrow keys or number pad)
//#define ARROW_KEYS //If undefined, defaults to number pad


//Number Of
#define NO_BLINKS			3	// (ON-OFF) = 1 blink
#define NO_BLINKABLE_ROWS	4	// 4 is the most you can clear at once

//Move()
#define dwn 0
#define lft 1
#define rht 2
//tet and area arrays
#define EMPTY_AREA	(-1)
#define X	0
#define Y	1
//Drop
#define dropHard  1
#define dropSoft  2
//Tetriminos
#define Itet	0
#define Jtet	1
#define Ltet	2
#define Otet	3
#define Stet	4
#define Ttet	5
#define Ztet	6
#define prob	7
#define ALL		7

//More...
#define GHOST_TET	0
#define REAL_TET	1
#define COMING		0
#define HOLD		1

#define C_BACK		0
#define C_TEXT		1
//#define C_BLINK		2

//ARRAYS

const Uint32 otherColors[]=
	{
	0x202020,	//Background
	0xDEDEDE	//Text, borders, etc.
	//0x404040	//Blinking Full Rows
	};

const Uint32 tetColors[7]=
	{
	0x00F0F0,	//ctetI
	0x0000F0,	//ctetJ
	0xF0A000,	//ctetL
	0xF0F000,	//ctetO
	0x00F000,	//ctetS
	0xF000F0,	//ctetT
	0xF00000	//ctetZ
	};

const Uint32 ghostColors[7]=
	{
	0x005050,	//ctetI Ghost
	0x000050,	//ctetJ Ghost
	0x503600,	//ctetL Ghost
	0x505000,	//ctetO Ghost
	0x005000,	//ctetS Ghost
	0x500050,	//ctetT Ghost
	0x500000 	//ctetZ Ghost
	};

const char tet[7][2][4][4]=
{
	{	//I tetromino
		{	//X
			{0,1,2,3},
			{1,1,1,1},
			{0,1,2,3},
			{2,2,2,2}
		},
		{	//Y
			{2,2,2,2},
			{0,1,2,3},
			{1,1,1,1},
			{0,1,2,3}
		}
	},
	{	//J tetromino
		{	//X
			{0,1,2,2},
			{1,1,1,0},
			{0,1,2,0},
			{1,1,1,2}
		},
		{	//Y
			{1,1,1,2},
			{0,1,2,2},
			{1,1,1,0},
			{0,1,2,0}
		}
	},
	{	//L tetromino
		{	//X
			{0,0,1,2},
			{0,1,1,1},
			{0,1,2,2},
			{1,1,1,2}
		},
		{	//Y
			{2,1,1,1},
			{0,0,1,2},
			{1,1,1,0},
			{0,1,2,2}
		}
	},
	{	//O tetromino
		{	//X
			{1,1,2,2},
			{1,1,2,2},
			{1,1,2,2},
			{1,1,2,2}
		},
		{	//Y
			{0,1,0,1},
			{0,1,0,1},
			{0,1,0,1},
			{0,1,0,1}
		}
	},
	{	//S tetromino
		{	//X
			{2,1,1,0},
			{0,0,1,1},
			{2,1,1,0},
			{1,1,2,2}
		},
		{	//Y
			{1,1,2,2},
			{0,1,1,2},
			{0,0,1,1},
			{0,1,1,2}
		}
	},
	{	//T tetromino
		{	//X
			{0,1,1,2},
			{1,1,0,1},
			{0,1,1,2},
			{1,1,2,1}
		},
		{	//Y
			{1,1,2,1},
			{0,1,1,2},
			{1,1,0,1},
			{0,1,1,2}
		}
	},
	{	//Z tetromino
		{	//X
			{0,1,1,2},
			{0,0,1,1},
			{0,1,1,2},
			{1,1,2,2}
		},
		{	//Y
			{1,1,2,2},
			{2,1,1,0},
			{0,0,1,1},
			{2,1,1,0}
		}
	}
};




char area[AREA_WIDTH][AREA_HEIGHT+2]; //Two hidden rows
int pieceCount[8];
int nextPiece[7];
int nextPieceRefresh[7];
bool pieceCreated[7];



//                 GLOBAL VARIABLES
bool alive			= 0;
bool prevQuadClear	= 0;
bool highMade		= 0;
bool floorKickUsed	= 0;
bool playagain		= true;
bool piecemoving	= true;
bool usedHoldPiece	= false;
bool paused			= false;

int viewDbg				=0;
int gamespeed			=0;
int delaytime			=0;
int lockDelay			=0;
int currentHoldPiece	=0;
int currentPiece		=0;
int currentRotate		=0;
int tetLR				=0;
int tetUD				=0;
int drop				=0;
int prevWaited			=0;
float speedup			=0;

unsigned long score		=0;
unsigned long level		=0;
unsigned long lines		=0;

SDL_Surface *screen		=NULL;
SDL_Surface *icon		=NULL;
TTF_Font *font			=NULL;


//					PROTOTYPES
bool inputOutput(int);
void ClearScreen(void);
void drawBoard(void);
void generate7Pieces(int*);
void quit(void);
