#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<ctime>
#include"Authority.h"
#include"Table.h"
#include"User.h"
using namespace std;

class Interface
{
private:
	vector<string> name_to_file[4];
	time_t now_time;
	vector<vector<string>> sys_log;
	void lower(string &a);
	void cat(string a);
	bool parse(string command, Table& g, User &u);
	void to_vector(vector<string> &a, string b, int begin, int end);
public:
	Interface() {};
	~Interface() {};
	void UI();
	void table_list(User& u);
	void in_n_to_f();
	void out_n_to_f();
	void in_sys_log();
	void to_log(string owner, int user, string name, time_t t, string auth);
	void out_sys_log();
	void sys_info(int owner);
};
