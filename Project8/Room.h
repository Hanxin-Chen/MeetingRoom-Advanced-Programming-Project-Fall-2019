#ifndef ROOM_H
#define ROOM_H
#include <iostream>
#include <string>
#include <mysql.h>
using namespace std;
class Room
{
public:
	Room(string="",int=0,double=0);
	void setRoomId(string);
	void setCapacity( int);
	void setRent(double);
	string getRoomId();
	int getCapacity();
	double getRent();
	void updateDB(MYSQL&);
	//void print();

private:
	string roomId;
	int capacity;
	double rent;
};

Room::Room(string rid,int cap,double ren){
	setRoomId(rid);
	setCapacity(cap);
	setRent(ren);
}

void Room::setRoomId(string rid) {
	roomId = rid;
}

void Room::setCapacity(int cap) {
	capacity = cap > 0 ? cap : 0;
}

void Room::setRent(double ren) {
	rent = ren > 0 ? ren : 0;
}

string Room::getRoomId() {
	return roomId;
}

int Room::getCapacity(){
	return capacity;
}

double Room::getRent() {
	return rent;
}

void Room::updateDB(MYSQL &mysql) {
	string statement = "update room set rent="+to_string(rent)+", capacity="+to_string(capacity)+" where rid="+roomId;
	mysql_query(&mysql, (char*)statement.c_str());
}

#endif // !ROOM_H
