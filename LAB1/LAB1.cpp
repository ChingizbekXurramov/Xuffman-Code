#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <bitset>

using namespace std;

map<char, string> codes;

map<char, int> freq;

struct MinHeapNode
{
    char data;
    int freq;
    MinHeapNode* left, * right;

    MinHeapNode(char data, int freq)
    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

struct compare
{
    bool operator()(MinHeapNode* l, MinHeapNode* r)
    {
        return (l->freq > r->freq);
    }
};

void printCodes(struct MinHeapNode* root, string str)
{
    if (!root)
        return;
    if (root->data != ',')
        cout << root->data << ": " << str << "\n";
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

void storeCodes(struct MinHeapNode* root, string str)
{
    if (root == NULL)
        return;
    if (root->data != ',')
        codes[root->data] = str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}

void HuffmanCodes(int size)
{
    struct MinHeapNode* left, * right, * top;
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

    for (map<char, int>::iterator v = freq.begin(); v != freq.end(); v++)
        minHeap.push(new MinHeapNode(v->first, v->second));
    while (minHeap.size() != 1)
    {
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();
        top = new MinHeapNode(',', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    storeCodes(minHeap.top(), "");
}

void calcFreq(string str, int n)
{
    for (int i = 0; i < str.size(); i++)
        freq[str[i]]++;
}

string encode(string str)
{
    string encodedString = "";
    for (int i = 0; i < str.length(); i++)
    {
        encodedString += codes[str[i]];
    }
    return encodedString;
}

string decode(struct MinHeapNode* root, string s)
{
    string decodedString = "";
    struct MinHeapNode* current = root;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '0')
            current = current->left;
        else
            current = current->right;


        if (current->left == nullptr && current->right == nullptr)
        {
            decodedString += current->data;
            current = root;
        }
    }
    return decodedString;
}

int main()
{
    string str, filename;
    int mode;

    cout << "Enter filename for input text: ";
    getline(cin, filename);

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "Error opening file. Exiting...";
        return 1;
    }

    cout << "Enter filename for output: ";
    getline(cin, filename);

    cout << "Mode (0 - Encode, 1 - Decode): ";
    cin >> mode;

    ofstream outputFile(filename, ios::binary);
    if (!outputFile.is_open()) {
        cout << "Error opening file. Exiting...";
        return 1;
    }

    // Read input text from file
    getline(inputFile, str);

    calcFreq(str, str.length());
    HuffmanCodes(str.length());

    if (mode == 0)
    {
        // Encode
        string encodedString = encode(str);
        outputFile << encodedString;
    }
    else if (mode == 1)
    {
        // Decode
        string encodedString;
        getline(inputFile, encodedString);
        string decodedString = decode(new MinHeapNode(',', 0), encodedString);
        outputFile << decodedString;
    }

    inputFile.close();
    outputFile.close();

    return 0;
}