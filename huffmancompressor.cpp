#include <bits/stdc++.h>
using namespace std;
struct Tree
{
    int frequency;
    unsigned char character;
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

Tree *buildHuffmanTree(vector<pair<unsigned char, int> > freqtable)
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

string toBinary(unsigned  char a)
{
    string output  = "";
    while(a!=0)
    {
        string bit = a%2==0?"0":"1";
        output+=bit;
        a/=2;
    }

    if(output.size()<8)
    {
        // cout<<"LESS";
        int deficit = 8 - output.size();
        for(int i=0; i<deficit; i++)
        {
            output+="0";
        }
    }

    reverse(output.begin(), output.end());
    // cout<<output<<endl;
    return output;
    
}
//We're traversing the whole tre, Iright?
void traverseHuffmanTree(Tree *root, int prev, int toAppend, map<unsigned char, int> &codemap)
{

    if (toAppend != -1)
    {
        prev = prev * 10 + toAppend;
    }
    if (root->right == NULL && root->left == NULL)
    {
        // cout << root->unsigned character << " ";
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

unsigned char *readFileIntoBuffer(char *path, int &sz)
{
    FILE *fp = fopen(path, "rb");
    sz = 0;
    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char *buffer = (unsigned char *)malloc(sz);
    fread(buffer, 1, sz, fp);
    return buffer;
}

void writeFileFromBuffer(char *path, unsigned char *buffer, int sz)
{
    FILE *fp = fopen(path, "wb");
    fwrite(buffer, 1, sz, fp);

    fclose(fp);
}

unsigned char getcharFromBuffer(vector<unsigned char> &buffer)
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
string getStringFromHuffman(unsigned char *buffer, map<unsigned char, int> codes, int sz)
{
    string outputBuffer="";
    for(int i=0; i<sz; i++)
    {
        outputBuffer=outputBuffer+to_string(codes[buffer[i]]);
    }

    if(outputBuffer.size()%8!=0)
    {
        int deficit = 8*((outputBuffer.size()/8)+1)-outputBuffer.size();
        for(int i=0; i<deficit; i++)
        {
            outputBuffer+="0";
        }
    }

    //This itself is wrong

    return outputBuffer;
    
}

vector<pair<unsigned char, int> > convertToVector(map<unsigned char, int> codes)
{
    vector<pair<unsigned char, int> > codesV;

    for (map<unsigned char, int>::iterator i = codes.begin(); i != codes.end(); i++)
    {
        codesV.push_back(make_pair(i->first, i->second));
    }

    return codesV;
}

unsigned char* convertStringToBuffer(string bitstring, int& sz)
{
    vector<unsigned char> outputBuffer;
    int interval = 0;
    unsigned char bit = 0;

    for(int i=0; i<sz; i++)
    {
        
        bit = (bit<<1)|(bitstring[i]-'0');
         
        interval++;
        if(interval==8)
        {
            // cout<<"Y"<<endl;
            interval = 0;
            cout<<toBinary(bit)<<endl;
            outputBuffer.push_back(bit);
            bit = 0;
        
        }

        
    }

    if(interval>0)
    {
        outputBuffer.push_back(bit);
    }

   
    sz = outputBuffer.size();
    return outputBuffer.data();
}



void compressFile(char *path, char *output_path, map<unsigned char, int> &codes)
{
    int sz = 0;
    unsigned char *buffer = readFileIntoBuffer(path, sz);
    // map<unsigned char, int> codes;
    map<unsigned char, int> freqtable;

    for (int i = 0; i < sz; i++)
    {
 
            freqtable[buffer[i]]++;
 
    }

    Tree *root = buildHuffmanTree(convertToVector(freqtable));
    traverseHuffmanTree(root, 0, -1, codes);

    // First get output bitstring
    //Then write in terms of bytes

    string outputString = getStringFromHuffman(buffer, codes, sz);
    cout<<outputString.size()<<endl;
    sz  = outputString.size();
    
    unsigned char* outputBuffer = convertStringToBuffer(outputString, sz);
    writeFileFromBuffer(output_path, outputBuffer, sz);
}


string convertBufferToBitString(unsigned char* buffer, int sz)
{
    string bitstring = "";

    
    for(int i=0; i<sz; i++)
    {
        bitstring+=toBinary(buffer[i]);
        cout<<"B"<<toBinary(buffer[i])<<endl;
    }

    return bitstring;
}

unsigned char* getDecodedBuffer(string bitstring, map<unsigned char, int> codes, int &sz)
{
    string bit = string(1,bitstring[0]);

    
    map<int, unsigned char> reversecodes;
    vector<unsigned char> buffer;

    for(map<unsigned char, int>::iterator i = codes.begin(); i!=codes.end(); i++)
    {
        reversecodes[i->second] = i->first;
    }

    //We map unsigned chars to Integers
    //Now we need to map Integers in the form of 0101011 to unsigned char
    //But instead, we have integers in the form of 22s


    //store bitstring anyways
    //convert to bitstring first

    //check order
    for(int i=0; i<bitstring.size(); i++)
    {
        if(reversecodes.find(stoi(bit))==reversecodes.end())
        {
            

            bit+=string(1, bitstring[i]);

            
            
        }

        else{
            
            buffer.push_back(reversecodes[stoi(bit)]);
            bit = string(1, bitstring[i+1]);
        }
    }

    

    sz = buffer.size();
    return buffer.data();
}


void decompressFile( char* inputPath,  char* outputPath, map<unsigned char, int> codes)
{
    int sz = 0;
    unsigned char* fileBuffer = readFileIntoBuffer(inputPath, sz);
    string fileBitString = convertBufferToBitString(fileBuffer, sz);
    cout<<fileBitString<<endl;
    unsigned char* outputBuffer = getDecodedBuffer(fileBitString, codes, sz);
    
    writeFileFromBuffer(outputPath, outputBuffer,sz);
    //take care of appended zeroes
}

int main(int argc, char* argv[])
{

    // vector<pair<unsigned char, int> > freqtable;

    // map<unsigned char, int> altTable;
    // for (int i = 0; i < 6; i++)
    // {
    //     unsigned char unsigned character;
    //     int frequency;
    //     cin >> unsigned character >> frequency;
    //     freqtable.push_back(make_pair(unsigned character, frequency));
    // }

    // Tree *root = buildHuffmanTree(freqtable);
    // vector<int> code;
    // traverseHuffmanTree(root, 0, -1, altTable);
    //fp state?

     char* dEI = "test.jpg";
    char* dEO = "test1.jpg";
    char *dDO = "decoded.jpg";


    if(argc==4)
    {
        dEI = argv[1];
        dEO = argv[2];
        dDO=argv[3];
    }
    map<unsigned char, int> codes;
    compressFile(dEI, dEO, codes);
    decompressFile(dEO, dDO, codes);
}