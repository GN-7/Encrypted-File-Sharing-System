#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <stack>
#include <unordered_map>
using namespace std;

class System
{
public:
    vector<string> fileDirectory;

    string extractFileName(string fp)
    { // Adds filename into the directory

        stack<char> filename;
        string fn;
        for (int i = fp.size() - 5; i >= 0; i--) // - 5 so that .txt doesn't come up in filename
        {
            if (fp[i] != '/' )
            {
                filename.push(fp[i]);
            }
            if (fp[i] == '/' || fp[i] == '\\')
            {
                break;
            }
        }

        while (!filename.empty())
        {
            fn.push_back(filename.top());
            filename.pop();
        }

        return fn;
    }

    string readFiletoString(string filepath)
    { // Reads the file into a string

        ifstream file(filepath);
        if (file.is_open() != true)
        {
            cout << "File reading unsuccesful";
        }

        ostringstream buffer;
        buffer << file.rdbuf();
        string fileContents = buffer.str();

        cout << "File reading successful!";
        return fileContents;
    }

    void writeStringtoFile(const string& str, const string& fp){
        ofstream file(fp, std::ios::binary);

        if (!file.is_open())
        {
            cout << "File writing unsuccesful\n";
            return;
        }

        file.write(str.data(), str.size());
        cout << "File is uploaded successfully";
}
 
    void merge(vector<string> &a, int low, int mid, int high){
        vector<string> temp;
        int left = low;
        int right = mid + 1;

        while(left < mid + 1 && right <= high){    
            if(a[left] <= a[right]){
                temp.push_back(a[left]);
                left++;
            }
            else{ temp.push_back(a[right]); right++;}
        }

        while(left <= mid){ temp.push_back(a[left]); left++;}
        while(right <= high){ temp.push_back(a[right]); right++;}

        for (int i = low; i <= high; i++)
        {
            a[i] = temp[i - low]; 
        }
    
}

    void mergeSort(vector<string> &a, int low, int high){
    if(low == high) return;
    int mid = (low + high)/2;

    mergeSort(a, low, mid);
    mergeSort(a, mid + 1, high);

    merge(a, low, mid, high);
}

    string BinarySearch(vector<string> fn, string tn)
{
    mergeSort(fileDirectory, 0, fileDirectory.size() - 1);
    int start = 0;
    int end = fn.size() - 1;

    while (start <= end)
    {
        int mid = start + (end-start) / 2;
        if (tn > fn[mid])
        {
            start = mid + 1;
        }
        else if (fn[mid] > tn)
        {
            end = mid - 1;
        }
        else
        {
            return fn[mid];
        }
    }
    return "**FILE_NOT_FOUND**";
}

    bool validateFileandRegister(string fn){
        if (BinarySearch(fileDirectory, fn) == "**ERROR_FILE_NOT_FOUND**"){ 
            fileDirectory.push_back(fn);
            cout << "File Name Registered successfully";
            return true;
        }
        else{ 
            cout << "File name taken. Enter unique filename";
            return false;
        }
    }

};

class Node
{
public:
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f)
    {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

class Compare
{
public:
    bool operator()(Node *a, Node *b)
    {
        return a->freq > b->freq;
    }
};

class Huffman
{
    Node *root;
    unordered_map<char, string> codes;

    string storedTree;
    vector<unsigned char> payload;
    int originalSize = 0;

    void generateCodes(Node *node, string code)
    {
        if (!node)
            return;

        if (!node->left && !node->right)
        {
            codes[node->ch] = code.empty() ? "0" : code;
            return;
        }

        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }

    Node *buildTree(const string &text)
    {
        unordered_map<char, int> freq;

        for (char ch : text)
            freq[ch]++;

        priority_queue<Node *, vector<Node *>, Compare> pq;

        for (auto &p : freq)
            pq.push(new Node(p.first, p.second));

        if (pq.empty())
            return nullptr;

        while (pq.size() > 1)
        {
            Node *left = pq.top();
            pq.pop();

            Node *right = pq.top();
            pq.pop();

            Node *parent = new Node(
                '\0', left->freq + right->freq);

            parent->left = left;
            parent->right = right;

            pq.push(parent);
        }

        return pq.top();
    }

    void storeTree(Node *node)
    { // preorder sequence
        if (!node)
            return;

        if (!node->left && !node->right)
        {
            storedTree += '1';
            storedTree += node->ch;
            return;
        }

        storedTree += '0';

        storeTree(node->left);
        storeTree(node->right);
    }

    Node *rebuildTree(int &pos)
    {
        if (pos >= (int)storedTree.size())
            return nullptr;

        char marker = storedTree[pos++];

        if (marker == '1')
        {
            if (pos >= (int)storedTree.size())
                return nullptr;

            char ch = storedTree[pos++];
            return new Node(ch, 0);
        }

        if (marker != '0')
            return nullptr;

        Node *node = new Node('\0', 0);

        node->left = rebuildTree(pos);
        node->right = rebuildTree(pos);

        if (!node->left || !node->right)
            return nullptr;

        return node;
    }

public:
    Huffman() : root(nullptr) {}

    string compress(const string &text)
    {
        codes.clear();
        storedTree.clear();
        payload.clear();

        root = buildTree(text);
        originalSize = text.size();

        if (!root)
            return "";

        generateCodes(root, "");
        storeTree(root);

        unsigned char buffer = 0;
        int bitCount = 0;

        for (char ch : text)
        {
            for (char bit : codes[ch])
            {
                buffer = (buffer << 1) | (bit == '1');

                bitCount++;

                if (bitCount == 8)
                {
                    payload.push_back(buffer);
                    buffer = 0;
                    bitCount = 0;
                }
            }
        }
        // for remaing that take bits less than 8
        if (bitCount > 0)
        {
            buffer <<= (8 - bitCount);
            payload.push_back(buffer);
        }

        return string(payload.begin(), payload.end());
    }

    string decompress(const string &compressedData)
    {
        // Convert the compressed string into the payload vector of unsigned chars
        payload.assign(compressedData.begin(), compressedData.end());

        string decoded;

        if (originalSize == 0)
            return decoded;

        int pos = 0;
        Node *decodeRoot = rebuildTree(pos);

        if (!decodeRoot || pos != (int)storedTree.size())
            return "";

        // If only 1 character was compressed
        if (!decodeRoot->left && !decodeRoot->right)
        {
            for (int i = 0; i < originalSize; i++)
                decoded += decodeRoot->ch;

            return decoded;
        }

        Node *current = decodeRoot;

        for (int i = 0; i < (int)payload.size(); i++)
        {
            unsigned char byte = payload[i];

            for (int bit = 7; bit >= 0; bit--)
            {
                int value = (byte >> bit) & 1;

                if (value == 0)
                    current = current->left;
                else
                    current = current->right;

                if (!current)
                    return "";

                if (!current->left && !current->right)
                {
                    decoded += current->ch;
                    current = decodeRoot;

                    // Stop as soon as we reach the original size
                    // (ignores padding bits in the final byte)
                    if ((int)decoded.size() == originalSize)
                        return decoded;
                }
            }
        }

        return decoded;
    }

    void printSizes(const string &text)
    {
        int originalBytes = text.size();
        int compressedBytes = payload.size();

        // Include the serialized tree metadata.
        int totalBytes = compressedBytes + storedTree.size();

        cout << "Original size: "
             << originalBytes << " bytes\n";

        cout << "Compressed payload size: "
             << compressedBytes << " bytes\n";

        cout << "Stored tree size: "
             << storedTree.size() << " bytes\n";

        cout << "Total compressed size (payload + tree): "
             << totalBytes << " bytes\n";

        if (originalBytes > 0)
        {
            double saved = 100.0 *
                           (originalBytes - totalBytes) / originalBytes;

            cout << "Space saved: " << saved << "%\n";
        }
    }
};



int main()
{
    System sys; Huffman h; string network_path = ""; //----> Put the network path here

        // cout << "Enter filepath: "; string inputFilePath; cin >> inputFilePath;
        // string inputFileName = sys.extractFileName(inputFilePath);
        // string outputfilepath =  network_path + inputFileName + ".txt"; 

        // string data = sys.readFiletoString(inputFilePath);
        // string data_compressed = h.compress(data);
        // sys.writeStringtoFile(data_compressed, outputfilepath);

        // string data9 = sys.readFiletoString(outputfilepath);
        // string data9_decompressed = h.decompress(data9);
        // sys.writeStringtoFile(data9_decompressed, "decompressed.txt");


    
    
    
    
}