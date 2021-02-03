#include "pch.h" // precompiled header
/*
Author: Fuad Hasan
Language: C++17 + SQLite
Program Name: SQLite Database sample with C++

There I didn't use the default sqlite.c file. Rather I compiled this file into a library file.
I used Precompiled Headers for speedy compilation
You can see here a sample demonstration about how to use the SQLite Database in the C++ program in order to store some data.
Use #define DEBUG to see more information about errors

Database class is a singletone class. (Only one instance is necessary here.

*/

#define NODEBUG

int main(); //I've written the main function below. So I declared it here.

class Database //Singletone class
{
public:
	Database() //Default constuctor starts when any function uses this function for the first time.
	{
		init();
	}
	~Database() //Destructor will be called at the end of the program.
	{
		close();
	}
	void store(const char* cmd) //method to store data to the database.
	{
		rc = sqlite3_exec(db, cmd, NULL, NULL, &err); //excecution command
		if (rc != SQLITE_OK) //if executed successfully
		{
			std::cout << "Error: store(); EC: " << rc << std::endl;
		}
	}
	auto get(const char* cmd) //this method returns the username and password
	{
		rc = sqlite3_prepare(db, cmd, -1, &stmt, NULL);
		if (rc != SQLITE_OK)
		{
			std::cout << "Error: get(); -> sqlite3_prepare();" << std::endl;
		}
		sqlite3_step(stmt);
		auto rt = std::make_pair(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0) != NULL ? sqlite3_column_text(stmt, 0) : (const unsigned char*)"NO DATA"), reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1) != NULL ? sqlite3_column_text(stmt, 1) : (const unsigned char*)"NO DATA"));

		return rt;
	}
	void init() //initializes the database
	{
		sqlite3_open("database.db", &db);
		rc = sqlite3_exec(db, "create table if not exists users(username varchar(20), password varchar(20));", NULL, NULL, &err);
		if (rc != SQLITE_OK)
		{
			std::cout << "Error: init(); RC: " << rc << std::endl;
		}
	}
	void close() //closes the database
	{
		sqlite3_close(db);
	}
public:
	static Database* database; //static member of the class
private:
	Database* dtbs;
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* err;
	int rc;
};
Database* Database::database = new Database();
bool sign_up()
{
	std::string username, password;
	bool state;
	std::pair<const char*, const char*> p;
	std::cout << "Enter username: ";
	getline(std::cin, username);
	//SQL command
	std::string command = "select username, password from users where username = '" + username + "'";
#ifdef DEBUG
	std::cout << command << std::endl;
#endif //DEBUG
	p = Database::database->get(command.c_str());
	if (p.first == username)
	{
		std::cout << "Username already exists. Try another one.\n"; //checks if username already exists
#ifdef DEBUG
		std::cout << "Username-> " << p.first << " | Password-> " << p.second << std::endl;
#endif // DEBUG
		state = false;
	}
	else
	{
		std::cout << "Enter password: ";
		getline(std::cin, password);
		command = "insert into users values('" + username + "', '" + password + "');"; //data store SQL command
#ifdef DEBUG
		std::cout << command << std::endl;
#endif // DEBUG
		Database::database->store(command.c_str()); //storing data

		state = true;
	}
	return state;
}

bool login()
{
	bool state;
	std::string username, password, command;

	std::cout << "Enter username: ";
	getline(std::cin, username);
	std::cout << "Enter password: ";
	getline(std::cin, password);
	command = "select username, password from users where username = '" + username + "'";
#ifdef DEBUG
	std::cout << command << std::endl;
#endif // DEBUG
	auto p = Database::database->get(command.c_str());
	if (username == p.first && password == p.second)
	{
#ifdef DEBUG
		std::cout << "Username-> " << p.first << " | Password-> " << p.second << std::endl;
#endif // DEBUG
		state = true;
	}
	else
	{
#ifdef DEBUG
		std::cout << "Username-> " << p.first << " | Password-> " << p.second << std::endl;
#endif // DEBUG
		state = false;
	}

	return state;
}

void start()
{
	std::cout << "1. Login\n2. Sign Up" << std::endl;
	switch (char g = _getch())
	{
	case '1':
		if (login())
		{
			std::cout << "Logged in!" << std::endl;
			main();
		}
		else
		{
			std::cout << "Incorrect Username or Password!" << std::endl;
			main();
		}
		break;
	case '2':
		if (sign_up())
		{
			std::cout << "Sign Up successfull!" << std::endl;
		}
		else
		{
			std::cout << "Sign Up failed!" << std::endl;
		}
		main();
		break;
	default:
		std::cout << "Invalid Command" << std::endl;
		main();
		break;
	}
}
int main() //program starts here
{
	start(); //starting function
}