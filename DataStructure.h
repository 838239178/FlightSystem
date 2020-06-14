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

    AVLPTR Find(string start, string end);                                              //按站点查找返回第一个相关数据
    bool Find(string start, string end, vector<AVLPTR>& ptrs);                          //按站点查找返回所有相关数据
	void Insert(string start, string end, AVLPTR data);                                 //插入一条索引
	bool Remove(string start, string end);                                              //查找并删除一条索引
	bool Remove(STATIONHEADPTR start, STATIONNODEPTR end);                              //直接删除索引
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
    int GetHeight(AVLTree A);                                                   //获取高度
    AVLTree FindMaxParent(AVLTree T);                                           //获取最右边结点的父节点
    AVLTree FindMinParent(AVLTree T);                                           //获取最左边结点的父节点
    AVLTree LeftRotation(AVLTree T);                                            //左旋
    AVLTree RightRotation(AVLTree T);                                           //右旋
    AVLTree LeftRightRotation(AVLTree T);                                       //左右旋
    AVLTree RightLeftRotation(AVLTree T);                                       //右左旋
    AVLTree Insert(AVLTree Key, AVLTree& T);                                    //插入
    AVLTree Delete(string Key, AVLTree T);                                      //删除
    AVLTree FindByCode(string key, AVLTree T);                                  //查找
    void ClearByDate(int date);                                                 //清除
private:
    void Deliver(AVLTree a, AVLTree b);                                         //把a赋值给b
};
