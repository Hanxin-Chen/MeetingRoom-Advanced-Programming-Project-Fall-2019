#ifndef TIME_H
#define TIME_H
#include<iostream>
#include <string>
#include <conio.h>
#include <sstream>
using namespace std;
class Time
{
public:
	Time(string,string);
	Time(string="2000-01-01 00:00:00");
	int setTime(string,string);
	int getYear() { return year; }
	int getMonth() { return month; }
	int getDay() { return day; }
	int getHour() { return hour; }
	int getMinute() { return minute; }
	int getSecond() { return second; }
	double operator -(Time);
	bool operator <(Time);
	string toString();

private:
	int year;
	int month;
	int day;
	int hour;
	int second;
	int minute;
	
	int check();
	int leapYear(int, int = 2);
};

Time::Time(string str1,string str2){ 
	setTime(str1,str2);
}

Time::Time(string str) {
	stringstream ss(str);
	string s1, s2;
	ss >> s1; ss >> s2;
	setTime(s1, s2);
}

int Time::setTime(string date,string time) { 
	//yyyy-mm-dd  hh:mm:ss
	//0123-56-89  01:34:67
	if (date.size() != 10) {
		cout << "日期格式错误！" << endl;
		return -6;
	}
	if (time.size() != 8) {
		cout << "日期格式错误！" << endl;
		return -7;
	}
	string tmp;
	tmp = date.substr(0, 4);
	year = stoi(tmp);
	tmp = date.substr(5, 2);
	month = stoi(tmp);
	tmp = date.substr(8, 2);
	day = stoi(tmp);

	tmp = time.substr(0, 2);
	hour = stoi(tmp);
	tmp = time.substr(3, 2);
	minute = stoi(tmp);
	
	if (time.size() > 5) {
		tmp = time.substr(6, 2);
		second = stoi(tmp);
	}
	else {
		second = 0;
	}
	
	return check();
}

int Time::leapYear(int y,int m) {
	if (m != 2)
		return 0;
	if (y % 4 == 0 && y % 100 != 0)
		return 1;
	if (y % 400 == 0)
		return 1;
	return 0;
}

int Time::check() {
	const int std_day[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	if (month < 1 || month>12) {
		month = 1;
		cout << "Month设置越界！" << endl;
		return -1;
	}
	if (day<1 || day>std_day[month] + leapYear(year, month)) {
		day = 1;
		cout << "Day设置越界！" << endl;
		return -2;
	}
	if (hour < 0 || hour>23) {
		hour = 0;
		cout << "Hour设置越界！" << endl;
		return -3;
	}
	if (minute < 0 || minute>59) {
		minute = 0;
		cout << "Minute设置越界！" << endl;
		return -4;
	}
	if (second < 0 || second>59) {
		second = 0;
		cout << "Second设置越界！" << endl;
		return -5;
	}
	return 0;
}

double Time::operator-(Time T) { //返回小时数,同一天内
	double h1 = hour + minute / 60.0 + second / (60.0 * 60);
	double h2 = T.hour + T.minute / 60.0 + T.second / (60.0 * 60);
	return h1 - h2;
}

bool Time::operator <(Time T) {
	if (year != T.year)
		return year < T.year;
	if (month != T.month)
		return month < T.month;
	if (day != T.day)
		return day < T.day;
	if (hour != T.hour)
		return hour < T.hour;
	if (minute != T.minute)
		return minute < T.minute;
	return second < T.second;
}

string Time::toString() {
	string res;
	res = to_string(year) + "-" + to_string(month) + "-" + to_string(day) + " " + to_string(hour) + ":" + to_string(minute) + ":" + to_string(second);
	return res;
}
#endif // !TIME_H
