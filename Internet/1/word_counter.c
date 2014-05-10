#include<stdio.h>
#include<stdlib.h>
#include<string.h>



#define unsigned int uint
#define TRUE 1
#define FALSE 0

#define HASH_SIZE 65535 /*size of the hash table*/
#define HASH_SEED 11    /*size of the hash table*/


//store word information
typedef struct _wordNode
{
    int count;
    char* word;
    struct _wordNode* next;
}wordNode;

typedef struct _hashTable
{
    wordNode* pWordNodes[HASH_SIZE];
}hashTable;


int totalWords;
static char* ign_list[] = {"s","es","'s"};


uint getHashKey(char *pWord)
{
    uint s = 0;
    while(*pWord)
    {
        s = HASH_SEED * s + *pWord++;
    }
    return s % HASH_SIZE;
}

void initHashTable(hashTable *pHashTable)
{
    int index;
    for(index = 0; index < HASH_SIZE; index++)
    {
        pHashTable->pWordNodes[index] = NULL;
    }
}



wordNode* newWordNode(char *pWord)
{
    int len = strlen(pWord);
    wordNode* pWordNode = malloc(sizeof(wordNode));
    pWordNode->count = 1;
    pWordNode->word = malloc(len);
    strncpy(pWordNode->word,pWord,len);
    pWordNode->next = NULL;
    return pWordNode;
}

int deleteWordNode(hashTable *pHashTable, uint key)
{
    wordNode *pWordNode = pHashTable->pWordNodes[key];
    if(pWordNode)
    {
        pHashTable->pWordNodes[key] = pWordNode->next;
        pWordNode->next = NULL;
        if(pWordNode->word)
        {
            free(pWordNode->word);
            pWordNode->word = NULL;
        }
        pWordNode->count = 0;
        free(pWordNode);
        return TRUE;
    }
    return FALSE;
}

int addWord(hashTable *pHashTable, char *pWord)
{
    uint key = getHashKey(pWord);
    wordNode *preWordNode,*curWordNode;

    while(preWordNode)
    {
        /* find the same word node*/
        if( 0 == strcmp(pWord,preWordNode->word))
        {
            preWordNode->count++;
            return key;
        }
        preWordNode = preWordNode->next;
    }
    /* don't find word in list , alloc  new word node , and insert into head*/
    curWordNode = newWordNode(pWord);
    preWordNode = pHashTable->pWordNodes[key];
    pHashTable->pWordNodes[key] = curWordNode;
    curWordNode->next = preWordNode;
    return key;
}


int main(int argc, char **argv)
{

    size_t t = &(((wordNode*)(NULL))->word);     
    printf("%d\n",t);    
    return 0;
}


