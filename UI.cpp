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
#include "UI.h"
//Button
void Button::SetImage(string filename)
{
	string s = "pic\\" + filename;
	loadimage(&ground, s.c_str());
	size.x = ground.getwidth();
	size.y = ground.getheight();
}
void Button::Show(Ve2 pos)
{
	p = pos;
	int w = ground.getwidth();
	int h = ground.getheight();
	settextcolor(BLACK);
	settextstyle(24, 12, "黑体",0,0,100,0,0,0);
	putimage(pos.x, pos.y, &ground);
	//根据字符串长度，居中输出
	Ve2 temp;
	temp.x = (w - label.length() * 12) / 2;
	temp.y = (h - 24) / 2;
	outtextxy(pos.x+temp.x, pos.y+temp.y, label.c_str());
}
void Button::Select(MOUSEMSG m)
{
	Ve2 s = p;
	Ve2 e = p + size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		selected = true;
	}
	else {
		selected = false;
	}
}
//Dialog
void Dialog::AddButton(Button* btn1, Button* btn2)
{
	btn_1 = btn1;
	btn_2 = btn2;
}
void Dialog::SetImage(string filename)
{
	string s = "pic\\" + filename;
	loadimage(&ground, s.c_str());
	size.x = ground.getwidth();
	size.y = ground.getheight();
}
void Dialog::Show(Ve2 pos)
{
	p = pos;
	int w = ground.getwidth();
	int h = ground.getheight();
	settextcolor(BLACK);
	settextstyle(24, 0, "黑体", 0, 0, 100, 0, 0, 0);
	putimage(pos.x, pos.y, &ground);
	outtextxy(pos.x+w/15, pos.y+h/15, title.c_str());
	string temp;
	int i = 0;
	//分割字符串输出
	do {
		if (i < label.length())
			temp = label.substr(i, 24);
		else
			temp = "";
		i += 24;
		outtextxy(pos.x+w/15, pos.y+h/3+i, temp.c_str());
	} while (temp.length());
}
void Dialog::Select(MOUSEMSG m)
{
	Ve2 s = p;
	Ve2 e = p + size;
	if (m.x > s.x&& m.y > s.y
		&& m.x < e.x && m.y < e.y) {
		selected = true;
	}
	else {
		selected = false;
	}
}
//Text
//重置文本域的内容
void Text::Reset()
{
	txt.clear();
	p.x = p.y = 0;
	size.x = size.y = 0;
}
void Text::Show(Ve2 pos)
{
	p = pos;
	settextcolor(GREEN);
	settextstyle(24, 12, "宋体", 0, 0, 1000, 0, 0, 0);
	for (int i = maxline-defaultline; i < txt.size() && i< maxline; i++) {
		outtextxy(pos.x, pos.y, txt[i].c_str());
		pos.y += 40;
	}
}
//设置文本域最大显示行数
void Text::SetLine(int n) {
	maxline = n;
	defaultline = n;
}
void Text::NextLine() {
	if (maxline < txt.size())
		maxline++;
}
void Text::PreLine() {
	if (maxline > defaultline)
		maxline--;
}