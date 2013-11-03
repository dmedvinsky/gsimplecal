#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <limits.h>
#include <sys/sem.h>
#include <signal.h>

#include "Unique.hpp"
#include "config.h"


Unique::Unique(const char* const cmd)
{
    char* filename = new char[PATH_MAX + 1];
#if HAVE_STRLCPY
    strlcpy(filename, cmd, PATH_MAX);
#else
    strncpy(filename, cmd, PATH_MAX);
    filename[PATH_MAX] = '\0';
#endif

    // Get unique key for semaphore.
    semaphore_key = ftok(filename, 1);
    if (semaphore_key == -1) {
        throw new UniqueException("ftok failed");
    }
    delete[] filename;
}

Unique::~Unique()
{
}

bool Unique::isRunning()
{
    int semid = semget(semaphore_key, 0, 0);
    if (semid == -1) {
        return false;
    }
    return true;
}

void Unique::start()
{
    if (!isRunning()) {
        // Create semaphore; fail if already exists.
        int semid = semget(semaphore_key, 1, IPC_CREAT | IPC_EXCL | 0660);
        if (semid == -1) {
            throw UniqueException("semget failed while creating semaphore");
        }

        // Perform an operation on semaphore so that semctl(GETPID) returns
        // current PID.
        struct sembuf ops = {0, 1, 0};
        if (semop(semid, &ops, 1) == -1) {
            throw UniqueException("semop failed");
        }
    }
}

void Unique::signal(int signal_id)
{
    kill(signal_id);
}
void Unique::kill()
{
    kill(SIGTERM);
}
void Unique::kill(int signal_id)
{
    if (isRunning()) {
        // Get semaphore; fail if not present.
        int semid = semget(semaphore_key, 1, 0660);
        if (semid == -1) {
            throw UniqueException("semget failed while trying to kill");
        }

        // Get the pid from semaphore value (stored before) to kill the process.
        int pid = semctl(semid, 0, GETPID, 0);
        if (pid <= 0) {
            throw UniqueException("semctl(GETPID) failed");
        }
        if (::kill(pid, signal_id)) {
            throw UniqueException("kill failed");
        }
    }
}

void Unique::stop()
{
    // Clean up semaphore, if exists.
    int semid = semget(semaphore_key, 1, 0660);
    if (semid != -1) {
        semctl(semid, 0, IPC_RMID, 0);
    }
}
