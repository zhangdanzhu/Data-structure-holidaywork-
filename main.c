#include "PriorityQue.h"

Status Setting(FILE** fp);
void ShowInterface(void);
void CreatePatients(PatientQue* pat_que); 
void SimulTest(FILE* fp);
void LogTreat(FILE* fp, Patient pat, int time);
void LogSettingInfo(FILE* fp);
void Analyse(int totaltime, int totalwaittime, FILE* fp);
 
/**
*函数名称：Setting 
*输入参数：文件指针地址 fp 
*返回值：  Status
*函数功能：进行模拟参数设定 
*/ 
Status Setting(FILE** fp) 
{
	int confirm = 0;
	char filename[30];
	while(!confirm) {
		system("cls");
		printf("---------------------------医院门诊仿真程序-----------------------------\n");
		printf("               计算机1409班    U201414797    张丹朱\n");
		printf("------------------------------------------------------------------------\n"); 
		printf("说明：本程序模拟医院门诊医师与病人看诊服务事件，并对实测结果进行统计分析\n"); 
		printf("------------------------------------------------------------------------\n"); 
		printf("\n**********请完成初始设置**********\n");
		printf("门诊窗口数：");
		scanf("%d", &DOCNUM);
		printf("诊治病人数：");
		scanf("%d", &PATNUM); 
		printf("单个病人最短诊治时间（分钟）：");
		scanf("%d", &STREATTIME);
		printf("单个病人最长诊治时间（分钟）：");
		scanf("%d", &LTREATTIME);
		while(LTREATTIME <= STREATTIME)
		{
			printf("  设置有误！最长时间应大于最短时间！请重输："); 
			scanf("%d", &LTREATTIME);
		}
		printf("两病人到达时间最短间隔（分钟）：");
		scanf("%d", &SINTERVAL);
		printf("两病人到达时间最长间隔（分钟）：");
		scanf("%d", &LINTERVAL);
		while(LINTERVAL <= SINTERVAL)
		{
			printf("  设置有误！最长间隔应大于最短间隔！请重输："); 
			scanf("%d", &LINTERVAL);
		}
		printf("模拟次数：");
		scanf("%d", &SIMULTIMES);
		printf("请输入测试结果记录文件名（15字以内）： ");
		scanf("%s",&filename);
		if ((*fp = fopen(filename, "wt+")) == NULL)
		{
			printf("文件打开失败！数据无法保存！\n ");
			return ERROR; 
		}
		LogSettingInfo(*fp); //记录设置 
		printf("\n输入 1 确认上述设置并进入模拟; 输入 0 重新设置 ："); 
		scanf("%d", &confirm);
	}
	return OK;	 
}

/**
*函数名称：LogSettingInfo 
*输入参数：文件指针fp 
*返回值：  无
*函数功能：记录设置信息 
*/
void LogSettingInfo(FILE* fp)
{
	fprintf(fp, "%s\n", "当前模拟参数设置如下：");
	fprintf(fp, "%s%d\t\t%s%d\n", "门诊窗口数： ", DOCNUM, "模拟病人数： ", PATNUM);
	fprintf(fp, "%s%d%s%d\t", "病人诊治时间：", STREATTIME, " - ", LTREATTIME);
	fprintf(fp, "%s%d%s%d\n", "病人到达时间间隔：", SINTERVAL, " - ", LINTERVAL); 
} 

/**
*函数名称：ShowInterface 
*输入参数：无
*返回值：  无
*函数功能：显示主界面 
*/
void ShowInterface(void) 
{
	system("cls");
	printf("---------------------------医院门诊仿真程序-----------------------------\n");
	printf("               计算机1409班    U201414797    张丹朱\n");
	printf("------------------------------------------------------------------------\n");	
	printf("*******************************当前设置*********************************\n");
	printf("门诊窗口数：%d    诊治病人数：%d      模拟次数：%d\n", DOCNUM, PATNUM, SIMULTIMES);
	printf("单个病人最短诊治时间：%d分钟\t单个病人最长诊治时间：%d分钟\n", STREATTIME, LTREATTIME);
	printf("两病人到达时间最短间隔：%d分钟\t两病人到达时间最长间隔：%d分钟\n", SINTERVAL, LINTERVAL);
	printf("************************************************************************\n");	
} 

/**
*函数名称：CreatePatients 
*输入参数：病人队列地址&pat_que
*返回值：  无
*函数功能：产生随机病人序列 
*/
void CreatePatients(PatientQue* pat_que)
{
	int i;
	srand((unsigned)time(NULL));
	//产生第一个病人并以其到达时间为时间零点 
	++(pat_que->cur_num);
	pat_que->patients[1].ID = 1;
	pat_que->patients[1].altime = 0;
	pat_que->patients[1].treattime = rand() % (LTREATTIME - STREATTIME) + STREATTIME;
	pat_que->patients[1].istreated = 0;
	//产生剩下病人 
	for (i = 2; i <= PATNUM; i++)
	{
		++(pat_que->cur_num);
		pat_que->patients[i].ID = i;
		pat_que->patients[i].altime = pat_que->patients[i-1].altime + rand() % (LINTERVAL - SINTERVAL) + SINTERVAL;
		pat_que->patients[i].treattime = rand() % (LTREATTIME - STREATTIME) + STREATTIME;
		pat_que->patients[i].istreated = 0;
	}
}

/**
*函数名称：SimulTest 
*输入参数：文件指针fp 
*返回值：  无
*函数功能：模拟测试 
*/
void SimulTest(FILE* fp)
{
	int i;
	int waittime, time;             //记录单次模拟总用时+总等待时间
	int totalwaittime = 0, totaltime = 0;//记录全部模拟总用时+总等待时间 
	PatientQue wait_que;            //等待病人队列 
	PatientQue treat_que;           //诊治病人队列  
	Patient temp;
	Init_PriorityQue(&wait_que, PATNUM);  //创建空队列 
	Init_PriorityQue(&treat_que, DOCNUM); //创建空队列 	
	for (i = 1; i <= SIMULTIMES; i++)
	{
		printf("第 %d 次模拟正在进行，请稍等......\n", i);
		Sleep(1000); 
		fprintf(fp, "\n%s%d%s\n", "第 ", i, " 次模拟数据："); 
		CreatePatients(&wait_que);
		waittime = 0;
		time = 0;
		
		//模拟 统计 记录   
		while (wait_que.cur_num) //有病人在等待 
		{
			if (!PriorityQue_Full(treat_que))//门诊窗口有空闲
			{
				temp = wait_que.patients[PATNUM - (--(wait_que.cur_num))];
				PriorityQue_Insert(&treat_que, temp);//病人进入治疗队列
				
			} 
			else //门诊窗口满 
			{
				temp = PriorityQue_DeletMin(&treat_que);
				if (!temp.istreated) //最先发生的是开始诊治事件 
				{
					if (time < temp.altime)//病人到达前无事件发生
					{
						time = temp.altime;   //时间进行到病人到达 
					} 
					LogTreat(fp, temp, time); //记录诊治病人的信息
					waittime += time - temp.altime;//总等待时间增加 
					temp.altime = time + temp.treattime;//获得病人离开时间
					temp.istreated = 1; 
					PriorityQue_Insert(&treat_que, temp);//病人进入治疗队列
				} 
				else ////最先发生的是离开事件 
				{
					time = temp.altime; //时间进行到病人离开
					fprintf(fp, "%4d%s\t%4d%s\n", time, "分钟时", temp.ID, "号病人诊治结束，离开。"); 
				}		
			}
		}
		while (treat_que.cur_num) //有病人在治疗 
		{
			temp = PriorityQue_DeletMin(&treat_que);
			if (!temp.istreated) //最先发生的是开始诊治事件 
			{
				if (time < temp.altime)//病人到达前无事件发生
				{
					time = temp.altime;   //时间进行到病人到达 
				} 
				LogTreat(fp, temp, time); //记录诊治病人的信息
				waittime += time - temp.altime;//总等待时间增加 
				temp.altime = time + temp.treattime;//获得病人离开时间
				temp.istreated = 1; 
				PriorityQue_Insert(&treat_que, temp);//病人进入治疗队列
			} 
			else ////最先发生的是离开事件 
			{
				time = temp.altime; //时间进行到病人离开
				fprintf(fp, "%4d%s\t%4d%s\n", time, "分钟时", temp.ID, "号病人诊治结束，离开。"); 
			}		
		} 
		//输出统计结果：总用时+平均等待时间等并记入日志
		printf("------------结果如下-------------\n");
		printf("总用时：%4d分钟，平均等待时间：%.1f分钟\n\n", time, (float)waittime/PATNUM); 
		//将本次模拟得到的数据存入总数据
		totalwaittime += waittime;
		totaltime += time;
	} 
	//输出多次模拟平均统计结果，并分析记录 
	Analyse(totaltime, totalwaittime, fp);
	fclose(fp);
	Destroy_PriorityQue(&wait_que);
	Destroy_PriorityQue(&treat_que); 
}

/**
*函数名称：LogTreat
*输入参数：Patient pat接受诊治的病人,int time当前时间,fp存入的文件 
*返回值： 无
*函数功能：记录诊治病人的信息
*/ 
void LogTreat(FILE* fp, Patient pat, int time)
{
	fprintf(fp, "%4d%s\t%4d%s", time, "分钟时", pat.ID, "号病人开始诊治。");
	fprintf(fp, "\t%s%4d%s", "该病人", pat.altime, "分钟时到达，"); 
	fprintf(fp, "%s%2d%s", "已等待", time-pat.altime, "分钟。");
	fprintf(fp, "%s%2d%s\n", "  预计诊治", pat.treattime, "分钟。");
}

/**
*函数名称：Analyse
*输入参数：int totaltime总时,int totalwaittime总等待时间 fp存入的文件 
*返回值： 无
*函数功能：对最后结果进行分析 
*/
void Analyse(int totaltime, int totalwaittime, FILE* fp)
{
	int ave_time = totaltime/SIMULTIMES;
	float ave_wait = (float)totalwaittime/(SIMULTIMES * PATNUM); 
	printf("\n************综合统计*************\n");
	printf("平均总用时：%4d分钟，平均等待时间：%.1f分钟\n", ave_time, ave_wait);
	fprintf(fp, "\n%s%4d", "平均总用时：", ave_time); 
	fprintf(fp, "%s%.2f%s\n", "分钟，平均等待时间：", ave_wait, "分钟");
	printf("**************分析***************\n");
	if (ave_time > 600 || ave_wait > 15)
	{
		printf("当前状况下繁忙，效率低下，建议增开门诊窗口\n\n"); 
	}
	else if (ave_time < 300 && ave_wait < 8)
	{
		printf("当前状况下清闲，可视情况减少门诊窗口\n\n"); 
	}
	else
	{
		printf("当前状况下效率适中，可照此开设门诊窗口\n\n"); 
	}	
}

int main(int argc, char *argv[]) 
{
	int choice = 1;
	FILE* fp;
	while (choice) 
	{
		if (Setting(&fp) == ERROR)
		{
			break;
		}
		ShowInterface();
		SimulTest(fp);
		printf("\n输入 1 重新设置参数进行模拟; 输入 0 退出 ：");
		scanf("%d", &choice); 
	}
	printf("欢迎下次再使用本系统！"); 
	return 0;
}