#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <unordered_map>
using namespace std;

class Node{
public:
    char data;
    int freq;
    Node* left;
    Node* right;

    Node(char d,int f){
        left = right = NULL;
        data = d;
        freq = f;
    }
};

class compare{
public:
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

// void printcodes(Node* root, string str){     // printing assigned huffman codes
//     if(root == NULL){
//         return;
//     }
//
//     if (root->data != '\0') {
//         cout << root->data << ": " << str <<"\n";
//     }
//
//     printcodes(root->left, str + "0");
//     printcodes(root->right, str + "1");
// }


void storeCodes(Node* root, string str, unordered_map<char, string>& huffmanCodes) {
    if (root == NULL) {
        return;
    }

    if (root->data != '\0') {
        huffmanCodes[root->data] = str;
        cout << root->data << ": " << str <<"\n";
        // u can also directly print here and remove the printnode function
    }

    storeCodes(root->left, str + "0", huffmanCodes);
    storeCodes(root->right, str + "1", huffmanCodes);
}

string decompress(Node* root, string& encoded) {
    string decoded = "";

    if(root == NULL) {
        return decoded;
    }

    //is tree contains single character
    if(!root->left && !root->right) {
        return string (encoded.length(), root->data);
    }

    Node* curr = root;
    for(char c : encoded) {
        if(c == '0'){
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }

        if(!curr->left && !curr->right) {
            decoded += curr->data;
            // decoded += "`";
            curr = root;
        }

        if(curr == NULL) break;
    }



    return decoded;
}

unordered_map<char, string> huffmancodes(vector<char> data, int freq[], int size, Node* &root){
    Node* left, *right, *top;
    priority_queue<Node*, vector<Node*>, compare> minheap;

    for(int i=0; i< size; i++){
        minheap.push(new Node(data[i], freq[(unsigned char)data[i]]));
    }

    while(minheap.size() > 1){
        left = minheap.top();
        minheap.pop();
        right = minheap.top();
        minheap.pop();

        top = new Node('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minheap.push(top);
    }

    root = minheap.top();
    // printcodes(minheap.top(), "");

    unordered_map<char, string> huffmanCodes;
    storeCodes(minheap.top(), "", huffmanCodes);
    return huffmanCodes;
}

int main(){
     string s = "Huffman coding is a popular lossless data compression algorithm. It assigns shorter binary codes to frequently occurring characters and longer binary codes to rare characters. This makes the encoded message smaller while preserving all the original data. For example, the letter 'e' appears more often than the letter 'z' in many English texts. A good compression algorithm should encode repeated letters efficiently, handle spaces and punctuation correctly, and decode the compressed data ////back into exactly the same text. Huffman coding, Huffman coding, Huffman coding!";
    // string s = "this is a test string to test huffman coding. this is a test string to test huffman coding. this is a test string to test huffman coding. this is a test string to test huffman coding.";

     // string s = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    set<char> set;
    vector<char> arr;
    int freq[256] ={0};

    for (char c : s) {
        set.insert(c);
        freq[c]++;
    }

    for(char c : set) {
        arr.push_back(c);
    }


    // vector<char> arr = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'};
    // int freq[] = { 3, 11, 2, 1, 6, 7, 8, 5, 9, 4, 10 };
    int size = arr.size();


    Node* root = NULL;

    unordered_map<char, string> huffmanCodes =  huffmancodes(arr, freq, size, root);

    for(auto h : huffmanCodes) {
        cout << h.first << ": " << h.second << "\n";
    }

    string huffmanstring = "";
    for (char c : s) {
        huffmanstring += huffmanCodes[c];
    }

    string decoded = decompress(root, huffmanstring);

    // vector<unsigned char> vec = packBits(huffmanstring);

    // cout <<endl<< sizeof(huffmanstring);
    // cout <<endl << s.length() <<endl << huffmanstring.length();
    //
    cout << "\nOriginal String length (bytes): " << s.length() << endl;
    cout << "Encoded Binary String length (bits): " << huffmanstring.length() << endl;
    cout << "Decoded Binary String  " << decoded << endl;
    cout << "\nEncoded Text:\n" << huffmanstring << endl;

}
