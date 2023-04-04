int LENGTH = 10;
#define _CRT_SECURE_NO_WARNINGS
#include<time.h>
#include<windows.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

int test_true(char* s, char* t)
{
	int n=0;
	for (int i = 0; s[i] != '\0' && t[i] != '\0'; i++)
		if (s[i] == t[i])
			n++;
	return n;
}
void init_origin(char* origin)
{
	
	for (int i = 0; i < LENGTH; i++)
	{
		if (rand() % 2)
			origin[i] = 'a' + rand() % 26;
		else
			origin[i] = 'A' + rand() % 26;
	}
	origin[LENGTH] = '\0';

}
struct account{
		char id[11];
		char pwd[20];
		char name[15];
		float mark=0.0;
		int rank;
};
int registers(void)
{
	struct account a, b;
	char temp_pwd[20];
	int count;
	FILE* fp = fopen("user.txt", "r");
	puts("欢迎来到注册界面");
	Sleep(1000);
	//读入第一个结构体字符块，到b
	fread(&b, sizeof(struct account), 1, fp);
	//注册账户
	puts("请输入账户");
	scanf("%s", &a.id);
	//判断账户是否已存在
	while (1)
	{
		if (strcmp(a.id, b.id)){//账户存在
			if (!feof(fp))
				fread(&b, sizeof(struct account), 1, fp);
			else break;
		}
		else {
			printf("此用户名已存在！请重新注册\n");
			Sleep(500);
			fclose(fp);
			return 0;
		}
	}
	printf("请输入昵称：\n");
	scanf("%s", &a.name);
	getchar();
	printf("请输入密码：\n");
	scanf("%s", &a.pwd);
	printf("请确认密码：\n");
	scanf("%s", temp_pwd);
	while (1)
	{
		if (!strcmp(a.pwd, temp_pwd)) {
			fp = fopen("user.txt", "a");
			fwrite(&a, sizeof(struct account), 1, fp);
			printf("账号注册成功，请登录！\n");
			Sleep(500);
			fclose(fp);
			return 1;
		}
		else {
			printf("两次密码不匹配！请重新输入！\n");
			scanf("%s", &a.pwd);
			printf("请确认密码\n");
			scanf("%s", &temp_pwd);
		}
	}
}
int account_login(void)
{
	//创建账户文件夹
	FILE* fp;
	fp = fopen("user.txt", "a+");
	fclose(fp);
	//注册账户
	printf("是否已有账户：\n");
	printf("1.有     2.无\n");
	int judge;
	do {
		scanf("%d", &judge);
		getchar();
	} while (judge != 1 && judge != 2);
	if (judge == 2)
		while (!registers());//如果注册不成功则返回0，就会再次注册；
	else
		;

	//登录
	struct account a, b;
	printf("欢迎来到登录界面\n");
	Sleep(200);
	fp = fopen("user.txt", "r");

	fread(&b, sizeof(struct account), 1, fp); //读入一个结构体字符块 写入b
	printf("请输入账号\n");
	scanf("%s", &a.id);
	while (1)
	{
		if (!strcmp(a.id, b.id))//账户名存在
		{
			break;
		}
		else
		{
			if (!feof(fp))
				fread(&b, sizeof(struct account), 1, fp);
			else {
				printf("账户不存在,请重新输入");
				Sleep(500);
				fclose(fp);
				return 0;
			}
		}
	}
	//密码
	printf("请输入密码\n");
	scanf("%s", &a.pwd);
	while (strcmp(a.pwd, b.pwd))//密码不对
	{
		printf("密码不正确！请重新输入\n");
		scanf("%s", &a.pwd);
	}
	fclose(fp);
	printf("登陆成功，欢迎使用！\n");
	Sleep(500);
	return 1;
}
