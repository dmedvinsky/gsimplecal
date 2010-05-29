#ifndef UNIQUE_HPP
#define UNIQUE_HPP

#include <exception>

#include <sys/types.h>


using namespace std;


class UniqueException : public exception
{
};


class Unique
{
public:
    Unique();
    ~Unique();

    bool isRunning();
    void start();
    void stop();

protected:
    key_t semaphore_key;

private:
    void _killProcess(int pid);

};

#endif
