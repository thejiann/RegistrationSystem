#pragma once
#include <iostream>
#include <mysql/mysql.h> //mysql提供的函数接口头文件
#include <vector>
#include <map>

using namespace std;

static MYSQL *connectDb() {
    const char *host = "localhost"; //主机名
    const char *user = "root"; //用户名
    const char *pwd = "123"; //密码
    const char *dbName = "su"; //数据库名称
    int port = 3306; //端口号

    // 创建mysql对象
    MYSQL *Cur = nullptr;
    Cur = mysql_init(Cur);

    // 连接mysql
    Cur = mysql_real_connect(Cur, host, user, pwd, dbName, port, nullptr, 0);
    return Cur;
}


// select
map<bool, vector<vector<string>>> Select(MYSQL *Cur, string sql) {
    map<bool, vector<vector<string>>> resMap;

    MYSQL_RES *result;  //Defines the result of the select
    unsigned int num_rows;

    int queryRes = mysql_query(Cur, sql.c_str());    //Execute the sql statement
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        string *rows;
        if (result)  // there are rows
        {
            MYSQL_ROW row;

            unsigned int num_fields;

            num_fields = mysql_num_fields(result);

            vector<vector<string>> rowsData;
            // Read a row at a time
            while ((row = mysql_fetch_row(result)))
            {
                vector<string> rowData;
                unsigned long *lengths;
                lengths = mysql_fetch_lengths(result);
                for(unsigned int i = 0; i < num_fields; i++)
                {
                    // call vector
                    rowData.emplace_back(row[i] ? row[i] : "NULL");
                }
                rowsData.emplace_back(rowData);
            }
            resMap.emplace(true, rowsData);
            return resMap;

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
    resMap.emplace(false, NULL);
    return resMap;
}
