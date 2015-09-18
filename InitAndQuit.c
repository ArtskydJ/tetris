void initReset(void)
	{
	printf("\n");
#ifdef DEBUG_STREAM
	printf("initReset - start");
#endif

	atexit(quit);
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0)
		{
		printf("SDL_Init() bad");
		fprintf( stderr, "Could not initialize SDL: %s\n", SDL_GetError() );
		quit();
		exit(SDL_INIT_ERROR);
		}
	else printf("SDL_Init() good");

	screen = SDL_SetVideoMode( SCR_WIDTH, SCR_HEIGHT, SCR_BPP, SDL_SWSURFACE );
	if( !screen )
		{
		printf("SDL_SetVideoMode() bad");
		fprintf( stderr, "Could not set video mode: %s\n", SDL_GetError() );
		exit(VIDEO_INIT_ERROR);
		}
	else printf("SDL_SetVideoMode() good");
	if( TTF_Init() == -1 )
		{
		printf("TTF_Init() bad");
		fprintf( stderr, "Could not initialize TTF: %s\n", SDL_GetError() );
		exit(TTF_INIT_ERROR);
		}
	else printf("TTF_Init() good");
	font = TTF_OpenFont("arial.ttf", TEXT_SIZE);
	if( font == NULL )
		{
		printf("TTF_OpenFont() bad");
		exit(OPEN_FONT_ERROR);
		}
	else printf("TTF_OpenFont() good");
	
	icon = SDL_CreateRGBSurface(SDL_SWSURFACE, 64, 64, SCR_BPP, 0, 0, 0, 255);
	if(icon == NULL)
		{
		printf("SDL_CreateRGBSurface() bad");
		fprintf(stderr, "CreateRGBSurface failed with icon: %s\n", SDL_GetError());
		exit(ICON_SURFACE_ERROR);
		}
	else printf("SDL_CreateRGBSurface() good");
	
	EZ_apply_rect(icon, EZ_new_rect(0,	0,	64,	64), otherColors[C_BACK]);
	EZ_apply_rect(icon, EZ_new_rect(48,	8,	16,	48), otherColors[C_TEXT]);
	EZ_apply_rect(icon, EZ_new_rect(16,	40,	16,	16), otherColors[C_TEXT]);
	SDL_WM_SetIcon(icon,NULL);
	
	SDL_WM_SetCaption("Tetris | Joseph Dykstra", NULL);
	
	SDL_EnableKeyRepeat(150,10); //Defaults are 500 and 30, respectively.
#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	}


void selectGame(char INfastmode)
	{
#ifdef DEBUG_STREAM
	printf("selectGame - start");
#endif
	if (INfastmode) //debugging
		{
		gamespeed=0;		//0=slow
		}
	else
		{
		//choose gamespeed
		}

	switch(gamespeed)
		{
		case 0:
			speedup=1; //speedup=0.975;
			delaytime=120;
			break;
		case 1:
			speedup=0.9375;
			delaytime=100;
			break;
		case 2:
			speedup=0.9;
			delaytime=80;
			break;
		}

#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	}
	

void gameResetVars(void)
	{
#ifdef DEBUG_STREAM
	printf("gameResetVars - start");
#endif
	for (int i=0; i<AREA_HEIGHT+2; i++)
		{
		for (int j=0; j<AREA_WIDTH; j++)
			{
			area[j][i]=EMPTY_AREA;
			}
		}
		
	generate7Pieces(nextPiece);
	generate7Pieces(nextPieceRefresh);
	
	for (int i=0; i<8; i++)
		{
		pieceCount[i]=0;
		}
	
	for (int i=0; i<7; i++)
		{
		pieceCreated[i]=0;
		}
	
	/*
	1	tPc = rand()%6
	2	nextpiece[0] = tPc
	
	3	tPc = rand()%5
	4	if pieceCreated[n] and tPc>=n then tPc++
	
	5	tPc = rand()%4
	6	if tPc>=nextpiece[0] then tPc++
	7	if tPc>=nextpiece[1] then tPc++
	
	8	tPc = rand()%3
	9	if tPc>=nextpiece[0] then tPc++
	10	if tPc>=nextpiece[1] then tPc++
	11	if tPc>=nextpiece[2] then tPc++
	*/
	
	viewDbg			= 0;
	lockDelay		= 0;
	currentPiece	= 0;
	currentRotate 	= 0;
	currentHoldPiece= EMPTY_AREA;
	usedHoldPiece	= false;
	tetLR			= 0;
	tetUD			= 0;
	drop			= 0;
	prevWaited		= 0;
	speedup			= 0;
	score			= 0;
	alive			= true;
	playagain		= true;
	piecemoving		= true;
	paused 			= false;
	prevQuadClear	= false;
#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
}


	
char gameEnd(void)
	{
#ifdef DEBUG_STREAM
	printf("gameEnd - start");
#endif
	//drawBoard();
	//bool tPlayAgain=false;
	//SDL_Delay(2500);
	//if (highMade) writeHighName(accesshigh,score);
	//ASK TO PLAY AGAIN
#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	return 0;//tPlayAgain;
	}


void quit(void)
	{
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
	}

	
	
	
// was in init()
// icon creation code
		
		
	/*drawRect.w = 64;
	drawRect.h = 64;
	drawRect.x = 0;
	drawRect.y = 0;
	SDL_FillRect(icon, &drawRect, SDL_MapRGB(icon->format,
		otherColors[C_BACK][0],
		otherColors[C_BACK][1],
		otherColors[C_BACK][2]));*/
		
		
	/*drawRect.w = 16;
	drawRect.h = 48;
	drawRect.x = 48;
	drawRect.y = 8;
	SDL_FillRect(icon, &drawRect, SDL_MapRGB(icon->format,
		tetColors[Jtet][0],
		tetColors[Jtet][1],
		tetColors[Jtet][2]));
	drawRect.w = 16;
	drawRect.h = 16;
	drawRect.x = 16;
	drawRect.y = 40;
	SDL_FillRect(icon, &drawRect, SDL_MapRGB(icon->format,
		tetColors[Jtet][0],
		tetColors[Jtet][1],
		tetColors[Jtet][2]));*/