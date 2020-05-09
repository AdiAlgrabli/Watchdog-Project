#include <time.h>
#include <unistd.h>
#include <assert.h>

#include "UID.h"

const UID_t BAD_UID = {0, 0, 0};

UID_t UIDCreate()
{
    UID_t uid;
    static int counter = 0;

    uid.time_stamp = time(NULL);
    uid.counter = counter;
    uid.pid = getpid();

    ++counter;

    return (uid);
}

int UIDIsSame(const UID_t uid1, const UID_t uid2)
{
    return (uid1.time_stamp == uid2.time_stamp &&
            uid1.counter == uid2.counter &&
            uid1.pid == uid2.pid);
}

int UIDIsBad(const UID_t uid)
{
    return (UIDIsSame(uid, BAD_UID));
}
