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

#ifndef __func__
#  define __func__ __FUNCTION__
#endif

#include <sqlite3.h>

#ifdef MOD
#   define DB_NAME "/tmp/zonekey.mgrt.db"
#else
#   define DB_NAME "zonekey.mgrt.db"
#endif //
//#define DB_NAME ":memory:"

/** mse 为 host, service, device, logic 的基类，保存通用信息，通过 name 字段进行关联.
        name：主键，唯一
        catalog: 类别，0 host, 1 service, 2 device, 3 logic
		parent: 父节点 name，如果有效，必定为一 logic 对象的 name
        showname：用于可读的显示信息，utf-8存储
        reg_stamp：记录创建时间，使用 time(0) 的秒；
        access_stamp：更新记录时间；
 */
#define SQL_CREATE_MSE "CREATE TABLE mse(name char(64) primary key, catalog int, parent char(64), showname text, reg_stamp int, access_stamp int)"

/** host 描述主机信息
        name：唯一标识，一般可以选用mac地址；
        ips：使用的ip，使用逗号分割多个ip;
 */
#define SQL_CREATE_HOST "CREATE TABLE host(name char(64) primary key, ips varchar(255))"

/** service 描述服务信息
        name：唯一标识，一般可以选择 hostname+type+id 的组合；
        hostname：绑定的主机，等于 host.name
        type：服务类型，诸如 eval, ptz ... 之类
        urls：提供的服务url列表，使用'\n'分割
        version：版本信息
 */
#define SQL_CREATE_SERVICE "CREATE TABLE service(name char(64) primary key, hostname char(64), type varchar(16), urls text, version varchar(128))"

/** logic: 保存 logic 对象信息
		name: 唯一标识，与 mse 的 name 一致
		children: 子节点信息，使用"\n"分割！！！！
 */
#define SQL_CREATE_LOGIC "CREATE TABLE logic(name char(40) primary key, children text)"

/** token：描述“在线”对象
        token：唯一标识，一般为每次 regXXX 新建，并在 heartBeat() 中保持；
        name：唯一标识，对应 mse.name
        last_stamp：最新 heartBeat() 的时间；
 */
#define SQL_CREATE_TOKEN_MAP "CREATE TABLE token(token char(40) primary key, name char(64), last_stamp int)"

/** config: 保存配置信息
		key: 唯一标识
		value: 值
 */
#define SQL_CREATE_CONFIG "CREATE TABLE config(key char(64) primary key, value text)"

// 作为数据库链接，需要考虑每个 http request 都应该重建 ..
sqlite3 *db_get();
void db_release(sqlite3 *db);

/** 初始化 db，自动创建所需的表 */
int db_init(sqlite3 *db);

// 执行非 select 语句
int db_exec_sql(sqlite3 *db, const char *sql);

/** 执行 SELECT 查询，通过 callback 得到每一行，如果 callback() 返回 -1 则提前中断 */
int db_exec_select(sqlite3 *db, const char *sql, int (*callback)(void *opaque, size_t row, sqlite3_stmt *stmt), void *opaque);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum dbhlpColumnType
{
	DBT_INT,
	DBT_STRING,
	DBT_DOUBLE,
} dbhlpColumnType ;

/** 描述一个记录的字段
 */
struct dbhlpColumn
{
	union {
		int i;
		char *s;		// 使用完成，需要释放. 
		double d;
	} data;

	enum dbhlpColumnType type;
};

/// 根据 col_desc，执行 sql select，将数据保存到 cols 中，行数保存在 rows 中
int db_exec_select2(sqlite3 *db, const char *sql, struct dbhlpColumn col_desc[], int cn, struct dbhlpColumn ***cols, int *rows);
void db_free_select2(struct dbhlpColumn col_desc[], int cn, struct dbhlpColumn **cols, int rows);

#ifdef WIN32
#	define snprintf _snprintf
#else
#	define _snprintf snprintf
#endif

#endif
