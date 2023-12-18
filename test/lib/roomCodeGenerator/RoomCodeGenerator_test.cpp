#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "RoomCodeGenerator.h"
#include "msgCreator.h"

#include <unordered_set>

using codeGenerator::RoomCodeGenerator;

class RoomCodeGeneratorTest: public ::testing::Test {
    protected:
        void SetUp() override {
            testCodeGenerator1 = RoomCodeGenerator(rangeMin, rangeSmallMax);
            testCodeGenerator2 = RoomCodeGenerator(rangeMin, rangeBigMax);
        }

        void TearDown() override {}

        int rangeMin = 0;
        int rangeSmallMax = 10;
        int rangeBigMax = 1000;

        RoomCodeGenerator testCodeGenerator1;
        RoomCodeGenerator testCodeGenerator2;
        std::unordered_set<GameRoomID, GameRoomIDHash> roomCodes;
};

// tests that it can generate a code
TEST_F(RoomCodeGeneratorTest, GenerateACode) {
    GameRoomID generatedCode = testCodeGenerator1.generateCode();
    EXPECT_NE(generatedCode, INVALID_ROOM_CODE);
}

// tests that all codes it generates are unique and valid 
TEST_F(RoomCodeGeneratorTest, GenerateOnlyUniqueCodes) {
    for (int i = 0; i < rangeBigMax; i++) {
        GameRoomID newCode = testCodeGenerator2.generateCode();
        EXPECT_NE(newCode, INVALID_ROOM_CODE);

        auto result = roomCodes.insert(newCode);
        EXPECT_TRUE(result.second);
    }
}

// tests that it properly returns an invalid room code after generating all codes in its range
TEST_F(RoomCodeGeneratorTest, GenerateInvalidCode) {
    for (int i = 0; i < rangeSmallMax; i++) {
        GameRoomID newCode = testCodeGenerator1.generateCode();
        EXPECT_NE(newCode, INVALID_ROOM_CODE);

        auto result = roomCodes.insert(newCode);
        EXPECT_TRUE(result.second);
    }

    GameRoomID invalidCode = testCodeGenerator1.generateCode();
    EXPECT_EQ(invalidCode, INVALID_ROOM_CODE);
}