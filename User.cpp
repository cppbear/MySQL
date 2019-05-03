#include"User.h"

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
