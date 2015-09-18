char move(int INdir, int INud)
	{
	int canMove=0;
	int addLR  =0;
	int addUD  =0;
	int tempLR =0;
	int tempUD =0;
	if		(INdir==dwn) addUD = 1;
	else if (INdir==lft) addLR =-1;
	else if (INdir==rht) addLR = 1;
	for (int i=0; i<4; i++)
		{
		tempLR = tet[currentPiece+currentRotate][i  ]+tetLR+addLR;
		tempUD = tet[currentPiece+currentRotate][i+4]+INud +addUD+2; //2 hidden rows
		if (tempLR>=0 && tempLR<AREA_WIDTH && tempUD>=0 && tempUD<AREA_HEIGHT+2)
			canMove+=(area[tempLR][tempUD])>0;
		else
			canMove++;
		}
#ifdef DEBUG_STREAM
	printf(" - can_move: %s",(canMove==0)?"yes":"no");
#endif
	return (canMove==0);	//canMove in:	0123	0123
	}						//canMove out:	1000	TFFF


char rotate(int INdir)
	{
	int cantRotate=1, addRot=0, tempLR, tempUD, addUD=0, addLR=0;
	if (INdir==lft)	addRot=(currentRotate+3)%4;
	else			addRot=(currentRotate+1)%4;
	char rotatePos[5]={0,1,-1,2,-2};	// -Left,Up +Right,Down
	for (int i=0; (i<5 && cantRotate>0); i++) // [i<5] was [i<3]
		{
		addUD=rotatePos[i];
		if (currentPiece/4!=Itet && i>2)
			addUD=rotatePos[2];
		for (int j=0; (j<5 && cantRotate>0); j++)
			{
			addLR=rotatePos[i];
			if (currentPiece/4!=Itet && i>2)
				addLR=rotatePos[2];
			cantRotate=0;
			for (int k=0; k<4; k++)
				{
				tempLR = tetLR + tet[currentPiece+addRot][k  ] + addLR;
				tempUD = tetUD + tet[currentPiece+addRot][k+4] + addUD + 2; //2 hidden rows (might not fix bug)
				if (tempLR>=0 && tempUD>=0 && tempLR<AREA_WIDTH && tempUD<AREA_HEIGHT+2)
					cantRotate += area[tempLR][tempUD];
				else
					cantRotate++;
				}
			}
		}
	if (cantRotate==0)
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
	printf(" - can rotate: %s",(cantRotate==0)?"yes":"no");
#endif
	return (cantRotate==0);
	}
	
	
	

void pieceCreate()
	{
#ifdef DEBUG_STREAM
	printf("pieceCreate - start");
#endif
	int pieceToCreate=prob;
	int rndm=(rand())%(chnc[prob]);
	int add=0;
	for (int j=0; (j<=Ztet)&&(pieceToCreate==prob); j++)
		{
		add+=chnc[j];
		if (add>rndm) pieceToCreate=j*4;
		}
	if (pieceToCreate==prob)
		{
		ClearScreen();
		TextOutput(0,24,add,0);
		TextOutput(0,16,rndm,0);
		TextOutput(0,00,0,0);
		Wait(1000);
		}
	currentPiece=nextPiece[0];
	for (int i=1; i<PREVIEW_PIECES; i++)
		{
		nextPiece[i-1]=nextPiece[i];
		}
	nextPiece[PREVIEW_PIECES-1]=pieceToCreate;

	lockDelay		= 0;
	//prevRht			= 0;
	//prevLft			= 0;
	//prevDrp			= 0;
	//prevRot			= 0;
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
	int tempLR,tempUD;
	for (int i=0; i<4; i++)
		{
		tempLR = tet[currentPiece+currentRotate][i  ]+tetLR;
		tempUD = tet[currentPiece+currentRotate][i+4]+tetUD+2; //2 upper hidden rows...
		area[tempLR][tempUD]=(pieceType/4)+1; //tetI=0, and Empty=0, so add 1
		}
#ifdef DEBUG_STREAM
	printf(" - end\n");
#endif
	}


int clearFullRows()
	{
#ifdef DEBUG_STREAM
	printf("clearFullRows - start");
#endif
/*
go down...
if full...
	start going up
	overwrite the one below with current
	until fully up
do not restart at previous full
*/
	int blocksInRow=0;
	int numOfFilledLines=0;
	for (int i=0; i<AREA_HEIGHT+2; i++)		//From the top, check all rows...
		{
		blocksInRow=0;
		for (int j=0; j<AREA_WIDTH; j++)		//Add up number of blocks in row...
			blocksInRow+=(area[j][i]>0);
		
		if (blocksInRow>=AREA_WIDTH)				//If the row is full...
			{
			for (int k=i; k>=0; k--)						//...start going up the rows
				{
				for (int j=0; j<AREA_WIDTH; j++)
					{
					//printf("clear:%d",area[j][k]);
					if (k>0)	area[j][k]=area[j][k-1];		//...and overwrite current row with row above it
					else		area[j][k]=0;					//...if no row above it, empty it.
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
	if (numOfFilledLines<0)
		disp=numOfFilledLines;
	
#ifdef DEBUG_STREAM
	printf("full=%d\n",numOfFilledLines);
	printf(" - end\n");
#endif
	return numOfFilledLines;
	}