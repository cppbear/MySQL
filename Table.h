#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

class Table
{
private:
	//string tablename;
	vector<vector<string>> con;
	vector<string> info;
public:
	Table() {};
	~Table() {};
	void display_vector(vector<vector<string>> a, string b = {}, bool sort = false);
	void print(string file, vector<int> a = {});
	int str_in_vec(string str, vector<string> vec);
	void read_file(string file);
	void create_to_file(string file, vector<string> col);
	void create_from_file(string file, vector<string> temp[]);
	void insert(string file, vector<string> val, vector<string> temp[], vector<string> col = {});
	void delete_single(string file, string col, string val, vector<string> temp[]);
	void delete_all(string file, vector<string> temp[]);
	void select(string file, vector<string> col, bool dis = false, int order = 0, bool where = false, bool to_file = false, string or_col = {}, string where_col = {}, string where_val = {}, string to_file_name = {});
	void Out_to_file(string file);
};