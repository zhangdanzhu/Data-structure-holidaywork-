#include "PriorityQue.h"

Status Setting(FILE** fp);
void ShowInterface(void);
void CreatePatients(PatientQue* pat_que); 
void SimulTest(FILE* fp);
void LogTreat(FILE* fp, Patient pat, int time);
void LogSettingInfo(FILE* fp);
void Analyse(int totaltime, int totalwaittime, FILE* fp);
 
/**
*�������ƣ�Setting 
*����������ļ�ָ���ַ fp 
*����ֵ��  Status
*�������ܣ�����ģ������趨 
*/ 
Status Setting(FILE** fp) 
{
	int confirm = 0;
	char filename[30];
	while(!confirm) {
		system("cls");
		printf("---------------------------ҽԺ����������-----------------------------\n");
		printf("               �����1409��    U201414797    �ŵ���\n");
		printf("------------------------------------------------------------------------\n"); 
		printf("˵����������ģ��ҽԺ����ҽʦ�벡�˿�������¼�������ʵ��������ͳ�Ʒ���\n"); 
		printf("------------------------------------------------------------------------\n"); 
		printf("\n**********����ɳ�ʼ����**********\n");
		printf("���ﴰ������");
		scanf("%d", &DOCNUM);
		printf("���β�������");
		scanf("%d", &PATNUM); 
		printf("���������������ʱ�䣨���ӣ���");
		scanf("%d", &STREATTIME);
		printf("�������������ʱ�䣨���ӣ���");
		scanf("%d", &LTREATTIME);
		while(LTREATTIME <= STREATTIME)
		{
			printf("  ���������ʱ��Ӧ�������ʱ�䣡�����䣺"); 
			scanf("%d", &LTREATTIME);
		}
		printf("�����˵���ʱ����̼�������ӣ���");
		scanf("%d", &SINTERVAL);
		printf("�����˵���ʱ�����������ӣ���");
		scanf("%d", &LINTERVAL);
		while(LINTERVAL <= SINTERVAL)
		{
			printf("  ������������Ӧ������̼���������䣺"); 
			scanf("%d", &LINTERVAL);
		}
		printf("ģ�������");
		scanf("%d", &SIMULTIMES);
		printf("��������Խ����¼�ļ�����15�����ڣ��� ");
		scanf("%s",&filename);
		if ((*fp = fopen(filename, "wt+")) == NULL)
		{
			printf("�ļ���ʧ�ܣ������޷����棡\n ");
			return ERROR; 
		}
		LogSettingInfo(*fp); //��¼���� 
		printf("\n���� 1 ȷ���������ò�����ģ��; ���� 0 �������� ��"); 
		scanf("%d", &confirm);
	}
	return OK;	 
}

/**
*�������ƣ�LogSettingInfo 
*����������ļ�ָ��fp 
*����ֵ��  ��
*�������ܣ���¼������Ϣ 
*/
void LogSettingInfo(FILE* fp)
{
	fprintf(fp, "%s\n", "��ǰģ������������£�");
	fprintf(fp, "%s%d\t\t%s%d\n", "���ﴰ������ ", DOCNUM, "ģ�ⲡ������ ", PATNUM);
	fprintf(fp, "%s%d%s%d\t", "��������ʱ�䣺", STREATTIME, " - ", LTREATTIME);
	fprintf(fp, "%s%d%s%d\n", "���˵���ʱ������", SINTERVAL, " - ", LINTERVAL); 
} 

/**
*�������ƣ�ShowInterface 
*�����������
*����ֵ��  ��
*�������ܣ���ʾ������ 
*/
void ShowInterface(void) 
{
	system("cls");
	printf("---------------------------ҽԺ����������-----------------------------\n");
	printf("               �����1409��    U201414797    �ŵ���\n");
	printf("------------------------------------------------------------------------\n");	
	printf("*******************************��ǰ����*********************************\n");
	printf("���ﴰ������%d    ���β�������%d      ģ�������%d\n", DOCNUM, PATNUM, SIMULTIMES);
	printf("���������������ʱ�䣺%d����\t�������������ʱ�䣺%d����\n", STREATTIME, LTREATTIME);
	printf("�����˵���ʱ����̼����%d����\t�����˵���ʱ��������%d����\n", SINTERVAL, LINTERVAL);
	printf("************************************************************************\n");	
} 

/**
*�������ƣ�CreatePatients 
*������������˶��е�ַ&pat_que
*����ֵ��  ��
*�������ܣ���������������� 
*/
void CreatePatients(PatientQue* pat_que)
{
	int i;
	srand((unsigned)time(NULL));
	//������һ�����˲����䵽��ʱ��Ϊʱ����� 
	++(pat_que->cur_num);
	pat_que->patients[1].ID = 1;
	pat_que->patients[1].altime = 0;
	pat_que->patients[1].treattime = rand() % (LTREATTIME - STREATTIME) + STREATTIME;
	pat_que->patients[1].istreated = 0;
	//����ʣ�²��� 
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
*�������ƣ�SimulTest 
*����������ļ�ָ��fp 
*����ֵ��  ��
*�������ܣ�ģ����� 
*/
void SimulTest(FILE* fp)
{
	int i;
	int waittime, time;             //��¼����ģ������ʱ+�ܵȴ�ʱ��
	int totalwaittime = 0, totaltime = 0;//��¼ȫ��ģ������ʱ+�ܵȴ�ʱ�� 
	PatientQue wait_que;            //�ȴ����˶��� 
	PatientQue treat_que;           //���β��˶���  
	Patient temp;
	Init_PriorityQue(&wait_que, PATNUM);  //�����ն��� 
	Init_PriorityQue(&treat_que, DOCNUM); //�����ն��� 	
	for (i = 1; i <= SIMULTIMES; i++)
	{
		printf("�� %d ��ģ�����ڽ��У����Ե�......\n", i);
		Sleep(1000); 
		fprintf(fp, "\n%s%d%s\n", "�� ", i, " ��ģ�����ݣ�"); 
		CreatePatients(&wait_que);
		waittime = 0;
		time = 0;
		
		//ģ�� ͳ�� ��¼   
		while (wait_que.cur_num) //�в����ڵȴ� 
		{
			if (!PriorityQue_Full(treat_que))//���ﴰ���п���
			{
				temp = wait_que.patients[PATNUM - (--(wait_que.cur_num))];
				PriorityQue_Insert(&treat_que, temp);//���˽������ƶ���
				
			} 
			else //���ﴰ���� 
			{
				temp = PriorityQue_DeletMin(&treat_que);
				if (!temp.istreated) //���ȷ������ǿ�ʼ�����¼� 
				{
					if (time < temp.altime)//���˵���ǰ���¼�����
					{
						time = temp.altime;   //ʱ����е����˵��� 
					} 
					LogTreat(fp, temp, time); //��¼���β��˵���Ϣ
					waittime += time - temp.altime;//�ܵȴ�ʱ������ 
					temp.altime = time + temp.treattime;//��ò����뿪ʱ��
					temp.istreated = 1; 
					PriorityQue_Insert(&treat_que, temp);//���˽������ƶ���
				} 
				else ////���ȷ��������뿪�¼� 
				{
					time = temp.altime; //ʱ����е������뿪
					fprintf(fp, "%4d%s\t%4d%s\n", time, "����ʱ", temp.ID, "�Ų������ν������뿪��"); 
				}		
			}
		}
		while (treat_que.cur_num) //�в��������� 
		{
			temp = PriorityQue_DeletMin(&treat_que);
			if (!temp.istreated) //���ȷ������ǿ�ʼ�����¼� 
			{
				if (time < temp.altime)//���˵���ǰ���¼�����
				{
					time = temp.altime;   //ʱ����е����˵��� 
				} 
				LogTreat(fp, temp, time); //��¼���β��˵���Ϣ
				waittime += time - temp.altime;//�ܵȴ�ʱ������ 
				temp.altime = time + temp.treattime;//��ò����뿪ʱ��
				temp.istreated = 1; 
				PriorityQue_Insert(&treat_que, temp);//���˽������ƶ���
			} 
			else ////���ȷ��������뿪�¼� 
			{
				time = temp.altime; //ʱ����е������뿪
				fprintf(fp, "%4d%s\t%4d%s\n", time, "����ʱ", temp.ID, "�Ų������ν������뿪��"); 
			}		
		} 
		//���ͳ�ƽ��������ʱ+ƽ���ȴ�ʱ��Ȳ�������־
		printf("------------�������-------------\n");
		printf("����ʱ��%4d���ӣ�ƽ���ȴ�ʱ�䣺%.1f����\n\n", time, (float)waittime/PATNUM); 
		//������ģ��õ������ݴ���������
		totalwaittime += waittime;
		totaltime += time;
	} 
	//������ģ��ƽ��ͳ�ƽ������������¼ 
	Analyse(totaltime, totalwaittime, fp);
	fclose(fp);
	Destroy_PriorityQue(&wait_que);
	Destroy_PriorityQue(&treat_que); 
}

/**
*�������ƣ�LogTreat
*���������Patient pat�������εĲ���,int time��ǰʱ��,fp������ļ� 
*����ֵ�� ��
*�������ܣ���¼���β��˵���Ϣ
*/ 
void LogTreat(FILE* fp, Patient pat, int time)
{
	fprintf(fp, "%4d%s\t%4d%s", time, "����ʱ", pat.ID, "�Ų��˿�ʼ���Ρ�");
	fprintf(fp, "\t%s%4d%s", "�ò���", pat.altime, "����ʱ���"); 
	fprintf(fp, "%s%2d%s", "�ѵȴ�", time-pat.altime, "���ӡ�");
	fprintf(fp, "%s%2d%s\n", "  Ԥ������", pat.treattime, "���ӡ�");
}

/**
*�������ƣ�Analyse
*���������int totaltime��ʱ,int totalwaittime�ܵȴ�ʱ�� fp������ļ� 
*����ֵ�� ��
*�������ܣ�����������з��� 
*/
void Analyse(int totaltime, int totalwaittime, FILE* fp)
{
	int ave_time = totaltime/SIMULTIMES;
	float ave_wait = (float)totalwaittime/(SIMULTIMES * PATNUM); 
	printf("\n************�ۺ�ͳ��*************\n");
	printf("ƽ������ʱ��%4d���ӣ�ƽ���ȴ�ʱ�䣺%.1f����\n", ave_time, ave_wait);
	fprintf(fp, "\n%s%4d", "ƽ������ʱ��", ave_time); 
	fprintf(fp, "%s%.2f%s\n", "���ӣ�ƽ���ȴ�ʱ�䣺", ave_wait, "����");
	printf("**************����***************\n");
	if (ave_time > 600 || ave_wait > 15)
	{
		printf("��ǰ״���·�æ��Ч�ʵ��£������������ﴰ��\n\n"); 
	}
	else if (ave_time < 300 && ave_wait < 8)
	{
		printf("��ǰ״�������У���������������ﴰ��\n\n"); 
	}
	else
	{
		printf("��ǰ״����Ч�����У����մ˿������ﴰ��\n\n"); 
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
		printf("\n���� 1 �������ò�������ģ��; ���� 0 �˳� ��");
		scanf("%d", &choice); 
	}
	printf("��ӭ�´���ʹ�ñ�ϵͳ��"); 
	return 0;
}