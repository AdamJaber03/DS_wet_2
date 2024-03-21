#include "olympics24a2.h"

olympics_t::olympics_t()
{
	// TODO: Your code goes here
}

olympics_t::~olympics_t()
{
	// TODO: Your code goes here
}


StatusType olympics_t::add_team(int teamId)
{
    //TODO -check invalid
    if(teamId <= 0) return StatusType::INVALID_INPUT;
    Team* team = new Team(teamId);
    StatusType status = hashTeams.insert(teamId, team);
    if(status != StatusType::SUCCESS) return status;
    //TODO : insert to tree
	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_team(int teamId)
{
	// TODO: check for invalid and failure
    StatusType status;
    Team* team = hashTeams.find(teamId);
    pair<int, int> key(team->getStrength(), teamId);
    status = hashTeams.remove(teamId);
    if (status!= StatusType::SUCCESS) return status;
    status = teamsTree.remove(key);
    if (status!= StatusType::SUCCESS) return status;
    delete team;
	return StatusType::SUCCESS;
}

StatusType olympics_t::add_player(int teamId, int playerStrength)
{
    //TODO - check for invalid input and failure
    Team* team = hashTeams.find(teamId);
    StatusType status;
    pair<int, int> oldKey(team->getStrength(), teamId);
    status = team->insertContestant(playerStrength);
    if (status!= StatusType::SUCCESS) return status;
    pair<int, int> newKey(team->getStrength(), teamId);
    if (!(team->getSize() - 1)){
        status = teamsTree.insert(newKey, team);
        if (status != StatusType::SUCCESS) return status;
        return StatusType::SUCCESS;
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
    StatusType status = team->removeContestant();
    if(status != StatusType::SUCCESS) return status;
    if(!team->getSize()) teamsTree.remove(*(new pair<int, int>(0, teamId)));
	return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) return output_t<int>(StatusType::INVALID_INPUT);
    Team * team1 = hashTeams.find(teamId1);
    Team * team2 = hashTeams.find(teamId2);
    if(!team1 || !team2) return output_t<int>(StatusType::FAILURE);
    return teamsTree.play_match(*(new pair<int, int>(team1->getStrength(), teamId1)), *(new pair<int, int>(team2->getStrength(), teamId2)));
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    // TODO: failure
    pair<int, int> key(hashTeams.find(teamId)->getStrength(), teamId);
    return teamsTree.NumWins(key);
}

output_t<int> olympics_t::get_highest_ranked_team()
{
    //TODO - check invalid
    return teamsTree.getMaxRank();
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) return StatusType::INVALID_INPUT;
    Team * team1 = hashTeams.find(teamId1);
    Team * team2 = hashTeams.find(teamId2);
    if(!team1 || !team2) return StatusType::FAILURE;
    if(team2->getSize() == 0){
        hashTeams.remove(teamId2);
        return StatusType::SUCCESS;
        //TODO: check if a team with strength 0 is in tree
    }
    return team1->unite(*team2);
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    // TODO: check failure
    return teamsTree.playTournament(lowPower, highPower);
}
