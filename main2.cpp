#include "sqlconn.h"
#include <unistd.h>

int main()
{
    sqlconn sc;
    sc.sqlInit(HOST, USR, PWD, DB);
    // sc.runTransaction();
    int index = 0;
    //sc.query("set autocommit = 0", 0);
    sleep(10);
    sc.query("begin", index++);
    sc.query("update stus set age = 800 where id = 1", index++);
    sc.query("select * from stus", index++);
    sc.query("commit", index++);
    //sc.query("set autocommit = 1", 0);
    sc.query("select * from stus", 0);
    return 0;
}