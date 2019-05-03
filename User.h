#pragma once
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

class User
{
private:
	string users[4][2];
	int user_number = 0;
public:
	User(string filename) {
		ifstream fin;
		fin.open(filename);
		string a;
		int i = 0;
		while (fin.good())
		{
			fin >> a;
			users[i][0] = a;
			fin >> a;
			users[i][1] = a;
			i++;
		}
		fin.close();
	};
	~User() {};
	bool login(string a, string b);
	int user_num() { return user_number; };
};
