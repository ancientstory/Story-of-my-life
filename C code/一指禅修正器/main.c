#include"tou.h"
int main()
{
	char typer[100],circle;
	int line,word_right=0,type=0,degree;
	int sum, sum_true;
	int old_time, new_time,sum_time;
	float accuracy;
	printf("#####打字程序######\n");
	//账户登录
	while (1)
	{
		if (account_login())
			break;
		else
			puts("账号或密码错误请重新输入");
	}
	do 
	{
		//初始化阶段
		printf("选择打字内容来源\n");
		printf("1.随机生成        2.导入\n");
		do{
		scanf("%d", &type);
		getchar();
		} while (type != 1 && type != 2);
		sum = 0; sum_true = 0;
		//运行阶段
		if (type == 1)
		{
			//选择模式
			printf("选择每行单词数量：\n");
			printf("1.低(5)   2.中（10）    3.高（15）\n");
			scanf("%d", &degree);
			switch (degree) {
			case 1:
				LENGTH = 5;
				break;
			case 2:
				LENGTH = 10;
				break;
			case 3:
				LENGTH = 15;
				break;
			default:
				printf("===========困难=========\n");
				LENGTH = 40;
				break;
			}
			old_time = time(NULL);
			srand((unsigned)time(NULL));//初始化种子
			char origin[11];
			for (line = 0; line < LENGTH; line++)
			{
				//初始化origin
				init_origin(origin);
				//输出origin
				printf("你需要输入的样本是:");
				puts(origin);
				//用户输入阶段
				printf("请输入:");
				while (1)
				{
					scanf("%s", typer);
					getchar();
					//检测输入是否过长
					if (strlen(typer) > LENGTH)
					{
						printf("length error!\n");
						memset(typer, 0, sizeof(typer));//重置typer
					}
					else
						break;
				}
				//正确率汇总
				sum_true += test_true(origin, typer);
				sum += strlen(origin);
			}
			new_time = time(NULL);
		}
		else
		{
			//获得自定义的单词
			char buff[255];
			FILE* fp;
			fp = fopen("file.txt", "r");
			while(!feof(fp))
			{
				fgets(buff, 255, fp);
				if(buff[strlen(buff)-1]=='\n')
					buff[strlen(buff) - 1] = '\0';
				printf("你需要输入的样本是:");
				printf("%s\n", buff);
				//计时开始
				old_time = time(NULL);
				//用户输入阶段
				printf("请输入:");
				while (1)
				{

					gets_s(typer);
					//检测输入是否过长
					if (strlen(typer) > strlen(buff))
					{
						printf("length error!\n");
						memset(typer, 0, sizeof(typer));//重置typer
					}
					else
						break;
				}
				//正确率汇总
				sum_true += test_true(buff, typer);
				sum += strlen(buff);
			}
			//计时结束
			new_time = time(NULL);
			fclose(fp);
		}

		accuracy = 100 * (float)sum_true / sum;
		printf("你的正确率是%.2f%%\n", accuracy);
		sum_time = new_time - old_time;
		printf("本次成绩：%.3f word/min\n", sum_true * 60 / (float)sum_time);
		//printf("历史最好成绩：%.f word/min\n", record(sum * 60 / (float)sum_time));

		printf("再来一次？\n");
		Sleep(500);
		printf("按\"q\"推出，其他键重新开始\n");
		scanf("%c", &circle);
	} while (circle != 'q');
	return 0;
}
