#include "sqlconn.h"
#include <unistd.h>

int main()
{
    sqlconn sc;
    sc.sqlInit(HOST, USR, PWD, DB);
    // sc.runTransaction();
    int index = 0;
    sc.query("set autocommit = 0", 0);
    sc.query("begin", index++);
    sc.query("update stus set age = 5555 where id = 1", index++);
    //sc.query("insert into stus values (7, 'ningguang', 28, 'economics')", index++);
    sc.query("select * from stus", 0);
    sleep(40);
    sc.query("commit", index++);
    // sc.query("set autocommit = 1", 0);
    // sc.query("select * from stus", 0);
    return 0;
}