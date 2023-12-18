#include "RoomCodeGenerator.h"
namespace codeGenerator {
    RoomCodeGenerator::RoomCodeGenerator(int rangeMin, int rangeMax) : rangeMin(rangeMin), rangeMax(rangeMax), maxSize(abs(rangeMax-rangeMin)){}

    // generates random 6 digit room codes that have not been used
    GameRoomID RoomCodeGenerator::generateCode() {
        // if there are no more codes then returns an invalid code
        if (usedCodes.size() >= maxSize) {
            return INVALID_ROOM_CODE;
        }

        // set up for generating random numbers from a uniform distrubition
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(rangeMin,rangeMax);

        int roomCode;

        // makes sure that the code has not been previously generated or in use
        do {
            roomCode = distribution(gen);
        } while (usedCodes.count(roomCode) > 0);

        usedCodes.emplace(roomCode);

        GameRoomID newCode = {roomCode};
        return newCode;
    }

}