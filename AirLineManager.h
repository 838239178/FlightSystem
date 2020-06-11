#pragma once
#define MAXTICKET 50
/*º½¿Õ¶©Æ±ÏµÍ³¿ØÖÆÆ÷*/
class AirLineManager
{
private:
	AirLineAVLTree AirLineData;
	StationTable StationData;
	string Version;
	void SaveBookResult(const AVLPTR data, string name);
	Customer FindBackUp(STATIONNODEPTR node, int tickets);
	void AddCustomer(AVLPTR T, Customer B);
public:
	void Add(string linecode, int planenum, int date, string start, string end, int remain = MAXTICKET);
	bool Remove(string linecode);
	void Remove(int date);
	const vector<AVLPTR> SearchByStation(string start, string end);
	const AVLPTR AddCustomer(string linecode, string name, int& num);
	void AddBackup(AVLPTR T, string name, int num);
	bool RemoveCustomer(string lincode, string name);
	void Save(int date);
	bool Load(string date);
	string GetVersion() {
		if (Version == "")
			return "not loaded";
		return Version;
	}
};
