//InputOutputAndDraw.c
//Input, Output, and Drawing functions for Tetris
//Written by Joseph Dykstra


bool inputOutput(bool INallowInput, bool INallowOutput, int INwaited)
	{
#ifdef DEBUG_STREAM
	printf("inputOutput - start");
#endif
	char temppiecemoving=true; //return this
//INPUT
//INPUT
	if (INallowInput)
		{
		char btnPressed=0;
		SDL_Event test_event;
		while (SDL_PollEvent(&test_event)) //GET INPUT
			{
			switch (test_event.type) //WHICH EVENT
				{
				case SDL_KEYDOWN: //IF KEY DOWN
					switch (test_event.key.keysym.sym) //WHICH KEY
						{
#ifdef ARROW_KEYS							//HARD DROP
						case SDLK_SPACE:
#else
						case SDLK_KP8:
#endif
							lockDelay=waitLock;
							drop=dropHard;
							break;
#ifdef ARROW_KEYS							//SOFT DROP
						case SDLK_DOWN:
#else
						case SDLK_KP2:
#endif
							if (lockDelay<waitLock)
								{
								//lockDelay=0;
								drop=dropSoft;
								}
							break;
#ifdef ARROW_KEYS							//ROTATE LEFT
						case SDLK_z:
#else
						case SDLK_KP3:
						case SDLK_KP7:
#endif
							if (rotate(lft) && lockDelay<waitLock)
								{
								lockDelay=0;
								refresh=true;
								}
							break;
#ifdef ARROW_KEYS							//ROTATE RIGHT
						case SDLK_UP:
						case SDLK_x:
#else
						case SDLK_KP1:
						case SDLK_KP5:
						case SDLK_KP9:
#endif
							if (rotate(rht) && lockDelay<waitLock)
								{
								lockDelay=0;
								refresh=true;
								}
							break;
#ifdef ARROW_KEYS							//MOVE LEFT
						case SDLK_LEFT:
#else
						case SDLK_KP4:
#endif
							if (move(lft,tetUD) && lockDelay<waitLock)
								{
								//lockDelay=0;
								tetLR--;
								refresh=true;
								}
							break;
#ifdef ARROW_KEYS							//MOVE RIGHT
						case SDLK_RIGHT:
#else
						case SDLK_KP6:
#endif
							if (move(rht,tetUD) && lockDelay<waitLock)
								{
								//lockDelay=0;
								tetLR++;
								refresh=true;
								}
							break;
						
						case SDLK_p:
							//pause...
							break;
						
						case SDLK_ESCAPE:
							exit(0);
							break;
						} //WHICH KEY
					break; //IF KEY DOWN
				
				case SDL_QUIT: //IF EXIT
					exit(0);
					break; //IF EXIT
				} //WHICH EVENT
			} //GET INPUT
		}

//OUTPUT
//OUTPUT
	if (INallowOutput) //Gravity and Drop
		{
		if (drop==dropHard)																	 //HARD DROP
			{
			char tempLoop=true;
			while (tempLoop)
				{
				if (move(dwn,tetUD))
					{
					tetUD++;
					score+=2;
					//lockDelay=0;
					}
				else
					{
					tempLoop=false;
					lockDelay++;
					if (lockDelay>=waitLock)
						temppiecemoving=false;
					}
				}
			refresh=true;
			drop=0;
			}
		if (INwaited==0 || drop==dropSoft) //SOFT DROP or GRAVITY
			{
			refresh=true;
			if (move(dwn,tetUD))
				{
				if (drop==dropSoft)
					score++;
				tetUD++;
				lockDelay=0;
				}
			else
				{
				lockDelay++;
				}
			if (lockDelay>=waitLock)
				{
				temppiecemoving=false;
				lockDelay=0;
				}
			drop=0;
			}
		prevWaited=INwaited;
		/*if (score>readHigh(accesshigh))
			{
			writeHigh(accesshigh,score);
			highMade=true;
			}*/
		}
#ifdef DEBUG_STREAM
	printf(" - moving:%s - end\n",temppiecemoving?"yes":"no");
#endif
	return temppiecemoving; //THIS LINE WAS ADDED LATER
	}
	
	
	
	
void drawTetromino(int INtet,int INrot, int INx, int INy, int INreal)
	{
#ifdef DEBUG_STREAM
	printf("drawTet - start");
#endif
	for (int i=0; i<4; i++)
		{
		drawRect.x = (INx + tet[INtet+INrot][i  ]) * BLOCK_SIZE;
		drawRect.y = (INy + tet[INtet+INrot][i+4]) * BLOCK_SIZE;
		drawRect.w = BLOCK_SIZE;
		drawRect.h = BLOCK_SIZE;
		switch(INreal)
			{
			case REAL_TET:				//REAL TETROMINO
				SDL_FillRect(screen, &drawRect, SDL_MapRGB(screen->format,
					tetColors[INtet/4][0],
					tetColors[INtet/4][1],
					tetColors[INtet/4][2]));
				break;
			case GHOST_TET:				//GHOST TETROMINO
				SDL_FillRect(screen, &drawRect, SDL_MapRGB(screen->format,
					tetColors[INtet/4+7][0],
					tetColors[INtet/4+7][1],
					tetColors[INtet/4+7][2]));
				break;
			default:
				fprintf( stderr, "drawTetromino, bad draw type value \n");
				SDL_Quit();
				exit( -1 );
				break;
			}
#ifdef DRAW_TET_DETAILS
		printf("\n(%d,%d)",INtet,INrot);
#endif
		}
#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	}

	
	

void ClearScreen()
	{
#ifdef DEBUG_STREAM
	printf(" clear");
#endif
	drawRect.w = SCR_WIDTH;
	drawRect.h = SCR_HEIGHT;
	drawRect.x = 0;
	drawRect.y = 0;
	SDL_FillRect(screen, &drawRect, SDL_MapRGB(screen->format,
		otherColors[C_BACK][0],
		otherColors[C_BACK][1],
		otherColors[C_BACK][2]));
//	SDL_Flip(screen);
#ifdef DEBUG_STREAM
	printf("screen\n");
#endif
	}


	
	
void drawBoard(bool INallowDraw)
	{
#ifdef DEBUG_STREAM
	printf("drawBoard - start");
#endif
	if (INallowDraw)			//refresh the screen:
		{
		if (refresh)
			ClearScreen();	
		
		//AREA...
		drawRect.w = BLOCK_SIZE;
		drawRect.h = BLOCK_SIZE;
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
				if (test>0) //Piece
					{
					drawRect.x = x*BLOCK_SIZE;
					drawRect.y = (y-2)*BLOCK_SIZE; //2 rows are hidden
					SDL_FillRect(screen, &drawRect, SDL_MapRGB(screen->format,
						tetColors[test-1][0],
						tetColors[test-1][1],
						tetColors[test-1][2]));
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
		
		
		//NEXT PIECES
#ifdef SHOW_NEXT_PIECES
		for (int i=0; i<PREVIEW_PIECES; i++)//tet,rot,x,			y,			type
			drawTetromino(nextPiece[i], 2, AREA_WIDTH+1, (PREVIEW_PIECE_SPACING*i), REAL_TET); 
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
		
		//OUTER EDGE LINES
		drawRect.x = AREA_WIDTH*BLOCK_SIZE;
		drawRect.y = 0;
		drawRect.w = BLOCK_SIZE/2;
		drawRect.h = AREA_HEIGHT*BLOCK_SIZE;
		SDL_FillRect(screen, &drawRect, SDL_MapRGB(screen->format,
			otherColors[C_TEXT][0],
			otherColors[C_TEXT][1],
			otherColors[C_TEXT][2]));
		drawRect.x = ((float)AREA_WIDTH+5.5)*BLOCK_SIZE;
		SDL_FillRect(screen, &drawRect, SDL_MapRGB(screen->format,
			otherColors[C_TEXT][0],
			otherColors[C_TEXT][1],
			otherColors[C_TEXT][2]));
		
		//SCORE IN WINDOW NAME
		char tempCaption[40];
		sprintf(tempCaption,"TETRIS | Score:%d | Joseph Dykstra",score);
		SDL_WM_SetCaption(tempCaption, NULL);
		SDL_Flip(screen);
		}
#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	}

	
	
	
	
	
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
	if (showDisp==0) refresh=2;
	if (showDisp>waitDisp) disp=0;
	}
*/
