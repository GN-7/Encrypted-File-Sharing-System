#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

class Node {
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

// Min-heap comparator
class Compare {
public:
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

class Huffman {
    Node* root;
    unordered_map<char, string> codes;

    string storedTree;
    vector<unsigned char> payload;
    int originalSize = 0;

    void generateCodes(Node* node, string code) {
        if (!node)
            return;

        if (!node->left && !node->right) {
            codes[node->ch] = code.empty() ? "0" : code;
            return;
        }

        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }

    Node* buildTree(const string& text) {
        unordered_map<char, int> freq;

        for (char ch : text)
            freq[ch]++;

        priority_queue<Node*, vector<Node*>, Compare> pq;

        for (auto& p : freq)
            pq.push(new Node(p.first, p.second));

        if (pq.empty())
            return nullptr;

        while (pq.size() > 1) {
            Node* left = pq.top();
            pq.pop();

            Node* right = pq.top();
            pq.pop();

            Node* parent = new Node(
                '\0', left->freq + right->freq
            );

            parent->left = left;
            parent->right = right;

            pq.push(parent);
        }

        return pq.top();
    }

    void storeTree(Node* node) {    //preorder sequence
        if (!node)
            return;

        if (!node->left && !node->right) {
            storedTree += '1';
            storedTree += node->ch;
            return;
        }

        storedTree += '0';

        storeTree(node->left);
        storeTree(node->right);
    }

    Node* rebuildTree(int& pos) {
        if (pos >= (int)storedTree.size())
            return nullptr;

        char marker = storedTree[pos++];

        if (marker == '1') {
            if (pos >= (int)storedTree.size())
                return nullptr;

            char ch = storedTree[pos++];
            return new Node(ch, 0);
        }

        if (marker != '0')
            return nullptr;

        Node* node = new Node('\0', 0);

        node->left = rebuildTree(pos);
        node->right = rebuildTree(pos);

        if (!node->left || !node->right)
            return nullptr;

        return node;
    }

public:
    Huffman() : root(nullptr) {}

    string compress(const string& text) {
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

        for (char ch : text) {
            for (char bit : codes[ch]) {
                buffer = (buffer << 1) | (bit == '1');

                bitCount++;

                if (bitCount == 8) {
                    payload.push_back(buffer);
                    buffer = 0;
                    bitCount = 0;
                }
            }
        }
        // for remaing that take bits less than 8
        if (bitCount > 0) {
            buffer <<= (8 - bitCount);
            payload.push_back(buffer);
        }

        return "";
    }

    string decompress() {
        string decoded;

        if (originalSize == 0)
            return decoded;

        int pos = 0;
        Node* decodeRoot = rebuildTree(pos);

        if (!decodeRoot || pos != (int)storedTree.size())
            return "";

        // if only 1 character.
        if (!decodeRoot->left && !decodeRoot->right) {
            for (int i = 0; i < originalSize; i++)
                decoded += decodeRoot->ch;

            return decoded;
        }

        Node* current = decodeRoot;

        for (int i = 0; i < (int)payload.size(); i++) {
            unsigned char byte = payload[i];

            for (int bit = 7; bit >= 0; bit--) {
                int value = (byte >> bit) & 1;

                if (value == 0)
                    current = current->left;
                else
                    current = current->right;

                if (!current)
                    return "";

                if (!current->left && !current->right) {
                    decoded += current->ch;
                    current = decodeRoot;

                    // Original size tells us when to stop,
                    // ignoring padding bits in the final byte.
                    if ((int)decoded.size() == originalSize)
                        return decoded;
                }
            }
        }

        return "";
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

    // h.compress(text);
    // h.printSizes(text);

    //Note: This calculates the size of the payload and serialized tree. If you later transmit the original size or other metadata separately, include those bytes too for an accurate total. A negative space-saved percentage means the compressed representation is larger than the original.

};

int main() {
    // string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco.";
    string text = "Lorem ipsum dolor sit amet consectetur adipiscing elit. Sem placerat in id cursus mi pretium tellus. Sed diam urna tempor pulvinar vivamus fringilla lacus. Nisl malesuada lacinia integer nunc posuere ut hendrerit. Ad litora torquent per conubia nostra inceptos himenaeos. Dis parturient montes nascetur ridiculus mus donec rhoncus. Maximus eget fermentum odio phasellus non purus est. Dictum risus blandit quis suspendisse aliquet nisi sodales. Luctus nibh finibus facilisis dapibus etiam interdum tortor. Tincidunt nam porta elementum a enim euismod quam. Velit aliquam imperdiet mollis nullam volutpat porttitor ullamcorper. Primis vulputate ornare sagittis vehicula praesent dui felis. Accumsan maecenas potenti ultricies habitant morbi senectus netus. Hac habitasse platea dictumst lorem ipsum dolor sit. Ex sapien vitae pellentesque sem placerat in id. Tempus leo eu aenean sed diam urna tempor. Bibendum egestas iaculis massa nisl malesuada lacinia integer. Class aptent taciti sociosqu ad litora torquent per. Natoque penatibus et magnis dis parturient montes nascetur. Nulla molestie mattis scelerisque maximus eget fermentum odio. Mauris pharetra vestibulum fusce dictum risus blandit quis. Ante condimentum neque at luctus nibh finibus facilisis. Sollicitudin erat viverra ac tincidunt nam porta elementum. Commodo augue arcu dignissim velit aliquam imperdiet mollis. Cras eleifend turpis fames primis vulputate ornare sagittis. Proin libero feugiat tristique accumsan maecenas potenti ultricies. Curabitur facilisi cubilia curae hac habitasse platea dictumst. Adipiscing elit quisque faucibus ex sapien vitae pellentesque. Pretium tellus duis convallis tempus leo eu aenean. Fringilla lacus nec metus bibendum egestas iaculis massa. Ut hendrerit semper vel class aptent taciti sociosqu. Inceptos himenaeos orci varius natoque penatibus et magnis. Donec rhoncus eros lobortis nulla molestie mattis scelerisque. Purus est efficitur laoreet mauris pharetra vestibulum fusce. Nisi sodales consequat magna ante condimentum neque at. Interdum tortor ligula congue sollicitudin erat viverra ac. Euismod quam justo lectus commodo augue arcu dignissim. Porttitor ullamcorper rutrum gravida cras eleifend turpis fames. Dui felis venenatis ultrices proin libero feugiat tristique. Senectus netus suscipit auctor curabitur facilisi cubilia curae. Dolor sit amet consectetur adipiscing elit quisque faucibus. In id cursus mi pretium tellus duis convallis. Urna tempor pulvinar vivamus fringilla lacus nec metus. Lacinia integer nunc posuere ut hendrerit semper vel. Torquent per conubia nostra inceptos himenaeos orci varius. Montes nascetur ridiculus mus donec rhoncus eros lobortis. Fermentum odio phasellus non purus est efficitur laoreet. Blandit quis suspendisse aliquet nisi sodales consequat magna. Finibus facilisis dapibus etiam interdum tortor ligula congue. Porta elementum a enim euismod quam justo lectus. Imperdiet mollis nullam volutpat porttitor ullamcorper rutrum gravida. Ornare sagittis vehicula praesent dui felis venenatis ultrices. Potenti ultricies habitant morbi senectus netus suscipit auctor. Platea dictumst lorem ipsum dolor sit amet consectetur. Vitae pellentesque sem placerat in id cursus mi. Eu aenean sed diam urna tempor pulvinar vivamus. Iaculis massa nisl malesuada lacinia integer nunc posuere. Taciti sociosqu ad litora torquent per conubia nostra. Et magnis dis parturient montes nascetur ridiculus mus. Mattis scelerisque maximus eget fermentum odio phasellus non. Vestibulum fusce dictum risus blandit quis suspendisse aliquet. Neque at luctus nibh finibus facilisis dapibus etiam. Viverra ac tincidunt nam porta elementum a enim. Arcu dignissim velit aliquam imperdiet mollis nullam volutpat. Turpis fames primis vulputate ornare sagittis vehicula praesent. Feugiat tristique accumsan maecenas potenti ultricies habitant morbi. Cubilia curae hac habitasse platea dictumst lorem ipsum. Quisque faucibus ex sapien vitae pellentesque sem placerat. Duis convallis tempus leo eu aenean sed diam. Nec metus bibendum egestas iaculis massa nisl malesuada. Semper vel class aptent taciti sociosqu ad litora. Orci varius natoque penatibus et magnis dis parturient. Eros lobortis nulla molestie mattis scelerisque maximus eget. Efficitur laoreet mauris pharetra vestibulum fusce dictum risus. Consequat magna ante condimentum neque at luctus nibh. Ligula congue sollicitudin erat viverra ac tincidunt nam. Justo lectus commodo augue arcu dignissim velit aliquam. Rutrum gravida cras eleifend turpis fames primis vulputate. Venenatis ultrices proin libero feugiat tristique accumsan maecenas. Suscipit auctor curabitur facilisi cubilia curae hac habitasse. Amet consectetur adipiscing elit quisque faucibus ex sapien. Cursus mi pretium tellus duis convallis tempus leo. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Nunc posuere ut hendrerit semper vel class aptent. Conubia nostra inceptos himenaeos orci varius natoque penatibus. Ridiculus mus donec rhoncus eros lobortis nulla molestie. Phasellus non purus est efficitur laoreet mauris pharetra. Suspendisse aliquet nisi sodales consequat magna ante condimentum. Dapibus etiam interdum tortor ligula congue sollicitudin erat. Aenim euismod quam justo lectus commodo augue. Nullam volutpat porttitor ullamcorper rutrum gravida cras eleifend. Vehicula praesent dui felis venenatis ultrices proin libero. Habitant morbi senectus netus suscipit auctor curabitur facilisi. Lorem ipsum dolor sit amet consectetur adipiscing elit. Sem placerat in id cursus mi pretium tellus. Sed diam urna tempor pulvinar vivamus fringilla lacus. Nisl malesuada lacinia integer nunc posuere ut hendrerit. Ad litora torquent per conubia nostra inceptos himenaeos. Dis parturient montes nascetur ridiculus mus donec rhoncus. Maximus eget fermentum odio phasellus non purus est. Dictum risus blandit quis suspendisse aliquet nisi sodales. Luctus nibh finibus facilisis dapibus etiam interdum tortor. Tincidunt nam porta elementum a enim euismod quam. Velit aliquam imperdiet mollis nullam volutpat porttitor ullamcorper. Primis vulputate ornare sagittis vehicula praesent dui felis. Accumsan maecenas potenti ultricies habitant morbi senectus netus. Hac habitasse platea dictumst lorem ipsum dolor sit. Ex sapien vitae pellentesque sem placerat in id. Tempus leo eu aenean sed diam urna tempor. Bibendum egestas iaculis massa nisl malesuada lacinia integer. Class aptent taciti sociosqu ad litora torquent per. Natoque penatibus et magnis dis parturient montes nascetur. Nulla molestie mattis scelerisque maximus eget fermentum odio. Mauris pharetra vestibulum fusce dictum risus blandit quis. Ante condimentum neque at luctus nibh finibus facilisis. Sollicitudin erat viverra ac tincidunt nam porta elementum. Commodo augue arcu dignissim velit aliquam imperdiet mollis. Cras eleifend turpis fames primis vulputate ornare sagittis. Proin libero feugiat tristique accumsan maecenas potenti ultricies. Curabitur facilisi cubilia curae hac habitasse platea dictumst. Adipiscing elit quisque faucibus ex sapien vitae pellentesque. Pretium tellus duis convallis tempus leo eu aenean. Fringilla lacus nec metus bibendum egestas iaculis massa. Ut hendrerit semper vel class aptent taciti sociosqu. Inceptos himenaeos orci varius natoque penatibus et magnis. Donec rhoncus eros lobortis nulla molestie mattis scelerisque. Purus est efficitur laoreet mauris pharetra vestibulum fusce. Nisi sodales consequat magna ante condimentum neque at. Interdum tortor ligula congue sollicitudin erat viverra ac. Euismod quam justo lectus commodo augue arcu dignissim. Porttitor ullamcorper rutrum gravida cras eleifend turpis fames. Dui felis venenatis ultrices proin libero feugiat tristique. Senectus netus suscipit auctor curabitur facilisi cubilia curae. Dolor sit amet consectetur adipiscing elit quisque faucibus. In id cursus mi pretium tellus duis convallis. Urna tempor pulvinar vivamus fringilla lacus nec metus. Lacinia integer nunc posuere ut hendrerit semper vel. Torquent per conubia nostra inceptos himenaeos orci varius. Montes nascetur ridiculus mus donec rhoncus eros lobortis. Fermentum odio phasellus non purus est efficitur laoreet. Blandit quis suspendisse aliquet nisi sodales consequat magna. Finibus facilisis dapibus etiam interdum tortor ligula congue. Porta elementum a enim euismod quam justo lectus. Imperdiet mollis nullam volutpat porttitor ullamcorper rutrum gravida. Ornare sagittis vehicula praesent dui felis venenatis ultrices. Potenti ultricies habitant morbi senectus netus suscipit auctor. Platea dictumst lorem ipsum dolor sit amet consectetur. Vitae pellentesque sem placerat in id cursus mi. Eu aenean sed diam urna tempor pulvinar vivamus. Iaculis massa nisl malesuada lacinia integer nunc posuere. Taciti sociosqu ad litora torquent per conubia nostra. Et magnis dis parturient montes nascetur ridiculus mus. Mattis scelerisque maximus eget fermentum odio phasellus non. Vestibulum fusce dictum risus blandit quis suspendisse aliquet. Neque at luctus nibh finibus facilisis dapibus etiam. Viverra ac tincidunt nam porta elementum a enim. Arcu dignissim velit aliquam imperdiet mollis nullam volutpat. Turpis fames primis vulputate ornare sagittis vehicula praesent. Feugiat tristique accumsan maecenas potenti ultricies habitant morbi. Cubilia curae hac habitasse platea dictumst lorem ipsum. Quisque faucibus ex sapien vitae pellentesque sem placerat. Duis convallis tempus leo eu aenean sed diam. Nec metus bibendum egestas iaculis massa nisl malesuada. Semper vel class aptent taciti sociosqu ad litora. Orci varius natoque penatibus et magnis dis parturient. Eros lobortis nulla molestie mattis scelerisque maximus eget. Efficitur laoreet mauris pharetra vestibulum fusce dictum risus. Consequat magna ante condimentum neque at luctus nibh. Ligula congue sollicitudin erat viverra ac tincidunt nam. Justo lectus commodo augue arcu dignissim velit aliquam. Rutrum gravida cras eleifend turpis fames primis vulputate. Venenatis ultrices proin libero feugiat tristique accumsan maecenas. Suscipit auctor curabitur facilisi cubilia curae hac habitasse. Amet consectetur adipiscing elit quisque faucibus ex sapien. Cursus mi pretium tellus duis convallis tempus leo. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Nunc posuere ut hendrerit semper vel class aptent. Conubia nostra inceptos himenaeos orci varius natoque penatibus. Ridiculus mus donec rhoncus eros lobortis nulla molestie. Phasellus non purus est efficitur laoreet mauris pharetra. Suspendisse aliquet nisi sodales consequat magna ante condimentum. Dapibus etiam interdum tortor ligula congue sollicitudin erat. Aenim euismod quam justo lectus commodo augue. Nullam volutpat porttitor ullamcorper rutrum gravida cras eleifend. Vehicula praesent dui felis venenatis ultrices proin libero. Habitant morbi senectus netus suscipit auctor curabitur facilisi. Lorem ipsum dolor sit amet consectetur adipiscing elit. Sem placerat in id cursus mi pretium tellus. Sed diam urna tempor pulvinar vivamus fringilla lacus. Nisl malesuada lacinia integer nunc posuere ut hendrerit. Ad litora torquent per conubia nostra inceptos himenaeos. Dis parturient montes nascetur ridiculus mus donec rhoncus. Maximus eget fermentum odio phasellus non purus est. Dictum risus blandit quis suspendisse aliquet nisi sodales. Luctus nibh finibus facilisis dapibus etiam interdum tortor. Tincidunt nam porta elementum a enim euismod quam. Velit aliquam imperdiet mollis nullam volutpat porttitor ullamcorper. Primis vulputate ornare sagittis vehicula praesent dui felis. Accumsan maecenas potenti ultricies habitant morbi senectus netus. Hac habitasse platea dictumst lorem ipsum dolor sit.";

    Huffman h;

    h.compress(text);
    h.printSizes(text);

    cout << "\nOriginal string: " << text << '\n';

    cout << "Packed bytes: ";

    string decoded = h.decompress();

    cout << "\nDecompressed string: " << decoded << '\n';

    cout << "Original size: "
         << text.size() << " bytes\n";




    cout << (decoded == text ? "Success!\n" : "Failed!\n");

    return 0;
}
