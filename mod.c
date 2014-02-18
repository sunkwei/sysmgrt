//
//  mod.c 用于生成 mod_gsoap 的模式
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-23.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include "soapStub.h"
#include "zkreg.nsmap"
#include "dbhlp.h"
#include "heartBeatCheck.h"
#ifdef WIN32
#  include <Windows.h>
#else
#  include <pthread.h>
#endif
#include "apache_gsoap.h"

#define MULTITHREAD_SERVER

IMPLEMENT_GSOAP_SERVER()

#ifdef MULTITHREAD_SERVER
#ifdef WIN32
static DWORD __stdcall _working_proc(void *param)
#else
static void *_working_proc(void *param)
#endif // win32
{
    struct soap *soap = (struct soap*)param;
    
#ifdef WIN32
#else
    pthread_detach(pthread_self());
#endif//
    soap_serve((struct soap*)soap);
    soap_destroy((struct soap*)soap); // dealloc C++ data
    soap_end((struct soap*)soap); // dealloc data and clean up
    soap_done((struct soap*)soap); // detach soap struct
    soap_free(soap);
    
    return NULL;
}
#endif // multi thread server mode

static int mainp(int argc, const char * argv[])
{
    char dbname[1024];
    int rc = 0;

    snprintf(dbname, sizeof(dbname), "/tmp/" DB_NAME);

   
    // 启动心跳线程
    // FIXME: 在每次 webservice 的请求时，检查 token table 是不是更好些呢？
    /*
#ifdef WIN32
	CloseHandle(CreateThread(0, 0, heartBeatCheck_run, _db, 0, 0));
#else
    pthread_t th;
    pthread_create(&th, 0, heartBeatCheck_run, _db);
#endif // os
    */
    return 0;
}

void init_lib()
{
//	mainp(0, 0);
}

