#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>

#define WELCOME "\n\n\t\t###################################\t\t\n\t\t\n\t\t||***MYBANK MANAGEMENT SYSTEM***||\n\n\t\t###################################\t\t\n\n"

void mainMenuBank(int);
void showMenu(int);
void admin_operations(int);
void LoginjointAccount(int);
void ShowjointMenu(int);
void AccountDelete(int socketDesc);
void JointAccountDelete(int socketDesc);
void newJointAccount(int socketDesc, int flag);
void signUp(int socketDesc, int flag);
void login(int socketDesc);
void checkDetails(int socketDesc);
void viewJointDetails(int socketDesc);
void deposit(int socketDesc);

void AccountDelete(int socketDesc)
{
	char username[20],buf[100];

	printf("Enter the username to be deleted: ");
	scanf(" %s", username);

	write(socketDesc, username, sizeof(username));
	read(socketDesc, buf, sizeof(buf));
	
	puts(buf);
	admin_operations(socketDesc);
}

void JointAccountDelete(int socketDesc)
{
	char username1[20], username2[20], buf[100];

	printf("\nEnter Username 1 to delete: ");
	scanf(" %s",username1);
	printf("\nEnter Username 2 to delete: ");
	scanf(" %s",username2);

	write(socketDesc, username1, sizeof(username1));
	write(socketDesc, username2, sizeof(username2));

	read(socketDesc, buf, sizeof(buf));

	puts(buf);
	admin_operations(socketDesc);
}

void newJointAccount(int socketDesc, int flag)
{
	char username1[20], password1[20], username2[20], password2[20], buf[100];
	printf("\nEnter first username: ");
	scanf(" %s", username1);
	printf("\nEnter 1 password: ");
	scanf(" %s", password1);
	printf("\nEnter second username: ");
	scanf(" %s", username2);
	printf("\nEnter 2 password: ");
	scanf(" %s", password2);

	write(socketDesc, username1, sizeof(username1));
	write(socketDesc, password1, sizeof(password1));
	write(socketDesc, username2, sizeof(username2));
	write(socketDesc, password2, sizeof(password2));

	read(socketDesc, buf, sizeof(buf));
	fflush(stdout);
	puts(buf);
	fflush(stdout);
	if(flag==1)
		mainMenuBank(socketDesc);
	else
		admin_operations(socketDesc);
}

void signUp(int socketDesc, int flag)
{
	int account_id;
	char username[20],password[20], buf[100];

	printf("\nEnter username: ");
	scanf(" %s",username);
	printf("\nEnter password: ");
	scanf(" %s",password);

	write(socketDesc, username,sizeof(username));
	write(socketDesc, password, sizeof(password));

	read(socketDesc, buf, sizeof(buf));
	puts(buf);
	if(flag==1)
		mainMenuBank(socketDesc);
	else
		admin_operations(socketDesc);
}

void login(int socketDesc)
{
	char buf[50];
	char username[20],password[20];
	int flag;

	printf("\nEnter username: ");
	scanf(" %s",username);
	printf("\nEnter password: ");
	scanf(" %s",password);

	write(socketDesc, username,sizeof(username));
	write(socketDesc, password, sizeof(password));

	read(socketDesc, &flag, sizeof(flag));
	read(socketDesc, buf, sizeof(buf));
	printf("%s",buf);

	if(flag==1)
		showMenu(socketDesc);
	else
		mainMenuBank(socketDesc);


}

void LoginjointAccount(int socketDesc)
{
	char buf[100];
	char username1[20],password[20],username2[20];
	int flag;

	printf("Enter username: ");
	scanf(" %s",username1);
	
	
	printf("Enter password for user: ");
	scanf(" %s",password);

	write(socketDesc, username1,sizeof(username1));
	write(socketDesc, password, sizeof(password));

	read(socketDesc, &flag, sizeof(flag));
	read(socketDesc, buf, sizeof(buf));
	printf("%s",buf);

	if(flag==1)
	{
		ShowjointMenu(socketDesc);
	}
	else
		mainMenuBank(socketDesc);

}

void checkDetails(int socketDesc)
{
	char username[20];
	int account_id, balance;

	read(socketDesc,username, sizeof(username));
	read(socketDesc,&account_id, sizeof(account_id));
	read(socketDesc,&balance, sizeof(balance));
	printf("\n\nDetails related to account: \n\n");
	printf("\nUsername: %s\n, Account ID: %d\n, Balance: %d\n\n",username,account_id,balance);
	showMenu(socketDesc);
}

void viewJointDetails(int socketDesc)
{
	char username1[20], username2[20];
	int account_id, balance;

	read(socketDesc,username1, sizeof(username1));
	read(socketDesc,username2, sizeof(username2));
	read(socketDesc,&account_id, sizeof(account_id));
	read(socketDesc,&balance, sizeof(balance));

	printf("\n\nDetails related to account: \n\n");
	printf("\nUsername1: %s\n,Username2: %s\n, Account ID: %d\n, Balance: %d\n\n",username1,username2,account_id,balance);
	printf("\n\nEnter any key to continue: ");
	ShowjointMenu(socketDesc);
}

void deposit(int socketDesc)
{
	char amount1[100];
	printf("\nEnter amount to deposit: ");
	scanf("%s",amount1);

	for(int i=0;i<strlen(amount1);i++)
	{
		if(amount1[i]<48 && amount1[i]>57)
		{
			printf("Enter valid amount");
			showMenu(socketDesc);
		}
	}
	int amount=atoi(amount1);
	
	write(socketDesc, &amount, sizeof(amount));
	read(socketDesc, &amount, sizeof(amount));
	printf("\n\nFinal Balance: %d",amount);
	
	showMenu(socketDesc);
}

void jointDeposit(int socketDesc)
{
	char amount1[100];
	printf("\nEnter amount to deposit: ");
	scanf("%s",amount1);

	for(int i=0;i<strlen(amount1);i++)
	{
		if(amount1[i]<48 && amount1[i]>57)
		{
			printf("Enter valid amount");
			ShowjointMenu(socketDesc);
		}
	}
	int amount=atoi(amount1);
	
	write(socketDesc, &amount, sizeof(amount));
	read(socketDesc, &amount, sizeof(amount));
	printf("\n\nFinal Balance: %d",amount);
	
	ShowjointMenu(socketDesc);
}

void jointWithdraw(int socketDesc)
{

	char amount1[100];
	printf("\nEnter amount to deposit: ");
	scanf("%s",amount1);

	for(int i=0;i<strlen(amount1);i++)
	{
		if(amount1[i]<48 && amount1[i]>57)
		{
			printf("Enter valid amount");
			ShowjointMenu(socketDesc);
		}
	}
	int amount=atoi(amount1);
	int flag;

	write(socketDesc, &amount, sizeof(amount));
	read(socketDesc, &flag, sizeof(flag));
	
	if(flag==0)
	{
		printf("\n\nInsufficient Balance\n");
		read(socketDesc, &amount, sizeof(amount));
		printf("\n\nBalance: %d",amount);
	}
	else
	{
		read(socketDesc, &amount, sizeof(amount));
		printf(" Final Balance: %d",amount);

	}
	
	ShowjointMenu(socketDesc);
}

void withdraw(int socketDesc)
{
	char amount1[100];
	printf("\nEnter amount to deposit: ");
	scanf("%s",amount1);

	for(int i=0;i<strlen(amount1);i++)
	{
		if(amount1[i]<48 && amount1[i]>57)
		{
			printf("Enter valid amount");
			showMenu(socketDesc);
		}
	}
	int amount=atoi(amount1);
	int flag;

	write(socketDesc, &amount, sizeof(amount));
	read(socketDesc, &flag, sizeof(flag));
	
	if(flag==0)
	{
		printf("\n\nInsufficient Balance\n");
		read(socketDesc, &amount, sizeof(amount));
		printf("Balance: %d",amount);
	}
	else
	{
		read(socketDesc, &amount, sizeof(amount));
		printf(" Final Balance: %d",amount);

	}
	
	showMenu(socketDesc);
}

void jointPassChange(int socketDesc)
{
	char password[20];
	
	printf("\nEnter the new Password: ");
	scanf(" %s",password);
    

	write(socketDesc,password,sizeof(password));
	ShowjointMenu(socketDesc);
	
}

void passChange(int socketDesc)
{
	char password[20];
	
	printf("\nEnter the new Password: ");
	scanf(" %s",password);

	write(socketDesc,password,sizeof(password));
	showMenu(socketDesc);
	
}

void showMenu(int socketDesc)
{
	int option;
	
	//system("clear");
	printf("%s", WELCOME);
	printf("\n\n\t USER FUCNTIONS \n\n");
	printf("\n1.View Details\n\n2.Deposit\n\n3.Withdraw\n\n4.Password Change\n\n5.Logout\n\n");
	printf("Enter your option: ");
	scanf("%d",&option);

	write(socketDesc, &option, sizeof(option));
	 
	switch(option)
	{
		case 1: checkDetails(socketDesc);
				break;
		case 2: deposit(socketDesc);
				break;
		case 3: withdraw(socketDesc);
				break;
		case 4: passChange(socketDesc);
				break;
		case 5: mainMenuBank(socketDesc);
				break;
		default: printf("Enter correct option");
				break;

	}
}

void ShowjointMenu(int socketDesc)
{
	int option;
	
	//system("clear"); 
	printf("%s", WELCOME);
	printf("\n\n\t USER FUCNTIONS \n\n");
	printf("\n1.View Details\n\n2.Deposit\n\n3.Withdraw\n\n4.Password Change\n\n5.Logout\n\n");
	printf("Enter your option: ");
	scanf("%d",&option);

	write(socketDesc, &option, sizeof(option));

	switch(option)
	{
		case 1: viewJointDetails(socketDesc);
				break;
		case 2: jointDeposit(socketDesc);
				break;
		case 3: jointWithdraw(socketDesc);
				break;
		case 4: jointPassChange(socketDesc);
				break;
		case 5: mainMenuBank(socketDesc);
				break;
	

	}

}

void admin_operations(int socketDesc)
{
	int option;
	//system("clear");
	printf("%s", WELCOME);
	printf("\n\n\t ADMIN FUNCTIONS \t\n\n");
	printf("\n1.Add Account\n\n2.Delete Account\n\n3.Search Account\n\n4.Add Joint Account\n\n5.Delete Joint Account\n\n6.Search Joint Account\n\n7.LogOut\n\nEnter your option: ");
	scanf("%d", &option);
	
	char buf[100], username[20], password[20], username1[20], username2[20];
	write(socketDesc, &option, sizeof(option));
	
	switch(option)
	{
		case 1:signUp(socketDesc,0);
			break;
		case 2:AccountDelete(socketDesc);
			break;
		case 3:puts("\n\nEnter username to search: ");
			scanf("%s", username);
			write(socketDesc, username, sizeof(username));
			read(socketDesc, &buf, sizeof(buf));
			puts(buf);
			admin_operations(socketDesc);
			break;
		case 4:newJointAccount(socketDesc,0);
			break;
		case 5:JointAccountDelete(socketDesc);
			break;
		case 6:puts("\n\nEnter username 1 to search: ");
			scanf(" %s", username1);
			puts("\n\nEnter username 2 to search: ");
			scanf(" %s", username2);
			write(socketDesc, username1, sizeof(username1));
			write(socketDesc, username2, sizeof(username2));
			read(socketDesc, &buf, sizeof(buf));
			puts(buf);
			admin_operations(socketDesc);
			break;
		case 7:mainMenuBank(socketDesc);
	}
}

void admin_Login(int socketDesc)
{
	char username[20], password[20], buf[100];
	int flag;
	
	printf("\n\nEnter the admin username: ");
	scanf(" %s",username);
	printf("\n\nEnter admin password: ");
	scanf(" %s",password);
	
	write(socketDesc, username, sizeof(username));
	write(socketDesc, password, sizeof(password));
	read(socketDesc, &flag, sizeof(flag));
	read(socketDesc, buf, sizeof(buf));
	
	printf("%s",buf);
	if(flag == 1)
		admin_operations(socketDesc);
	else
		mainMenuBank(socketDesc);
}

void mainMenuBank(int socketDesc)
{
	int option;
	
	//system("clear");
	printf("%s", WELCOME);
	printf("\n\n1.LOGIN as SINGLE USER\n\n2.LOGIN as ADMIN \n\n3.LOGIN as JOINT ACCOUNT USER\n\n4.EXIT\n\n");
	printf("Enter your option: ");
	scanf("%d",&option);

	write(socketDesc, &option, sizeof(option));

	switch(option)
	{
		case 1: login(socketDesc);
				break;
		case 2: admin_Login(socketDesc);
				break;
		case 3: LoginjointAccount(socketDesc);
				break;
		case 4: exit(0);
				break;

	}
}

int main()
{
	printf("%s", WELCOME);
	struct sockaddr_in server;
	int socketDesc;
	char buf[100];
	socketDesc = socket(AF_INET, SOCK_STREAM,0);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8080);

	connect(socketDesc, (struct sockaddr *)(&server), sizeof(server));
	
	mainMenuBank(socketDesc);
	close(socketDesc);
	return 0;
}
