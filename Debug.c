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
	printf("Debug #%d\n",INmode);
	SDL_Delay(2000);
	prevQuadClear=false;
	selectGame(1);
	gameResetVars();
	score=rand()%20000+10000;
	ClearScreen();

	if (INmode==2) //ClearFullRows()
		{
		for (int i=0; i<AREA_HEIGHT+2; i++)
			{
			for (int j=0; j<AREA_WIDTH; j++)
				{
				area[j][i]=rand()%15;
				if (area[j][i]>0)
					area[j][i]=area[j][i]%3+1;
				}
			}
		refresh=true;
		drawBoard(true);
		SDL_Delay(5000);
		clearFullRows();
		SDL_Delay(250);
		refresh=true;
		drawBoard(true);
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
				drawTetromino(tets*4,angl,0,0,REAL_TET);
				SDL_Delay(1000);
				}
			}
		}

	else if (INmode==4) //peiceSetinto()
		{
		pieceCreate();
		tetUD=10;
		
		drawTetromino(currentPiece,currentRotate,tetLR,tetUD,REAL_TET);
		SDL_Delay(3000);
		
		drawBoard(true);
		SDL_Delay(3000);
		
		pieceSetInto(currentPiece);
		drawBoard(true);
		SDL_Delay(3000);
		
		debugInfo(0);
		}
	
	else if (INmode==5) //peiceCreate()
		{
		for (int i=0; i<10; i++)
			{
			pieceCreate();
			ClearScreen();
			printf("c%d n%d n%d n%d\n",currentPiece/4,nextPiece[0]/4,nextPiece[1]/4,nextPiece[2]/4);
			SDL_Delay(100);
			}
		}
	
	else if (INmode==6) //gravity
		{
		pieceCreate();
		while(tetUD<AREA_HEIGHT)
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
printf("ds %d   ah %d   su %d   lp %d\n",disp,accesshigh,speedup,INint); //change ds and ah to [%s]
printf("sc %d   gt %d   gs %d   dt %d   lD %d\n",score,gametype,gamespeed,delaytime,lockDelay);
printf("pW %d   pL %d   pR %d   pD %d   pR %d\n",prevWaited,prevLft,prevRht,prevDrp,prevRot);
printf("cP %d   cR %d   tL %d   tU %d\n",currentPiece,currentRotate,tetLR,tetUD);
printf("dp %d   lv %d   pQ %d   hM %d\n",drop,alive,prevQuadClear,highMade);
*/
