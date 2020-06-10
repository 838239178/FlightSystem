#pragma once
struct AirLineAVLNode;
typedef AirLineAVLNode* AVLPTR;
typedef AVLPTR AVLTree;
/*二维向量*/
typedef struct Position
{
    int x;
    int y;

    Position operator + (const Position& a) {
        return Position{ x + a.x, y + a.y };
    }
}Ve2;
/*客户信息*/
typedef struct Customer
{
	string Name;
	string AirLineCode;
	int TicketsNum;
	int SeatNo;

	Customer(string n, string a, int num, int no) {
		Name = n;
		AirLineCode = a;
		TicketsNum = num;
		SeatNo = no;
	}
}Customer;
/*终点结点*/
typedef struct StationNode
{
	string EndName;
	list<Customer> CustomerList;
	queue<Customer> WatingQueue;
	AVLPTR AirLineData;

	StationNode(string e = "", AVLPTR p = NULL) {
		EndName = e;
		AirLineData = p;
	}
}StationNode;
/*起点结点*/
typedef struct StationHead
{
	string StartName;
	list<StationNode> edge;

	StationHead(string s = "") { StartName = s; }
}StationHead;
typedef list<StationNode>::iterator STATIONNODEPTR;
typedef list<StationHead>::iterator STATIONHEADPTR;
/*邻接表类*/
class StationTable
{
public:
	list<StationHead> headlist;

    AVLPTR Find(string start, string end);
    bool Find(string start, string end, vector<AVLPTR>& ptrs);
	void Insert(string start, string end, AVLPTR data);
	bool Remove(string start, string end);
	bool Remove(STATIONHEADPTR start, STATIONNODEPTR end);
};
/*AVL结点*/
typedef struct AirLineAVLNode
{
    string Code;
    string EndName;
    string StartName;
    int FlightDate;
    int PlaneNo;
    int RemainTickets;
    int Height;
    AirLineAVLNode* Left;
    AirLineAVLNode* Right;
    STATIONNODEPTR  node;
    STATIONHEADPTR  head;

    AirLineAVLNode():FlightDate(),PlaneNo(){
        Left = Right = NULL;
        Height = 1;
        RemainTickets = 50;
        Code = EndName = StartName = "";
    }
}AirLineAVLNode;
/*AVL类*/
class AirLineAVLTree
{
public:
    AirLineAVLNode* root;

    AirLineAVLTree() { root = NULL; }
    int GetHeight(AVLTree A);
    AVLTree FindMax(AVLTree T);
    AVLTree FindMin(AVLTree T);
    AVLTree LeftRotation(AVLTree T);
    AVLTree RightRotation(AVLTree T);
    AVLTree LeftRightRotation(AVLTree T);
    AVLTree RightLeftRotation(AVLTree T);
    AVLTree Insert(AVLTree Key, AVLTree& T);
    AVLTree Delete(string Key, AVLTree T);
    AVLTree FindByCode(string key, AVLTree T);
    void ClearByDate(int date);
private:
    void GetPTR(AVLTree T, int date, vector<AVLPTR>& code);
};
