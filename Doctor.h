//#pragma once
//
//#include <iostream>
//#include "Database/Mysql.h"
//#include "User/User.h"
//
//using namespace std;
//
//
////医生类
//class Doctor : public User {
//private:
//    int userId;
//    int departmentId;
//    string professionalTitle;
//    string briefIntroduction;
//
//
//public:
//    static Doctor *getDoctorByUserId(int userId);
//
//    string getProfessionalTitle() {
//        return this->professionalTitle;
//    }
//
//    string getBriefIntroduction() {
//        return this->briefIntroduction;
//    }
//
//    static void setCommonUserToDoctor();
//
//    static void addDoctor(int userId);
//
//    static void deleteDoctorByUserId(int userId);
//
//    string getName() {
//        return this->name;
//    }
//};
//
//
//Doctor *Doctor::getDoctorByUserId(int userId) {
//    User *user = User::getUserById(userId);
//    Doctor *doctor = new Doctor();
//    doctor->userId = user->id;
//    doctor->password = user->password;
//    doctor->name = user->name;
//    doctor->sex = user->sex;
//    doctor->phoneNumber = user->phoneNumber;
//    doctor->idCard = user->idCard;
//    doctor->address = user->address;
//    doctor->userType = user->userType;
//    MYSQL *Cur = connectDb();
//    MYSQL_RES *result;  //Defines the result of the select
//    char sql[MAX_SQL_LENGTH];
//    sprintf(sql, "select * from doctor where user_id=%d", userId);
//    int queryRes = mysql_query(Cur, sql);    //执行sql语句
//    if (queryRes) {
//        // error
//    } else { // query succeeded, process any data returned by it
//        result = mysql_store_result(Cur);
//        if (result) { // there are rows
//            MYSQL_ROW row = mysql_fetch_row(result);
//            doctor->id = atoi(row[0]);
//            // row[1]是user_id
//            doctor->departmentId = atoi(row[2]);
//            doctor->professionalTitle = row[3];
//            doctor->briefIntroduction = row[4];
//            return doctor;
//        }
//    }
//    return NULL;
//}
//
//void Doctor::setCommonUserToDoctor() {
//    string phoneNumber;
//    cout << "请输入需要修改为医生的用户的手机号: ";
//    cin >> phoneNumber;
//    User *user = User::findUserByPhoneNumber(phoneNumber);
//    User::setUserType(user->getId(), DOCTOR);
//    addDoctor(user->getId());
//}
//
//void Doctor::addDoctor(int userId) {
//    int departmentId;
//    string professionalTitle;
//    string briefIntroduction;
//    cout << "科室id: ";
//    cin >> departmentId;
//    cout << "职称: ";
//    cin >> professionalTitle;
//    cout << "简介: ";
//    cin >> briefIntroduction;
//
//    MYSQL *Cur = connectDb();
//    char sql[MAX_SQL_LENGTH];
//    sprintf(sql,
//            "insert into doctor (user_id, department_id,professional_title, brief_introduction) values(%d,%d,'%s','%s')",
//            userId,
//            departmentId,
//            professionalTitle.c_str(),
//            briefIntroduction.c_str());
//    mysql_query(Cur, sql);
//}
//
//void Doctor::deleteDoctorByUserId(int userId) {
//    MYSQL *Cur = connectDb();
//    char sql[MAX_SQL_LENGTH];
//    sprintf(sql, "delete from doctor where user_id=%d", userId);
//    mysql_query(Cur, sql);
//}

