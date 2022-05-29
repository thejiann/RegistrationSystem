#pragma once

#include <string>
#include "Department.h"
#include "Database/Mysql.h"
#include "User/User.h"

#define MAX_SQL_LENGTH 300

class Hao {
private:
    int id;
    int haoTypeId;
    int patientId;
public:
    static int getRestOfHaoByHaoTypeId(int haoTypeId);

    static void guahao(int patientId);

    static void fetchHao(int patientId, int haoTypeId);

    static int showPatientHaoInfo(int patientId);

    static void cancelHao(int patientId, int haoId);

    static void addHao(int haoTypeId, int amount);

    static void showAllGuahao();

    static void showMyPatientGuahao(vector<int> haoTypeIds);

    static void showHaosByHaoTypeId(int haoTypeId);
};

//号类型
class HaoType {
private:
    int id;
    int DepartmentId;
    int DoctorId;
    string Time;
    double Fee;

public:
    static void CreateHaoType();

    static void showHaoTypeInfosByDepartmentId(int departmentId);

    static vector<int> getHaoTypeIdsByDoctorsUserId(int userId);
};

void HaoType::CreateHaoType() {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    int departmentId;
    int doctorId;
    string time;
    double fee;
    cout << "请输入号的科室id:";  //问题：管理员不知道科室id，换成名称？
    cin >> departmentId;
    cout << "请输入号的医生id:";  //问题：管理员不知道医生id，换成名称？
    cin >> doctorId;
    cout << "请输入号的时间:";
    cin >> time;
    cout << "请输入号的挂号费:";
    cin >> fee;
    sprintf(sql, "insert into hao_type (department_id,doctor_id,time,fee) values('%d,'%d','%s','%lf')",
            departmentId,
            doctorId,
            time.c_str(),
            fee);
    mysql_query(Cur, sql);    //执行sql语句
}

void HaoType::showHaoTypeInfosByDepartmentId(int departmentId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from hao_type where department_id=%d", departmentId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        MYSQL_ROW row;
        int restOfHao;
        unsigned int num_fields = mysql_num_fields(result);
        while ((row = mysql_fetch_row(result))) {
            for (int i = 0; i < num_fields; i++) {
                // 遍历当前行的所有列
                if (i == 0) {
                    restOfHao = Hao::getRestOfHaoByHaoTypeId(atoi(row[i]));
                    cout << row[i];
                    cout << "\t";
                } else if (i == 1) {
                    // 科室 , 不用处理，用不上

                } else if (i == 2) {
                    // 医生id
                    Doctor *doctor = Doctor::getDoctorByUserId(atoi(row[i]));
                    cout << doctor->getName() + "\t";
                    cout << doctor->getProfessionalTitle() + "\t";
                    cout << doctor->getBriefIntroduction() + "\t";
                } else {
                    cout << row[i] << "\t";
                }
            }
            cout << to_string(restOfHao) << "\n";
        }
    }
}

vector<int> HaoType::getHaoTypeIdsByDoctorsUserId(int userId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select id from hao_type where doctor_id=%d", userId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    vector<int> resInts;
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result) { // there are rows
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result))) {
                resInts.emplace_back(atoi(row[0]));
            }
        }
        if (mysql_field_count(Cur) == 0) { // when (update || insert || delete)
            // query does not return data
            // (it was not a SELECT)
            // num_rows = mysql_affected_rows(Cur);
        } else { // mysql_store_result() should have returned data
            fprintf(stderr, "Error: %s\n", mysql_error(Cur));
        }
    }
    return resInts;
}

int Hao::getRestOfHaoByHaoTypeId(int haoTypeId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select COUNT(*) from hao where hao_type_id=%d", haoTypeId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result) { // there are rows
            MYSQL_ROW row = mysql_fetch_row(result);
            int restOfHao = atoi(row[0]);
            return restOfHao;
        }
        if (mysql_field_count(Cur) == 0) { // when (update || insert || delete)
            // query does not return data
            // (it was not a SELECT)
            // num_rows = mysql_affected_rows(Cur);
        } else { // mysql_store_result() should have returned data
            fprintf(stderr, "Error: %s\n", mysql_error(Cur));
        }
    }
    return -1;
}

void Hao::guahao(int patientId) {
    int departmentId;
    int haoTypeId;
//    HaoType::showHaoTypeInfo();
    Department::showAllDepartments();
    cout << "请选择科室: ";
    cin >> departmentId;
    system("clear");

    HaoType::showHaoTypeInfosByDepartmentId(departmentId);
    cout << "请选择： ";
    cin >> haoTypeId;

    if (Hao::getRestOfHaoByHaoTypeId(haoTypeId) <= 0) {
        cout << "没号了,请重新选择！";
    } else {
        // 有号
        Hao::fetchHao(patientId, haoTypeId);
    }
}

void Hao::fetchHao(int patientId, int haoTypeId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sqlSelect[MAX_SQL_LENGTH];
    sprintf(sqlSelect, "select id from hao where hao_type_id=%d and patient_id=-1", haoTypeId);
    int queryRes = mysql_query(Cur, sqlSelect);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result)  // there are rows
        {
            MYSQL_ROW row = mysql_fetch_row(result);
            int haoId = atoi(row[0]);
            char sqlUpdate[MAX_SQL_LENGTH];
            sprintf(sqlUpdate, "update hao set patient_id=%d where id=%d", patientId, haoId);
            mysql_query(Cur, sqlUpdate);
            cout << "挂号成功！\n";
        }
    }
}

int Hao::showPatientHaoInfo(int patientId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from hao where patient_id=%d", patientId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result)  // there are rows
        {
            MYSQL_ROW row;
            unsigned int num_fields = mysql_num_fields(result);
            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < num_fields; i++) {
                    cout << row[i];
                    cout << "\t";
                }
            }
            return 1;
        } else {
            cout << "没有号哦" << endl;
        }
    }
    return 0;
}

void Hao::cancelHao(int patientId, int haoId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sqlSelect[MAX_SQL_LENGTH];
    sprintf(sqlSelect, "update hao set patient_id=-1 where id=%d and patient_id=%d", haoId, patientId);
    mysql_query(Cur, sqlSelect);    //执行sql语句
    cout << "取消成功！";
}

void Hao::addHao(int haoTypeId, int amount) {
    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    for (int i = 0; i < amount; i++) {
        sprintf(sql, "insert into hao (hao_type_id,patient_id) values(%d,%d)", haoTypeId, -1);
        mysql_query(Cur, sql);    //执行sql语句
    }
    cout << "添加成功！" << endl;
}

void Hao::showAllGuahao() {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from hao where patient_id<>-1");
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result)  // there are rows
        {
            MYSQL_ROW row;
            unsigned int num_fields = mysql_num_fields(result);
            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < num_fields; i++) {
                    cout << row[i];
                    cout << "\t";
                }
            }
        } else {
            cout << "没有号哦" << endl;
        }
    }
}

void Hao::showMyPatientGuahao(vector<int> haoTypeIds) {
    for (int i = 0; i < haoTypeIds.size(); i++) {
        showHaosByHaoTypeId(haoTypeIds.at(i));
    }
}

void Hao::showHaosByHaoTypeId(int haoTypeId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from hao where hao_type_id=%d and patient_id<>-1", haoTypeId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result)  // there are rows
        {
            MYSQL_ROW row;
            unsigned int num_fields = mysql_num_fields(result);
            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < num_fields; i++) {
                    cout << row[i];
                    cout << "\t";
                }
            }
        }
    }
}
