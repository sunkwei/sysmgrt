//
//  heartBeatCheck.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-25.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include "dbhlp.h"
#ifdef WIN32
#	include <Windows.h>
#else
#	include <unistd.h>
#endif
#include "heartBeatCheck.h"

void heartCheck(sqlite3 *db)
{
    char sql[256];
    
    snprintf(sql, sizeof(sql), "DELETE FROM token WHERE last_stamp < %u", (unsigned)time(0) - CHECK_INTERVAL);
    db_exec_sql(db, sql);
}

#ifdef WIN32
DWORD __stdcall heartBeatCheck_run(void *param)
#else
void *heartBeatCheck_run(void *param)
#endif 
{
    sqlite3 *db = db_get();
    
    while (1) {
#ifdef WIN32
		Sleep(CHECK_INTERVAL*1000);
#else
        usleep(CHECK_INTERVAL*1000*1000);
#endif

        heartCheck(db);
    }
    
    db_release(db);
    
    return 0;
}
