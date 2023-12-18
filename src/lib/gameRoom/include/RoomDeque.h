

#include <deque>
#include "msgCreator.h"

enum RoomExecution {OK, FINISHED};

/*
 * Wrapper struct around a Deque of json msg that can be used for the GameRoom to communicate with the server and update
 * its status.
 */
struct RoomDeque {
    void insert(std::deque<jsonMsg>& msg);

    void insert(std::deque<jsonMsg>&& msg);

    void insert(RoomDeque&& deque);

    void insert(jsonMsg& jsonMsg);

    std::deque<jsonMsg> messages;

    RoomExecution status = OK;
};


