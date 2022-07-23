#include "header.h"


//***********************************************************//
//this file is used to print various details of the file required to be displayed 
//***********************************************************//

string getSecutityMeta(string file){
    struct stat info;
    struct passwd *pw; 
    struct group  *gr;
    struct tm *mtime;

    char buf[200];
    string p="drwxrwxrwx";
    string owner,group;
    if(stat(file.c_str(), &info) == 0){
        stringstream securityMeta;
        mode_t perm = info.st_mode;
        
        if((S_ISDIR(perm))){
            p[0] = 'd';
           // checkDir.push_back(1);
        }else{
            p[0] = '-';
            //checkDir.push_back(0);
        }
        p[1] = (perm & S_IRUSR) ? 'r' : '-';
        p[2] = (perm & S_IWUSR) ? 'w' : '-';
        p[3] = (perm & S_IXUSR) ? 'x' : '-';
        p[4] = (perm & S_IRGRP) ? 'r' : '-';
        p[5] = (perm & S_IWGRP) ? 'w' : '-';
        p[6] = (perm & S_IXGRP) ? 'x' : '-';
        p[7] = (perm & S_IROTH) ? 'r' : '-';
        p[8] = (perm & S_IWOTH) ? 'w' : '-';
        p[9] = (perm & S_IXOTH) ? 'x' : '-';
        pw=getpwuid(info.st_uid);
        gr=getgrgid(info.st_gid);

        if(pw!=0){
            owner=pw->pw_name;
        };
        if(gr!=0){
            group=gr->gr_name;
        }
        time_t t = info.st_mtime;
        mtime=localtime(&t);
        strftime(buf, sizeof(buf), "%d.%m.%Y\t%H:%M:%S", mtime);
        securityMeta<<setw(10)<<p<<setw(10)<<owner<<setw(10)<<group<<setw(23)<<buf<<endl;
        return securityMeta.str();     
    }else{
        return strerror(errno);
    }   
}
string trim(string name){
    char * trimmedName=(char *)malloc(sizeof(char) * 23 + 1);
    int i=0;
    while(name[i]!='\0'){
        trimmedName[i]=name[i];
        if(i==20){
            trimmedName[i++]='.';
            trimmedName[i++]='.';
            trimmedName[i++]='.';
            break;
        }
        i++;
    }
    trimmedName[i]='\0';
    return trimmedName;
}
void printFiles(){
    //checkDir.clear();
    clearScreen();
    int limit=LIMIT+list_top;
    files=getFiles(curr_path);
    for(int i=list_top;i<limit && i<files.size();i++){
        string filePath=getAbsolutePath(files[i]);//curr_path+'/'+files[i];
        cout<<setw(25)<<trim(files[i])<<setw(10)<<getSize(filePath)<<setw(57)<<getSecutityMeta(filePath);
    }
    gotoXY(cursor_position,0);
}