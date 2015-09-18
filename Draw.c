void drawTetromino(int INtet,int INrot, int INx, int INy, int INreal)
	{
#ifdef DEBUG_STREAM
	printf("drawTet - start");
#endif
	if (INtet>EMPTY_AREA)
		{
		SDL_Rect tRect;
		for (int i=0; i<4; i++)
			{
			EZ_set_rect(&tRect,
				((INx + tet[INtet][X][INrot][i]) * BLOCK_SIZE),	//X
				((INy + tet[INtet][Y][INrot][i]) * BLOCK_SIZE),	//Y
				BLOCK_SIZE,										//W
				BLOCK_SIZE);									//H
			switch(INreal)
				{
				case REAL_TET:				//REAL TETROMINO
					EZ_apply_rect(screen, tRect, tetColors[INtet]);
					break;
				case GHOST_TET:				//GHOST TETROMINO
					EZ_apply_rect(screen, tRect, ghostColors[INtet]);
					break;
				default:
					fprintf( stderr, "drawTetromino, bad draw type value \n");
					SDL_Quit();
					exit(FAKE_TET_ERROR);
					break;
				}
#ifdef DRAW_TET_DETAILS
			printf("\n(%d,%d)",INtet,INrot);
#endif
			}
		}
#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	}





void blinkyFullRows()
	{
	int noBlocks=0;
	int noFilled=0;
	int fullRows[NO_BLINKABLE_ROWS];
	for (int i=0; i<NO_BLINKABLE_ROWS; i++)
		fullRows[i] = -1; //Will not draw if negative
	for (int i=0; i<AREA_HEIGHT+2; i++)		//From the top, check all rows...
		{
		noBlocks=0;
		for (int j=0; j<AREA_WIDTH; j++)		//Add up number of blocks in row...
			noBlocks+=(area[j][i]>EMPTY_AREA);
		
		if (noBlocks>=AREA_WIDTH)				//If the row is full...
			{
			if (noFilled<NO_BLINKABLE_ROWS) //fullRows declared as array of 4 items
				fullRows[noFilled]=i;
			noFilled++;
			}
		}
	if (noFilled>0) //if some rows are full
		{
		Uint32 tColor;
		int y;
		for (int i=0; i<NO_BLINKS*2; i++) //Repeat the number of blinks
			{
			for (int j=0; j<NO_BLINKABLE_ROWS; j++) //Go through the full rows
				{
				y=fullRows[j];
				for (int x=0; x<AREA_WIDTH; x++)
					{
					if ((i+1)%2)	tColor=ghostColors[area[x][y]];	//First
					else			tColor = otherColors[C_BACK];	//Second		//tetColors[area[x][y]];
					if (y>=0)
						EZ_apply_rect(screen, EZ_new_rect(x*BLOCK_SIZE, (y-2)*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE), tColor);
					}
				}
			SDL_Flip(screen);
			SDL_Delay(BLINK_TIME);
			}
		}
	}

	
	

void ClearScreen(void)
	{
#ifdef DEBUG_STREAM
	printf(" clear");
#endif
	//EZ_apply_rect(screen,EZ_new_rect(0,0,SCR_WIDTH-TEXT_EXTRA,SCR_HEIGHT),otherColors[C_BACK]);
	EZ_apply_rect(screen,EZ_new_rect(0,0,SCR_WIDTH,SCR_HEIGHT),otherColors[C_BACK]);
#ifdef DEBUG_STREAM
	printf("screen\n");
#endif
	}


	
	
void drawBoard(void)
	{
#ifdef DEBUG_STREAM
	printf("drawBoard - start");
#endif
	ClearScreen();	
	SDL_Rect tRect;
	//TEXT
#ifdef SHOW_TEXT
	char tempPrint[17]; //17 allows 10 digit score
	SDL_Color tColor = EZ_Uint32_to_SDL(otherColors[C_TEXT]);
	
	tRect = EZ_new_rect( (AREA_WIDTH+6)*BLOCK_SIZE+TEXT_OFFSET, TEXT_OFFSET, 0, 0);
	sprintf(tempPrint,"Score: %d",score);
	EZ_apply_text(screen, tempPrint, font, tRect, tColor);
	
	tRect.y += TEXT_SIZE+TEXT_OFFSET;
	sprintf(tempPrint,"Level: %d",level);
	EZ_apply_text(screen, tempPrint, font, tRect, tColor);
	
	tRect.y += TEXT_SIZE+TEXT_OFFSET;
	sprintf(tempPrint,"Lines: %d",lines);
	EZ_apply_text(screen, tempPrint, font, tRect, tColor);
	
	tRect.y += TEXT_SIZE+TEXT_OFFSET;
	
	tRect.y += TEXT_SIZE+TEXT_OFFSET;
	sprintf(tempPrint,"Pieces: %d",pieceCount[ALL]);
	EZ_apply_text(screen, tempPrint, font, tRect, tColor);
	
	//char tempPrint2[10];
	for (int b=0; b<7; b++)
		{
		tRect.y += TEXT_SIZE+TEXT_OFFSET;
		sprintf(tempPrint,"%d",pieceCount[b]);
		EZ_apply_text(screen, tempPrint, font, tRect, EZ_Uint32_to_SDL(tetColors[b]));
		}
#endif


	if (!paused)
		{
#ifdef DEBUG_BOARD
		for (int x=0; x<AREA_WIDTH; x++)
			printf("-");
		printf("\n");
#endif
		for (int y=2; y<AREA_HEIGHT+2; y++)
			{
			for (int x=0; x<AREA_WIDTH; x++)
				{
				int test=area[x][y];
				if (test>EMPTY_AREA) //Piece
					{
					SDL_Rect tRect = EZ_new_rect(x*BLOCK_SIZE, (y-2)*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
					EZ_apply_rect(screen, tRect, tetColors[test]);
#ifndef DEBUG_BOARD
					}
				}
			}
#else
					printf("%d",test);
					}
				else
					printf(" ");
				}
			printf("\n");
			}
		for (int x=0; x<AREA_WIDTH; x++)
			printf("-");
		printf("\n\n");
#endif
		
		//HOLD PIECE
#ifdef SHOW_HOLD_PIECE
		drawTetromino(currentHoldPiece, 2, AREA_WIDTH+1, 0, REAL_TET); 
#endif

		//NEXT PIECES
#ifdef SHOW_NEXT_PIECES
		for (int i=0; i<PREVIEW_PIECES; i++)//tet,rot,x,			y,			type
			drawTetromino(nextPiece[i], 2, AREA_WIDTH+1, (PREVIEW_PIECE_SPACING*(i+1)), REAL_TET); 
#endif
		
		//GHOST PIECE
#ifdef SHOW_GHOST_PIECE
		int tempUD=tetUD;
		bool tempLoop=true;
		//Ghost Piece
		while (tempLoop)
			{
			if (move(dwn,tempUD))
				{
				tempUD++;
				}
			else
				{
				//tempUD++;
				tempLoop=false;
				drawTetromino(currentPiece,currentRotate,tetLR,tempUD,GHOST_TET);
				}
			}
#endif

		//CURRENT TETROMINO
#ifdef SHOW_CURR_PIECE
	drawTetromino(currentPiece,currentRotate,tetLR,tetUD,REAL_TET);
#endif
		}
	else //if (paused)
		{
		char tempPrintPaused[10]="PAUSED!";
		SDL_Color tempColorPaused = EZ_Uint32_to_SDL(otherColors[C_TEXT]);
		//SDL_Rect tRect = EZ_new_rect(SCR_WIDTH/2-30,SCR_HEIGHT/2,0,0);
		EZ_set_rect(&tRect, (tetLR*BLOCK_SIZE>0)?(tetLR*BLOCK_SIZE):(0),(tetUD*(tetUD>=0)*BLOCK_SIZE)+BLOCK_SIZE/2,0,0);
		EZ_apply_text(screen, tempPrintPaused, font, tRect, tempColorPaused);
		}


#ifdef SHOW_LINES
	//OUTER EDGE LINES
	EZ_set_rect(&tRect, AREA_WIDTH*BLOCK_SIZE, 0, BLOCK_SIZE/2, AREA_HEIGHT*BLOCK_SIZE);
	EZ_apply_rect(screen,tRect,otherColors[C_TEXT]);
	tRect.x = ((float)AREA_WIDTH+5.5)*BLOCK_SIZE;
	EZ_apply_rect(screen,tRect,otherColors[C_TEXT]);
	
	//HOLD PIECE SEPARATOR
	EZ_set_rect(&tRect, AREA_WIDTH*BLOCK_SIZE, ((float)BLOCK_SIZE*2.25), BLOCK_SIZE*6, BLOCK_SIZE/2);
	EZ_apply_rect(screen,tRect,otherColors[C_TEXT]);
#endif

	SDL_Delay(10);
	SDL_Flip(screen);
#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	}

/*
#ifdef SHOW_LINES
drawRect.x = AREA_WIDTH*BLOCK_SIZE;
drawRect.y = 0;
drawRect.w = BLOCK_SIZE/2;
drawRect.h = AREA_HEIGHT*BLOCK_SIZE;
SDL_FillRect(screen, &drawRect, SDL_MapRGB(screen->format,
	otherColors[C_TEXT][0],
	otherColors[C_TEXT][1],
	otherColors[C_TEXT][2]));
SDL_FillRect(screen, &drawRect, SDL_MapRGB(screen->format,
	otherColors[C_TEXT][0],
	otherColors[C_TEXT][1],
	otherColors[C_TEXT][2]));
drawRect.x = AREA_WIDTH*BLOCK_SIZE;
drawRect.y = ((float)BLOCK_SIZE*2.25);
drawRect.w = BLOCK_SIZE*6;
drawRect.h = BLOCK_SIZE/2;
SDL_FillRect(screen, &drawRect, SDL_MapRGB(screen->format,
	otherColors[C_TEXT][0],
	otherColors[C_TEXT][1],
	otherColors[C_TEXT][2]));
#endif
*/
	
	
/*
for (int i=0; i<previewPieces; i++)//tet,rot,	x,					y
	drawTetromino(nextPiece[i], 0, 63-4*BLOCK_SIZE, 78-(4*BLOCK_SIZE*i)+(nextPiece[i]==Otet*4)*BLOCK_SIZE,REAL_TET);
if (showDisp>0&&disp==0)
	{
	TextOutput(msgX,msgY,0,0);
	showDisp=0;
	}
if (disp!=0)
	{
	TextOutput(msgX,msgY,disp,0);
	showDisp++;
	//if (showDisp==0) refresh=2;
	if (showDisp>waitDisp) disp=0;
	}
*/