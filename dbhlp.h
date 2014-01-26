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
//#define DB_NAME ":memory:"

/** mse 为 host, service, device, logic 的基类，保存通用信息，通过 name 字段进行关联.
        name：主键，唯一
        catalog: 类别，0 host, 1 service, 2 device, 3 logic
        showname：用于可读的显示信息，utf-8存储
        reg_stamp：记录创建时间，使用 time(0) 的秒；
        access_stamp：更新记录时间；
 */
#define SQL_CREATE_MSE "CREATE TABLE mse(name char(64) primary key, catalog int, showname text, reg_stamp int, access_stamp int)"

/** host 描述主机信息
        name：唯一标识，一般可以选用mac地址；
        ips：使用的ip，使用逗号分割多个ip;
 */
#define SQL_CREATE_HOST "CREATE TABLE host(name char(64) primary key, ips varchar(255))"

/** service 描述服务信息
        name：唯一标识，一般可以选择 hostname+type+id 的组合；
        hostname：绑定的主机，等于 host.name
        type：服务类型，诸如 eval, ptz ... 之类
        urls：提供的服务url列表，使用逗号分割
        version：版本信息
 */
#define SQL_CREATE_SERVICE "CREATE TABLE service(name char(64) primary key, hostname char(64), type varchar(16), urls text, version varchar(128))"

/** token：描述“在线”对象
        token：唯一标识，一般为每次 regXXX 新建，并在 heartBeat() 中保持；
        name：唯一标识，对应 mse.name
        last_stamp：最新 heartBeat() 的时间；
 */
#define SQL_CREATE_TOKEN_MAP "CREATE TABLE token(token char(40) primary key, name char(64), last_stamp int)"

/** 初始化 db，自动创建所需的表 */
int db_init(sqlite3 *db);

// 执行非 select 语句
int db_exec_sql(sqlite3 *db, const char *sql);

/** 执行 SELECT 查询，通过 callback 得到每一行，如果 callback() 返回 -1 则提前中断 */
int db_exec_select(sqlite3 *db, const char *sql, int (*callback)(void *opaque, size_t row, sqlite3_stmt *stmt), void *opaque);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
