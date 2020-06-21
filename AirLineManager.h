#pragma once
#define MAXTICKET 50
/*���ն�Ʊϵͳ������*/

const string PATH = "DATA\\";

class AirLineManager
{
private:
	AirLineAVLTree AirLineData;																				//������Ϣ
	StationTable StationData;																				//վ���������ͻ�������
	string Version;																							//���ݰ汾����
	void SaveBookResult(const AVLPTR data, string name);													//���涩����¼
	Customer FindBackUp(STATIONNODEPTR node, int tickets);													//Ѱ�Һ򲹳�Ա
	void AddCustomer(AVLPTR T, Customer B);																	//ֱ����ӿͻ�
public:
	void Add(string linecode, int planenum, int date, string start, string end, int remain = MAXTICKET);	//���һ������
	bool Remove(string linecode);																			//�������ɾ��
	void Remove(int date);																					//���������
	const vector<AVLPTR> SearchByStation(string start, string end);											//��վ�����
	const AVLPTR AddCustomer(string linecode, string name, int& num);										//��ӿͻ���Ϣ
	void AddBackup(AVLPTR T, string name, int num);															//��Ӻ���Ϣ
	bool RemoveCustomer(string lincode, string name);														//ɾ���ͻ���Ϣ
	void Save(int date);																					//������������
	bool Load(string date);																					//��������
	bool AutoLoad();																						//�������һ�α��������
	string GetVersion() {																					//��ȡ��ǰ��������
		if (Version == "")
			return "not loaded";
		return Version;
	}
};
