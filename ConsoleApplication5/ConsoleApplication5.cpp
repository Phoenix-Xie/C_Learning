// ConsoleApplication5.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)
#define fileName "Student.dat"
#define LEN sizeof(struct Student)
#define false 0;
#define true 1;
#define configFile "Config.dat"
struct Student
{
	int Number;
	char Name[10];
	char Sex[3];
	char PhoneNumber[12];
	float CScore;
	float MathScore;
	float ForeignSocre;
	float AverageScore;
	int Rank;  //排名
	float MutualScore; //同学互评分
	float VirtueScore; //品德评分
	float TeacherScore;
	float TotalScore;
}globalstu[10000],globalstu2[10000];

void quicksort(struct Student stu[], int p, int r, int mode); //快排主函数， 参数：所需排序的Student结构体，p r 为排序的左右区间 （左右闭区间） mode = 1按学号 mode = 2 按分数排序
int partition(struct Student stu[], int p, int r, int mode); //快排分类函数 参数：所需排序的Student结构体，p r 为排序的左右区间 （左右闭区间） mode = 1按学号 mode = 2 按分数排序
int write(FILE * fp, struct Student stu); //写入文件函数,返回-1写入失败 参数：fp 所写入文件的指针 所需写入的Student结构体内容
int Test(struct Student stu); //检测是否为正常分数，参数: Student结构体 
int addNewStudentImformation(); //增加数据主函数
int SortInOrderOfNumber(); //按学号顺序排序文件内容
int CountRank(); //计算名次
int ReadAll(); //读取所有学生内容用于排序,正常返回正确读取数，失败返回false
int disPlayInfo();
int CountData(); //计算当前数据量
int ReadConfig();//读取配置文件,显示数据条数
int NewConfig();//更新或新建记录数据条数的配置文件
int changeSomeInfo();//批量按学号修改信息;
int seekInglobal(int Number, int c);//寻找globalstu数组中的Number信息 参数 Number 学号 c 数组当前长度
int seek(FILE *fp, int length, int text);
int turnToInt(char s[]);
int changeInfo();
int fDeleteInfo();
int fSearch();
int disPlayInfo();
int cleanData(); //清空数据文件
int Match(char Str[], char str[]); //在Str 中匹配str字符串  由于模糊搜索
int checkRepeat();//检测是否有重复


int main()
{
	int operate;
	while (1)
	{
		system("color 70");
		system("cls");
		printf("------------学生综合测评系统----------------\n1.添加学生信息\n2.修改学生信息\n3.删除学生信息\n4.浏览学生信息\n5.计算数据条数\n6.显示数据条数\n7.删除所有学生信息\n8.学生数据查询\n9.检查重复\n10.批量修改学生信息\n0.结束程序\n--------------------------------------------\n请输入操作选项:");
		scanf("%d", &operate);
		switch (operate)
		{
		case 1:addNewStudentImformation(); break;
		case 2:changeInfo(); break;
		case 3:fDeleteInfo(); break;
		case 4:disPlayInfo(); break;
		case 5:NewConfig(); break;
		case 6:printf("当前数据条数为%d条\n", ReadConfig()); getchar(); getchar(); break;
		case 7:cleanData(); break;	
		case 8:fSearch(); break;
		case 9:checkRepeat(); break;
		case 10:changeSomeInfo(); break;
		case 0:exit(0);
		default:system("cls"); printf("指令有误,请重新输入\n");
			break;
		}
	}
	getchar();
}

int write(FILE *fp, struct Student stu)
{
	if (fwrite(&stu, sizeof(stu), 1, fp) == 0)
	{
		return false;
	}
	return true;
}

int addNewStudentImformation()
{
	struct Student stu;
	printf("请按顺序输入：学号、姓名、性别、联系电话、C语言、高等数学、外语三门单科成绩、\n同学互评分、品德成绩、任课教师评分\n 若成绩未登入，可先输入0,之后修改\n学号输入0结束\n");
	FILE * fp = fopen(fileName, "ab");
	while (1)
	{
		scanf("%d", &stu.Number);
		if (stu.Number == 0)
		{
			break;
		}
		scanf("%s%s%s%f%f%f%f%f%f", stu.Name, stu.Sex, stu.PhoneNumber, &stu.CScore, &stu.MathScore, &stu.ForeignSocre, &stu.MutualScore, &stu.VirtueScore, &stu.TeacherScore);
		if (!Test(stu))
		{
			printf("该学生学号为%d", stu.Number);
			continue;
		}		
		stu.AverageScore = (stu.CScore + stu.MathScore + stu.ForeignSocre) / 3;
		stu.TotalScore = stu.AverageScore*0.6 + stu.MutualScore*0.1 + stu.VirtueScore*0.1 + stu.TeacherScore*0.2;
		stu.Rank = 0; /*初始化为0*/

		if (!write(fp, stu))
		{
			return false;
		}
		
		CountData();
	}
	fclose(fp);
	CountRank();
	checkRepeat();
	return true;

}

int ReadAll()
{
	FILE *fp;
	int i = 0;
	if ((fp = fopen(fileName, "rb")) == NULL)
	{
		printf("文件排序读取打开错误\n");
		return false;
	}
	while (1)
	{
		if (fread(&globalstu[i], LEN, 1, fp) == 0)
			break;
		else
		{
			i++;
		}
	}
	fclose(fp);
	return i;
}

int Test(struct Student stu)
{
	if (!(stu.CScore >= 0 && stu.CScore <= 100))
	{
		printf("C程数据输入错误,未录入，请检查\n");
		return false;
	}
	if (!(stu.MathScore >= 0 && stu.MathScore <= 100))
	{
		printf("数学成绩错误,未录入，请检查\n");
		return false;
	}
	if (!(stu.ForeignSocre >= 0 && stu.ForeignSocre <= 100))
	{
		printf("外语成绩错误,未录入，请检查\n");
		return false;
	}
	if (!(stu.VirtueScore >= 0 && stu.VirtueScore <= 100))
	{
		printf("外语成绩错误,未录入，请检查\n");
		return false;
	}
	if (!(stu.TeacherScore >= 0 && stu.TeacherScore <= 100))
	{
		printf("外语成绩错误,未录入，请检查\n");
		return false;
	}
	if (!(stu.MutualScore >= 0 && stu.MutualScore <= 100))
	{
		printf("外语成绩错误,未录入，请检查\n");
		return false;
	}
	if (!(strcmp(stu.Sex, "男") == 0 || strcmp(stu.Sex, "女") == 0))
	{
		printf("性别输入错误，未录入，请检查\n");
		return false;
	}
	return true;
}

void quicksort(struct Student stu[], int p, int r, int mode)
{
	if (p < r)
	{
		int mid = partition(stu, p, r, mode);
		quicksort(stu, p, mid - 1, mode);
		quicksort(stu, mid + 1, r, mode);
	}
}

int partition(struct Student stu[], int p, int r, int mode)
{
	if (mode == 1)
	{
		srand(time(NULL));
		int Randi = rand() % (r - p + 1);
		struct Student Randt = stu[r];
		stu[r] = stu[Randi + p];
		stu[Randi + p] = Randt;
		//随机化
		int key = stu[r].Number;
		struct Student t;
		int i = p, j = p;
		for (j = p; j < r; j++)
		{
			if (stu[j].Number < key)
			{
				t = stu[i];
				stu[i] = stu[j];
				stu[j] = t;
				i++;
			}
		}
		t = stu[i];
		stu[i] = stu[r];
		stu[r] = t;
		return i;
	}
	if (mode == 2)
	{
		srand(time(NULL));
		int Randi = rand() % (r - p + 1);
		struct Student Randt = stu[r];
		stu[r] = stu[Randi + p];
		stu[Randi + p] = Randt;
		//随机化
		//int key = stu[r].AverageScore;
		float key = stu[r].AverageScore;
		struct Student t;
		int i = p, j = p;
		for (j = p; j < r; j++)
		{
			if (stu[j].AverageScore < key)
			{
				t = stu[i];
				stu[i] = stu[j];
				stu[j] = t;
				i++;
			}
		}
		t = stu[i];
		stu[i] = stu[r];
		stu[r] = t;
		return i;
	}
}

int SortInOrderOfNumber()
{

	int i;
	int c = ReadAll();
	FILE *fp;
	fp = fopen(fileName, "wb");
	quicksort(globalstu, 0, c - 1, 1); //快排左右闭区间
	for (i = 0; i < c; i++)
	{
		if (!write(fp, globalstu[i]))
		{
			printf("排序写入失败\n");
			return false;
		}
	}
	fclose(fp);
	return true;
}

int CountRank()
{
	int c = ReadAll();
	int i;
	quicksort(globalstu, 0, c - 1, 2);
	for (i = c; i >= 1; i--)
	{
		globalstu[c - i].Rank = i;
	}
	quicksort(globalstu, 0, c - 1, 1);
	//checkRepeat();
	FILE * fp = fopen(fileName, "wb");
	for (i = 0; i < c; i++)
	{
		if (!write(fp, globalstu[i]))
		{
			printf("名次计算写入失败\n");
			return false;
		}
	}
	fclose(fp);
	return true;
}


int CountData()
{
	FILE * fp;
	if ((fp = fopen(configFile, "rb")) == NULL)
	{
		printf("配置文件打开错误\n");
		return false;
	}
	int n;
	if (fread(&n, sizeof(int), 1, fp) == 0)
	{
		printf("配置信息读取错误\n");
		return false;
	}
	n++;
	fclose(fp);
	if ((fp = fopen(configFile, "wb")) == NULL)
	{
		printf("配置文件打开错误\n");
		return false;
	}
	if (fwrite(&n, sizeof(int), 1, fp) == 0)
	{
		printf("配置信息写入错误\n");
		return false;
	}
	fclose(fp);
	return true;
}

int ReadConfig()
{
	FILE * fp;
	if ((fp = fopen(configFile, "rb")) == NULL)
	{
		printf("配置文件打开错误,有可能配置文件不存在\n");
		return false;
	}
	int n;
	if (fread(&n, sizeof(int), 1, fp) == 0)
	{
		printf("配置信息读取错误\n");
		return false;
	}
	//printf("当前数据量为%d条\n", n);
	return n;
}

int NewConfig()
{
	FILE * fp;
	int n = 0;
	n = ReadAll();
	if ((fp = fopen(configFile, "wb")) == NULL)
	{
		printf("配置文件生成错误\n");
		return false;
	}
	if (fwrite(&n, sizeof(int), 1, fp) == 0)
	{
		printf("配置信息写入错误\n");
		return false;
	}
	fclose(fp);
}

int changeSomeInfo()
{
	struct Student stu;
	strcpy(stu.Sex, "男");
	printf("学号， C语言、高等数学、外语三门单科成绩、同学互评分、品德成绩、任课教师评分\n学号输入0结束\n");
	int c = ReadAll();
	FILE * fp = fopen(fileName, "wb");
	while (1)
	{
		scanf("%d", &stu.Number);
		if (stu.Number == 0)
		{
			break;
		}
		scanf("%f%f%f%f%f%f",&stu.CScore, &stu.MathScore, &stu.ForeignSocre, &stu.MutualScore, &stu.VirtueScore, &stu.TeacherScore);
		if (!Test(stu))
		{
			printf("该学生学号为%d", stu.Number);
			continue;
		}
		int index = seekInglobal(stu.Number, c);
		if (index == -1)
		{
			printf("该学生不存在\n");
			continue;
		}
		globalstu[index].CScore = stu.CScore;
		globalstu[index].MathScore = stu.MathScore;
		globalstu[index].ForeignSocre = stu.ForeignSocre;
		globalstu[index].MutualScore = stu.	MutualScore;
		globalstu[index].VirtueScore = stu.VirtueScore;
		globalstu[index].TeacherScore = stu.TeacherScore;
		globalstu[index].AverageScore = (stu.CScore + stu.MathScore + stu.ForeignSocre) / 3;
		globalstu[index].TotalScore = globalstu[index].AverageScore*0.6 + stu.MutualScore*0.1 + stu.VirtueScore*0.1 + stu.TeacherScore*0.2;
		stu.Rank = 0; /*初始化为0*/
	}
	int i;
	for (i = 0; i < c; i++)
	{
		if (!write(fp, globalstu[i]))
		{
			return false;
		}
	}
	fclose(fp);
	CountRank();
	checkRepeat();
	return true;
	
}
int turnToInt(char s[])
{
	int i;
	int n = 0;
	for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
	{
		n = 10 * n + (s[i] - '0');
	}
	return n;
}
int changeInfo()
{
	FILE *fp, *fp2;
	int studentNum, check = 0, length = ReadConfig();
	struct Student temp, stu;
	printf("请输入要修改的学生的学号:");
	scanf("%d", &stu.Number);
	if (stu.Number == 0)
	{
		return 0;
	}
	if ((fp = fopen(fileName, "rb")) == NULL)
	{
		printf("文件不存在");
		return 0;
	}
	if ((fp2 = fopen("temp.dat", "wb")) == NULL)
	{
		printf("文件不存在");
		return 0;
	}
	if (seek(fp, length, stu.Number))
	{
		scanf("%s%s%s%f%f%f%f%f%f", &stu.Name, &stu.Sex, &stu.PhoneNumber, &stu.CScore, &stu.MathScore, &stu.ForeignSocre, &stu.MutualScore, &stu.VirtueScore, &stu.TeacherScore);
		if (!Test(stu))
		{
			printf("该学生学号为%d", stu.Number);
			return 0;
		}
		stu.AverageScore = (stu.CScore + stu.MathScore + stu.ForeignSocre) / 3;
		stu.TotalScore = stu.AverageScore*0.6 + stu.MutualScore*0.1 + stu.VirtueScore*0.1 + stu.TeacherScore*0.2;
		stu.Rank = 0; /*初始化为0*/
		rewind(fp);
		while (fread(&temp, LEN, 1, fp) != NULL)
		{
			if (temp.Number != stu.Number)
				fwrite(&temp, LEN, 1, fp2);
			else fwrite(&stu, LEN, 1, fp2);
		}
		printf("修改成功");
		fclose(fp);
		fclose(fp2);
		remove(fileName);
		rename("temp.dat", fileName);
		CountRank();
		printf("按enter键返回主界面\n");
		getchar();
		return 0;
	}
	else printf("没有找到该账号的信息,若要添加,请在主菜单选择添加学生信息\n");
	fclose(fp);
	fclose(fp2);
	printf("按enter键返回主界面\n");
	getchar();
	getchar();

}
int fDeleteInfo()
{
	FILE *fp, *fp2;
	int studentNum, check = 0, length;
	struct Student temp;
	printf("请输入要删除的学生的学号:");
	scanf("%d", &studentNum);
	if ((fp = fopen(fileName, "rb")) == NULL || (fp2 = fopen("temp.dat", "wb")) == NULL)
	{
		printf("文件不存在");
		return 0;
	}
	while (fread(&temp, LEN, 1, fp) != NULL)
	{
		if (temp.Number == studentNum)
		{
			check = 1;
			char operate;
			printf("%d %s %s %s %g %g %g %g %d %g %g %g %g\n", temp.Number, temp.Name, temp.Sex, temp.PhoneNumber, temp.CScore, temp.MathScore, temp.ForeignSocre, temp.AverageScore, temp.Rank, temp.MutualScore, temp.VirtueScore, temp.TeacherScore, temp.TotalScore);
			printf("请认真核对上面的信息,输入Y确定删除,否则取消");
			getchar();
			scanf("%c", &operate);
			if (operate != 'Y')
				fwrite(&temp, LEN, 1, fp2);
		}
		else fwrite(&temp, LEN, 1, fp2);
	}
	fclose(fp);
	fclose(fp2);
	remove(fileName);
	rename("temp.dat", fileName);
	if (check == 0)
		printf("未找到该学生信息.");
	else printf("操作成功!");
	getchar();
	getchar();
	return 0;
}
int fSearch()//学生数据查询函数
{
	int num, length = ReadConfig(), check = 1, numInfo = 0, searchNum;
	FILE *fp;
	char searchText[20];
	struct Student temp[999], info;
	if ((fp = fopen(fileName, "rb")) == NULL)
	{
		printf("File Error!");
		return 0;//返回0,文件错误
	}
	while (check)
	{
		printf("请选择要搜索的类型\n1.按学号搜索\n2.按姓名搜索\n3.按电话号码搜索\n4.按性别搜索\n0.返回主界面\n");
		scanf("%d", &num);
		switch (num)
		{
		case 1:
			printf("请输入要检索的学号:");
			scanf("%d", &searchNum);
			while (1)
			{
				if (seek(fp, length, searchNum))
				{
					fseek(fp, -1L * LEN, 1);
					fread(&temp[numInfo++], LEN, 1, fp);
				}
				break;
			}
			check = 0;
			break;
		case 2:
			printf("请输入要检索的姓名:");
			scanf("%s", searchText);
			while (fread(&info, LEN, 1, fp) != NULL)
			{
				if (Match(info.Name, searchText) != 0)
					temp[numInfo++] = info;
			}
			check = 0;
			break;
		case 3:
			printf("请输入要检索的电话号码:");
			scanf("%s", searchText);
			while (fread(&info, LEN, 1, fp) != NULL)
			{
				if (Match(info.Name, searchText) != 0)
					temp[numInfo++] = info;
			}
			check = 0;
			break;
		case 4:
			printf("请输入要检索的性别:");
			scanf("%s", searchText);
			while (fread(&info, LEN, 1, fp) != NULL)
			{
				if (strcmp(info.Sex, searchText) == 0)
					temp[numInfo++] = info;
			}
			check = 0;
			break;
		case 0:return 0;
		default:printf("指令输入错误!\n");
			break;

		}
	}
	printf("检索到如下%d个学生信息\n----------------------------------------------------\n", numInfo);
	printf("序号\t学号\t姓名\t性别\t电话\t\tC成绩\t\t数学成绩\t外语成绩\t平均分\t\t排名\t同学互评分\t品德评分\t教师评分\t总分\n");
	if (numInfo <= 10)
	{
		for (check = 0; check < numInfo; check++)
		{
			printf("%d.\t%d\t%s\t%s\t%s\t%f\t%f\t%f\t%f\t%d\t%f\t%f\t%f\t%f\n", check + 1, temp[check].Number, temp[check].Name, temp[check].Sex, temp[check].PhoneNumber, temp[check].CScore, temp[check].MathScore, temp[check].ForeignSocre, temp[check].AverageScore, temp[check].Rank, temp[check].MutualScore, temp[check].VirtueScore, temp[check].TeacherScore, temp[check].TotalScore);
		}
		printf("----------------------------------------------------\n");
	}
	else
	{
		int page = 1, allpage, numDisplay = 20;//allpage为总页数,numDisplay为每页显示的数据
		allpage = numInfo / numDisplay;
		if (numInfo % numDisplay != 0)//如果显示不完,则总页数加一
			allpage++;
		for (;;)
		{
			system("cls");
			if (page == 0)
				break;
			else if (page > allpage || page < 0)
			{
				page = 1;
				continue;
			}
			printf("序号\t学号\t姓名\t性别\t电话\t\tC成绩\t\t数学成绩\t外语成绩\t平均分\t\t排名\t同学互评分\t品德评分\t教师评分\t总分\n");
			for (check = (page - 1) * numDisplay; check < page * numDisplay&&check<numInfo; check++)
				printf("%d.\t%d\t%s\t%s\t%s\t%f\t%f\t%f\t%f\t%d\t%f\t%f\t%f\t%f\n", check + 1, temp[check].Number, temp[check].Name, temp[check].Sex, temp[check].PhoneNumber, temp[check].CScore, temp[check].MathScore, temp[check].ForeignSocre, temp[check].AverageScore, temp[check].Rank, temp[check].MutualScore, temp[check].VirtueScore, temp[check].TeacherScore, temp[check].TotalScore);
			printf("总共检索到%d个相关信息,一共%d页,当前%d页; 跳转至", numInfo, allpage, page);
			scanf("%d", &page);
		}
	}
	fclose(fp);
	getchar();
	getchar();
}
int seek(FILE *fp, int length, int text)//二分法查找函数,将FILE指针定位到 type为查找的类型,length为文件长度,text为查找的内容
{
	struct Student temp;
	int start = 0, end = length, mid = (start + end) / 2, check = 0;
	while (start <= end)
	{
		mid = (start + end) / 2;
		fseek(fp, mid*LEN, 0);
		fread(&temp, LEN, 1, fp);
		if (temp.Number == text)
		{
			check = 1;
			break;
		}
		//return 1;//返回1找到数据
		if (text > temp.Number)
			start = mid + 1;
		else
			end = mid - 1;
	}

	return check;//返回0未找到数据;

}

int seekInglobal(int Number, int c)
{
	int low = 0;
	int high = c - 1;
	int mid;
	while (high >= low) 
	{
		mid = (low + high) / 2;
		if (globalstu[mid].Number == Number)
		{
			return mid;
		}
		else if (globalstu[mid].Number > Number)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	return -1;
}
int disPlayInfo()
{
	FILE *fp;
	int num = 0, page = 1, which = 0, allpage, numDisplay = 20;//allpage为总页数,numDisplay为每页显示的数据,page为当前显示的页面,which是显示的行
	struct Student temp;
	system("cls");
	CountRank();
	if ((fp = fopen(fileName, "rb")) == NULL)
	{
		printf("打开文件时遇到错误!");
		return 0;
	}
	while (fread(&globalstu[num], LEN, 1, fp) != NULL)
		num++;
	if (num == 0)
	{
		printf("当前无任何信息,按下enter返回主界面\n");
		getchar();
		getchar();
		return -1;
	}
	allpage = num / numDisplay;
	if (num % numDisplay != 0)
		allpage++;
	for (;;)
	{
		system("cls");
		if (page == 0)
			break;
		else if (page > allpage || page < 0)
		{
			page = 1;
			continue;
		}
		printf("序号\t学号\t姓名\t性别\t电话\t\tC成绩\t\t数学成绩\t外语成绩\t平均分\t\t排名\t同学互评分\t品德评分\t教师评分\t总分\n");
		for (which = (page - 1) * numDisplay; which < page * numDisplay&&which<num; which++)
			printf("%d.\t%d\t%s\t%s\t%s\t%f\t%f\t%f\t%f\t%d\t%f\t%f\t%f\t%f\n", which + 1, globalstu[which].Number, globalstu[which].Name, globalstu[which].Sex, globalstu[which].PhoneNumber, globalstu[which].CScore, globalstu[which].MathScore, globalstu[which].ForeignSocre, globalstu[which].AverageScore, globalstu[which].Rank, globalstu[which].MutualScore, globalstu[which].VirtueScore, globalstu[which].TeacherScore, globalstu[which].TotalScore);
		printf("总共检索到%d个相关信息,一共%d页,当前%d页; 跳转至", num, allpage, page);
		scanf("%d", &page);
	}
	fclose(fp);
	printf("按enter键返回主界面\n");
	getchar();
	getchar();

}
int cleanData()
{
	FILE *fp;
	if ((fp = fopen(fileName, "wb")) == NULL)
	{
		printf("文件清空打开错误\n");
		return false;
	}
	fclose(fp);
	return true;
}

int Match(char Str[], char str[])
{
	int i = 0, j = 0, c;
	while (1)
	{
		j = 0;
		if (Str[i] == '\0')
			break;
		if (Str[i] == str[j])
		{
			c = i;
			while (1)
			{
				if (Str[c] != str[j])
				{
					break;
				}
				if (str[j + 1] == '\0')
					return 1;
				j++;
				c++;
			}
		}
		i++;
	}
	return 0;
}
int checkRepeat()
{
	FILE *fp, *fp2;
	struct Student temp, last;
	int num = 0;
	if ((fp = fopen(fileName, "rb")) == NULL)
	{
		printf("打开文件错误!");
		return 0;
	}
	if ((fp2 = fopen("temp.dat", "wb")) == NULL)
	{
		printf("打开文件错误!");
		return 0;
	}
	if (fread(&last, LEN, 1, fp))
		fwrite(&last, LEN, 1, fp2);
	while (1)
	{
		if (fread(&temp, LEN, 1, fp))
		{
			if (temp.Number == last.Number)
			{
				num++;
				if (num == 1)
					printf("----------------------------------------------------------------------\n序号\t学号\t姓名\t性别\t电话\t\tC成绩\t\t数学成绩\t外语成绩\t平均分\t\t排名\t同学互评分\t品德评分\t教师评分\t总分\n");
				printf("%d.\t%d\t%s\t%s\t%s\t%f\t%f\t%f\t%f\t%d\t%f\t%f\t%f\t%f\n", num, temp.Number, temp.Name, temp.Sex, temp.PhoneNumber, temp.CScore, temp.MathScore, temp.ForeignSocre, temp.AverageScore, temp.Rank, temp.MutualScore, temp.VirtueScore, temp.TeacherScore, temp.TotalScore);
			}
			else
			{
				fwrite(&temp, LEN, 1, fp2);
				last = temp;
			}
		}
		else break;
	}
	if (num > 0)
		printf("----------------------------------------------------------------------\n以上%d条数据学号重复,未被录入,请检查后重新录入\n", num);
	fclose(fp);
	fclose(fp2);
	CountRank();
	remove(fileName);
	rename("temp.dat", fileName);
	NewConfig();
	getchar();
	getchar();
}
