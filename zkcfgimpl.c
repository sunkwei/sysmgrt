#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#  include <malloc.h>
#else
#  include <alloca.h>
#endif // 
#include "soapStub.h"
#include "dbhlp.h"
#include "dboper.h"

int zkcfg__getAllKeys(struct soap *soap, void *notused, struct zkcfg__Keys *keys)
{
	struct dbhlpColumn desc[1] = {
		{ { 0 }, DBT_STRING },
	};

	struct dbhlpColumn **all = 0;
	int rows = 0, i;

	db_exec_select2(_db, "SELECT key FROM config", desc, 1, &all, &rows);
	if (rows > 0) {
		keys->__ptr = (char**)soap_malloc(soap, rows * sizeof(char*));
		keys->__size = rows;

		for (i = 0; i < rows; i++) {
			keys->__ptr[i] = soap_strdup(soap, all[i][0].data.s);
		}
	}
	else {
		keys->__ptr = 0;
		keys->__size = 0;
	}

	db_free_select2(desc, 1, all, rows);

	return SOAP_OK;
}

int zkcfg__getValue(struct soap *soap, char *key, struct zkcfg__Ret *res)
{
	struct dbhlpColumn desc[1] = {
		{{ 0 }, DBT_STRING },
	};

	struct dbhlpColumn **all = 0;
	int rows = 0, i, rc;
	char *sql = (char*)alloca(1024);

	snprintf(sql, 1024, "SELECT value FROM config WHERE key='%s'", key);

	rc = db_exec_select2(_db, sql, desc, 1, &all, &rows);
	if (rc >= 0) {
		if (rows == 0) {
			res->value = 0;
			res->result = -1;
		}
		else {
			res->value = soap_strdup(soap, all[0][0].data.s);
			res->result = 0;
		}

		db_free_select2(desc, 1, all, rows);
	}

	return SOAP_OK;
}

int zkcfg__setValue(struct soap *soap, char *key, char *value, struct zkcfg__Ret *res)
{
	char *sql = (char*)alloca(1024);
	struct zkcfg__Ret ret;
	zkcfg__getValue(soap, key, &ret);

	if (ret.value) {
		// 更新
		snprintf(sql, 1024, "UPDATE config SET value='%s' WHERE key='%s'", 
			value, key);
		res->result = 1;
	}
	else {
		// 新建
		snprintf(sql, 1024, "INSERT INTO config VALUES('%s', '%s')", key, value);
		res->result = 0;
	}

	db_exec_sql(_db, sql);

	return SOAP_OK;
}

int zkcfg__delKey(struct soap *soap, char *key, struct zkcfg__Ret *res)
{
	char *sql = (char*)alloca(1024);
	snprintf(sql, 1024, "DELETE FROM config WHERE key='%s'", key);
	db_exec_sql(_db, sql);

	res->result = 0;

	return SOAP_OK;
}

