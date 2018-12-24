//*******************************************************************
// TinyEngine����  
// ����: ������(WanLizhong)
// ����: www.wanlizhong.com 
// ����: 2013-08-02
// ����: 2018-12-13
// ��Ȩ���� 2007-2018 ������
// (C) 2007-2018 WanLizhong All Rights Reserved
//*******************************************************************

#include "T_Util.h"
#include<iostream>
using namespace std;

wchar_t * T_Util::int_to_wstring(int i)
{
	wchar_t *w_str;
	w_str = new wchar_t[20];
	if(!w_str)
	{
		delete []w_str;
		return NULL;
	}
	swprintf(w_str, 20, L"%d ", i);
	return w_str;
}

// ��������rangeָ����Χ�ĵȶ������
// ����iArrayΪ�����������������
// ����iArray�������ȶ���ΪԪ�ظ�������range������
void T_Util::GetRandomNum(int range, int* iArray)
{
	int i;
	// ��ʼ�������
	srand((unsigned)time(NULL));	
	for(i=0; i<range; ++i) 
	{
		iArray[i]=i;
	}
	for(i=range; i>0; --i) 
	{
		int r = rand()%i;
		swap(iArray[i-1], iArray[r]);
	}
}


//������������ָ�����������x��y������ʣ�б���ƶ����ٶȣ�
//xRatio��yRatioΪ�������, �����ռ�����
void T_Util::GetBevelSpeed(IN POINT startPT, IN POINT destPT, 
									OUT int& xRatio, OUT int& yRatio)
{
	float aX, bY, Bias;
	if(destPT.x > startPT.x)
	{
		aX = (float)(destPT.x - startPT.x);
	}
	else
	{
		aX = (float)(startPT.x - destPT.x);
	}

	if(destPT.y > startPT.y)
	{
		bY = (float)(destPT.y - startPT.y);	
	}
	else
	{
		bY = (float)(startPT.y - destPT.y);	
	}

	if(aX==0 || bY==0)
	{
		xRatio = 0;
		yRatio = 0;
		return;
	}

	Bias = sqrt(aX*aX+bY*bY);
	xRatio = (int)(aX/Bias);
	yRatio = (int)(bY/Bias);

	if(startPT.x-destPT.x == 0) xRatio = 0;
	if(startPT.y-destPT.y == 0) yRatio = 0;
	if(startPT.x-destPT.x > 0) xRatio = -xRatio;
	if(startPT.y-destPT.y > 0) yRatio = -yRatio;
}


//�������������������ָ������㡢�յ�������ƶ��ٶ�
//���Ҫ��������ֱ���ƶ��ҳ����ٶ�Ϊָ���ƶ��ٶ�
//������������̱߶�Ӧ�ķ����ٶ�
//xRatio��yRatioΪ�������, �����ռ�����
//���������Զ��жϷ���
//	xRatio��yRatioΪ��ֵ����ʾ���ҡ����ƶ�
//	xRatio��yRatioΪ��ֵ����ʾ�������ƶ�
void T_Util::GetBevelSpeed(IN POINT startPT, IN POINT destPT, IN int Speed, 
									 OUT int& xRatio, OUT int& yRatio)
{
	float aX,bY;
	if(destPT.x > startPT.x)
	{
		aX = (float)(destPT.x - startPT.x);
	}
	else
	{
		aX = (float)(startPT.x - destPT.x);
	}

	if(destPT.y > startPT.y)
	{
		bY = (float)(destPT.y - startPT.y);	
	}
	else
	{
		bY = (float)(startPT.y - destPT.y);	
	}

	if(aX==0 || bY==0)
	{
		xRatio = 0;
		yRatio = 0;
		return;
	}

	if(bY>aX)
	{
		yRatio = Speed;
		xRatio = (int)((Speed*aX)/bY);
	}
	else
	{
		xRatio = Speed;
		yRatio = (int)((Speed*bY)/aX);
	}
	if(startPT.x-destPT.x == 0) xRatio = 0;
	if(startPT.y-destPT.y == 0) yRatio = 0;
	if(startPT.x-destPT.x > 0) xRatio = -xRatio;
	if(startPT.y-destPT.y > 0) yRatio = -yRatio;
}

string T_Util::Trim(string & str)
{
	str.erase(0, str.find_first_not_of("\t\r\n"));
	str.erase(str.find_last_not_of("\t\r\n") + 1);
	return str;
}

wstring T_Util::StringToWstring(const string & str)
{
	wstring wstr(str.length(), L' ');
    copy(str.begin(), str.end(), wstr.begin());
    return wstr;
}

vector<MOTASPINFO> T_Util::ParseCsv(const char * filePath)
{
	ifstream fin(filePath);
	vector<MOTASPINFO> motaInfos;
	string line;
	while (getline(fin, line))
	{
		istringstream sin(line);
		MOTASPINFO temp;
		vector<string>fields;
		string field;
		while (getline(sin, field, ',')) //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ���
		{
			fields.push_back(field); //���ոն�ȡ���ַ�����ӵ�����fields��
		}
		temp.ImaPath = StringToWstring(Trim(fields[0]));
		temp.Aggressivity = stoi(Trim(fields[1]));
		temp.Defense = stoi(Trim(fields[2]));
		temp.LifeValue = stoi(Trim(fields[3]));
		temp.SpBasicInfo.Score = stoi(Trim(fields[4]));
		temp.Money = stoi(Trim(fields[5]));
		temp.SpBasicInfo.X = stoi(Trim(fields[6])) * 33;
		temp.SpBasicInfo.Y = stoi(Trim(fields[7])) * 33;
		temp.RoleType = stoi(Trim(fields[8]));
		temp.SpBasicInfo.Active = false;
		temp.SpBasicInfo.Alpha = 255;
		temp.SpBasicInfo.Dead = false;
		temp.SpBasicInfo.Rotation = TRANS_NONE;
		temp.SpBasicInfo.Dir = DIR_DOWN;
		temp.SpBasicInfo.Speed = 0;
		temp.SpBasicInfo.Ratio = 1.0f;
		temp.SpBasicInfo.Visible = true;
		temp.SpBasicInfo.Level = 0;
		temp.DoorOpen = 0;
		motaInfos.push_back(temp);
	}
	return motaInfos;
}

