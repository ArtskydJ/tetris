//Definitions and Data
//Created for use with Tetris
//Written by Joseph Dykstra

const char tetColors[14][3]={
	{000,240,240},	//ctetI
	{000,000,240},	//ctetJ
	{240,160,000},	//ctetL
	{240,240,000},	//ctetO
	{000,240,000},	//ctetS
	{240,000,240},	//ctetT
	{240,000,000},	//ctetZ
	{000, 48, 48},	//ctetI Ghost
	{000,000, 48},	//ctetJ Ghost
	{ 48, 32,000},	//ctetL Ghost
	{ 48, 48,000},	//ctetO Ghost
	{000, 48,000},	//ctetS Ghost
	{ 48,000, 48},	//ctetT Ghost
	{ 48,000,000}	//ctetZ Ghost
};

const char otherColors[2][3]={
	{ 32, 32, 32},	//Background
	{222,222,222}	//Text, borders, etc.
};
#define C_BACK		0
#define C_TEXT		1


//DIMENSIONS
#define BLOCK_SIZE	32
#define AREA_WIDTH	10
#define AREA_HEIGHT	20

#define SCR_WIDTH  ((AREA_WIDTH+6)*BLOCK_SIZE)
#define SCR_HEIGHT (AREA_HEIGHT*BLOCK_SIZE)
#define SCR_BPP    32


/*#define TX      35
#define SX      94
#define SY      61*/
#define msgX    0
#define msgY    56
#define PREVIEW_PIECE_SPACING	3
#define PREVIEW_PIECES   		(AREA_HEIGHT/PREVIEW_PIECE_SPACING)
//TIMING
#define waitGravity     5
#define waitDisp        18
#define waitSlowMove    3
#define waitFastMove    0
#define waitRotateAgain 3
#define waitHardDrop    3
#define waitLock        2
#define fastRepeat      5

//Buttons (Arrow keys or number pad)
//#define ARROW_KEYS //If undefined, defaults to number pad


//Move()
#define dwn 0
#define lft 1
#define rht 2
//Drop
#define dropHard  1
#define dropSoft  2
//Tetriminos
#define Itet 0
#define Jtet 1
#define Ltet 2
#define Otet 3
#define Stet 4
#define Ttet 5
#define Ztet 6
#define prob 7

//More...
#define GHOST_TET	0
#define REAL_TET	1

//					PROTOTYPES
bool inputOutput(bool,bool,int);
void ClearScreen();
void drawBoard(bool);

//					ARRAYS
const char tet[28][8]=
  {
// I
  {0,1,2,3,
   2,2,2,2},
  {1,1,1,1,
   0,1,2,3},
  {0,1,2,3,
   1,1,1,1},
  {2,2,2,2,
   0,1,2,3},
// J
  {0,1,2,2,
   1,1,1,2},
  {1,1,1,0,
   0,1,2,2},
  {0,1,2,0,
   1,1,1,0},
  {1,1,1,2,
   0,1,2,0},
// L
  {0,0,1,2,
   2,1,1,1},
  {0,1,1,1,
   0,0,1,2},
  {0,1,2,2,
   1,1,1,0},
  {1,1,1,2,
   0,1,2,2},
// O
  {1,1,2,2,
   0,1,0,1},
  {1,1,2,2,
   0,1,0,1},
  {1,1,2,2,
   0,1,0,1},
  {1,1,2,2,
   0,1,0,1},
// S
  {2,1,1,0,
   1,1,2,2},
  {0,0,1,1,
   0,1,1,2},
  {2,1,1,0,
   0,0,1,1},
  {1,1,2,2,
   0,1,1,2},
//T
  {0,1,1,2,
   1,1,2,1},
  {1,1,0,1,
   0,1,1,2},
  {0,1,1,2,
   1,1,0,1},
  {1,1,2,1,
   0,1,1,2},
// Z
  {0,1,1,2,
   1,1,2,2},
  {0,0,1,1,
   2,1,1,0},
  {0,1,1,2,
   0,0,1,1},
  {1,1,2,2,
   2,1,1,0}
};


//                 GLOBAL VARIABLES
bool alive			= 0;
bool prevQuadClear	= 0;
bool highMade		= 0;
bool floorKickUsed	= 0;
bool refresh		= 0;
bool playagain		= true;
bool piecemoving	= true;

//START  ARRAYS
char area[AREA_WIDTH][AREA_HEIGHT+2]; //Two hidden rows
char chnc[8];
int nextPiece[PREVIEW_PIECES];
// END   ARRAYS

int showDisp      =0;
int viewDbg       =0;
int gametype      =0;
int gamespeed     =0;
int delaytime     =0;
int lockDelay     =0;
int currentPiece  =0;
int currentRotate =0;
int tetLR         =0;
int tetUD         =0;
int drop          =0;
int prevWaited    =0;
float speedup     =0;
long score        =0;
//these were strings:
int msg        =0;
int disp       =0;
int accesshigh =0;

SDL_Surface *screen=NULL;
SDL_Rect drawRect;

