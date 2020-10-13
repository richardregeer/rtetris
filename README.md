# RTetris
RTetris is a Tetris clone by Richard Regeer.
This is the first game I have developed in C++. I created this project to learn C++ back in 1999.

It's written for DOS and using the [Allegro game library](https://liballeg.org/).
When you take a look at the source code, keep in mind all the comments are in my native language (dutch). Thats what I did back in 1999.

I found the source code in my archive and thought it will be nice to share it and preserve a part of my personal history.

The game can still be played using [DOSBox](https://www.dosbox.com/)

## Running the game
The game requires DOS to run. To be able to run it on a modern operating system, [DOSBox](https://www.dosbox.com/) can be used.

For the examples I will use `~/games/rtetris` as installation directory.

### Download the release (Mac OS X / Linux)
```bash
wget https://github.com/richardregeer/rtetris/archive/2.0.2.zip -O rtetris.zip \
    && unzip rtetris.zip -d ~/games \
    && mv ~/games/rtetris-2.0.2/ ~/games/rtetris \
    && rm rtetris.zip
```

### Install DOSBox
**Ubuntu**

```bash
sudo apt install dosbox
```

**Homebrew Mac OS-X**

Make sure [homebrew](https://brew.sh/) is installed.
```bash
 brew install dosbox
```

**Download**

Download [Dosbox](https://www.dosbox.com/download.php?main=1) and install it manually.

### Starting the game
```
~/games/rtetris/RTetris.sh
```

## How to play
### Controls
```
Move Left      :Left arrow
Move Right     :Right arrow 
Down           :Down arrow 
Rotate         :Space
Pause game     :'p'
Help           :F1
Quit           :Esc
```

### Reset the highscores
To reset the highscores delete the `highs.dat` and restart RTetris. RTetris will create a new default highscore dat file.

### Soundcard
RTetris will autodetect your soundcard, if no soundcard is detected sound will be disabled.
A Sound Blaster Live card is not supported.

## Building
Did not investigate if the code can still be compiled with modern compilers. 
I developed the code and compiled it using [DJGPP](http://www.delorie.com/djgpp/) back in 1999.