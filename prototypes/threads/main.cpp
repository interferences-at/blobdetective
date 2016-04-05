#include "mutex.h"
#include <iostream>

int main(int argc, char** argv)
{
    blobdetective::Mutex mutex;
    blobdetective::Mutex::ScopedLock lock(mutex);
    if (mutex.is_locked_right_now())
    {
        std::cout << "It's locked, as expected." << std::endl;
        return 0;
    }
    else
    {
        std::cout << "It's unlocked, something must be wrong." << std::endl;
        return 1;
    }
    return 0;
}
