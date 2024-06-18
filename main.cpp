#define _CRT_SECURE_NO_WARNINGS
//libraries
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include<stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <windows.h>
#include <process.h>
#include <sys/stat.h>
#include <direct.h>
#include"unistd.h"
#include "dirent.h.h"
struct user {

	char name[51];                        //user name
	char username[51];                   //user username
	char pass[51];                       //password
	int pass_s;                 //password security
	short int isadmin = 0;               //is admin or not
	int wrong = 0;              //wrong command number
	int time;                            //time limit
	char permission[1000] = " ";               //folder entrance permission
};

typedef struct user user;
//functions
void setcolor(int colorcode)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, colorcode % 255);
}
void create_main_admin(user users[]);
int security_check(char pass[]);
void file_write(user users[], int);
void wc(char* file);
void compareFiles(FILE* fp1, FILE* fp2);
void create_user(user users[], int user_n);
void permission_access(int user_n);
void su(int user_n);
void passwd(user users[], int user_n);
void passw_l(user users[], int user_n);
void chgr(user users[], int user_n);
void pwd(void);
void cd(user users[]);
void help(void);
void mkdir(void);
void cat(void);
void rm(void);
void rm_r(void);
void cp(void);
void mv(void);
void ls(void);
void wc(void);
void diff(void);
void appendopperand(char command[]);
void writeopperand(char command[]);
void hiddenplus(void);
void hiddenmin(void);
void searchfile(void);

//global variations
int user_n = 1;                         //users number
int working_user;                        //who works now
char command[21];                         //commands
FILE* usersfile;                          //main file
FILE* usersnumberfile;                     //number of users
user users[100];                        //users
int uflag = 0;
int lflag = 0;
int suflag = 0;
int passflag = 0;
int logflag = 0;
int chflag = 0;
int cdflag = 0;
int tflag = 0;
int dnow;
FILE* test;
FILE* test2;
char permission[10000];
char password[51];
char exePath[MAX_PATH];

int main()
{
	create_main_admin(users);
	//reading number o users
	usersnumberfile = fopen("usersnumberfile.txt", "rb");
	if (usersnumberfile == NULL)
	{
		usersnumberfile = fopen("usersnumberfile.txt", "wb");
		fprintf(usersnumberfile, "%d", 1);
	}
	fscanf(usersnumberfile, "%d", &user_n);
	fclose(usersnumberfile);
	//create main file
	usersfile = fopen("usersfile.txt", "r");
	if (usersfile == NULL)
	{
		_mkdir(users[0].username);
		usersfile = fopen("usersfile.txt", "w");
		fwrite(users, sizeof(user), 1, usersfile);
	}
	else
	{
		fread(users, sizeof(user), user_n, usersfile);
	}
	fclose(usersfile);
	//for (int i = 0; i < user_n; i++)
		//printf("%s %s %s %d %d %d %d %s \n", users[i].name, users[i].username, users[i].pass, users[i].time, users[i].isadmin, users[i].wrong, users[i].pass_s, users[i].permission);

	//start
	setcolor(245);
	puts("************************************************WELCOM TO NEGAR SHELL***************************************************");
	int login = 0;
	while (1) {
		//log in informations
		char logusername[51];
		char logpass[51];
		//login
		setcolor(3);
		printf("please enter your username to log in : \n");
		setcolor(15);
		scanf("%s", logusername);
		for (int i = 0; i < user_n; i++)
		{
			if (login == 1) break;
			uflag = 0;

			if (!strcmp(logusername, users[i].username))
			{
				uflag = 1;
				setcolor(3);
				printf("please enter your password:\n");
				setcolor(15);
				int j = 0;                   //wrong pass enter
				for (j = 0; j < 3; j++)
				{
					int passhold = 0;
					do
					{
						password[passhold] = _getch();
						if (password[passhold] != '\r')
							printf("*");
						passhold++;
					} while (password[passhold - 1] != '\r');
					password[passhold - 1] = '\0';
					printf("\n");
					strcpy(logpass, password);
					if (!strcmp(logpass, users[i].pass))
					{
						//time check
						time_t now = time(0);
						tm nowtm = *(gmtime(&now));
						int today = (nowtm.tm_year + 1900) * 10000 + (nowtm.tm_mon + 1) * 100 + nowtm.tm_mday;
						dnow = today;
						if (users[i].time <= today)
						{
							setcolor(5);
							printf("your time has expierd so You can't log in!!!\nplease try again with another username\n ");
							setcolor(15);
						}
						else
						{
							//open foldr
							setcolor(5);
							puts("you logged in successfully!!!");
							setcolor(15);
							login = 1;
							working_user = i;
							_chdir(users[working_user].username);
						}
						break;
					}
					else
					{

						if (j == 2)
						{
							setcolor(5);
							printf(" you entered wrong password 3 times ! please try again with another username!\n");
							setcolor(15);
							break;
						}
						setcolor(5);
						printf("wrong password please try again!!!\n");
						setcolor(15);
					}
				}
			}
			else
			{
				if (uflag) break;
				if (i == (user_n - 1))
				{
					setcolor(5);
					printf(" can't find this username! please try again with another username!\n ");
					setcolor(15);
					break;
				}
			}
		}
		if (login > 0)
			break;
	}
	//commands
	int check = 0;

	while (1)
	{
		system("pause");
		system("cls");
		//commands info
		int hold;
		char name2[51];
		char username2[51];
		char pass2[51];
		char repass2[51];
		unsigned int pass2_security = 0;
		int time2;
		int time2chek;
		setcolor(249);
		printf("*$*%s*$*\n", users[working_user].name);
		setcolor(4);
		printf("please use one of the available commands(for more info type HELP)\n");
		setcolor(15);
		//command
		scanf("%s", command);

		if (!strcmp(command, "createuser"))
			//create_user(users, user_n);
			{
			    int hold;
	char name2[51];
	char username2[51];
	char pass2[51];
	char repass2[51];
	unsigned int pass2_security = 0;
	int time2;
	int time2chek;
	if (!users[working_user].isadmin)
	{
		setcolor(5);
		printf("Only admins can create user so you are not allowed !\n");
		setcolor(15);
		users[working_user].wrong++;
	}
	else
	{
	    setcolor(3);
		printf("\n please enter the new user's name:\n");
		setcolor(15);
		scanf("%s", name2);
		setcolor(3);
		printf("\n please enter the new user's username:\n");
		setcolor(15);
		scanf("%s", username2);
	loop2:
	    setcolor(3);
		printf("Do you want to make this user unlimited?\nif yes enter 1 else 0\n");
		setcolor(15);
		scanf("%d", &time2chek);
		switch (time2chek)
		{
		case 1:
			time2 = 25000101;
			break;
		case 0:
		    setcolor(3);
			printf("enter the time limit like this yyyymmdd(example: 21001227 = 2100/12/27)\n");
			setcolor(15);
			scanf("%d", &time2);
			break;
		default:
            char wrong_input[101];
            setcolor(5);
			printf("Wrong input!! try a valid number :\n");
			setcolor(15);
			gets_s(wrong_input);
			goto loop2;
		}
	loop3:
	    setcolor(3);
		printf("if you want to give a permission to this user to enter other's folders enter 1 else 0:\n");
		setcolor(15);
		scanf("%d", &hold);
		switch (hold)
		{
		case 1:
		    setcolor(3);
			printf("please enter the allowed folders :\n");
			setcolor(15);
			scanf("%s", permission);
			break;
		case 0:
			strcpy(permission, "");
			break;
		default:
			char wrong_input[101];
			setcolor(5);
			printf("Wrong input!! try a valid number :\n");
			setcolor(15);
			gets(wrong_input);
			goto loop3;
		}
		logflag = 0;
		int j = 0;                   //wrong password number
		for (j = 0; j < 3; j++)
		{
			pass2_security = 0;
			setcolor(3);
			printf("\n please enter the new users password:\n");
			setcolor(15);
            scanf("%s",pass2);
			pass2_security = security_check(pass2);
			if (pass2_security > 33)
			{
			    setcolor(3);
				printf("\n please enter the new users password again:\n");
				setcolor(15);
				scanf("%s", repass2);
				if (!strcmp(repass2, pass2))
				{
				    setcolor(5);
					printf("user %s has been created successfully!!!\n", username2);
					setcolor(15);
					user_n++;
					logflag = 1;
					break;
				}
				else
				{
				    setcolor(5);
					printf("passwords are not match!!!try again!!!\n");
					setcolor(15);
				}
			}
			else
			{
				if (j == 2)
				{
				    setcolor(5);
					printf("You entered wrong password 3 times! please try again with another command!!!\n");
					setcolor(15);
					break;
				}
				setcolor(5);
				printf("password is not strong enough try again!!!");
				setcolor(5);
			}
		}
	}
	if (logflag)
	{
		strcpy(users[user_n - 1].permission, permission);
		strcpy(users[user_n - 1].name, name2);
		strcpy(users[user_n - 1].username, username2);
		strcpy(users[user_n - 1].pass, pass2);
		users[user_n - 1].pass_s = pass2_security;
		users[user_n - 1].time = time2;
		users[user_n - 1].isadmin = 0;
		if (GetModuleFileName(NULL, exePath, sizeof(exePath)) != 0) 
		{
			// Extract the directory part of the executable path
			char* lastSlash = strrchr(exePath, '\\');
			if (lastSlash != NULL) 
			{
				*lastSlash = '\0';
			}
		}
		_chdir(exePath);
		file_write(users, user_n);
		_mkdir(username2);
		_chdir(users[working_user].username);
	}
			}

		else if (!strcmp(command, "permission"))
			permission_access(user_n);

		else if (!strcmp(command, "su"))
			su(user_n);

		else if (!strcmp(command, "passwd"))
			passwd(users, user_n);

		else if (!(strcmp(command, "passwd-l")))
			passw_l(users, user_n);

		else if (!strcmp(command, "chgr"))
			chgr(users, user_n);

		else if (!strcmp(command, "pwd"))
			pwd();

		else if (!strcmp(command, "cd"))
			cd(users);

		else if (!strcmp(command, "HELP"))
			help();

		else if (!strcmp(command, "mkdir"))
			mkdir();

		else if (!strcmp(command, "cat"))
			cat();

		else if (!strcmp(command, "rm"))
			rm();

		else if (!strcmp(command, "rm-r"))
			rm_r();

		else if (!strcmp(command, "cp"))
			cp();
		else if (!strcmp(command, "mv"))
			mv();
		else if (!strcmp(command, "ls"))
			ls();

		else if (!strcmp(command, "wc"))
			wc();

		else if (!strcmp(command, "diff"))
			diff();
		else if (!strcmp(command, "search"))
			searchfile();
		else if (!strcmp(command, "hidden+"))
			hiddenplus();
		else if (!strcmp(command, "hidden-"))
			hiddenmin();

		else
		{
			char amalgar[51];
			setcolor(15);
			scanf("%s", amalgar);
			if (strcmp(amalgar, ">") == 0)
				writeopperand(command);
			else if (strcmp(amalgar, ">>") == 0)
				appendopperand(command);
			else
			{
				setcolor(5);
				printf("command does not exist!!!\n");
				setcolor(15);
				char test[51];
				_getcwd(test, 51);
				users[working_user].wrong++;
				_chdir(exePath);
				file_write(users, user_n);
				_chdir(test);

			}
		}

	}


	return 0;
}



//functions
void create_main_admin(user users[])
{

	strcpy(users[0].name, "admin");
	strcpy(users[0].username, "admin");
	strcpy(users[0].pass, "admin");
	users[0].pass_s = 100;
	users[0].isadmin = 1;
	users[0].wrong = 0;
	users[0].time = 25000102;

}

int security_check(char pass[])
{
	int pass_security = 0;
	for (int i = 0; pass[i] != '\0'; i++)
	{
		if (pass[i] == 32)
			pass_security++;
		else if (pass[i] >= 65 && pass[i] <= 90)
			pass_security += 4;
		else if (pass[i] >= 48 && pass[i] <= 57)
			pass_security += 3;
		else if (pass[i] >= 97 && pass[i] <= 122)
			pass_security += 2;
		else
			pass_security += 8;
	}
	if (pass_security >= 100)
		pass_security = 100;

	return pass_security;
}
void file_write(user users[], int user_n)
{
	usersfile = fopen("usersfile.txt", "w");
	fwrite(users, sizeof(user), user_n, usersfile);
	fclose(usersfile);
	usersnumberfile = fopen("usersnumberfile.txt", "wb");
	fprintf(usersnumberfile, "%d", user_n);
	fscanf(usersnumberfile, "%d", &user_n);
	fclose(usersfile);
}
void compareFiles(FILE* fp1, FILE* fp2)
{

	char ch1 = getc(fp1);

	char ch2 = getc(fp2);

	int error = 0, pos = 0, line = 1;

	while (ch1 != EOF && ch2 != EOF)

	{

		pos++;

		if (ch1 == '\n' && ch2 == '\n')

		{

			line++;

			pos = 0;

		}

		if (ch1 != ch2)

		{
			error++;
            setcolor(5);
			printf("Line Number : %d \tError"

				" Position : %d \n", line, pos);
				setcolor(15);
		}
		ch1 = getc(fp1);

		ch2 = getc(fp2);
	}
	setcolor(5);
	printf("Total Errors : %d\n", error);
	setcolor(15);
}

/*void create_user(user users[], int user_n)
{
	int hold;
	char name2[51];
	char username2[51];
	char pass2[51];
	char repass2[51];
	unsigned int pass2_security = 0;
	int time2;
	int time2chek;
	if (!users[working_user].isadmin)
	{
		setcolor(5);
		printf("Only admins can create user so you are not allowed !\n");
		setcolor(15);
		users[working_user].wrong++;
	}
	else
	{
	    setcolor(3);
		printf("\n please enter the new user's name:\n");
		setcolor(15);
		scanf("%s", name2);
		setcolor(3);
		printf("\n please enter the new user's username:\n");
		setcolor(15);
		scanf("%s", username2);
	loop2:
	    setcolor(3);
		printf("Do you want to make this user unlimited?\nif yes enter 1 else 0\n");
		setcolor(15);
		scanf("%d", &time2chek);
		switch (time2chek)
		{
		case 1:
			time2 = 25000101;
			break;
		case 0:
		    setcolor(3);
			printf("enter the time limit like this yyyymmdd(example: 21001227 = 2100/12/27)\n");
			setcolor(15);
			scanf("%d", &time2);
			break;
		default:
            char wrong_input[101];
            setcolor(5);
			printf("Wrong input!! try a valid number :\n");
			setcolor(15);
			gets(wrong_input);
			goto loop2;
		}
	loop3:
	    setcolor(3);
		printf("if you want to give a permission to this user to enter other's folders enter 1 else 0:\n");
		setcolor(15);
		scanf("%d", &hold);
		switch (hold)
		{
		case 1:
		    setcolor(3);
			printf("please enter the allowed folders :\n");
			setcolor(15);
			scanf("%s", permission);
			break;
		case 0:
			strcpy(permission, "");
			break;
		default:
			char wrong_input[101];
			setcolor(5);
			printf("Wrong input!! try a valid number :\n");
			setcolor(15);
			gets(wrong_input);
			goto loop3;
		}
		logflag = 0;
		int j = 0;                   //wrong password number
		for (j = 0; j < 3; j++)
		{
			pass2_security = 0;
			setcolor(3);
			printf("\n please enter the new users password:\n");
			setcolor(15);
            scanf("%s",pass2);
			pass2_security = security_check(pass2);
			if (pass2_security > 33)
			{
			    setcolor(3);
				printf("\n please enter the new users password again:\n");
				setcolor(15);
				scanf("%s", repass2);
				if (!strcmp(repass2, pass2))
				{
				    setcolor(5);
					printf("user %s has been created successfully!!!\n", username2);
					setcolor(15);
					user_n++;
					logflag = 1;
					break;
				}
				else
				{
				    setcolor(5);
					printf("passwords are not match!!!try again!!!\n");
					setcolor(15);
				}
			}
			else
			{
				if (j == 2)
				{
				    setcolor(5);
					printf("You entered wrong password 3 times! please try again with another command!!!\n");
					setcolor(15);
					break;
				}
				setcolor(5);
				printf("password is not strong enough try again!!!");
				setcolor(5);
			}
		}
	}
	if (logflag)
	{
		strcpy(users[user_n - 1].permission, permission);
		strcpy(users[user_n - 1].name, name2);
		strcpy(users[user_n - 1].username, username2);
		strcpy(users[user_n - 1].pass, pass2);
		users[user_n - 1].pass_s = pass2_security;
		users[user_n - 1].time = time2;
		users[user_n - 1].isadmin = 0;
		_chdir(exePath);
		file_write(users, user_n);
		_mkdir(username2);
		_chdir(users[working_user].username);
	}
}*/
void permission_access(int user_n)
{
    _chdir(exePath);
	usersfile = fopen("usersfile.txt", "r");
		fread(users, sizeof(user), user_n, usersfile);
	fclose(usersfile);
	char usernamebuff[51];
	setcolor(3);
	printf("please Enter the user's username that can access your folder:\n");
    setcolor(15);
	scanf("%s", usernamebuff);
	int perflag = 0;
	for (int i = 0; i < user_n; i++)
	{
		if (!strcmp(usernamebuff, users[i].username))
		{
			strcat(users[i].permission, users[working_user].username);
			perflag = 1;
			setcolor(5);
			printf("Now %s can access to your directories and files !!!\n", users[i].name);
			setcolor(15);
			_chdir(exePath);
			file_write(users, user_n);
			_chdir(users[working_user].username);
		}
	}
	if (!perflag)
    {
        setcolor(5);
		printf(" This user does not exist!!!\n");
		setcolor(15);
    }
}
void su(int user_n)
{
	_chdir(exePath);
	usersfile = fopen("usersfile.txt", "r");
		fread(users, sizeof(user), user_n, usersfile);
	fclose(usersfile);
	char usernamebuff[51];
	char pass2buff[51];
	setcolor(3);
	printf("please enter the username:\n");
	setcolor(15);
	scanf("%s", usernamebuff);
	suflag = 0;
	for (int i = 0; i < user_n; i++)
	{
		if (!strcmp(usernamebuff, users[i].username))
		{
			suflag = 1;
			if (dnow < users[i].time)
			{
				if (users[working_user].isadmin)
				{
					working_user = i;
					setcolor(5);
					printf("you changed the user successfully!!!\n");
					setcolor(15);
					_chdir(exePath);
					_chdir(users[working_user].username);
					break;
				}
				else
				{
				    setcolor(3);
					printf("please enter this user's password: \n");
					setcolor(15);
					scanf("%s", pass2buff);
					if (!strcmp(pass2buff, users[i].pass))
					{
						working_user = i;
						setcolor(5);
						printf("you changed the user successfully!!!\n");
						setcolor(15);
						_chdir(exePath);
						_chdir(users[working_user].username);
						break;
					}
					else
					{
					    setcolor(5);
						printf("you have entered the wrong password! please try again with another command !\n");
                        setcolor(15);
						break;
					}
				}
			}
			else
			{
			    setcolor(5);
				printf("you can't change to this due to time limit!!!");
				setcolor(15);
				break;
			}
		}
	}
	if (!suflag)
	{
	    setcolor(5);
		printf("this user does not exist! please try again with another command !\n");
		setcolor(15);

	}
	_chdir(users[working_user].username);
}


void passwd(user users[], int user_n)
{
	int x;
	int pass2_security;
	char pass2[51], repass2[51], username2[51];
	if (users[working_user].isadmin)
	{
	loop4:
	    setcolor(3);
		printf(" you  can change your own password  by entering 1 or other users by entering 0!!!\n");
		setcolor(15);
		scanf("%d", &x);
		switch (x)
		{
		case 1:
		    setcolor(3);
			printf("please enter the new password:\n");
			setcolor(15);
			scanf("%s", pass2);
			pass2_security = security_check(pass2);
			if (pass2_security > 33)
			{
			    setcolor(3);
				printf("please enter the new password again:\n");
				setcolor(15);
				scanf("%s", repass2);
				if (!strcmp(pass2, repass2))
				{
					strcpy(users[working_user].pass, pass2);
					users[working_user].pass_s = pass2_security;
					setcolor(5);
					printf("password changed successfully!!!\n");
					setcolor(15);
					_chdir(exePath);
					file_write(users, user_n);
					_chdir(users[working_user].username);

				}

				else
				{
				    setcolor(5);
					printf(" passwords are not match!!!try again with another command!!!\n");
					setcolor(15);
				}
			}
			else
            {
                setcolor(5);
				printf("password is not strong enough please try again with another command !\n");
				setcolor(15);
            }

			break;

		case 0:
		    setcolor(3);
			printf("please enter the username that you want to change the password : \n");
			setcolor(15);
			scanf("%s", username2);
			for (int i = 0; i < user_n; i++)
			{
				if (!strcmp(username2, users[i].username))
				{
					passflag = 1;
					setcolor(3);
					printf("please enter the new password:\n");
					setcolor(15);
					scanf("%s", pass2);
					pass2_security = security_check(pass2);
					if (pass2_security > 33)
					{
					    setcolor(3);
						printf("please enter the new password again:\n");
						setcolor(15);
						scanf("%s", repass2);
						if (!strcmp(pass2, repass2))
						{
							strcpy(users[i].pass, pass2);
							users[i].pass_s = pass2_security;
							setcolor(5);
							printf("password changed successfully!!!\n");
							setcolor(15);
							_chdir(exePath);
							file_write(users, user_n);
							_chdir(users[working_user].username);
							break;
						}

						else
						{
						    setcolor(5);
							printf("passwords are not match!!!try again with another command!!!\n");
							setcolor(15);
							break;
						}
					}
					else
					{
					    setcolor(3);
						printf("password is not strong enough! please try again with another command !!!\n");
						setcolor(15);
						break;
					}
				}
			}
			if (!passflag)
            {
				setcolor(5);
				printf("this user does not exist please try again with another command !!!\n");
                setcolor(15);
            }
			break;
		default:
			char wrong_input[101];
			setcolor(5);
			printf("Wrong input!! try a valid number :\n");
			setcolor(15);
			gets(wrong_input);
			goto loop4;
		}



	}

	else

	{
        setcolor(3);
		printf("please enter the new password:\n");
		setcolor(15);
		scanf("%s", pass2);
		pass2_security = security_check(pass2);
		if (pass2_security > 33)
		{
		    setcolor(3);
			printf("please enter the new password again:\n");
			setcolor(15);
			scanf("%s", repass2);
			if (!strcmp(pass2, repass2))
			{
				strcpy(users[working_user].pass, pass2);
				users[working_user].pass_s = pass2_security;
				setcolor(5);
				printf("password changed successfully!!!\n");
				setcolor(15);
				_chdir(exePath);
				file_write(users, user_n);
				_chdir(users[working_user].username);
			}

			else
			{
			    setcolor(5);
				printf("passwords are not match!!!try again with another command!!!\n");
				setcolor(15);
			}
		}
		else
			{
			    setcolor(5);
			    printf("password is not strong enough please try again with another command !\n");
			    setcolor(15);
			}
	}
}
void passw_l(user users[], int user_n)
{
	char username2[51];
	int time2;
	if (!users[working_user].isadmin)
	{
	    setcolor(5);
		printf("You are not admin you can't change time limit !\n");
		setcolor(15);
		users[working_user].wrong++;
	}
	else
	{
	    setcolor(3);
		printf("please enter the username that you want to change time limit: \n");
		setcolor(15);
		scanf("%s", username2);
		for (int i = 0; i < user_n; i++)
		{
			if (!strcmp(username2, users[i].username))
			{
				tflag = 1;
				setcolor(3);
				printf("enter the time limit like this yyyymmdd(example: 21001227 = 2100/12/27)\n");
				setcolor(15);
				scanf("%d", &time2);
				users[i].time = time2;
				setcolor(5);
				printf("time limit changed successfully!!!\n");
				setcolor(15);
				_chdir(exePath);
				file_write(users, user_n);
				_chdir(users[working_user].username);
				break;
			}
		}
		if (!tflag)
        {
            setcolor(5);
			printf("this user does not exist please try again with another command !!!\n");
			setcolor(15);
        }
	}
}
void chgr(user users[], int user_n)
{
	_chdir(exePath);
	usersfile = fopen("usersfile.txt", "r");
	fread(users, sizeof(user), user_n, usersfile);
	fclose(usersfile);
	char username2[51];
	if (!users[working_user].isadmin)
	{
	    setcolor(5);
		printf("You are not admin you can't change group !\n");
		setcolor(15);
		users[working_user].wrong++;
	}
	else
	{
	    setcolor(3);
		printf("please enter the username that you want to change group : \n");
		setcolor(15);
		scanf("%s", username2);
		for (int i = 0; i < user_n; i++)
		{
			if (!strcmp(username2, users[i].username))
			{
				chflag = 1;
				if (users[i].isadmin) {setcolor(5); printf("this user is an admin you can't change group !\n"); setcolor(15);}
				else
				{
					if (users[i].pass_s > 75 && users[i].wrong < 11)
					{
						users[i].isadmin = 1;
						setcolor(5);
						printf("group changed successfully!!!\n");
						setcolor(15);
						_chdir(exePath);
						file_write(users, user_n);
						_chdir(users[working_user].username);
						break;
					}
					else
						{setcolor(5);printf("group can't be changed!!!\n");setcolor(15);}
				}
			}
		}
		if (!chflag)
			{setcolor(5);printf("this user does not exist please try again with another command !!!\n");setcolor(15);}
	}

	_chdir(users[working_user].username);
}

void pwd(void)
{

	char buff[51];
	_getcwd(buff, 51);
	setcolor(95);
	printf("current directory is: %s\n", buff);
	setcolor(15);
}

void cd(user users[])
{
	char username2[51];
	setcolor(3);
	printf("please enter the wanted directory name:\n");
	setcolor(15);
	scanf("%s", username2);
	if (!strcmp(username2, ".."))
	{
		_chdir("..");
		setcolor(5);
		printf("directory changed successfully !!!\n");
		setcolor(15);
	}
	else
	{
		if (!users[working_user].isadmin)
		{
			char buff[51];
			_getcwd(buff, 51);
			if (!strcmp(buff, exePath))
			{
				if (!strcmp(username2, users[working_user].username) || strstr(users[working_user].permission, username2))
				{
					if (_chdir(username2) != 0)

						{setcolor(5);printf("this directory does not exist !!!please try again with another command!!!\n");setcolor(15);}

					else

						{setcolor(5);printf("directory changed successfully !!!\n");setcolor(15);}

				}
				else

				{
				    setcolor(5);
					printf("you don't have the permission to enter the directory !!!\n");
					setcolor(15);
				}

			}
			else
			{
				if (_chdir(username2) != 0)

				{setcolor(5);	printf("this directory does not exist !!!please try again with another command!!!\n");setcolor(15);}

				else


					{setcolor(5);printf("directory changed successfully !!!\n");setcolor(15);}

			}
		}

		else
		{
			if (_chdir(username2) != 0)

				{setcolor(5);printf("this directory does not exist !!!please try again with another command!!!\n");setcolor(15);}

			else
				{setcolor(5);printf("directory changed successfully !!!\n");setcolor(15);}

		}
	}
}
void help(void)
{
	setcolor(12);
	char help[51];
	puts("For more info on a specific command ,enter command name else enter no!!\n");
	puts("su:     will change the current user to another ");
	puts("createuser:    will create new user and gets his/her info");
	puts("passwd:    will change yours or others password");
	puts("passwd-l:    will change other users time limit ");
	puts("permission:    will alow other users to use the current users folder ");
	puts("chgr:    will makes a normal user an admin");
	puts("cd:    will change current directory!!!");
	puts("pwd:    will show the current addres");
	puts("mkdir:    will make a new directory");
	puts(">:    will write a text or a file on another");
	puts(">>:    will append a text or a file on another");
	puts("cat:    will read a file");
	puts("rm:    will remove a file");
	puts("rm-r:    will remove a folder");
	puts("cp:     will copy a file or folder to another folder");
	puts("mv:    will move a file or folder to another folder");
	puts("ls:    will list all the files and folders in the current directory");
	puts("exif:   will show the files info");
	puts("wc:    will show the number of words and lines in a file");
	puts("diff:    will compare two files");
	puts("help:   will help you(if you search help you dont have the IQ required to use this shell :/)");
	setcolor(15);
	scanf("%s", help);
	setcolor(12);
	if (!strcmp(help, "su"))
		puts("first enter su then I will ask of a username then enter one if you are a admin the user will be changed else you have to enter the users pass");
	else if (!strcmp(help, "createuser"))
		puts("only an admin is alowd to use this command first you have to enter the new users username then i will ask if its unlimited or not then you can give it some permission then enter the pass two times ");
	else if (!strcmp(help, "passwd"))
		puts("first i will ask if you want to change your own pass or someone else then enter the pass two times");
	else if (!strcmp(help, "passwd-l"))
		puts("first enter the username then the new time limit ");
	else if (!strcmp(help, "permission"))
		puts("enter the users username that you are going to alow to enter your folder");
	else if (!strcmp(help, "chgr"))
		puts("only an admin can use this command on a normal user with more pass stregth than 75  and less misstakes than 10");
	else if (!strcmp(help, "cd"))
		puts("first we will ask you the wanted directory enter it p.s:(..)means back");
	else if (!strcmp(help, "mkdir"))
		puts("I will ask you to enter the folders name");
	else if (!strcmp(help, ">"))
		puts("enter like this example(text/file > file)");
	else if (!strcmp(help, ">>"))
		puts("enter like this example(text/file >> file)");
	else if (!strcmp(help, "cp"))
		puts("I will ask the file/folders name then the destination");
	else if (!strcmp(help, "mv"))
		puts("I will ask you the file/folders name then the destination");
	else if (!strcmp(help, "ls"))
		puts("no needed info");
	else if (!strcmp(help, "exif"))
		puts("");
	else if (!strcmp(help, "cat"))
		puts("I will ask you the name of the file");
	else if (!strcmp(help, "rm"))
		puts("I will ask you the name of the file");
	else if (!strcmp(help, "rm-r"))
		puts("I will ask you the name of the folder");
	else if (!strcmp(help, "wc"))
		puts("I will ask you the name of the file");
	else if (!strcmp(help, "diff"))
		puts("i will ask you the name of twe files ");
	else if (!strcmp(help, "help"))
		puts("WOW!!!!");
	else if (!strcmp(help, "EXIF"))
		puts("IDK!!!");
	else
		puts("wrong input try the command again!!!");
	setcolor(15);
}

void mkdir(void)
{
	char username2[51];
	setcolor(3);
	printf("please enter the directory name to create:\n");
	setcolor(15);
	scanf("%s", username2);
	_mkdir(username2);
	setcolor(5);
	printf("directory has been made successfully !!!\n");
	setcolor(15);
}

void cat(void)
{
	char username2[51];
	setcolor(3);
	printf("please enter the file name to show:\n");
	setcolor(15);
	scanf("%s", username2);
	char str[51];
	strcpy(str, "type ");
	strcpy(str + 5, username2);
	setcolor(14);
	system(str);
	setcolor(15);
	printf("\n");
}

void rm(void)
{
	char username2[51];
	setcolor(3);
	printf("please enter the file name to remove:\n");
	setcolor(15);
	scanf("%s", username2);

	if (remove(username2))
		{setcolor(5);printf("file does not exist!!!\n");setcolor(15);}
	else
		{setcolor(5);printf("file deleted successfully!!!\n");setcolor(15);}
}

void rm_r(void)
{
	char username2[51];
	setcolor(3);
	printf("please enter the directory name to remove:\n");
	setcolor(15);
	scanf("%s", username2);
	if (_rmdir(username2))
		{setcolor(5);printf("folder does not exist!!!\n");setcolor(15);}
	else
		{setcolor(5);printf("folder deleted successfully!!!\n");setcolor(15);}
}
void cp(void)
{
	char des[51], username2[51];
	setcolor(3);
	printf("enter file name to copy:\n");
	setcolor(15);
	char input[51];
	scanf("%s", username2);
	setcolor(3);
	printf("enter destination\n");
	setcolor(15);
	scanf("%s", des);
	strcat(username2, " ");
	strcat(username2, des);
	strcpy(input, "copy ");
	setcolor(5);
	system(strcat(input, username2));
	setcolor(15);
}

void mv(void)
{
	char des[51], username2[51];
	setcolor(3);
	printf("enter file name to move\n");
	setcolor(15);
	char input[51];
	scanf("%s", username2);
	setcolor(3);
	printf("enter destination\n");
	setcolor(15);
	scanf("%s", des);
	strcat(username2, " ");
	strcat(username2, des);
	strcpy(input, "move ");
	setcolor(5);
	system(strcat(input, username2));
	setcolor(15);
}
void ls(void)
{
    setcolor(13);
	DIR* d;
	struct dirent* dir;
	d = opendir(".");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			printf("%s\n", dir->d_name);
		}
		closedir(d);
	}
	else
		{setcolor(5);printf("folder is empty!!\n");setcolor(15);}
		setcolor(15);
}

void wc(void)
{
	char filename[51];
	setcolor(3);
	printf("please enter the file name to be count:\n");
	setcolor(15);
	scanf("%s", filename);
	int bytes = 0;
	int words = 0;
	int newLine = 1;
	char buffer[1];
	enum states { WHITESPACE, WORD };
	int state = WHITESPACE;
	FILE* file = fopen(filename, "r");

	if (file == NULL)
	 {
            setcolor(5);
		printf("can not find :%s\n", filename);
    setcolor(15);
		return;
	}
	else {
		char* thefile = filename;
		char last = ' ';
		while (_read(_fileno(file), buffer, 1) == 1)
		{
			bytes++;
			if (buffer[0] == ' ' || buffer[0] == '\t')
			{
				state = WHITESPACE;
			}
			else if (buffer[0] == '\n')
			{
				newLine++;
				state = WHITESPACE;
			}
			else
			{
				if (state == WHITESPACE)
				{
					words++;
				}
				state = WORD;
			}
			last = buffer[0];
		}
		setcolor(13);
		printf("number of lines : %d\nnumber of words: %d\nnumber of bytes: %d\n", newLine, words, bytes);
		setcolor(15);
	}
}

void diff(void)
{
	char file1[51], file2[2];
	setcolor(3);
	printf("Enter first file :\n");
	setcolor(15);
	scanf("%s", file1);
	setcolor(3);
	printf("Enter second file :\n");
	setcolor(15);
	scanf("%s", file2);
	FILE* fp1 = fopen(file1, "r");

	FILE* fp2 = fopen(file2, "r");


	if (fp1 == NULL || fp2 == NULL)

	{
        setcolor(5);
		printf("Error : Files not open!\n");
		setcolor(15);
		return;

	}
	compareFiles(fp1, fp2);

}

void writeopperand(char command[])
{
	char comm[51], comm2[51];
	strcpy(comm, "type ");
	strcat(comm, command);
	setcolor(15);
	scanf("%s", comm2);
	test = fopen(command, "r");
	if (test == NULL)
	{
		test2 = fopen(comm2, "w");
		fprintf(test2, "%s\n", command);
		fclose(test2);
		setcolor(5);
		printf("written successfully!!!\n");
		setcolor(15);
	}
	else
	{
		fclose(test);
		strcat(comm, ">");
		system(strcat(comm, comm2));
		setcolor(5);
		printf("written successfully!!!\n");
		setcolor(15);
	}
}
void appendopperand(char command[])
{
	char comm[51], comm2[51];
	strcpy(comm, "type ");
	strcat(comm, command);
	setcolor(15);
	scanf("%s", comm2);
	test = fopen(command, "r");
	if (test == NULL)
	{
		test2 = fopen(comm2, "a");
		fprintf(test2, "%s\n", command);
		fclose(test2);
		setcolor(5);
		printf("written successfully!!!\n");
		setcolor(15);
	}
	else
	{
		fclose(test);
		strcat(comm, ">>");
		system(strcat(comm, comm2));
		setcolor(5);
		printf("written successfully!!!\n");
		setcolor(15);
	}
}
void hiddenplus(void)
{
	char username2[51];
	if (users[working_user].isadmin)
	{
	    setcolor(3);
		printf("enter a file name to create and hide:\n");
		setcolor(15);
		char t[51];
		FILE* ptr;
		scanf("%s", username2);
		ptr = fopen(username2, "r");
		if (ptr == NULL)
		{
			ptr = fopen(username2, "w");
			fclose(ptr);
		}
		fclose(ptr);
		sprintf(t, "attrib +h %s", username2);
		printf("\n");
		setcolor(5);
		system(t);
		printf("\n");
		setcolor(15);

	}
	else
	{
	    setcolor(5);
		printf("this command is nit alowded for you\n");
		setcolor(15);
	}
}
void hiddenmin(void)
{
	char username2[51];
	if (users[working_user].isadmin)
	{
	    setcolor(3);
		printf("enter a file name to unhide:\n");
		setcolor(15);
		char t[51];

		scanf("%s", username2);
		sprintf(t, "attrib -h %s", username2);
		printf("\n");
		setcolor(5);
		system(t);
		printf("\n");
		setcolor(15);

	}
	else
	{
	    setcolor(5);
		printf("this command is not allowed for you\n");
		setcolor(15);
	}
}
void searchfile(void)
{
	char username2[51];
	setcolor(5);
	printf("enter file's name to search:\n");
	setcolor(15);
	scanf("%s", username2);
	char q[51];
	sprintf(q, "dir /s /b %s", username2);
	printf("\n");
	setcolor(5);
	system(q);
	printf("\n");
	setcolor(15);
}
