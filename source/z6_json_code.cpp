/**
 * @file source/singleton/singleton.h
 * @author  kalaiyarasan S
 * @date 2026-08-01
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */
#include "z6_json_code.h"

class Z6BoardJson : public Z6json {
private:
    bool active_;
    bool active_valid_;
    int turn_;
    bool turn_valid_;
    int nsides_;
    bool nsides_valid_;
    int nseeds_;
    bool nseeds_valid_;
    int npits_;
    bool npits_valid_;
    std::vector<Z6PitJson> pits_;
    bool pits_valid_;
    int nround_;
    bool nround_valid_;
    std::string visibility_;
    bool visibility_valid_;
    std::vector<Z6MoveHistoryJson> history_;
    bool history_valid_;

public:
    // Default Constructor
    Z6BoardJson() : active_(true), active_valid_(false), turn_(0), turn_valid_(false), nsides_(2), nsides_valid_(false), nseeds_(4), nseeds_valid_(false), npits_(7), npits_valid_(false), pits_valid_(false), nround_(1), nround_valid_(false), visibility_("TRANSPARENT"), visibility_valid_(false), history_valid_(false) {}

    // Copy Constructor
    Z6BoardJson(const Z6BoardJson& other) : active_(other.active_), active_valid_(other.active_valid_), turn_(other.turn_), turn_valid_(other.turn_valid_), nsides_(other.nsides_), nsides_valid_(other.nsides_valid_), nseeds_(other.nseeds_), nseeds_valid_(other.nseeds_valid_), npits_(other.npits_), npits_valid_(other.npits_valid_), pits_(other.pits_), pits_valid_(other.pits_valid_), nround_(other.nround_), nround_valid_(other.nround_valid_), visibility_(other.visibility_), visibility_valid_(other.visibility_valid_), history_(other.history_), history_valid_(other.history_valid_) {}

    // Copy Assignment Operator
    Z6BoardJson& operator=(const Z6BoardJson& other) {
        if (this != &other) {
            active_ = other.active_;
            active_valid_ = other.active_valid_;
            turn_ = other.turn_;
            turn_valid_ = other.turn_valid_;
            nsides_ = other.nsides_;
            nsides_valid_ = other.nsides_valid_;
            nseeds_ = other.nseeds_;
            nseeds_valid_ = other.nseeds_valid_;
            npits_ = other.npits_;
            npits_valid_ = other.npits_valid_;
            pits_ = other.pits_;
            pits_valid_ = other.pits_valid_;
            nround_ = other.nround_;
            nround_valid_ = other.nround_valid_;
            visibility_ = other.visibility_;
            visibility_valid_ = other.visibility_valid_;
            history_ = other.history_;
            history_valid_ = other.history_valid_;
        }
        return *this;
    }

    bool getActive(bool& val) const {
        if (!active_valid_) return false;
        val = active_;
        return true;
    }

    bool getTurn(int& val) const {
        if (!turn_valid_) return false;
        val = turn_;
        return true;
    }

    bool getNsides(int& val) const {
        if (!nsides_valid_) return false;
        val = nsides_;
        return true;
    }

    bool getNseeds(int& val) const {
        if (!nseeds_valid_) return false;
        val = nseeds_;
        return true;
    }

    bool getNpits(int& val) const {
        if (!npits_valid_) return false;
        val = npits_;
        return true;
    }

    bool getPits(size_t index, Z6PitJson& val) const {
        if (!pits_valid_ || index >= pits_.size()) return false;
        val = pits_[index];
        return true;
    }

    bool getPitsVector(std::vector<Z6PitJson>& val) const {
        if (!pits_valid_) return false;
        val = pits_;
        return true;
    }

    bool getNround(int& val) const {
        if (!nround_valid_) return false;
        val = nround_;
        return true;
    }

    bool getVisibility(std::string& val) const {
        if (!visibility_valid_) return false;
        val = visibility_;
        return true;
    }

    bool getHistory(size_t index, Z6MoveHistoryJson& val) const {
        if (!history_valid_ || index >= history_.size()) return false;
        val = history_[index];
        return true;
    }

    bool getHistoryVector(std::vector<Z6MoveHistoryJson>& val) const {
        if (!history_valid_) return false;
        val = history_;
        return true;
    }

    void setActive(const bool& val) {
        active_ = val;
        active_valid_ = true;
    }

    void setTurn(const int& val) {
        turn_ = val;
        turn_valid_ = true;
    }

    void setNsides(const int& val) {
        nsides_ = val;
        nsides_valid_ = true;
    }

    void setNseeds(const int& val) {
        nseeds_ = val;
        nseeds_valid_ = true;
    }

    void setNpits(const int& val) {
        npits_ = val;
        npits_valid_ = true;
    }

    void setPits(const std::vector<Z6PitJson>& val) {
        pits_ = val;
        pits_valid_ = true;
    }

    void setNround(const int& val) {
        nround_ = val;
        nround_valid_ = true;
    }

    void setVisibility(const std::string& val) {
        visibility_ = val;
        visibility_valid_ = true;
    }

    void setHistory(const std::vector<Z6MoveHistoryJson>& val) {
        history_ = val;
        history_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Active")) {
            active_ = j["Active"];
            active_valid_ = true;
        }
        if (j.contains("Turn")) {
            turn_ = j["Turn"];
            turn_valid_ = true;
        }
        if (j.contains("Nsides")) {
            nsides_ = j["Nsides"];
            nsides_valid_ = true;
        }
        if (j.contains("Nseeds")) {
            nseeds_ = j["Nseeds"];
            nseeds_valid_ = true;
        }
        if (j.contains("Npits")) {
            npits_ = j["Npits"];
            npits_valid_ = true;
        }
        if (j.contains("Pits")) {
            pits_.clear();
            for (const auto& item : j["Pits"]) {
                Z6PitJson obj;
                obj.from_json(item.dump());
                pits_.push_back(obj);
            }
            pits_valid_ = true;
        }
        if (j.contains("Nround")) {
            nround_ = j["Nround"];
            nround_valid_ = true;
        }
        if (j.contains("Visibility")) {
            visibility_ = j["Visibility"];
            visibility_valid_ = true;
        }
        if (j.contains("History")) {
            history_.clear();
            for (const auto& item : j["History"]) {
                Z6MoveHistoryJson obj;
                obj.from_json(item.dump());
                history_.push_back(obj);
            }
            history_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (active_valid_) {
            j["Active"] = active_;
        }
        if (turn_valid_) {
            j["Turn"] = turn_;
        }
        if (nsides_valid_) {
            j["Nsides"] = nsides_;
        }
        if (nseeds_valid_) {
            j["Nseeds"] = nseeds_;
        }
        if (npits_valid_) {
            j["Npits"] = npits_;
        }
        if (pits_valid_) {
            json arr = json::array();
            for (const auto& item : pits_) {
                arr.push_back(json::parse(item.to_json()));
            }
            j["Pits"] = arr;
        }
        if (nround_valid_) {
            j["Nround"] = nround_;
        }
        if (visibility_valid_) {
            j["Visibility"] = visibility_;
        }
        if (history_valid_) {
            json arr = json::array();
            for (const auto& item : history_) {
                arr.push_back(json::parse(item.to_json()));
            }
            j["History"] = arr;
        }
        return j.dump();
    }
};

class Z6GameJson : public Z6json {
private:
    std::string name_;
    bool name_valid_;
    int level_;
    bool level_valid_;
    std::vector<std::string> players_;
    bool players_valid_;
    bool human_;
    bool human_valid_;

public:
    // Default Constructor
    Z6GameJson() : name_("INDIAN_CLASSIC_SURYA"), name_valid_(false), level_(1), level_valid_(false), players_valid_(false), human_(false), human_valid_(false) {}

    // Copy Constructor
    Z6GameJson(const Z6GameJson& other) : name_(other.name_), name_valid_(other.name_valid_), level_(other.level_), level_valid_(other.level_valid_), players_(other.players_), players_valid_(other.players_valid_), human_(other.human_), human_valid_(other.human_valid_) {}

    // Copy Assignment Operator
    Z6GameJson& operator=(const Z6GameJson& other) {
        if (this != &other) {
            name_ = other.name_;
            name_valid_ = other.name_valid_;
            level_ = other.level_;
            level_valid_ = other.level_valid_;
            players_ = other.players_;
            players_valid_ = other.players_valid_;
            human_ = other.human_;
            human_valid_ = other.human_valid_;
        }
        return *this;
    }

    bool getName(std::string& val) const {
        if (!name_valid_) return false;
        val = name_;
        return true;
    }

    bool getLevel(int& val) const {
        if (!level_valid_) return false;
        val = level_;
        return true;
    }

    bool getPlayers(size_t index, std::string& val) const {
        if (!players_valid_ || index >= players_.size()) return false;
        val = players_[index];
        return true;
    }

    bool getPlayersVector(std::vector<std::string>& val) const {
        if (!players_valid_) return false;
        val = players_;
        return true;
    }

    bool getHuman(bool& val) const {
        if (!human_valid_) return false;
        val = human_;
        return true;
    }

    void setName(const std::string& val) {
        name_ = val;
        name_valid_ = true;
    }

    void setLevel(const int& val) {
        level_ = val;
        level_valid_ = true;
    }

    void setPlayers(const std::vector<std::string>& val) {
        players_ = val;
        players_valid_ = true;
    }

    void setHuman(const bool& val) {
        human_ = val;
        human_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Name")) {
            name_ = j["Name"];
            name_valid_ = true;
        }
        if (j.contains("Level")) {
            level_ = j["Level"];
            level_valid_ = true;
        }
        if (j.contains("Players")) {
            players_ = j["Players"].get<std::vector<std::string>>();
            players_valid_ = true;
        }
        if (j.contains("Human")) {
            human_ = j["Human"];
            human_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (name_valid_) {
            j["Name"] = name_;
        }
        if (level_valid_) {
            j["Level"] = level_;
        }
        if (players_valid_) {
            j["Players"] = players_;
        }
        if (human_valid_) {
            j["Human"] = human_;
        }
        return j.dump();
    }
};

class Z6MoveHistoryJson : public Z6json {
private:
    Z6MoveJson move_;
    bool move_valid_;
    std::vector<Z6DetailJson> details_;
    bool details_valid_;

public:
    // Default Constructor
    Z6MoveHistoryJson() : move_valid_(false), details_valid_(false) {}

    // Copy Constructor
    Z6MoveHistoryJson(const Z6MoveHistoryJson& other) : move_(other.move_), move_valid_(other.move_valid_), details_(other.details_), details_valid_(other.details_valid_) {}

    // Copy Assignment Operator
    Z6MoveHistoryJson& operator=(const Z6MoveHistoryJson& other) {
        if (this != &other) {
            move_ = other.move_;
            move_valid_ = other.move_valid_;
            details_ = other.details_;
            details_valid_ = other.details_valid_;
        }
        return *this;
    }

    bool getMove(Z6MoveJson& val) const {
        if (!move_valid_) return false;
        val = move_;
        return true;
    }

    bool getDetails(size_t index, Z6DetailJson& val) const {
        if (!details_valid_ || index >= details_.size()) return false;
        val = details_[index];
        return true;
    }

    bool getDetailsVector(std::vector<Z6DetailJson>& val) const {
        if (!details_valid_) return false;
        val = details_;
        return true;
    }

    void setMove(const Z6MoveJson& val) {
        move_ = val;
        move_valid_ = true;
    }

    void setDetails(const std::vector<Z6DetailJson>& val) {
        details_ = val;
        details_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Move")) {
            move_.from_json(j["Move"].dump());
            move_valid_ = true;
        }
        if (j.contains("Details")) {
            details_.clear();
            for (const auto& item : j["Details"]) {
                Z6DetailJson obj;
                obj.from_json(item.dump());
                details_.push_back(obj);
            }
            details_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (move_valid_) {
            j["Move"] = json::parse(move_.to_json());
        }
        if (details_valid_) {
            json arr = json::array();
            for (const auto& item : details_) {
                arr.push_back(json::parse(item.to_json()));
            }
            j["Details"] = arr;
        }
        return j.dump();
    }
};

class Z6MoveJson : public Z6json {
private:
    int sequence_;
    bool sequence_valid_;
    std::string timestamp_;
    bool timestamp_valid_;
    std::string player_;
    bool player_valid_;
    int index_;
    bool index_valid_;

public:
    // Default Constructor
    Z6MoveJson() : sequence_(0), sequence_valid_(false), timestamp_valid_(false), player_valid_(false), index_(0), index_valid_(false) {}

    // Copy Constructor
    Z6MoveJson(const Z6MoveJson& other) : sequence_(other.sequence_), sequence_valid_(other.sequence_valid_), timestamp_(other.timestamp_), timestamp_valid_(other.timestamp_valid_), player_(other.player_), player_valid_(other.player_valid_), index_(other.index_), index_valid_(other.index_valid_) {}

    // Copy Assignment Operator
    Z6MoveJson& operator=(const Z6MoveJson& other) {
        if (this != &other) {
            sequence_ = other.sequence_;
            sequence_valid_ = other.sequence_valid_;
            timestamp_ = other.timestamp_;
            timestamp_valid_ = other.timestamp_valid_;
            player_ = other.player_;
            player_valid_ = other.player_valid_;
            index_ = other.index_;
            index_valid_ = other.index_valid_;
        }
        return *this;
    }

    bool getSequence(int& val) const {
        if (!sequence_valid_) return false;
        val = sequence_;
        return true;
    }

    bool getTimestamp(std::string& val) const {
        if (!timestamp_valid_) return false;
        val = timestamp_;
        return true;
    }

    bool getPlayer(std::string& val) const {
        if (!player_valid_) return false;
        val = player_;
        return true;
    }

    bool getIndex(int& val) const {
        if (!index_valid_) return false;
        val = index_;
        return true;
    }

    void setSequence(const int& val) {
        sequence_ = val;
        sequence_valid_ = true;
    }

    void setTimestamp(const std::string& val) {
        timestamp_ = val;
        timestamp_valid_ = true;
    }

    void setPlayer(const std::string& val) {
        player_ = val;
        player_valid_ = true;
    }

    void setIndex(const int& val) {
        index_ = val;
        index_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Sequence")) {
            sequence_ = j["Sequence"];
            sequence_valid_ = true;
        }
        if (j.contains("Timestamp")) {
            timestamp_ = j["Timestamp"];
            timestamp_valid_ = true;
        }
        if (j.contains("Player")) {
            player_ = j["Player"];
            player_valid_ = true;
        }
        if (j.contains("Index")) {
            index_ = j["Index"];
            index_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (sequence_valid_) {
            j["Sequence"] = sequence_;
        }
        if (timestamp_valid_) {
            j["Timestamp"] = timestamp_;
        }
        if (player_valid_) {
            j["Player"] = player_;
        }
        if (index_valid_) {
            j["Index"] = index_;
        }
        return j.dump();
    }
};

class Z6DetailJson : public Z6json {
private:
    Z6PitJson pit_;
    bool pit_valid_;

public:
    // Default Constructor
    Z6DetailJson() : pit_valid_(false) {}

    // Copy Constructor
    Z6DetailJson(const Z6DetailJson& other) : pit_(other.pit_), pit_valid_(other.pit_valid_) {}

    // Copy Assignment Operator
    Z6DetailJson& operator=(const Z6DetailJson& other) {
        if (this != &other) {
            pit_ = other.pit_;
            pit_valid_ = other.pit_valid_;
        }
        return *this;
    }

    bool getPit(Z6PitJson& val) const {
        if (!pit_valid_) return false;
        val = pit_;
        return true;
    }

    void setPit(const Z6PitJson& val) {
        pit_ = val;
        pit_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Pit")) {
            pit_.from_json(j["Pit"].dump());
            pit_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (pit_valid_) {
            j["Pit"] = json::parse(pit_.to_json());
        }
        return j.dump();
    }
};

class Z6MoveReqJson : public Z6json {
private:
    std::string gameID_;
    bool gameID_valid_;
    Z6MoveJson move_;
    bool move_valid_;

public:
    // Default Constructor
    Z6MoveReqJson() : gameID_valid_(false), move_valid_(false) {}

    // Copy Constructor
    Z6MoveReqJson(const Z6MoveReqJson& other) : gameID_(other.gameID_), gameID_valid_(other.gameID_valid_), move_(other.move_), move_valid_(other.move_valid_) {}

    // Copy Assignment Operator
    Z6MoveReqJson& operator=(const Z6MoveReqJson& other) {
        if (this != &other) {
            gameID_ = other.gameID_;
            gameID_valid_ = other.gameID_valid_;
            move_ = other.move_;
            move_valid_ = other.move_valid_;
        }
        return *this;
    }

    bool getGameID(std::string& val) const {
        if (!gameID_valid_) return false;
        val = gameID_;
        return true;
    }

    bool getMove(Z6MoveJson& val) const {
        if (!move_valid_) return false;
        val = move_;
        return true;
    }

    void setGameID(const std::string& val) {
        gameID_ = val;
        gameID_valid_ = true;
    }

    void setMove(const Z6MoveJson& val) {
        move_ = val;
        move_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("GameID")) {
            gameID_ = j["GameID"];
            gameID_valid_ = true;
        }
        if (j.contains("Move")) {
            move_.from_json(j["Move"].dump());
            move_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (gameID_valid_) {
            j["GameID"] = gameID_;
        }
        if (move_valid_) {
            j["Move"] = json::parse(move_.to_json());
        }
        return j.dump();
    }
};

class Z6BoardRespJson : public Z6json {
private:
    bool active_;
    bool active_valid_;
    int turn_;
    bool turn_valid_;
    int nsides_;
    bool nsides_valid_;
    int nseeds_;
    bool nseeds_valid_;
    int npits_;
    bool npits_valid_;
    std::vector<Z6PitJson> pits_;
    bool pits_valid_;
    int nround_;
    bool nround_valid_;
    std::string visibility_;
    bool visibility_valid_;
    std::vector<Z6MoveJson> history_;
    bool history_valid_;
    std::vector<Z6DetailJson> details_;
    bool details_valid_;

public:
    // Default Constructor
    Z6BoardRespJson() : active_(true), active_valid_(false), turn_(0), turn_valid_(false), nsides_(2), nsides_valid_(false), nseeds_(4), nseeds_valid_(false), npits_(7), npits_valid_(false), pits_valid_(false), nround_(1), nround_valid_(false), visibility_("TRANSPARENT"), visibility_valid_(false), history_valid_(false), details_valid_(false) {}

    // Copy Constructor
    Z6BoardRespJson(const Z6BoardRespJson& other) : active_(other.active_), active_valid_(other.active_valid_), turn_(other.turn_), turn_valid_(other.turn_valid_), nsides_(other.nsides_), nsides_valid_(other.nsides_valid_), nseeds_(other.nseeds_), nseeds_valid_(other.nseeds_valid_), npits_(other.npits_), npits_valid_(other.npits_valid_), pits_(other.pits_), pits_valid_(other.pits_valid_), nround_(other.nround_), nround_valid_(other.nround_valid_), visibility_(other.visibility_), visibility_valid_(other.visibility_valid_), history_(other.history_), history_valid_(other.history_valid_), details_(other.details_), details_valid_(other.details_valid_) {}

    // Copy Assignment Operator
    Z6BoardRespJson& operator=(const Z6BoardRespJson& other) {
        if (this != &other) {
            active_ = other.active_;
            active_valid_ = other.active_valid_;
            turn_ = other.turn_;
            turn_valid_ = other.turn_valid_;
            nsides_ = other.nsides_;
            nsides_valid_ = other.nsides_valid_;
            nseeds_ = other.nseeds_;
            nseeds_valid_ = other.nseeds_valid_;
            npits_ = other.npits_;
            npits_valid_ = other.npits_valid_;
            pits_ = other.pits_;
            pits_valid_ = other.pits_valid_;
            nround_ = other.nround_;
            nround_valid_ = other.nround_valid_;
            visibility_ = other.visibility_;
            visibility_valid_ = other.visibility_valid_;
            history_ = other.history_;
            history_valid_ = other.history_valid_;
            details_ = other.details_;
            details_valid_ = other.details_valid_;
        }
        return *this;
    }

    bool getActive(bool& val) const {
        if (!active_valid_) return false;
        val = active_;
        return true;
    }

    bool getTurn(int& val) const {
        if (!turn_valid_) return false;
        val = turn_;
        return true;
    }

    bool getNsides(int& val) const {
        if (!nsides_valid_) return false;
        val = nsides_;
        return true;
    }

    bool getNseeds(int& val) const {
        if (!nseeds_valid_) return false;
        val = nseeds_;
        return true;
    }

    bool getNpits(int& val) const {
        if (!npits_valid_) return false;
        val = npits_;
        return true;
    }

    bool getPits(size_t index, Z6PitJson& val) const {
        if (!pits_valid_ || index >= pits_.size()) return false;
        val = pits_[index];
        return true;
    }

    bool getPitsVector(std::vector<Z6PitJson>& val) const {
        if (!pits_valid_) return false;
        val = pits_;
        return true;
    }

    bool getNround(int& val) const {
        if (!nround_valid_) return false;
        val = nround_;
        return true;
    }

    bool getVisibility(std::string& val) const {
        if (!visibility_valid_) return false;
        val = visibility_;
        return true;
    }

    bool getHistory(size_t index, Z6MoveJson& val) const {
        if (!history_valid_ || index >= history_.size()) return false;
        val = history_[index];
        return true;
    }

    bool getHistoryVector(std::vector<Z6MoveJson>& val) const {
        if (!history_valid_) return false;
        val = history_;
        return true;
    }

    bool getDetails(size_t index, Z6DetailJson& val) const {
        if (!details_valid_ || index >= details_.size()) return false;
        val = details_[index];
        return true;
    }

    bool getDetailsVector(std::vector<Z6DetailJson>& val) const {
        if (!details_valid_) return false;
        val = details_;
        return true;
    }

    void setActive(const bool& val) {
        active_ = val;
        active_valid_ = true;
    }

    void setTurn(const int& val) {
        turn_ = val;
        turn_valid_ = true;
    }

    void setNsides(const int& val) {
        nsides_ = val;
        nsides_valid_ = true;
    }

    void setNseeds(const int& val) {
        nseeds_ = val;
        nseeds_valid_ = true;
    }

    void setNpits(const int& val) {
        npits_ = val;
        npits_valid_ = true;
    }

    void setPits(const std::vector<Z6PitJson>& val) {
        pits_ = val;
        pits_valid_ = true;
    }

    void setNround(const int& val) {
        nround_ = val;
        nround_valid_ = true;
    }

    void setVisibility(const std::string& val) {
        visibility_ = val;
        visibility_valid_ = true;
    }

    void setHistory(const std::vector<Z6MoveJson>& val) {
        history_ = val;
        history_valid_ = true;
    }

    void setDetails(const std::vector<Z6DetailJson>& val) {
        details_ = val;
        details_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Active")) {
            active_ = j["Active"];
            active_valid_ = true;
        }
        if (j.contains("Turn")) {
            turn_ = j["Turn"];
            turn_valid_ = true;
        }
        if (j.contains("Nsides")) {
            nsides_ = j["Nsides"];
            nsides_valid_ = true;
        }
        if (j.contains("Nseeds")) {
            nseeds_ = j["Nseeds"];
            nseeds_valid_ = true;
        }
        if (j.contains("Npits")) {
            npits_ = j["Npits"];
            npits_valid_ = true;
        }
        if (j.contains("Pits")) {
            pits_.clear();
            for (const auto& item : j["Pits"]) {
                Z6PitJson obj;
                obj.from_json(item.dump());
                pits_.push_back(obj);
            }
            pits_valid_ = true;
        }
        if (j.contains("Nround")) {
            nround_ = j["Nround"];
            nround_valid_ = true;
        }
        if (j.contains("Visibility")) {
            visibility_ = j["Visibility"];
            visibility_valid_ = true;
        }
        if (j.contains("History")) {
            history_.clear();
            for (const auto& item : j["History"]) {
                Z6MoveJson obj;
                obj.from_json(item.dump());
                history_.push_back(obj);
            }
            history_valid_ = true;
        }
        if (j.contains("Details")) {
            details_.clear();
            for (const auto& item : j["Details"]) {
                Z6DetailJson obj;
                obj.from_json(item.dump());
                details_.push_back(obj);
            }
            details_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (active_valid_) {
            j["Active"] = active_;
        }
        if (turn_valid_) {
            j["Turn"] = turn_;
        }
        if (nsides_valid_) {
            j["Nsides"] = nsides_;
        }
        if (nseeds_valid_) {
            j["Nseeds"] = nseeds_;
        }
        if (npits_valid_) {
            j["Npits"] = npits_;
        }
        if (pits_valid_) {
            json arr = json::array();
            for (const auto& item : pits_) {
                arr.push_back(json::parse(item.to_json()));
            }
            j["Pits"] = arr;
        }
        if (nround_valid_) {
            j["Nround"] = nround_;
        }
        if (visibility_valid_) {
            j["Visibility"] = visibility_;
        }
        if (history_valid_) {
            json arr = json::array();
            for (const auto& item : history_) {
                arr.push_back(json::parse(item.to_json()));
            }
            j["History"] = arr;
        }
        if (details_valid_) {
            json arr = json::array();
            for (const auto& item : details_) {
                arr.push_back(json::parse(item.to_json()));
            }
            j["Details"] = arr;
        }
        return j.dump();
    }
};

class Z6PitJson : public Z6json {
private:
    std::string type_;
    bool type_valid_;
    int index_;
    bool index_valid_;
    int owner_;
    bool owner_valid_;
    int side_;
    bool side_valid_;
    std::vector<int> share_;
    bool share_valid_;
    int value_;
    bool value_valid_;

public:
    // Default Constructor
    Z6PitJson() : type_("NORMAL"), type_valid_(false), index_(100), index_valid_(false), owner_(10), owner_valid_(false), side_(0), side_valid_(false), share_valid_(false), value_(100), value_valid_(false) {}

    // Copy Constructor
    Z6PitJson(const Z6PitJson& other) : type_(other.type_), type_valid_(other.type_valid_), index_(other.index_), index_valid_(other.index_valid_), owner_(other.owner_), owner_valid_(other.owner_valid_), side_(other.side_), side_valid_(other.side_valid_), share_(other.share_), share_valid_(other.share_valid_), value_(other.value_), value_valid_(other.value_valid_) {}

    // Copy Assignment Operator
    Z6PitJson& operator=(const Z6PitJson& other) {
        if (this != &other) {
            type_ = other.type_;
            type_valid_ = other.type_valid_;
            index_ = other.index_;
            index_valid_ = other.index_valid_;
            owner_ = other.owner_;
            owner_valid_ = other.owner_valid_;
            side_ = other.side_;
            side_valid_ = other.side_valid_;
            share_ = other.share_;
            share_valid_ = other.share_valid_;
            value_ = other.value_;
            value_valid_ = other.value_valid_;
        }
        return *this;
    }

    bool getType(std::string& val) const {
        if (!type_valid_) return false;
        val = type_;
        return true;
    }

    bool getIndex(int& val) const {
        if (!index_valid_) return false;
        val = index_;
        return true;
    }

    bool getOwner(int& val) const {
        if (!owner_valid_) return false;
        val = owner_;
        return true;
    }

    bool getSide(int& val) const {
        if (!side_valid_) return false;
        val = side_;
        return true;
    }

    bool getShare(size_t index, int& val) const {
        if (!share_valid_ || index >= share_.size()) return false;
        val = share_[index];
        return true;
    }

    bool getShareVector(std::vector<int>& val) const {
        if (!share_valid_) return false;
        val = share_;
        return true;
    }

    bool getValue(int& val) const {
        if (!value_valid_) return false;
        val = value_;
        return true;
    }

    void setType(const std::string& val) {
        type_ = val;
        type_valid_ = true;
    }

    void setIndex(const int& val) {
        index_ = val;
        index_valid_ = true;
    }

    void setOwner(const int& val) {
        owner_ = val;
        owner_valid_ = true;
    }

    void setSide(const int& val) {
        side_ = val;
        side_valid_ = true;
    }

    void setShare(const std::vector<int>& val) {
        share_ = val;
        share_valid_ = true;
    }

    void setValue(const int& val) {
        value_ = val;
        value_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Type")) {
            type_ = j["Type"];
            type_valid_ = true;
        }
        if (j.contains("Index")) {
            index_ = j["Index"];
            index_valid_ = true;
        }
        if (j.contains("Owner")) {
            owner_ = j["Owner"];
            owner_valid_ = true;
        }
        if (j.contains("Side")) {
            side_ = j["Side"];
            side_valid_ = true;
        }
        if (j.contains("Share")) {
            share_ = j["Share"].get<std::vector<int>>();
            share_valid_ = true;
        }
        if (j.contains("Value")) {
            value_ = j["Value"];
            value_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (type_valid_) {
            j["Type"] = type_;
        }
        if (index_valid_) {
            j["Index"] = index_;
        }
        if (owner_valid_) {
            j["Owner"] = owner_;
        }
        if (side_valid_) {
            j["Side"] = side_;
        }
        if (share_valid_) {
            j["Share"] = share_;
        }
        if (value_valid_) {
            j["Value"] = value_;
        }
        return j.dump();
    }
};

class Z6RootJson : public Z6json {
private:
    std::string gameID_;
    bool gameID_valid_;
    Z6GameJson game_;
    bool game_valid_;
    Z6BoardJson board_;
    bool board_valid_;
    std::string createdAt_;
    bool createdAt_valid_;
    std::string status_;
    bool status_valid_;
    Z6ResultJson result_;
    bool result_valid_;

public:
    // Default Constructor
    Z6RootJson() : gameID_valid_(false), game_valid_(false), board_valid_(false), createdAt_valid_(false), status_valid_(false), result_valid_(false) {}

    // Copy Constructor
    Z6RootJson(const Z6RootJson& other) : gameID_(other.gameID_), gameID_valid_(other.gameID_valid_), game_(other.game_), game_valid_(other.game_valid_), board_(other.board_), board_valid_(other.board_valid_), createdAt_(other.createdAt_), createdAt_valid_(other.createdAt_valid_), status_(other.status_), status_valid_(other.status_valid_), result_(other.result_), result_valid_(other.result_valid_) {}

    // Copy Assignment Operator
    Z6RootJson& operator=(const Z6RootJson& other) {
        if (this != &other) {
            gameID_ = other.gameID_;
            gameID_valid_ = other.gameID_valid_;
            game_ = other.game_;
            game_valid_ = other.game_valid_;
            board_ = other.board_;
            board_valid_ = other.board_valid_;
            createdAt_ = other.createdAt_;
            createdAt_valid_ = other.createdAt_valid_;
            status_ = other.status_;
            status_valid_ = other.status_valid_;
            result_ = other.result_;
            result_valid_ = other.result_valid_;
        }
        return *this;
    }

    bool getGameID(std::string& val) const {
        if (!gameID_valid_) return false;
        val = gameID_;
        return true;
    }

    bool getGame(Z6GameJson& val) const {
        if (!game_valid_) return false;
        val = game_;
        return true;
    }

    bool getBoard(Z6BoardJson& val) const {
        if (!board_valid_) return false;
        val = board_;
        return true;
    }

    bool getCreatedAt(std::string& val) const {
        if (!createdAt_valid_) return false;
        val = createdAt_;
        return true;
    }

    bool getStatus(std::string& val) const {
        if (!status_valid_) return false;
        val = status_;
        return true;
    }

    bool getResult(Z6ResultJson& val) const {
        if (!result_valid_) return false;
        val = result_;
        return true;
    }

    void setGameID(const std::string& val) {
        gameID_ = val;
        gameID_valid_ = true;
    }

    void setGame(const Z6GameJson& val) {
        game_ = val;
        game_valid_ = true;
    }

    void setBoard(const Z6BoardJson& val) {
        board_ = val;
        board_valid_ = true;
    }

    void setCreatedAt(const std::string& val) {
        createdAt_ = val;
        createdAt_valid_ = true;
    }

    void setStatus(const std::string& val) {
        status_ = val;
        status_valid_ = true;
    }

    void setResult(const Z6ResultJson& val) {
        result_ = val;
        result_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("GameID")) {
            gameID_ = j["GameID"];
            gameID_valid_ = true;
        }
        if (j.contains("Game")) {
            game_.from_json(j["Game"].dump());
            game_valid_ = true;
        }
        if (j.contains("Board")) {
            board_.from_json(j["Board"].dump());
            board_valid_ = true;
        }
        if (j.contains("CreatedAt")) {
            createdAt_ = j["CreatedAt"];
            createdAt_valid_ = true;
        }
        if (j.contains("Status")) {
            status_ = j["Status"];
            status_valid_ = true;
        }
        if (j.contains("Result")) {
            result_.from_json(j["Result"].dump());
            result_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (gameID_valid_) {
            j["GameID"] = gameID_;
        }
        if (game_valid_) {
            j["Game"] = json::parse(game_.to_json());
        }
        if (board_valid_) {
            j["Board"] = json::parse(board_.to_json());
        }
        if (createdAt_valid_) {
            j["CreatedAt"] = createdAt_;
        }
        if (status_valid_) {
            j["Status"] = status_;
        }
        if (result_valid_) {
            j["Result"] = json::parse(result_.to_json());
        }
        return j.dump();
    }
};

class Z6RulesJson : public Z6json {
private:
    bool sowing_;
    bool sowing_valid_;
    bool capture_;
    bool capture_valid_;
    bool pasu_;
    bool pasu_valid_;
    bool doubleCapture_;
    bool doubleCapture_valid_;
    bool freeCapture_;
    bool freeCapture_valid_;
    bool roundReset_;
    bool roundReset_valid_;
    bool kingsCapture_;
    bool kingsCapture_valid_;
    bool blockedReturn_;
    bool blockedReturn_valid_;
    bool roundDivident_;
    bool roundDivident_valid_;
    bool gameEnded_;
    bool gameEnded_valid_;

public:
    // Default Constructor
    Z6RulesJson() : sowing_(false), sowing_valid_(false), capture_(false), capture_valid_(false), pasu_(false), pasu_valid_(false), doubleCapture_(false), doubleCapture_valid_(false), freeCapture_(false), freeCapture_valid_(false), roundReset_(false), roundReset_valid_(false), kingsCapture_(false), kingsCapture_valid_(false), blockedReturn_(false), blockedReturn_valid_(false), roundDivident_(false), roundDivident_valid_(false), gameEnded_(false), gameEnded_valid_(false) {}

    // Copy Constructor
    Z6RulesJson(const Z6RulesJson& other) : sowing_(other.sowing_), sowing_valid_(other.sowing_valid_), capture_(other.capture_), capture_valid_(other.capture_valid_), pasu_(other.pasu_), pasu_valid_(other.pasu_valid_), doubleCapture_(other.doubleCapture_), doubleCapture_valid_(other.doubleCapture_valid_), freeCapture_(other.freeCapture_), freeCapture_valid_(other.freeCapture_valid_), roundReset_(other.roundReset_), roundReset_valid_(other.roundReset_valid_), kingsCapture_(other.kingsCapture_), kingsCapture_valid_(other.kingsCapture_valid_), blockedReturn_(other.blockedReturn_), blockedReturn_valid_(other.blockedReturn_valid_), roundDivident_(other.roundDivident_), roundDivident_valid_(other.roundDivident_valid_), gameEnded_(other.gameEnded_), gameEnded_valid_(other.gameEnded_valid_) {}

    // Copy Assignment Operator
    Z6RulesJson& operator=(const Z6RulesJson& other) {
        if (this != &other) {
            sowing_ = other.sowing_;
            sowing_valid_ = other.sowing_valid_;
            capture_ = other.capture_;
            capture_valid_ = other.capture_valid_;
            pasu_ = other.pasu_;
            pasu_valid_ = other.pasu_valid_;
            doubleCapture_ = other.doubleCapture_;
            doubleCapture_valid_ = other.doubleCapture_valid_;
            freeCapture_ = other.freeCapture_;
            freeCapture_valid_ = other.freeCapture_valid_;
            roundReset_ = other.roundReset_;
            roundReset_valid_ = other.roundReset_valid_;
            kingsCapture_ = other.kingsCapture_;
            kingsCapture_valid_ = other.kingsCapture_valid_;
            blockedReturn_ = other.blockedReturn_;
            blockedReturn_valid_ = other.blockedReturn_valid_;
            roundDivident_ = other.roundDivident_;
            roundDivident_valid_ = other.roundDivident_valid_;
            gameEnded_ = other.gameEnded_;
            gameEnded_valid_ = other.gameEnded_valid_;
        }
        return *this;
    }

    bool getSowing(bool& val) const {
        if (!sowing_valid_) return false;
        val = sowing_;
        return true;
    }

    bool getCapture(bool& val) const {
        if (!capture_valid_) return false;
        val = capture_;
        return true;
    }

    bool getPasu(bool& val) const {
        if (!pasu_valid_) return false;
        val = pasu_;
        return true;
    }

    bool getDoubleCapture(bool& val) const {
        if (!doubleCapture_valid_) return false;
        val = doubleCapture_;
        return true;
    }

    bool getFreeCapture(bool& val) const {
        if (!freeCapture_valid_) return false;
        val = freeCapture_;
        return true;
    }

    bool getRoundReset(bool& val) const {
        if (!roundReset_valid_) return false;
        val = roundReset_;
        return true;
    }

    bool getKingsCapture(bool& val) const {
        if (!kingsCapture_valid_) return false;
        val = kingsCapture_;
        return true;
    }

    bool getBlockedReturn(bool& val) const {
        if (!blockedReturn_valid_) return false;
        val = blockedReturn_;
        return true;
    }

    bool getRoundDivident(bool& val) const {
        if (!roundDivident_valid_) return false;
        val = roundDivident_;
        return true;
    }

    bool getGameEnded(bool& val) const {
        if (!gameEnded_valid_) return false;
        val = gameEnded_;
        return true;
    }

    void setSowing(const bool& val) {
        sowing_ = val;
        sowing_valid_ = true;
    }

    void setCapture(const bool& val) {
        capture_ = val;
        capture_valid_ = true;
    }

    void setPasu(const bool& val) {
        pasu_ = val;
        pasu_valid_ = true;
    }

    void setDoubleCapture(const bool& val) {
        doubleCapture_ = val;
        doubleCapture_valid_ = true;
    }

    void setFreeCapture(const bool& val) {
        freeCapture_ = val;
        freeCapture_valid_ = true;
    }

    void setRoundReset(const bool& val) {
        roundReset_ = val;
        roundReset_valid_ = true;
    }

    void setKingsCapture(const bool& val) {
        kingsCapture_ = val;
        kingsCapture_valid_ = true;
    }

    void setBlockedReturn(const bool& val) {
        blockedReturn_ = val;
        blockedReturn_valid_ = true;
    }

    void setRoundDivident(const bool& val) {
        roundDivident_ = val;
        roundDivident_valid_ = true;
    }

    void setGameEnded(const bool& val) {
        gameEnded_ = val;
        gameEnded_valid_ = true;
    }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Sowing")) {
            sowing_ = j["Sowing"];
            sowing_valid_ = true;
        }
        if (j.contains("Capture")) {
            capture_ = j["Capture"];
            capture_valid_ = true;
        }
        if (j.contains("Pasu")) {
            pasu_ = j["Pasu"];
            pasu_valid_ = true;
        }
        if (j.contains("DoubleCapture")) {
            doubleCapture_ = j["DoubleCapture"];
            doubleCapture_valid_ = true;
        }
        if (j.contains("FreeCapture")) {
            freeCapture_ = j["FreeCapture"];
            freeCapture_valid_ = true;
        }
        if (j.contains("RoundReset")) {
            roundReset_ = j["RoundReset"];
            roundReset_valid_ = true;
        }
        if (j.contains("KingsCapture")) {
            kingsCapture_ = j["KingsCapture"];
            kingsCapture_valid_ = true;
        }
        if (j.contains("BlockedReturn")) {
            blockedReturn_ = j["BlockedReturn"];
            blockedReturn_valid_ = true;
        }
        if (j.contains("RoundDivident")) {
            roundDivident_ = j["RoundDivident"];
            roundDivident_valid_ = true;
        }
        if (j.contains("GameEnded")) {
            gameEnded_ = j["GameEnded"];
            gameEnded_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (sowing_valid_) {
            j["Sowing"] = sowing_;
        }
        if (capture_valid_) {
            j["Capture"] = capture_;
        }
        if (pasu_valid_) {
            j["Pasu"] = pasu_;
        }
        if (doubleCapture_valid_) {
            j["DoubleCapture"] = doubleCapture_;
        }
        if (freeCapture_valid_) {
            j["FreeCapture"] = freeCapture_;
        }
        if (roundReset_valid_) {
            j["RoundReset"] = roundReset_;
        }
        if (kingsCapture_valid_) {
            j["KingsCapture"] = kingsCapture_;
        }
        if (blockedReturn_valid_) {
            j["BlockedReturn"] = blockedReturn_;
        }
        if (roundDivident_valid_) {
            j["RoundDivident"] = roundDivident_;
        }
        if (gameEnded_valid_) {
            j["GameEnded"] = gameEnded_;
        }
        return j.dump();
    }
};


class Z6jsonFactory {
public:
    static std::unique_ptr<Z6json> createObject(const std::string& type) {
        if (type == "Board" || type == "Z6BoardJson") return std::make_unique<Z6BoardJson>();
        if (type == "Game" || type == "Z6GameJson") return std::make_unique<Z6GameJson>();
        if (type == "MoveHistory" || type == "Z6MoveHistoryJson") return std::make_unique<Z6MoveHistoryJson>();
        if (type == "Move" || type == "Z6MoveJson") return std::make_unique<Z6MoveJson>();
        if (type == "Detail" || type == "Z6DetailJson") return std::make_unique<Z6DetailJson>();
        if (type == "MoveReq" || type == "Z6MoveReqJson") return std::make_unique<Z6MoveReqJson>();
        if (type == "BoardResp" || type == "Z6BoardRespJson") return std::make_unique<Z6BoardRespJson>();
        if (type == "Pit" || type == "Z6PitJson") return std::make_unique<Z6PitJson>();
        if (type == "Root" || type == "Z6RootJson") return std::make_unique<Z6RootJson>();
        if (type == "Rules" || type == "Z6RulesJson") return std::make_unique<Z6RulesJson>();
        return nullptr;
    }
};
