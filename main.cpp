/*
题目14：职工电话号码查询系统
设计散列表，实现职工电话号码查询系统，要求：
   （1）每个记录包括下列内容：
       （电话号码（11位）、姓名、性别、院系名）总记录数不少于300条。
   （2）从文件中读入各记录，分别以电话号码和用户名为关键字建立散列表
   （散列函数要求使用除留余数法，采用开 放定址法处理冲突）。
   （3）查找并显示给定电话号码的记录，并输出查找长度。
   （4）查找并显示给定用户名的记录（若有同名记录，则输出全部同名用户信息），并输出查找长度。
   （5）从文件读入各记录，以院系名为关键字建立散列表
   （散列函数要求使用除留余数法，采用链地址法处理冲突）。
   （6）查找并显示给定院系的所有职工信息。
*/ 
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std; 
#define HASH_TABLE_MAX_LENGTH 18

typedef struct Record {
	bool empty = true;
	string phone;
	string name;
	bool sex;
	string department;
} Record, *pRecord;
typedef struct RecordList {
	pRecord records;
	int length = 0;
	int m;
} RecordList, *pRecordList;

typedef struct RecordNode {
	Record record;
	struct RecordNode *next = NULL;
} RecordNode, *pRecordNode;
typedef struct RecordLinkList {
	pRecordNode recordNodes;
	int length = 0;
} RecordLinkList, *pRecordLinkList;

bool load_data(
	pRecordList hash_table_open_address_phone,
	pRecordList hash_table_open_address_name,
	pRecordLinkList hash_table_link_department,
	string data_file_path
);
void load_menu();
int getMaxPrime(int number);
int simple_hash(string str, int hash_table_length);
// 电话号码和用户名使用的开放地址法建立散列表 
void add_hash_table_open_address(RecordList *recordList, string name, string phone, bool sex, string department);
void add_hash_table_table_link(RecordList *recordList, string name, string phone, bool sex, string department);

int main(void) {
	RecordList hash_table_open_address_phone;
	hash_table_open_address_phone.records = new Record[HASH_TABLE_MAX_LENGTH];
	hash_table_open_address_phone.m = getMaxPrime(HASH_TABLE_MAX_LENGTH);
	RecordList hash_table_open_address_name;
	hash_table_open_address_name.records = new Record[HASH_TABLE_MAX_LENGTH];
	hash_table_open_address_name.m = getMaxPrime(HASH_TABLE_MAX_LENGTH);
	RecordLinkList hash_table_link_department;
	hash_table_link_department.recordNodes = new RecordNode[HASH_TABLE_MAX_LENGTH];
	if(!load_data(&hash_table_open_address_phone, &hash_table_open_address_name, &hash_table_link_department, "C:\\Users\\Wuifan\\Desktop\\data.txt")) {
		cout << "数据加载失败！" ;
		return 1;
	}
	int position;
	string op, value;
	while (true)
	{
		cout << "欢迎使用职工电话号码查询系统！" << endl;
		cout << "1. 电话号码查询\t\t2. 用户名查询" << endl;
		cout << "3. 院系职工查询\t\t0. 退出" << endl;
		cout << "请选择您要执行的操作:" << endl;
		cin >> op;
		if (op == "0")
		{
			break;
		}
		else if (op == "1" || op == "2" || op == "3")
		{
			cout << "请输入查询的值：" << endl;
			cin >> value;
			cout << endl;
			if (op == "1")
			{
				position = simple_hash(value, hash_table_open_address_phone.m);
				bool has_record = false;
				int count = 0;
				while(count<HASH_TABLE_MAX_LENGTH&&!hash_table_open_address_phone.records[position].empty) {
					if(hash_table_open_address_phone.records[position].phone==value) {
						if(!has_record) {
							cout << "电话号码查询结果如下：" << endl ;
							has_record = true;
						}
						cout<<"院系："<<hash_table_open_address_phone.records[position].department <<"姓名："<<hash_table_open_address_phone.records[position].name << "，性别："<<(hash_table_open_address_phone.records[position].sex?"男":"女") << "，电话："<<hash_table_open_address_phone.records[position].phone  << "（查找长度："<<(count+1)<<"）"<< endl;
					}
					position=(position+1)%HASH_TABLE_MAX_LENGTH;
					count++;
				}
				if(!has_record) {
					cout << "找不到该用户。" << endl;
				}
			}
			else if (op == "2")
			{
				position = simple_hash(value, hash_table_open_address_name.m);
				bool has_record = false;
				int count = 0;
				while(count<HASH_TABLE_MAX_LENGTH&&!hash_table_open_address_name.records[position].empty) {
					if(hash_table_open_address_name.records[position].name == value) {
						if(!has_record) {
							cout << "用户名查询结果如下：" << endl ;
							has_record = true;
						}
						cout << "院系："<<hash_table_open_address_name.records[position].department <<"姓名："<<hash_table_open_address_name.records[position].name << "，性别："<<(hash_table_open_address_name.records[position].sex?"男":"女") << "，电话："<<hash_table_open_address_name.records[position].phone<< "（查找长度："<<(count+1)<<"）" << endl;
					}
					position=(position+1)%HASH_TABLE_MAX_LENGTH;
					count++;
				}
				if(!has_record) {
					cout << "找不到该用户名。" << endl;
				}
			}
			else if (op == "3")
			{
				position = simple_hash(value, HASH_TABLE_MAX_LENGTH);
				pRecordNode recordNode = &hash_table_link_department.recordNodes[position];
				if(!recordNode->next) {
					cout << "找不到该院系。" << endl;
				} else {
					cout << "院系查询结果如下：" << endl;
					recordNode = recordNode->next;
					while(recordNode) {
						if(recordNode->record.department==value)
						cout << "姓名："<<recordNode->record.name << "，性别："<<(recordNode->record.sex?"男":"女") << "，电话："<<recordNode->record.phone << endl;
						recordNode = recordNode->next;
					}
				}
			}
		}
		else
		{
			cout << "没有找到您选择的操作，请重新选择！" << endl;
		}
		cout << endl
			 << endl;
	}
	return 0;
}

bool add_hash_table_open_address(pRecordList recordList, string key, string name, string phone, bool sex, string department) {
	int position = simple_hash(key, recordList->m);
	if(recordList->length >= HASH_TABLE_MAX_LENGTH) {
		cout<<"数据数超过开放地址法散列表最大长度！"<<endl;
		return false;
	}
	while(!recordList->records[position].empty) {
		position = (position + 1) % HASH_TABLE_MAX_LENGTH;
	}
	recordList->length++;
	recordList->records[position].empty = false;
	recordList->records[position].name = name;
	recordList->records[position].phone = phone;
	recordList->records[position].sex = sex;
	recordList->records[position].department = department;
	return true;
}


void add_hash_table_table_link(pRecordLinkList recordLinkList, string key, string name, string phone, bool sex, string department) {
	int position = simple_hash(key, HASH_TABLE_MAX_LENGTH);
	pRecordNode recordNode = &recordLinkList->recordNodes[position];
	while(recordNode->next) {
		recordNode = recordNode->next;
	}
	pRecordNode newRecordNode = new RecordNode;
	recordLinkList->length++;
	newRecordNode->record.name = name;
	newRecordNode->record.phone = phone;
	newRecordNode->record.sex = sex;
	newRecordNode->record.department = department;
	recordNode->next = newRecordNode;
}
/*
 加载数据
 参数 hash_table_open_address_phone 开放地址法的电话号码数组地址
 参数 hash_table_open_address_name 开放地址法的用户名数组地址
 参数 hash_table_link_department 链地址法的院系名记录指针数组地址
 加载成功返回 true， 失败返回 false 
*/
bool load_data(
	pRecordList hash_table_open_address_phone,
	pRecordList hash_table_open_address_name,
	pRecordLinkList hash_table_link_department,
	string data_file_path
) {
	string name, phone, _sex, department;
	bool sex;
	int position;
	ifstream in_filestream(data_file_path);
	if (in_filestream)
	{
		while (
			(in_filestream >> name) && (in_filestream >> phone) && (in_filestream >> _sex) && (in_filestream >> department))
		{
			sex = _sex=="1";
			// 添加到电话号码散列表 
			if(!add_hash_table_open_address(hash_table_open_address_phone, phone, name, phone, sex, department)) {
				return false;
			}
			// 添加到用户名散列表 
			if(!add_hash_table_open_address(hash_table_open_address_name, name, name, phone, sex, department)) {
				return false;
			}
			// 添加到院系散列表 
			add_hash_table_table_link(hash_table_link_department, department, name, phone, sex, department);
		}
		in_filestream.close();
		in_filestream.clear();
		return true;
	}
	return false;
}

int simple_hash(string str, int hash_table_length)
{
	int number = 0;
	for (int i = 0; i != str.length(); i++)
	{
		number += toascii(str[i]);
	}
	return number % hash_table_length;
}
int isPrime(int number) {
	if(number<=1) return false;
	for(int i=2;i<number;i++) {
		if(number%i==0) {
			return false;
		}
	}
	return true;
}
int getMaxPrime(int number)
{
	int i;
	for (i = number; i > 1; i--)
	{
		if (isPrime(i))
			return i;
	}
	return 1;
}

