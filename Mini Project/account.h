#define SIZE 20

struct account
{
	char username[SIZE];
	char password[SIZE];
	int account_id;
	int balance;
	int activeStatus;
};

struct jointAccount
{
	char username1[SIZE];
	char password1[SIZE];
	char username2[SIZE];
	char password2[SIZE];
	int account_id;
	int balance;
	int activeStatus;
};