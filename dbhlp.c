//
//  storage.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-24.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dbhlp.h"
#include "heartBeatCheck.h"

/** 执行 select 语句
 */
int db_exec_select(sqlite3 *db, const char *sql, int (*callback)(void *opaque, size_t row, sqlite3_stmt *stmt), void *opaque)
{
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, (int)strlen(sql), &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "%s: sqlite3_prepare_v2 err\n", __func__);
        exit(-1);
        return -1;
    }
    
    size_t row = 0;
    int rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        if (callback(opaque, row++, stmt) < 0)
            break;
        
        rc = sqlite3_step(stmt);
    }
    
    // sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    
    return 0;
}

int db_exec_sql(sqlite3 *db, const char *sql)
{
    return sqlite3_exec(db, sql, 0, 0, 0);
}

struct cbparamSelectCount
{
    int cnt;
};

static int cb_select_count(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    struct cbparamSelectCount *cnt = (struct cbparamSelectCount*)opaque;
    fprintf(stdout, "%s: calling: row=%u\n", __func__, row);
    
    // select COUNT(*) from ....
    if (sqlite3_column_count(stmt) == 1) {
        cnt->cnt = sqlite3_column_int(stmt, 0);
        fprintf(stderr, "\tcnt=%d\n", cnt->cnt);
    }
    
    return -1;
}

/** 判断是否存在
 */
static int db_table_exist(sqlite3 *db, const char *name)
{
    char sql[128];
    struct cbparamSelectCount cnt = { 0 };
    snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM sqlite_master where name=\"%s\"", name);
    int rc = db_exec_select(db, sql, cb_select_count, &cnt);
    
    return cnt.cnt;
}

/** 初始化数据库
 */
int db_init(sqlite3 *db)
{
    /** 检查是否存在 host, service, 等...
     */
    if (!db_table_exist(db, "mse")) {
        const char *sql = SQL_CREATE_MSE;
        int rc = db_exec_sql(db, sql);
        if (rc != SQLITE_OK) {
            
        }
    }
    
    if (!db_table_exist(db, "host")) {
        const char *sql = SQL_CREATE_HOST;
        int rc = db_exec_sql(db, sql);
        if (rc != SQLITE_OK) {
            
        }
    }
    
    if (!db_table_exist(db, "service")) {
        const char *sql = SQL_CREATE_SERVICE;
        int rc = db_exec_sql(db, sql);
        if (rc != SQLITE_OK) {
            
        }
    }
    
    if (!db_table_exist(db, "token")) {
        const char *sql = SQL_CREATE_TOKEN_MAP;
        int rc = db_exec_sql(db, sql);
        if (rc != SQLITE_OK) {
            
        }
    }
    
    // 清空 token table 中所有超时的
    unsigned curr = (unsigned)time(0) - CHECK_INTERVAL;
    char *sql = (char*)alloca(1024);
    snprintf(sql, 1024, "DELETE FROM token WHERE last_stamp < %u", curr);
    db_exec_sql(db, sql);
    
    return 0;
}
