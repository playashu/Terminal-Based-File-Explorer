#include "header.h"

//****************************************//
//return the present user's home directory//
//***************************************//
string getHomeDir(){
    struct passwd *pw = getpwuid(getuid());
    string homedir = pw->pw_dir;
    return homedir;
}

//********************************//
//return present working directory
//********************************//
string getPresentDirectory(){
    char cwd[500];
    getcwd(cwd, 500);       //if path length greater than 500, will 
                            //return NULL and 
                            //set errno variable (definded in errorno.h)
                            //to the repective integer. 
                            //header file
                            //should have done ERROR check over here!
    return cwd;
}

//*************************************************//
//this function is used to set the cursor position
//***********************************************//
void gotoXY(int x,int y){
    printf("\033[%d;%dH",x,y);
}

//*************************************************//
//this functionis used to clear the screen
//***********************************************//
void clearScreen(){
    cout << "\033c"; //clear screen
    gotoXY(0,0);
}

//***********************************************************//
//this function prints the sizes of files in human readable form
//***********************************************************//
string formatSize(double size) { 
    string units[5]= {"B", "KB", "MB", "GB", "TB"}; 
    int power = floor((size ? log(size) : 0) / log(1024)); 
    stringstream formattedSize;
    power = min(power, 4); 
    size /= pow(1024, power);
    formattedSize<<setprecision(2)<<fixed<<size;
    return formattedSize.str()+units[power]; 
} 

//***********************************************************//
//this function is used to get the size of the file/folder
//***********************************************************//
string getSize(string file){
    struct stat st;
    long size;
    size= (stat(file.c_str(), &st) == 0 )? st.st_size : -1;
    return formatSize(size);
}

//***********************************************************//
//this function returns the absolute path of the file/folder
//***********************************************************//
string getAbsolutePath(string path){
    if(path[0]!='/'){
        if(path[0]=='~'){
            path=getHomeDir()+'/'+path.substr(2);
        }else if(path[0]=='.'){
            path = curr_path + "/" + path;  
        }else{
            path = curr_path + "/" + path;  
        }
    }
    return path;
}

//***********************************************************//
//this function checks if the passed argument is a path to file or a directory
//***********************************************************//
int checkDir(string file){
    struct stat info;
    string path=getAbsolutePath(file);
    if(stat(path.c_str(), &info) == 0){
        mode_t perm = info.st_mode;
        return ((S_ISDIR(perm)))?1:0;
    }    
}

//***********************************************************//
//this function gets all the files present in the given path
//***********************************************************//
vector<string> getFiles(const string p){
    DIR *dir; 
    struct dirent **list;
    int n,i=0;
    vector<string> files;
    n = scandir(p.c_str(), &list, 0, alphasort);
    if (n < 0){
        perror("scandir");
    }else {
        while (i<n) {
            files.push_back(list[i++]->d_name);
        }
        free(list);
    }
    return files;
}

//***********************************************************//
//this function clears the command space in the command mode
//***********************************************************//
void clearCommandSpace(){
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    //window.ws_col
    printFiles();
    gotoXY(window.ws_row,0);
}

//***********************************************************//
//this function is used to enter into the non Canonical Mode
//***********************************************************//
void nonCanonicalMode(){
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

//***********************************************************//
//this function clears the sreen and prints files when entered into a new directory
//***********************************************************//
void enterFolder(){
    files.clear(); //******************STRANGE!! NOT WOKING
    list_top=0;
    cursor_position=1;
    files=getFiles(curr_path);
    clearCommandSpace();
    cout<<"** Normal Mode **";
    gotoXY(0,0);
    if(mode==0){
        normalMode_keyListner();
    }else{
        enterCommandMode();
    }
}

//***********************************************************//
//this function is used to enter into the command mode;
//***********************************************************//
void enterCommandMode(){    
    clearCommandSpace();
    cout<<setw(10)<<"** Command Mode **"<<setw(10)<<"cmd:";
    commandMode_keyListner();
}