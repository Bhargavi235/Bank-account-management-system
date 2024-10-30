/*This program deals with a bank account management system which incorporates basic functionalities and features. I have 
attempted to implement many OOPS concepts that can be used in C++, namely classes and objects, polymorphism, inheritance etc.
 
The C++ implementations that are included in this program are classes and objects, function overloading, default
arguments, pass by value, constructors, static variable and function, friend function, operator overloading, 
dynamic memory allocation and deallocation, different types of inheritance and virtual functions. */

#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

class transaction 
{
public:
    string type;
    double tamount;
    transaction(string t, double ta) 
	{
		type=t;
		tamount=ta;
	}
};

class notification
{
	public: 
	virtual void sendnoti(string message)=0;       //virtual function
	
	~notification()
	{}
};

class sms: public notification
{
public: 
string phone;
sms(string phoneno) 
{
	phone=phoneno;
	}	
	
	void sendnoti(string message)
	{
		cout<<endl<<"Sending SMS to "<<phone<<" : "<<endl<<message<<endl;
	}
};

class bank 
{
public:
    double bal = 0;
    string name;
    vector<transaction*> tranhistory;
    static int acccount;       //static variable to keep a count of accounts created
    notification* notif;
    
    bank(string n, double bl,string phone) : name(n), bal(bl)     //parameterized constructor
	{
		                       
		                        notif=new sms(phone);
    }

bank(bank &z)    //copy constructor
	{
		name=z.name;
		bal=z.bal;
	}
	
    ~bank() 
	{
        for (size_t i = 0; i < tranhistory.size(); ++i) 
		{
            delete tranhistory[i];      //using delete operator to deallocate memory for every transaction made
            cout<<endl<<"Deleted transaction history of transaction "<<i+1<<endl;
        }
    }
    
    static void showacccount()      //static function to show number of accounts created
	{
        cout<<"Total number of accounts created: "<<acccount<<endl;
    }

    void deposit(double amount)        
	{
        if (amount > 0) 
		{
            bal += amount;
            tranhistory.push_back(new transaction("Deposit", amount));     //using new operator to dynamically add transactions
            cout<<"Deposited: rs "<<amount<< endl;
            sendnoti("Your account has been credited with rs " + converttostring(amount));
        } 
		else 
		{
            cout<<"Deposit amount must be positive"<<endl;
        }
    }
    
    void deposit(int amount)            //overloaded deposit and withdraw functions to accept both double and int inputs
	{
        if (amount > 0) 
		{
            bal += amount;
            tranhistory.push_back(new transaction("Deposit", amount));
            cout<<"Deposited: rs "<<amount<< endl;
            sendnoti("Your account has been credited with rs " + converttostring(amount));
        } 
		else 
		{
            cout << "Deposit amount must be positive" << endl;
        }
    }

    void withdraw(double amount) 
	{
        if (amount > 0 && amount <= bal) 
		{
            bal -= amount;
            tranhistory.push_back(new transaction("Withdrawal", amount));       //using new operator to dynamically add transactions
            cout <<"Withdrew: rs "<<amount<<endl;
            sendnoti("Your account has been debited with rs " + converttostring(amount));
        } 
		else if (amount > bal) 
		{
            cout<<"Insufficient funds"<<endl;
        } 
		else 
		{
            cout<<"Withdrawal amount must be positive"<< endl;
        }
    }
    
    void withdraw(int amount) 
	{
        if (amount > 0 && amount <= bal) 
		{
            bal -= amount;
            tranhistory.push_back(new transaction("Withdrawal", amount));
            cout <<"Withdrew: rs "<<amount<<endl;
            sendnoti("Your account has been debited with rs " + converttostring(amount));
        } 
		else if (amount > bal) 
		{
            cout<<"Insufficient funds"<<endl;
        } 
		else 
		{
            cout<<"Withdrawal amount must be positive"<< endl;
        }
    }

    void displayhistory() 
	{
        cout << "Transaction history for " << name << ":\n";
        for (size_t i = 0; i < tranhistory.size(); ++i) 
		{
            cout << tranhistory[i]->type << ": rs " << tranhistory[i]->tamount << endl;
        }
    }
    
    void showbalance() 
	{
        cout << "Account holder: " << name << endl;
        cout << "Current balance: rs " << bal << endl;
    }

         void accincrement()
    {
                acccount++;
	}
	
	bank& operator-=(double amount)        //operator overloading
		{
			if(amount<=bal && amount>0)
		{
			bal-=amount;
			tranhistory.push_back(new transaction("Transfer out", amount));
            cout << "Transferred out: rs " << amount << endl;
		}
		else if (amount > bal)
		 {
            cout << "Insufficient funds for transfer" << endl;
            exit(0);
        } 
		else
		 {
            cout << "Transfer amount must be positive" << endl;
            exit(0);
        }
		return *this;
		}
   
	bank& operator+=(double amount)         //operator overloading
	{
		if(amount>0)
		{
			bal+=amount;
			tranhistory.push_back(new transaction("Transfer in", amount));
            cout << "Transferred in: rs " << amount << endl;
		}
		else 
		{
            cout << "Transfer amount must be positive" << endl;
            exit(0);
        }
		return *this;
	}

	void applyloan(double princi=20000, double time=1, double interest=3.5)       //default arguments
{
	double si;
	si=(princi*interest*time)/100;
	cout<<"Here is an example of a loan taken with default values:"<<endl;
	cout<<"Loan has been approved for rs "<<princi<<" for the time period of "<<time<<" years"<<" at interest rate of "<<interest
	<<"%"<<endl<<"The interest for your loan will be rs "<<si<<endl;
	cout<<"Total amount repayable is rs "<<princi+si<<endl;
}
	
	friend void resetbalance(bank &b);    //friend function for admin functionalities
	
    private:
    void sendnoti(string message)
    {
        if (notif)
        {
            notif->sendnoti(message);
        }
    }

    string converttostring(double value)
    {
        stringstream ss;
        ss << value;
        return ss.str();
    }

};

void resetbalance(bank &b)    //friend function definition
{
	b.bal=0;
	cout<<"Balance has been reset to zero for admin purposes"<<endl;
	cout<<"Account holder: "<<b.name<<endl<<"New reset balance: "<<b.bal<<endl;
}

class savings : virtual public bank         //heirarchical inheritance as currentacc class inherits from bank class too
{
public:
	bool intapplied=false;
    savings(string n, double bl,string phone) : bank(n, bl,phone) {}          //parameterized constructor

    void applyint(double interest = 2.0) 
	{
		if(intapplied)
		{
			cout<<"Interest has already been applied for this period"<<endl<<endl;
			return;
		}
		
        cout << endl<<"Welcome to the savings account portal" << endl;
        cout << "Enter interest rate(default is 2%)"<<endl;
        cin >> interest;
        if(interest<0)
        {
        	cout<<"Interest rate must be positive"<<endl<<endl;
        	return;
		}
        bal += bal * (interest / 100);
        intapplied=true;
        cout << endl<<"Interest of " << interest << "% has been applied" << endl;
        cout << "Balance of savings account: rs " << bal << endl;
    }
     
     void resetint()
     {
     	intapplied=false;
	 }
};

class currentacc : virtual public bank               //heirarchical inheritance as savings class inherits from bank class too
{
public:

double maxod=1000.0;
    double od = 0;
    currentacc(string n, double bl,string phone) : bank(n, bl,phone) {}             //parameterized constructor

    void applyod() {
    	over: cout<<"Welcome to current account portal"<<endl<<"Overdraft limit is rs "<<maxod<<endl;
        cout << "Enter overdraft amount "<<endl;
        cin >> od;
        if(od<=0)
        {
        	cout<<"Invalid input. Overdraft amount must be positive"<<endl<<endl;
        	return;
		}
		else if(od>maxod)
		{
			cout<<"You have exceeded the overdraft limit"<<endl<<endl;
			goto over;
			return;
		}
		else
		{
			cout << endl<<"Overdraft of rs " << od << " applied" << endl;
        bal += od; 
        cout<<"New current account balance: rs "<<bal<<endl;
		}
    }
};

class loan 
{
public:
    double loanamt;
    double intrate;
loan(double la, double ir)                           //parameterized constructor
{
	loanamt=la;
	intrate=ir;
}
    loan() : loanamt(0), intrate(0) {} 

    void showloan() 
	{
        cout << endl<<"Enter loan amount"<<endl;
        cin >> loanamt;
        cout << "Enter interest rate"<<endl;
        cin >> intrate;
        cout <<endl<< "Loan has been approved for rs " << loanamt << endl;
        cout << "Default interest rate: 2 %" << endl;
        cout << "Interest amount on loan: rs " << loanamt * (2 / 100) << endl;
        cout<<"Total amount to repay: rs "<< loanamt+(loanamt*(2/100))<<endl;
    }
};

class busacc : public savings, public currentacc,public loan          //multiple and hybrid inheritance
{
public:
    busacc(string n, double bl,  double la, double ir,string phone)
    : bank(n, bl,phone), savings(n, bl,phone), currentacc(n, bl,phone), loan(la, ir)          //parameterized constructor
	 {}
	 
    void showbus() 
	{
        cout <<endl<<"Displaying account details before approving loan:"<<endl;
        showbalance();  
        showloan();     
    } 
};

int bank::acccount = 0;     //static variable initialised

int main() 
{
	double transferamt;
	static int acccount;
    int choice;
    double amount,loanamt,intrate;
    string name,phone;
    busacc b("", 0,0,0,""); 

cout<<"Welcome to bank account management system!"<<endl;
    while (true) 
	{
        cout <<endl<<"1. Create account" << endl << "2. Deposit money" << endl
<< "3. Withdraw money" << endl<< "4. Show account details" << endl<< "5. Show number of accounts created" << endl
<< "6. Show transaction history" << endl<< "7. Apply interest (Savings account)" << endl<< 
"8. Apply overdraft (Current account)" << endl << "9. Apply for a loan" << endl<< "10. View a copy of account details(Admin)" 
<< endl<<"11. Reset balance(Admin)"<<endl<< "12. Implement a default fund transfer between two hardcoded accounts"
<<endl<<"13.Exit"<<endl<<endl<<"Enter your choice"<<endl;
        cin >> choice;

bank b2(b);  //creating a second object to call the copy constructor

bank b1("Ram", 20000, "8090878909");         // example account
    bank b3("Shreya", 5600, "9988087890");       // example account
    
      switch (choice) 
		{
        case 1: 
        cout<<endl<<"Account creation portal:"<<endl;
        	cout << "Enter your name to create a bank account "<<endl;
        	cin.ignore();
                getline(cin,name);
                ini:	 cout << "Enter initial balance(minimum is rs 2000)"<<endl;
                cin >> amount;
                if(amount<2000)
                {
                	cout<<"Initial balance must be a minimum of rs 2000. Please try again"<<endl;
                	goto ini;
				}
                cout << "Enter your phone number for notifications "<<endl;
                cin >> phone;
                b = busacc(name, amount,loanamt,intrate,phone);
				b.accincrement(); 
                break;
        case 2:
        	cout<<endl<<"Deposit portal:"<<endl;
                cout << "Enter the amount to deposit"<<endl;
                cin >> amount;
                b.deposit(amount);   //pass by value
                break;
        case 3:
        	cout<<endl<<"Withdrawal portal:"<<endl;
                cout << "Enter the amount to withdraw "<<endl;
                cin >> amount;
                b.withdraw(amount);  //pass by value
                break;
        case 4:
        	cout<<endl<<"Account details:"<<endl;
                b.showbalance(); break;
        case 5:
                bank::showacccount(); break;      //calling the static function
        case 6:
        	cout<<endl;
                b.displayhistory(); break;
        case 7:
                b.applyint(); break;
        case 8:
                b.applyod(); break;
        case 9:
        	cout<<endl;
        	b.applyloan();      //not passing parameters so that the default values are used
                b.showbus(); break;
        case 10: 
        
                cout<<endl<<"Details for admin"<<endl;
                b2.showbalance();     //calls the function through the copy object
                break;
        case 11: 
		resetbalance(b);     //calling of friend function
		 break; 
		 case 12:  	double transferamt;
		 cout<<endl<<"Balance before transfer"<<endl;
		cout<<b1.name<<" : "<<b1.bal<<endl;
                cout<<b3.name<<" : "<<b3.bal<<endl;
                beg: cout << "Enter the amount to transfer from " << b1.name << " to " << b3.name <<endl;
                cin >> transferamt;
                b1 -= transferamt;     //fund transfer through operator overloading
                b3 += transferamt; 
                cout<<endl<<"Balance after transfer"<<endl;
                cout<<b1.name<<" : "<<b1.bal<<endl;
                cout<<b3.name<<" : "<<b3.bal<<endl;
                
                break;
        case 13:
                cout << "Exiting...";
                return 0;
            default:
                cout << "Invalid input. Please choose a valid option." << endl;
        }
    }
    return 0;
}

