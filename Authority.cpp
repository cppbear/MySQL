#include"Authority.h"

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
