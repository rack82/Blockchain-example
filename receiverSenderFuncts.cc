#include "utils.h"

using namespace std;


void initTransactions(string TransactionsFile, hashTable** receiver, hashTable** sender, int size, wallet** wlist, tnode** Btree)
{
  ssize_t read;
  size_t length = 0;
  char **sentence;
  FILE *file = fopen("trans", "r");

  wallet *templist = *wlist;
  tnode *btree = *Btree;
  char *line = NULL;
  char *token = NULL;
  sentence = new char *[6];
  while((read = getline(&line, &length, file)) != -1){
    int count = 0;
    token = strtok(line, " \n");
    while(token != NULL){
      sentence[count] = new char[sizeof (strlen(token) + 1)];
      strcpy(sentence[count], token);
      //cout << sentence[count] << endl;
      count++;
      token = strtok(NULL, " \n");
    }


    if ((*sender) == NULL){
      fprintf(stderr, "uninitialized htable.....\n");
      exit(1);
    }
    //cout << "sender wallet id = " << sentence[1] << "\n\n\n" <<  endl;
    int h = Hash(sentence[1]);
    int h1 = Hash(sentence[2]);
    hashTable *tempSender = *sender;
    hashTable *tempReceiver = *receiver;
    allocateHashTable(&tempSender, sentence, size, h, 1, &templist, &btree);
    allocateHashTable(&tempReceiver, sentence, size, h1, 2, &templist, &btree);
  }
  fclose(file);
  if(line)
    delete line;
  for (int i = 0; i < 6; i++)
    delete sentence[i];
  delete sentence;
}

void allocateHashTable(hashTable** table, char** sentence, int size, int h, int choice, wallet** wlist, tnode** btree)
{
  int flag = 0;
  if (choice == 1)
    flag = 2;
  else
    flag = 1;

  wallet *list = *wlist;
  tnode *tree = *btree;
  if ((*table)->items != NULL){
    // no bucket at all
    if ((*table)->items[h]->headBucket == NULL){
      (*table)->items[h]->headBucket = new bucket;
      (*table)->items[h]->headBucket->nextBucket = NULL;
      (*table)->items[h]->headBucket->size = size;
      (*table)->items[h]->headBucket->item = new bucketItem *[size];

      for (int i = 0; i < size; i++){
        (*table)->items[h]->headBucket->item[i] = new bucketItem;
        (*table)->items[h]->headBucket->item[i]->walletID = NULL;
        (*table)->items[h]->headBucket->item[i]->TransList = NULL;
      }
      for (int i = 0; i < size; i++){
        if (((*table)->items[h]->headBucket->item[i]->walletID) == NULL){
          (*table)->items[h]->headBucket->item[i]->walletID = new char[sizeof (strlen(sentence[choice]) + 1)];
          strcpy((*table)->items[h]->headBucket->item[i]->walletID, sentence[choice]);
          bucketItem *current1 = (*table)->items[h]->headBucket->item[i];
          allocateTransNode(&current1, sentence, flag, &list, &tree);
          break;
        }
      }
    }
    // at least one bucket
    else {
      for (int i = 0; i < size; i++){
        if (((*table)->items[h]->headBucket->item[i]->walletID) != NULL){
          if (strcmp((*table)->items[h]->headBucket->item[i]->walletID, sentence[choice]) == 0){
            bucketItem *current2 = (*table)->items[h]->headBucket->item[i];
            allocateTransNode(&current2, sentence, flag, &list, &tree);
            //append trans list
            break;
          }
          if (i == (size - 1)){
            int stop = 0;
            bucket *current = (*table)->items[h]->headBucket;
            while (current->nextBucket != NULL){
              current = current->nextBucket;
              for (int k = 0; k < size; k++){
                if (current->item[k]->walletID != NULL){
                  if (strcmp(current->item[k]->walletID, sentence[choice]) == 0){
                    bucketItem *curr1 = current->item[k];
                    allocateTransNode(&curr1, sentence, flag, &list, &tree);
                    //append Trans List
                    stop = 1;
                    break;
                  }
                }
                else {
                  current->item[k]->walletID = new char[sizeof (strlen(sentence[2]) + 1)];
                  strcpy(current->item[k]->walletID, sentence[2]);
                  bucketItem *curr2 = current->item[k];
                  allocateTransNode(&curr2, sentence, flag, &list, &tree);
                  //append TransList
                  stop = 1;
                  break;
                }
              }
            }
            if (stop == 1)
              break;
            current->nextBucket = new bucket;
            current->nextBucket->item = new bucketItem *[size];
            current->nextBucket->size = size;
            for (int j = 0; j < size; j++){
              current->nextBucket->item[j] = new bucketItem;
              current->nextBucket->item[j]->walletID = NULL;
              current->nextBucket->item[j]->TransList = NULL;
            }
            for (int j = 0; j < size; j++){
              if ((current->nextBucket->item[j]->walletID) == NULL){
                current->nextBucket->item[j]->walletID = new char[sizeof (strlen(sentence[choice]) + 1)];
                strcpy(current->nextBucket->item[j]->walletID, sentence[choice]);
                bucketItem *curr3 = current->item[j];
                allocateTransNode(&curr3, sentence, flag, &list, &tree);
                // append TransList
                break;
              }
            }
          }
        }
        else {
          (*table)->items[h]->headBucket->item[i]->walletID = new char[sizeof (strlen(sentence[choice]) + 1)];
          strcpy((*table)->items[h]->headBucket->item[i]->walletID, sentence[choice]);
          bucketItem *current4 = (*table)->items[h]->headBucket->item[i];
          allocateTransNode(&current4, sentence, flag, &list, &tree);
          break;
        }
      }
    }
  }
}

void PrintTable(hashTable** table)
{
  bucket *current = NULL;
  if ((*table)->items != NULL){
    for (int i = 0; i < (*table)->size; i++){
      current = (*table)->items[i]->headBucket;
      while (current != NULL){
        for (int j = 0; j < current->size; j++){
          if ((current->item[j]->walletID) != NULL){
            cout << current->item[j]->walletID << " ";
            bucketItem *temp = current->item[j];
            while (temp->TransList != NULL){
              cout << temp->TransList->transactionId << " " << temp->TransList->sender_receiver << " " << temp->TransList->Date << " " << temp->TransList->time << " " << temp->TransList->amount  << " " << "\n" << endl;
              temp->TransList = temp->TransList->next;
            }
          }
        }
        current = current->nextBucket;
      }
    }
  }
}



void allocateTransNode(bucketItem** node, char** sentence, int choice, wallet** wlist, tnode** Btree )
{
  int flag = 0;
  if (choice == 2)
    flag = 1;
  else
    flag = 2;
  bucketItem *temp = *node;
  wallet *tempList = *wlist;
  tnode *tree = *Btree;
  if (temp->TransList == NULL){
    temp->TransList = new TransListNode;
    temp->TransList->transactionId = atoi(sentence[0]);
    temp->TransList->sender_receiver = new char[sizeof (strlen(sentence[choice]) + 1)];
    strcpy(temp->TransList->sender_receiver, sentence[choice]);
    temp->TransList->amount = atoi(sentence[3]);
    temp->TransList->Date = new char[sizeof (strlen(sentence[4]) + 1)];
    strcpy(temp->TransList->Date, sentence[4]);
    temp->TransList->time = new char[sizeof (strlen(sentence[5]) + 1)];
    strcpy(temp->TransList->time, sentence[5]);
    TransListNode *tempNode = temp->TransList;
    UpdateWallet(&tempList, &tree, sentence, flag, &tempNode);
  }
  else {
    TransListNode *list = temp->TransList;
    while (list->next != NULL)
      list = list->next;
    list->next = new TransListNode;
    list->next->transactionId = atoi(sentence[0]);
    list->next->sender_receiver = new char[sizeof (strlen(sentence[choice]) + 1)];
    strcpy(list->next->sender_receiver, sentence[choice]);
    list->next->amount = atoi(sentence[3]);
    list->next->Date = new char[sizeof (strlen(sentence[4]) + 1)];
    strcpy(list->next->Date, sentence[4]);
    list->next->time = new char[sizeof (strlen(sentence[5]) + 1)];
    strcpy(list->next->time, sentence[5]);
    TransListNode *tempNode = list;
    UpdateWallet(&tempList, &tree, sentence, flag, &tempNode);
  }
}

void UpdateWallet(wallet** wlist, tnode** tree, char** sentence, int choice, TransListNode** transListNode)
{
  int amount = atoi(sentence[3]);
  int count = 0;
  int sub = amount;
  wallet *current = *wlist;
  TransListNode *listnode = *transListNode;
  while (current != NULL){
    //cout << "comparing " << current->walletID << ", " << sentence[choice] << "\n" << endl;
    if (strcmp(current->walletID, sentence[choice]) == 0){
      BN *temp = current->child;
      if (choice == 1){//sender
        //cout << "looking for to update sender " << sentence[choice] << "\n\n" << endl;
        wallet *receiver = *wlist;
        //cout << "checking " << current->walletID << "\n\n\n" << endl;
        if (checkBalance(&temp, amount) == 1){
          //cout << "checking " << current->walletID << "\n\n\n" << endl;
          while ((temp != NULL) && (sub > 0)){
            if ((sub = sub - temp->value) < 0){
              temp->value = sub * (-1);
              //updatebitCoinTree(temp->bitCoinID, amount, temp->value, sentence[1], sentence[2])

              /** update receiver's wallet*/
              wallet *curReceiver = *wlist;
              while (curReceiver->next != NULL){
                if (strcmp(curReceiver->walletID, sentence[2]) == 0){
                  BN *tempReceiver = (curReceiver)->child;
                  while (tempReceiver != NULL){
                    if (strcmp(tempReceiver->bitCoinID, temp->bitCoinID) == 0){
                      tempReceiver->value = tempReceiver->value + amount;
                      break;
                    }
                    if (tempReceiver->next == NULL){
                      tempReceiver->next = new BN;
                      tempReceiver->next->bitCoinID = new char[sizeof (strlen(temp->bitCoinID) + 1)];
                      strcpy(tempReceiver->next->bitCoinID, temp->bitCoinID);
                      tempReceiver->next->value = amount;
                      tempReceiver->next->next = NULL;
                      tempReceiver->next->prev = tempReceiver;
                      break;
                    }
                    tempReceiver = tempReceiver->next;
                  }
                  //find bitcoin and update
                  break;
                }
                curReceiver = curReceiver->next;
              }
            }
            else {
              int remain = sub - amount;
              remain = remain * (-1);
              //temp->bitCoinID
              //updateBitCoinTree(temp->bitid, remain, 0, sentence[1], sentence[2])

              /** update receiver's wallet*/
              wallet *curReceiver = *wlist;
              while (curReceiver->next != NULL){
                if (strcmp(curReceiver->walletID, sentence[2]) == 0){
                  BN *tempReceiver = (curReceiver)->child;
                  while (tempReceiver != NULL){
                    if (strcmp(tempReceiver->bitCoinID, temp->bitCoinID) == 0){
                      tempReceiver->value = tempReceiver->value + remain;
                      break;
                    }
                    if (tempReceiver->next == NULL){
                      tempReceiver->next = new BN;
                      tempReceiver->next->bitCoinID = new char[sizeof (strlen(temp->bitCoinID) + 1)];
                      strcpy(tempReceiver->next->bitCoinID, temp->bitCoinID);
                      tempReceiver->next->value = remain;
                      tempReceiver->next->next = NULL;
                      tempReceiver->next->prev = tempReceiver;
                      break;
                    }
                    tempReceiver = tempReceiver->next;
                  }
                  //find bitcoin and update
                  break;
                }
                curReceiver = curReceiver->next;
              }


              if (temp->prev == NULL){
                if (temp->next != NULL){
                  current->child = temp->next;
                  temp->next->prev = NULL;
                  delete temp;
                  temp = current->child;
                }
                else {
                  delete temp;
                  current->child = NULL;
                }
              }
              else {
                BN *cur = temp;
                cur->next->prev = temp->prev;
                cur->prev->next = temp->next;
                temp = cur->next;
                delete cur;
              }
            }
          }
        }
        else if (checkBalance(&temp, amount) == 0){
          cout << "not enough Coin....\n\n" << endl;
          return;
        }
        else{
          cout << "no Coins at all...." << endl;
          return;
        }
      }
      else
        return;
      break;
    }
    current = current->next;
  }
}


int checkBalance(BN** bitcoins, int amount)
{
  int count = 0;
  BN *current = *bitcoins;
  if (current != NULL){
    count = count + current->value;
    while (current->next != NULL){
      count = count + current->value;
      current = current->next;
    }
    return (count >= amount);
  }
  else{
    //cout << "zero available coins" << endl;
    return -1;
  }
}

void updateBitCoinTree(tnode** Btree, char* bitCoin, int sentAmount, int remains, char* sender, char* receiver, TransListNode** transList)
{
  tnode *tempTree = *Btree;
  while (tempTree->next != NULL){
    if (strcmp(tempTree->bitCoinId, bitCoin) == 0){
      if ((tempTree->left == NULL) && (tempTree->right == NULL)){
        tempTree->left = new tnode;
        tempTree->left->walletID = new char[sizeof (strlen(receiver) + 1)];
        strcpy(tempTree->left->walletID, receiver);
        tempTree->left->amount = sentAmount;
        tempTree->left->ref = *transList;
        tempTree->left->left = NULL;
        tempTree->left->right = NULL;

        tempTree->right = new tnode;
        tempTree->right->walletID = new char[sizeof (strlen(sender) + 1)];
        strcpy(tempTree->right->walletID, sender);
        tempTree->right->amount = remains;
        tempTree->right->ref = NULL;
        tempTree->right->left = NULL;
        tempTree->right->right = NULL;
      }
      break;
    }
    tempTree = tempTree->next;
  }
}

unsigned int TransIdHash(char *s)
{
  unsigned int h = 0;
  int c;

  while((c = *s++))
    h = ((h << 4) + h) ^ c;
    //h = h + c;
  return h;
}

unsigned int Hash(char* s)
{
    unsigned int h = 0;
    int c;

    while((c = *s++))
      h = ((h << 4) + h) + c;
      //h = h + c;
    h = h % 11;
    return h;
}
