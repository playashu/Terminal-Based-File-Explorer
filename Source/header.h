//***********************************************************//
//This file includes definiton of all the functions and variables
//required by the program
//***********************************************************//

#include<iostream>
#include<iomanip>
#include <unistd.h>
#include<vector>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <string>
#include <stack>
#include <cmath>
#include <pwd.h>
#include <fcntl.h>
#include <grp.h>
#include <time.h>
#include <regex>
#include <termios.h>
#include <fstream>
#include <unistd.h>
using namespace std;
#define LIMIT 20
#define KEY_ESCAPE 	0x001b
#define KEY_ENTER 	0x000a
#define BACKSPACE 	127
#define KEY_UP		65
#define KEY_DOWN	66
#define KEY_RIGHT   67 
#define KEY_LEFT	68                 
#define COLON		58
#define START 3 
#define STATUS_ROW 2
#define COMMAND_ROW 0

extern void printHeader();
extern int cursor_position;
extern int list_top;
extern int moveFlag;
extern int mode;
extern vector<string> files;
extern vector<string> curr_command;
//static vector<int> checkDir;
extern string curr_path;
extern string HOME;
extern stack<string> leftStack,rightStack;
extern void enterFolder();
extern void deleteDir(string);
extern void deleteFile(string);
//extern void copy_move_multi(vector<string>);
extern void createDir(string);

extern void renameFile(string,string);
extern void createFile(string);
extern int kbhit(void);
extern int getch(void);
extern int kbesc(void);
extern int kbget(void);
extern string getHomeDir(void);
extern string getPresentDirectory(void);
extern void gotoXY(int,int);
extern void clearScreen();
extern string formatSize(double);
extern string getSize(string);
extern string getAbsolutePath(string);
extern int checkDir(string);
extern string getSecutityMeta(string);
extern string trim(string);
extern vector<string> getFiles(const string);
extern void nonCanonicalMode(void);
extern void printFiles(void);
extern void clearCommandSpace(void);
extern void parseCommand(string);
extern int copyFile(string,string);
extern int copyDir(string,string);
extern void deleteFile(string);
extern void deleteDir(string);
extern bool search(string,string);
extern void executeCommand(string);
extern void enterCommandMode(void);
extern void commandMode_keyListner(void);
extern void normalMode_keyListner(void);
extern void enterFolder(void);
extern void gotoDirectory(string);
extern void printStatus();
extern void copy_move_multi(vector<string>);
//################################################################################################################################################