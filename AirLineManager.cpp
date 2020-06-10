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

void AirLineManager::Add(string linecode, int planenum, int date, string start, string endd, int remain)
{
	AVLPTR key = new AirLineAVLNode;
	key->Code = linecode;
	key->StartName = start;
	key->RemainTickets = remain;
	key->EndName = endd;
	key->FlightDate = date;
	key->PlaneNo = planenum;
	AirLineData.Insert(key, AirLineData.root);
	StationData.Insert(start, endd, key);
}
bool AirLineManager::Remove(string linecode)
{
	AVLTree T = AirLineData.FindByCode(linecode, AirLineData.root);
	if (!T) return false;
	StationData.Remove(T->head, T->node);
	AirLineData.Delete(linecode, AirLineData.root);
	return true;
}
void AirLineManager::Remove(int date)
{
	AirLineData.ClearByDate(date);
}
const vector<AVLPTR> AirLineManager::SearchByStation(string start, string endd)
{
	vector<AVLPTR> ptrs;
	StationData.Find(start, endd, ptrs);
	return ptrs;
}
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
void AirLineManager::AddBackup(AVLPTR T, string name, int num)
{
	STATIONNODEPTR s = T->node;
	s->WatingQueue.push(Customer{ name,T->Code,num,-1 });
}
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
void AirLineManager::Order_save(FILE* F1, FILE* F2, FILE* F3, AVLPTR T)
{
	if (T) {
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
		Order_save(F1, F2, F3, T->Left);
		Order_save(F1, F2, F3, T->Right);
	}
}
void AirLineManager::Save(int date)
{
	char s1[25];;
	sprintf(s1, "%d", date);
	this->Version = s1;
	strcat(s1, "AirLine.txt");
	char s2[25];
	sprintf(s2, "%d", date);
	strcat(s2, "Customer.txt");
	char s3[25];
	sprintf(s3, "%d", date);
	strcat(s3, "Backup.txt");
	FILE* F1 = fopen(s1, "w");
	FILE* F2 = fopen(s2, "w");
	FILE* F3 = fopen(s3, "w");
	Order_save(F1, F1, F3, AirLineData.root);
	fclose(F1);
	fclose(F2);
	fclose(F3);
}
void AirLineManager::Load(string date)
{
	this->Version = date;
	string s1 = date + "AirLine.txt";
	string s2 = date + "Customer.txt";
	string s3 = date + "Backup.txt";
	FILE* F1 = fopen(s1.c_str(), "r");
	FILE* F2 = fopen(s2.c_str(), "r");
	FILE* F3 = fopen(s3.c_str(), "r");
	char e[4][10];
	int i[5];
	string a, b;
	while (1) {
		if (feof(F1)) break;
		fscanf(F1, "%s%d%d%s%s%d", e[0], &i[0], &i[1], e[1], e[2],&i[2]);
		a = e[1];
		b = e[2];
		Add(e[0], i[0], i[1], a, b, i[2]);
	}
	fclose(F1);
	while (1) {
		if (feof(F2)) break;
		fscanf(F2, "%s%d%s%d", e[0], &i[0], e[1], &i[1]);
		a = e[0];
		b = e[1];
		AVLPTR T = AirLineData.FindByCode(a, AirLineData.root);
		T->node->CustomerList.push_back(Customer{ b,a,i[1],i[0] });
	}
	fclose(F2);
	while (1) {
		if (feof(F3) )break;
		fscanf(F3, "%s%d%s%d", e[0], &i[0], e[1], &i[1]);
		a = e[0];
		b = e[1];
		AVLPTR T = AirLineData.FindByCode(a, AirLineData.root);
		T->node->WatingQueue.push(Customer{ b,a,i[1],i[0] });
	}
	fclose(F3);
}
void AirLineManager::SaveBookResult(const AVLPTR data, string name)
{
	FILE* F = fopen("BookRecord.txt", "a");
	fprintf(F, "%d\t%s\t%s\t%d\t%s\t%s\t%d\n", nowDate, name, data->Code.c_str(), data->PlaneNo, data->StartName, data->EndName, data->FlightDate);
	fclose(F);
}
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
			tickets = -1;
		}
		else
			node->WatingQueue.push(temp);
		if (node->WatingQueue.front().Name == name)
			break;
	}
	return res;
}