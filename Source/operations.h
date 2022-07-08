#include "header.h"

//***********************************************************//
//this file implements various operations liek delete/copy/search/move/rename
//***********************************************************//
//NOTE:-
//      Although stat() call can fail for a variety of reasons
//      like permission denied, file not readable, etc..
//      but let'us just assume here that file don't exists!

//***********************************************************//
//For copying a file
//***********************************************************//
int copyFile(string src,string dest){                                 //assuming given processed/absolute path
    int status=-1;
    struct stat info;
    std::ifstream  srcn(src.c_str(), std::ios::binary);
    std::ofstream  dst(dest.c_str(), std::ios::binary);
    dst << srcn.rdbuf();
    if(stat(src.c_str(), &info)==0){
        chown(dest.c_str(), info.st_uid, info.st_gid);
        chmod(dest.c_str(), info.st_mode);
        status=0;                                                   // on success;
    }
    if(moveFlag==1 && status==0){
        deleteFile(src);
    }
    return status;
}

//***********************************************************//
//For copying a directory recursively
//***********************************************************//
int copyDir(string folderPath,string destination){                  //assuming given processed/absolute path
    vector<string> currList=getFiles(folderPath);
    vector<string> currFolders;
    int status=0;
    struct stat info;
    string path="";
    int f= mkdir(destination.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(f==0){
        for(auto currfile : currList){
            path=folderPath+"/"+currfile;
            if(stat(path.c_str(), &info) == 0){
                mode_t perm = info.st_mode;
                string destinationPath=destination+'/'+currfile;
                if((S_ISDIR(perm))){
                    if(currfile!="."&&currfile!=".."){
                        status+=copyDir(path,destinationPath);
                        if(status!=0){
                            cout<<"error copying";
                        }
                    }
                }else{
                    status+=copyFile(path,destinationPath);
                    if(status!=0){
                        cout<<"error copying";
                    }
                }
            }else{
                //should never happen cause we are reading the files of the curr folder!!
                cout<<"File not present";
            }
        }
    }
    if(moveFlag==1 && status>=0){
        deleteDir(folderPath);
    }
    return (status>=0)?f:-1;                                        // on success;
}

//***********************************************************//
//For deleting a file
//***********************************************************//
void deleteFile(string path){
    struct stat info;
    if(stat(path.c_str(), &info) == 0){
        mode_t perm = info.st_mode;
        if((S_ISDIR(perm))){
            cout<<"Target Is Not a file!";
        }else{
            int f= remove(path.c_str());
            if(f!=0){
               cout<<"Error Deleting";
            } 
        }
    }else{
        cout<<"File Don't exist";
    } 
}

//***********************************************************//
//For deleting a directory recursively
//***********************************************************//
void deleteDir(string folderPath){                  //assuming processed/absolute Path
    struct stat info;
    if(stat(folderPath.c_str(), &info) == 0){
        mode_t perm = info.st_mode;
        if((S_ISDIR(perm))){
            vector<string> currList=getFiles(folderPath);
            vector<string> currFolders;
            struct stat info;
            string path="";
            for(auto currfile : currList){
                path=folderPath+"/"+currfile;
                if(stat(path.c_str(), &info) == 0){
                    mode_t perm = info.st_mode;
                    if((S_ISDIR(perm))){
                        if(currfile!="."&&currfile!=".."){
                            currFolders.push_back(path);
                        }
                    }else{
                        deleteFile(path);
                    }
                }
            }
            for(auto i: currFolders){
                deleteDir(i);
            }
            int s=rmdir(folderPath.c_str()); 
            if(s==-1){
                cout<<"error Deleteing directory";
            }
        }else{
            cout<<"Target Is Not a folder!";
            return;
        }
    }else{
        
        cout<<"Folder Don't exist";
    }
            
}

//***********************************************************//
//For searching a directory recursively
//***********************************************************//
bool search(string file,string folderPath){
    vector<string> currList=getFiles(folderPath);
    vector<string> currFolders;
    bool present =false;
    struct stat info;
    string path="";
    for(auto currfile : currList){
        path=folderPath+"/"+currfile;
        if(stat(path.c_str(), &info) == 0){
            mode_t perm = info.st_mode;
            if((S_ISDIR(perm))){
                if(file==currfile){
                    present=true;
                }
                if(currfile!="."&&currfile!=".."){
                    currFolders.push_back(path);
                }
            }else{
                if(file==currfile){
                    present=true;
                }
            }
        }
        if(present){
            return true;
        }
    }
    for(auto i: currFolders){
        present=search(file,i);
        if(present){
            return true;
        }
    }
    return present;
}
