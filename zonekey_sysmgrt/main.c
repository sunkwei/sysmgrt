//
//  main.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-23.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include "../soapStub.h"
#include "../zkreg.nsmap"
#include "../dbhlp.h"
#include "../heartBeatCheck.h"
#ifdef WIN32
#  include <Windows.h>
#else
#  include <pthread.h>
#endif

#define MULTITHREAD_SERVER

sqlite3 *_db = 0;

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

int main(int argc, const char * argv[])
{
    struct soap soap;

	// sqlite3 设置为工作在 Serialized 模式下，因为“心跳线程”会周期操作 token table
    if (sqlite3_threadsafe()) {
        sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    }
    
    sqlite3_initialize();
    
    sqlite3_open(DB_NAME, &_db);
    db_init(_db);
    
    // 启动 webservice
    soap_init(&soap);
    
    soap_set_mode(&soap, SOAP_C_UTFSTRING); // 如果没有这个，无法正确显示 utf-8 编码的汉字
    
    if (!soap_valid_socket(soap_bind(&soap, 0, 8899, 100))) {
        fprintf(stderr, "ERR: soap_bind 8899 err\n");
        exit(-1);
    }
    
    fprintf(stdout, "start %d\n", 8899);
    
    // 启动心跳线程
    // FIXME: 在每次 webservice 的请求时，检查 token table 是不是更好些呢？
//    pthread_t th;
//    pthread_create(&th, 0, heartBeatCheck_run, _db);
    
    // 开始处理所有 webservice 请求...
    while (1) {
        int n = soap_accept(&soap);
        if (n < 0) {
            fprintf(stderr, "ERR: soap_accept err\n");
            soap_print_fault(&soap, stderr);
            if (soap.errnum) {
                soap_print_fault(&soap, stderr);
                exit(1);
            }
            exit(-1);
        }
#ifdef MULTITHREAD_SERVER
		{
        struct soap *ts = soap_copy(&soap);
        if (ts) {
#ifdef WIN32
			HANDLE th;
			CloseHandle(CreateThread(0, 0, _working_proc, ts, 0, 0));
#else
            pthread_t th;
            pthread_create(&th, 0, _working_proc, ts);
#endif // 
        }
		}
#else
        soap_serve(&soap);
        soap_destroy(&soap);
        soap_end(&soap);
#endif // multi thread server
    }
    
    soap_done(&soap);
    
    return 0;
}

