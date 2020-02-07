#include "utils.h"

using namespace std;


int main(int argc, char *argv[])
{
  wallet *BalList;
  hashTable *ReceiverHtable, *SenderHtable;
  char *balanceFile, *TransFile;
  int senderHashSize = 0, receiverHashSize = 0, bucketSize = 0, i = 0;

  if (argc < 11){
    fprintf(stderr, "not enough arguments... \n");
    exit(1);
  }


  tnode *bitcoins = NULL;
  BalList = InitBalanceList();
  ReceiverHtable = initHashTable(50);
  SenderHtable = initHashTable(50);
  InitBitCoinBalances("test", &BalList, &bitcoins, 500);
  //PrintBalanceList(&BalList);
  cout << "\n" << endl;
  initTransactions("trans", &ReceiverHtable, &SenderHtable, 4, &BalList, &bitcoins);
  cout << "\n" << endl;
  PrintBalanceList(&BalList);
  //PrintTable(&SenderHtable);
  //PrintTable(&ReceiverHtable);



  cout << "\n\n" << endl;
  cout << "\t Please Type your command.." << endl;
  cout << "\t /requestTransaction [sender] [receiver] [amount)] [date] [time]" << endl;
  cout << "\t /requestTransactions [sender] [receiver] [amount)] [date] [time];[sender] [receiver] [amount)] [date] [time] " << endl;
  cout << "\t /requestTransactions [input file]" << endl;
  cout << "\t /findEarnings [walletID] [time1] [year1] [time2] [year2]" << endl;
  cout << "\t /findPayments [walletID] [time1] [year1] [time2] [year2]" << endl;
  cout << "\t /walletStatus [walletID]" << endl;
  cout << "\t /bitCoinStatus [bitCoinID]" << endl;
  cout << "\t /traceCoin bitCoinID" << endl;
  cout << "\t /exit\n\n" << endl;

  string command;
  char *cstr;
  char *input, *token, *tkn, *tempstr;
  char **sentence;

  while(1){
    cout << ">>";

    getline(cin, command);
    cstr = new char[sizeof (strlen(command.c_str()) + 1)];
    strcpy(cstr, command.c_str());
    input = strtok(cstr, " \n");
    int count = 0;
    if (strcmp(input, "/requestTransaction") == 0){
      sentence = new char *[6];
      while (input != NULL){
        cout << input << endl;
        sentence[count] = new char[sizeof ((strlen(input)) + 1)];
        strcpy(sentence[count], input);
        input = strtok(NULL, " \n");
        count++;
      }
      cout << "before transaction" << endl;
      requestTransaction(sentence, &ReceiverHtable, &SenderHtable, 4, &BalList, &bitcoins);
      cout << "after transaction" << endl;
      //PrintBalanceList(&BalList);

      for (int i = 0; i < 6; i++)
        delete[] sentence[i];
      delete[] sentence;


    }
    else if (strcmp(input, "/requestTransactions") == 0){

      sentence = new char *[6];
      token = strtok(NULL, ";");
      while (token != NULL){
        cout << token << endl;
        //tempstr = new char[sizeof (strlen(token) + 1)];
        //strcpy(tempstr, token);
        //cout << "tempstr = " << tempstr << endl;
        int j = 1;
        /*
        tkn = strtok(token, " ");
        while (tkn != NULL){
          cout << "tkn = " << tkn << endl;
          //sentence[j] = new char[sizeof ((strlen(tkn)) + 1)];
          cout << "tkn after malloc = " << tkn << endl;
          //strcpy(sentence[j], tkn);
          tkn = strtok(NULL, " ");
          j++;
        }
        */
        //delete tempstr;
        //requestTransactions(sentence, &ReceiverHtable, &SenderHtable, 4, &BalList, &bitcoins);
        token = strtok(NULL, ";");
        for (int i = 0; i < 6; i++)
          delete[] sentence[i];
      }
    }
    delete[] cstr;
  }
  return 0;
}
