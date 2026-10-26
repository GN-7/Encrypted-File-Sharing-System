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

    void getFileDirectory(string fp){
        ifstream file(fp);
        if (file.is_open() != true)
        {
            cout << " > File reading unsuccesful!" << endl;
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            fileDirectory.push_back(line);
        }
        file.close();
    }

    void pushFileDirectory(vector<string> fd, string fp){
        ofstream file(fp);

        if (!file.is_open())
        {
            cout << " > File writing unsuccesful" << endl;
            return;
        }
        
        for ( auto i : fd){
            file << i + "\n";
        }
    }

    string extractFileName(string fp)
    { 
        stack<char> filename;
        string fn;
        // - 5 so that .txt doesn't come up in filename
        for (int i = fp.size() - 5; i >= 0; i--) 
        {
            if (fp[i] == '/' || fp[i] == '\\' || '/')
            {
                break;
            }
            filename.push(fp[i]);
        }

        while (!filename.empty())
        {
            fn.push_back(filename.top());
            filename.pop();
        }

        return fn;
    }

    string readFiletoString(string filepath)
    { 
        ifstream file(filepath, std::ios::binary);
        if (file.is_open() != true)
        {
            cout << " > File reading unsuccesful!" << endl;
            return "";
        }

        ostringstream buffer;
        buffer << file.rdbuf();
        string fileContents = buffer.str();

        cout << " > File reading successful!" << endl;
        return fileContents;
    }

    void writeStringtoFile(const string& str, const string& fp){
        ofstream file(fp, std::ios::binary);
        if (!file.is_open())
        {
            cout << " > File writing unsuccesful to: " << fp << endl;
            return;
        }

        file.write(str.data(), str.size());
        cout << " > File writing successful" << endl;
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

    bool BinarySearch(vector<string> &fn, string tn)
    {
        if (fn.empty()) return false;
        
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
                return true;
            }
        }
        return false;
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
    string encryptionKey;
    string storedTree;
    vector<unsigned char> payload;
    
    string xor_ed(const string &data, const string &key)
    {
        if (key.empty()) return data;
        string encryptedstring = data;
        for (int i = 0; i < data.size(); ++i)
        {
            encryptedstring[i] = data[i] ^ key[i % key.size()];
        }
        return encryptedstring;
    }
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
    { 
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
    Huffman(string key) : root(nullptr), encryptionKey(key) {}

    void setKey(string key)
    {
        encryptionKey = key;
    }
    
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

        if (bitCount > 0)
        {
            buffer <<= (8 - bitCount);
            payload.push_back(buffer);
        }

        string compressedStr(payload.begin(), payload.end());
        string encryptedData = xor_ed(compressedStr, encryptionKey);

        // Package metadata with the payload so it can be decompressed later
        return to_string(originalSize) + "~" + storedTree + "~" + encryptedData;
    }

    string decompress(const string &packagedData)
    {
        // Unpackage the metadata
        size_t firstDelim = packagedData.find('~');
        if (firstDelim == string::npos) return "";

        size_t secondDelim = packagedData.find('~', firstDelim + 1);
        if (secondDelim == string::npos) return "";

        string sizeStr = packagedData.substr(0, firstDelim);
        originalSize = stoi(sizeStr);
        storedTree = packagedData.substr(firstDelim + 1, secondDelim - firstDelim - 1);
        string encryptedData = packagedData.substr(secondDelim + 1);

        // Decrypt 
        string decryptedData = xor_ed(encryptedData, encryptionKey);
        payload.assign(decryptedData.begin(), decryptedData.end());

        string decoded;

        if (originalSize == 0)
            return decoded;

        int pos = 0;
        Node *decodeRoot = rebuildTree(pos);

        if (!decodeRoot || pos != (int)storedTree.size())
            return "";

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

                    if ((int)decoded.size() == originalSize)
                        return decoded;
                }
            }
        }

        return decoded;
    }

    void printSizes(const string& text) {
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

        if (originalBytes > 0) {
            double saved = 100.0 *
                (originalBytes - totalBytes) / originalBytes;

            cout << "Space saved: " << saved << "%\n";
        }
    }
};

int main()
{
    System sys; 
    Huffman h ("4"); 
    
    // Ensure this folder exists on your drive
    string network_path = "C:\\Nipun_Code\\"; 
    
    sys.getFileDirectory("directory.txt");
    
    while(true){
        sys.fileDirectory.clear();
        sys.getFileDirectory(network_path + "directory.txt");
        int choice;
        cout << "-------------------------------------------" << endl;
        cout << "To upload files to the shared server, enter 1\n";
        cout << "To download files from the shared server, enter 2\n";
        cout << "To exit the program, enter 3\n";
        cout << "Enter your choice: "; 
        cin >> choice;

        if(choice == 1){
            string filepath;
            cout << "-------------------------------------------" << endl;
            cout << "Enter absolute filepath: " << endl; 
            
            // Rolled back to original cin method
            cin >> filepath;
            
            string inputFileName = sys.extractFileName(filepath);
            
            if(sys.BinarySearch(sys.fileDirectory, inputFileName) == false){
                
                string ofp = network_path + inputFileName + ".txt";
                string data = sys.readFiletoString(filepath);
                
                if (!data.empty()) {
                    sys.fileDirectory.push_back(inputFileName);
                    sys.mergeSort(sys.fileDirectory, 0, sys.fileDirectory.size() - 1);
                    string data_compressed = h.compress(data);
                    h.printSizes(data);
                    
                    sys.writeStringtoFile(data_compressed, ofp);  
                    sys.pushFileDirectory(sys.fileDirectory, network_path + "directory.txt");            
                }
            }
            else{ 
                cout << "File already exists" << endl;
            }
        }   
        else if(choice == 2){
            string filename;
            cout << "-------------------------------------------" << endl;
            cout << "Enter filename: " << endl; 
            
            // Rolled back to original cin method
            cin >> filename;
            
            if(!sys.BinarySearch(sys.fileDirectory, filename)){
                cout << "File does not exist in server! Enter proper filename" << endl;
            }
            else{
                string shared_file_path = network_path + filename + ".txt";
                string data_ = sys.readFiletoString(shared_file_path);
                
                if (!data_.empty()) {
                    string data_decompressed = h.decompress(data_);
                    cout << " > File is decompressed and decrypted" << endl;
                    
                    // Rolled back to original hardcoded path
                    sys.writeStringtoFile(data_decompressed, "C:\\Users\\Rescue\\Downloads\\" + filename + ".txt");
                }
            }
        }
        else{
            sys.pushFileDirectory(sys.fileDirectory, network_path + "directory.txt");
            break;
        }
    };    
    
    return 0;
}