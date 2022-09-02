#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string.h>
#include "BankManagement-header.h"

using namespace std;
//constructor
Account::Account() {
	balance_c = 0;
}
//destructor
Account::~Account() {
}
/* constructor 
Account::Account(int Id, const char* fname, char* lname, int deposit) {

	strcpy(fName_c, fname);
	strcpy(lName_c, lname);
	balance_c = deposit;
}*/

void Account::DisplayAccount_c() {
	system("CLS");
	cout << "Account Details:" << endl;
	cout << "Account Number: " << accountNumber_c << endl;
	cout << "First Name: " << fName_c << endl;
	cout << "Last Name: " << lName_c << endl;
	cout << "Account Balance: " << balance_c << endl;
}

void Account::OpenBankAccount_c() {
	//checking valid input
	cout << "Enter Number Of Account:" << endl;
	while (true) {
		if (cin >> accountNumber_c) {
			break;
		}
		else{
			cout << "INPUT ERROR - input should be a number" << endl;
			cout << "Enter Number Of Account:" << endl;
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

		}
	}
	cout << "Enter your first name:" << endl;
	cin >> fName_c;
	cout << "Enter you last name:" << endl;
	cin >> lName_c;
	
	//checking valid input
	cout << "enter deposit:" << endl;
	while (true) {
		if (cin >> balance_c) {
			break;
		}
		else{
			cout << "INPUT ERROR - input should be a number" << endl;
			cout << "enter deposit:" << endl;
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

		}
	}
}

int Account::ReturnAccNum() {
	return accountNumber_c;
}

int Account::ReturnBalance() {
	return balance_c;
}

void Account::Deposit(int amount) {
	balance_c += amount;
}

void Account::Drawal(int amount) {
	balance_c -= amount;
}

void Account::ShowAllAccount_c() {
	cout << accountNumber_c << "\t" << fName_c << "\t" << lName_c << "\t " << balance_c << endl;
}

//functions that handle the data file
void OpenBankAccount_f();
void DisplayAccount_f(int n);
void DepositWithdrawal(int n,int option);
void CloseAnAccount(int n);
void ShowAllAccounts_f();

int main() {
	int MenuChoose = 0;
	int idNum = 0;

	do {

		//Main Screen Menu
		cout << "**Bank Management System**" << endl;
		cout << "     1 - Open an account" << endl;
		cout << "     2 - Balance Enquiry" << endl;
		cout << "     3 - Deposit" << endl;
		cout << "     4 - Withdrawal" << endl;
		cout << "     5 - Close an Account" << endl;
		cout << "     6 - Show All Accounts" << endl;
		cout << "     7 - Quit" << endl;

		//User is choosing an option
		cout << endl << "Enter the desired option:" << endl;
		cin >> MenuChoose;

		//Validation check
		while (MenuChoose < 1 || MenuChoose > 7) {
			cout << "Invalid entry! Please enter a valid value:" << endl;
			cin >> MenuChoose;
		}
		system("CLS");

		switch (MenuChoose) {

			//open account
		case(1):
			OpenBankAccount_f();
			break;

			//display account
		case(2):
			cout << "Enter Number of Account:" << endl;
			cin >> idNum;
			DisplayAccount_f(idNum);
			break;

			//deposit
		case(3):
			cout << "Enter Number of Account:" << endl;
			cin >> idNum;
			DepositWithdrawal(idNum, 1);
			break;

			//withdrawal
		case(4):
			cout << "Enter Number of Account:" << endl;
			cin >> idNum;
			DepositWithdrawal(idNum, 2);
			break;

			//delete account
		case(5):
			cout << "Enter Number of Account:" << endl;
			cin >> idNum;
			CloseAnAccount(idNum);
			break;
		case(6):
			ShowAllAccounts_f();

		}
		cout << endl;

	} while (MenuChoose != 7);

}

void OpenBankAccount_f() {
	Account newAccount;
	ofstream myFile;
	myFile.open("Bank_Account.dat", ios::binary | ios::app);
	newAccount.OpenBankAccount_c();
	myFile.write(reinterpret_cast<char*> (&newAccount), sizeof(Account));
	myFile.close();
}

void DisplayAccount_f(int n) {
	Account ac;
	bool flag = false;

	ifstream readFile;
	readFile.open("Bank_Account.dat", ios::binary );
	if (!readFile)
	{
		cout << "File could not be open";
		return;
	}
	//looping through all accounts in file
	while (readFile.read(reinterpret_cast<char*> (&ac), sizeof(Account)))
	{
		//if wanted account found
		if (ac.ReturnAccNum() == n)
		{
			ac.DisplayAccount_c();
			flag = true;
		}
	}
	readFile.close();
	
	//if account wasnt found
	if (flag == false)
		cout << "\n\n\tBank_Account number does not exist";
}

void DepositWithdrawal(int n, int option) {
	Account ac;
	bool flag = false;
	fstream ChangeFile;
	ChangeFile.open("Bank_Account.dat", ios::binary | ios::in | ios::out);

	if (!ChangeFile){
		cout << "File could not be open";
		return;
	}

	//looping through all accounts in file
	while (ChangeFile.read(reinterpret_cast<char*> (&ac), sizeof(Account))) {

		//if wanted account found
		if (ac.ReturnAccNum() == n) {

			//deposit option
			if (option == 1) {
				int amount = 0;
				cout << "Enter Deposit Amount: " << endl;
				cin >> amount;
				ac.Deposit(amount);

			}
			//withdrawal option
			if (option == 2) {
				int amount = 0;
				cout << "Enter withdrawal Amount: " << endl;
				cin >> amount;
				//if withdrawal is greater than the balance
				if (ac.ReturnBalance() < amount) {
					cout << "Insufficience Balance" << endl;
				}
				else {
					ac.Drawal(amount);
				}
			}

			//at this stage the current read point is at the end of the wanted account
			//so we need to move back sizeof account in the file
			int moveBack = static_cast<int>(sizeof(ac)) * (-1);
			ChangeFile.seekp(moveBack, ios::cur);
			ChangeFile.write(reinterpret_cast<char*> (&ac), sizeof(Account));
			cout << "\n\n\tRecord Updated";
			flag = true;
			break;
		}
	}
	
	ChangeFile.close();
	//if account wasnt found
	if (flag == false)
		cout << "\n\n\tBank_Account number does not exist";
}

void CloseAnAccount(int n) {
	Account ac;

	ifstream OldFile;
	ofstream NewFile;

	NewFile.open("TemporaryFile.dat", ios::binary | ios::in | ios::app);
	OldFile.open("Bank_Account.dat", ios::binary | ios::out);

	OldFile.seekg(0, ios::beg);

	//looping through all accounts in file
	while (OldFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {

		//writing all accounts to the newfile except the one we need to delete
		if (ac.ReturnAccNum() != n) {
			NewFile.write(reinterpret_cast<char*> (&ac), sizeof(Account));
		}

	}

	OldFile.close();
	NewFile.close();
	remove("Bank_Account.dat");
	rename("TemporaryFile.dat", "Bank_Account.dat");
	cout << "Account Deleted" << endl;
}

void ShowAllAccounts_f() {
	Account ac;
	bool flag = false;

	ifstream readFile;
	readFile.open("Bank_Account.dat", ios::binary);
	if (!readFile)
	{
		cout << "File could not be open";
		return;
	}
	//looping through all accounts in file
	
	cout << "Account\tName\t\t Balance" << endl;
	cout << "Number" << endl;
	while (readFile.read(reinterpret_cast<char*> (&ac), sizeof(Account))){
		
		
		ac.ShowAllAccount_c();
		flag = true;	
	}
	readFile.close();

	//if account wasnt found
	if (flag == false)
		cout << "\n\nNo Accounts Founded" << endl;
}