#include <cstdio>
#include <dlfcn.h>
#include <pthread.h>

typedef int(*pthread_mutex_lock_func)(pthread_mutex_t*);
typedef int(*pthread_mutex_unlock_func)(pthread_mutex_t*);

pthread_mutex_lock_func original_pthread_mutex_lock;
pthread_mutex_unlock_func original_pthread_mutex_unlock;

static void __attribute__ ((constructor)) pthread_msanity_init(void)
{
	fprintf(stderr, "Hello!\n");
    original_pthread_mutex_lock = (pthread_mutex_lock_func) dlsym(RTLD_NEXT, "pthread_mutex_lock");
    original_pthread_mutex_unlock = (pthread_mutex_unlock_func) dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    if (original_pthread_mutex_lock == NULL) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());        
    }
    if (original_pthread_mutex_unlock == NULL) {
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	}
}

static void __attribute__ ((destructor)) pthread_msanity_destroy(void)
{
	fprintf(stderr, "Goodbye!\n");
	original_pthread_mutex_lock = NULL;
	original_pthread_mutex_unlock = NULL;
}

int pthread_mutex_lock(pthread_mutex_t* mutex) throw()
{
    fprintf(stderr, "Lock %p \n", mutex);
    return original_pthread_mutex_lock(mutex);
}

int pthread_mutex_unlock(pthread_mutex_t* mutex) throw()
{
	fprintf(stderr, "Unlock %p \n", mutex);
    return original_pthread_mutex_unlock(mutex);
}