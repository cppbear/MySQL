#include"Table.h"
#include<algorithm>

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
			for (int t = 0; t < temp[1].size(); t++)
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
