#pragma once

#include <string>
#include "Department.h"
#include "Database/Mysql.h"
#include "User/User.h"
#include "bits/stdc++.h"

#define MAX_SQL_LENGTH 300

//号
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

    static void yicixing();
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

    static HaoType *getHaoTypeInfoById(int haoTypeId);

    static void showHaoTypeInfoByIdWithoutHeader(int haoTypeId);

    static vector<int> getHaoTypeIdsByDoctorId(int doctorId);  //通过科室编号显示号类型信息

    static bool haveHaoType(int haoTypeId);

    int getId() { return this->id; };

    int getDepartmentId() { return this->DepartmentId; };

    int getDoctorId() { return this->DoctorId; };

    string getTime() { return this->Time; };

    double getFee() { return this->Fee; };
};

void HaoType::CreateHaoType() {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    int departmentId;
    int doctorId;
    string time;
    double fee;
    cout << "请输入号的科室id:";
    cin >> departmentId;
    cout << "请输入号的医生id:";
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
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        if (mysql_field_count(Cur) == 0) { // when (update || insert || delete)
            cout << "添加成功！" << endl;
            return;
        } else { // mysql_store_result() should have returned data
            fprintf(stderr, "Error: %s\n", mysql_error(Cur));
        }
    }
    string errorInfo = mysql_error(Cur);
    cout << "添加失败！原因:[" + errorInfo + "]" << endl;
}

//通过科室编号显示号类型信息
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

        cout.width(4);
        cout << std::left << "id";
        cout.width(12);
        cout << std::left << "name";
        cout.width(28);
        cout << std::left << "professional title";
        cout.width(108);
        cout << std::left << "brief introduction";
        cout.width(16);
        cout << std::left << "time";
        cout.width(8);
        cout << std::left << "fee";
        cout.width(10);
        cout << std::left << "remaining" << endl;
        if (result) {
            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < num_fields; i++) {
                    // 遍历当前行的所有列
                    if (i == 0) {
                        restOfHao = Hao::getRestOfHaoByHaoTypeId(atoi(row[i]));
                        cout.width(3);
                        cout << std::left << row[i];
                        cout << "\t";
                    } else if (i == 1) {
                    } else if (i == 2) {
                        // 医生id
                        Doctor *doctor = Doctor::getDoctorByDoctorId(atoi(row[i]));
                        if (doctor == NULL) {
                            cout << "医生信息获取失败！" << endl;
                            return;
                        }
                        cout.width(3);
                        cout << std::left << doctor->getName();
                        cout << "\t";
                        cout.width(27);
                        cout << std::left << doctor->getProfessionalTitle();
                        cout << "\t";
                        cout.width(105);
                        cout << std::left << doctor->getBriefIntroduction();
                        cout << "\t";
                    } else if (i == 3) {
                        cout.width(15);
                        cout << std::left << row[i];
                        cout << "\t";
                    } else {
                        cout.width(5);
                        cout << std::left << row[i];
                        cout << "\t";
                    }
                }
                cout << to_string(restOfHao) << "\n";
            }
            return;
        } else  // mysql_store_result() returned nothing: should it have?
        {
            if (mysql_field_count(Cur) == 0) { // when (update || insert || delete)
                // query does not return data
                // (it was not a SELECT)
                // num_rows = mysql_affected_rows(Cur);
            } else // mysql_store_result() should have returned data
            {
                fprintf(stderr, "Error: %s\n", mysql_error(Cur));
            }
        }
    }
    cout << "当前选择的科室没有号！" << endl;
}

vector<int> HaoType::getHaoTypeIdsByDoctorId(int doctorId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select id from hao_type where doctor_id=%d", doctorId);
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
            return resInts;
        }
        if (mysql_field_count(Cur) == 0) { // when (update || insert || delete)
            // query does not return data
            // (it was not a SELECT)
            // num_rows = mysql_affected_rows(Cur);
        } else { // mysql_store_result() should have returned data
            fprintf(stderr, "Error: %s\n", mysql_error(Cur));
        }
    }
    return vector<int>{};
}

bool HaoType::haveHaoType(int haoTypeId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from hao_type where id=%d", haoTypeId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result) { // there are rows
            return true;
        }
    }
    return false;
}

HaoType *HaoType::getHaoTypeInfoById(int haoTypeId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from hao_type where id=%d", haoTypeId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error

    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        MYSQL_ROW row;
        HaoType *haoType = new HaoType();
        row = mysql_fetch_row(result);
        haoType->id = atoi(row[0]);
        haoType->DepartmentId = atoi(row[1]);
        haoType->DoctorId = atoi(row[2]);
        haoType->Time = row[3];
        haoType->Fee = atof(row[4]);
        return haoType;
    }
    return NULL;
}

void HaoType::showHaoTypeInfoByIdWithoutHeader(int haoTypeId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from hao_type where id=%d", haoTypeId);
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
                    cout.width(15);
                    cout << std::left << row[i];
                    cout << "\t";
                } else if (i == 1) {
                } else if (i == 2) {
                    // 医生id
                    Doctor *doctor = Doctor::getDoctorByDoctorId(atoi(row[i]));
                    if (doctor == NULL) {
                        cout << "医生信息获取失败！" << endl;
                        return;
                    }
                    cout.width(10);
                    cout << std::left << doctor->getName();
                    cout << "\t";
                } else if (i == 3) {
                    cout.width(15);
                    cout << std::left << row[i];
                    cout << "\t";
                } else {
                    cout.width(5);
                    cout << std::left << row[i];
                    cout << "\t";
                }
            }
            cout << "\n";
//            cout << to_string(restOfHao) << "\n";
        }
    }
    return;
}

int Hao::getRestOfHaoByHaoTypeId(int haoTypeId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select COUNT(*) from hao where hao_type_id=%d and patient_id=-1", haoTypeId);
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
    //显示所有的科室
    Department::showAllDepartments();
    cout << "请选择科室: ";
    cin >> departmentId;
    system("clear");

    HaoType::showHaoTypeInfosByDepartmentId(departmentId);
    cout << "请选择相应编号： ";
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
    sprintf(sql, "select id, hao_type_id from hao where patient_id=%d", patientId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    int haoTypeId;
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        int i = 0;
        if (result)  // there are rows
        {
            cout.width(8);
            cout << std::left << "hao_id";
            cout.width(15);
            cout << std::left << "hao_type_id";
            cout.width(15);
            cout << std::left << "doctor_name";
            cout.width(13);
            cout << std::left << "time";
            cout.width(8);
            cout << std::left << "fee" << endl;;
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result))) {
                haoTypeId = atoi(row[1]);
                cout.width(8);
                cout << row[0];
                HaoType::showHaoTypeInfoByIdWithoutHeader(haoTypeId);
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
    if (HaoType::haveHaoType(haoTypeId)) {
        MYSQL *Cur = connectDb();
        char sql[MAX_SQL_LENGTH];
        for (int i = 0; i < amount; i++) {
            sprintf(sql, "insert into hao (hao_type_id,patient_id) values(%d,%d)", haoTypeId, -1);
            mysql_query(Cur, sql);    //执行sql语句
        }
        cout << "添加成功！" << endl;
    } else {
        cout << "输入的hao_type_id不存在！" << endl;
    }

}

void Hao::showAllGuahao() {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from hao where patient_id<>-1");
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    cout.width(20);
    cout << std::left << "号id";
    cout.width(20);
    cout << std::left << "号类型id";
    cout.width(20);
    cout << std::left << "挂号患者id" << endl;
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
                    cout.width(20);
                    cout << std::left << row[i];
                }
                cout << "\n";
            }
        } else {
            cout << "没有号哦" << endl;
        }

    }
}

void Hao::showMyPatientGuahao(vector<int> haoTypeIds) {
    cout.width(20);
    cout << std::left << "hao_id";
    cout.width(20);
    cout << std::left << "patient_name";
    cout.width(20);
    cout << std::left << "phone_number";
    cout.width(20);
    cout << std::left << "doctor_name";
    cout.width(20);
    cout << std::left << "time";
    cout.width(20);
    cout << std::left << "fee" << endl;
    for (int i = 0; i < haoTypeIds.size(); i++) {
        showHaosByHaoTypeId(haoTypeIds.at(i));
    }
}

void Hao::showHaosByHaoTypeId(int haoTypeId) {
    HaoType *haoType = HaoType::getHaoTypeInfoById(haoTypeId);
    Doctor *doctor = Doctor::getDoctorByDoctorId(haoType->getDoctorId());
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select id,patient_id from hao where hao_type_id=%d and patient_id<>-1", haoTypeId);
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
                cout.width(18);
                cout << std::left << row[0] << "\t";
                User *user = User::getUserById(atoi(row[1]));
                cout.width(18);
                cout << std::left << user->getName() << "\t";
                cout.width(18);
                cout << std::left << user->getPhoneNumber() << "\t";
                cout.width(19);
                cout << std::left << doctor->getName() << "\t";
                cout.width(17);
                cout << std::left << haoType->getTime() << "\t";
                cout.width(20);
                cout << std::left << haoType->getFee() << "\t\n";
            }
        }
    }
}

//一次性添加号
void Hao::yicixing() {
    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    for (int i = 10; i <= 79; i++) {
        for (int j = 0; j < 5; j++) {
            sprintf(sql, "insert into hao (hao_type_id,patient_id) values(%d,%d)", i, -1);
            mysql_query(Cur, sql);    //执行sql语句
        }
    }
}
