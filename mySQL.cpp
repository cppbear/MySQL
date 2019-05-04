#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<algorithm>
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

class Interface
{
private:
	vector<string> name_to_file[4] = {};
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

int main()
{
	Interface main;
	main.UI();
	return 0;
}

void Interface::lower(string &a)
{
	for (int i = 0; i < (int)a.length(); i++)
		if (a[i] >= 'A' && a[i] <= 'Z')
			a[i] = a[i] + 32;
}

void Interface::cat(string a)
{
	string file = a.substr(4, a.length() - 4);
	ifstream fin;
	fin.open(file);
	if (!fin.good())
	{
		cout << "File does not exist!\n";
		return;
	}
	else
	{
		string str;
		while (fin.good())
		{
			getline(fin, str);
			for (int i = 0; i < str.length(); i++)
				if (str[i] == '*')
					str[i] = ' ';
			cout << str << endl;
		}
	}
}

//01234567890123456789
//CREATE TABLE name (column1, column2, ···, columnT) TO file
//CREATE TABLE name FROM filename
//DROP TABLE name
//TABLE LIST
//INSERT INTO name VALUES (value1, value2, ···, valueT)
//INSERT INTO name (column1, column2, ···) VALUES (value1, value2, ···)
//DELETE FROM name WHERE column = value
//DELETE * FROM name

//SELECT column1, column2, ··· FROM name
//SELECT * FROM name

//SELECT DISTINCT column1, column2, ··· FROM name

//SELECT * FROM name ORDER BY column1, column2, ··· ASC|DESC

//SELECT column1, column2, ··· FROM name WHERE column = value
//SELECT * FROM name WHERE column = value

//SELECT column1, column2, ··· FROM name TO file
//SELECT * FROM name TO file

//GRANT <权限列表> on <表名> to <用户列表>
//REVOKE <权限列表> on <表名> from <用户列表>

bool Interface::parse(string command, Table &g, User &u)
{
	int n = command.length();
	int left_bracket = 0;
	int right_bracket = 0;
	if (command[0] == ' ' || command[n - 1] == ' ')
		return false;
	for (int i = 0; i < n; i++)
	{
		if (command[i] == ' ')
			if (command[i + 1] == ' ')
				return false;
		if (command[i] == ',')
			if (command[i + 1] != ' ')
				return false;
		if (command[i] == '(')
		{
			if (command[i + 1] == ')' || command[i] == ' ')
				return false;
			else
				left_bracket++;
		}
		if (command[i] == ')')
		{
			if (command[i - 1] == ' ' || command[i + 1] == '(')
				return false;
			else
				right_bracket++;
		}
	}
	if (left_bracket != right_bracket)
		return false;
	//创建
	if (command.find("CREATE TABLE ") == 0)
	{
		int p = command.find(" (", 13);
		int q = command.find(" FROM ", 13);
		if (p == -1 && q == -1)
			return false;
		//从文件
		else if (p == -1)
		{
			for (int i = 13; i < q; i++)
			{
				if (command[i] == ' ')
					return false;
			}
			for (int i = q + 6; i < n; i++)
				if (command[i] == ' ')
					return false;
			string name = command.substr(13, q - 13);
			string filename = command.substr(q + 6, n - q - 6);
			//cout << name << " " << filename << endl;
			string f = filename;
			lower(f);
			if (f.length() < 4)
				return false;
			if (f.substr(f.length() - 4, 4) != ".txt")
				return false;
			for (int i = 0; i < name_to_file[0].size(); i++)
				if (name == name_to_file[0][i])
				{
					cout << "Table already exist!\n";
					return false;
				}
			for (int i = 0; i < name_to_file[1].size(); i++)
				if (filename == name_to_file[1][i])
				{
					cout << "File has already been occupied!\n";
					return false;
				}
			ifstream fin;
			fin.open(filename);
			if (!fin.good())
			{
				cout << "File does not exist!\n";
				return false;
			}
			else
			{
				name_to_file[0].push_back(name);
				name_to_file[1].push_back(filename);
				g.create_from_file(filename, name_to_file);
				Authority h("user" + to_string(u.user_num()));
				h.auth_to_file(name);
				return true;
			}
		}
		//到文件
		else
		{
			int r = command.rfind(") TO ", n - 1);
			if (r == -1)
				return false;
			else
			{
				for (int i = 13; i < p; i++)
					if (command[i] == ' ')
						return false;
				for (int i = r + 5; i < n; i++)
					if (command[i] == ' ')
						return false;
				vector<string> col;
				int T = 0;
				//将column读入vector
				to_vector(col, command, p + 2, r - 1);
				string name = command.substr(13, p - 13);
				string file = command.substr(r + 5, n - r - 5);
				string f = file;
				lower(f);
				if (f.length() < 4)
					return false;
				if (f.substr(f.length() - 4, 4) != ".txt")
					return false;
				for (int i = 0; i < name_to_file[0].size(); i++)
					if (name == name_to_file[0][i])
					{
						cout << "Table already exist!\n";
						return false;
					}
				for (int i = 0; i < name_to_file[1].size(); i++)
					if (file == name_to_file[1][i])
					{
						cout << "File already exist!!\n";
						return false;
					}
				//cout << name << " " << file << endl;
				//for (int i = 0; i < (int)col.size(); i++)
					//cout << col[i] << " ";
				//cout << endl;
				name_to_file[0].push_back(name);
				name_to_file[1].push_back(file);
				name_to_file[2].push_back(to_string(col.size()));
				name_to_file[3].push_back(to_string(0));
				g.create_to_file(file, col);
				Authority h("user" + to_string(u.user_num()));
				h.auth_to_file(name);
				return true;
			}
		}
	}
	//删除表及文件
	else if (command.find("DROP TABLE ") == 0)
	{
		for (int i = 11; i < n; i++)
			if (command[i] == ' ')
				return false;
		string name = command.substr(11, n - 11);
		int j;
		string file;
		for (j = 0; j < (int)name_to_file[0].size(); j++)
			if (name_to_file[0][j] == name)
			{
				file = name_to_file[1][j];
				break;
			}
		if (j == name_to_file[0].size())
		{
			cout << "No such table!\n";
			return false;
		}
		else
		{
			//删除表格
			//检查权限
			Authority h;
			bool s = h.auth_check(u.user_num(), name, 0);
			if (s == true)
			{
				remove(file.c_str());
				name_to_file[0].erase(name_to_file[0].begin() + j);
				name_to_file[1].erase(name_to_file[1].begin() + j);
				return true;
			}
			else
			{
				cout << "Permission denied!\n";
				//报告权限
				string owner = h.out_owner();
				to_log(owner, u.user_num(), name, now_time, "DROP");
			}
		}
	}
	//展示当前用户所能访问的表集合
	else if (command == "TABLE LIST")
		table_list(u);
	//插入信息
	else if (command.find("INSERT INTO ") == 0)
	{
		int p = command.find(" VALUES ", 12);
		if (p == -1)
			return false;
		else
		{
			if (command[p - 1] != ')')
			{
				if (command[p + 8] != '(' || command[n - 1] != ')')
					return false;
				for (int i = 12; i < p; i++)
					if (command[i] == ' ')
						return false;
				string name = command.substr(12, p - 12);
				string file;
				//检查是否存在
				int count = 0;
				for (int i = 0; i < name_to_file[0].size(); i++)
					if (name == name_to_file[0][i])
					{
						count++;
						file = name_to_file[1][i];
					}
				if (count == 0)
				{
					cout << "Table does not exist!\n";
					return false;
				}
				Authority h;
				bool s = h.auth_check(u.user_num(), name, 1);
				if (s == true)
				{
					vector<string> val;
					int j = p + 9;
					int k = j;
					while (j < n - 1)
					{
						if (command[j] == ',')
						{
							val.push_back(command.substr(k, j - k));
							k = j + 2;
						}
						j++;
					}
					val.push_back(command.substr(k, n - k - 1));
					//cout << name << endl;
					//for (int i = 0; i < val.size(); i++)
						//cout << val[i] << ' ';
					//cout << endl;
					//插入数据
					g.insert(file, val, name_to_file);
				}
				else
				{
					cout << "Permission denied!\n";
					//报告权限
					string owner = h.out_owner();
					to_log(owner, u.user_num(), name, now_time, "INSERT");
				}
			}
			else
			{
				if (left_bracket != 2)
					return false;
				else if (command[p + 8] != '(' || command[n - 1] != ')')
					return false;
				int r = command.find(" (", 12);
				if (r > p)
					return false;
				else
				{
					for (int i = 12; i < r - 1; i++)
						if (command[i] == ' ')
							return false;
					string name = command.substr(12, r - 12);
					string file;
					int count = 0;
					for (int i = 0; i < name_to_file[0].size(); i++)
						if (name == name_to_file[0][i])
						{
							count++;
							file = name_to_file[1][i];
						}
					if (count == 0)
					{
						cout << "Table does not exist!\n";
						return false;
					}
					Authority h;
					bool s = h.auth_check(u.user_num(), name, 1);
					if (s == true)
					{
						vector<string> col;
						vector<string> val;
						to_vector(col, command, r + 2, p - 2);
						to_vector(val, command, p + 9, n - 2);
						if (col.size() != val.size())
							return false;
						//cout << name << endl;
						//for (int i = 0; i < col.size(); i++)
							//cout << col[i] << ' ' << val[i] << endl;
						//插入数据
						g.insert(file, val, name_to_file, col);
					}
					else
					{
						cout << "Permission denied!\n";
						//报告权限
						string owner = h.out_owner();
						to_log(owner, u.user_num(), name, now_time, "INSERT");
					}
				}
			}
		}
	}
	//删除部分
	else if (command.find("DELETE FROM ") == 0)
	{
		int p = command.find(" WHERE ");
		int q = command.find(" = ");
		if (p == -1 || q == -1 || p > q)
			return false;
		else
		{
			string name = command.substr(12, p - 12);
			string col = command.substr(p + 7, q - p - 7);
			string val = command.substr(q + 3, n - q - 3);
			string file;
			//查找是否存在
			int count = 0;
			for (int i = 0; i < name_to_file[0].size(); i++)
				if (name == name_to_file[0][i])
				{
					count++;
					file = name_to_file[1][i];
				}
			if (count == 0)
			{
				cout << "Table does not exist!\n";
				return false;
			}
			//cout << name << " " << col << ' ' << val << endl;
			//删除部分行
			Authority h;
			bool s = h.auth_check(u.user_num(), name, 2);
			if (s == true)
				g.delete_single(file, col, val, name_to_file);
			else
			{
				cout << "Permission denied!\n";
				//报告权限
				string owner = h.out_owner();
				to_log(owner, u.user_num(), name, now_time, "DELETE");
			}
		}
	}
	//删除全部
	else if (command.find("DELETE * FROM ") == 0)
	{
		string name = command.substr(14, n - 14);
		string file;
		//查找是否存在
		int count = 0;
		for (int i = 0; i < name_to_file[0].size(); i++)
			if (name == name_to_file[0][i])
			{
				count++;
				file = name_to_file[1][i];
			}
		if (count == 0)
		{
			cout << "Table does not exist!\n";
			return false;
		}
		Authority h;
		bool s = h.auth_check(u.user_num(), name, 2);
		if (s == true)
			g.delete_all(file, name_to_file);
		else
		{
			cout << "Permission denied!\n";
			//报告权限
			string owner = h.out_owner();
			to_log(owner, u.user_num(), name, now_time, "DELETE");
		}
	}
	//选择
	else if (command.find("SELECT ") == 0)
	{
		int p = command.find(" FROM ", 8);
		int q = command.find(" DISTINCT ", 6);
		int r = command.find(" ORDER BY ", 14);
		int s = command.find(" WHERE ");
		int t = command.find(" TO ");
		if (p != -1)
		{
			vector<string> col;
			int j;
			if (q != -1)
				j = q + 10;
			else
				j = 7;
			to_vector(col, command, j, p - 1);
			if (q == -1)
			{
				string name = command.substr(p + 6, n - p - 6);
				//检查是否存在
				//实际函数
				string file;
				int t;
				for (t = 0; t < name_to_file[0].size(); t++)
					if (name == name_to_file[0][t])
					{
						file = name_to_file[1][t];
						break;
					}
				if (t == name_to_file[0].size())
				{
					cout << "Table does not exist!\n";
					return false;
				}
				Authority h;
				bool s = h.auth_check(u.user_num(), name, 3);
				if (s == true)
				{
					if (q == -1)
					{
						g.select(file, col);
						return true;
					}
				}
				else
				{
					cout << "Permission denied!\n";
					//报告权限
					string owner = h.out_owner();
					to_log(owner, u.user_num(), name, now_time, "SELECT");
				}
			}
			else if (q != -1 && q < p)
			{
				string name = command.substr(p + 6, n - p - 6);
				//检查是否存在
				//实际函数
				string file;
				int t;
				for (t = 0; t < name_to_file[0].size(); t++)
					if (name == name_to_file[0][t])
					{
						file = name_to_file[1][t];
						break;
					}
				if (t == name_to_file[0].size())
				{
					cout << "Table does not exist!\n";
					return false;
				}
				Authority h;
				bool s = h.auth_check(u.user_num(), name, 3);
				if (s == true)
				{
					g.select(file, col, true);
					return true;
				}
				else
				{
					cout << "Permission denied!\n";
					//报告权限
					string owner = h.out_owner();
					to_log(owner, u.user_num(), name, now_time, "SELECT");
				}
			}
			else if (r != -1 && r > p)
			{
				string name = command.substr(p + 6, r - p - 6);
				string order = command.substr(n - 4, 4);
				if (order == " ASC")
				{
					string or_col;
					or_col = command.substr(r + 10, n - r - 14);
					//检查是否存在
					//实际函数
					string file;
					int t;
					for (t = 0; t < name_to_file[0].size(); t++)
						if (name == name_to_file[0][t])
						{
							file = name_to_file[1][t];
							break;
						}
					if (t == name_to_file[0].size())
					{
						cout << "Table does not exist!\n";
						return false;
					}
					Authority h;
					bool s = h.auth_check(u.user_num(), name, 3);
					if (s == true)
						g.select(file, col, false, 1, false, false, or_col);
					else
					{
						cout << "Permission denied!\n";
						//报告权限
						string owner = h.out_owner();
						to_log(owner, u.user_num(), name, now_time, "SELECT");
					}
				}
				else if (order == "DESC")
				{
					string or_col;
					or_col = command.substr(r + 10, n - r - 15);
					//检查是否存在
					//实际函数
					string file;
					int t;
					for (t = 0; t < name_to_file[0].size(); t++)
						if (name == name_to_file[0][t])
						{
							file = name_to_file[1][t];
							break;
						}
					if (t == name_to_file[0].size())
					{
						cout << "Table does not exist!\n";
						return false;
					}
					Authority h;
					bool s = h.auth_check(u.user_num(), name, 3);
					if (s == true)
						g.select(file, col, false, 2, false, false, or_col);
					else
					{
						cout << "Permission denied!\n";
						//报告权限
						string owner = h.out_owner();
						to_log(owner, u.user_num(), name, now_time, "SELECT");
					}
				}
				else
					return false;
			}
			else if (s != -1 && s > p)
			{
				int uw = command.find(" = ", s + 7);
				if (uw == -1)
					return false;
				else
				{
					string name = command.substr(p + 6, s - p - 6);
					string column = command.substr(s + 7, uw - s - 7);
					string val = command.substr(uw + 3, n - uw - 3);
					//检查是否存在
					//实际函数
					string file;
					int t;
					for (t = 0; t < name_to_file[0].size(); t++)
						if (name == name_to_file[0][t])
						{
							file = name_to_file[1][t];
							break;
						}
					if (t == name_to_file[0].size())
					{
						cout << "Table does not exist!\n";
						return false;
					}
					Authority h;
					bool s = h.auth_check(u.user_num(), name, 3);
					if (s == true)
						g.select(file, col, false, 0, true, false, {}, column, val);
					else
					{
						cout << "Permission denied!\n";
						//报告权限
						string owner = h.out_owner();
						to_log(owner, u.user_num(), name, now_time, "SELECT");
					}
				}
			}
			else if (t != -1 && t > p)
			{
				string name = command.substr(p + 6, t - p - 6);
				string to_file_name = command.substr(t + 4, n - t - 4);
				//检查是否存在
				//实际函数
				string file;
				int t;
				for (t = 0; t < name_to_file[0].size(); t++)
					if (name == name_to_file[0][t])
					{
						file = name_to_file[1][t];
						break;
					}
				if (t == name_to_file[0].size())
				{
					cout << "Table does not exist!\n";
					return false;
				}
				Authority h;
				bool s = h.auth_check(u.user_num(), name, 3);
				if (s == true)
					g.select(file, col, false, 0, false, true, {}, {}, {}, to_file_name);
				else
				{
					cout << "Permission denied!\n";
					//报告权限
					string owner = h.out_owner();
					to_log(owner, u.user_num(), name, now_time, "SELECT");
				}
			}
			else
				return false;
		}
		else
			return false;
	}
	//授权
	else if (command.find("GRANT ") == 0)
	{
		int p = command.find(" on ");
		int q = command.find(" to ");
		if (p == -1 || q == -1)
			return false;
		else
		{
			vector<string> auth;
			string name;
			vector<string> user;
			to_vector(auth, command, 6, p - 1);
			name = command.substr(p + 4, q - p - 4);
			to_vector(user, command, q + 4, n - 1);
			//检查权限
			//授权
			Authority h;
			vector<int> auth_num;
			for (int i = 0; i < auth.size(); i++)
			{
				int t;
				if (auth[i] == "DROP")
					t = 0;
				else if (auth[i] == "INSERT")
					t = 1;
				else if (auth[i] == "DELETE")
					t = 2;
				else if (auth[i] == "SELECT")
					t = 3;
				else
				{
					cout << "No such permission!\n";
					return false;
				}
				if (!h.auth_check(u.user_num(), name, t))
				{
					cout << "Permission denied!\n";
					return false;
				}
				auth_num.push_back(t);
			}
			h.grant(name, u.user_num(), auth_num, user);
		}
	}
	//取消授权
	else if (command.find("REVOKE ") == 0)
	{
		int p = command.find(" on ");
		int q = command.find(" from ");
		if (p == -1 || q == -1)
			return false;
		else
		{
			vector<string> auth;
			string name;
			vector<string> user;
			to_vector(auth, command, 7, p - 1);
			name = command.substr(p + 4, q - p - 4);
			to_vector(user, command, q + 6, n - 1);
			//检查权限
			//取消授权
			Authority h;
			vector<int> auth_num;
			if (auth[0] == "all")
				auth_num.push_back(-1);
			else
			{
				for (int i = 0; i < auth.size(); i++)
				{
					int t;
					if (auth[i] == "DROP")
						t = 0;
					else if (auth[i] == "INSERT")
						t = 1;
					else if (auth[i] == "DELETE")
						t = 2;
					else if (auth[i] == "SELECT")
						t = 3;
					else
					{
						cout << "No such permission!\n";
						return false;
					}
					auth_num.push_back(t);
				}
			}
			h.revoke(name, u.user_num(), auth_num, user);
		}
	}
	else
		return false;
}

void Interface::to_vector(vector<string> &a, string b, int begin, int end)
{
	int j = begin;
	int k = j;
	while (j < end + 1)
	{
		if (b[j] == ',')
		{
			a.push_back(b.substr(k, j - k));
			k = j + 2;
		}
		j++;
	}
	a.push_back(b.substr(k, end + 1 - k));
}

void Interface::UI()
{
	in_n_to_f();
	while (true)
	{
		User user("Users.txt");
		cout << "~$ ";
		string a;
		getline(cin, a);
		bool flag = false;
		while (a == "mySQL")
		{
			//登录
			while (!flag)
			{
				cout << "(mysql)==> login: ";
				string username;
				cin >> username;
				cout << "(mysql)==> password: ";
				string password;
				cin >> password;
				flag = user.login(username, password);
				if (flag == true)
				{
					in_sys_log();
					sys_info(user.user_num());
				}
				cin.get();
			}
			cout << "(mysql)==> ";
			string command;
			getline(cin, command);
			now_time = time(NULL);
			if (command == "quit")
			{
				out_sys_log();
				a = "";
				break;
			}
			else if (command == "")
				continue;
			else
			{
				Table g;
				bool flag = parse(command, g, user);
				if (flag == false)
					cout << "Invalid command!\n";
			}
			//else
				//cout << "Invalid command!\n";
		}
		if (a == "")
			continue;
		else if (a == "quit")
		{
			out_n_to_f();
			return;
		}
		else if (a.substr(0, 4) == "cat ")
			cat(a);
		else
			cout << "Invalid command!\n";
	}
}

void Interface::table_list(User& u)
{
	int n = name_to_file[0].size();
	cout << "\ttotal:" << n << endl;
	for (int m = 0; m < n; m++)
	{
		string file = name_to_file[1][m];
		ifstream fin;
		fin.open(file);
		string inf;
		getline(fin, inf);
		vector<string> information;
		int i = 0;
		int j = i;
		while (i < inf.size())
		{
			if (inf[i] == ' ' || inf[i] == '\t')
				i++;
			else
			{
				j = i;
				while (inf[j] != ' ' && inf[j] != '\t' && j < inf.size())
					j++;
				information.push_back(inf.substr(i, j - i));
				i = j + 1;
			}
		}
		fin.close();
		cout << "\t\t" << name_to_file[0][m] << ": (" << name_to_file[2][m] << "," << name_to_file[3][m] << ") [";
		for (int t = 1; t < information.size() - 1; t++)
			cout << information[t] << ", ";
		cout << information[information.size() - 1] << "] ";
		//打印权限
		Authority h;
		h.display_auth(u.user_num(), name_to_file[0][m]);
		cout << endl;
	}
}

void Interface::in_n_to_f()
{
	ifstream fin;
	fin.open("name_to_file.txt");
	if (!fin.good())
		return;
	else
	{
		while (fin.good())
		{
			string temp[4];
			fin >> temp[0] >> temp[1] >> temp[2] >> temp[3];
			for (int i = 0; i < 4; i++)
				name_to_file[i].push_back(temp[i]);
		}
	}
	fin.close();
}

void Interface::out_n_to_f()
{
	ofstream fout;
	fout.open("name_to_file.txt");
	int n = name_to_file[0].size();
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (name_to_file[j][i].length() < 8)
				fout << name_to_file[j][i] << "\t\t";
			else
				fout << name_to_file[j][i] << "\t";
		}
		fout << name_to_file[3][i] << endl;
	}
	for (int j = 0; j < 3; j++)
	{
		if (name_to_file[j][n - 1].length() < 8)
			fout << name_to_file[j][n - 1] << "\t\t";
		else
			fout << name_to_file[j][n - 1] << "\t";
	}
	fout << name_to_file[3][n - 1];
	fout.close();
}

void Interface::in_sys_log()
{
	sys_log.clear();
	ifstream fin;
	fin.open("sys_log.txt");
	while (fin.good())
	{
		vector<string> log;
		for (int i = 0; i < 8; i++)
		{
			string a;
			fin >> a;
			log.push_back(a);
		}
		sys_log.push_back(log);
	}
}

void Interface::to_log(string owner, int user, string name, time_t t, string auth)
{
	struct tm* pt;
	pt = localtime(&t);
	vector<string> log;
	log.push_back(owner);
	log.push_back("user" + to_string(user));
	log.push_back(auth);
	log.push_back(name);
	log.push_back(to_string(pt->tm_hour));
	log.push_back(to_string(pt->tm_min));
	log.push_back(to_string(pt->tm_mon + 1));
	log.push_back(to_string(pt->tm_mday));
	sys_log.push_back(log);
}

void Interface::out_sys_log()
{
	ofstream fout;
	fout.open("sys_log.txt");
	if (sys_log.size() > 0)
	{
		for (int i = 0; i < sys_log.size() - 1; i++)
		{
			for (int j = 0; j < sys_log[i].size() - 1; j++)
				fout << sys_log[i][j] << "\t";
			fout << sys_log[i][sys_log[i].size() - 1] << endl;
		}
		for (int j = 0; j < sys_log[sys_log.size() - 1].size() - 1; j++)
			fout << sys_log[sys_log.size() - 1][j] << "\t";
		fout << sys_log[sys_log.size() - 1][sys_log[sys_log.size() - 1].size() - 1];
	}
	fout.close();
}

void Interface::sys_info(int owner)
{
	for (int i = 0; i < sys_log.size(); i++)
	{
		if (sys_log[i][0] == "user" + to_string(owner))
		{
			cout << "(mysql)==> [System infomation] " << sys_log[i][1] << " requested " << sys_log[i][2] << " TABLE permission on table " << sys_log[i][3] << " at ";
			cout << sys_log[i][4] << ":" << sys_log[i][5] << " ";
			switch (stoi(sys_log[i][6]))
			{
			case 1:cout << "January"; break;
			case 2:cout << "February"; break;
			case 3:cout << "March"; break;
			case 4:cout << "April"; break;
			case 5:cout << "May"; break;
			case 6:cout << "June"; break;
			case 7:cout << "July"; break;
			case 8:cout << "August"; break;
			case 9:cout << "September"; break;
			case 10:cout << "October"; break;
			case 11:cout << "November"; break;
			case 12:cout << "December"; break;
			}
			cout << " " << sys_log[i][7] << ", grant permission? (Y/N)\n";
			cout << "(mysql)==> ";
			string a;
			cin >> a;
			if (a == "Y" || a == "y")
			{
				int p;
				if (sys_log[i][2] == "DROP")
					p = 0;
				else if (sys_log[i][2] == "INSERT")
					p = 1;
				else if (sys_log[i][2] == "DELETE")
					p = 2;
				else if (sys_log[i][2] == "SELECT")
					p = 3;
				vector<int> q;
				q.push_back(p);
				vector<string> s;
				s.push_back(sys_log[i][1]);
				Authority r;
				r.grant(sys_log[i][3], owner, q, s);
				cout << sys_log[i][2] << " TABLE permission on table " << sys_log[i][3] << " granted to " << sys_log[i][1] << " successfully\n";
			}
			sys_log.erase(sys_log.begin() + i);
			i--;
		}
	}
}

bool com_asc(vector<string> a, vector<string> b)
{
	return a[0] < b[0];
}

bool com_desc(vector<string> a, vector<string> b)
{
	return a[0] > b[0];
}

void Table::display_vector(vector<vector<string>> a, string b, bool sort)
{
	int n = a.size();
	int m;
	if (n > 0)
		m = a[0].size();
	else
	{
		cout << endl;
		return;
	}
	if (sort == false)
	{
		if (b.length() == 0)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < m - 1; j++)
				{
					if (a[i][j].length() < 8)
						cout << a[i][j] << "\t\t";
					else
						cout << a[i][j] << "\t";
				}
				cout << a[i][m - 1] << endl;
			}
		}
		else
		{
			for (int i = 0; i < n; i++)
			{
				if (a[i][m - 1] == b)
				{
					for (int j = 0; j < m - 2; j++)
					{
						if (a[i][j].length() < 8)
							cout << a[i][j] << "\t\t";
						else
							cout << a[i][j] << "\t";
					}
					cout << a[i][m - 2] << endl;
				}
			}
		}
	}
	else
	{
		if (b.length() == 0)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 1; j < m - 1; j++)
				{
					if (a[i][j].length() < 8)
						cout << a[i][j] << "\t\t";
					else
						cout << a[i][j] << "\t";
				}
				cout << a[i][m - 1] << endl;
			}
		}
		else
		{
			for (int i = 0; i < n; i++)
			{
				if (a[i][m - 1] == b)
				{
					for (int j = 1; j < m - 2; j++)
					{
						if (a[i][j].length() < 8)
							cout << a[i][j] << "\t\t";
						else
							cout << a[i][j] << "\t";
					}
					cout << a[i][m - 2] << endl;
				}
			}
		}
	}
}

void Table::print(string file, vector<int> a)
{
	ifstream fin;
	fin.open(file);
	if (a.size() == 0)
	{
		string str;
		while (fin.good())
		{
			getline(fin, str);
			for (int i = 0; i < str.length(); i++)
				if (str[i] == '*')
					str[i] = ' ';
			cout << str << endl;
		}
	}
	else
	{
		int n = a.size();
		for (int i = 0; i < n - 1; i++)
		{
			if (info[a[i]].length() < 8)
				cout << info[a[i]] << "\t\t";
			else
				cout << info[a[i]] << "\t";
		}
		cout << info[a[n - 1]] << endl;
		for (int j = 0; j < con.size(); j++)
		{
			for (int i = 0; i < n - 1; i++)
			{
				if (con[j][a[i]].length() < 8)
					cout << con[j][a[i]] << "\t\t";
				else
					cout << con[j][a[i]] << "\t";
			}
			cout << con[j][a[n - 1]] << endl;
		}
	}
}

int Table::str_in_vec(string str, vector<string> vec)
{
	int i = 0;
	for (i = 0; i < vec.size(); i++)
		if (str == vec[i])
			return i;
	if (i == vec.size())
		return -1;
}

void Table::read_file(string file)
{
	con.clear();
	ifstream fin;
	fin.open(file);
	string inf;
	getline(fin, inf);
	vector<string> information;
	int i = 0;
	int j = i;
	while (i < inf.size())
	{
		if (inf[i] == ' ' || inf[i] == '\t')
			i++;
		else
		{
			j = i;
			while (inf[j] != ' ' && inf[j] != '\t' && j < inf.size())
				j++;
			information.push_back(inf.substr(i, j - i));
			i = j + 1;
		}
	}
	info = information;
	int n = information.size();
	while (fin.good())
	{
		string a;
		vector<string> val;
		for (int j = 0; j < n; j++)
		{
			fin >> a;
			val.push_back(a);
		}
		con.push_back(val);
	}
	fin.close();
}

void Table::create_to_file(string file, vector<string> col)
{
	ofstream fout;
	fout.open(file);
	fout << "ID\t";
	for (int i = 0; i < (int)col.size() - 1; i++)
		fout << col[i] << "\t\t";
	fout << col[col.size() - 1];
	fout.close();
}

void Table::create_from_file(string file, vector<string> temp[])
{
	read_file(file);
	temp[2].push_back(to_string(info.size() - 1));
	temp[3].push_back(to_string(con.size()));
}

void Table::insert(string file, vector<string> val, vector<string> temp[], vector<string> col)
{
	read_file(file);
	if (col.size() == 0)
	{
		if (info.size() != val.size() + 1)
		{
			cout << "Numbers do not match!\n";
			return;
		}
		else
		{
			vector<string> add;
			add.push_back(to_string(con.size() + 1));
			for (int i = 0; i < val.size(); i++)
				add.push_back(val[i]);
			con.push_back(add);
			int t = 0;
			for (t = 0; t < temp[1].size(); t++)
				if (temp[1][t] == file)
					break;
			temp[3][t] = to_string(stoi(temp[3][t]) + 1);
			Out_to_file(file);
			return;
		}
	}
	else
	{
		for (int i = 0; i < col.size(); i++)
			if (str_in_vec(col[i], info) == -1)
			{
				cout << "At least one column does not exist!\n";
				return;
			}
		vector<string> add;
		add.push_back(to_string(con.size() + 1));
		for (int i = 1; i < info.size(); i++)
		{
			int t = str_in_vec(info[i], col);
			if (t == -1)
				add.push_back("*");
			else
				add.push_back(val[t]);
		}
		con.push_back(add);
		int t = 0;
		for (t = 0; t < temp[1].size(); t++)
			if (temp[1][t] == file)
				break;
		temp[3][t] = to_string(stoi(temp[3][t]) + 1);
		Out_to_file(file);
		return;
	}
}

void Table::delete_single(string file, string col, string val, vector<string> temp[])
{
	read_file(file);
	int n = info.size();
	int k;
	for (k = 0; k < n; k++)
	{
		if (info[k] == col)
			break;
	}
	if (k == n)
	{
		cout << "Column does not exist!\n";
		return;
	}
	else
	{
		int t = 0;
		for (t = 0; t < temp[1].size(); t++)
			if (temp[1][t] == file)
				break;
		for (int i = 0; i < con.size(); i++)
		{
			if (con[i][k] == val)
			{
				con.erase(con.begin() + i);
				temp[3][t] = to_string(stoi(temp[3][t]) - 1);
				i--;
			}
		}
		for (int i = 0; i < con.size(); i++)
			con[i][0] = to_string(i + 1);
		Out_to_file(file);
		return;
	}
}

void Table::delete_all(string file, vector<string> temp[])
{
	ifstream fin;
	fin.open(file);
	string inf;
	getline(fin, inf);
	vector<string> information;
	int i = 0;
	int j = i;
	while (i < inf.size())
	{
		if (inf[i] == ' ' || inf[i] == '\t')
			i++;
		else
		{
			j = i;
			while (inf[j] != ' ' && inf[j] != '\t' && j < inf.size())
				j++;
			information.push_back(inf.substr(i, j - i));
			i = j + 1;
		}
	}
	info = information;
	fin.close();
	ofstream fout;
	fout.open(file);
	int n = info.size();
	fout << "ID\t";
	for (int i = 1; i < n - 1; i++)
		fout << info[i] << "\t\t";
	fout << info[n - 1];
	fout.close();
	int t = 0;
	for (int t = 0; t < temp[1].size(); t++)
		if (temp[1][t] == file)
			break;
	temp[3][t] = to_string(0);
}

void Table::select(string file, vector<string> col, bool dis, int order, bool where, bool to_file, string or_col, string where_col, string where_val, string to_file_name)
{
	read_file(file);
	if (dis == false && order == 0 && where == false && to_file == false)
	{
		if (col[0] == "*")
			print(file);
		else
		{
			vector<int> a;
			for (int i = 0; i < col.size(); i++)
			{
				int p = str_in_vec(col[i], info);
				if (p == -1)
				{
					cout << "At least one column does not exist!\n";
					return;
				}
				else
					a.push_back(p);
			}
			print(file, a);
		}
	}
	else if (dis == true)
	{
		vector<int> a;
		for (int i = 0; i < col.size(); i++)
		{
			int p = str_in_vec(col[i], info);
			if (p == -1)
			{
				cout << "At least one column does not exist!\n";
				return;
			}
			else
				a.push_back(p);
		}
		cout << "If you choose more than one column,\nthe data in the same line in the table will NOT be displayed\nonly if all of them are the same with the previous ones.\n\n";
		int n = a.size();
		for (int i = 0; i < n - 1; i++)
		{
			if (info[a[i]].length() < 8)
				cout << info[a[i]] << "\t\t";
			else
				cout << info[a[i]] << "\t";
		}
		cout << info[a[n - 1]] << endl;
		vector<vector<string>> DIS;
		for (int i = 0; i < con.size(); i++)
		{
			vector<string> temp;
			for (int j = 0; j < a.size(); j++)
				temp.push_back(con[i][a[j]]);
			int t = 0;
			for (t = 0; t < DIS.size(); t++)
				if (temp == DIS[t])
					break;
			if (t == DIS.size())
				DIS.push_back(temp);
		}
		int m = DIS[0].size();
		for (int i = 0; i < DIS.size(); i++)
		{
			for (int j = 0; j < m - 1; j++)
			{
				if (DIS[i][j].length() < 8)
					cout << DIS[i][j] << "\t\t";
				else
					cout << DIS[i][j] << "\t";
			}
			cout << DIS[i][m - 1] << endl;
		}
	}
	else if (order == 1)
	{
		int p = str_in_vec(or_col, info);
		if (p == -1)
		{
			cout << "Column does not exist!\n";
			return;
		}
		else
		{
			vector<vector<string>> copy;
			copy = con;
			for (int i = 0; i < copy.size(); i++)
			{
				copy[i].push_back(copy[i][copy[i].size() - 1]);
				for (int j = copy[i].size() - 2; j > 0; j--)
					copy[i][j] = copy[i][j - 1];
				copy[i][0] = copy[i][p + 1];
			}
			sort(copy.begin(), copy.end(), com_asc);
			for (int i = 0; i < info.size() - 1; i++)
			{
				if (info[i].length() < 8)
					cout << info[i] << "\t\t";
				else
					cout << info[i] << "\t";
			}
			cout << info[info.size() - 1] << endl;
			display_vector(copy, {}, true);
		}
	}
	else if (order == 2)
	{
		int p = str_in_vec(or_col, info);
		if (p == -1)
		{
			cout << "Column does not exist!\n";
			return;
		}
		else
		{
			vector<vector<string>> copy;
			copy = con;
			for (int i = 0; i < copy.size(); i++)
			{
				copy[i].push_back(copy[i][copy[i].size() - 1]);
				for (int j = copy[i].size() - 2; j > 0; j--)
					copy[i][j] = copy[i][j - 1];
				copy[i][0] = copy[i][p + 1];
			}
			sort(copy.begin(), copy.end(), com_desc);
			for (int i = 0; i < info.size() - 1; i++)
			{
				if (info[i].length() < 8)
					cout << info[i] << "\t\t";
				else
					cout << info[i] << "\t";
			}
			cout << info[info.size() - 1] << endl;
			display_vector(copy, {}, true);
		}
	}
	else if (where == true)
	{
		int p = str_in_vec(where_col, info);
		if (p == -1)
		{
			cout << "Column does not exist!\n";
			return;
		}
		else
		{
			if (col[0] == "*")
			{
				for (int i = 0; i < info.size() - 1; i++)
				{
					if (info[i].length() < 8)
						cout << info[i] << "\t\t";
					else
						cout << info[i] << "\t";
				}
				cout << info[info.size() - 1] << endl;
				vector<vector<string>> copy;
				copy = con;
				for (int i = 0; i < con.size(); i++)
					copy[i].push_back(copy[i][p]);
				display_vector(copy, where_val);
			}
			else
			{
				vector<int> a;
				for (int i = 0; i < col.size(); i++)
				{
					int q = str_in_vec(col[i], info);
					if (q == -1)
					{
						cout << "At least one column does not exist!\n";
						return;
					}
					else
						a.push_back(q);
				}
				a.push_back(p);
				int n = a.size();
				vector<vector<string>> copy;
				for (int i = 0; i < con.size(); i++)
				{
					vector<string> temp;
					for (int j = 0; j < n; j++)
						temp.push_back(con[i][a[j]]);
					copy.push_back(temp);
				}
				for (int i = 0; i < col.size() - 1; i++)
				{
					if (col[i].length() < 8)
						cout << col[i] << "\t\t";
					else
						cout << col[i] << "\t";
				}
				cout << col[col.size() - 1] << endl;
				display_vector(copy, where_val);
			}
		}
	}
	else if (to_file == true)
	{
		if (col[0] == "*")
			Out_to_file(to_file_name);
		else
		{
			vector<int> a;
			for (int i = 0; i < col.size(); i++)
			{
				int p = str_in_vec(col[i], info);
				if (p == -1)
				{
					cout << "At least one column does not exist!\n";
					return;
				}
				else
					a.push_back(p);
			}
			ofstream fout;
			fout.open(to_file_name);
			int n = a.size();
			fout << "ID\t";
			for (int i = 0; i < n - 1; i++)
				fout << info[a[i]] << "\t\t";
			fout << info[a[n - 1]] << endl;
			for (int i = 0; i < con.size() - 1; i++)
			{
				fout << con[i][0] << "\t";
				for (int j = 0; j < n - 1; j++)
				{
					if (con[i][a[j]].length() < 8)
						fout << con[i][a[j]] << "\t\t";
					else
						fout << con[i][a[j]] << "\t";
				}
				fout << con[i][a[n - 1]] << endl;
			}
			int t = con.size() - 1;
			fout << con[t][0] << "\t";
			for (int j = 0; j < n - 1; j++)
			{
				if (con[t][a[j]].length() < 8)
					fout << con[t][a[j]] << "\t\t";
				else
					fout << con[t][a[j]] << "\t";
			}
			fout << con[t][a[n - 1]];
			fout.close();
		}
	}
}

void Table::Out_to_file(string file)
{
	ofstream fout;
	fout.open(file);
	int n = info.size();
	fout << "ID\t";
	for (int i = 1; i < n - 1; i++)
		fout << info[i] << "\t\t";
	fout << info[n - 1] << endl;
	for (int i = 0; i < con.size() - 1; i++)
	{
		fout << con[i][0] << "\t";
		for (int j = 1; j < n - 1; j++)
		{
			if (con[i][j].length() < 8)
				fout << con[i][j] << "\t\t";
			else
				fout << con[i][j] << "\t";
		}
		fout << con[i][n - 1] << endl;
	}
	int t = con.size() - 1;
	fout << con[t][0] << "\t";
	for (int j = 1; j < n - 1; j++)
	{
		if (con[t][j].length() < 8)
			fout << con[t][j] << "\t\t";
		else
			fout << con[t][j] << "\t";
	}
	fout << con[t][n - 1];
	fout.close();
}

void Authority::copy_1(int a[][4], int b)
{
	if (b == 0)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a[i][j] = DROP[i][j];
	else if (b == 1)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a[i][j] = INSERT[i][j];
	else if (b == 2)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a[i][j] = DELETE[i][j];
	else if (b == 3)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a[i][j] = SELECT[i][j];
}

void Authority::copy_2(int a[][4], int b)
{
	if (b == 0)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				DROP[i][j] = a[i][j];
	else if (b == 1)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				INSERT[i][j] = a[i][j];
	else if (b == 2)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				DELETE[i][j] = a[i][j];
	else if (b == 3)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				SELECT[i][j] = a[i][j];
}

void Authority::Warshall(int r[][4])
{
	int i, j, k;
	int temp[4][4];
	for (k = 0; k < 4; k++)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				temp[i][j] = (r[i][j]) || (r[i][k] & r[k][j]);
			}
		}
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				r[i][j] = temp[i][j];
	}
}

void Authority::auth_to_file(string name_)
{
	ofstream fout;
	string filename = "auth_on_" + name_ + ".txt";
	fout.open(filename);
	fout << owner << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			fout << DROP[i][j] << ' ';
		fout << endl;
	}
	fout << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			fout << INSERT[i][j] << ' ';
		fout << endl;
	}
	fout << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			fout << DELETE[i][j] << ' ';
		fout << endl;
	}
	fout << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			fout << SELECT[i][j] << ' ';
		fout << endl;
	}
	fout.close();
}

void Authority::read_auth(string name_)
{
	name = name_;
	ifstream fin;
	string filename = "auth_on_" + name_ + ".txt";
	fin.open(filename);
	fin >> owner;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			fin >> DROP[i][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			fin >> INSERT[i][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			fin >> DELETE[i][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			fin >> SELECT[i][j];
	fin.close();
}

bool Authority::auth_check(int user, string name_, int auth)
{
	read_auth(name_);
	if ("user" + to_string(user) == owner)
		return true;
	else if (auth == 0)
	{
		bool temp = false;
		for (int i = 0; i < 4; i++)
			temp = temp || DROP[i][user - 1];
		return temp;
	}
	else if (auth == 1)
	{
		bool temp = false;
		for (int i = 0; i < 4; i++)
			temp = temp || INSERT[i][user - 1];
		return temp;
	}
	else if (auth == 2)
	{
		bool temp = false;
		for (int i = 0; i < 4; i++)
			temp = temp || DELETE[i][user - 1];
		return temp;
	}
	else if (auth == 3)
	{
		bool temp = false;
		for (int i = 0; i < 4; i++)
			temp = temp || SELECT[i][user - 1];
		return temp;
	}
}

void Authority::display_auth(int user, string name_)
{
	read_auth(name_);
	vector<string> auth;
	if ("user" + to_string(user) == owner)
		cout << "DROP, INSERT, DELETE, SELECT [OWNER]";
	else
	{
		bool temp = false;
		for (int i = 0; i < 4; i++)
			temp = temp || DROP[i][user - 1];
		if (temp == true)
		{
			auth.push_back("DROP");
			temp = false;
		}
		for (int i = 0; i < 4; i++)
			temp = temp || INSERT[i][user - 1];
		if (temp == true)
		{
			auth.push_back("INSERT");
			temp = false;
		}
		for (int i = 0; i < 4; i++)
			temp = temp || DELETE[i][user - 1];
		if (temp == true)
		{
			auth.push_back("DELETE");
			temp = false;
		}
		for (int i = 0; i < 4; i++)
			temp = temp || SELECT[i][user - 1];
		if (temp == true)
		{
			auth.push_back("SELECT");
			temp = false;
		}
		if (auth.size() == 0)
		{
			cout << "No permission";
			return;
		}
		else
		{
			for (int i = 0; i < auth.size() - 1; i++)
				cout << auth[i] << ", ";
			cout << auth[auth.size() - 1];
		}
	}
}

void Authority::grant(string name, int author, vector<int> auth, vector<string> users)
{
	read_auth(name);
	if (users[0] == "public")
	{
		for (int i = 0; i < auth.size(); i++)
		{
			int temp[4][4];
			copy_1(temp, auth[i]);
			for (int j = 0; j < 4; j++)
				temp[author - 1][j] = 1;
			Warshall(temp);
			if (temp[0][0] == 1 || temp[1][1] == 1 || temp[2][2] == 1 || temp[3][3] == 1)
			{
				cout << "Grant failed!\n";
				return;
			}
			else
			{
				copy_1(temp, auth[i]);
				for (int j = 0; j < 4; j++)
					temp[author - 1][j] = 1;
				copy_2(temp, auth[i]);
			}
		}
		auth_to_file(name);
	}
	else
	{
		vector<int> user_num;
		for (int i = 0; i < users.size(); i++)
			user_num.push_back(users[i][4]-48);
		for (int i = 0; i < auth.size(); i++)
		{
			int temp[4][4];
			copy_1(temp, auth[i]);
			for (int j = 0; j < user_num.size(); j++)
				temp[author - 1][user_num[j] - 1] = 1;
			Warshall(temp);
			if (temp[0][0] == 1 || temp[1][1] == 1 || temp[2][2] == 1 || temp[3][3] == 1)
			{
				cout << "Grant failed!\n";
				return;
			}
			else
			{
				copy_1(temp, auth[i]);
				for (int j = 0; j < user_num.size(); j++)
					temp[author - 1][user_num[j] - 1] = 1;
				copy_2(temp, auth[i]);
			}
		}
		auth_to_file(name);
	}
}

void Authority::clear_all(int user, int auth)
{
	if (auth == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if (DROP[user - 1][i] == 1)
			{
				DROP[user - 1][i] = 0;
				if (!(DROP[0][i] || DROP[1][i] || DROP[2][i] || DROP[3][i]))
					clear_all(i + 1, auth);
			}
		}
	}
	else if (auth == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			if (INSERT[user - 1][i] == 1)
			{
				INSERT[user - 1][i] = 0;
				if (!(INSERT[0][i] || INSERT[1][i] || INSERT[2][i] || INSERT[3][i]))
					clear_all(i + 1, auth);
			}
		}
	}
	else if (auth == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			if (DELETE[user - 1][i] == 1)
			{
				DELETE[user - 1][i] = 0;
				if (!(DELETE[0][i] || DELETE[1][i] || DELETE[2][i] || DELETE[3][i]))
					clear_all(i + 1, auth);
			}
		}
	}
	else if (auth == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			if (SELECT[user - 1][i] == 1)
			{
				SELECT[user - 1][i] = 0;
				if (!(SELECT[0][i] || SELECT[1][i] || SELECT[2][i] || SELECT[3][i]))
					clear_all(i + 1, auth);
			}
		}
	}
}

void Authority::revoke(string name, int author, vector<int> auth, vector<string> users)
{
	read_auth(name);
	if (auth[0] == -1)
	{
		vector<int> user_num;
		for (int i = 0; i < users.size(); i++)
			user_num.push_back(users[i][4] - 48);
		for (int k = 0; k < user_num.size(); k++)
		{
			if (DROP[author - 1][user_num[k] - 1] == 1)
			{
				DROP[author - 1][user_num[k] - 1] = 0;
				clear_all(user_num[k], 0);
			}
			if (INSERT[author - 1][user_num[k] - 1] == 1)
			{
				INSERT[author - 1][user_num[k] - 1] = 0;
				clear_all(user_num[k], 1);
			}
			if (DELETE[author - 1][user_num[k] - 1] == 1)
			{
				DELETE[author - 1][user_num[k] - 1] = 0;
				clear_all(user_num[k], 2);
			}
			if (SELECT[author - 1][user_num[k] - 1] == 1)
			{
				SELECT[author - 1][user_num[k] - 1] = 0;
				clear_all(user_num[k], 3);
			}
		}
		auth_to_file(name);
	}
	else
	{
		vector<int> user_num;
		for (int i = 0; i < users.size(); i++)
			user_num.push_back(users[i][4] - 48);
		for (int k = 0; k < user_num.size(); k++)
		{
			for (int m = 0; m < auth.size(); m++)
			{
				if (auth[m] == 0)
				{
					if (DROP[author - 1][user_num[k] - 1] == 0)
					{
						cout << "Revoke failed!\n";
						return;
					}
					else
					{
						DROP[author - 1][user_num[k] - 1] = 0;
						clear_all(user_num[k], 0);
					}
				}
				else if (auth[m] == 1)
				{
					if (INSERT[author - 1][user_num[k] - 1] == 0)
					{
						cout << "Revoke failed!\n";
						return;
					}
					else
					{
						INSERT[author - 1][user_num[k] - 1] = 0;
						clear_all(user_num[k], 1);
					}
				}
				else if (auth[m] == 2)
				{
					if (DELETE[author - 1][user_num[k] - 1] == 0)
					{
						cout << "Revoke failed!\n";
						return;
					}
					else
					{
						DELETE[author - 1][user_num[k] - 1] = 0;
						clear_all(user_num[k], 2);
					}
				}
				else if (auth[m] == 3)
				{
					if (SELECT[author - 1][user_num[k] - 1] == 0)
					{
						cout << "Revoke failed!\n";
						return;
					}
					else
					{
						SELECT[author - 1][user_num[k] - 1] = 0;
						clear_all(user_num[k], 3);
					}
				}
			}
		}
		auth_to_file(name);
	}
}

bool User::login(string a, string b)
{
	int i = 0;
	for (i = 0; i < 4; i++)
		if (users[i][0] == a)
		{
			if (users[i][1] == b)
			{
				user_number = i + 1;
				return true;
			}
			else
			{
				cout << "Wrong password!\n";
				return false;
			}
		}
	if (i == 4)
	{
		cout << "Wrong username!\n";
		return false;
	}
}
