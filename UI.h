#pragma once
class Button
{
private:
	IMAGE ground;
	bool selected;
public:	
	string label;
	Ve2 p;
	Ve2 size;
	void (*Click)(Button& sender, MOUSEMSG e);
	void Select(MOUSEMSG);
	void SetImage(string filename);
	void Show(Ve2 pos);
};
class Dialog
{
private:
	IMAGE ground;
	Button* btn_1;
	Button* btn_2;
	bool selected;
public:	
	string title;
	string label;
	Ve2 p;
	Ve2 size;
	void (*Click)(Dialog& sender, MOUSEMSG e);
	void Select(MOUSEMSG);
	void SetImage(string filename);
	void Show(Ve2 pos);
	void AddButton(Button* btn1, Button* btn2 = NULL);
};
class Text
{
private:
	vector<string> txt;
public:	
	Ve2 p;
	Ve2 size;
	void Show(Ve2 pos);
	void Reset();
	void operator += (const string& s) { txt.push_back(s); }
};
