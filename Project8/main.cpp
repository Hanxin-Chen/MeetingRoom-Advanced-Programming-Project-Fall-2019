#include <iostream>
#include <cstdio>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cstring>
#include <map>
#include <conio.h>
#include <mysql.h> 
#include <vector>
#include "Room.h"
#include "Company.h"
#include "Reseration.h"
#include "Page.h"
using namespace std;

vector<Reservation> reservations;
map<string,Company> companies;
map<string,Room>rooms;
string cur_name;
MYSQL mysql;
int role = 0; //1-manager, 2-company
int unpaid_flag = 0;

int db_ini(MYSQL &mysql) {

	mysql_init(&mysql); //初始化数据库
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	if (mysql_real_connect(&mysql, "localhost", "root", "root", "meetingroom", 3306, NULL, 0) == NULL) {
		cout << "数据库初始化异常！";
		return -1;
	}
	return 1;
}

void room_ini(MYSQL &mysql) {
	MYSQL_RES* res;
	MYSQL_ROW row;
	string rid;
	int capacity;
	double rent;

	rooms.clear();
	string statement = "select * from room";
	
	mysql_query(&mysql, (char*)statement.c_str());
	res = mysql_store_result(&mysql);
	
	while (row = mysql_fetch_row(res)){
		rid = row[0];
		capacity = atoi(row[1]); //char* to int
		rent = atof(row[2]);// char* to double
		rooms[rid]=Room(rid, capacity, rent);
	}
	
	mysql_free_result(res);//释放结果集
}

void companies_ini(MYSQL &mysql) {
	MYSQL_RES* res;
	MYSQL_ROW row;
	
	string statement = "select * from company";

	companies.clear();
	mysql_query(&mysql, (char*)statement.c_str());
	res = mysql_store_result(&mysql);

	while (row = mysql_fetch_row(res)) {
		companies[row[2]]=Company(row[1], row[2], row[4], row[3]);
	}

	mysql_free_result(res);
}

void reservations_ini(MYSQL& mysql) {
	MYSQL_RES* res;
	MYSQL_ROW row;

	reservations.clear();
	string statement = "select * from reservation";

	mysql_query(&mysql, (char*)statement.c_str());
	res = mysql_store_result(&mysql);

	while (row = mysql_fetch_row(res)) {
		reservations.push_back(Reservation(companies[row[1]],rooms[row[2]],Time(row[3]),Time(row[4]),
			row[7],atof(row[5]),atoi(row[6]),atoi(row[8]),atoi(row[0])));
	}

	mysql_free_result(res);
}

string input_password() {
	string password; //返回密码
	char tmp; //输入字符
	while ((tmp = _getch()) != '\r') //getch输入不回显
	{
		if (tmp == 0)   //判断null
			continue;
		if (tmp == 8 && password.empty()) { //判断backspace
			continue;
		}
		if (tmp != 8) {   //不等于backspace，回显*
			password += tmp;
			putchar('*');
		}
		else {
			password.erase(password.size() - 1);  
			putchar('\b');
			putchar(' ');
			putchar('\b');
		}
		_getch();
	}
	return password;
}

int login(MYSQL &mysql) {
	string password, username;
	int role; //返回用户角色，1为管理员，2为公司专员

	cout << "请输入用户名：";
	cin >> username;
	cout << "请输入密码：";
	password = input_password();
	cout << endl;

	MYSQL_RES* res;
	MYSQL_ROW row;
	string statement = "select * from user where username= '" + username + "' and password= '"+password+"'";
	mysql_query(&mysql, (char*)statement.c_str());
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row != NULL){
		cur_name = row[1];
		role = row[3][0] - '0';
	}
	else {
		cout << "\n用户名或密码错误，请重试！\n" << endl;
		return -1;
	}

	mysql_free_result(res);//释放结果集
	return role;
}

Time getNow() {
	MYSQL_RES* res;
	MYSQL_ROW row;
	string statement = "select now()"; //获取服务器当前时间
	mysql_query(&mysql, (char*)statement.c_str());
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	Time now(row[0]);
	mysql_free_result(res);
	return now;
}

/////////////////////////////// 公司专员 /////////////////////////////////

void modifyCompanyInfo(Company& com) {
	string newCompanyName;
	string newAddress;
	string newPhoneNumber;
	string oldCompanyName = com.getCompanyName();
	cout << "\n** 若相关信息不修改，请输入[x] **\n"<<endl;
	cin.ignore(); //清除缓冲区的换行符
	cout << "请输入新的公司名称："; getline(cin,newCompanyName);
	cout << "请输入新的公司地址："; getline(cin, newAddress);
	cout << "请输入新的联系方式："; getline(cin, newPhoneNumber);
	
	if (newCompanyName != "x")
		com.setCompanyName(newCompanyName);
	if (newAddress != "x")
		com.setAddress(newAddress); 
	if (newPhoneNumber != "x")
		com.setPhoneNumber(newPhoneNumber);
	com.updateDB(mysql);

	string statement = "update reservation set company='" + com.getCompanyName() + "' where company='" + oldCompanyName + "'";
	mysql_query(&mysql, (char*)statement.c_str());
	cout << "\n**信息修改成功！**" << endl;
	reservations_ini(mysql);
	companies_ini(mysql);
}

void queryCompanyInfo(Company& com) {
	cout << "\n公司信息如下：\n" << endl;
	cout << "----------------------------------------------\n" << endl;
	cout << "\t公司名称：" << com.getCompanyName() << endl;
	cout << "\t公司地址：" << com.getAddress() << endl;
	cout << "\t联系方式：" << com.getPhoneNumber() << endl;
	cout << endl;
	cout << "\t[-1] 返回 \t[1]修改信息\n" << endl;
	cout << "----------------------------------------------\n" << endl;
	int flag; cin >> flag;
	if (flag == 1)
		modifyCompanyInfo(com);
	else if (flag == -1)
		return;
	else {
		cout << "\n输入错误，自动返回！" << endl;
		return;
	}
	
}

void queryBill(Company com) {
	
	vector<pair<int, Reservation> >res;
	map<int, int> vis;
	double total=0;
	char choice;
	unpaid_flag = 0;

	for (int i = 0; i < reservations.size(); i++) {
		if (reservations[i].getCompany().getCompanyName() == com.getCompanyName()
			&& reservations[i].getPaidFlag() == 0) {
			total += reservations[i].calTotal();
			res.push_back({ i,reservations[i] });
			unpaid_flag++;
			vis[i] = 1;
		}
	}

	if (unpaid_flag == 0) {
		cout << "----------------------------------------------\n" << endl;
		cout << "               您没有欠费账单！               \n" << endl;
		cout << "----------------------------------------------\n" << endl;
		return;
	}
	else {
		cout << "--------------------------------------------------------------------------------------------" << endl;
		cout << "编号\t房间号\t开始时间\t\t结束时间\t\t茶歇费\t瓶装水\t应缴" << endl;
		cout << "--------------------------------------------------------------------------------------------" << endl;
		for (int i = 0; i < res.size(); i++) {
			Reservation tmp = res[i].second;
			cout << res[i].first<< "\t";
			cout << tmp.getRoom().getRoomId() << "\t";
			cout << tmp.getStartTime().toString() << "\t";
			cout << tmp.getEndTime().toString() << "\t";
			cout << tmp.getServiceFee() << "\t";
			cout << tmp.getBottleNumber() << "\t";
			cout << "￥" << fixed << showpoint << setprecision(2)<<tmp.calTotal() << endl;
		}
		cout << "\n--------------------------------------------------------------------------------------------" << endl;
		cout << "\t总计：￥" <<fixed<<showpoint<<setprecision(2)<< total << endl << endl;
	}
	cout << "是否缴费？[Y/N] "; cin >> choice;
	if (choice == 'Y' || choice == 'y') {
		int cnt;
		vector<int> pay;
		total = 0;
		while (true){
			cout << "请输入缴费单总数(-1 放弃缴费): "; cin >> cnt;
			if (cnt == -1) {
				cout << "放弃缴费！" << endl;
				return;
			}
			if (cnt <= unpaid_flag && cnt > 0) {
				break;
			}
			else {
				cout << "单数错误,请重新输入!" << endl;
			}
		}
		cout << "请输入账单编号：";
		for (int i = 0; i < cnt; i++) {
			int tmp;
			cin >> tmp;
			if (vis[tmp] == 0)
				cout << "编号为：" << tmp << " 的订单不存在！" << endl;
			else {
				total += reservations[tmp].calTotal();
				pay.push_back(tmp);
			}
		}
		if (pay.size() == 0) {
			cout << "支付失败！" << endl;
			return;
		}
		cout << "支付总额：￥" << total << ", 是否立即支付？[Y/N] "; cin >> choice;
		if (choice == 'Y' || choice == 'y') {
			for (int i = 0; i < pay.size(); i++) {
				reservations[pay[i]].setPaidFlag(1);
				reservations[pay[i]].updateDB(mysql);
			}
			reservations_ini(mysql);
			cout << "支付成功！" << endl;
		}
		else {
			cout << "放弃支付！" << endl;
			return;
		}

	}
	else {
		return;
	}
}

void makeReservation(Company com) {
	string password;
	Room room;
	Time srt;
	Time end;
	int btNum;
	double serFee;
	
	string str,str2;
	int tmp;
	int flag = 0;
	
	while (true){
		cout << "================== 请填写表单(-1终止) ===================" << endl;
		cout << "============= 会议室开放时间：6:00~22:00 ================" << endl;
		while (true){
			cout << "请输入预约房间号："; cin >> str;
			if (str == "-1") {
				flag = 1;
				break;
			}
			if (rooms[str].getRoomId()!=str) {
				cout << "\n房间号不能存在！\n" << endl;
				continue;
			}
			room = rooms[str];
			break;
		}
		if (flag)
			break;
		//未考虑半小时打扫卫生时间
		while (true){
			cout << "请输入开始日期(yyyy-mm-dd)："; cin >> str;
			if (str == "-1") {
				flag = 1;
				break;
			}
			cout << "请输入开始时间(hh:mm:ss)："; cin >> str2;
			if (str2 == "-1") {
				flag = 1;
				break;
			}
			if (srt.setTime(str,str2)) {
				continue;
			}
			Time now = getNow();
			if (srt < now) {
				cout << "\n时间错误，当前服务器时间为：" << now.toString() << endl << endl;
				continue;
			}
			if (srt.getYear()==now.getYear()&&srt.getMonth()==now.getMonth()
				&&srt.getDay()==now.getDay()&&srt - now < 0.5) {
				cout << "\n不可预约半小时以内的会议室，当前服务器时间为：" << now.toString() << endl << endl;
				continue;
			}
			if (srt.getHour() < 6) {
				cout << "\n会议室在 22:00~次日6:00 不开放！\n" << endl;
				continue;
			}
			if (srt.getHour() > 22) {
				cout << "\n会议室在 22:00~次日6:00 不开放！\n" << endl;
				continue;
			}
			int flag2 = 0;
			for (int i = 0; i < reservations.size(); i++){
				if (reservations[i].getStartTime() < srt && srt < reservations[i].getEndTime()
					&& reservations[i].getRoom().getRoomId()==room.getRoomId()) {
					cout << endl;
					cout << reservations[i].getStartTime().toString() << "~"
						<< reservations[i].getEndTime().toString() << " 时间段已被预约！\n" << endl;
					flag2 = 1;
					break;
				}
			}
			if (!flag2)
				break;
		}
		if (flag)
			break;
		
		while (true) {
			cout << "请输入结束日期(yyyy-mm-dd)："; cin >> str;
			if (str == "-1") {
				flag = 1;
				break;
			}
			cout << "请输入结束时间(hh:mm:ss)："; cin >> str2;
			if (str2 == "-1") {
				flag = 1;
				break;
			}
			if (end.setTime(str, str2)) {
				continue;
			}
			
			if (end< srt) {
				cout << "\n结束时间不能早于开始时间！\n" << endl;
				continue;
			}
			
			if (end.getHour() < 6) {
				cout << "\n会议室在 22:00~次日6:00 不开放！\n" << endl;
				continue;
			}
			if (end.getHour() > 22) {
				cout << "\n会议室在 22:00~次日6:00 不开放！\n" << endl;
				continue;
			}
			int flag2 = 0;
			for (int i = 0; i < reservations.size(); i++) {
				if (srt<reservations[i].getStartTime() && reservations[i].getStartTime()<end
					&& reservations[i].getRoom().getRoomId() == room.getRoomId()) {
					cout << endl;
					cout << reservations[i].getStartTime().toString() << "~"
						<< reservations[i].getEndTime().toString() << " 时间段已被预约！\n" << endl;
					flag2 = 1;
					break;
				}
			}
			if (!flag2)
				break;
		}
		if (flag)
			break;

		while (true){
			cout << "请输入人均茶歇费(￥0、￥15、￥20、￥30)：￥";
			cin >> serFee;
			if (serFee == -1) {
				flag = 1;
				break;
			}
			if (serFee != 0 && serFee != 15 && serFee != 20 && serFee != 30) {
				cout << "\n暂不提供该服务档次！\n" << endl;
				continue;
			}
			break;
		}
		if (flag)
			break;

		while (true) {
			cout <<"房间容量："<<room.getCapacity()<< "，请输入需要准备的拼装水数量：";
			cin >> btNum;
			if (btNum == -1) {
				flag = 1;
				break;
			}
			if (btNum < 0) {
				cout << "\n数量错误！\n" << endl;
				continue;
			}
			break;
		}
		if (flag)
			break;

		password = "";
		tmp = rand() % 10; password += to_string(tmp);
		tmp = rand() % 10; password += to_string(tmp);
		tmp = rand() % 10; password += to_string(tmp);
		tmp = rand() % 10; password += to_string(tmp);
		tmp = rand() % 10; password += to_string(tmp);
		tmp = rand() % 10; password += to_string(tmp);

		Reservation newReserve(com, room, srt, end, password, serFee, btNum, 0, reservations.size() + 1);
		newReserve.insertDB(mysql);
		
		reservations_ini(mysql);
		cout << "=========================================================" << endl;
		cout << "预约成功！" << endl;
		cout << room.getRoomId() << " 房间，" << srt.toString() << "~" << end.toString() << endl;
		cout << "房间密码：" << password << endl;
		break;
	}
	
}

/////////////////////////////////////////////////////////////////////////
void function_choose1() {
	int choice = 0;
	while (choice != -1) {
		page_fun2(cur_name);
		cin >> choice;
	}
}

void function_choose2() {
	Company cur_company;
	map<string, Company>::iterator it;
	unpaid_flag = 0;

	for (it = companies.begin(); it != companies.end(); it++) {
		if (it->second.getUsername()==cur_name) {
			cur_company = it->second;
			break;
		}
	}
	
	for (int i = 0; i < reservations.size(); i++) {
		if (reservations[i].getCompany().getCompanyName() == cur_company.getCompanyName()
			&& reservations[i].getPaidFlag() == 0) {
			unpaid_flag++;
		}
	}
	
	int choice = 0;
	while (choice != -1) {
		page_fun1(cur_name);
		cin >> choice;
		switch (choice)
		{
		case 1:queryCompanyInfo(cur_company);
			break;
		case 2:makeReservation(cur_company);
			break;
		case 3:queryBill(cur_company);
			break;
		case -1:break;
		default:
			cout << "未找到此功能，请重新输入！" << endl;
			break;
		}
	}
}


int main(void)
{

	if (db_ini(mysql) == -1)
		return -1;
	
	room_ini(mysql);
	companies_ini(mysql);
	reservations_ini(mysql);

	page_welcome();
	while (role != 1 && role != 2) {
		role = login(mysql);
	}

	switch (role)
	{
	case 1: function_choose1();
		break;
	case 2:function_choose2();
	default:
		break;
	}

	mysql_close(&mysql);
	return 0;
}
