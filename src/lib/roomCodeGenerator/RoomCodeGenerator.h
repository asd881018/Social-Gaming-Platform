#pragma once

#include <unordered_set>
#include <random>
#include "msgCreator.h"

inline const GameRoomID INVALID_ROOM_CODE = {-1};

namespace codeGenerator {
    class RoomCodeGenerator {
        public:
            RoomCodeGenerator() = default;
            ~RoomCodeGenerator() = default;
            RoomCodeGenerator(int rangeMin, int rangeMax);
            // generates random room code and makes sures the room code has not been previously used
            GameRoomID generateCode();
        private:
            int rangeMin;
            int rangeMax;
            int maxSize;
            std::unordered_set<int> usedCodes;
    };
}