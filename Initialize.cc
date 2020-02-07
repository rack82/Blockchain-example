#include "utils.h"

using namespace std;

wallet* InitBalanceList()
{
  wallet *node = NULL;
  node = new wallet;
  node->walletID = NULL;
  node->next = NULL;
  node->child = NULL;
  return node;
}

tnode* InitBitCoinTreeList()
{
  tnode *head = NULL;
  head = new tnode;
  head->bitCoinId = NULL;
  head->walletID = NULL;
  head->amount = 0;
  head->ref = NULL;
  head->next = NULL;
  head->left = NULL;
  head->right = NULL;
  return head;
}

void PrintBalanceList(wallet** List)
{
  wallet *temp = *List;
  BN *temp2;
  while(temp != NULL){
    cout << temp->walletID << endl;
    temp2 = (temp)->child;
    while(temp2 != NULL){
      cout << temp2->bitCoinID << " = " << temp2->value << endl;
      temp2 = temp2->next;
    }
    temp = temp->next;
  }
}

void InitBitCoinBalances(string BalancesFile, wallet** BalanceList, tnode** bTree, int value)
{
  ssize_t read;
  size_t length = 0;
  FILE *file = fopen("test", "r");
  int count = 0;

  tnode *tree = *bTree;
  char *line = NULL;
  char *token = NULL;
  while((read = getline(&line, &length, file)) != -1){
    count = 0;
    token = strtok(line, " \n");
    while (token != NULL){
      count++;
      if (*BalanceList != NULL){
        if ((*BalanceList)->walletID == NULL){
          //cout << "before copying " << token << "\n\n" << endl;
          (*BalanceList)->walletID = new char[sizeof (strlen(token) + 1)];
          strcpy((*BalanceList)->walletID, token);
          //cout << "wallet ID = " << (*BalanceList)->walletID << "\n\n" << endl;
        }
        else if ((*BalanceList)->walletID != NULL && count == 1){
          wallet *current = *BalanceList;
          while(current->next != NULL)
            current = current->next;
          current->next = new wallet;
          current->next->walletID = new char[sizeof (strlen(token) + 1)];
          //cout << "before copying " << token << "\n\n" << endl;
          strcpy(current->next->walletID, token);
          //cout << "wallet ID = " << current->next->walletID << "\n\n" << endl;
          current->next->next = NULL;
          current->next->child = NULL;
        }
        else if ((*BalanceList)->walletID != NULL && count > 1){
          wallet *current = *BalanceList;
          BN *cur;
          while (current->next != NULL)
            current = current->next;
          if (current->child != NULL){
            cur = current->child;
            while(cur->next != NULL)
              cur = cur->next;
            cur->next = new BN;
            cur->next->bitCoinID = new char[sizeof (strlen(token) + 1)];
            strcpy(cur->next->bitCoinID, token);
            cur->next->value = value;
            cur->next->next = NULL;
            cur->next->prev = cur;
            allocateBitCoin(&tree, token, value);
          }
          else{
            current->child = new BN;
            current->child->bitCoinID = new char[sizeof (strlen(token) + 1)];
            strcpy(current->child->bitCoinID, token);
            current->child->value = value;
            current->child->next = NULL;
            current->child->prev = NULL;
            allocateBitCoin(&tree, token, value);
          }
        }
      }
      else
        cout << "not initialized" << endl;
      token = strtok(NULL, " \n");
    }
  }
  fclose(file);
  if(line)
    delete line;
}

void allocateBitCoin(tnode** tree, char* id, int value)
{
  tnode *current = *tree;
  if (current == NULL){
    current = new tnode;
    current->bitCoinId = new char[sizeof (strlen(id) + 1)];
    strcpy(current->bitCoinId, id);
    current->walletID = NULL;
    current->amount = value;
    current->ref = NULL;
    current->next = NULL;
    current->left = NULL;
    current->right = NULL;
  }
  else {
    while (current->next != NULL)
      current = current->next;
    current->next = new tnode;
    current->next->bitCoinId = new char[sizeof (strlen(id) + 1)];
    strcpy(current->next->bitCoinId, id);
    current->next->walletID = NULL;
    current->next->amount = value;
    current->next->ref = NULL;
    current->next->next = NULL;
    current->next->left = NULL;
    current->next->right = NULL;
  }
}

hashTable* initHashTable(int size)
{
  hashTable *ht = NULL;
  ht = new hashTable;
  ht->size = size;
  ht->items = new hashItem *[size];
  for (int i = 0; i < size; i++){
    ht->items[i] = new hashItem;
    ht->items[i]->index = i;
    ht->items[i]->headBucket = NULL;
  }
  return ht;
}
