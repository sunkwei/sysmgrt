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
#include "dboper.h"

int cb_exist(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    *(int*)opaque = sqlite3_column_int(stmt, 0);    // 总是 COUNT(*)
    return -1;  // 总是中断，
}

int zkreg__regHost(struct soap* soap, struct zkreg__Host *req, char **token)
{
    uuid_t id;
    uuid_string_t s;
    uuid_generate(id);
    uuid_unparse(id, s);
    
    int rc = db_regHost(_db, req, s);
    if (rc >= 0) {
        fprintf(stdout, "INFO: %s: reg %s OK\n", __func__, req->name);
        *token = soap_strdup(soap, s);
    }
    else {
        *token = "";
        fprintf(stdout, "ERR: %s: reg %s ERR\n", __func__, req->name);
    }
    
    return SOAP_OK;
}

int zkreg__unregHost(struct soap *soap, char *token, int *code)
{
    // 从 token table 中找到，删除，但不应该删除 host table 中的记录，或许将来增加一个 removeHost 的函数
    *code = db_unregXXX(_db, token);
    fprintf(stdout, "INFO: %s: unreg token %s\n", __func__, token);

    return SOAP_OK;
}

int zkreg__regService(struct soap *soap, struct zkreg__Service *req, char **token)
{
    uuid_t id;
    uuid_string_t s;
    uuid_generate(id);
    uuid_unparse(id, s);
    
    int rc = db_regService(_db, req, s);
    if (rc >= 0) {
        fprintf(stdout, "INFO: %s: reg %s OK\n", __func__, req->name);
        *token = soap_strdup(soap, s);
    }
    else {
        *token = "";
        fprintf(stderr, "ERR: %s: reg %s ERR\n", __func__, req->name);
    }
    
    return SOAP_OK;
}

int zkreg__unregService(struct soap *soap, char *token, int *code)
{
    return SOAP_OK;
}

int zkreg__regDevice(struct soap *soap, struct zkreg__Device *req, char **token)
{
    return SOAP_OK;
}

int zkreg__unregDevice(struct soap *soap, char *token, int *code)
{
    return SOAP_OK;
}

int zkreg__heartBeat(struct soap *soap, char *token, int *code)
{
    /** 心跳，更新 token table 的 last_stamp */
    *code = db_heartBeat(_db, token);
    fprintf(stdout, "INFO: %s: token %s\n", __func__, token);

    return SOAP_OK;
}

int zkreg__setShowName(struct soap *soap, char *name, char *showname, int *code)
{
    *code = db_setShowname(_db, name, showname);
    fprintf(stdout, "INFO: %s: %s: change showname to '%s'\n", __func__, name, showname);
    
    return SOAP_OK;
}
