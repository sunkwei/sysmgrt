//
//  heartBeatCheck.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-25.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#ifdef linux
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "dbhlp.h"
#include "heartBeatCheck.h"

void *heartBeatCheck_run(void *param)
{
    sqlite3 *db = (sqlite3*)param;
    
    while (1) {
        usleep(CHECK_INTERVAL*1000*1000);
        
        char sql[256];
        snprintf(sql, sizeof(sql), "DELETE FROM token WHERE last_stamp < %u", (unsigned)time(0) - CHECK_INTERVAL);
        db_exec_sql(db, sql);
    }
    
    return 0;
}
#endif