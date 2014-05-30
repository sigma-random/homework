#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<iterator>
#include<algorithm>

using namespace std;


typedef unsigned int uint;

typedef struct _WordNode
{
    string word;
    uint count;
}WordNode;


 /* 
  *	 sort rule
  */
int WordCountSort(const WordNode &A, const WordNode &B )
{
    return A.count > B.count;
}

 /* 
  *	 sort and print each word
  */
void printWordCount(map<string, uint> &WordMap)
{
    map<string, uint>::iterator MapIter;
    vector<WordNode> WordList;
    vector<WordNode>::iterator VecIter;
    WordNode Node;
    for(MapIter = WordMap.begin(); MapIter != WordMap.end(); MapIter++)
    {
        //cout << MapIter->first <<"    "<<MapIter->second<<endl;
        Node.word = MapIter->first;
        Node.count = MapIter->second;
        WordList.push_back(Node);
    }
    sort(WordList.begin(), WordList.end(),WordCountSort);
    for(VecIter = WordList.begin(); VecIter != WordList.end(); VecIter++)
    {
        cout <<left  << setw(50)<< VecIter->word <<VecIter->count<<endl;
    }
}


 /* 
  *	parse a line of string to  words 
  */
void WordParser(string line, map<string,uint> &WordMap)  
{
    int index = 0;
    int count = 0;
    string word;
    int len = line.length();
    for(; index < len; index++)
    {
        if( (line[index]>='a' && line[index]<='z') ||\
            (line[index]>='A' && line[index]<='Z') ) 
        {
            count++;
        }
        else if(count)	 //not a letter 
        {
            word = line.substr(index-count, count);
            WordMap[word]++;
            count = 0;   
        }
        if(len-1 == index && count)
        {
            word = line.substr(index-count+1, count);
            WordMap[word]++;
            return;
       }
    }
}

int main(int argc, char **argv)
{
    map<string,uint> WordMap;
    string word;
    string line;

	if(argc!=2)
	{
		cout<<"usage:  "<<argv[0]<<" filename"<<endl;
	}
    ifstream in(argv[1]);
    while(in >> line)    /*while(getline(in, line))*/
    {
      WordParser(line, WordMap);
    }
    printWordCount(WordMap);
    in.close();
    cin.get();
    return 0;
}
