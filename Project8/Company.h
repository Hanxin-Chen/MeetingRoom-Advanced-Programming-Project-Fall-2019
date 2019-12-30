#ifndef COMPANY_H
#define COMPANY_H
#include<iostream>
#include<string>
#include <mysql.h>
using namespace std;
class Company
{
public:
	Company(string="",string="",string="",string="");
	void setUsername(string);
	void setCompanyName(string);
	void setPhoneNumber(string);
	void setAddress(string);
	string getAddress() { return address; }
	string getUsername() { return username; }
	string getCompanyName() { return companyName; }
	string getPhoneNumber() { return phoneNumber; }
	void updateDB(MYSQL&);
	void updateUsername(MYSQL&,string);

private:
	string username;
	string companyName;
	string phoneNumber;
	string address;
};

Company::Company(string uname,string cname,string pnum,string add){
	setUsername(uname);
	setCompanyName(cname);
	setPhoneNumber(pnum);
	setAddress(add);
}

void Company::setUsername(string uname) {
	username = uname;
}

void Company::setCompanyName(string cname) {
	companyName = cname;
}

void Company::setPhoneNumber(string pnum) {
	phoneNumber = pnum;
}

void Company::setAddress(string add) {
	address = add;
}

void Company::updateDB(MYSQL& mysql) {
	string statement = "update company set cname = '" + companyName + "', address='"+address+"', phone='"+phoneNumber+"' where username='"+username+"'";
	mysql_query(&mysql, (char*)statement.c_str());
}

void Company::updateUsername(MYSQL& mysql,string unameNew) {
	string statement = "update company set username='" + unameNew + "' where username='" + username + "'";
	mysql_query(&mysql, (char*)statement.c_str());
	statement = "update user set username='" + unameNew + "' where username='" + username + "'";
	mysql_query(&mysql, (char*)statement.c_str());
	setUsername(unameNew);
	//todo：用户名重名验证！
}

#endif // !COMPANY_H

