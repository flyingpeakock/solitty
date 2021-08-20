# Solitty

Simple ncurses program to play sudoku in the terminal
written entirely in C++

## How to Play
Move around the board and select cards using the keys set in ```config.h```.
To move a card or a stack of cards first select the card you with to move 
then select the stack you want to move it onto.

## How to configure
Configuration is done in the config.h file. Documentation for the
configuration is in the form of comments in that same file.
All configuration must be done before installation.
Changing any configuration will require reinstallation.

## How to Install
Clone this repository then compile using g++ or clang++

### Linux
```
git clone https://github.com/flyingpeakock/solitty.git
cd solitty/
g++ -Ofast -pthread ./src/*.cpp -lncurses -o solitty
```

### OSX
```
git clone https://github.com/flyingpeakock/solitty.git
cd solitty/
clang++ -Ofast -pthread -std=c++11 -stdlib=libc++ ./src/*.cpp -lncursesw -o solitty
```
-lncursesw flag is required to link curses.h

### Aur
This package can be found as solitty-git on the AUR.
```
paru -S solitty-git

# If you wish to edit config.h
vim ~/.cache/paru/clone/solitty-git/Config.h
paru -S --rebuild solitty-git
```

To run this program from any directory move the generated solitty file to anywhere in you $PATH
```
ln -s solitty ~/.local/bin/solitty
```

### Screenshot
![new game](https://i.imgur.com/sMBQvif.png)

### Troubleshooting
If the compiler cannot find ncurses.h you need to make sure that ncurses is installed.
If you cannot find ncurses in your package manager it might be called something else like
libcurses or curses instead. If it still doesn't work use the -lncurses flag instead of -lncursesw when compiling.
