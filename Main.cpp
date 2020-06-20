/*line: 1183*/
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <map>
#include <queue>
#include <functional>
#include <set>
#include <iomanip>
#include <algorithm>
#include <easyx.h>
using namespace std;
#include "DataStructure.h"
#include "AirLineManager.h"
#include "UI.h"
#define EXIT	0
#define NORMAL	1
#define RESULT	2
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
Button Exit;
Button Login;
Button Auto;
Dialog Log;
Text Result(6);
string UserName;
int Tag;									//用于判断是否绘制特定窗口或结束绘制的标志
/*UI互动事件*/
void btn0_Click(Button& sender, MOUSEMSG m);
void btn1_Click(Button& sender, MOUSEMSG m);
void btn2_Click(Button& sender, MOUSEMSG m);
void btn3_Click(Button& sender, MOUSEMSG m);
void btn4_Click(Button& sender, MOUSEMSG m);
void btn5_Click(Button& sender, MOUSEMSG m);
void btn6_Click(Button& sender, MOUSEMSG m);
void auto_Click(Button& sender, MOUSEMSG m);
void login_Click(Button& sender, MOUSEMSG m);
void exit_Click(Button& sender, MOUSEMSG m);
void result_Scroll(Text& sender, MOUSEMSG m);
/*对话框延时绘制*/
void showdialog(Ve2 p,string title, string label, int time = 1000);

void OnLoad()
{
	initgraph(800, 600);
	setbkmode(TRANSPARENT);
	Tag = NORMAL;
	UserName = "none";
	CLOCK.GetTime();
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
	Login.label = "登录";
	Login.SetImage("btn.png");
	Exit.label = "退出";
	Exit.SetImage("btn.png");
	Auto.label = "最近记录";
	Auto.SetImage("btn.png");
	for (int i = 0; i < 7; i++) {
		btn[i].label = s[i];
		btn[i].SetImage("btn.png");
	}
	btn[0].Click = btn0_Click;
	btn[1].Click = btn1_Click;
	btn[2].Click = btn2_Click;
	btn[3].Click = btn3_Click;
	btn[4].Click = btn4_Click;
	btn[5].Click = btn5_Click;
	btn[6].Click = btn6_Click;
	Login.Click = login_Click;
	Exit.Click = exit_Click;
	Auto.Click = auto_Click;
	Result.Scroll = result_Scroll;
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
			for (int i = 0; i < 8; i++) {
				if (btn[i].Click != NULL)
					btn[i].Click(btn[i], m);
			}
			if (Login.Click) Login.Click(Login,m);
			if (Exit.Click) Exit.Click(Exit, m);
			if (Auto.Click) Auto.Click(Auto, m);
		}
		if (m.uMsg == WM_MOUSEWHEEL) {
			if (Result.Scroll) Result.Scroll(Result,m);
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
	settextcolor(RED);
	settextstyle(20, 0, "黑体", 0, 0, 100, 0, 0, 0);
	sprintf(s, "当前日期:%d", CLOCK.date);
	outtextxy(600, 20, s);
	outtextxy(600, 40, v.c_str());	
	v = "当前使用者:" + UserName;
	outtextxy(600, 60, v.c_str());
	//show button
	for (; p.y < 595; p.y += 80, cnt++) btn[cnt].Show(p);
	Auto.Show(Ve2{ 600,p.y - 240 });
	Login.Show(Ve2{ 600, p.y-160 });
	Exit.Show(Ve2{ 600, p.y - 80 });
}
bool BookTicket()
{
	if (SysManager.GetVersion() == "not loaded") return false;
	if (UserName == "none") return false;
	Result.Reset();
	Result.p = Ve2{ 240,90 };
	char input[100];
	string linecode;
	char s[10];
	int n;
	int temp;
	InputBox(input, 100, "航班号  票数", "按格式订票信息:");
	sscanf(input, "%s %d", s, &n);
	linecode = s;
	temp = n;
	const AVLPTR T = SysManager.AddCustomer(linecode, UserName, n);
	if (T && n > 0) {
		Result += "订票成功，您的航班为：";
		itoa(n, s, 10);
		string seat = "    座位号：";
		char t[10];
		itoa(n - temp + 1, t, 10);
		seat += t;
		if (temp > 1) {
			seat += "—";
			seat += s;
		}
		Result += T->Code + seat;
		return true;
	}
	else if (T) {
		InputBox(s, 5, "是否加入候补队列?(y/n)", "余票不足");
		if (s[0] == 'y') {
			SysManager.AddBackup(T, UserName, temp);
			return true;
		}
		char is[10];
		char ie[10];
		char ir[10]; 
		Result += "该航班余票不足，请参考以下航班：";
		Result += " ";
		Result += "航班号    飞机号  起飞时间    余票";
		const vector<AVLPTR> msg = SysManager.SearchByStation(T->StartName, T->EndName);
		if (msg.size() == 0) return false;
		for (auto i = 0; i < msg.size(); i++) {
			if (msg[i]->Code == T->Code)
				continue;
			itoa(msg[i]->PlaneNo, is, 10);
			itoa(msg[i]->FlightDate, ie, 10);
			itoa(msg[i]->RemainTickets, ir, 10);
			Result += msg[i]->Code + "        " + is + "    " + ie + "     " + ir;
		}
		return true;
	}
	return false;
}
bool RefundTiket()
{
	if (SysManager.GetVersion() == "not loaded") return false;
	if (UserName == "none") return false;
	char input[100];
	InputBox(input, 100, "航班号", "输入航班号退票:");
	if (input[0] == '\0') return false;
	string code = input;
	return SysManager.RemoveCustomer(code, UserName);
}
bool Search()
{	
	if (SysManager.GetVersion() == "not loaded") return false;
	Result.Reset();	
	Result.p = Ve2{ 240,90 };
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
		itoa(msg[i]->PlaneNo, is, 10);
		itoa(msg[i]->FlightDate, ie, 10);
		itoa(msg[i]->RemainTickets, ir, 10);
		Result += msg[i]->Code + "        " + is + "    " + ie + "     " + ir;
	}
	return true;
}
bool Write()
{
	if (SysManager.GetVersion() == "not loaded") return false;
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
	InputBox(s, 10, "年月日(EX:20200605)", "请输入日期:", 0, 100, 100);
	if (s[0] == '\0')
		return false;
	string date = s;
	SysManager.Load(date);
	return true;
}
bool Clear()
{
	if (SysManager.GetVersion() == "not loaded") return false;
	int date;
	char s[10] = { '\0' };
	InputBox(s, 10, "年月日(EX:20200605)", "请输入截止日期:",0,100,100);
	if (s[0] == '\0')
		return false;
	sscanf(s, "%d", &date);
	SysManager.Remove(date);
	return true;
}
bool LogIN()
{
	char input[15] = { '\0' };
	InputBox(input, 15, "(EX:小王)", "请输入姓名:", 0, 100, 100);
	if (input[0] == '\0')
		return false;
	UserName = input;
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
void btn0_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (!BookTicket()) {
			string s = "订票异常,";
			if (UserName == "none")
				s += "请检查是否登录";
			else if (SysManager.GetVersion() == "not loaded")
				s += "未载入数据";
			else
				s += "没有该航班";
			showdialog(Ve2{ 300,200 }, "提示", s);
		}
		else {
			Tag = RESULT;
		}
	}
}
void result_Scroll(Text& sender, MOUSEMSG m)
{

	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (m.wheel > 0)
			Result.PreLine();
		else
			Result.NextLine();
	}
}

void btn1_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if(RefundTiket())
			showdialog(Ve2{ 300,200 }, "提示:", "退票成功");
		else {
			string s = "退票异常,";
			if (UserName == "none")
				s += "请检查是否登录";
			else if (SysManager.GetVersion() == "not loaded")
				s += "当前未加载数据";
			else
				s += "您没有预定该航班";
			showdialog(Ve2{ 300,200 }, "提示", s);
		}
	}
}
void btn4_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (UserName != "administrator") {
			showdialog(Ve2{ 300,200 }, "提示:", "无权限！");
			return;
		}
		Save();
		showdialog(Ve2{ 300,200 }, "提示:", "保存成功");
		Tag = NORMAL;
	}
	
}
void btn3_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (UserName != "administrator") {
			showdialog(Ve2{ 300,200 }, "提示:", "无权限！");
			return;
		}
		if(Write())
			showdialog(Ve2{ 300,200 }, "提示:", "录入成功");
		else {
			string s;
			if (SysManager.GetVersion() == "not loaded")
				s += "当前未加载数据";
			else
				s += "输入错误";
			showdialog(Ve2{ 300,200 }, "提示:", s);
		}
		Tag = NORMAL;
	}
}
void btn5_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (UserName != "administrator") {
			showdialog(Ve2{ 300,200 }, "提示:", "无权限！");
			return;
		}
		if (Load())
			showdialog(Ve2{ 300,200 }, "提示:", "加载成功");
		else
			showdialog(Ve2{ 300,200 }, "提示:", "加载失败");
		Tag = NORMAL;
	}
}
void btn6_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (UserName != "administrator") {
			showdialog(Ve2{ 300,200 }, "提示:", "无权限！");
			return;
		}
		if (Clear())
			showdialog(Ve2{ 300,200 }, "提示:", "清除成功");
		else {
			string s;
			if (SysManager.GetVersion() == "not loaded")
				s = "未载入数据";
			else
				s = "输入错误";
			showdialog(Ve2{ 300,200 }, "提示:", s);
		}
		Tag = NORMAL;
	}
}
void btn2_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (!Search()) {
			string s;
			if (SysManager.GetVersion() == "not loaded")
				s += "当前未加载数据";
			else
				s += "暂时没有该航班";
			showdialog(Ve2{ 300,200 }, "提示:", s);
		}
		else
			Tag = RESULT;
	}
}
void login_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (LogIN())
			showdialog(Ve2{ 300,200 }, "提示:", "登陆成功");
		else {
			showdialog(Ve2{ 300,200 }, "提示:", "登陆失败");
		}
		Tag = NORMAL;
	}
}
void exit_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		Tag = EXIT;
	}
}
void auto_Click(Button& sender, MOUSEMSG m)
{
	Ve2 s = sender.p;
	Ve2 e = sender.p + sender.size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		if (UserName != "administrator") {
			showdialog(Ve2{ 300,200 }, "提示:", "无权限！");
			return;
		}
		if(SysManager.AutoLoad())
			showdialog(Ve2{ 300,200 }, "提示:", "加载成功");
		else 
			showdialog(Ve2{ 300,200 }, "提示:", "加载失败，没有最近一次的记录", 1200);
	}
}
int main()
{
	//程序开始前
	OnLoad();
	//主程序循环
	while (Tag) {
		UpDate();
		Control();
	}
	//程序结束后
	OnExit();
	return 0;
}
void showdialog(Ve2 p, string title, string label, int time)
{
	Log.label = label;
	Log.title = title;
	Log.Show(p);
	FlushBatchDraw();
	Sleep(time);
}