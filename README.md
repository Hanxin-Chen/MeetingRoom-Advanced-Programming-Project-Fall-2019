# MeetingRoom
Final Project for Advanced Programming (Fall 2019)

## 题目及要求
题目：共享会议室管理系统的设计与实现
某创业园区针对微小型创业公司提供了共享会议室。共享会议室一共7间，基本信息如下表：
|会议室|最大容量|租金（元/小时）|
|---|---|---|
|101|50|100|
|301|20|80|
|501|20|80|
|302|10|50|
|401|10|50|
|402|10|50|
|502|10|50|

会议室的使用规则如下：
1. 会议室使用前需要至少提前30分钟预约;上一场会议结束后至少需要间隔30分钟(用于整理和打扫)才能用于下一场会议。
2. 会议室预约成功后，系统将临时生成并返回一个6位数的开门密码，用于打开会议室门。
3. 预约会议室时可要求摆放瓶装水，每瓶水收费1元；可提供茶歇服务，人均收费有15、20、30三个标准供选择。
4. 每个公司只有指定的用户才有权限登录该系统并进行会议室预约。
5. 会议室的使用费用每个月月底结算一次。

要求：
根据题目描述进行系统设计，分析并确定类及类的关系；
实现会议室信息的编辑（增、删、改、查）功能，公司用户信息的编辑（增、删、改、查）功能；
实现租借会议室的预约功能，注意处理会议室时间冲突、瓶装水、茶歇预定等问题；
实现会议室费用管理功能，包括按公司进行的费用查询，欠费提醒；
实现系统整体查询功能，包括按会议室查询指定时间内的租借情况；按公司查询会议室租借情况，按月份查询会议室租借收入情况等；
实现不同角色的权限设置，根据权限进行功能菜单的显示；
使用文件存储基本信息和租借信息（可考虑建立不同的文件或文件夹）；
适当进行界面的美化。

扩展：
上月未结清费用的公司下个月不能预约会议室。
连续租用5次，可享免费使用同类型会议室一次；如果有免费使用次数，可在用户预约会议室时提醒是否使用该免费权益。

## 功能分析
共享会议室管理系统用例图
![image1](https://github.com/Hanxin-Chen/MeetingRoom/blob/master/images/image1.png)


## 类设计分析
<img src="https://github.com/Hanxin-Chen/MeetingRoom/blob/master/images/image2.png" alt="GitHub" title="GitHub,Social Coding" width="300" height="300" />
<img src="https://github.com/Hanxin-Chen/MeetingRoom/blob/master/images/image3.png" alt="GitHub" title="GitHub,Social Coding" width="300" height="300" />
<img src="https://github.com/Hanxin-Chen/MeetingRoom/blob/master/images/image4.png" alt="GitHub" title="GitHub,Social Coding" width="300" height="300" />
<img src="https://github.com/Hanxin-Chen/MeetingRoom/blob/master/images/image5.png" alt="GitHub" title="GitHub,Social Coding" width="300" height="300" />

Time类
成员数据：年、月、日、时、分、秒
成员函数：成员数据的set和get函数

Room类
成员数据：房间号、最大容量、每小时租金、房间使用状态、密码
成员数据的set和get函数
OpenTheDoor函数：参数为string类型的密码，当用户输入密码时，将其与房间密码对比，相同则开门(返回1)，反之则报错返回-1.
备注：房间使用状态为0表示当前房间空闲，为1表示当前正在使用。

Company类
成员数据：公司名称、地址、联系电话
成员函数：成员数据的set和get函数

Reservation类
成员数据：公司、会议室、预约开始时间、房间使用时间、瓶装水数量、茶歇服务费、费用是否结清标识
成员数据的set和get函数
CalFee函数：计算总费用
Show函数：以表格的形式在用户界面显示预约信息


## 功能块实现
![image1](https://github.com/Hanxin-Chen/MeetingRoom/blob/master/images/image6.png)

1. 系统初始化的流程图如图所示：首先，按照题目要求创建7个会议室对象，将其存放在Room*或者vector<Room>中(也可以用map<string, Room>存储，便于后续查询)；然后，从数据库或者txt文件中读入预约数据，放入vector<Reservation>中；最后，初始化登陆界面，用户选择角色并登陆后，可执行相应的功能。当用户退出系统后，将预约信息写入数据库或者对应文件。

2. 公司专员功能实现：
  + 查询当前空会议室：遍历vector<Room>，调用Room::getState函数，判断当前会议室状态。

  + 预约会议室：
    1. 用户输入预约房间号，时间段，需要的瓶装水数量与茶歇服务档次；根据房间号，遍历vector<Reservation>，若当前预约时间段与已预约时间段重合，或者二者预约时间段间隔小于30分钟，则返回错误信息。
    2. 如果遍历后不存在上述冲突，则新建Reservation对象；调用该对象的setCompany设置为当前用户所在公司；遍历vector<Room>找到对应房间好的Room对象，调用Reservation::setRoom将Reservation::room设置为该对象；调用Reservation对象的setStartTime、setDuration、setServiceFee、setBottleNumber函数设置相关信息；调用setFlag将付费标志置为0，即未付费。
    3. 将设置完成后的Reservation对象push back到vector<Reservation>

  + 修改预约信息：遍历vector<Reservation>调用Reservation::getCompany找到公司为当前用户的预约信息，将其输出到界面中；用户从中选取要修改的信息后，判断预约开始时间距离当前时间是否有30分钟（规定：预约信息至少要提前30分钟进行修改）；如果是，则调用Reservation::setXxx函数进行修改，其中flag标识不允许用户修改。

  + 删除预约信息：遍历vector<Reservation>调用Reservation::getCompany找到公司为当前用户的预约信息，将其输出到界面中；用户从中选取要删除的信息后，判断预约开始时间距离当前时间是否有30分钟；如果是，则删除选中的Reservation对象。

  + 公司信息的增删改查：调用Company::setXxx函数，对信息进行修改。

  + 查询账单与缴费：遍历vector<Reservation>调用Reservation::getCompany找到与当前用户所在公司匹配的预约信息，调用Reservation::getFlag判断该预约是否已经付费，若未付费则显示在用户界面。用户选择需要支付的账单调用Reservation::calFee计算费用，支付成功后调用Reservation::setFlag函数，将该预约的付费标志置为1表示已经付费。

  + 开门：用户选择对应的会议室后，调用Room::openTheDoor函数，输入密码，将其与会议室的密码进行比对，比对成功后执行开门(返回1)。

3. 管理员功能实现
  + 查询预约信息：遍历vector<Reservation>调用Reservation::show将预约信息显示在用户界面中。

  + 查询收入：遍历vector<Reservation>调用Reservation::getFlag判断该预约是否已经付费，如果已经付费，则调用Reservation::calFee计算费用，将该数组中所有已付费费用相加即可的总收入。

  + 查询当前会议室状态：遍历vector<Room>，调用Room::getState函数，判断当前会议室状态。

  + 会议室信息的增删改查：输入需要修改的会议室房间号，遍历vector<Room>选择对应的会议室，调用Room::setXxx函数进行修改。

  + 查询未缴账单：遍历vector<Reservation>调用Reservation::getFlag判断每条预约是否已经付费，若未付费则显示在用户界面，并调用Reservation的calFee函数计算出每条预约的应缴纳金额。
