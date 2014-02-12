#include <stdio.h>
#include <stdlib.h>
#include "zk_sysmgrt_proxy.h"
#include "impl.h"
#include <assert.h>

static void xfree(const void *p)
{
	if (p) free((void*)p);
}

static zksysmgrt_ss *ut_clone_ss(const char **ptr, int size)
{
	zksysmgrt_ss *ss = (zksysmgrt_ss*)malloc(sizeof(zksysmgrt_ss));
	ss->cnt = size;
	if (ss->cnt == 0) {
		ss->ptr = 0;
	}
	else {
		int i;
		ss->ptr = (char**)malloc(ss->cnt * sizeof(char*));
		for (i = 0; i < ss->cnt; i++) {
			assert(ptr[i]);
			ss->ptr[i] = strdup(ptr[i]);
		}
	}

	return ss;
}

static void ut_free_ss(const zksysmgrt_ss **ss)
{
	zksysmgrt_ss *s = (zksysmgrt_ss*)(*ss);
	if (s) {
		int i;
		for (i = 0; i < s->cnt; i++)
			xfree(s->ptr[i]);			// 对应 strdup
		xfree(s->ptr);					// 对应 char **
		free(s);						// 对应 zksysmgrt_ss*
	}

	*ss = 0;
}

int zksysmgrt_q_getallhosts(zksysmgrt_t *sm, int offline,
							zksysmgrt_Host **hosts, int *cnt)
{
	int rc, i;
	struct zkreg__Hosts hs;
	struct soap soap;

	hs.__ptr = 0;
	hs.__size = 0;
	
	soap_init(&soap);
	rc = soap_call_zkq__getAllHosts(&soap, sm->url, 0, offline ? xsd__boolean__true_ : xsd__boolean__false_, &hs);

	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (hs.__size == 0) {
			// 没有获得 ..
			*cnt = 0;
			*hosts = 0;
		}
		else {
			zksysmgrt_Host *rhs = (zksysmgrt_Host*)malloc(hs.__size * sizeof(zksysmgrt_Host));

			for (i = 0; i < hs.__size; i++) {
				zksysmgrt_Host *h = &rhs[i];
				struct zkreg__Host *hh = &hs.__ptr[i];
				h->name = strdup(hh->name);
				h->showname = strdup(hh->showname);
				h->ips = ut_clone_ss(hh->ips->__ptr, hh->ips->__size);
			}

			*cnt = hs.__size;
			*hosts = rhs;
		}

		soap_end(&soap);
		return 0;
	}
}

int zksysmgrt_q_getallservices(zksysmgrt_t *sm, int offline,
							   zksysmgrt_Service **services, int *cnt)
{
	int rc, i;
	struct zkreg__Services ss;
	struct soap soap;

	ss.__ptr = 0;
	ss.__size = 0;

	soap_init(&soap);
	rc = soap_call_zkq__getAllServices(&soap, sm->url, 0, offline ? xsd__boolean__true_ : xsd__boolean__false_, &ss);

	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (ss.__size == 0) {
			*cnt = 0;
			*services = 0;
		}
		else {
			zksysmgrt_Service *rss = (zksysmgrt_Service*)malloc(ss.__size * sizeof(zksysmgrt_Service));

			for (i = 0; i < ss.__size; i++) {
				zksysmgrt_Service *s = &rss[i];
				struct zkreg__Service *rs = &ss.__ptr[i];

				s->name = strdup(rs->name);
				s->hostname = strdup(rs->hostname);
				s->showname = strdup(rs->showname);
				s->type = strdup(rs->type);
				s->version = strdup(rs->version);
				s->urls = ut_clone_ss(rs->urls->__ptr, rs->urls->__size);
			}

			*cnt = ss.__size;
			*services = rss;
		}
		soap_end(&soap);
		return 0;
	}
}

int zksysmgrt_q_getalllogics(zksysmgrt_t *sm, zksysmgrt_Logic **logics, int *cnt)
{
	int rc, i;
	struct zkreg__Logics ls;
	struct soap soap;

	ls.__ptr = 0;
	ls.__size = 0;

	soap_init(&soap);
	rc = soap_call_zkq__getAllLogics(&soap, sm->url, 0, xsd__boolean__true_, &ls);

	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (ls.__size == 0) {
			*cnt = 0;
			*logics = 0;
		}
		else {
			zksysmgrt_Logic *rls = (zksysmgrt_Logic*)malloc(ls.__size * sizeof(zksysmgrt_Logic));

			for (i = 0; i < ls.__size; i++) {
				zksysmgrt_Logic *l = &rls[i];
				struct zkreg__Logic *r = &ls.__ptr[i];

				l->name = strdup(r->name);
				l->showname = strdup(r->showname);
				l->children = ut_clone_ss(r->children, r->childnum);
			}

			*cnt = ls.__size;
			*logics = rls;
		}
		soap_end(&soap);
		return 0;
	}
}

int zksysmgrt_q_getparents(zksysmgrt_t *sm, const char *name,
						   zksysmgrt_Logic **mses, int *cnt)
{
	int rc, i;
	struct zkreg__Logics rs;
	struct soap soap;

	rs.__ptr = 0;
	rs.__size = 0;

	soap_init(&soap);
	rc = soap_call_zkq__getParent(&soap, sm->url, 0, (char*)name, &rs);

	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (rs.__size == 0) {
			*cnt = 0;
			*mses = 0;
		}
		else {
			zksysmgrt_Logic *rrs = (zksysmgrt_Logic*)malloc(rs.__size * sizeof(zksysmgrt_Logic));

			for (i = 0; i < rs.__size; i++) {
				zksysmgrt_Logic *l = &rrs[i];
				struct zkreg__Logic *r = &rs.__ptr[i];

				l->name = strdup(r->name);
				l->showname = strdup(r->showname);
				l->children = ut_clone_ss(r->children, r->childnum);
			}

			*cnt = rs.__size;
			*mses = rrs;
		}
		soap_end(&soap);
		return 0;
	}
}

int zksysmgrt_q_getbrothers(zksysmgrt_t *sm, const char *name,
							zksysmgrt_Brother **brothers, int *cnt)
{
	int rc, i;
	struct zkq__Brothers rs;
	struct soap soap;

	rs.__ptr = 0;
	rs.__size = 0;

	soap_init(&soap);
	rc = soap_call_zkq__getBrothers(&soap, sm->url, 0, (char*)name, &rs);

	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (rs.__size == 0) {
			*cnt = 0;
			*brothers = 0;
		}
		else {
			zksysmgrt_Brother *rrs = (zksysmgrt_Brother*)malloc(rs.__size * sizeof(zksysmgrt_Brother));

			for (i = 0; i < rs.__size; i++) {
				zksysmgrt_Brother *p = &rrs[i];
				struct zkq__Brother *q = &rs.__ptr[i];

				p->parent = strdup(q->parent);
				p->brothers = ut_clone_ss(q->brothers, q->brothernum);
			}

			*cnt = rs.__size;
			*brothers = rrs;
		}
		soap_end(&soap);
		return 0;
	}
}

int zksysmgrt_q_gethost(zksysmgrt_t *sm, const char *name, zksysmgrt_Host **host)
{
	int rc;
	struct zkreg__Host *rh;
	struct soap soap;

	soap_init(&soap);
	rc = soap_call_zkq__getHostDesc(&soap, sm->url, 0, (char*)name, &rh);

	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (!rh) {
			*host = 0;	// 没有得到.
		}
		else {
			zksysmgrt_Host *h = (zksysmgrt_Host*)malloc(1 * sizeof(zksysmgrt_Host));
			*host = h;

			h->name = strdup(rh->name);
			h->showname = strdup(rh->showname);
			h->ips = ut_clone_ss(rh->ips->__ptr, rh->ips->__size);
		}
		soap_end(&soap);
		return 0;
	}
}

int zksysmgrt_q_getservice(zksysmgrt_t *sm, const char *name, zksysmgrt_Service **service)
{
	int rc;
	struct zkreg__Service *rs;
	struct soap soap;

	soap_init(&soap);
	rc = soap_call_zkq__getServiceDesc(&soap, sm->url, 0, (char*)name, &rs);
	
	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (!rs) {
			*service = 0; // 没有得到
		}
		else {
			zksysmgrt_Service *s = (zksysmgrt_Service*)malloc(1 * sizeof(zksysmgrt_Service));
			*service = s;

			s->name = strdup(rs->name);
			s->hostname = strdup(rs->hostname);
			s->showname = strdup(rs->showname);
			s->type = strdup(rs->type);
			s->version = strdup(rs->version);
			s->urls = ut_clone_ss(rs->urls->__ptr, rs->urls->__size);
		}
		soap_end(&soap);

		return 0;
	}
}

int zksysmgrt_q_getlogic(zksysmgrt_t *sm, const char *name, zksysmgrt_Logic **logic)
{
	int rc;
	struct zkreg__Logic *rl;
	struct soap soap;

	soap_init(&soap);
	rc = soap_call_zkq__getLogicDesc(&soap, sm->url, 0, (char*)name, &rl);

	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (!rl)
			*logic = 0;
		else {
			zksysmgrt_Logic *l = (zksysmgrt_Logic*)malloc(1 * sizeof(zksysmgrt_Logic));
			*logic = l;

			l->name = strdup(rl->name);
			l->showname = strdup(rl->showname);
			l->children = ut_clone_ss(rl->children, rl->childnum);
		}
		soap_end(&soap);
		return 0;
	}
}

int zksysmgrt_q_getservicesbytype(zksysmgrt_t *sm, int offline, const char *type, 
								zksysmgrt_Service **services, int *cnt)
{
	int rc, i;
	struct zkreg__Services ss;
	struct soap soap;

	ss.__ptr = 0;
	ss.__size = 0;

	soap_init(&soap);
	rc = soap_call_zkq__getServicesByType(&soap, sm->url, 0, offline ? xsd__boolean__true_ : xsd__boolean__false_, (char*)type, &ss);

	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (ss.__size == 0) {
			*cnt = 0;
			*services = 0;
		}
		else {
			zksysmgrt_Service *rss = (zksysmgrt_Service*)malloc(ss.__size * sizeof(zksysmgrt_Service));

			for (i = 0; i < ss.__size; i++) {
				zksysmgrt_Service *s = &rss[i];
				struct zkreg__Service *rs = &ss.__ptr[i];

				s->name = strdup(rs->name);
				s->hostname = strdup(rs->hostname);
				s->showname = strdup(rs->showname);
				s->type = strdup(rs->type);
				s->version = strdup(rs->version);
				s->urls = ut_clone_ss(rs->urls->__ptr, rs->urls->__size);
			}

			*cnt = ss.__size;
			*services = rss;
		}
		soap_end(&soap);
		return 0;
	}
}

void zksysmgrt_q_freehosts(zksysmgrt_t *sm, zksysmgrt_Host *hosts, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++) {
		zksysmgrt_Host *h = &hosts[i];

		xfree(h->name);
		xfree(h->showname);
		ut_free_ss(&h->ips);
	}

	xfree(hosts);
}

void zksysmgrt_q_freeservices(zksysmgrt_t *sm, zksysmgrt_Service *services, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++) {
		zksysmgrt_Service *s = &services[i];

		xfree(s->name);
		xfree(s->hostname);
		xfree(s->showname);
		xfree(s->type);
		xfree(s->version);
		ut_free_ss(&s->urls);
	}

	xfree(services);
}

void zksysmgrt_q_freelogics(zksysmgrt_t *sm, zksysmgrt_Logic *logics, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++) {
		zksysmgrt_Logic *l = &logics[i];

		xfree(l->name);
		xfree(l->showname);
		ut_free_ss(&l->children);
	}

	xfree(logics);
}

void zksysmgrt_q_freebrothers(zksysmgrt_t *sm, zksysmgrt_Brother *brothers, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++) {
		zksysmgrt_Brother *b = &brothers[i];

		xfree(b->parent);
		ut_free_ss(&b->brothers);
	}

	xfree(brothers);
}

void zksysmgrt_q_freemses(zksysmgrt_t *sm, zksysmgrt_Mse *mses, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++) {
		zksysmgrt_Mse *m = &mses[i];

		xfree(m->name);
		xfree(m->showname);
	}

	xfree(mses);
}
