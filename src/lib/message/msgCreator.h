#ifndef MESSAGE_MESSAGECREATOR_H
#define MESSAGE_MESSAGECREATOR_H

#include <string>
#include <nlohmann/json.hpp>

#include "Server.h"
using json = nlohmann::json;

/**
 *  An identifier for a target, used to match messageContent
 *  to the variable in the databucket that we want to update
 */
struct Target {
  std::string id;

  bool
  operator==(Target other) const {
    return id == other.id;
  }
};

/**
 *  An identifier for a GameRoom. The ID of a GameRoom is
 *  guaranteed to be unique across all actively connected Client instances.
 */
struct GameRoomID {
  int id;

  bool
  operator==(GameRoomID other) const {
    return id == other.id;
  }

  bool
  operator!=(GameRoomID other) const {
    return id != other.id;
  }

  bool
  operator>(GameRoomID other) const {
    return id > other.id;
  }
};

struct GameRoomIDHash {
  size_t
  operator()(GameRoomID gameRoomID) const {
    return std::hash<decltype(gameRoomID.id)>{}(gameRoomID.id);
  }
};

/**
 *  An identifier for the ExeuctionState object that send the message.
 *  For GameTree
 */
struct ExecState {
  size_t id;

  bool
  operator==(ExecState other) const {
    return id == other.id;
  }
};

enum PromptType{
    STRING = 0,
    INT = 1,
    BOOL = 2
};

struct Prompt {
    //todo: figure out how to do empty prompt types.
    PromptType promptType = PromptType::BOOL;
    int rangeMin;
    int rangeMax;
};
struct jsonMsg {
    Target target = {""};
    std::string messageContent = "";
    Prompt prompt = {};
    GameRoomID gameRoom = {0};
    networking::Connection connection {0};
    ExecState execState = {0};
};

inline void to_json(json& j, const jsonMsg& msg) {
    j["target"] = {
        {"id", msg.target.id}
    };
    j["messageContent"] = msg.messageContent;
    j["prompt"] = {
        {"promptType", msg.prompt.promptType}, 
        {"rangeMin", msg.prompt.rangeMin}, 
        {"rangeMax", msg.prompt.rangeMax}
    };
    j["gameRoom"] = {
        {"id", msg.gameRoom.id}
    };
    j["connection"] = {
        {"id", msg.connection.id}
    };
    j["execState"] = {
        {"id", msg.execState.id}
    };
};

inline void from_json(const json& j, jsonMsg& msg) {
    j.at("target").at("id").get_to(msg.target.id);
    j.at("messageContent").get_to(msg.messageContent);
    j.at("prompt").at("promptType").get_to(msg.prompt.promptType);
    j.at("prompt").at("rangeMin").get_to(msg.prompt.rangeMin);
    j.at("prompt").at("rangeMax").get_to(msg.prompt.rangeMax);
    j.at("gameRoom").at("id").get_to(msg.gameRoom.id);
    j.at("connection").at("id").get_to(msg.connection.id);
    j.at("execState").at("id").get_to(msg.execState.id);
};

std::string createJsonMessage(jsonMsg msg);

// converts one json msg to a network msg; sets the connection field of the jsonmsg
networking::Message parseJsonMsgToNetworkMsg(jsonMsg &message);

// transforms a collection of json msgs to a collection of networking msgs
std::deque<networking::Message> transformJsonMsgs(const std::deque<jsonMsg> &messages);
#endif