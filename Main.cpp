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
#include <easyx.h>
using namespace std;
#include "DataStructure.h"
#include "AirLineManager.h"
#include "UI.h"
#define NORMAL 1
#define RESULT 2
/*
获取日期的类
EX:20200604
*/
class Timer
{
private:
	struct tm* info;
public:
	int date;
	int GetTime() {
		time_t rawtime;
		time(&rawtime);
		info = gmtime(&rawtime);
		date = 0;
		date += (info->tm_year+1900) * 1e4;
		date += (info->tm_mon+1) * 1e2;
		date += info->tm_mday;
		return date;
	}
};

AirLineManager SysManager;
Timer CLOCK;
IMAGE background;
Button btn[8];
Button YES;
Button NO;
Button Exit;
Dialog Log;
Text Result;
int Tag;

void btn2_Click(Button sender, MOUSEMSG m);
void btn3_Click(Button sender, MOUSEMSG m);
void btn4_Click(Button sender, MOUSEMSG m);
void btn5_Click(Button sender, MOUSEMSG m);
void btn6_Click(Button sender, MOUSEMSG m);
void showdialog(Ve2 p,string title, string label);

void OnLoad()
{
	initgraph(800, 600);
	setbkmode(TRANSPARENT);
	CLOCK.GetTime();
	Tag = NORMAL;
	BeginBatchDraw();
	//
	loadimage(&background, "pic\\background.png");
	//
	string s[] = {
	"订票",//0
	"退票",//1
	"查询",//2
	"录入",//3
	"保存",//4
	"加载",//5
	"清除",//6
	};
	for (int i = 0; i < 7; i++) {
		btn[i].label = s[i];
		btn[i].SetImage("btn.png");
	}
	btn[2].Click = btn2_Click;
	btn[3].Click = btn3_Click;
	btn[4].Click = btn4_Click;
	btn[5].Click = btn5_Click;
	btn[6].Click = btn6_Click;
	//
	Log.SetImage("dialog.png");
}
void OnExit()
{
	EndBatchDraw();
	closegraph();
}
void Control()
{
	if (MouseHit()) {
		MOUSEMSG m = GetMouseMsg();
		if (m.mkLButton == true) {
			for (int i = 0; i < 7; i++) {
				if (btn[i].Click != NULL)
					btn[i].Click(btn[i], m);
			}
		}
	}	
	FlushMouseMsgBuffer();
}
void ShowMenu()
{
	Ve2 p;
	p.x = 60;
	p.y = 35;
	int cnt = 0;	
	char s[20];
	string v = "数据日期:" + SysManager.GetVersion();
	settextcolor(BLACK);
	settextstyle(40, 0, "宋体", 0, 0, 100, 0, 0, 0);
	outtextxy(240, 20, "航空航天订票系统");
	settextstyle(20, 0, "黑体", 0, 0, 100, 0, 0, 0);
	sprintf(s, "当前日期:%d", CLOCK.date);
	outtextxy(600, 20, s);
	outtextxy(600, 40, v.c_str());
	
	for (; p.y < 595; p.y += 80, cnt++) btn[cnt].Show(p);
}
void BookTicket()
{

}
void RefundTiket()
{

}
bool Search()
{	
	Result.Reset();	
	Result += "航班号    飞机号  起飞时间    余票";
	string start, endd;
	char input[100];
	InputBox(input, 100, "起点  终点", "按格式输入位置信息:");
	char is[20];
	char ie[20];
	char ir[10];
	sscanf(input, "%s %s", is, ie);
	start = is;
	endd = ie;
	const vector<AVLPTR> msg = SysManager.SearchByStation(start, endd);
	if (msg.size() == 0) return false;
	for (auto i = 0; i < msg.size(); i++) {
		itoa(msg[i]->PlaneNumber, is, 10);
		itoa(msg[i]->FlightDate, ie, 10);
		itoa(msg[i]->RemainTickets, ir, 10);
		Result += msg[i]->Code + "        " + is + "    " + ie + "     " + ir;
	}
	return true;
}
bool Write()
{
	char input[100] = { '\0' };
	InputBox(input, 100,  "航班号  飞机号  日期  起点  终点", "按格式输入航班信息:");
	if (input[0] == '\0')
		return false;
	string linecode, start, endd;
	char c[10];
	char s[25];
	char e[25];
	int num, date;
	sscanf(input, "%s %d %d %s %s", c, &num, &date, s, e);
	linecode = c;
	start = s;
	endd = e;
	SysManager.Add(linecode, num, date, start, endd);
	return true;
}
void Save()
{
	SysManager.Save(CLOCK.GetTime()); 
}
bool Load()
{
	char s[10] = { '\0' };
	InputBox(s, 10, "年月日（EX:20200605)", "请输入日期:", 0, 100, 100);
	if (s[0] == '\0')
		return false;
	string date = s;
	SysManager.Load(date);
	return true;
}
bool Clear()
{
	int date;
	char s[10] = { '\0' };
	InputBox(s, 10, "年月日（EX:20200605)", "请输入日期:",0,100,100);
	if (s[0] == '\0')
		return false;
	sscanf(s, "%d", &date);
	SysManager.Remove(date);
	return true;
}
void UpDate()
{
	cleardevice();
	putimage(0, 0, &background);
	ShowMenu();
	if (Tag == RESULT) Result.Show(Result.p);
	FlushBatchDraw();
}
void btn4_Click(Button sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		Save();
		showdialog(Ve2{ 300,200 }, "提示:", "保存成功");
	}
}
void btn3_Click(Button sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if(Write())
			showdialog(Ve2{ 300,200 }, "提示:", "录入成功");
		else
			showdialog(Ve2{ 300,200 }, "提示:", "录入失败");
	}
}
void btn5_Click(Button sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (Load())
			showdialog(Ve2{ 300,200 }, "提示:", "加载成功");
		else
			showdialog(Ve2{ 300,200 }, "提示:", "加载失败");
	}
}
void btn6_Click(Button sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (Clear())
			showdialog(Ve2{ 300,200 }, "提示:", "清除成功");
		else
			showdialog(Ve2{ 300,200 }, "提示:", "清除失败");
	}
}
void btn2_Click(Button sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (!Search())
			showdialog(Ve2{ 300,200 }, "提示:", "暂时没有该航班");
		else
			Tag = RESULT;
	}
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
void showdialog(Ve2 p, string title, string label)
{
	Log.label = label;
	Log.title = title;
	Log.Show(p);
	FlushBatchDraw();
	Sleep(800);
}