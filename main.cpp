#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <stack>
using namespace std;

class Compare {
public:
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

class Node {    //binary tree
public:
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

class System
{
protected:
    vector<string> fileDirectory;
    vector<string> uploadQueue;
    vector<string> downloadQueue;

    void registerFileName(string fp){//Adds filename into the directory

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

    string readFile(string filepath){//Reads the file into a string

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

    void validateFile(){//Binary Search for Filename if it exists in the Directory
    }



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
