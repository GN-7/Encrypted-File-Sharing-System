#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
using namespace std;

class System
{
protected:
    vector<string> fileDirectory;
    vector<string> downloadQueue;

    void readFile(){}
    void validateFileName(){}
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
    while (true)
    {
        //MENU
        
    }
    
    return 0;
}