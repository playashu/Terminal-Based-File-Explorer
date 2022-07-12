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
//For creating a file
//***********************************************************//
void createFile(string path){
    struct stat info;
    if(stat(path.c_str(), &info) != 0){ 
        int f=open(path.c_str(), O_WRONLY | O_CREAT, 0644);    
        if(f==-1){
            cout<<"Error Creating File";
        }else{
            cout<<"File Created";
        }
        close(f);
        // ofstream outfile;
        // string createFile = "";
        // createFile = getAbsolutePath(curr_command[2] + "/" + curr_command[1]);    
        // //cout<<createFile;
        // outfile.open(createFile.c_str());       
        // outfile.close();   
    }else{
        cout<<"File Already Exists!";
    }  
}

//***********************************************************//
//for renaming files 
//***********************************************************//
void rename(string s1,string s2){
    struct stat info;
    string destination=getAbsolutePath(s2);
    if(stat(destination.c_str(), &info) == 0){ 
        cout<<"File with the same name alreay exists!";
    }else{  
        string file1=getAbsolutePath(s1);
        string file2=getAbsolutePath(s2);
        int a=rename(file1.c_str(),file2.c_str());
        if(a==0)
            cout<<"File Renamed";
        else
            cout<<"Error Renaming";
    }
}

//***********************************************************//
//for jumping to directories expects absolute paths
//***********************************************************//
void gotoDirectory(string& curr_path){                      //expects absoulute paths
    leftStack.push(curr_path);
    while(!rightStack.empty()){
        rightStack.pop();            
    }
    enterFolder();
}
//***********************************************************//
//For copying or moving multiple directories or files
//***********************************************************//
void copy_move_multi(vector<string> &curr_command){
    struct stat info;
    int status=-1;
    string destination=getAbsolutePath(curr_command[curr_command.size()-1]);
    if(stat(destination.c_str(), &info) == 0){
        mode_t perm = info.st_mode;
        if(!(S_ISDIR(perm))){
            cout<<"Destination Doesn't Exists";
            return;
        }
    }
    for(int i=1;i<curr_command.size()-1;i++){
        bool present=false;
        string path=getAbsolutePath(curr_command[i]);
        if(stat(path.c_str(), &info) == 0){
            mode_t perm = info.st_mode;
            int last = path.find_last_of("/");
            string sourcePath=path.substr(0, last);
            string sourceName=path.substr(last+1);
            string newPath=destination+"/"+sourceName;
            if(stat(newPath.c_str(), &info) != 0){
                if((S_ISDIR(perm))){
                    status=copyDir(path,newPath);
                }else{
                    status=copyFile(path,newPath);
                }
            }else{
                cout<<sourceName<<": Already Present: Skipped! ||";
                continue;   
            }
        }else{
            cout<<"Source Doesn't Exists";
            break;
        }    
    }  
    if(status==0){
        cout<<"Done!";
    } 
}
//***********************************************************//
//For creating an empty directory
//***********************************************************//
void createDir(string path){
    struct stat info;
    if(stat(path.c_str(), &info) != 0){ 
        int f = mkdir(path.c_str(),0775);
        if(f!=0){
            cout<<"Error Creating Directory";
        }else{
            cout<<"Directory Created";
        }
    }else{
        cout<<"Directory Already Exists";
    }
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
