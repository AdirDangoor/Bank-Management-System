using namespace std;
#include <iostream>

class Account {
private:
	int accountNumber_c;
	char fName_c[30];
	char lName_c[30];
	int balance_c;

public:
	static int accountsNum;

	Account();
	Account(int Id,const char* fname, char* lname,int deposit);
	~Account();

	void OpenBankAccount_c();
	void DisplayAccount_c();
	int ReturnAccNum();
	int ReturnBalance();
	void Deposit(int amount);
	void Drawal(int amount);
	void ShowAllAccount_c();
};