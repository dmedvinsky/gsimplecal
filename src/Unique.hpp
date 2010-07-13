#ifndef UNIQUE_HPP
#define UNIQUE_HPP

#include <exception>
#include <string>

#include <sys/types.h>


using namespace std;


class UniqueException : public exception
{
public:
    UniqueException(string m="Unique Exception") : msg(m) {}
    ~UniqueException() throw() {}
    const char* what() const throw() { return msg.c_str(); }

private:
    string msg;
};


class Unique
{
public:
    Unique();
    ~Unique();

    bool isRunning();
    void kill();
    void start();
    void stop();

protected:
    key_t semaphore_key;

};

#endif
