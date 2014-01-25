//
//  zkregimpl.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-24.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include <uuid/uuid.h>
#include <time.h>
#include "soapStub.h"
#include "dbhlp.h"

int cb_exist(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    *(int*)opaque = sqlite3_column_int(stmt, 0);    // 总是 COUNT(*)
    return -1;  // 总是中断，
}

int __zkreg__regHost(struct soap* soap, struct zkreg__Host *req, char **token)
{
    uuid_t id;
    uuid_string_t s;
    uuid_generate(id);
    uuid_unparse(id, s);
    
    char *ips = (char*)malloc(256); // FIXME: 用于穿行化 ip，使用“，”分割
    ips[0] = 0;
    fprintf(stdout, "INFO: %s[%d]: hostname=%s, showname=%s, ipcnt=%d\n",
            __func__, req->catalog, req->name, req->showname, req->ips->__size);
    if (req->ips->__size > 0) {
        strcat(ips, req->ips->__ptr[0]);
    }
    for (int i = 1; i < req->ips->__size; i++) {
        strcat(ips, ",");   // 逗号分割
        strcat(ips, req->ips->__ptr[i]);
    }
    
    char *sql = (char*)malloc(4096); // FIXME: 应该足够了 :)
    // 检查是否存在，如果存在，需要构造 UPDATE，否则 INSERT
    int exist = 0;
    snprintf(sql, 4096, "SELECT COUNT(*) FROM host WHERE name=\"%s\"", req->name);
    db_exec_select(_db, sql, cb_exist, &exist);
    
    if (exist) {
        // FIXME: 总是先删除，然后在插入 :)
        snprintf(sql, 4096, "DELETE FROM host WHERE name='%s'; INSERT INFO host VALUES ('%s', '%s', '%s')",
                 req->name, req->name, ips, req->showname);
    }
    else {
        // 构造 INSERT
        snprintf(sql, 4096, "INSERT INTO host (name, ips, showname) VALUES ('%s', '%s', '%s')",
                 req->name, ips, req->showname);
    }
    
    db_exec_sql(_db, sql);  // 执行语句
    
    // 创建 token 记录
    time_t t = time(0); // last_stamp
    snprintf(sql, 4096, "INSERT INTO token VALUES('%s', '%s', 0, %d)", s, req->name, t);
    db_exec_sql(_db, sql);
    
    *token = (char*)soap_malloc(soap, 64);
    strcpy(*token, s);
    
    free(ips);
    free(sql);
    
    return SOAP_OK;
}

int __zkreg__unregHost(struct soap *soap, char *token, int *code)
{
    // 从 token table 中找到，删除，但不应该删除 host table 中的记录，或许将来增加一个 removeHost 的函数
    int exist = 0;
    char *sql = (char*)alloca(1024);
    snprintf(sql, 1024, "SELECT COUNT(*) FROM token WHERE token='%s'", token);
    db_exec_select(_db, sql, cb_exist, &exist);
    
    if (exist) {
        *code = 0;
        snprintf(sql, 1024, "DELETE FROM token WHERE token='%s'", token);
        db_exec_sql(_db, sql);
    }
    else {
        *code = -1; // 没有找到
    }
    
    return SOAP_OK;
}

int __zkreg__regService(struct soap *soap, struct zkreg__Service *req, char **token)
{
    return SOAP_OK;
}

int __zkreg__unregService(struct soap *soap, char *token, int *code)
{
    return SOAP_OK;
}

int __zkreg__regDevice(struct soap *soap, struct zkreg__Device *req, char **token)
{
    return SOAP_OK;
}

int __zkreg__unregDevice(struct soap *soap, char *token, int *code)
{
    return SOAP_OK;
}

int __zkreg__heartBeat(struct soap *soap, char *token, int *code)
{
    /** 心跳，更新 token table 的 last_stamp */
    int exist = 0;
    char *sql = (char*)alloca(1024);
    snprintf(sql, 1024, "SELECT COUNT(*) FROM token WHERE token='%s'", token);
    db_exec_select(_db, sql, cb_exist, &exist);
    
    if (exist) {
        *code = 0;
        snprintf(sql, 1024, "UPDATE token SET last_stamp=%d WHERE token='%s'", time(0), token);
        db_exec_sql(_db, sql);
    }
    else {
        *code = -1; // 没有找到
    }
    return SOAP_OK;
}
