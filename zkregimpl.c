//
//  zkregimpl.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-24.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include <uuid/uuid.h>
#include "soapStub.h"

int __zkreg__regHost(struct soap* soap, struct _zkreg__Host *req, char **token)
{
    uuid_t id;
    uuid_string_t s;
    uuid_generate(id);
    uuid_unparse(id, s);
    
    fprintf(stdout, "INFO: %s[%d]: hostname=%s, showname=%s, ipcnt=%d\n",
            __func__, req->catalog, req->name, req->showname, req->ips->__size);
    for (int i = 0; i < req->ips->__size; i++) {
        fprintf(stdout, "\tip%d: %s\n", i, req->ips->__ptr[i]);
    }
    
    *token = (char*)soap_malloc(soap, 64);
    strcpy(*token, s);
    
    return SOAP_OK;
}

int __zkreg__unregHost(struct soap *soap, char *token, int *code)
{
    *code = -1; // 没有找到 :)
    return SOAP_OK;
}

int __zkreg__regService(struct soap *soap, struct _zkreg__Service *req, char **token)
{
    return SOAP_OK;
}

int __zkreg__unregService(struct soap *soap, char *token, int *code)
{
    return SOAP_OK;
}

int __zkreg__regDevice(struct soap *soap, struct _zkreg__Device *req, char **token)
{
    return SOAP_OK;
}

int __zkreg__unregDevice(struct soap *soap, char *token, int *code)
{
    return SOAP_OK;
}

int __zkreg__heartBeat(struct soap *soap, char *token, int *code)
{
    return SOAP_OK;
}
