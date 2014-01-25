//
//  dbhlp.h
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-24.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//
//  为了方便存储，首先使用 sqlite3 实现

#ifndef zonekey_sysmgrt_storage_h
#define zonekey_sysmgrt_storage_h

#include <sqlite3.h>

extern sqlite3 *_db;    // 方便全局使用

#define DB_NAME "zonekey.mgrt.db"
#define SQL_CREATE_MSE "CREATE TABLE mse(name char(40) primary key, catalog int, showname text)"
#define SQL_CREATE_HOST "CREATE TABLE host(name char(40) primary key, ips varchar(255))"
#define SQL_CREATE_SERVICE "CREATE TABLE service(name char(40) primary key, hostname char(40), type varchar(128), urls text, version varchar(128))"
#define SQL_CREATE_TOKEN_MAP "CREATE TABLE token(token char(40) primary key, name char(40), catalog int, last_stamp int)"

/** 初始化 db，自动创建所需的表 */
int db_init(sqlite3 *db);

// 执行非 select 语句
int db_exec_sql(sqlite3 *db, const char *sql);

/** 执行 SELECT 查询，通过 callback 得到每一行，如果 callback() 返回 -1 则提前中断 */
int db_exec_select(sqlite3 *db, const char *sql, int (*callback)(void *opaque, size_t row, sqlite3_stmt *stmt), void *opaque);

#endif
