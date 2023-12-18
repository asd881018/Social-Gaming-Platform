#include <iostream>
#include <unistd.h>

#include <treeSitter/treeSitter.h>
#include <memory>
#include <nlohmann/json.hpp>

#include "NodeActions.h"
#include "GameRunner.h"
#include "Server.h"

#include "msgCreator.h"

//#include "InOrderNode.h"
//#include "MessageAllNode.h"
//#include "InputTextNode.h"
//#include "InParallelNode.h"
//#include "TimerNode.h"

using json = nlohmann::json;


void onConnect(networking::Connection c);

void onDisconnect(networking::Connection c);

/*
void nodeDemo(classifier::MessageClassifier* messageClassifier) {

    InOrderNode rules{};
    InOrderNode rules2{};


    TimerNode timer1 = TimerNode{2};
    Node pTimer = Node(std::make_unique<TimerNode>(timer1));

    TimerNode timer2 = TimerNode{4};
    Node pTimer2 = Node(std::make_unique<TimerNode>(timer2));

    MessageAllNode helloMessage("Hello there!", messageClassifier);
    Node pHello =  Node(std::make_unique<MessageAllNode>(helloMessage));

    InputTextNode getX({"x", "0"}, "Give me a value for x", "Billy Jean", messageClassifier);
    Node pGet = Node(std::make_unique<InputTextNode>(getX));

    InParallelNode parallel{};
    Node messageAll =  Node(std::make_unique<MessageAllNode>("You should get this twice", messageClassifier));
    parallel.insert(messageAll);

    Node pParallel = Node(std::make_unique<InParallelNode>(std::move(parallel)));

    rules.insert(pHello);
    rules.insert(pTimer);
    rules.insert(pGet);
    rules2.insert(pTimer2);
    rules2.insert(pParallel);

    bool run = true;
    while (run) {
        auto result = rules.execute(DataBucket{});
        auto result2 = rules2.execute(DataBucket{});
        if (result.isOk() && result2.isOk()) {
            run = false;
        }

        if (!result.isOk()) {
            std::cout<<"Waiting on 1\n";
        }

        if (!result2.isOk()) {
            std::cout<<"Waiting on 2\n";
        }


        sleep(1);
    }
}
*/   // nodeDemo

/*
void parallelismDemo(classifier::MessageClassifier* messageClassifier) {
    InOrderNode gameWithInput{};

    // GameInfo prints a value, waits 4 seconds, then prints another value
    InputTextNode getName({"x", "0"}, "Give me a value for x", "Billy Bob", messageClassifier);
    Node pGet = Node(std::make_unique<InputTextNode>(getName));

    TimerNode getNameDelay = TimerNode{4};
    Node pTimer = Node(std::make_unique<TimerNode>(getNameDelay));

    MessageAllNode helloMessage("Hello there!", messageClassifier);
    Node pHello =  Node(std::make_unique<MessageAllNode>(helloMessage));

    InParallelNode parallel{};

    parallel.insert(pGet);
    parallel.insert(pTimer);
    parallel.insert(pHello);

    Node pParallel = Node(std::make_unique<InParallelNode>(std::move(parallel)));

    MessageAllNode doneMessage("Goodbye!", messageClassifier);
    Node pDone =  Node(std::make_unique<MessageAllNode>(doneMessage));

    gameWithInput.insert(pParallel);

    gameWithInput.insert(pDone);

    for (int i = 0; i < 2; ++i) {
        bool run = true;
        while (run) {
            auto result = gameWithInput.execute(DataBucket{});
            if (result.isOk()) {
                run = false;
            }

            if (!result.isOk()) {
                std::cout<<"Waiting\n";
            }


            sleep(1);
        }

    }
}
*/   // parallelismDemo

/*
 std::string
 getHTTPMessage(const char* htmlLocation) {
   if (access(htmlLocation, R_OK ) != -1) {
     std::ifstream infile{htmlLocation};
     return std::string{std::istreambuf_iterator<char>(infile),
                        std::istreambuf_iterator<char>()};

   }

   std::cerr << "Unable to open HTML index file:\n"
             << htmlLocation << "\n";
   std::exit(-1);
 }
*/   // getHTTPMessage


void parallelismDemoRefactor() {
    std::cout << "Parallelism Demo Refactor:\n";

    // testing args
    Arguments dummy_arguments{sg::ExpressionValue{0}};

    /* Build Demo Tree */
    auto root = std::make_unique<RuleNode>("root", dummy_arguments);

    Arguments simple_msg_args{sg::ExpressionValue{"all"}, sg::ExpressionValue{"I am a basic message to all players"}};
    Arguments simple_msg_args2{sg::ExpressionValue{"all"}, sg::ExpressionValue{"I am a second message to all players"}};

    root->addChild("message", simple_msg_args);
    root->addChild("message", simple_msg_args2);

    // todo: pass a list instead
    Arguments parallel_for_arguments{sg::ExpressionValue{4}};
    root->addChild("parallel_for", parallel_for_arguments);

    // todo: make 0 and 2 identifiers instead.  DANGER with int->uintptr_t conversion as is.
    Arguments textInputArgs{sg::ExpressionValue{1234},
                            sg::ExpressionValue{"Please enter text input"},
                            sg::ExpressionValue{"TargetIdentifier"}};
    root->getChild(2)->addChild("input_text", textInputArgs);

    Arguments child_msg_args{sg::ExpressionValue{std::vector<sg::ExpressionValue>{1}},
                             sg::ExpressionValue{"Hello from Parallel"}};
    root->getChild(2)->addChild("message", child_msg_args);
//    root->getChild(2)->addChild("message", simple_msg_args2);

    root->addChild("message", simple_msg_args);

    Arguments parallel_for_args2{sg::ExpressionValue{2}};
    root->addChild("parallel_for", parallel_for_args2);

    root->getChild(4)->addChild("input_text", textInputArgs);
    Arguments child_msg_args2{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello from Parallel 2"}};
    root->getChild(4)->addChild("message", child_msg_args2);

    root->addChild("message", simple_msg_args);
//    root->addChild("message", dummy_arguments);


    // jsonMsg usage demo:
    // todo: need an identifier for the target GameRunner as well?

    Prompt myPrompt = {PromptType::STRING, 0, 10};
    GameRoomID gameRoomID = {100132}; // unique id, one GameRunner per GameRoom
    networking::Connection connection = {18}; // player
    Target target = {"targetName "};
    ExecState exState = {0};
    jsonMsg newMessage = {target, "Test!", myPrompt, gameRoomID, connection, exState};


//    root->print();

//    auto bucket = std::make_unique<DataBucket>();
    DataBucket dummyBucket;
    GameParams dummyParams;
//    dummyParams.bucket = dummyBucket;
    dummyParams.globalState = ValueMap{};
    // todo: testing only
    dummyParams.players = {networking::Connection{11},
                           networking::Connection{12},
                           networking::Connection{13},
                           networking::Connection{14}};

    dummyParams.gameRoomId = GameRoomID{16};

    GameRunner gr(std::move(root), dummyParams);
    auto results = gr.runGameUntilBlocked(newMessage);
//    std::cout << "Results: " << results

    Target target3 = {"player"};
    jsonMsg updateMessage3 = {target3, "Updated3!", myPrompt, gameRoomID, connection, ExecState{3}};
    auto results3 = gr.runGameUntilBlocked(updateMessage3);

    Target target4 = {"player"};
    jsonMsg updateMessage4 = {target4, "Updated4!", myPrompt, gameRoomID, connection, ExecState{4}};
    auto results4 = gr.runGameUntilBlocked(updateMessage4);

    Target target1 = {"player"};
    jsonMsg updateMessage1 = {target1, "Updated!", myPrompt, gameRoomID, connection, ExecState{1}};
    auto results1 = gr.runGameUntilBlocked(updateMessage1);

    Target target2 = {"player"};
    jsonMsg updateMessage2 = {target2, "Updated2!", myPrompt, gameRoomID, connection, ExecState{2}};
    auto results2 = gr.runGameUntilBlocked(updateMessage2);

}

#define SIMPLE_MESSAGE_GAME "./resource/games/simple-message.game"
#define HELLO_TREE_SITTER_GAME "./resource/games/hello-tree-sitter.game"
#define ROCK_PAPER_SCISSORS_GAME "./resource/games/rock-paper-scissors.game"

void demoParsingRun() {

    std::cout << "Demo Parsing Run:\n";
    TreeParser tp{HELLO_TREE_SITTER_GAME};
    ts::Node root = tp.getRootNode();
    tp.parseRulesNode(root);
    std::unique_ptr<RuleNode> ruleTreeRoot = tp.getRuleTreeRoot();

    /** ------------------ Debuggin Printing ------------------ */
    std::cout << "Printing ruleTreeRoot: ";
    ruleTreeRoot->print();
    std::cout << "\n";
    /** ------------------ Debuggin Printing ------------------ */

    /** ------------------ Game Runner ------------------ */

    Prompt myPrompt = {PromptType::STRING, 0, 10};
    GameRoomID gameRoomID = {100132}; // unique id, one GameRunner per GameRoom
    networking::Connection connection = {18}; // player
    Target target = {"targetName "};
    ExecState exState = {0};
    jsonMsg newMessage = {target, "Test!", myPrompt, gameRoomID, connection, exState};

    GameParams dummyParams;
//    dummyParams.bucket = dummyBucket;
    dummyParams.globalState = ValueMap{};
    // todo: testing only
    dummyParams.players = {networking::Connection{11},
                           networking::Connection{12},
                           networking::Connection{13},
                           networking::Connection{14}};

    dummyParams.gameRoomId = GameRoomID{16};
    GameRunner gr(std::move(ruleTreeRoot), dummyParams);
    auto results = gr.runGameUntilBlocked(newMessage);

//    delete ruleTreeRoot;
}

int main() {
//    unsigned short port = 8000;
//    std::string http = "webchat.html";

//    std::unique_ptr<networking::Server> serverPtr = std::make_unique<networking::Server>(port, http, onConnect, onDisconnect);
//    classifier::MessageClassifier messageClassifier(std::move(serverPtr));
    demoParsingRun();
//    std::cout << "Hello world!\n";

//    generateRuleTreeRoot();
//    nodeDemo(&messageClassifier);
//    sayHelloLogic();
//    parallelismDemo(&messageClassifier);
//    parallelismDemoRefactor();
}