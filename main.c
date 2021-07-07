#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <pthread.h>
#include <unistd.h>


#define RD_COUNT                3
#define WR_COUNT                2
#define IT_COUNT                3
#define MAX_S_READING_TIME      1
#define MAX_S_READER_DELAY      3
#define MAX_S_WRITING_TIME      2
#define MAX_S_WRITER_DELAY      4

#define ERR_OKAY                0
#define ERR_RWLOCK_FAILED       1
#define ERR_THREAD_CR_FAILED    2


#define NEW_LINE        "\n"
#define LOG(FORMAT, ...) fprintf(stderr, FORMAT NEW_LINE, __VA_ARGS__)


pthread_rwlock_t lock;
int gen_num = 0;


void *thread_reader(void *num_vptr)
{
    int thread_num = *(int *)num_vptr;
    unsigned sleep_sec = 0;

    for (unsigned i = 0; i < IT_COUNT; ++i)
    {
        LOG("R%-2d iter %-3u - %s", thread_num, i, "wonna read");

        if (0 == pthread_rwlock_rdlock(&lock))
        {
            LOG("R%-2d iter %-3u - %s", thread_num, i, "start reading");
            sleep_sec = (unsigned) (rand() % MAX_S_READING_TIME);
            sleep(sleep_sec);

            int value = gen_num;
            LOG("R%-2d iter %-3u - %s %d", thread_num, i, "value is", value);
            LOG("R%-2d iter %-3u - %s", thread_num, i, "stop reading");

            pthread_rwlock_unlock(&lock);
        }
        else
        {
            LOG("R%-2d iter %-3u - %s", thread_num, i, "rdlock failed");
        }


        sleep_sec = (unsigned) (rand() % MAX_S_READER_DELAY);
        sleep(sleep_sec);
    }

    return NULL;
}

void *thread_writer(void *int_vptr)
{
    int thread_num = *(int *)int_vptr;
    unsigned sleep_sec = 0;

    for (unsigned i = 0; i < IT_COUNT; ++i)
    {
        LOG("W%-2d iter %-3u - %s", thread_num, i, "wonna write");

        if (0 == pthread_rwlock_wrlock(&lock))
        {
            LOG("W%-2d iter %-3u - %s", thread_num, i, "start writing");
            sleep_sec = (unsigned) (rand() % MAX_S_WRITING_TIME);
            sleep(sleep_sec);

            int old_value = gen_num;
            int new_value = gen_num + rand() - RAND_MAX / 2;
            gen_num = new_value;
            LOG("W%-2d iter %-3u - %s %d->%d",
                    thread_num, i, "value changed", old_value, new_value);
            LOG("W%-2d iter %-3u - %s", thread_num, i, "stop writing");

            pthread_rwlock_unlock(&lock);
        }
        else
        {
            LOG("W%-2d iter %-3u - %s", thread_num, i, "rwlock failed");
        }

        sleep_sec = (unsigned) (rand() % MAX_S_WRITER_DELAY);
        sleep(sleep_sec);
    }
    return NULL;
}

int run_threads(pthread_t readers[RD_COUNT], pthread_t writers[WR_COUNT])
{
    int rc = ERR_OKAY;

#define MAX_COUNT  (RD_COUNT > WR_COUNT ? RD_COUNT : WR_COUNT)
    unsigned nums[MAX_COUNT];
    for (unsigned i = 0; i < MAX_COUNT; ++i)
        nums[i] = i + 1;
#undef MAX_COUNT

    for (int i = 0; i < RD_COUNT; ++i)
    {
        if (0 != pthread_create(readers + i, NULL, thread_reader, nums + i))
        {
            LOG("rd thread %d creation failed", i);
            rc = ERR_THREAD_CR_FAILED;
        }
    }

    for (int i = 0; i < WR_COUNT; ++i)
    {
        if (0 != pthread_create(writers + i, NULL, thread_writer, nums + i))
        {
            LOG("wr thread %d creation failed", i);
            rc = ERR_THREAD_CR_FAILED;
        }
    }

    for (int i = 0; i < RD_COUNT; ++i)
            pthread_join(readers[i], NULL);
    for (int i = 0; i < WR_COUNT; ++i)
            pthread_join(writers[i], NULL);

    return rc;
}

int main(void)
{
    int rc, grc = ERR_OKAY;
    rc = pthread_rwlock_init(&lock, NULL);

    if (0 != rc)
    {
        LOG("rwlock_init failed; errno = %d", errno);
        grc = ERR_RWLOCK_FAILED;
    }
    else
    {
        pthread_t readers[RD_COUNT];
        pthread_t writers[WR_COUNT];

        rc = run_threads(readers, writers);
        if (ERR_OKAY != rc)
        {
            grc = rc;
        }
    }


    return grc;
}
