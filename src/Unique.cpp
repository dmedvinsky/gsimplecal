#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <limits.h>
#include <sys/sem.h>
#include <signal.h>

#include "Unique.hpp"


union semun
{
    int val;
};


Unique::Unique()
{
    // Get path to the current binary.
    // It's a bit ugly, I guess, to rely on /proc, but it'll do for now.
    char szTmp[32];
    sprintf(szTmp, "/proc/%d/exe", getpid());

    char* filename = new char[PATH_MAX + 1];
    int bytes = readlink(szTmp, filename,
                         sizeof(*filename) * PATH_MAX);
    if (bytes > PATH_MAX - 1) {
        bytes = PATH_MAX;
    }
    filename[bytes] = '\0';

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
        int semid = semget(semaphore_key, 1,
                           IPC_CREAT | IPC_EXCL | 0660);
        if (semid == -1) {
            throw UniqueException("semget failed while creating semaphore");
        }

        // Set initial semaphore value to the current pid, so we could use it to
        // kill the process from the second instance.
        union semun semopts;
        semopts.val = getpid();
        if (semctl(semid, 0, SETVAL, semopts) == -1) {
            throw UniqueException("semctl (SETVAL) failed");
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
        int pid = semctl(semid, 0, GETVAL, 0);
        if (pid <= 0) {
            throw UniqueException("semctl (GETVAL) failed");
        }
        if (::kill(pid, signal_id)) {
            throw UniqueException("kill failed");
        }
    }
}

void Unique::stop()
{
    int semid = semget(semaphore_key, 1, 0660);
    if (semid != -1) {
        semctl(semid, 0, IPC_RMID, 0);
    }
}
