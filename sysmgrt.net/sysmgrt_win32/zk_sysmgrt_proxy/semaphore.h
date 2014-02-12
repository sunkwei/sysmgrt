#ifndef _simple_semaphore__hh
#define _simple_semaphore__hh

typedef struct semaphore_t semaphore_t;

semaphore_t *simple_sem_create(int cnt);
void simple_sem_destroy(semaphore_t *s);

void simple_sem_post(semaphore_t *s);
void simple_sem_wait(semaphore_t *s);

/** return 0���յ��źţ�
	return -1�� ʧ�ܣ���ʱ
 */
int simple_sem_wait_timeout(semaphore_t *s, int timeout_ms);

#endif // semaphore.h
