#pragma once
/*º½¿Õ¶©Æ±ÏµÍ³¿ØÖÆÆ÷*/
class AirLineManager
{
private:
	AirLineAVLTree AirLineData;
	StationTable StationData;
	string Version;
	void SaveBookResult(const AVLPTR data, string name);
	Customer FindBackUp(STATIONNODEPTR node, int tickets);
	void Order_save(FILE* F1, FILE* F2, FILE* F3, AVLPTR T);
public:
	void Add(string linecode, int planenum, int date, string start, string end);
	bool Remove(string linecode);
	void Remove(int date);
	const vector<AVLPTR> SearchByStation(string start, string end);
	const AVLPTR AddCustomer(string linecode, string name, int& num);
	void AddBackup(AVLPTR T, string name, int num);
	bool RemoveCustomer(string lincode, string name);
	void Save(int date);
	void Load(string date);
	string GetVersion() {
		if (Version == "")
			return "not loaded";
		return Version;
	}
};
