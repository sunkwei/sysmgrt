#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "soapStub.h"
#include "dbhlp.h"
#include "dboper.h"

int zkcfg__getAllKeys(struct soap *soap, struct zkcfg__keys *keys)
{
	struct dbhlpColumn desc[1] = {
		{ { 0 }, DBT_STRING },
	};

	struct dbhlpColumn **all = 0;
	int rows = 0, i;

	int rc = db_exec_select2(_db, "SELECT key FROM config", desc, 1, &all, &rows);
	if (rc >= 0) {
		keys->__ptr = (char**)soap_malloc(soap, rows * sizeof(char*));
		keys->__size = rows;

		for (i = 0; i < rows; i++) {
			keys->__ptr[i] = soap_strdup(soap, all[i][0].data.s);
		}

		db_free_select2(desc, 1, all, rows);
	}

	return SOAP_OK;
}

int zkcfg__getValue(struct soap *soap, char *key, char **value)
{
	struct dbhlpColumn desc[1] = {
		{{ 0 }, DBT_STRING },
	};

	struct dbhlpColumn **all = 0;
	int rows = 0, i, rc;
	char *sql = (char*)alloca(1024);

	*value = 0;

	_snprintf(sql, 1024, "SELECT value FROM config WHERE key='%s'", key);

	rc = db_exec_select2(_db, sql, desc, 1, &all, &rows);
	if (rc >= 0) {
		if (rows == 0)
			*value = 0;
		else
			*value = soap_strdup(soap, all[0][0].data.s);

		db_free_select2(desc, 1, all, rows);
	}

	return SOAP_OK;
}

int zkcfg__setValue(struct soap *soap, char *key, char *value)
{
	char *sql = (char*)alloca(1024);
	char *v;
	zkcfg__getValue(soap, key, &v);

	if (v) {
		// 更新
		_snprintf(sql, 1024, "UPDATE config SET value='%s' WHERE key='%s'", 
			value, key);
	}
	else {
		// 新建
		_snprintf(sql, 1024, "INSERT INTO config VALUES('%s', '%s')", key, value);
	}

	db_exec_sql(_db, sql);

	return SOAP_OK;
}

int zkcfg__delKey(struct soap *soap, char *key)
{
	char *sql = (char*)alloca(1024);
	_snprintf(sql, 1024, "DELETE FROM config WHERE key='%s'", key);
	db_exec_sql(_db, sql);

	return SOAP_OK;
}
