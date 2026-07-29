#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Base Class
class Z6json {
public:
    virtual ~Z6json() = default;
    virtual void from_json(const std::string& json_str) = 0;
    virtual std::string to_json() const = 0;
};

// Pit Class (Schema from Source 8)
class Pit : public Z6json {
private:
    std::string type_;
    bool type_valid_;
    int index_;
    bool index_valid_;
    int owner_;
    bool owner_valid_;
    std::vector<int> share_;
    bool share_valid_;
    int value_;
    bool value_valid_;

public:
    Pit() 
        : type_("NORMAL"), type_valid_(false),
          index_(100), index_valid_(false),
          owner_(10), owner_valid_(false),
          share_valid_(false),
          value_(100), value_valid_(false) {}

    // Getters returning bool and taking a reference parameter
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

    // Setters
    void setType(const std::string& t) { type_ = t; type_valid_ = true; }
    void setIndex(int i) { index_ = i; index_valid_ = true; }
    void setOwner(int o) { owner_ = o; owner_valid_ = true; }
    void setShare(const std::vector<int>& s) { share_ = s; share_valid_ = true; }
    void setValue(int v) { value_ = v; value_valid_ = true; }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Type")) { type_ = j["Type"]; type_valid_ = true; }
        if (j.contains("Index")) { index_ = j["Index"]; index_valid_ = true; }
        if (j.contains("Owner")) { owner_ = j["Owner"]; owner_valid_ = true; }
        if (j.contains("Share")) { share_ = j["Share"].get<std::vector<int>>(); share_valid_ = true; }
        if (j.contains("Value")) { value_ = j["Value"]; value_valid_ = true; }
    }

    std::string to_json() const override {
        json j;
        if (type_valid_) j["Type"] = type_;
        if (index_valid_) j["Index"] = index_;
        if (owner_valid_) j["Owner"] = owner_;
        if (share_valid_) j["Share"] = share_;
        if (value_valid_) j["Value"] = value_;
        return j.dump();
    }
};

// Move Class (Schema from Source 4)
class Move : public Z6json {
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
    Move() 
        : sequence_(0), sequence_valid_(false),
          timestamp_valid_(false),
          player_valid_(false),
          index_(0), index_valid_(false) {}

    // Getters returning bool and using output parameters
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

    // Setters
    void setSequence(int s) { sequence_ = s; sequence_valid_ = true; }
    void setTimestamp(const std::string& t) { timestamp_ = t; timestamp_valid_ = true; }
    void setPlayer(const std::string& p) { player_ = p; player_valid_ = true; }
    void setIndex(int i) { index_ = i; index_valid_ = true; }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Sequence")) { sequence_ = j["Sequence"]; sequence_valid_ = true; }
        if (j.contains("Timestamp")) { timestamp_ = j["Timestamp"]; timestamp_valid_ = true; }
        if (j.contains("Player")) { player_ = j["Player"]; player_valid_ = true; }
        if (j.contains("Index")) { index_ = j["Index"]; index_valid_ = true; }
    }

    std::string to_json() const override {
        json j;
        if (sequence_valid_) j["Sequence"] = sequence_;
        if (timestamp_valid_) j["Timestamp"] = timestamp_;
        if (player_valid_) j["Player"] = player_;
        if (index_valid_) j["Index"] = index_;
        return j.dump();
    }
};

// Board Class (Schema from Source 1)
class Board : public Z6json {
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
    std::vector<Pit> pits_;
    bool pits_valid_;
    int nround_;
    bool nround_valid_;
    std::string visibility_;
    bool visibility_valid_;

public:
    Board() 
        : active_(true), active_valid_(false),
          turn_(0), turn_valid_(false),
          nsides_(2), nsides_valid_(false),
          nseeds_(4), nseeds_valid_(false),
          npits_(7), npits_valid_(false),
          pits_valid_(false),
          nround_(1), nround_valid_(false),
          visibility_("TRANSPARENT"), visibility_valid_(false) {}

    // Getters returning bool and checking boundaries for array
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

    bool getPit(size_t index, Pit& val) const {
        if (!pits_valid_ || index >= pits_.size()) return false;
        val = pits_[index];
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

    // Setters
    void setActive(bool a) { active_ = a; active_valid_ = true; }
    void setTurn(int t) { turn_ = t; turn_valid_ = true; }
    void setNsides(int ns) { nsides_ = ns; nsides_valid_ = true; }
    void setNseeds(int ns) { nseeds_ = ns; nseeds_valid_ = true; }
    void setNpits(int np) { npits_ = np; npits_valid_ = true; }
    void setPits(const std::vector<Pit>& p) { pits_ = p; pits_valid_ = true; }
    void setNround(int nr) { nround_ = nr; nround_valid_ = true; }
    void setVisibility(const std::string& v) { visibility_ = v; visibility_valid_ = true; }

    void from_json(const std::string& json_str) override {
        json j = json::parse(json_str);
        if (j.contains("Active")) { active_ = j["Active"]; active_valid_ = true; }
        if (j.contains("Turn")) { turn_ = j["Turn"]; turn_valid_ = true; }
        if (j.contains("Nsides")) { nsides_ = j["Nsides"]; nsides_valid_ = true; }
        if (j.contains("Nseeds")) { nseeds_ = j["Nseeds"]; nseeds_valid_ = true; }
        if (j.contains("Npits")) { npits_ = j["Npits"]; npits_valid_ = true; }
        if (j.contains("Nround")) { nround_ = j["Nround"]; nround_valid_ = true; }
        if (j.contains("Visibility")) { visibility_ = j["Visibility"]; visibility_valid_ = true; }
        if (j.contains("Pits")) {
            pits_.clear();
            for (const auto& item : j["Pits"]) {
                Pit pit;
                pit.from_json(item.dump());
                pits_.push_back(pit);
            }
            pits_valid_ = true;
        }
    }

    std::string to_json() const override {
        json j;
        if (active_valid_) j["Active"] = active_;
        if (turn_valid_) j["Turn"] = turn_;
        if (nsides_valid_) j["Nsides"] = nsides_;
        if (nseeds_valid_) j["Nseeds"] = nseeds_;
        if (npits_valid_) j["Npits"] = npits_;
        if (nround_valid_) j["Nround"] = nround_;
        if (visibility_valid_) j["Visibility"] = visibility_;
        if (pits_valid_) {
            json pits_array = json::array();
            for (const auto& pit : pits_) {
                pits_array.push_back(json::parse(pit.to_json()));
            }
            j["Pits"] = pits_array;
        }
        return j.dump();
    }
};

// Factory Class
class Z6jsonFactory {
public:
    static std::unique_ptr<Z6json> createObject(const std::string& type) {
        if (type == "Pit") return std::make_unique<Pit>();
        if (type == "Move") return std::make_unique<Move>();
        if (type == "Board") return std::make_unique<Board>();
        return nullptr;
    }
};