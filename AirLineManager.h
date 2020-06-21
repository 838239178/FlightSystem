#pragma once
#define MAXTICKET 50
/*航空订票系统控制器*/

const string PATH = "DATA\\";

class AirLineManager
{
private:
	AirLineAVLTree AirLineData;																				//航班信息
	StationTable StationData;																				//站点索引及客户候补名单
	string Version;																							//数据版本日期
	void SaveBookResult(const AVLPTR data, string name);													//保存订单记录
	Customer FindBackUp(STATIONNODEPTR node, int tickets);													//寻找候补成员
	void AddCustomer(AVLPTR T, Customer B);																	//直接添加客户
public:
	void Add(string linecode, int planenum, int date, string start, string end, int remain = MAXTICKET);	//添加一条航班
	bool Remove(string linecode);																			//按航班号删除
	void Remove(int date);																					//按日期清除
	const vector<AVLPTR> SearchByStation(string start, string end);											//按站点查找
	const AVLPTR AddCustomer(string linecode, string name, int& num);										//添加客户信息
	void AddBackup(AVLPTR T, string name, int num);															//添加候补信息
	bool RemoveCustomer(string lincode, string name);														//删除客户信息
	void Save(int date);																					//保存所有数据
	bool Load(string date);																					//加载数据
	bool AutoLoad();																						//加载最近一次保存的数据
	string GetVersion() {																					//获取当前数据日期
		if (Version == "")
			return "not loaded";
		return Version;
	}
};
