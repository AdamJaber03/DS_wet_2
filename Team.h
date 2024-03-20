//
// Created by JaberA on 17/03/2024.
//

#ifndef DS_WET_2_TEAM_H
#define DS_WET_2_TEAM_H

//temp
class Team {
private:
    int id;
    int power;

public:
    // Constructor
    Team(int teamId, int teamPower) : id(teamId), power(teamPower) {}

    // Getter for ID
    int getId() const {
        return id;
    }

    // Setter for ID
    void setId(int teamId) {
        id = teamId;
    }

    // Getter for Power
    int getPower() const {
        return power;
    }

    // Setter for Power
    void setPower(int teamPower) {
        power = teamPower;
    }
};


#endif //DS_WET_2_TEAM_H
