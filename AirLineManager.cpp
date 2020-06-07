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

void AirLineManager::Add(string linecode, int planenum, int date, string start, string endd)
{
	AVLPTR key = (AVLPTR)malloc(sizeof(AirLineAVLNode));
	key->Code = linecode;
	key->StartName = start;

	key->EndName = endd;
	key->FlightDate = date;
	key->PlaneNumber = planenum;
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
const AVLPTR AirLineManager::SearchByStation(string start, string endd)
{
	STATIONNODEPTR res = StationData.Find(start, endd);
	return res->AirLineData;
}
const AVLPTR AirLineManager::AddCustomer(string linecode, string name, int& num)
{
	AVLPTR t = AirLineData.FindByCode(linecode, AirLineData.root);
	STATIONNODEPTR s = t->node;
	if (t->RemainTickets > 0) {
		t->RemainTickets--;
		s->CustomerList.push_back(Customer{ name,linecode,num, t->PlaneNumber });
		num = -1;
	}
	return t;
}
void AirLineManager::AddBackup(AVLPTR T, string name, int num)
{
	STATIONNODEPTR s = T->node;
	s->WatingQueue.push(Customer{ name,T->Code,num,T->PlaneNumber });
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
		S->CustomerList.erase(it);
		//FindBackUp(S);
		return true;
	}
	else
		return false;
}
void AirLineManager::Order_save(FILE* F1, FILE* F2, FILE* F3, AVLPTR T)
{
	if (T) {
		fprintf(F1, "%s\t%d\t%d\t%s\t%s\n",
			T->Code.c_str(), T->PlaneNumber, T->FlightDate, T->StartName.c_str(), T->EndName.c_str());
		STATIONNODEPTR S = T->node;
		for (auto it = S->CustomerList.begin(); it != S->CustomerList.end(); it++) {
			fprintf(F2, "%s\t%d\t%s\t%d\n",
				it->AirLineCode.c_str(), it->PlaneNum, it->Name.c_str(), it->TicketsNum);
		}
		string name = S->WatingQueue.front().Name;
		while (1) {
			Customer top = S->WatingQueue.front();
			S->WatingQueue.pop();
			S->WatingQueue.push(top);
			fprintf(F3, "%s\t%d\t%s\t%d\n",
				top.AirLineCode.c_str(), top.PlaneNum, top.Name.c_str(), top.TicketsNum);
			if (S->WatingQueue.front().Name == name)
				break;
		}
		Order_save(F1, F2, F3, T->Left);
		Order_save(F1, F2, F3, T->Right);
	}
}
void AirLineManager::Save(int date)
{
	char s1[20];
	itoa(date, s1, 8);
	strcat(s1, "航班信息.txt");
	char s2[20];
	itoa(date, s2, 8);
	strcat(s2, "客户信息.txt");
	char s3[20];
	itoa(date, s3, 8);
	strcat(s3, "候补信息.txt");
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
	string s1 = date + "航班信息.txt";
	string s2 = date + "客户信息.txt";
	string s3 = date + "候补信息.txt";
	FILE* F1 = fopen(s1.c_str(), "r");
	FILE* F2 = fopen(s2.c_str(), "r");
	FILE* F3 = fopen(s3.c_str(), "r");
	char e[3][10];
	char i[5];
	string a, b;
	while (1) {
		fscanf(F1, "%s%d%d%s%s", e[0], &i[0], &i[1], e[1], e[2]);
		a = e[1];
		b = e[2];
		Add(e[0], i[0], i[1], a, b);
		if (feof(F1)) break;
	}
	while (1) {
		fscanf(F2, "%s%d%s%d", e[0], &i[0], e[1], &i[1]);
		a = e[0];
		b = e[1];
		AVLPTR T = AirLineData.FindByCode(a, AirLineData.root);
		T->node->CustomerList.push_back(Customer{ b,a,i[1],i[0] });
	}
	while (1) {
		fscanf(F3, "%s%d%s%d", e[0], &i[0], e[1], &i[1]);
		a = e[0];
		b = e[1];
		AVLPTR T = AirLineData.FindByCode(a, AirLineData.root);
		T->node->WatingQueue.push(Customer{ b,a,i[1],i[0] });
	}
}