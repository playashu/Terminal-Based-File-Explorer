#include "header.h"

//***********************************************************//
//following function uses regex to parse the commands and all the arguments from
//the input string. 
//***********************************************************//
void parseCommand(string s){
    smatch match;
    curr_command.clear();
    regex com("^\\s*(\\w+)\\s*");
    regex args("^\"((~/)?.*?)\"\\s*");
    regex arg("^([~|a-zA-Z_/0-9.]+)\\s*");
    if(regex_search(s, match, com) == true) {
        curr_command.push_back(match[1]);
        s = match.suffix();
        while(regex_search(s, match, args) == true || regex_search(s, match, arg) == true) {
            curr_command.push_back(match[1]);
            s = match.suffix();
        }
    }
}

//***********************************************************//
//this function passses the command to respective function based on the command
//***********************************************************//
void executeCommand(string command){
    clearCommandSpace();
    parseCommand(command);
    cout<<"Status: ";
    if(!curr_command.empty()){
        if(curr_command[0]=="delete_file"){                 //function working tested!!
            if(curr_command.size()==2){
                string path=getAbsolutePath(curr_command[1]);
                deleteFile(path); 
            }else{
                cout<<"Enter Proper Argumetns";
            }
        }else if(curr_command[0]=="delete_dir"){            //function working tested!!
           if(curr_command.size()==2 && curr_command[1]!=""){
                string path=getAbsolutePath(curr_command[1]);
                deleteDir(path);
            }else{
                cout<<"Enter Proper Argumetns";
            }
        }else if(curr_command[0]=="create_file"){           //working tested!!
            if(curr_command[1]!="" && curr_command[2]!=""){
                string path = getAbsolutePath(curr_command[2]) + "/" + curr_command[1];
                createFile(path);
            }else{
                cout<<"Enter Proper Argumetns";
            }
        }else if(curr_command[0]=="create_dir"){            //working Tested!!
            if(curr_command[1]!="" && curr_command[2]!=""){
                string path = getAbsolutePath(curr_command[2]) + "/" + curr_command[1];    
                createDir(path);
            }else{
                cout<<"Enter Proper Argumetns";
            }
        }else if(curr_command[0]=="copy" || curr_command[0]=="move"){          //createFile not worknig no matter what!!
            if(curr_command[0]=="move"){
                moveFlag=1;
            }else{
                moveFlag=0;
            }
            if(curr_command.size()>=3){
                copy_move_multi(curr_command);
            }else{
                cout<<"Enter Proper Argumetns";
            }

        }else if(curr_command[0]=="rename"){
            if(curr_command[1]!="" && curr_command[2]!=""){ 
                renameFile(curr_command[1],curr_command[2]);
            }else{
                cout<<"Enter Proper Argumetns";
            }
        }else if(curr_command[0]=="goto"){
            if(curr_command[1]!=""){
                path=getAbsolutePath(curr_command[1]);
                gotoDirectory(path);
            }else{
                cout<<"Enter Proper Argumetns";
            }
        }else if(curr_command[0]=="search"){
            if(curr_command.size()==2){
                bool present = search(curr_command[1],curr_path);
                if(present){
                    cout<<"True";
                }else{
                    cout<<"False";
                }
            }else{
                cout<<"Enter Proper Argumetns";
            }
        }else if(curr_command[0]=="parse"){
            for(auto i: curr_command){
                cout<<i<<": ";
            }
        }else{
            cout<<"Enter a valid command";
        }
    }else{
        cout<<"Enter a valid command";
    }
    cout<<"\t\tPress Any Key to Continue";
    char c;
    cin>>c;
    clearCommandSpace();
    cout<<"** Command Mode **"<<setw(10)<<"cmd:";
   // enterCommandMode();
}