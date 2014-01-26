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

/** 处理service */
int db_regService(sqlite3 *db, struct zkreg__Service *service, const char *token);

/** 处理 unregXXX 对数据库的操作 */
int db_unregXXX(sqlite3 *db, const char *token);

/** 处理 getAllHosts 对数据库的操作
        hosts 为出参，必须合理释放
        n 为出参，指定 host 的数目
 */
int db_getAllHosts(sqlite3 *db, int offline, struct zkreg__Host ***hosts, int *n);
int db_getAllMses(sqlite3 *db, int offline, struct zkreg__Mse ***mses, int *n);

/** 处理 heartBeat 对数据库的操作 */
int db_heartBeat(sqlite3 *db, const char *token);

/** 处理 setShowName 对数据库的操作 */
int db_setShowname(sqlite3 *db, const char *name, const char *showname);

/** 处理 get Service list
 */
int db_getServiceList(sqlite3 *db, int offline, struct zkreg__Service ***services, int *n);
int db_getServiceListByType(sqlite3 *db, int offline, const char *type, struct zkreg__Service ***service, int *n);

#endif
