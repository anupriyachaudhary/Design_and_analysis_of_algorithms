#include <iostream>
#include <string>
using namespace std;

class HuffmanTree
{
private:
    struct HuffmanTreeNode
    {
        unsigned char ch;
        unsigned freq;
        HuffmanTreeNode * left;
        HuffmanTreeNode * right;
    };
    static HuffmanTreeNode* getnode(unsigned char ch, unsigned freq, HuffmanTreeNode *left = nullptr, HuffmanTreeNode *right = nullptr);
    
    class PriorityQueue
    {
    public:
        PriorityQueue();
        HuffmanTreeNode * top();
        void push(HuffmanTreeNode * node);
        void pop();
        unsigned size();
    private:
        HuffmanTreeNode * * _heap;
        int _lastElemPos;
        int _size;
        HuffmanTreeNode * * allocate(unsigned els);
        void grow(unsigned newEls);
        bool _is_leaf(unsigned pos);
        unsigned _left_child_pos(unsigned pos);
        unsigned _right_child_pos(unsigned pos);
        unsigned _parent_pos(unsigned pos);
        void _min_heapify();
    };
    void printHuffmanCodeRec(HuffmanTreeNode * node, string code);
public:
    HuffmanTreeNode * buildHuffmanTree(int array[]);
    void printHuffmanCode(HuffmanTreeNode * node, string code);
};

int main()
{
    int array[256];
    for(int i = 0; i <= 255; i++)
        array[i] = 0;
    
    for(string line; getline(cin, line);)
    {
        string str1, str2 = "";
        unsigned i;
        for (i = 0; line[i] != '\t'; i++)
            str1 = str1 + line[i];
        
        for (i = i+1; line[i] != '\0'; i++)
            str2 = str2 + line[i];
        
        array[stoi(str1)] = stoi(str2);
    }
    
    HuffmanTree tree;
    tree.printHuffmanCode(tree.buildHuffmanTree(array), "");
    
    return 0;
}

HuffmanTree::HuffmanTreeNode * HuffmanTree::buildHuffmanTree(int array[])
{
    PriorityQueue queue;
    
    for(int i = 0; i <= 255; i++)
    {
        if(array[i] != 0)
        {
            unsigned char ch = i;
            queue.push(getnode(ch, array[i]));
        }
    }
    
    while(queue.size() > 1)
    {
        HuffmanTreeNode * parent;
        HuffmanTreeNode * right;
        HuffmanTreeNode * left;
        
        left = queue.top();
        queue.pop();
        
        right = queue.top();
        queue.pop();
        
        parent = getnode('*', (left->freq + right->freq), left, right);  // '*' identifies internal nodes
        queue.push(parent);
    }
    return queue.top();
}

void HuffmanTree::printHuffmanCodeRec(HuffmanTreeNode * node, string code)
{
    if (!node)
        return;
    
    printHuffmanCodeRec(node->left, code + '0');
    if (node->left == nullptr && node->right == nullptr)
        cout << (int)node->ch << '\t' << code << endl;
    printHuffmanCodeRec(node->right, code + '1');
}

void HuffmanTree::printHuffmanCode(HuffmanTreeNode * node, string code)
{
    if (node->left == nullptr && node->right == nullptr)
    {
        cout << (int)node->ch << '\t' << '0' << endl;
        return;
    }
    printHuffmanCodeRec(node, code);
}

HuffmanTree::HuffmanTreeNode * HuffmanTree::getnode(unsigned char ch, unsigned freq, HuffmanTreeNode *left, HuffmanTreeNode *right)
{
    HuffmanTreeNode * ret = new HuffmanTreeNode;
    ret->ch = ch;
    ret->freq = freq;
    ret->left = left;
    ret->right = right;
    
    return ret;
}

HuffmanTree::PriorityQueue::PriorityQueue()
{
    _size = 0;
    _lastElemPos = -1;
    _heap = nullptr;
}

HuffmanTree::HuffmanTreeNode * HuffmanTree::PriorityQueue::top()
{
    if(size() == 0)
        return nullptr;
    else
        return _heap[0];
}

void HuffmanTree::PriorityQueue::push(HuffmanTreeNode * node)
{
    if(_size == 0)
        _heap = allocate(10);
    else if(_lastElemPos >= (_size - 1))
        grow(10);
    
    unsigned pos = _lastElemPos + 1;
    _heap[pos] = node;
    
    while(pos > 0)
    {
        unsigned parent = _parent_pos(pos);
        if(_heap[parent]->freq <= node->freq)
        {
            _heap[pos] = node;
            break;
        }
        _heap[pos] = _heap[parent];
        pos = parent;
    }
    if(pos == 0)
    {
        _heap[pos] = node;
    }
    _lastElemPos++;
}

void HuffmanTree::PriorityQueue::pop()
{
    _heap[0] = _heap[_lastElemPos];
    _lastElemPos = _lastElemPos - 1;
    _min_heapify();
}

unsigned HuffmanTree::PriorityQueue::size() { return _lastElemPos + 1; }

HuffmanTree::HuffmanTreeNode * * HuffmanTree::PriorityQueue::allocate(unsigned els)
{
    _size = els;
    return new HuffmanTreeNode * [els];
}

void HuffmanTree::PriorityQueue::grow(unsigned newEls)
{
    HuffmanTreeNode * * temp = allocate(_size + newEls);
    for (int i = 0; i <= _lastElemPos; i++)
        temp[i] = _heap[i];
    for (int j = _lastElemPos + 1; j <= _lastElemPos + newEls; j++)
        temp[j] = nullptr;
    
    delete [] _heap;
    
    _heap = temp;
}

bool HuffmanTree::PriorityQueue::_is_leaf(unsigned pos) { return (_left_child_pos(pos) > (size() - 1)); }

unsigned HuffmanTree::PriorityQueue::_left_child_pos(unsigned pos) {return (2 * pos + 1);}

unsigned HuffmanTree::PriorityQueue::_right_child_pos(unsigned pos) {return (2 * pos + 2);}

unsigned HuffmanTree::PriorityQueue::_parent_pos(unsigned pos) {return unsigned((pos - 1) / 2);}

void HuffmanTree::PriorityQueue::_min_heapify()
{
    HuffmanTreeNode * node = _heap[0];
    
    int currentPos = 0;
    
    while(!_is_leaf(currentPos))
    {
        int nodeToSwap = 0;
        int left = _left_child_pos(currentPos);
        int right = _right_child_pos(currentPos);
        
        if(right <= _lastElemPos && _heap[right]->freq <= _heap[left]->freq)
            nodeToSwap = right;
        else
            nodeToSwap = left;
        
        if(node->freq <= _heap[nodeToSwap]->freq)
        {
            _heap[currentPos] = node;
            break;
        }
        
        _heap[currentPos] = _heap[nodeToSwap];
        currentPos = nodeToSwap;
    }
    
    if(_is_leaf(currentPos))
    {
        _heap[currentPos] = node;
    }
}
