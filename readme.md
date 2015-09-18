# tetris

> tetris clone in c

![screenshot](https://github.com/ArtskydJ/tetris/blob/master/screenshot.PNG)

## How to compile

- Get MinGW
- Run this command:

```bash
gcc -std=c99 -o Tetris.exe Tetris.c -lmingw32 -lSDLmain -lSDL -lSDL_ttf -mwindows
```

## Known issues

- Never speeds up
- Does not save high scores
- Does not pause when you switch to a different window (but you can pause manually)

## License

[VOL](http://veryopenlicense.com)
