#ifndef SETUP_H
#define SETUP_H

#include "GameRoom.h"
#include "InOrderNode.h"
#include "GameInfo.h"
#include "GameScanner.h"
#include "DataBucket.h"

namespace roomState {

class Setup : public RoomState {
public:

    explicit Setup(const std::vector<GameInfo>& gameSelection, DataBucket* dataBucket);
    explicit Setup(const std::vector<GameInfo>& gameSelection);

    RoomDeque poll(const jsonMsg& roomMessages, GameRoom& room) override;

    jsonMsg generateGameSelectionMessage(const networking::Connection& connection);
    void generateConfigurationPrompts(const std::string& gameKey, DataBucket* dataBucket, const networking::Connection& connection);
    void handlePlayerRangeConfig(nlohmann::json& configuration, const std::string& gameKey, DataBucket* dataBucket, const networking::Connection& connection);
    void handleAudienceConfig(nlohmann::json& configuration, const std::string& gameKey, DataBucket* dataBucket, const networking::Connection& connection);
    void handleRoundsConfig(nlohmann::json& configuration, const std::string& gameKey, DataBucket* dataBucket, const networking::Connection& connection);
    std::string waitForResponse();
    DataBucket* getDataBucket() const { return dataBucket; }
private:
    std::vector<GameInfo> catalogue;
    DataBucket* dataBucket;
    bool sentGameOptions = false;
};

}

#endif
