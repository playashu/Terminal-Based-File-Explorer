#include "header.h"

//***********************************************************//
//this file implements the key listners and implements various functions of the keys.
//***********************************************************//
void commandMode_keyListner(){
    string command;
    char c;
    c=getch();
    while(true){
        switch(c){
            case BACKSPACE:
                command=command.substr(0,command.size()-1);
                clearCommandSpace();
                cout<<"** Command Mode **"<<setw(10)<<"cmd:";
                cout<<command;
                break;
            case KEY_ENTER:
                executeCommand(command);
                command.clear();
                break;
            case '`':
                fflush(0);
                exit(0);
            case KEY_ESCAPE:
                mode=0;
                enterFolder();
            default:
                command.push_back(c);
                cout<<c;
        }
        c=getch();
    }
}

void normalMode_keyListner(){
    char c;
    c=kbget();
    while(c!='q'){
        switch(c){
            case KEY_DOWN:
                if(cursor_position!=LIMIT && cursor_position<files.size()){
                    gotoXY(++cursor_position,0);
                }
                break;
            case KEY_UP:
                if(cursor_position!=1){
                    gotoXY(--cursor_position,0);
                }
                break;
            case int('k'):
                if(cursor_position==1 && list_top>0){
                    list_top--;
                    clearCommandSpace();
                    cout<<"** Normal Mode **";
                    gotoXY(cursor_position,0);
                }
                break;
            case int('l'):
                if(cursor_position==LIMIT && list_top<files.size()-LIMIT){
                    list_top++;
                    clearCommandSpace();
                    cout<<"** Normal Mode **";
                    gotoXY(cursor_position,0);
                }
                break;
            case KEY_LEFT:
                if(rightStack.size()>=1){
                    
                    leftStack.push(rightStack.top());
                    rightStack.pop();
                    curr_path=leftStack.top();
                    enterFolder();
                }
                break;
            case KEY_RIGHT:
                if(leftStack.size()>1){
                   
                    rightStack.push(leftStack.top());
                    leftStack.pop();
                    curr_path=leftStack.top();
                    enterFolder();
                }
                break;

            case KEY_ENTER://
                if(checkDir(files[cursor_position+list_top-1])){
                    if(files[cursor_position+list_top-1]==".."){
                        int last = curr_path.find_last_of("/");
                        if(last!=0){
                            leftStack.push(curr_path.substr(0, last));
                            curr_path=leftStack.top();
                        }    
                    }else if(files[cursor_position+list_top-1]!="."){
                        leftStack.push(getAbsolutePath(files[cursor_position+list_top-1]));
                        curr_path=leftStack.top();
                    }
                   
                    while(!rightStack.empty()){
                        rightStack.pop();            
                    }
                    enterFolder();
                }else{
                    // int pid = fork();
                    // if (pid == 0) {
                    //   execl("/usr/bin/xdg-open", "xdg-open", getAbsolutePath(files[cursor_position]).c_str(), (char *)0);
                    //   exit(1);
                    // }
                     pid_t pid = fork();
                    if (pid == 0) {
                       // child process
                       execlp("open","open", getAbsolutePath(files[cursor_position+list_top-1]).c_str(), NULL);
                       _exit(127);
                    }
                    //system(getAbsolutePath(files[cursor_position]).c_str());
                    //cout<<getAbsolutePath(files[cursor_position+list_top-1]).c_str();
                }
                break;
            case 'h':
                leftStack.push(HOME);

                curr_path=leftStack.top();
                enterFolder();
                break;
            case ':':
                mode=1;
                enterCommandMode();
                break;
            case KEY_ESCAPE:
                 fflush(0);
                exit(0);
            case BACKSPACE:
                int last = curr_path.find_last_of("/");
                if(last!=0){
                    curr_path = curr_path.substr(0, last); 
                    leftStack.push(curr_path);
                    while(!rightStack.empty()){
                        rightStack.pop();            
                    }
                    enterFolder();
                }
                //cout<<curr_path;  
                break; 

        }
        c=kbget();
    }
    clearScreen();
    fflush(0);
    exit(0);
}