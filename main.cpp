#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <stack>
using namespace std;

class System
{
protected:
    vector<string> fileDirectory;
    vector<string> downloadQueue;

    void registerFileName(string fp){
        stack<char> filename;
        string fn;
        for (int i = fp.size(); i > 0; i--)
        {
            if (fp[i] != '/')
            {
                filename.push(fp[i]);
            }
            if (fp[i] == '/')
            {
                break;
            }
            
        }

        while (!filename.empty())
        {
            fn.push_back(filename.top());
            filename.pop();
        }
        fileDirectory.push_back(fn);
        
        
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