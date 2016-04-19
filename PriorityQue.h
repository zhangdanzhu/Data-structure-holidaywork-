#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
 
//�������״̬���� 
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int Status; 

int LTREATTIME;  //�����ʱ�� 
int STREATTIME;  //�������ʱ��
int DOCNUM;      //���ﴰ���� 
int PATNUM;      //���Բ����� 
int LINTERVAL;   //�����������ʱ���� 
int SINTERVAL;   //��������̵���ʱ���� 
int SIMULTIMES;  //ģ����� 

typedef struct {
	int ID;  		//���˱�� 
	int altime;    	//���˵ĵ�����뿪ʱ�� 
	int treattime; 	//���˵�����ʱ��
	int istreated;  //��ǲ����Ƿ��Ѿ����ι� 
}Patient; 

typedef struct {
	Patient* patients; //ȫ�����˶��� 
	int cur_num;       //��ǰ������ 
	int maxnum;		   //������� 
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
*�������ƣ�Init_PriorityQue 
*������������е�ַ &pat_que �������int num 
*����ֵ��  Status �ɹ�����OK������ʧ���˳� 
*�������ܣ������ն��� 
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
*�������ƣ�Destroy_PriorityQue 
*������������е�ַ &pat_que 
*����ֵ��  Status �ɹ�����OK
*�������ܣ����ٶ��� 
*/
Status Destroy_PriorityQue(PatientQue* pat_que)
{
	free(pat_que->patients);
	pat_que->patients = NULL;
	pat_que->cur_num = 0;
	return OK;
}

/**
*�������ƣ�Clear_PriorityQue
*������������е�ַ &pat_que 
*����ֵ��  Status �ɹ�����OK
*�������ܣ��ÿն��� 
*/
Status Clear_PriorityQue(PatientQue* pat_que)
{
	pat_que->cur_num = 0; 
	return OK;
}

/**
*�������ƣ�PriorityQue_Empty
*������������� pat_que 
*����ֵ��  Status
*�������ܣ��ж϶����Ƿ�Ϊ�գ��շ���TRUE������FALSE 
*/
Status PriorityQue_Empty(PatientQue pat_que)
{
	return (pat_que.cur_num == 0)?TRUE:FALSE;
}

/**
*�������ƣ�PriorityQue_Full
*������������� pat_que 
*����ֵ��  Status
*�������ܣ��ж϶����Ƿ�����������TRUE������FALSE 
*/
Status PriorityQue_Full(PatientQue pat_que)
{
	return (pat_que.cur_num ==  pat_que.maxnum)?TRUE:FALSE;
}

/**
*�������ƣ�PriorityQue_Insert
*������������е�ַ &pat_que, ����Ĳ���pat 
*����ֵ��  Status
*�������ܣ������µĲ��ˣ��ɹ�����OK��ʧ�ܷ���ERROR 
*/
Status PriorityQue_Insert(PatientQue* pat_que, Patient pat)
{
	int i;
	if (PriorityQue_Full(*pat_que))
	{
		return ERROR;
	}
	pat_que->patients[++(pat_que->cur_num)] = pat; 
	//��patients[1...cur_num]���µ�����С����
	for (i = pat_que->cur_num/2; i > 0; --i)
	{
		HeapAdjust(pat_que->patients, i, pat_que->cur_num);
	}
	return OK;
}

/**
*�������ƣ�PriorityQue_DeletMin
*������������е�ַ &pat_que
*����ֵ��  ɾ���Ĳ��� Patient 
*�������ܣ�ɾ�����ȼ���С�Ĳ��ˣ�����������Ϣ 
*/
Patient PriorityQue_DeletMin(PatientQue* pat_que)
{
	Patient temp;
	temp = pat_que->patients[1];
	pat_que->patients[1] = pat_que->patients[pat_que->cur_num];
	pat_que->patients[(pat_que->cur_num)--] = temp;
	//��patients[1...cur_num]���µ�����С���� 
	HeapAdjust(pat_que->patients, 1, pat_que->cur_num);
	return temp;
}

/**
 *�������ƣ�HeapAdjust 
 *����������������� patient* patients,��ʼ�����λ�����s��m 
 *����ֵ��  �� 
 *�������ܣ���patients[s...m]������С����   
*/
void HeapAdjust(Patient* patients, int s, int m) 
{
	int j;
	Patient sb = patients[s];
	for (j = 2 * s; j <= m; j *= 2)//���Źؼ��ֽ�С�ĺ��ӽ������ɸѡ 
	{
		if (j < m && patients[j].altime > patients[j+1].altime)
		{
			j++;
		}
		if (!(sb.altime > patients[j].altime))
		{
			break; //sbӦ������λ��s�� 
		}
		patients[s] = patients[j];
		s = j;
	}	
	patients[s] = sb; //���� 
} 