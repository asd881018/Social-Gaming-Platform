#include <gtest/gtest.h>
#include "Setup.h"
#include "DataBucket.h"
#include "GameInfo.h"
// Include other necessary headers

class SetupTest : public ::testing::Test {
protected:
    std::unique_ptr<roomState::Setup> setupWithDataBucket;
    std::unique_ptr<roomState::Setup> setupWithoutDataBucket;
    std::unique_ptr<DataBucket> dataBucket;
    std::vector<GameInfo> gameInfos;

    void SetUp() override {
        dataBucket = std::make_unique<DataBucket>();
        setupWithDataBucket = std::make_unique<roomState::Setup>(gameInfos, dataBucket.get());
        setupWithoutDataBucket = std::make_unique<roomState::Setup>(gameInfos);
        // Initialize gameInfos as necessary
    }
};

TEST_F(SetupTest, ConstructorWithDataBucket) {
    ASSERT_NE(setupWithDataBucket, nullptr);
    ASSERT_NE(setupWithDataBucket->getDataBucket(), nullptr);
}

TEST_F(SetupTest, ConstructorWithoutDataBucket) {
    ASSERT_NE(setupWithoutDataBucket, nullptr);
    ASSERT_EQ(setupWithoutDataBucket->getDataBucket(), nullptr);
}

    


