#pragma once

#include <vector>
#include <string>

class DummyBucket {
public:
    DummyBucket() : ok(true) {}

    explicit DummyBucket(bool ok) : ok(ok) {}

    [[nodiscard]] bool isOk() const {
        return ok;
    }

    void setOk(bool set) {
        this->ok = set;
    }

    // Function to get a list of player names
    [[nodiscard]] const std::vector<std::string>& getPlayers() const {
        return players;
    }

    // Function to add a player name to the list
    void addPlayer(const std::string& playerName) {
        players.push_back(playerName);
    }

    [[nodiscard]] std::string_view getPlayerName(size_t index) const {
        if (index < players.size()) {
            return players[index];
        }
        // Return an empty string_view or handle the out-of-range index as needed
        return std::string_view();
    }

    bool isTargetUpdated(const std::pair<std::string, std::string>& target) {
        return true; //TODO
    }

private:
    bool ok;
    std::vector<std::string> players;
};
