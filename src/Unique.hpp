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
    Unique(const char* const path);
    ~Unique();

    bool isRunning();
    void kill();
    void kill(int signal_id);
    void signal(int signal_id);
    void start();
    void stop();

protected:
    key_t semaphore_key;

private:
    void getPathnameFromFile(const char *const path, char* pathname);
    void getPathnameFromExe(char* pathname);
};

#endif
