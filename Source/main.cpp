#include "header.h"
#include "processKeys.h"
#include "utilities.h"
#include "fileDetails.h"
#include "keyListner.h"
#include "parseCommand.h"
#include "operations.h"

using namespace std; 

int cursor_position=0;
int list_top=0;
int moveFlag=0;
int mode=0;
string curr_path;
/// handle when command empty!!!
vector<string> files;
vector<string> curr_command;
string HOME;
stack<string> leftStack,rightStack;

int main(){
    curr_path=getPresentDirectory();
    HOME=getHomeDir();
    leftStack.push(curr_path);
    nonCanonicalMode();
    clearScreen();
    enterFolder();
    return 0;
}