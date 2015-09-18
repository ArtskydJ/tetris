char move(int INdir, int INud)
	{
	int cantMove=0;
	int addLR  =0;
	int addUD  =0;
	int tempLR =0;
	int tempUD =0;
	if		(INdir==dwn) addUD = 1;
	else if (INdir==lft) addLR =-1;
	else if (INdir==rht) addLR = 1;
	for (int i=0; i<4; i++)
		{
		tempLR = tet[currentPiece][X][currentRotate][i]+tetLR+addLR;
		tempUD = tet[currentPiece][Y][currentRotate][i]+INud +addUD+2; //2 hidden rows
		if (tempLR>=0 && tempLR<AREA_WIDTH && tempUD>=0 && tempUD<AREA_HEIGHT+2)
			cantMove+=(area[tempLR][tempUD])>EMPTY_AREA;
		else
			cantMove++;
		}
#ifdef DEBUG_STREAM
	printf(" - can_move: %s",(cantMove==false)?"yes":"no");
#endif
	return (cantMove==false);	//cantMove in:	0123	0123
	}						//cantMove out:	1000	TFFF


char rotate(int INdir)
	{
	int cantRotate=1, addRot=0, tempLR, tempUD, addUD=0, addLR=0;
	if (INdir==lft)	addRot=(currentRotate+3)%4;
	else			addRot=(currentRotate+1)%4;
	char rotatePos[5]={0,1,-1,2,-2};	// -Left,Up +Right,Down
	for (int i=0; (i<5 && cantRotate>0); i++) // [i<5] was [i<3]
		{
		addUD=rotatePos[i];
		if (currentPiece!=Itet && i>=3)
			addUD=rotatePos[2]; //disable jumping 2
		for (int j=0; (j<5 && cantRotate>0); j++)
			{
			addLR=rotatePos[i];
			if (currentPiece!=Itet && i>=3)
				addLR=rotatePos[2]; //disable jumping 2
			cantRotate=0;
			for (int k=0; k<4; k++)
				{// tet[tet][xORy][rot][PIECE_PART]
				tempLR = tetLR + tet[currentPiece][X][addRot][k] + addLR;
				tempUD = tetUD + tet[currentPiece][Y][addRot][k] + addUD + 2; //2 hidden rows (might not fix bug)
				if (tempLR>=0 && tempUD>=0 && tempLR<AREA_WIDTH && tempUD<AREA_HEIGHT+2)
					cantRotate += area[tempLR][tempUD]>EMPTY_AREA; //empty=-1, 'I' tetromino=0, add 1
				else
					cantRotate++;
				}
			}
		}
	if (cantRotate==false)
		{
#ifdef ONE_FLOOR_KICK
		if (!(addUD>0 && floorKickUsed==true))
			{
			tetLR+=addLR;
			tetUD+=addUD;
			currentRotate=addRot;
			if (addUD>0)
				floorKickUsed=true;
			}
#else
		tetLR+=addLR;
		tetUD+=addUD;
		currentRotate=addRot;
#endif
		}
#ifdef DEBUG_STREAM
	printf(" - can rotate: %s",(cantRotate==false)?"yes":"no");
#endif
	return (cantRotate==false);
	}
	

void generate7Pieces(int *INarray)
	{
	for (int i=0; i<7; i++)
		{
		pieceCreated[i]=0;
		}
	
	for (int i=0; i<7; i++)
		{
		char tPiece = rand()%(7-i);
		
		for (int j=0; j<7; j++)
			if (tPiece>=j && pieceCreated[j])
				tPiece++;
		
		INarray[i]=tPiece;
		pieceCreated[tPiece]=true;
		}
		
	/*char tText[30];
	sprintf(tText,"%d  %d  %d  %d  %d  %d  %d\n",
		INarray[0], INarray[1], INarray[2], INarray[3],
		INarray[4], INarray[5], INarray[6]);
	EZ_apply_text(screen,tText,font,EZ_new_rect(10,10,0,0),EZ_new_color(100,100,100));
	SDL_Flip(screen);
	SDL_Delay(5000);*/
	}

	

void pieceCreate(bool INcreateHold)
	{
#ifdef DEBUG_STREAM
	printf("pieceCreate - start");
#endif

	int tPiece = currentHoldPiece;
	if (INcreateHold==HOLD) //get the piece from the hold
		{
		currentHoldPiece = currentPiece;	//Switch the hold and current pieces
		if (tPiece!=EMPTY_AREA)
			currentPiece = tPiece;
		}
	if (INcreateHold==COMING || tPiece==EMPTY_AREA) //create new piece
		{
		currentPiece=nextPiece[0];
		for (int i=1; i<7; i++)
			{
			nextPiece[i-1] = nextPiece[i];
			}
		nextPiece[6]=nextPieceRefresh[0];
		for (int i=1; i<7; i++)
			{
			nextPieceRefresh[i-1] = nextPieceRefresh[i];
			}
		nextPieceRefresh[6]=-1;
		if (nextPieceRefresh[0]<0)
			generate7Pieces(nextPieceRefresh);
		pieceCount[currentPiece]++;
		pieceCount[ALL]++;
		}

	currentRotate	= 2;
	prevWaited		= 0;
	drop			= 0;
	lockDelay		= 0;
	floorKickUsed	= 0;
	tetLR			= (AREA_WIDTH/2)-2;
	tetUD			= -2;
	piecemoving		= true;
	alive			= true;

#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	}


void pieceSetInto(int pieceType)
	{
#ifdef DEBUG_STREAM
	printf("pieceSetInto - start");
#endif
	usedHoldPiece=false;
	int tempLR,tempUD;
	for (int i=0; i<4; i++)
		{// tet[tet][xORy][rot][PIECE_PART]
		tempLR = tet[currentPiece][X][currentRotate][i]+tetLR;
		tempUD = tet[currentPiece][Y][currentRotate][i]+tetUD+2; //2 upper hidden rows...
		area[tempLR][tempUD]=pieceType;
		}
#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	}


/*
go down...
if full...
	start going up
	overwrite the one below with current
	until fully up
do not restart at previous full
*/
int clearFullRows()
	{
#ifdef DEBUG_STREAM
	printf("clearFullRows - start");
#endif
	int blocksInRow=0;
	int numOfFilledLines=0;
	for (int i=0; i<AREA_HEIGHT+2; i++)		//From the top, check all rows...
		{
		blocksInRow=0;
		for (int j=0; j<AREA_WIDTH; j++)		//Add up number of blocks in row...
			blocksInRow+=(area[j][i]>EMPTY_AREA);
		
		if (blocksInRow>=AREA_WIDTH)				//If the row is full...
			{
			for (int k=i; k>=0; k--)						//...start going up the rows
				{
				for (int j=0; j<AREA_WIDTH; j++)
					{
					//printf("clear:%d",area[j][k]);
					if (k>0)	area[j][k]=area[j][k-1];		//...and overwrite current row with row above it
					else		area[j][k]=EMPTY_AREA;			//...if no row above it, empty it.
					}
				}
			numOfFilledLines++;
			}
		if (blocksInRow>0 && i<2)					//If the row has some blocks too high...
			{
			alive=false;								//...then ur dead, lol.
			}
		}

	//add to score
	switch(numOfFilledLines)
		{
		case 1:
			score+=100;
			prevQuadClear=false;
			break;
		case 2:
			score+=300;
			prevQuadClear=false;
			break;
		case 3:
			score+=500;
			prevQuadClear=false;
			break;
		case 4:
			if (!prevQuadClear)
				score+=800;
			else
				score+=1200;
			prevQuadClear=true;
			break;
		}
	lines+=numOfFilledLines;
	
#ifdef DEBUG_STREAM
	printf("full=%d\n",numOfFilledLines);
	printf(" - end\n");
#endif
	return numOfFilledLines;
	}