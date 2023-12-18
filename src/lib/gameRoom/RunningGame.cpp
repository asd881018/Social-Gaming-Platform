#include "RunningGame.h"

#include <numeric>

// todo: temporarily disabled while updating GameRunner
roomState::RunningGame::RunningGame(const GameParams& params) : gameInfo(params.game), catalogue(params.gameInfos) {

    // testing args
    Arguments dummy_arguments{sg::ExpressionValue{0}};

    // Build Demo Tree
    auto root = std::make_unique<RuleNode>("root", dummy_arguments);

    Arguments simple_msg_args{sg::ExpressionValue{"all"}, sg::ExpressionValue{"I am a basic message to all players"}};
    Arguments simple_msg_args2{sg::ExpressionValue{"all"}, sg::ExpressionValue{"I am a second message to all players"}};

    root->addChild("message", simple_msg_args);
    root->addChild("message", simple_msg_args2);

    Arguments parallel_for_arguments{sg::ExpressionValue{4}};
    root->addChild("parallel_for", parallel_for_arguments);

    root->getChild(2)->addChild("message", simple_msg_args);

    Arguments child_msg_args{ sg::ExpressionValue{ std::vector<sg::ExpressionValue>{1} }, sg::ExpressionValue{"Hello from Parallel"} };
    root->getChild(2)->addChild("message", child_msg_args);

    root->addChild("message", simple_msg_args);

    Arguments parallel_for_args2{sg::ExpressionValue{2}};
    root->addChild("parallel_for", parallel_for_args2);

    root->getChild(4)->addChild("message", simple_msg_args2);
    Arguments child_msg_args2{ sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello from Parallel 2"}};
    root->getChild(4)->addChild("message", child_msg_args2);

    root->addChild("message", simple_msg_args);

    Target target = { "targetName "};
    Prompt myPrompt = { PromptType::STRING, 0, 10 };
    GameRoomID gameRoomID = { 100132 }; // unique id, one GameRunner per GameRoom
    networking::Connection connection = { 18 };
    ExecState exState = {1};
    jsonMsg newMessage = { target, "Test!", myPrompt, gameRoomID, connection, exState };

    gameRunner = GameRunner(std::move(root), params);
}

RoomDeque roomState::RunningGame::poll(const jsonMsg& roomMessage, GameRoom& room) {
    RoomDeque returnMessages { gameRunner.runGameUntilBlocked(roomMessage) };

    return returnMessages;
}

std::deque<networking::Message> roomState::RunningGame::createMessagesFromBucket(const DummyBucket& bucket) {
    return {};
}

bool roomState::RunningGame::gameComplete(DummyBucket bucket) {
    return true;
}






