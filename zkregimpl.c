//
//  zkregimpl.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-24.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <malloc.h>
#include "soapStub.h"
#include "dbhlp.h"
#include "dboper.h"
#ifdef WIN32
#  include <Windows.h>
#else
#  include <uuid/uuid.h>
#endif

#ifdef WIN32
static void uuid_generate(uuid_t *id)
{
	CoCreateGuid(id);
}

static void uuid_unparse(uuid_t *id, char s[40])
{
	char *ss;
	UuidToString(id, &ss);
	strcpy(s, ss);
	RpcStringFree(&ss);
}

#endif // win32

int cb_exist(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    *(int*)opaque = sqlite3_column_int(stmt, 0);    // 总是 COUNT(*)
    return -1;  // 总是中断.
}

int zkreg__regHost(struct soap* soap, struct zkreg__Host *req, char **token)
{
    uuid_t id;
    char s[40];
	int rc;
	memset(&id, 0, sizeof(uuid_t));


    uuid_generate(&id);
    uuid_unparse(&id, s);
    
    rc = db_regHost(_db, req, s);
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
    // 从 token table 中找到，删除，但不应该删除 host table 中的记录，或许将来增加一个 removeHost 的函数.
    *code = db_unregXXX(_db, token);
    fprintf(stdout, "INFO: %s: unreg token %s\n", __func__, token);

    return SOAP_OK;
}

int zkreg__regService(struct soap *soap, struct zkreg__Service *req, char **token)
{
    uuid_t id;
	int rc;
    char s[40];
    uuid_generate(&id);
    uuid_unparse(&id, s);
    
    rc = db_regService(_db, req, s);
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

int zkreg__regLogic(struct soap *soap, struct zkreg__Logic *logic, char **token)
{
	*token = soap_strdup(soap, "000000");

	return SOAP_OK;
}

int zkreg__delMse(struct soap *soap, char *name, int *code)
{
	// 删除记录
	char *st = (char*)alloca(1024);

	struct dbhlpColumn desc[] = {
		{ { 0 }, DBT_STRING },	// name
		{ { 0 }, DBT_INT },		// catalog
	};

	struct dbhlpColumn **all;
	int rows = 0;

	snprintf(st, 1024, "SELECT name FROM mse WHERE name='%s'", name);
	db_exec_select2(_db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		assert(rows == 1);	// 必须的 :)
		
		// 说明找到了. 
		switch (all[0][1].data.i) {
		case zkreg__Catalog__Host:
			snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
				"DELETE FROM host WHERE name='%s'", name, name);
			break;

		case zkreg__Catalog__Service:
			snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
				"DELETE FROM service WHERE name='%s'", name, name);
			break;

		case zkreg__Catalog__Device:
			snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
				"DELETE FROM device WHERE name='%s'", name, name);
			break;

		case zkreg__Catalog__Logic:
			snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
				"DELETE FROM logic WHERE name='%s'", name, name);
			break;

		default:
			fprintf(stderr, "ERR: %s:%d unknown type!\n", __func__, __LINE__);
			exit(-1);
			break;
		}

		db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);

		db_exec_sql(_db, st);

		*code = 0;	// 成功
	}
	else {
		*code = -1;	// 没有找到
	}

	return SOAP_OK;
}

int zkreg__bind(struct soap *soap, char *logic, char *mse, int *code)
{
	char *st = (char*)alloca(1024);

	struct dbhlpColumn desc[] = {
		{ { 0 }, DBT_STRING },	// name
		{ { 0 }, DBT_STRING },	// children
	};

	struct dbhlpColumn **all;
	int rows = 0;

	snprintf(st, 1024, "SELECT name,children FROM logic WHERE name='%s'", logic);
	db_exec_select2(_db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		assert(rows == 1);	// 必须的 :)

		// 检查 mse 是否已经在 children 中
		if (strstr(all[0][1].data.s, mse)) {
			*code = 1;	// 已经存在
		}
		else {
			snprintf(st, 1024, "UPDATE logic SET children='%s\n%s' WHERE name=%s",
				all[0][1].data.s, mse, logic);
			db_exec_sql(_db, st);

			db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);
		}
	}
	else {
		*code = -1;	// 没有找到
	}

	return SOAP_OK;
}

int zkreg__unbind(struct soap *soap, char *logic, char *mse, int *code)
{
	return SOAP_OK;
}

int zkreg__setParent(struct soap *soap, char *mse, char *parent, int *code)
{
	return SOAP_OK;
}
