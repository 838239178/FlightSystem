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
	settextstyle(24, 0, "ºÚÌå",0,0,100,0,0,0);
	putimage(pos.x, pos.y, &ground);
	outtextxy(pos.x+w/3, pos.y+h/3, label.c_str());
}
//Dialog
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
	settextstyle(24, 0, "ºÚÌå", 0, 0, 100, 0, 0, 0);
	putimage(pos.x, pos.y, &ground);
	outtextxy(pos.x+w/15, pos.y+h/15, title.c_str());
	outtextxy(pos.x+w/15, pos.y+h/3, label.c_str()); 
}
//Text
void Text::Show(Ve2 pos)
{
	p = pos;
	for (int i = 0; i < txt.size(); i++) {
		outtextxy(pos.x, pos.y, txt[i].c_str());
		pos.y += 10;
	}
}