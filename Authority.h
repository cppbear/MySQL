#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

class Authority
{
private:
	string owner;
	string name;
	int DROP[4][4] = { 0 };
	int INSERT[4][4] = { 0 };
	int DELETE[4][4] = { 0 };
	int SELECT[4][4] = { 0 };
public:
	Authority(string username) { owner = username; };
	Authority() {};
	~Authority() {};
	string out_owner() { return owner; }
	void copy_1(int a[][4], int b);
	void copy_2(int a[][4], int b);
	void Warshall(int r[][4]);
	void auth_to_file(string name_);
	void read_auth(string name_);
	bool auth_check(int user, string name_, int auth);
	void display_auth(int user, string name_);
	void grant(string name, int author, vector<int> auth, vector<string> users);
	void clear_all(int user, int auth);
	void revoke(string name, int author, vector<int> auth, vector<string> users);
};