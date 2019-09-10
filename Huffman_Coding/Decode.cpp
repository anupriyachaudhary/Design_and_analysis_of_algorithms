#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
using namespace std;

class HuffmanTree
{
private:
    struct HuffmanTreeNode
    {
        unsigned char ch;
        HuffmanTreeNode * left;
        HuffmanTreeNode * right;
    };
    HuffmanTreeNode *root;
    HuffmanTreeNode *partiallyDecodedNode;
    static HuffmanTreeNode* getnode(char ch, HuffmanTreeNode *left = nullptr, HuffmanTreeNode *right = nullptr);
    
public:
    void buildHuffmanTree(string array[], int size);
    
    // return true if completely decoded or false if partially decoded
    bool decode(unsigned char ch, int startPos, int endPos, int *decodedPos, char *decodedChar);
};

bool HuffmanTree::decode(unsigned char ch, int startPos, int endPos, int *decodedPos, char *decodedChar)
{
    HuffmanTreeNode *currentNode = partiallyDecodedNode;
    for (int i = startPos; i < endPos; i++)
    {
        // get the ith bit value
        int bit = ch & (1 << (7 - i));
        if (bit == 0)
        {
            currentNode = currentNode->left;
        }
        else
        {
            currentNode = currentNode->right;
        }
        
        // check if leaf node
        if (currentNode->left == nullptr && currentNode->right == nullptr)
        {
            *decodedPos = i;
            *decodedChar = currentNode->ch;
            partiallyDecodedNode = root;
            return true;
        }
    }
    partiallyDecodedNode = currentNode;
    return false;
}

void HuffmanTree::buildHuffmanTree(string array[], int size)
{
    // create root node
    root = getnode('*');
    
    for (int i = 0; i < size; i++)
    {
        if (array[i].empty())
        {
            continue;
        }
        
        HuffmanTreeNode *currentNode = root;
        string code = array[i];
        for (unsigned j = 0; j < code.length(); j++)
        {
            if (code[j] == '0')
            {
                // if node not created, create node
                if (currentNode->left == nullptr)
                {
                    currentNode->left = getnode('*');
                }
                currentNode = currentNode->left;
            }
            else
            {
                // if node not created, create node
                if (currentNode->right == nullptr)
                {
                    currentNode->right = getnode('*');
                }
                currentNode = currentNode->right;
            }
        }
        
        // initialize the node with character value
        currentNode->ch = i;
    }
    partiallyDecodedNode = root;
}

HuffmanTree::HuffmanTreeNode * HuffmanTree::getnode(char ch, HuffmanTreeNode *left, HuffmanTreeNode *right)
{
    HuffmanTreeNode * ret = new HuffmanTreeNode;
    ret->ch = ch;
    ret->left = left;
    ret->right = right;
    
    return ret;
}

int main(int argc, char *argv[])
{
    ifstream code(argv[1]);
    
    string array[256];
    for (int i = 0; i <= 255; i++)
        array[i] = "";
    
    
    for (string line; getline(code, line);)
    {
        string str1, str2 = "";
        unsigned i;
        for (i = 0; line[i] != '\t'; i++)
            str1 = str1 + line[i];
        
        for (i = i + 1; line[i] != '\0'; i++)
            str2 = str2 + line[i];
        
        array[stoi(str1)] = str2;
    }
    
    code.close();
    
    // build huffman tree
    HuffmanTree* tree = new HuffmanTree();
    tree->buildHuffmanTree(array, 256);
    
    unsigned char firstByte = '\0';
    unsigned char secondByte = '\0';
    unsigned char thirdByte = '\0';
    
    char ch;
    int pos = 0;
    
    
    // read 1st byte from stream
    cin.read((char*)&firstByte, 1);
    
    // read 2nd byte
    cin.read((char*)&secondByte, 1);
    
    while (true)
    {
        // read 1 byte
        cin.read((char*)&thirdByte, 1);
        
        // secondByte can be last byte and eof has reached
        if (cin.eof())                                                             //??????????????
        {
            // get the bytes to decode
            int bytesCount = (int)secondByte;                                       //?????????????
            
            if (bytesCount > 0)
            {
                while (true)
                {
                    bool isDecoded = tree->decode(firstByte, pos, bytesCount, &pos, &ch);
                    if (isDecoded)
                    {
                        cout.write((char*)&ch, 1);
                        ch = '\0';
                        if (pos >= (bytesCount - 1))
                        {
                            pos = 0;
                            break;
                        }
                        pos = pos + 1;
                    }
                    else
                    {
                        pos = 0;
                        ch = '\0';
                        break;
                    }
                }
                
            }
            break;
        }
        
        // decode first byte
        while (true)
        {
            bool isDecoded = tree->decode(firstByte, pos, 8, &pos, &ch);
            if (isDecoded)
            {
                cout.write((char*)&ch, 1);
                ch = '\0';
                if (pos >= 7)
                {
                    pos = 0;
                    break;
                }
                pos = pos + 1;
            }
            else
            {
                pos = 0;
                ch = '\0';
                break;
            }
        }
        
        // assign second byte to first byte and assign third byte to second byte
        firstByte = secondByte;
        secondByte = thirdByte;
    }
    
    return 0;
}
