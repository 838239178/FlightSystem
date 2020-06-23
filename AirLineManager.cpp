#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <iomanip>
#include <algorithm>
using namespace std;
#include "DataStructure.h"
#include "AirLineManager.h"

//���뺽��š��ɻ��š��������ڡ���㡢�յ������һ������
bool AirLineManager::Add(string linecode, int planenum, int date, string start, string endd, int remain)
{
	/*������ظ������ʧ��*/
	if (AirLineData.FindByCode(linecode, AirLineData.root))
		return false;
	AVLPTR key = new AirLineAVLNode;
	key->Code = linecode;
	key->StartName = start;
	key->RemainTickets = remain;
	key->EndName = endd;
	key->FlightDate = date;
	key->PlaneNo = planenum;
	AirLineData.Insert(key, AirLineData.root);
	StationData.Insert(start, endd, key);
	return true;
}
//�������ɾ�����ɹ�����true
bool AirLineManager::Remove(string linecode)
{
	AVLTree T = AirLineData.FindByCode(linecode, AirLineData.root);
	if (!T) return false;
	StationData.Remove(T->head, T->node);
	AirLineData.Delete(linecode, AirLineData.root);
	return true;
}
//����ֹ����ɾ��
void AirLineManager::Remove(int date)
{
	AirLineData.ClearByDate(date);
}
//������㡢�յ㣬���ش������н����vector
const vector<AVLPTR> AirLineManager::SearchByStation(string start, string endd)
{
	vector<AVLPTR> ptrs;
	StationData.Find(start, endd, ptrs);
	return ptrs;
}
//���뺽��š�������Ʊ�������غ�����Ϣָ�루ֻ����
const AVLPTR AirLineManager::AddCustomer(string linecode, string name, int& num)
{
	AVLPTR t = AirLineData.FindByCode(linecode, AirLineData.root);
	if (!t) return NULL;
	STATIONNODEPTR s = t->node;
	if (t->RemainTickets-num >= 0) {
		t->RemainTickets -= num;
		s->CustomerList.push_back(Customer{ name,linecode,num, 50 - t->RemainTickets });
		num = 50 - t->RemainTickets;
		SaveBookResult(t, name);
	}
	else
		num = -1;
	return t;
}
//˽��-ֱ����ӿͻ�
void AirLineManager::AddCustomer(AVLPTR T, Customer B)
{
	AVLPTR t = T;
	STATIONNODEPTR s = t->node;
	if (t->RemainTickets - B.TicketsNum >= 0) {
		t->RemainTickets -= B.TicketsNum;
		B.SeatNo = 50 - t->RemainTickets;
		s->CustomerList.push_back(B);
		SaveBookResult(T, B.Name);
	}
}
//���뺽����Ϣ��ָ�롢������Ʊ��
void AirLineManager::AddBackup(AVLPTR T, string name, int num)
{
	STATIONNODEPTR s = T->node;
	s->WatingQueue.push(Customer{ name,T->Code,num,-1 });
}
//���뺽��š��ͻ����֣��ɹ�����true
bool AirLineManager::RemoveCustomer(string linecode, string name)
{
	AVLPTR T = AirLineData.FindByCode(linecode, AirLineData.root);
	STATIONNODEPTR S = T->node;
	auto it = S->CustomerList.begin();
	while (it != S->CustomerList.end()) {
		if (it->Name == name)
			break;
		it++;
	}
	if (it != S->CustomerList.end()) {
		T->RemainTickets += it->TicketsNum;
		S->CustomerList.erase(it);
		Customer B = FindBackUp(S, T->RemainTickets);
		if (B.TicketsNum != -1) {
			AddCustomer(T, B);
		}
		return true;
	}
	else
		return false;
}
//��������
void AirLineManager::Save(int date)
{
	const string DT = to_string(date);
	string s1, s2, s3;
	this->Version = DT;
	s1 = PATH + DT + "AirLine.txt";
	s2 = PATH + DT + "Customer.txt";
	s3 = PATH + DT + "Backup.txt";
	FILE* F1 = fopen(s1.c_str(), "w");
	FILE* F2 = fopen(s2.c_str(), "w");
	FILE* F3 = fopen(s3.c_str(), "w");
	FILE* F4 = fopen("resent.txt", "w");
	stack<AVLPTR> s;
	AVLPTR T = AirLineData.root;
	while (!s.empty() || T) {
		while (T)
		{
			fprintf(F1, "%s\t%d\t%d\t%s\t%s\t%d\n",
				T->Code.c_str(), T->PlaneNo, T->FlightDate, T->StartName.c_str(), T->EndName.c_str(), T->RemainTickets);
			STATIONNODEPTR S = T->node;
			for (auto it = S->CustomerList.begin(); it != S->CustomerList.end(); it++) {
				fprintf(F2, "%s\t%d\t%s\t%d\n",
					it->AirLineCode.c_str(), it->SeatNo, it->Name.c_str(), it->TicketsNum);
			}
			if (!S->WatingQueue.empty()) {
				string name = S->WatingQueue.front().Name;
				while (1) {
					Customer top = S->WatingQueue.front();
					S->WatingQueue.pop();
					S->WatingQueue.push(top);
					fprintf(F3, "%s\t%d\t%s\t%d\n",
						top.AirLineCode.c_str(), top.SeatNo, top.Name.c_str(), top.TicketsNum);
					if (S->WatingQueue.front().Name == name)
						break;
				}
			}
			s.push(T);
			T = T->Left;
		}
		if (!s.empty()) {
			T = s.top();
			s.pop();
			T = T->Right;
		}
	}
	fprintf(F4, "%d\n", date);
	fclose(F1);
	fclose(F2);
	fclose(F3);
	fclose(F4);
}
//��������
bool AirLineManager::Load(string date)
{
	this->Version = date;
	string s1 = PATH + date + "AirLine.txt";
	string s2 = PATH + date + "Customer.txt";
	string s3 = PATH + date + "Backup.txt";
	FILE* F1 = fopen(s1.c_str(), "r");
	FILE* F2 = fopen(s2.c_str(), "r");
	FILE* F3 = fopen(s3.c_str(), "r");
	char e[4][10] = {'\0'};
	int i[5] = { 0 };
	int flag = 0;
	string a, b;
	if (!(F1 && F2 && F3)) return false;
	while (1) {
		flag = fscanf(F1, "%s %d %d %s %s %d", e[0], &i[0], &i[1], e[1], e[2], &i[2]);
		if (flag == -1) break;
		a = e[1];
		b = e[2];
		Add(e[0], i[0], i[1], a, b, i[2]);
	}
	fclose(F1);
	while (1) {
		flag = fscanf(F2, "%s %d %s %d", e[0], &i[0], e[1], &i[1]);
		if (flag == -1) break;
		a = e[0]; 
		b = e[1];
		AVLPTR T = AirLineData.FindByCode(a, AirLineData.root);
		T->node->CustomerList.push_back(Customer{ b,a,i[1],i[0] });
	}
	fclose(F2);
	while (1) {
		flag = fscanf(F3, "%s %d %s %d", e[0], &i[0], e[1], &i[1]);
		if (flag == -1)break;
		a = e[0];
		b = e[1];
		AVLPTR T = AirLineData.FindByCode(a, AirLineData.root);
		T->node->WatingQueue.push(Customer{ b,a,i[1],i[0] });
	}
	fclose(F3);
	return true;
}
//��ȡ����ı����¼
bool AirLineManager::AutoLoad() 
{
	FILE* F = fopen("resent.txt", "r");
	int date = 0;
	int flag = fscanf(F, "%d", &date);
	if (flag == -1) return false;
	string s = to_string(date);
	Load(s);
	return true;
}
//˽��-���涩��
void AirLineManager::SaveBookResult(const AVLPTR data, string name)
{
	FILE* F = fopen("DATA\\BookRecord.txt", "a");
	/*��������-�ͻ�����-�����-�ɻ���-���-�յ�-��������*/
	fprintf(F, "%s %-20s\t%s %-4d %-4s %-4s\t%d\n", Version.c_str(), name.c_str(), data->Code.c_str(),
		data->PlaneNo, data->StartName.c_str(), data->EndName.c_str(), data->FlightDate);
	fclose(F);
}
/*
˽��-Ѱ�Һ��ʵ��油��������������վ�������������Ʊ�������ظÿͻ�����Ϣ�ṹ��
û�з��Ͻ���򷵻صĽṹ����Ʊ��Ϊ-1
*/
Customer AirLineManager::FindBackUp(STATIONNODEPTR node, int tickets)
{
	Customer res("", "", -1, 0);
	if (node->WatingQueue.empty())
		return res;
	string name = node->WatingQueue.front().Name;
	
	if (node->WatingQueue.front().TicketsNum <= tickets) {
		res = node->WatingQueue.front();
		node->WatingQueue.pop();
		return res;
	}
	while (1) {
		Customer temp = node->WatingQueue.front();
		node->WatingQueue.pop();		
		if (temp.TicketsNum <= tickets) {
			res = temp;
		}
		else
			node->WatingQueue.push(temp);
		if (node->WatingQueue.front().Name == name)
			break;
	}
	return res;
}