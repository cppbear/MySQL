#define _CRT_SECURE_NO_WARNINGS
#include"Interface.h"
//using namespace std;

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
				bool s = h.auth_check(u.user_num(), name, 1);
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
				bool s = h.auth_check(u.user_num(), name, 1);
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
					bool s = h.auth_check(u.user_num(), name, 1);
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
					bool s = h.auth_check(u.user_num(), name, 1);
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
					bool s = h.auth_check(u.user_num(), name, 1);
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
				bool s = h.auth_check(u.user_num(), name, 1);
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
