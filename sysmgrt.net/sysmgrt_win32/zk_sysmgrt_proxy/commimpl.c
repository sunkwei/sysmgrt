#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "impl.h"

static void xfree(const void *p)
{
	if (p) free((void*)p);
}

void hb_one(zksysmgrt_t *sm, const char *token)
{
	int code;
	soap_call_zkreg__heartBeat(&sm->soap, sm->url, 0, (char*)token, &code);
}

static void hb_list(zksysmgrt_t *sm)
{
	list_head *pos;

	/** FIXME: 这样可能影响新的注册了 :)
	 */
	simple_mutex_lock(sm->lock);

	list_for_each(pos, &sm->tokens) {
		token_string *token = (token_string*)pos;
		hb_one(sm, token->token);
	}

	simple_mutex_unlock(sm->lock);
}

static int proc_heart_beat(void *p)
{
	// TODO: 更好的做法应该是平均化的发出心跳，而不是一起 :)

	zksysmgrt_t *sm = (zksysmgrt_t*)p;
	long last = time(0);

	for (;;) {
		void *req;
		long now;

		if (simple_thread_chkreq(sm->th, &req, 10000)) {
			if (req == 0) {
				// 要求结束
				simple_thread_reply(sm->th, 0);
				break;
			}
		}

		now = time(0);
		if (now - last > 30) {
			// 大于三十秒，则发送一次心跳
			last = now;

			hb_list(sm);
		}
	}

	return 0;
}

int save_token(zksysmgrt_t *sm, const char *token, const char *name)
{
	token_string *ts = (token_string*)malloc(sizeof(token_string));
	ts->name = strdup(name);
	ts->token = strdup(token);

	simple_mutex_lock(sm->lock);
	list_add((list_head*)ts, &sm->tokens);
	simple_mutex_unlock(sm->lock);

	return 0;
}

int remove_token(zksysmgrt_t *sm, const char *name)
{
	list_head *pos, *n;

	simple_mutex_lock(sm->lock);

	list_for_each_safe(pos, n, &sm->tokens) {
		token_string *ts = (token_string*)pos;
		if (!strcmp(name, ts->name)) {
			list_del(pos);

			xfree(ts->name);
			xfree(ts->token);
			free(ts);

			break;
		}
	}

	simple_mutex_unlock(sm->lock);

	return 0;
}

zksysmgrt_t *zksysmgrt_open(const char *url)
{
	zksysmgrt_t *sm = (zksysmgrt_t *)malloc(sizeof(zksysmgrt_t));
	sm->url = strdup(url);

	soap_init(&sm->soap);

	list_init(&sm->tokens);
	sm->lock = simple_mutex_create();
	sm->th = simple_thread_create(proc_heart_beat, sm);

	return sm;
}

void zksysmgrt_close(zksysmgrt_t *sm)
{
	soap_done(&sm->soap);

	simple_thread_req(sm->th, 0);
	simple_thread_join(sm->th);

	simple_mutex_destroy(sm->lock);

	free(sm->url);
	free(sm);
}
