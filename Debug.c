void debugInfo(int INint)
	{
	printf("DEBUG STREAM\n");
	printf("spdup:%d\tloop#:%d\t drop:%d\talive:%d\n",	speedup,		INint,			drop,		alive);
	printf("score:%d\tdlytm:%d\tlkDly:%d\tprevW:%d\n",	score,			delaytime,		lockDelay,	prevWaited);
	//printf("prevL:%d\tprevR:%d\tprevD:%d\tprevR:%d\n",	prevLft,		prevRht,		prevDrp,	prevRot);
	printf("currP:%d\tcurrR:%d\ttetLR:%d\ttetUD:%d\n",	currentPiece,	currentRotate,	tetLR,		tetUD);
	printf("DEBUG STREAM\n");
	}


void debug(int INmode)
	{
	ClearScreen();
	SDL_Flip(screen);
	selectGame(1);
	gameResetVars();

	if (INmode==2) //ClearFullRows()
		{
		for (int i=0; i<AREA_HEIGHT+2; i++)
			{
			for (int j=0; j<AREA_WIDTH; j++)
				{
				area[j][i] = (rand()%15)-EMPTY_AREA;
				if (area[j][i]>EMPTY_AREA)
					area[j][i]=area[j][i]%3+1;
				}
			}
		drawBoard();
		SDL_Delay(5000);
		clearFullRows();
		SDL_Delay(250);
		drawBoard();
		SDL_Delay(5000);
		}

	else if (INmode==3) //drawTetromino()
		{
		int tets,angl;
		for (tets=0; tets<7; tets++)
			{
			for (angl=0; angl<4; angl++)
				{
				ClearScreen();
				drawTetromino(tets,(angl+2)%4,0,0,REAL_TET);
				SDL_Flip(screen);
				SDL_Delay(600);
				}
			}
		}

	else if (INmode==4) //peiceSetinto()
		{
		pieceCreate(COMING);
		tetUD=10;
		
		drawTetromino(currentPiece,currentRotate,tetLR,tetUD,REAL_TET);
		SDL_Delay(3000);
		
		drawBoard();
		SDL_Delay(3000);
		
		pieceSetInto(currentPiece);
		drawBoard();
		SDL_Delay(3000);
		
		debugInfo(0);
		}
	
	else if (INmode==5) //peiceCreate()
		{
		for (int i=0; i<10; i++)
			{
			pieceCreate(COMING);
			ClearScreen();
			printf("c%d n%d n%d n%d\n",currentPiece,nextPiece[0],nextPiece[1],nextPiece[2]);
			SDL_Delay(100);
			}
		}
	
	else if (INmode==6) //gravity
		{
		pieceCreate(COMING);
		while(tetUD<AREA_HEIGHT) //off by 2?
			{
			ClearScreen();
			//drawBoard(true);
			drawTetromino(currentPiece,currentRotate,tetLR,tetUD,REAL_TET);
			printf("y=%d",tetUD);
			SDL_Delay(1000);
			tetUD++;
			}
		}
	}
	
	
	
/*
// OLD debugStream()
printf("su %d   lp %d\n",disp,accesshigh,speedup,INint); //change ds and ah to [%s]
printf("sc %d   gt %d   gs %d   dt %d   lD %d\n",score,gametype,gamespeed,delaytime,lockDelay);
printf("pW %d   pL %d   pR %d   pD %d   pR %d\n",prevWaited,prevLft,prevRht,prevDrp,prevRot);
printf("cP %d   cR %d   tL %d   tU %d\n",currentPiece,currentRotate,tetLR,tetUD);
printf("dp %d   lv %d   pQ %d   hM %d\n",drop,alive,prevQuadClear,highMade);
*/
