#ifdef __cplusplus
extern "C" {
#endif

int pthread_mutex_lock(pthread_mutex_t*) throw();
int pthread_mutex_unlock(pthread_mutex_t*) throw();

#ifdef __cplusplus
}
#endif