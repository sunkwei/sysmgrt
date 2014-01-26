//
//  dboper.h
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-26.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//
//  封装应用对数据库的操作
// 

#ifndef zonekey_sysmgrt_dboper_h
#define zonekey_sysmgrt_dboper_h

#include "dbhlp.h"
#include "soapStub.h"

/** 处理 regHost 对数据库的操作 */
int db_regHost(sqlite3 *db, struct zkreg__Host *host, const char *token);

/** 处理 unregHost 对数据库的操作 */
int db_unregHost(sqlite3 *db, const char *token);

/** 处理 getAllHosts 对数据库的操作
        hosts 为出参，必须合理释放
        n 为出参，指定 host 的数目
 */
int db_getAllHosts(sqlite3 *db, int offline, struct zkreg__Host ***hosts, int *n);

/** 处理 heartBeat 对数据库的操作 */
int db_heartBeat(sqlite3 *db, const char *token);

/** 处理 setShowName 对数据库的操作 */
int db_setShowname(sqlite3 *db, const char *name, const char *showname);

#endif
