#ifndef RESERVATION_H
#define RESERVATION_H
#include <ctime>
#include <iostream>
#include <string>
#include <cstring>
#include "Room.h"
#include "Company.h"
#include "Time.h"
#include <mysql.h>
using namespace std;
class Reservation
{
public:
	Reservation(Company,Room,Time,Time,string="999999",double=0,int=0,int=0,int=0);
	double calTotal();
	
	void setStartTime(Time st) { startTime = st; }
	void setEndTime(Time ed) { endTime = ed; }
	void setRoom(Room ro) { room = ro; }
	void setCompany(Company cp) { company = cp; }
	void setServiceFee(double sf) { serviceFee = sf; }
	void setBottleNumber(int bn) { bottleNumber = bn; }
	void setPaidFlag(int pd) { paidFlag = pd; }
	void setPassword(string pw) { password = pw; }
	
	int getReid() { return reid; }
	Time getStartTime() { return startTime; }
	Time getEndTime() { return endTime; }
	Room getRoom() { return room; }
	Company getCompany() { return company; }
	double getServiceFee() { return serviceFee; }
	int getBottleNumber() { return bottleNumber; }
	int getPaidFlag() { return paidFlag; }
	string getPassword() { return password; }

	void updateDB(MYSQL&);
	void insertDB(MYSQL&);

private:
	int reid;
	Time startTime;
	Time endTime;
	Room room;
	Company company;
	double serviceFee;
	int bottleNumber;
	int paidFlag;
	string password;
};

Reservation::Reservation(Company cmp, Room ro, Time st, Time ed, string pw, double sf, int bn , int pd ,int id)
	:company(cmp),room(ro),startTime(st),endTime(ed),password(pw),serviceFee(sf),bottleNumber(bn),paidFlag(pd),reid(id){}

double Reservation::calTotal() {
	double res = 0;
	res += serviceFee * room.getCapacity();
	res += (endTime - startTime)*room.getRent();
	res += bottleNumber;
	return res;
}

void Reservation::updateDB(MYSQL& mysql) {
	string statement = "update reservation set company = '" + company.getCompanyName() + "', room=" + room.getRoomId()+ ", startTime='" + startTime.toString()
		+"', endTime='" +endTime.toString()+ "', service="+to_string(serviceFee)+", bottle="+to_string(bottleNumber)+", password='"+password 
		+"', paid="+to_string(paidFlag)+ " where reid=" + to_string(reid);
	mysql_query(&mysql, (char*)statement.c_str());
}

void Reservation::insertDB(MYSQL& mysql) {
	string statement = "insert into reservation (company, room, startTime, endTime, service, bottle, password, paid) values ('" + company.getCompanyName()
		+ "', '" + room.getRoomId() + "', '" + startTime.toString() + "', '" + endTime.toString() + "', '"
		+ to_string(serviceFee) + "', '" + to_string(bottleNumber) + "', '" + password + "', '" + to_string(paidFlag) + "')";
	mysql_query(&mysql, (char*)statement.c_str());
}

#endif // ! RESERVATION_H
