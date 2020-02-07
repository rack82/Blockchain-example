#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

typedef struct bitCoinTree tnode;
typedef struct bitCoinIDNode BN;
typedef struct BitCoinBalanceNode wallet;
typedef struct bucketNodeStruct bucket;
typedef struct bucketItemStruct bucketItem;
typedef struct hashItemStruct hashItem;
typedef struct hashTableStruct hashTable;
typedef struct TransListNodeStruct TransListNode;


struct bitCoinTree
{
  char *bitCoinId;
  char *walletID;
  int amount;
  TransListNode *ref;
  tnode *next;
  tnode *left;
  tnode *right;
};//tnode

struct bitCoinIDNode
{
  int value;
  char *bitCoinID;
  BN *next;
  BN *prev;
};//BN

struct BitCoinBalanceNode
{
  char *walletID;
  BN *child;
  wallet *next;
};//wallet

struct TransListNodeStruct
{
  int transactionId;
  char *sender_receiver;
  char *Date;
  char *time;
  int amount;
  TransListNode *next;
};//TransListNode

struct bucketItemStruct
{
  char *walletID;
  TransListNode *TransList;
};//bucketitem

struct bucketNodeStruct
{
  int size;
  bucketItem **item;
  bucket *nextBucket;
};//bucket

struct hashItemStruct
{
  int index;
  bucket *headBucket;
};//hashitem

struct hashTableStruct
{
  int size;
  hashItem **items;
};//hashtable

unsigned int Hash(char*);
unsigned int TransIdHash(char*);
void InitBitCoinBalances(std::string, wallet**, tnode**, int);
tnode* InitBitCoinTreeList();
wallet* InitBalanceList();
void allocateBitCoin(tnode**, char*, int);
void BalanceListAdd(wallet*, char*);
void PrintBalanceList(wallet**);
hashTable* initHashTable(int);
void initTransactions(std::string, hashTable**, hashTable**, int, wallet**, tnode**);
void PrintTable(hashTable**);
void allocateTransNode(bucketItem**, char**, int, wallet**, tnode**);
void allocateHashTable(hashTable**, char**, int, int, int, wallet**, tnode**);
void updateBitCoinTree(tnode**, char*, int, int, char*, char*, TransListNode**);
void UpdateWallet(wallet**, tnode**, char**, int, TransListNode**);
int checkBalance(BN**, int);


void requestTransaction(char**, hashTable**, hashTable**, int, wallet**, tnode**);
void requestTransactions(char**, hashTable**, hashTable**, int, wallet**, tnode**);
