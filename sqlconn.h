#include <iostream>
#include <string>
#include <vector>
#include <mysql/mysql.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

const string HOST = "localhost";
const string USR = "root";
const string PWD = "";
const string DB = "students";
const int TIMEOUT = 6e7;

vector<string> trans1 = {"begin", "update stus set age = 3 where id = 3", "insert into stus values (5, 'keqing', 20, 'math')", "commit", "select * from stus"};

vector<string> trans2 = {"begin", "update stus set age = 10000 where id = 1", "select * from stus", "commit"};

class sqlconn
{
private:
    MYSQL *conn;
    vector<vector<string>> buffer;

public:
    sqlconn() : conn(nullptr)
    {
    }

    ~sqlconn()
    {
        if (conn)
            mysql_close(conn);
    }

    void sqlInit(string host, string user, string password, string database)
    {
        conn = mysql_init(conn);
        if (!conn)
        {
            cerr << "initialize failed! " << mysql_error(conn) << endl;
            exit(-1);
        }
        /* 设置自动重连 */
        char optval = 1;
        mysql_options(conn, MYSQL_OPT_RECONNECT, &optval);
        auto p = mysql_real_connect(conn, HOST.c_str(), USR.c_str(), PWD.c_str(), DB.c_str(), 0, NULL, 0);
        if (!p)
        {
            cerr << "connnected failed! " << mysql_error(conn) << endl;
            exit(-1);
        }
    }

    int query(string cmd, int index)
    {
        const char* str = cmd.c_str();
        int res = -1;
        /* 检测+重连 */
        auto start = steady_clock::now();
        while(duration_cast<microseconds>(steady_clock::now() - start).count() < TIMEOUT)
        {
            if (!mysql_ping(conn))
            {
                res = 0;
                break;
            }
        }
        if(res == -1)
        {
            cerr << "connection is lost! " << mysql_error(conn) << endl;
            return res;
        }
        if (res = mysql_query(conn, str))
        {
            cerr << "query failed! " << mysql_error(conn) << endl;
            /* 回滚 */
            // if(index)
            //     mysql_query(conn, "rollback");
        }
        else if(cmd.substr(0, 6) == "select")
        {
            select(str);
        }
        if(res)
            exit(-1);
        return res;
    }

    vector<vector<string>> getResult()
    {
        return buffer;
    }

private:
    void update(const char *str)
    {

    }

    void remove(const char *str)
    {

    }

    void insert(const char* str)
    {

    }

    void select(const char* str)
    {
        vector<vector<string>>().swap(buffer);
        auto res_ptr = mysql_use_result(conn);
        if (res_ptr)
        {
            int colsNum = mysql_field_count(conn);
            while(1)
            {
                vector<string> tmp;
                auto res_row = mysql_fetch_row(res_ptr);
                if (!res_row)
                    break;
                for (int j = 0; j < colsNum; j++)
                {
                    tmp.emplace_back(res_row[j]);
                    cout << res_row[j] << ", ";
                }
                buffer.emplace_back(tmp);
                cout << endl;
            }
            mysql_free_result(res_ptr);
        }
    }

public:
    void runTransaction()
    {
        int index = 0;
        for (auto& cmd : trans1)
        {
            if (cmd == "begin" || cmd == "start transaction")
            {
                if (query("set autocommit = 0", 0))
                    return;
                if (query(cmd.c_str(), index++))
                    return;
                continue;
            }
            else if (cmd == "commit")
            {
                if (query(cmd.c_str(), index++))
                    return;
                if (query("set autocommit = 1", 0))
                    return;
            }
            else if (cmd == "q")
                break;
            if (query(cmd.c_str(), index++))
                return;
        }
    }
};