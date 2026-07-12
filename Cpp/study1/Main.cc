#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <string>


int ticket = 1000;

typedef struct threadData
{
    std::string name;
    pthread_mutex_t *plock;
}threaddata_t;

void *route(void *arg)
{
    threaddata_t *td = static_cast<threaddata_t *>(arg);
    while ( 1 ) {
        // pthread_mutex_lock(td->plock);
        if ( ticket > 0 ) {
            usleep(1000);
            printf("%s sells ticket:%d\n", td->name.c_str(), ticket);
                        ticket--;
            // pthread_mutex_unlock(td->plock);
        } else {
            // pthread_mutex_unlock(td->plock);
            break;
        }
    }

    return nullptr;
}

int main( void )
{
    pthread_t t1, t2, t3, t4;

    pthread_mutex_t lock;
    pthread_mutex_init(&lock, nullptr);

    threaddata_t data1 = {"thread-1", &lock};
    pthread_create(&t1, NULL, route, (void*)&data1);

    threaddata_t data2 = {"thread-2", &lock};
    pthread_create(&t2, NULL, route, (void*)&data2);

    threaddata_t data3 = {"thread-3", &lock};
    pthread_create(&t3, NULL, route, (void*)&data3);

    threaddata_t data4 = {"thread-4", &lock};
    pthread_create(&t4, NULL, route, (void*)&data4);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}

