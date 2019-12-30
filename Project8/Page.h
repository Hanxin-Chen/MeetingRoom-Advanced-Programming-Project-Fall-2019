#ifndef PAGE_H
#define PAGE_H
#include<iostream>
using namespace std;

void page_welcome() {
	cout << "----------------------------------------------\n" << endl;
	cout << "              欢迎使用共享会议室              \n" << endl;
	cout << "----------------------------------------------\n" << endl;
}

void page_fun1(string name) {
	cout << "\n----------------------------------------------\n" << endl;
	cout << "  您好，" + name << endl;
	cout << "\n             请输入对应数字选择功能           \n" << endl;
	cout << "  [1] 查询与公司信息" << endl;
	cout << "  [2] 预约会议室" << endl;
	cout << "  [3] 查询账单与缴费" << endl;
	cout << "  [4] 查询与修改预约信息" << endl;
	cout << "  [-1] 退出系统\n" << endl;
	cout << "----------------------------------------------\n" << endl;
	cout << "功能选择：";
}

void page_fun2(string name) {
	cout << "\n----------------------------------------------\n" << endl;
	cout << "  您好，" + name << endl;
	cout << "\n             请输入对应数字选择功能           \n" << endl;
	cout << "  [1] 查询与公司信息" << endl;
	cout << "  [2] 预约会议室" << endl;
	cout << "  [3] 查询账单" << endl;
	cout << "  [4] 查询与修改预约信息" << endl;
	cout << "  [-1] 退出系统\n" << endl;
	cout << "----------------------------------------------\n" << endl;
	cout << "功能选择：";
}
#endif // !PAGE_H
