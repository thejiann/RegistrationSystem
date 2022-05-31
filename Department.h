#pragma once

#include "Department.h"
#include "Database/Mysql.h"

#define MAX_SQL_LENGTH 300

#include <string>

using namespace std;

class Department {
private:
    int id;
    string name;
    string address;

public:
    Department();
    static Department *getDepartmentById(int departmentId);
    static void showAllDepartments();
    string getName() {return this->name;}
    string getAddress() {return this->address;}
};

Department *Department::getDepartmentById(int departmentId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from department where id=%d", departmentId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result) { // there are rows
            MYSQL_ROW row = mysql_fetch_row(result);
            Department *department;
            department->id = atoi(row[0]);
            department->name = row[1];
            department->address = row[2];
            return department;
        }
    }
    return NULL;
}

void Department::showAllDepartments() {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from department");
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result) { // there are rows
            {
                MYSQL_ROW row;
                unsigned int num_fields = mysql_num_fields(result);
                Department *department = new Department();
                cout.width(5);
                cout << std::left << "id";
                cout.width(20);
                cout << std::left << "name";
                cout.width(20);
                cout << std::left << "address" << endl;
                while ((row = mysql_fetch_row(result))) {
                    for (int i = 0; i < num_fields; i++) {
                        if (i == 0) {
                            cout.width(5);
                        } else {
                            cout.width(20);
                        }
                        cout << std::left << row[i];
                    }
                    cout << "\n";
                }
            }
        }
    }
}

Department::Department() {}