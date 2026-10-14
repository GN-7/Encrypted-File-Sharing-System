#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
using namespace std;

class System
{
protected:
    vector<string> fileDirectory;
    vector<string> downloadQueue;

    void registerFileName(string fp){

    }

    string readFile(string filepath){
        
        registerFileName(filepath);

        ifstream file(filepath);        
        if (file.is_open() != true)
        {
            cout << "File reading unsuccesful";
        }

        ostringstream buffer;
        buffer << file.rdbuf();
        string fileContents = buffer.str();

        return fileContents;
    }
    void validateFileName(){
        //Binary Search
    }
    //void validateDownload(){}
};

class Encryptor : protected System {
    protected:
        void encrypt(){}
        void decrypt(){}
};

class Compressor : protected System {
    protected:
        void compress(){}
        void decompress(){}
};

int main(){
    
    
    return 0;
}