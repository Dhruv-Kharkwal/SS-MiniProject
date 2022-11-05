#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "account.h"

#define SIZE 20
char currUser[20];


int singlefd,jointfd;
struct flock slock,jlock;

void mainMenuBank(int );
void showMenu(int, struct account);
void admin_operations(int);
void AccountDelete(int);
void JointAccountDelete(int);
void DisplayMenuJoint(int, struct jointAccount,struct flock,int );

void searchDetails(struct account account_user)
{
	struct account account_struct;
	int fileDesc1 = open("Accounts.txt", O_CREAT | O_RDWR , 0666);

	while(read(fileDesc1, (char *)&account_struct, sizeof (struct account)))
	{
		
		if(!strcmp(account_user.username, account_struct.username))
		{
			account_struct.balance = account_user.balance;
			strcpy(account_struct.password,account_user.password);
			lseek(fileDesc1,-sizeof(struct account),SEEK_CUR);
			write(fileDesc1, (char *)&account_struct,sizeof(struct account));
			break;
		}
	}
}
//Used for finding and also for updating
void SearchjointDetails(struct jointAccount account_user)
{
	struct jointAccount account_struct;
	
	int fileDesc1 = open("Joint_Account.txt", O_CREAT | O_RDWR , 0666);

	while(read(fileDesc1, (char *)&account_struct, sizeof (struct jointAccount)))
	{
	       
		if(!strcmp(account_user.username1, account_struct.username1) && !strcmp(account_user.username2, account_struct.username2))
		{
			

			account_struct.balance = account_user.balance;
			strcpy(account_struct.password1,account_user.password1);
			strcpy(account_struct.password2,account_user.password2);
			lseek(fileDesc1,-sizeof(struct jointAccount),SEEK_CUR);
			write(fileDesc1, (char *)&account_struct,sizeof(struct jointAccount));
			break;
		}
	}
}

int jointAccountExists(char username1[],char username2[])
{
	struct jointAccount account_struct;

	int fileDesc1 = open("Joint_Account.txt", O_CREAT | O_APPEND | O_RDWR, 0666);
    
	while(read(fileDesc1, (char *)&account_struct, sizeof(struct jointAccount)))
	{
               if(account_struct.activeStatus)
               {
		if((!strcmp(account_struct.username1, username1)) && (!strcmp(account_struct.username2,username2)) || (!strcmp(account_struct.username1, username2)) && (!strcmp(account_struct.username2,username1)))
			{
				close(fileDesc1);
				return 1;
			}
               }	
	}
	close(fileDesc1);
	return 0;	
}

int checkAccountExists(char uname[])
{
	struct account account_struct;

	int fileDesc1 = open("Accounts.txt", O_CREAT | O_APPEND | O_RDWR , 0666);
	
	while(read(fileDesc1, (char *)&account_struct, sizeof(struct account)))
	{
        if(account_struct.activeStatus)
        {
		    if(!strcmp(account_struct.username, uname))
			{
				close(fileDesc1);
				return 1;
			}
		}
	}
	close(fileDesc1);
	return 0;	
	
}

void addjointAccount(char username1[], char password1[], char username2[], char password2[], int socketDesc)
{
	srand(time(0));

	int fileDesc1 = open("Joint_Account.txt", O_CREAT | O_APPEND | O_RDWR  , 0666);

	struct jointAccount account_struct;

	strcpy(account_struct.username1, username1);
	strcpy(account_struct.password1, password1);
	strcpy(account_struct.username2, username2);
	strcpy(account_struct.password2, password2);

	account_struct.balance=0;
        account_struct.activeStatus=1;
	account_struct.account_id=(rand()%1001)+100000;

	write(fileDesc1, (char *)&account_struct, sizeof(struct jointAccount));

	write(socketDesc, "New Joint Account Created successfully", sizeof("New Joint Account Created successfully"));
	
	close(fileDesc1);
}

void addAccount(char uname[], char pass[],int socketDesc)
{
	srand(time(0));

	int fileDesc1 = open("Accounts.txt", O_CREAT | O_APPEND | O_RDWR , 0666);

	struct account account_struct;

	strcpy(account_struct.username, uname);
	strcpy(account_struct.password, pass);
        account_struct.activeStatus=1;
	account_struct.balance=0;

	account_struct.account_id=(rand()%1001)+1000;

	write(fileDesc1, (char *)&account_struct, sizeof(struct account));

	write(socketDesc, "New Account Created succesfully" , sizeof("New Account Created succesfully"));
	
	close(fileDesc1);
}

void newJointAccount(int socketDesc, int flag)
{
	char username1[20], password1[20], username2[20], password2[20];

	read(socketDesc, username1, sizeof(username1));
	read(socketDesc, password1, sizeof(password1));
	read(socketDesc, username2, sizeof(username2));
	read(socketDesc, password2, sizeof(password2));

	if(!jointAccountExists(username1,username2))
	{
		addjointAccount(username1, password1, username2, password2, socketDesc);
		if(flag==1)
			mainMenuBank(socketDesc);
		else
			admin_operations(socketDesc);
	}
	else
	{
		write(socketDesc, "Cannot Create account Username already exist", sizeof("Cannot Create Account Username already exist"));
		if(flag ==1)
			mainMenuBank(socketDesc);
		else
			admin_operations(socketDesc);
	}


}

void signUp(int socketDesc,int flag)
{
	char uname[20], pass[20];

	read(socketDesc, uname, sizeof(uname));
	read(socketDesc, pass, sizeof(pass));

	if(!checkAccountExists(uname))
	{
		addAccount(uname, pass, socketDesc);
		if(flag ==1)
			mainMenuBank(socketDesc);
		else
			admin_operations(socketDesc);
	}
	else
	{
		write(socketDesc, "Username already exist", sizeof("Username already exist"));
		if(flag ==1)
			mainMenuBank(socketDesc);
		else
			admin_operations(socketDesc);
	}	

}

void login(int socketDesc)
{
	
	char uname[20], pass[20];

	read(socketDesc, uname, sizeof(uname));
	read(socketDesc, pass, sizeof(pass));

	struct account account_struct;
	int flag=1,f=0;
       
       singlefd = open("Accounts.txt", O_CREAT | O_APPEND | O_RDWR , 0666);
	
	while(read(singlefd, (char *)&account_struct, sizeof(struct account)))
	{

        if(account_struct.activeStatus)
        {
			if(!strcmp(account_struct.username, uname) && !strcmp(account_struct.password, pass))
			{
			    lseek(singlefd,-sizeof(struct account),SEEK_CUR);
				slock.l_type=F_WRLCK;
				slock.l_whence=SEEK_CUR;
				slock.l_start=0;
				slock.l_len=sizeof(struct account);

			/*Locking*/	
			// This if call is used to avoid login of same customer again and again..
			
				if(fcntl(singlefd,F_SETLK,&slock)!=0) 
				{
				    f=1;break;
				}
				write(socketDesc, &flag, sizeof(flag));
				write(socketDesc, "Account logged-in Succesfully", sizeof("Account logged-in Succesfully"));
				showMenu(socketDesc,account_struct);
			        close(singlefd);
				return;		
			}
		}
	}
	close(singlefd);
	flag=0;
	write(socketDesc, &flag, sizeof(flag));
	if(f!=1) write(socketDesc, "\nInvalid Credentials", sizeof("\nInvalid Credentials"));
	else write(socketDesc, "\n\nUser Already logged in, Try again later\n\n", sizeof("\n\nUser Already logged in, Try again later\n\n"));
	mainMenuBank(socketDesc);	
}

void LoginjointAccount(int socketDesc,int fileDesc)
{
	char username1[20], username2[20], pass[20];
	struct flock lock;

	read(socketDesc, username1, sizeof(username1));
	read(socketDesc, pass, sizeof(pass));
	// TO BE CHECKED..
	strcpy(currUser,username1);

	struct jointAccount account_struct;
	int flag=1;
	

	int fileDesc1 = open("Joint_Account.txt", O_CREAT | O_APPEND | O_RDWR , 0666);
	int f=0;
	
	while(read(fileDesc1, (char *)&account_struct, sizeof(struct jointAccount)))
	{
               if(account_struct.activeStatus)
		{
		if((!strcmp(account_struct.username1, username1) && !strcmp(account_struct.password1,pass)) || !strcmp(account_struct.username2, username1) && !strcmp(account_struct.password2,pass))
			{
				lseek(fileDesc1,-sizeof(struct jointAccount),SEEK_CUR);
				lock.l_type=F_WRLCK;
				lock.l_whence=SEEK_CUR;
				lock.l_start=0;
				lock.l_len=sizeof(struct jointAccount);

			/*Locking*/	
				if(fcntl(fileDesc1,F_SETLK,&lock)!=0) 
				{
				    f=1;break;
				}
				
				write(socketDesc, &flag, sizeof(flag));
				write(socketDesc, "Account Login Successful", sizeof("Account Login Successful"));
				DisplayMenuJoint(socketDesc,account_struct,lock,fileDesc1);
				
				

				close(fileDesc1);
			
				return;		
		}	
		}
	}
      	close(fileDesc1);
	flag=0;
	write(socketDesc, &flag, sizeof(flag));
	if(f!=1) write(socketDesc, "Invalid Credentials", sizeof("Invalid Credentials"));
	else write(socketDesc, "Another user logged in", sizeof("Another user logged in"));
	mainMenuBank(socketDesc);
}

void checkDetails(int socketDesc, struct account account_struct)
{
	write(socketDesc,account_struct.username, sizeof(account_struct.username));
	write(socketDesc,&account_struct.account_id, sizeof(account_struct.account_id));
	write(socketDesc,&account_struct.balance, sizeof(account_struct.balance));
	showMenu(socketDesc,account_struct);
}

void viewJointDetails(int socketDesc, struct jointAccount acccount_struct,struct flock lock,int fileDesc)
{
	write(socketDesc,acccount_struct.username1, sizeof(acccount_struct.username1));
	write(socketDesc,acccount_struct.username2, sizeof(acccount_struct.username2));
	write(socketDesc,&acccount_struct.account_id, sizeof(acccount_struct.account_id));
	write(socketDesc,&acccount_struct.balance, sizeof(acccount_struct.balance));
	DisplayMenuJoint(socketDesc,acccount_struct,lock,fileDesc);
}

void deposit(int socketDesc, struct account account_struct)
{
	int amount;
	
	read(socketDesc, &amount, sizeof(amount));
	if(amount>0)
	account_struct.balance = account_struct.balance + amount;
	searchDetails(account_struct);
	write(socketDesc, &account_struct.balance, sizeof(account_struct.balance));
	
	showMenu(socketDesc,account_struct);
}

void jointDeposit(int socketDesc, struct jointAccount account_struct,struct flock lock,int fileDesc)
{
	int amount;
	
	
	read(socketDesc, &amount, sizeof(amount));

    if(amount>0)
	account_struct.balance = account_struct.balance + amount;
	
       SearchjointDetails(account_struct);
	write(socketDesc, &account_struct.balance, sizeof(account_struct.balance));
	
	DisplayMenuJoint(socketDesc,account_struct,lock,fileDesc);
}

void jointWithdraw(int socketDesc, struct jointAccount account_struct,struct flock lock, int fileDesc)
{
	int amount, flag=0;
	
	read(socketDesc, &amount, sizeof(amount));

	if(amount>account_struct.balance || amount <0)
	{
		write(socketDesc,&flag,sizeof(flag));
		write(socketDesc, &account_struct.balance, sizeof(account_struct.balance));
	}
	else
	{
		flag=1;
		account_struct.balance = account_struct.balance - amount;
		SearchjointDetails(account_struct);
		write(socketDesc,&flag,sizeof(flag));
		write(socketDesc, &account_struct.balance, sizeof(account_struct.balance));
	}


	DisplayMenuJoint(socketDesc,account_struct,lock,fileDesc);
}

void withdraw(int socketDesc, struct account account_struct)
{
	int amount, flag=0;
	
	read(socketDesc, &amount, sizeof(amount));

	if(amount>account_struct.balance || amount<0)
	{
		write(socketDesc,&flag,sizeof(flag));
		write(socketDesc, &account_struct.balance, sizeof(account_struct.balance));
	}
	else
	{
		flag=1;
		account_struct.balance = account_struct.balance - amount;
		searchDetails(account_struct);
		write(socketDesc,&flag,sizeof(flag));
		write(socketDesc, &account_struct.balance, sizeof(account_struct.balance));
	}


	showMenu(socketDesc,account_struct);
}

void jointPassChange(int socketDesc, struct jointAccount account_struct,struct flock lock)
{
	char pass[SIZE];
	read(socketDesc,pass,sizeof(pass));
    if(!strcmp(currUser,account_struct.username1))
	   strcpy(account_struct.password1,pass);
	else
		strcpy(account_struct.password2,pass);

	SearchjointDetails(account_struct);
	mainMenuBank(socketDesc);

}

void passChange(int socketDesc, struct account account_struct)
{
	char pass[SIZE];
	read(socketDesc,pass,sizeof(pass));
	strcpy(account_struct.password,pass);
	searchDetails(account_struct);
	mainMenuBank(socketDesc);

}

void showMenu(int socketDesc, struct account account_struct)
{
	int option;
	read(socketDesc, &option, sizeof(option));
	switch(option)
	{
		case 1: checkDetails(socketDesc,account_struct);
				break;
		case 2: deposit(socketDesc,account_struct);
				break;
		case 3: withdraw(socketDesc,account_struct);
				break;
		case 4: passChange(socketDesc,account_struct);
				break;
		case 5: {slock.l_type=F_UNLCK;
	               fcntl(singlefd,F_SETLK,&slock);
		         mainMenuBank(socketDesc);
				break;}

	}

}

void DisplayMenuJoint(int socketDesc,struct jointAccount account_struct,struct flock lock,int fileDesc)
{
	int option;
	read(socketDesc, &option, sizeof(option));
	switch(option)
	{
		case 1: viewJointDetails(socketDesc,account_struct,lock,fileDesc);
				break;
		case 2: jointDeposit(socketDesc,account_struct,lock,fileDesc);
				break;
		case 3: jointWithdraw(socketDesc,account_struct,lock,fileDesc);
				break;
		case 4: jointPassChange(socketDesc,account_struct,lock);
				break;
		case 5: lock.l_type=F_UNLCK;
				fcntl(fileDesc,F_SETLK,&lock); 
				mainMenuBank(socketDesc);
				break;

	}
}

void admin_operations(int socketDesc)
{
	int option;
	char uname[20], pass[20], username1[20], username2[20];
	read(socketDesc, &option, sizeof(option));
	switch(option)
	{
		case 1:signUp(socketDesc,0);
			break;
		case 2:AccountDelete(socketDesc);
			break;
		case 3:read(socketDesc, uname, sizeof(uname));
			if(checkAccountExists(uname))
			{
				write(socketDesc, "Account Exists", sizeof("Account Exists"));
			}
			else
			{
				write(socketDesc, "Account does not exist with the given Username", sizeof("Account does not exist with the given Username"));
			}
			admin_operations(socketDesc);
			break;
		case 4:newJointAccount(socketDesc,0);
			break;
		case 5:JointAccountDelete(socketDesc);
		     	break;
		case 6:read(socketDesc, username1, sizeof(username1));
				read(socketDesc, username2, sizeof(username2));
				if(jointAccountExists(username1,username2))
			{
				write(socketDesc, "Account Found", sizeof("Account Found"));
			}
			else
			{
				write(socketDesc, "Account does not exist with the given Username", sizeof("Account does not exist with the given Username"));
			}
			admin_operations(socketDesc);
			break;
		default:mainMenuBank(socketDesc);
	}
}

void AccountDelete(int socketDesc)
{
	char uname[20];
	int fileDesc1;

	fileDesc1 = open("Accounts.txt", O_CREAT | O_RDWR , 0666);
	read(socketDesc, uname, sizeof(uname));

	if(!checkAccountExists(uname))
	{
		write(socketDesc, "Account does not exist", sizeof("Account does not exist"));
	}
	else
	{	

		struct account account_struct;
		int i=0;
		lseek(fileDesc1, 0, SEEK_SET);
		while(read(fileDesc1, (char *)&account_struct, sizeof(struct account)))
		{
			if(!strcmp(account_struct.username, uname))
			{
			        account_struct.activeStatus=0;
			        lseek(fileDesc1,sizeof(account_struct)*i,SEEK_SET);
				write(fileDesc1,(char *)&account_struct,sizeof(struct account));
				break;				
			}
			i++;
		}
		lseek(fileDesc1,0, SEEK_SET);
		write(socketDesc,"Account deleted Successfully", sizeof("Account deleted Successfully"));
	}

	close(fileDesc1);
	admin_operations(socketDesc);
}

void JointAccountDelete(int socketDesc)
{
	char username1[20], username2[20];
	int fileDesc1;

	fileDesc1 = open("Joint_Account.txt", O_CREAT | O_RDWR , 0666);
	read(socketDesc, username1, sizeof(username1));
	read(socketDesc, username2, sizeof(username2));
    
	if(!jointAccountExists(username1,username2))
	{
		
		write(socketDesc, "Account does not exist", sizeof("Account does not exist"));
	}
	else
	{	
               struct jointAccount account_struct;

		int i=0;
		lseek(fileDesc1, 0, SEEK_SET);
		while(read(fileDesc1, (char *)&account_struct, sizeof(struct jointAccount)))
		{
			if((!strcmp(account_struct.username1, username1)) && (!strcmp(account_struct.username2,username2)) || (!strcmp(account_struct.username1, username2)) && (!strcmp(account_struct.username2,username1)))
			{
			        account_struct.activeStatus=0;
			        lseek(fileDesc1,sizeof(account_struct)*i,SEEK_SET);
				write(fileDesc1,(char *)&account_struct,sizeof(struct jointAccount));
				break;				
			}
			i++;
		}
		write(socketDesc,"Account deleted Successfully", sizeof("Account deleted Successfully"));
	}

	close(fileDesc1);
	admin_operations(socketDesc);

}

void admin_Login(int socketDesc)
{
	
	char uname[20], pass[20];
	int flag=1;
	read(socketDesc, uname, sizeof(uname));
	read(socketDesc, pass, sizeof(pass));
	if(!strcmp(uname,"dhruv") && !strcmp(pass,"dhruv"))
	{
		write(socketDesc, &flag, sizeof(flag));
		write(socketDesc,"\n\nWelcome dhruv\n\n", sizeof("\n\nWelcome dhruv\n\n"));
		admin_operations(socketDesc);
	}
	else
	{	
		flag=0;
		write(socketDesc, &flag, sizeof(flag));
		write(socketDesc,"\n\nInvalid Credentials\n\n", sizeof("\n\nInvalid Credentials\n\n"));
		mainMenuBank(socketDesc);
	}

}

void mainMenuBank(int socketDesc)
{
	int option,fileDesc;
	read(socketDesc, &option, sizeof(option));
	switch(option)
	{
		case 1: login(socketDesc);
				break;
		case 2: admin_Login(socketDesc);
				break;
		case 3: LoginjointAccount(socketDesc,fileDesc);
				break;
		case 4: exit(0);
				break;

	}
}

int main()
{

	struct sockaddr_in server, client;
	int socketDesc, client_addr_len, nsd;
	
	socketDesc = socket(AF_INET, SOCK_STREAM,0);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8080);

	if(bind(socketDesc,(struct sockaddr *)(&server), sizeof(server))<0){
		printf("\nError during bind\n");
	}

	listen(socketDesc,10);
	client_addr_len = sizeof(client);
	write(1,"Waiting for the client.....\n",sizeof("Waiting for the client.....\n"));
	while(1){

		nsd = accept(socketDesc, (struct sockaddr *)&client, &client_addr_len);
		write(1,"Connected to the client.....\n",sizeof("Connected to the client.....\n"));
		if(!fork()){

			close(socketDesc);
			mainMenuBank(nsd);
			exit(0);

		}
		else
		{
			close(nsd);
		}

	}
		
	close(nsd);
	close(socketDesc);
	return 0;
}
