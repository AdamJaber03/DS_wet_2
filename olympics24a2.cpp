#include "olympics24a2.h"

olympics_t::olympics_t(): hashTeams(TeamsHashTable()), teamsTree(TeamsTree())
{
}

olympics_t::~olympics_t()
{
    for(int i = 0; i < hashTeams.getMaxSize(); i++){
        pair<int, Team*>* tempList = hashTeams.getTeams()[i].getInorder();
        for (int j = 0; j < hashTeams.getTeams()[i].getSize(); ++j) {
            delete tempList[j].getP2();
        }
        delete[] tempList;
    }
}


StatusType olympics_t::add_team(int teamId)
{
    if(teamId <= 0) return StatusType::INVALID_INPUT;
    if (hashTeams.find(teamId)) return StatusType::FAILURE;
    Team* team = new Team(teamId);
    StatusType status = hashTeams.insert(teamId, team);
    if(status != StatusType::SUCCESS){
        delete team;
    }
	return status;
}

StatusType olympics_t::remove_team(int teamId)
{
    if (teamId <= 0) return StatusType::INVALID_INPUT;
    StatusType status;
    Team* team = hashTeams.find(teamId);
    if (!team) return StatusType::FAILURE;

    pair<int, int> key(team->getStrength(), teamId);
    status = hashTeams.remove(teamId);
    if (status!= StatusType::SUCCESS) return status;
    if (team->getSize()){
        status = teamsTree.remove(key);
        if (status!= StatusType::SUCCESS) return status;
    }
    delete team;
	return StatusType::SUCCESS;
}

StatusType olympics_t::add_player(int teamId, int playerStrength)
{
    if (teamId <= 0 || playerStrength <= 0) return StatusType::INVALID_INPUT;
    Team* team = hashTeams.find(teamId);
    if (!team) return StatusType::FAILURE;
    StatusType status;
    pair<int, int> oldKey(team->getStrength(), teamId);
    status = team->insertContestant(playerStrength);
    if (status!= StatusType::SUCCESS) return status;
    pair<int, int> newKey(team->getStrength(), teamId);
    if (!(team->getSize() - 1)){
        status = teamsTree.insert(newKey, team, team->getMedals());
        return status;
    }
    int curMedals = teamsTree.NumWins(oldKey);
    status = teamsTree.remove(oldKey);
    if (status != StatusType::SUCCESS) return status;
    status = teamsTree.insert(newKey, team, curMedals);
    if (status != StatusType::SUCCESS) return status;
	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_newest_player(int teamId)
{
    if(teamId <= 0) return StatusType::INVALID_INPUT;
    Team * team = hashTeams.find(teamId);
    if(!team) return StatusType::FAILURE;
    pair<int, int> oldKey(team->getStrength(), teamId);
    StatusType status = team->removeContestant();
    pair<int, int> newKey(team->getStrength(), teamId);
    if(status != StatusType::SUCCESS) return status;
    int medals = teamsTree.NumWins(oldKey);
    teamsTree.remove(oldKey);
    team->setMedals(medals);
    if (team->getSize()){
        teamsTree.insert(newKey, team, medals);
    }
	return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) return output_t<int>(StatusType::INVALID_INPUT);
    Team * team1 = hashTeams.find(teamId1);
    Team * team2 = hashTeams.find(teamId2);
    if(!team1 || !team2 || !team1->getSize() || !team2->getSize()) return output_t<int>(StatusType::FAILURE);
    return teamsTree.play_match(pair<int, int>(team1->getStrength(), teamId1), pair<int, int>(team2->getStrength(), teamId2));
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    if (teamId <= 0) return StatusType::INVALID_INPUT;
    Team* team = hashTeams.find(teamId);
    if (!team) return StatusType::FAILURE;
    if (!team->getSize()){
        return team->getMedals();
    }
    pair<int, int> key(team->getStrength(), teamId);
    return teamsTree.NumWins(key);
}

output_t<int> olympics_t::get_highest_ranked_team()
{
    if (!hashTeams.getSize()) return -1;
    return teamsTree.getMaxRank();
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) return StatusType::INVALID_INPUT;
    Team * team1 = hashTeams.find(teamId1);
    Team * team2 = hashTeams.find(teamId2);
    if(!team1 || !team2) return StatusType::FAILURE;
    pair<int, int> team2Key(team2->getStrength(), teamId2), team1oldKey(team1->getStrength(), teamId1);
    hashTeams.remove(teamId2);
    if(team2->getSize() == 0){
        delete team2;
        return StatusType::SUCCESS;
    }
    teamsTree.remove(team2Key);
    if (team1->getSize()){
        team1->setMedals(teamsTree.NumWins(team1oldKey));
        teamsTree.remove(team1oldKey);
    }
    StatusType status = team1->unite(*team2);
    pair<int, int> team1newKey(team1->getStrength(), teamId1);
    if (team1->getSize()){
        teamsTree.insert(team1newKey, team1, team1->getMedals());
    }
    if (status != StatusType::SUCCESS) return status;
    delete team2;
    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    // TODO: check failure
    if (lowPower <= 0 || highPower <= 0 || lowPower >= highPower) return StatusType::INVALID_INPUT;
    return teamsTree.playTournament(lowPower, highPower);
}
