#include "utils.h"

using namespace std;

void requestTransaction(char** sentence, hashTable** receiver, hashTable** sender, int size, wallet** wlist, tnode** Btree)
{
  wallet *templist = *wlist;
  tnode *btree = *Btree;
  stringstream str;

  int s = TransIdHash(sentence[4]);
  str << s;
  delete(sentence[0]);
  sentence[0] = new char[sizeof (strlen(str.str().c_str()) + 1)];
  strcpy(sentence[0], str.str().c_str());

  for (int i = 0; i < 6; i++)
    cout << sentence[i] << endl;

  int h = Hash(sentence[1]);
  int h1 = Hash(sentence[2]);
  hashTable *tempSender = *sender;
  hashTable *tempReceiver = *receiver;
  allocateHashTable(&tempSender, sentence, size, h, 1, &templist, &btree);
  allocateHashTable(&tempReceiver, sentence, size, h1, 2, &templist, &btree);
}

void requestTransactions(char** sentence, hashTable** receiver, hashTable** sender, int size, wallet** wlist, tnode** Btree)
{
  wallet *templist = *wlist;
  tnode *btree = *Btree;
  stringstream str;

  int s = TransIdHash(sentence[4]);
  str << s;
  sentence[0] = new char[sizeof (strlen(str.str().c_str()) + 1)];
  strcpy(sentence[0], str.str().c_str());
  int h = Hash(sentence[1]);
  int h1 = Hash(sentence[2]);
  hashTable *tempSender = *sender;
  hashTable *tempReceiver = *receiver;
  allocateHashTable(&tempSender, sentence, size, h, 1, &templist, &btree);
  allocateHashTable(&tempReceiver, sentence, size, h1, 2, &templist, &btree);
}
