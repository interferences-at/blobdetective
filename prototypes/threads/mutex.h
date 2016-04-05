#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>

namespace blobdetective {

/**
 * Mutually exclusive thread-safe operation.
 */
class Mutex
{
    public:
        Mutex()
        {
            pthread_mutex_init(& this->m_mutex, NULL);
        }
        void lock()
        {
            pthread_mutex_lock(& this->m_mutex);
        }
        void unlock()
        {
            pthread_mutex_unlock(& this->m_mutex);
        }
        bool is_locked_right_now()
        {
            if (pthread_mutex_trylock(& this->m_mutex) == 0)
            {
                pthread_mutex_unlock(& this->m_mutex);
                return true;
            }
            else
            {
                return false;
            }
        }
        class ScopedLock
        {
            public:
                ScopedLock(Mutex& mutex):
                    _mutex(mutex)
                {
                    _mutex.lock();
                }
                ~ScopedLock()
                {
                    _mutex.unlock();
                }
            private:
                Mutex& _mutex;
        };
    private:
        pthread_mutex_t m_mutex;
};

} // end of namespace

#endif // include guard
