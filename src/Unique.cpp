#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/sysctl.h>
#include <sys/types.h>

#include "Unique.hpp"
#include "config.h"


Unique::Unique(const char *const path)
{
    char pathname[PATH_MAX + 1];

    if (path) {
        getPathnameFromFile(path, *&pathname);
    } else {
        getPathnameFromExe(*&pathname);
    }

    // Get unique key for semaphore.
    semaphore_key = ftok(pathname, 1);
    if (semaphore_key == -1) {
        throw new UniqueException("ftok failed");
    }
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

void Unique::getPathnameFromFile(const char *const path, char* pathname)
{
#if HAVE_STRLCPY
    strlcpy(pathname, path, PATH_MAX + 1);
#else
    strncpy(pathname, path, PATH_MAX);
    pathname[PATH_MAX] = '\0';
#endif
}

void Unique::getPathnameFromExe(char* pathname)
{
#if HAVE_GETEXECNAME && HAVE_STRLCPY
    // Try getexecname (Solaris).
    const char* execname = getexecname();
    strlcpy(pathname, execname, PATH_MAX + 1);
#elif HAVE_SYSCTL && defined(CTL_KERN) && defined(KERN_PROC) && defined(KERN_PROC_PATHNAME)
    // Try sysctl call (FreeBSD).
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    size_t cb = PATH_MAX;
    sysctl(mib, 4, pathname, &cb, NULL, 0);
#else
    // Try reading procfs links.
    const char *const paths[] = {
        "/proc/self/exe",     // Linux
        "/proc/curproc/exe",  // NetBSD
        "/proc/curproc/file", // FreeBSD with procfs
        NULL
    };
    for (const char *const *path = paths; *path; path++) {
        pathname[0] = 0;
        int bytes = readlink(*path, pathname, sizeof(*pathname) * PATH_MAX);
        if (bytes > 0) {
            if (bytes > PATH_MAX - 1) {
                bytes = PATH_MAX;
            }
            pathname[bytes] = '\0';
            break;
        }
    }
#endif
}
