#include <bits/stdc++.h>
using namespace std;
struct Tree
{
    int frequency;
    char character;
    Tree *left = NULL;
    Tree *right = NULL;
};

class TreeComparator
{
  public:
    bool operator()(Tree *a, Tree *b)
    {
        return a->frequency > b->frequency;
    }
};

void printVector(vector<int> A)
{
    for (int i = 0; i < A.size(); i++)
    {
        cout << A[i];
    }
}

Tree *buildHuffmanTree(vector<pair<char, int> > freqtable)
{

    priority_queue<Tree *, vector<Tree *>, TreeComparator> huffqueue;
    for (int i = 0; i < freqtable.size(); i++)
    {
        Tree *node = new Tree();
        node->frequency = freqtable[i].second;
        node->character = freqtable[i].first;
        huffqueue.push(node);
    }

    while (huffqueue.size() > 1)
    {
        Tree *a, *b;
        a = huffqueue.top();
        huffqueue.pop();
        b = huffqueue.top();
        huffqueue.pop();
        Tree *c = new Tree();
        c->frequency = a->frequency + b->frequency;
        c->left = a;
        c->right = b;
        huffqueue.push(c);
    }

    return huffqueue.top();
}

int length(int a)
{
    int cc = 0;
    while(a!=0)
    {
        cc++;
        a/=10;
    }

    return cc;
}

void traverseHuffmanTree(Tree *root, int prev, int toAppend, map<char, int> &codemap)
{

    if (toAppend != -1)
    {
        prev = prev * 10 + toAppend;
    }
    if (root->right == NULL && root->left == NULL)
    {
        // cout << root->character << " ";
        if(length(prev)>8)
        {
            cout << length(prev);
            cout << endl;
        }
        
        
        codemap[root->character] = prev;
    }
    if (root->right != NULL)
    {
        traverseHuffmanTree(root->right, prev, 1, codemap);
    }

    if (root->left != NULL)
    {
        traverseHuffmanTree(root->left, prev, 0, codemap);
    }
}

char *readFileIntoBuffer(char *path, int &sz)
{
    FILE *fp = fopen(path, "r");
    sz = 0;
    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = (char *)malloc(sz);
    fread(buffer, 1, sz, fp);
    return buffer;
}

void writeFileFromBuffer(char *path, char *buffer, int sz)
{
    FILE *fp = fopen(path, "w");
    fwrite(buffer, 1, sz, fp);
}

char getCharFromBuffer(vector<char> &buffer)
{
    int op = 0;
    for (int i = 0; i < buffer.size(); i++)
    {
        op = op * 10 + (buffer[i] - '0');
    }

    buffer.erase(buffer.begin(), buffer.end());
    return op - '0';
    if (buffer.size() < 8)
    {
    }
    else
    {
    }
}
char *getStringFromHuffman(char *buffer, map<char, int> codes, int sz)
{
    char* outputBuffer = (char*)malloc(sz);
    for(int i=0; i<sz; i++)
    {
        outputBuffer[i]=codes[buffer[i]]-'0';
    }

    //This itself is wrong

    return outputBuffer;
    
}

vector<pair<char, int> > convertToVector(map<char, int> codes)
{
    vector<pair<char, int> > codesV;

    for (map<char, int>::iterator i = codes.begin(); i != codes.end(); i++)
    {
        codesV.push_back(make_pair(i->first, i->second));
    }

    return codesV;
}

char* convertStringToBuffer(char* bitstring, int sz)
{
    vector<char> outputBuffer;
    int interval = 0;
    char bit = 0;
    for(int i=0; i<sz; i++)
    {
        
        if(interval==8)
        {
            interval = 0;
            // cout<<(bit-'0')<<endl;
            outputBuffer.push_back(bit);
            bit = 0;
        
        }

        else{
            bit = (bit<<1)|(bitstring[i]-'0');
        }

        interval++;
    }

    if(interval>0)
    {
        outputBuffer.push_back(bit);
    }

    return outputBuffer.data();
}

void compressFile(char *path, char *output_path, map<char, int> &codes)
{
    int sz = 0;
    char *buffer = readFileIntoBuffer(path, sz);
    // map<char, int> codes;
    map<char, int> freqtable;

    for (int i = 0; i < sz; i++)
    {
 
            freqtable[buffer[i]]++;
 
    }

    Tree *root = buildHuffmanTree(convertToVector(freqtable));
    traverseHuffmanTree(root, 0, -1, codes);

    // First get output bitstring
    //Then write in terms of bytes

    char *outputString = getStringFromHuffman(buffer, codes, sz);
    
    char* outputBuffer = convertStringToBuffer(outputString, sz);
    writeFileFromBuffer(output_path, outputBuffer, sz);
}

char* getDecodedBuffer(char* fileBuffer, map<char, int> codes, int &sz)
{
    int bit = 0;

    
    map<int, char> reversecodes;
    vector<char> buffer;

    for(map<char, int>::iterator i = codes.begin(); i!=codes.end(); i++)
    {
        reversecodes[i->second] = i->first;
    }
    for(int i=0; i<sz; i++)
    {
        if(reversecodes.find(bit)==reversecodes.end())
        {
            

            bit = (bit<<1)|(fileBuffer[i]);

            
            
        }

        else{
            
            buffer.push_back(reversecodes[bit]);
            bit = 0;
        }
    }

    sz = buffer.size();
    return buffer.data();
}


void decompressFile(char* inputPath, char* outputPath, map<char, int> codes)
{
    int sz = 0;

    char* fileBuffer = readFileIntoBuffer(inputPath, sz);
    char* outputBuffer = getDecodedBuffer(fileBuffer, codes, sz);
    
    writeFileFromBuffer(outputPath, outputBuffer,sz);

}

int main(int argc, char* argv[])
{

    // vector<pair<char, int> > freqtable;

    // map<char, int> altTable;
    // for (int i = 0; i < 6; i++)
    // {
    //     char character;
    //     int frequency;
    //     cin >> character >> frequency;
    //     freqtable.push_back(make_pair(character, frequency));
    // }

    // Tree *root = buildHuffmanTree(freqtable);
    // vector<int> code;
    // traverseHuffmanTree(root, 0, -1, altTable);

    char* dEI = "test.jpg";
    char* dEO = "test1.jpg";
    char *dDO = "decoded.jpg";

    if(argc==4)
    {
        dEI = argv[1];
        dEO = argv[2];
        dDO=argv[3];
    }
    map<char, int> codes;
    compressFile(dEI, dEO, codes);
    decompressFile(dEO, dDO, codes);
}