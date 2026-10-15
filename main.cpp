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
    public:
    int BinarySearch(vector<string>fn,string tn);
    int SortFile(vector<string>& fn);
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



int System::BinarySearch(vector<string>fn,string tn)
{
 int start=0;
 int end=fn.size()-1;




while(start<=end)
{
int mid=(start+end)/2;
if(tn>fn[mid])
 {
    start=mid+1;
 }
 else if(fn[mid]>tn)
 {
    end=mid-1;
 }
 else
 {
    return mid;
 }
  }
  return -1;
}

int System::SortFile(vector<string>& fn)
{
    for(int i=0;i<fn.size()-1;i++)
    { bool isSwap = false;
        for(int j=0;j<fn.size()-i-1;j++)
        {
            if(fn[j]>fn[j+1])
            {
                swap(fn[j],fn[j+1]);
                isSwap=true;
            }
            
        }
        if(isSwap==false)
        {
            return 0;
        }
    }
}

class FileManager : public System {
public:

    void sortFiles() {
        SortFile(fileDirectory);
    }
    int searchFile(string name) {
        return BinarySearch(fileDirectory, name);
    }
};


int main(){

FileManager manage;
    
manage.sortFiles();


for(int i=0;i<arry.size();i++)
{
if(searchFile(arry[i])==-1)
{
    cout<<"File"<<arry[i]<<"not found";
}
else
{
    cout<<"File"<<arry[i]<<"found";
}
}

    
    return 0;
}