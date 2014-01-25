//
//  main.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-23.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include "../soapStub.h"
#include "../zkq.nsmap"
#include "../dbhlp.h"
#include <pthread.h>
#include "../heartBeatCheck.h"

sqlite3 *_db = 0;

int main(int argc, const char * argv[])
{
    // sqlite3 设置为工作在 Serialized 模式下，因为“心跳线程”会周期操作 token table
    if (sqlite3_threadsafe()) {
        sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    }
    
    sqlite3_initialize();
    
    sqlite3_open(DB_NAME, &_db);
    db_init(_db);
    
    // 启动 webservice
    struct soap soap;
    soap_init(&soap);
    
    soap_set_mode(&soap, SOAP_C_UTFSTRING); // 如果没有这个，无法正确显示 utf-8 编码的汉字
    
    if (!soap_valid_socket(soap_bind(&soap, 0, 8899, 100))) {
        fprintf(stderr, "ERR: soap_bind 8899 err\n");
        exit(-1);
    }
    
    fprintf(stdout, "start %d\n", 8899);
    
    // 启动心跳线程
    // FIXME: 在每次 webservice 的请求时，检查 token table 是不是更好些呢？
    pthread_t th;
    pthread_create(&th, 0, heartBeatCheck_run, _db);
    
    // 开始处理所有 webservice 请求...
    while (1) {
        int n = soap_accept(&soap);
        if (n < 0) {
            fprintf(stderr, "ERR: soap_accept err\n");
            exit(-1);
        }
        
        soap_serve(&soap);
        soap_destroy(&soap);
        soap_end(&soap);
    }
    
    soap_done(&soap);
    
    return 0;
}

