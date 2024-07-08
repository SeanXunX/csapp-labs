#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINENUM 30
#define ADDNUM 17
#define bool int
#define true 1
#define false 0

typedef struct LineNode
{
    int flag;
    struct LineNode *next;
} LineNode;

typedef struct Set
{
    /**
     * head-> node(most frequently) -> ... -> node(least frequently)
     */
    LineNode *head;
    int size; // now lines
} Set;

int Str2Int(const char *str)
{
    int res = 0;
    sscanf(str, "%d", &res);
    return res;
}

int HexStr2Int(const char *str)
{
    int res = 0;
    sscanf(str, "%x", &res);
    return res;
}

int hash(int origin, int setNum)
{
    return origin % setNum;
}

void getSubstr(const char *src, char *dest, int begin, int end)
{
    for (int i = begin; i < end; ++i)
    {
        dest[i - begin] = src[i];
    }
}

/**
 * extract address from the line
 */
void getAddress(char *line, char *address)
{
    int i = 3, size = 0;
    while (line[i] != ',')
    {
        address[size] = line[i];
        ++i;
        ++size;
    }
    --size;
    if (size < 15)
    {
        // extends to 16 bits, adding '0' ahead
        for (i = 15; size >= 0; --size, --i)
        {
            address[i] = address[size];
        }
        while (i >= 0)
        {
            address[i--] = '0';
        }
    }
    address[ADDNUM - 1] = '\0';
}

LineNode *createNode(int flag_, LineNode *next_)
{
    LineNode *node = (LineNode *)malloc(sizeof(LineNode));
    node->flag = flag_;
    node->next = next_;
    return node;
}

void initHashTable(Set hashTable[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        hashTable[i].head = createNode(-1, NULL);
        hashTable[i].size = 0;
    }
}

bool isHit(Set *targetSet, int flag_)
{
    LineNode *temp = targetSet->head->next, *prev = targetSet->head;
    while (temp)
    {
        if (temp->flag == flag_)
        {
            // update the set according to least-frequently-used rule
            prev->next = temp->next;
            temp->next = targetSet->head->next;
            targetSet->head->next = temp;
            return true;
        }
        temp = temp->next;
        prev = prev->next;
    }
    return false;
}

void addLine(Set *targetSetPtr, LineNode *node)
{
    // add at the first of set
    LineNode *temp = targetSetPtr->head->next;
    targetSetPtr->head->next = node;
    node->next = temp;
    ++targetSetPtr->size;
}

void evictLine(Set *targetSetPtr, LineNode *node)
{
    LineNode *temp = targetSetPtr->head->next;
    targetSetPtr->head->next = node;
    node->next = temp;
    LineNode *prev = node;
    while (temp->next)
    {
        temp = temp->next;
        prev = prev->next;
    }
    prev->next = NULL;
    if (temp)
    {
        free(temp);
    }
}

void deleteHT(Set ht[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        LineNode *cur = ht[i].head, *follow = cur->next;
        while (cur)
        {
            follow = cur->next;
            free(cur);
            cur = follow;
        }
    }
}

void ProcessFile(int setBits, int lineNum, int blockBits,
                 const char *fileName)
{
    FILE *infile = fopen(fileName, "r");
    char address[ADDNUM], line[LINENUM];
    int setNum = 1 << setBits;
    int hits = 0, misses = 0, evicts = 0;

    // create empty sets heads
    Set hashTable[setNum];
    initHashTable(hashTable, sizeof(hashTable) / sizeof(Set));

    while (fgets(line, LINENUM, infile) != NULL)
    {
        if (line[0] != ' ')
        {
            continue;
        }
        // reads in lines of the tracefile
        // extract the address
        getAddress(line, address);
        int addNum = HexStr2Int(address);

        /**
         * get index and flag
         */
        addNum >>= blockBits;
        int t1 = (-1) ^ ((-1) << setBits);
        int index = addNum & t1;
        int flag = addNum >> setBits;

        // int totalLen = strlen(address);
        // // get flag string
        // getSubstr(address, flagStr, 0, totalLen - blockBits - setBits);
        // // get index string
        // getSubstr(address, indexStr, totalLen - blockBits - setBits,
        //          totalLen - blockBits);
        // int index = hash(HexStr2Int(indexStr), setNum);
        Set *targetSet = &hashTable[index];
        if (isHit(targetSet, flag))
        {
            ++hits;
        }
        else
        {
            // not hit
            ++misses;
            LineNode *lineNode = createNode(flag, NULL);
            if (targetSet->size < lineNum)
            {
                // not full in targetSet
                addLine(targetSet, lineNode);
            }
            else
            {
                ++evicts;
                evictLine(targetSet, lineNode);
            }
        }
        if (line[1] == 'M')
        {
            ++hits;
        }
    }
    printSummary(hits, misses, evicts);
    deleteHT(hashTable, sizeof(hashTable) / sizeof(Set));
    fclose(infile);
}

int main(int argc, char *argv[])
{
    int s = Str2Int(argv[2]);       // number of set index bits
    int E = Str2Int(argv[4]);       // associativity, number of lines per set
    int b = Str2Int(argv[6]);       // number of block bits
    const char *fileName = argv[8]; // file name
    ProcessFile(s, E, b, fileName);
    return 0;
}
