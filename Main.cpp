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
/*EX:20200604*/
class Timer
{
private:
	int time;
public:
	int GetTime();
};

AirLineManager SysManager;
Timer CLOCK;


void OnLoad()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
}
void OnExit()
{

}
int Control()
{
	return 0;
}
void ShowMenu()
{

}
void BookTicket()
{

}
void RefundTiket()
{

}
void Search()
{

}
void UpDate()
{
	
}
int main()
{
	//程序开始前
	OnLoad();
	//主程序循环
	while (1) {
		UpDate();
		Control();
	}
	//程序结束后
	OnExit();

	return 0;
}