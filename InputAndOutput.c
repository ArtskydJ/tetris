//InputOutputAndDraw.c
//Input, Output, and Drawing functions for Tetris
//Written by Joseph Dykstra


bool inputOutput(int INwaited)
	{
#ifdef DEBUG_STREAM
	printf("inputOutput - start");
#endif
//INPUT
//INPUT
	SDL_Event test_event;
	while (SDL_PollEvent(&test_event)) //GET INPUT
		{
		switch (test_event.type) //WHICH EVENT
			{
			case SDL_KEYDOWN: //KEYDOWN
				if (!paused) //IF NOT PAUSED
					{
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
#ifdef ARROW_KEYS							//HOLD PIECE
						case SDLK_c:
#else
						case SDLK_KP0:
#endif
							if (!usedHoldPiece)
								{
								usedHoldPiece=true;
								pieceCreate(HOLD);
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
								}
							break;
						}
					} //IF NOT PAUSED
				switch (test_event.key.keysym.sym) //WHICH KEY
					{
					case SDLK_p:
						paused = !paused;
						break;
					
					case SDLK_ESCAPE:
						exit(0);
						break;
					} //WHICH KEY
				break; //IF KEY DOWN
			
			case SDL_KEYUP:
				switch (test_event.key.keysym.sym)
					{
#ifdef ARROW_KEYS							//SOFT DROP END
					case SDLK_DOWN:
#else
					case SDLK_KP2:
#endif
						drop=0;
						break;
					}
				break;
			
			
			case SDL_QUIT: //IF EXIT
				exit(0);
				break; //IF EXIT
			} //WHICH EVENT
		} //GET INPUT

//OUTPUT
//OUTPUT
	char temppiecemoving=true; //return this
	if (!paused) //Gravity and Drop
		{
		if (drop==dropHard)							 //HARD DROP
			{
			char tempLoop=true;
			while (tempLoop)
				{
				if (move(dwn,tetUD))
					{
					tetUD++;
					score+=2;
					}
				else
					{
					tempLoop=false;
					temppiecemoving=false;
					}
				}
			drop=0;
			}
		if (INwaited==0 || drop==dropSoft)			//SOFT DROP or GRAVITY
			{
			if (move(dwn,tetUD))
				{
				if (drop==dropSoft)
					score++;
				tetUD++;
				lockDelay=0;
				}
			else
				{
				if (drop!=dropSoft)
					lockDelay++;
				}
			if (lockDelay>=waitLock)
				{
				temppiecemoving=false;
				lockDelay=0;
				}
			//drop=0;
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

