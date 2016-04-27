#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
 
//函数结果状态代码 
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int Status; 

int LTREATTIME;  //最长诊治时间 
int STREATTIME;  //最短诊治时间
int DOCNUM;      //门诊窗口数 
int PATNUM;      //测试病人数 
int LINTERVAL;   //两病人最长到达时间间隔 
int SINTERVAL;   //两病人最短到达时间间隔 
int SIMULTIMES;  //模拟次数 

typedef struct {
	int ID;  		//病人编号 
	int altime;    	//病人的到达或离开时间 
	int treattime; 	//病人的诊治时间
	int istreated;  //标记病人是否已经诊治过 
}Patient; 

typedef struct {
	Patient* patients; //全部病人队列 
	int cur_num;       //当前病人数 
	int maxnum;		   //最大病人数 
}PatientQue;

Status Init_PriorityQue(PatientQue* pat_que, int num); 
Status Destroy_PriorityQue(PatientQue* pat_que);
Status Clear_PriorityQue(PatientQue* pat_que);
Status PriorityQue_Insert(PatientQue* pat_que, Patient pat);
Patient PriorityQue_DeletMin(PatientQue* pat_que); 
Status PriorityQue_Empty(PatientQue pat_que);
Status PriorityQue_Full(PatientQue pat_que);
void HeapAdjust(Patient* patients, int s, int m); 
void HeapSort(Patient* patients, int num); 

/**
*函数名称：Init_PriorityQue 
*输入参数：队列地址 &pat_que 最大人数int num 
*返回值：  Status 成功返回OK，分配失败退出 
*函数功能：创建空队列 
*/ 
Status Init_PriorityQue(PatientQue* pat_que, int num)
{
	pat_que->patients = (Patient*)malloc((num + 1) * sizeof(Patient));
	if (!pat_que->patients)
	{
		exit(OVERFLOW);
	}
	pat_que->cur_num = 0;
	pat_que->maxnum = num; 
	return OK; 
}

/**
*函数名称：Destroy_PriorityQue 
*输入参数：队列地址 &pat_que 
*返回值：  Status 成功返回OK
*函数功能：销毁队列 
*/
Status Destroy_PriorityQue(PatientQue* pat_que)
{
	free(pat_que->patients);
	pat_que->patients = NULL;
	pat_que->cur_num = 0;
	return OK;
}

/**
*函数名称：Clear_PriorityQue
*输入参数：队列地址 &pat_que 
*返回值：  Status 成功返回OK
*函数功能：置空队列 
*/
Status Clear_PriorityQue(PatientQue* pat_que)
{
	pat_que->cur_num = 0; 
	return OK;
}

/**
*函数名称：PriorityQue_Empty
*输入参数：队列 pat_que 
*返回值：  Status
*函数功能：判断队列是否为空，空返回TRUE，否则FALSE 
*/
Status PriorityQue_Empty(PatientQue pat_que)
{
	return (pat_que.cur_num == 0)?TRUE:FALSE;
}

/**
*函数名称：PriorityQue_Full
*输入参数：队列 pat_que 
*返回值：  Status
*函数功能：判断队列是否满，满返回TRUE，否则FALSE 
*/
Status PriorityQue_Full(PatientQue pat_que)
{
	return (pat_que.cur_num ==  pat_que.maxnum)?TRUE:FALSE;
}

/**
*函数名称：PriorityQue_Insert
*输入参数：队列地址 &pat_que, 插入的病人pat 
*返回值：  Status
*函数功能：插入新的病人，成功返回OK，失败返回ERROR 
*/
Status PriorityQue_Insert(PatientQue* pat_que, Patient pat)
{
	int i;
	if (PriorityQue_Full(*pat_que))
	{
		return ERROR;
	}
	pat_que->patients[++(pat_que->cur_num)] = pat; 
	//将patients[1...cur_num]重新调整成小顶堆
	for (i = pat_que->cur_num/2; i > 0; --i)
	{
		HeapAdjust(pat_que->patients, i, pat_que->cur_num);
	}
	return OK;
}

/**
*函数名称：PriorityQue_DeletMin
*输入参数：队列地址 &pat_que
*返回值：  删除的病人 Patient 
*函数功能：删除优先级最小的病人，并返回其信息 
*/
Patient PriorityQue_DeletMin(PatientQue* pat_que)
{
	Patient temp;
	temp = pat_que->patients[1];
	pat_que->patients[1] = pat_que->patients[pat_que->cur_num];
	pat_que->patients[(pat_que->cur_num)--] = temp;
	//将patients[1...cur_num]重新调整成小顶堆 
	HeapAdjust(pat_que->patients, 1, pat_que->cur_num);
	return temp;
}

/**
 *函数名称：HeapAdjust 
 *输入参数：病人数组 patient* patients,起始与结束位置序号s、m 
 *返回值：  无 
 *函数功能：将patients[s...m]调整成小顶堆   
*/
void HeapAdjust(Patient* patients, int s, int m) 
{
	int j;
	Patient sb = patients[s];
	for (j = 2 * s; j <= m; j *= 2)//沿着关键字较小的孩子结点向下筛选 
	{
		if (j < m && patients[j].altime > patients[j+1].altime)
		{
			j++;
		}
		if (!(sb.altime > patients[j].altime))
		{
			break; //sb应插入在位置s上 
		}
		patients[s] = patients[j];
		s = j;
	}	
	patients[s] = sb; //插入 
} 