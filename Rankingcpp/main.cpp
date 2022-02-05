#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <tgmath.h>
#include <cmath>
#include <vector>
#include <sstream>
#include <ctime>
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include "Calc.h"
#include "Table.h"
#include "Analysis.h"

using namespace std;

class Club;

class Coach {
public:
    int id = 0;
    string name = "Coach";
    int team = 0;
    float value = 2000;
    float matchValue = 2000;
};

class Player {
public:
    int id;
    string name;
    int team;
    float value[4];
    float defense, offense, technical;
    float averagePos;
    int gamesPlayed;
    Player(int ID, string Name, int Team, float Value[4], int GamesPlayed, float AveragePos) {
        id = ID;
        name = Name;
        team = Team;
        for (int i = 0; i < 4; i++) {
            value[i] = Value[i];
        }
        gamesPlayed = GamesPlayed;
        averagePos = AveragePos;
    }
    void ChangeValue(float amount, int position) {
        if (position == 0) {
            value[0] += amount;
            for (int i = 0; i < 3; i++) {
                value[i + 1] *= 0.9f;
            }
        }
        else if (position == 1) {
            value[0] *= 0.9f;
            value[1] += amount;
            value[2] *= 0.9998f;
            value[2] += 0.5f * amount;
            value[3] *= 0.9995f;
            value[3] += 0.2f * amount;
        }
        else if (position == 2) {
            value[0] *= 0.9f;
            value[1] *= 0.9998f;
            value[1] += 0.5f * amount;
            value[2] += amount;
            value[3] *= 0.9998f;
            value[3] += 0.5f * amount;
        }
        else if (position == 3) {
            value[0] *= 0.9f;
            value[1] *= 0.9995f;
            value[1] += 0.2f * amount;
            value[2] *= 0.9998f;
            value[2] += 0.5f * amount;
            value[3] += amount;

        }
    }

public: 
    Player copy() {
        Player p = Player(id, name, team, value, gamesPlayed, averagePos);
        return p;
    }
};

vector<Player> freeAgents;
vector<Coach> CfreeAgents;
void DisplayTableFromPersons(vector<Player> players, int amount, string title, bool write = false);
void DisplayMenu();
void writeData();
void Editor();
void Statistics();
void sortPlayers();
vector<float> allPlayerValues(int position, bool BundesligaOnly);
void gradient(vector<float> expected, vector<float> actual, float a, float k, float bias, vector<bool> use);
float error(vector<float> probs, vector<float> actual, float a, float k, float bias, vector<bool> use);
void optimizeFormula(int iterations);
string positions[4] = { "Torhueter", "Verteidiger", "Mittelfeldspieler", "Stuermer" };
//clubs
vector<Club> clubs;


class Club {
public:
    int id = 0;
    string name = "Club";
    vector<Player> players = {};
    Coach Trainer = Coach();
    vector<Player> lineup[4] = {};
    float previousValues[2] = {};
    int gamesPlayed = 0;
    float averageFormation[4] = {};
    float homeValue = 2000;
    float homeAdv = 1;
    int league = 0;

    void AddPlayer(string name) {
        vector<float> playerValueVector;
        for (int i = 0; i < players.size(); i++) {
            playerValueVector.push_back(players[i].value[Calc::round(players[i].averagePos)]);
        }
        float val = Calc::getAveragefromHeader(playerValueVector, 18);
        float values[4] = { val,val,val,val };
        AddPlayer(name, values);
    }

    void AddPlayer(string Name, float value[4]) {
        Player p(players.size(), Name, id, value, 0, 0);
        int found = 0;
        int in;
        vector<Player> foundPlayers;
        vector<Player> foundPlayersInAll;
        vector<int> foundIDs;
        for (int i = 0; i < freeAgents.size(); i++) {
            if (Name == freeAgents[i].name) {
                foundPlayers.push_back(freeAgents[i]);
                foundIDs.push_back(i);
                found++;
            }
        }
        if (found > 0) {
            cout << "Es wurden " << found << " Vereinslose Spieler mit diesem Namen gefunden. Ansehen?\n";
            cout << "1:Ja, 2:Nein\n";
            cin >> in;
            if (in == 1) {
                for (int i = 0; i < found; i++) {
                    Player tmp = foundPlayers[i];
                    cout << "Spieler " << i << ": " << tmp.name << ", zuletzt von " << clubs[tmp.team].name << ", Wert " << tmp.value[Calc::round(tmp.averagePos)] << ", " << tmp.gamesPlayed << " Einsaetze, " << positions[Calc::round(tmp.averagePos)] << endl;
                    cout << "Diesen Spieler ubernehmen?\n1:Ja, 2:Nein\n";
                    cin >> in;
                    if (in == 1) {
                        foundPlayers[i].id = players.size();
                        players.push_back(foundPlayers[i]);
                        freeAgents.erase(freeAgents.begin() + foundIDs[i]);
                        goto next;
                    }
                }
            }
        }
        found = 0;
        for (int i = 0; i < clubs.size(); i++) {
            for (int j = 0; j < clubs[i].players.size(); j++) {
                if (clubs[i].players[j].name == Name) {
                    foundPlayersInAll.push_back(clubs[i].players[j]);
                    found++;
                }
            }
        }
        if (found > 0) {
            cout << "Es wurden " << found << " gleichnamige Spieler bei anderen Vereinen gefunden. Ansehen?\n";
            cout << "1:Ja, 2:Nein\n";
            cin >> in;
            for (int i = 0; i < found; i++) {
                Player tmp = foundPlayersInAll[i];
                cout << "Spieler " << i << ": " << tmp.name << " von " << clubs[tmp.team].name << ", Wert " << tmp.value[Calc::round(tmp.averagePos)] << ", " << tmp.gamesPlayed << " Einsaetze, " << positions[Calc::round(tmp.averagePos)] << endl;
                cout << "Diesen Spieler ubernehmen?\n1:Ja, 2:Nein\n";
                cin >> in;
                if (in == 1) {
                    Club* c = &clubs[tmp.team];
                    Player* p = &c->players[tmp.id];
                    cout << p->name << " wird zu " << this->name << " hinzugefuegt. Mit '1' bestaetigen, mit '2' abbrechen." << endl;
                    cin >> in;
                    if (in == 1) {
                        AddPlayer(*p);
                        c->iRemovePlayer(p->id, true);
                        cout << p->name << " von " << c->name << " zu " << this->name << " transferiert" << endl;
                        PrintPlayers();
                        writeData();
                        goto next;
                    }
                    else {
                        cout << "Aufgabe abgebrochen, keine Aenderungen vorgenommen" << endl;
                    }
                }
            }
        }



        players.push_back(p);

    next:;
    }

    void AddPlayer(Player player) {
        player.team = id;
        player.id = players.size();
        players.push_back(player);
    }

    void iRemovePlayer(int ID,bool transfer = false) {
        if (!transfer) {
            freeAgents.push_back(players[ID]);
        }
        players.erase(players.begin() + ID);
    }

    void nRemovePlayer(string Name) {

    }

    void NewCoach(string Name) {
        CfreeAgents.push_back(Trainer);
        Coach c;
        c.team = id;
        c.name = Name;
        c.value = 2000;
        int found = 0, in;
        vector<float> foundValues;
        vector<string> foundNames;
        vector<Coach> foundCoaches;
        vector<int> foundIDs;
        for (int i = 0; i < CfreeAgents.size(); i++) {
            if (CfreeAgents[i].name == Name) {
                found++;
                foundValues.push_back(CfreeAgents[i].value);
                foundNames.push_back(CfreeAgents[i].name);
                foundCoaches.push_back(CfreeAgents[i]);
                foundIDs.push_back(i);
            }
        }
        if (found > 0) {
            cout << "Es wurden " << found << " vereinslose Trainer mit diesem Namen gefunden. Ansehen?\n";
            cout << "1:Ja, 2:Nein\n";
            cin >> in;
            if (in == 1) {
                for (int i = 0; i < found; i++) {
                    Coach tmp = foundCoaches[i];
                    cout << "Trainer " << i << ": " << tmp.name << " " << tmp.team << " " << tmp.value << endl;
                    cout << "Diesen Trainer ubernehmen?\n1:Ja, 2:Nein\n";
                    cin >> in;
                    if (in == 1) {
                        Trainer = foundCoaches[i];
                        Trainer.team = id;
                        CfreeAgents.erase(CfreeAgents.begin() + foundIDs[i]);
                        goto next;
                    }
                }
            }

        }
        Trainer = c;
    next:;
    }

    void NewCoach(Coach c,bool transfer = false) {
        if (!transfer) {
            CfreeAgents.push_back(Trainer);
        }
        cout << "Alter Trainer: " << Trainer.name << endl;
        c.team = id;
        Trainer = c;
        cout << "Neuer Trainer: " << Trainer.name  << " mit " << Trainer.value << " Punkten"<< endl;
    }

    void AssignEmptyCoach() {
        Coach c;
        c.name = "";
        c.team = id;
        c.value = 2000;
        Trainer = c;
    }

    void PrintPlayers() {
        vector<string> pNames, PPositions;
        vector<float> pValues;
        vector<int> pIndices;
        for (int i = 0; i < players.size(); i++) {
            Player tmp = players[i];
            pNames.push_back(tmp.name);
            if (tmp.gamesPlayed > 0){
                int ccc = Calc::round(tmp.averagePos);
                PPositions.push_back(positions[ccc]);

            }
            else {
                PPositions.push_back("Unbekannt");
            }
            pValues.push_back(tmp.value[Calc::round(tmp.averagePos)]);
            pIndices.push_back(tmp.id);
        }
        string table = Table::AsTable(pIndices, pNames, pValues, PPositions, {"ID", "Name", "Wert", "Position"}, "Spieler fuer " + name + ":");
        cout << table;
    }

    void SetHomeValue(float value) {
        homeValue = value;
        homeAdv = (homeValue / 2000.0f - 1.0f) / 10.0f + 1.0035f;
    }

    float AverageValue() {
        float sum = 0;
        for (int i = 0; i < players.size(); i++) {
            sum += players[i].value[Calc::round(players[i].averagePos)];
        }
        return sum / (float)players.size();
    }
};

class League {
public:
    vector<Club> teams;
    string name;
    int id;

    float AveragePlayerValue(){
        vector<float> values;
        float result;
        int count = 0;
        for (int i = 0; i < teams.size(); i++) {
            for (int j = 0; j < teams[i].players.size(); j++) {
                values.push_back(teams[i].players[j].value[Calc::round(teams[i].players[j].averagePos)]);
                count++;
            }
        }
        result = Calc::getAveragefromHeader(values, count);
        return result;
    }

    void DisplayLeagueMembers() {
        vector<string> teamNames;
        vector<float> teamValues;
        for (int i = 0; i < teams.size(); i++) {
            teamNames.push_back(teams[i].name);
            teamValues.push_back(teams[i].AverageValue());
        }
        cout << Table::AsTable(teamNames, teamValues, name);
    }
};

class Match {
public:
    Club* home;
    Club* away;
    int hg = 0, ag = 0;
    int result[2] = {0,0};
    void setHomeGoals(int g) {
        hg = g;
        result[0] = hg;
        result[1] = ag;
    }

    void setAwayGoals(int g) {
        ag = g;
        result[0] = hg;
        result[1] = ag;
    }

    void selectLineup(int activeTeam) {
        Club* team = activeTeam == 0 ? home : away;
        team->PrintPlayers();
        int tempPos = 0;
        int playersSelectedSoFar = 0;
        int choice;
        bool error = false;
    nextpos:
        cout << positions[tempPos] << " fur " << team->name << " auswahlen: (\"99\" eingeben um zu nachster Position zu wechseln)" << endl;
        while (playersSelectedSoFar < 11) {
            cin >> choice;
            if (!cin) {
                cout << "Fehler: Unzulaessige eingabe. Beende Programm." << endl;
                error = true;
                goto End;
            }
            if (choice == 99) {
                tempPos += 1;
                goto nextpos;
            }
            team->lineup[tempPos].push_back(team->players[choice]);
            playersSelectedSoFar += 1;
        }
    End:;
        if (error) {
            system("pause");
            terminate();
        }

    }

    Match(Club* h, Club* a) {
        home = h;
        away = a;
    }

    Match() {

    }
};
Match currentMatch;


//new system:
const float position_bonus[4][3] = { {10,-5,-5},{10,-4,-6},{-2,4,7},{-10,3,7} };
const float shots[4] = { 0, 0.46f, 1.396f, 1.79f };
const float conversion_rate = 0.15f;
const int possesions = 100;
const float wonPossessions[4] = { 0, 57, 35, 8 };
const float lostPossessions[4] = { 0, 37.4f, 40.3f, 22.3f };
const float changeProportion_concededGoals[4] = { 1,1,0.5f,0.25f };
const float changeProportion_possession[4] = { 0,0.93f,1,0.55f };
const float changeProportion_scoredGoals[4] = { 0, 0.25f,0.9f,1 };
float POSSESSION[2];
float POSSESSION_TOTAL[2];
float grand_Mean;
float teamValues_def[2][4], teamValues_tech[2][4], teamValues_att[2][4];
float pensPerGame = 0.137f;

//structure
string sepLine = "\n-----------------------\n";
string breakLine = "  \n<------------------>\n";
string teamNames[18] = { "FC Bayern Munchen", "RB Leipzig", "VfL Wolfsburg", "Eintracht Frankfurt", "Bayer 04 Leverkusen", "Borussia Dortmund", "1.FC Union Berlin", "SC Freiburg", "VfB Stuttgart", "Borussia Monchengladbach", "TSG 1899 Hoffenheim", "SV Werder Bremen", "FC Augsburg", "1.FC Koln", "Hertha BSC Berlin", "Arminia Bielefeld", "1.FSV Mainz 05", "FC Schalke 04" };
bool correctVersion = true;
string newBlock = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";



//leagues
vector<League> leagues;

//matches
Club activeClubs[2];
int goals[2];
int Gdif[2];

//calculations
float temp1, temp2;
float winProb[2][4], probValue[2][4], defValue[2], normProb[3], normalProbPrevious[3], goalProbability[2][5];
float xG[2], xGdif[2], sd[2];
float matchValuesTeamPosition[2][4];
float changeTeamPosition[2][4];
float previousValues[2][2];
int y[11];
float positionCountPowerBasis = 1.0015f;
vector<float> v1, v2, vComb;
float NetPoints;
bool neutralVenue_game = false;


//settings
float changeConstant = 20.0f;
float homeAdv = 0.004f;
float homeFac = 1.0f + homeAdv;
float awayFac = 1.0f - homeAdv;
float maxxG = 3.21f;//19/20 durchschnittstore
float aExponentialFactor = 0.6f;
float kExponentialFactor = 2.0f;
float biasExponential = 0;
int version = 3;
HANDLE hConsole;


//analytics
vector<float> Analysis_Probs[3];
vector<int> predictedResult;
vector<int> result;
vector<float> Analysis_xG[2];
vector<int> Analysis_goals[2];
vector<int> Analysis_teams[2];
float Ranalysis_Values[2][4];

//optimization
float aGrad, kGrad, bGrad;

void writeData() {
    //main database; player data
    sortPlayers();
    cout << "WRITING TO DATABASE..." << endl;
    ofstream data("Data/data.txt");
    for (int i = 0; i < clubs.size(); i++) {
        for (int j = 0; j < clubs[i].players.size(); j++) {
            Player tmp = clubs[i].players[j];
            data << j << "," << tmp.name << "," << tmp.team << "," << tmp.value[0] << "," << tmp.value[1] << "," << tmp.value[2] << "," << tmp.value[3] << "," << tmp.gamesPlayed << "," << tmp.averagePos << endl;
        }
    }
    data.close();
    //club database
    ofstream dat("Data/clubData.txt");
    for (int i = 0; i < clubs.size(); i++) {
        dat << clubs[i].previousValues[0] << "," << clubs[i].previousValues[1] << "," << clubs[i].name << "," << clubs[i].id << "," << clubs[i].Trainer.name << "," << clubs[i].Trainer.value << "," << clubs[i].gamesPlayed << "," << clubs[i].averageFormation[0] << "," << clubs[i].averageFormation[1] << "," << clubs[i].averageFormation[2] << "," << clubs[i].averageFormation[3] << "," << clubs[i].homeValue << "," << clubs[i].league << endl;
    }
    dat.close();
    //config
    ofstream cfg("Data/config.txt");
    cfg << changeConstant << endl;
    cfg << homeAdv << endl;
    cfg << maxxG << endl;
    cfg << version << endl;
    cfg << positionCountPowerBasis << endl;
    cfg << aExponentialFactor << endl;
    cfg << kExponentialFactor << endl;
    cfg << biasExponential << endl;
    cfg.close();

    //Free Agents
    ofstream fas("Data/FAplayers.txt");
    for (int i = 0; i < freeAgents.size(); i++) {
        Player tmp = freeAgents[i];
        fas << tmp.id << "," << tmp.name << "," << tmp.team << "," << tmp.value[0] << "," << tmp.value[1] << "," << tmp.value[2] << "," << tmp.value[3] << "," << tmp.gamesPlayed << "," << tmp.averagePos << endl;

    }
    fas.close();
    ofstream facs("Data/FAcoaches.txt");
    for (int i = 0; i < CfreeAgents.size(); i++) {
        Coach tmp = CfreeAgents[i];
        facs << tmp.id << "," << tmp.name << "," << tmp.team << "," << tmp.value << endl;
    }
    facs.close();
    cout << "!WRITING COMPLETE!" << endl;
}

void readData() {
    string x;
    string con[8];
    //config

    ifstream cfg("Data/config.txt");
    string disc, chgC, hAdv, maxXG, vers;
    cout << "Konfigurationsdatei geoeffnet\n";
    int i = 0;
    while (cfg >> x) {
        con[i] = x;
        i++;
    }
    changeConstant = stof(con[0]);
    homeAdv = stof(con[1]);
    maxxG = stof(con[2]);
    if (Calc::round(stof(con[3])) != version) {
        correctVersion = false;
    }
    else {
        correctVersion = true;
    }
    cout << "Fussballrangliste Version " << version << endl;
    positionCountPowerBasis = stof(con[4]);
    aExponentialFactor = stof(con[5]);
    kExponentialFactor = stof(con[6]);
    biasExponential = stof(con[7]);
    
    cfg.close();
    cout << "Konfigurationsdatei gelesen und geschlossen.\n";

    //club database
    if (clubs.size() > 0) {
        clubs.clear();
    }
    ifstream prev("Data/clubData.txt");
    if (!prev.good()) {
        ofstream prevv("Data/clubData.txt");
        prevv.close();
    }
    cout << "Vereinsdatenbank geoeffnet\n";
    string val1, val2, nm, idd, tnm, tval, tgp, tav[4], homeVal, league;
    while (getline(prev, x)) {
        stringstream ss(x);
        getline(ss, val1, ',');
        getline(ss, val2, ',');
        getline(ss, nm, ',');
        getline(ss, idd, ',');
        getline(ss, tnm, ',');
        getline(ss, tval, ',');
        getline(ss, tgp, ',');
        getline(ss, tav[0], ',');
        getline(ss, tav[1], ',');
        getline(ss, tav[2], ',');
        getline(ss, tav[3], ',');
        getline(ss, homeVal, ',');
        getline(ss, league, ',');


        Club temp;
        temp.previousValues[0] = stof(val1);
        temp.previousValues[1] = stof(val2);
        temp.name = nm;
        temp.id = stoi(idd);
        temp.Trainer.name = tnm;
        temp.Trainer.team = stoi(idd);
        temp.Trainer.value = stof(tval);
        temp.gamesPlayed = stoi(tgp);
        temp.SetHomeValue(stof(homeVal));

        for (int i = 0; i < 4; i++) {
            temp.averageFormation[i] = stof(tav[i]);
        }
        temp.league = stoi(league);

        clubs.push_back(temp);

    }
    prev.close();
    cout << "Vereinsdatenbank geschlossen, " << clubs.size() << " Vereine geladen.\n";

    //main database
    for (int i = 0; i < clubs.size(); i++) {
        clubs[i].players.clear();
    }
    ifstream data("Data/data.txt");
    if (!data.good()) {
        ofstream dataa("Data/data.txt");
        dataa.close();
    }
    cout << "Hauptdatenbank geoeffnet\n";
    int plsc = 0;
    float VLS[4];
    string id, nam, tm, val[4], gp, avgp;
    while (getline(data, x)) {
        stringstream ss(x);
        getline(ss, id, ',');
        getline(ss, nam, ',');
        getline(ss, tm, ',');
        if(correctVersion) {
            getline(ss, val[0], ',');
            getline(ss, val[1], ',');
            getline(ss, val[2], ',');
            getline(ss, val[3], ',');
            for (int i = 0; i < 4; i++) {
                VLS[i] = stof(val[i]);
            }
            getline(ss, gp, ',');
            getline(ss, avgp, ',');
            Player tmp(clubs[stoi(tm)].players.size(), nam, stoi(tm), VLS, stoi(gp), stof(avgp));
            clubs[stoi(tm)].players.push_back(tmp);
        
        }
        else {
            getline(ss, val[0], ',');
            getline(ss, gp, ',');
            getline(ss, avgp, ',');
            for (int i = 0; i < 4; i++) {
                VLS[i] = stof(val[0]);
            }
            Player tmp(clubs[stoi(tm)].players.size(), nam, stoi(tm), VLS, stoi(gp), stof(avgp));
            clubs[stoi(tm)].players.push_back(tmp);

        }
        plsc++;
    }
    data.close();
    
    
    cout << "Hauptdatenbank geschlossen, " << plsc << " Spieler geladen.\n";

    //prepare table file
    ifstream tab("Data/Tabellen.txt");
    if (!tab.good()) {
        ofstream tabb("Data/Tabellen.txt");
        tabb.close();
    }
    tab.close();
    
    //analytics file
    ifstream analytics("Data/analytics.txt");
    if (!analytics.good()) {
        ofstream analyticss("Data/analytics.txt");
        analyticss.close();
    }
    string v1, v2, v3, v4, v5, v6, v7, v8, v9;
    cout << "Analysedatenbank geoffnet" << endl;
    while (getline(analytics, x)) {
        stringstream ss(x);
        getline(ss, v1, ',');
        getline(ss, v2, ',');
        getline(ss, v3, ',');
        getline(ss, v4, ',');
        getline(ss, v5, ',');
        getline(ss, v6, ',');
        getline(ss, v7, ',');
        getline(ss, v8, ',');
        getline(ss, v9, ',');
        Analysis_teams[0].push_back(stoi(v1));
        Analysis_teams[1].push_back(stoi(v2));
        Analysis_Probs[0].push_back(stof(v5));
        Analysis_Probs[1].push_back(stof(v6));
        Analysis_Probs[2].push_back(stof(v7));
        Analysis_xG[0].push_back(stof(v3));
        Analysis_xG[1].push_back(stof(v4));
        Analysis_goals[0].push_back(stoi(v8));
        Analysis_goals[1].push_back(stoi(v9));
        if (stoi(v3) > stoi(v4)) {
            predictedResult.push_back(0);
        }
        else if (stoi(v3) < stoi(v4)) {
            predictedResult.push_back(2);
        }
        else if (stoi(v3) == stoi(v4)) {
            predictedResult.push_back(1);
        }

        if (stoi(v8) > stoi(v9)) {
            result.push_back(0);
        }
        else if (stoi(v8) < stoi(v9)) {
            result.push_back(2);
        }
        else if (stoi(v8) == stoi(v9)) {
            result.push_back(1);
        }
    }
    cout << "Analysedatenbank geschlossen, " << Analysis_goals[0].size() << " Spiele geladen." << endl;
    analytics.close();

    //Free Agents
    freeAgents.clear();

    ifstream fas("Data/FAplayers.txt");
    if (!fas.good()) {
        ofstream fass("Data/FAplayers.txt");
        fass.close();
    }
    cout << "Vereinslosendatenbank geoeffnet\n";
    int kk = 0;
    while (getline(fas, x)) {
        stringstream ss(x);
        getline(ss, id, ',');
        getline(ss, nam, ',');
        getline(ss, tm, ',');
        if (correctVersion) {
            getline(ss, val[0], ',');
            getline(ss, val[1], ',');
            getline(ss, val[2], ',');
            getline(ss, val[3], ',');
            for (int i = 0; i < 4; i++) {
                VLS[i] = stof(val[i]);
            }
            getline(ss, gp, ',');
            getline(ss, avgp, ',');
            Player tmp(clubs[stoi(tm)].players.size(), nam, stoi(tm), VLS, stoi(gp), stof(avgp));
            freeAgents.push_back(tmp);

        }
        else {
            getline(ss, val[0], ',');
            getline(ss, gp, ',');
            getline(ss, avgp, ',');
            for (int i = 0; i < 4; i++) {
                VLS[i] = stof(val[0]);
            }
            Player tmp(clubs[stoi(tm)].players.size(), nam, stoi(tm), VLS, stoi(gp), stof(avgp));
            freeAgents.push_back(tmp);

        }
        kk++;
    }
    data.close();



    CfreeAgents.clear();
    ifstream facs("Data/FAcoaches.txt");
    if (!facs.good()) {
        ofstream facss("Data/FAcoaches.txt");
        facss.close();
    }

    int cc = 0;
    string vls;
    while (getline(facs, x)) {
        stringstream ss(x);
        getline(ss, id, ',');
        getline(ss, nam, ',');
        getline(ss, tm, ',');
        getline(ss, vls, ',');
        Coach tmp;
        tmp.id = CfreeAgents.size();
        tmp.name = nam;
        tmp.team = stoi(tm);
        tmp.value = stof(vls);
        CfreeAgents.push_back(tmp);
        cc++;
    }
    fas.close();
    cout << "Vereinslosendatenbanken geschlossen, " << kk << " Spieler und " << cc << " Trainer geladen.\n";
}

Match newMatch() {
    Match match;
    cout << "Teams nach Index in der untenstehenden Auflistung auswaehlen:" << endl;
    int choice, choice2;
    bool error = false;
    for (int i = 0; i < clubs.size(); i++) {
        cout << i << ": " << clubs[i].name << endl;
    }
    cout << "Team 1 auswahlen: \n";
    cin >> choice;
    if (!cin) {
        cout << "Fehler: Unzulaessige eingabe. Beende Programm." << endl;
        error = true;
        goto End;

    }
    activeClubs[0] = clubs[choice];
    
    cout << "Team 2 auswahlen: \n";
    cin >> choice2;
    if (!cin) {
        cout << "Fehler: Unzulaessige eingabe. Beende Programm." << endl;
        error = true;
        goto End;

    }
    activeClubs[1] = clubs[choice2];
    match = Match(&clubs[choice], &clubs[choice2]);

    cout << sepLine << "Begegnung:" << activeClubs[0].name << " gegen " << activeClubs[1].name << sepLine;
End:;
    if (error) {
        system("pause");
        terminate();
    }
    return match;
}

/*
void calculateDefPositionValues(Club team1, Club team2) {
    Club tms[2] = { team1,team2 };
    float sum = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            sum = 0;
            for (int p = 0; p < tms[i].lineup[j].size(); p++) {
                sum += tms[i].lineup[j][p].defense;
            }

            teamValues_def[i][j] = sum / (float)tms[i].lineup[j].size() + position_bonus[j][0];

            if (j == 1) {
                teamValues_def[i][j] *= tms[i].lineup[j].size() / (float)(tms[i].lineup[j].size() + 1.0f);
                teamValues_def[i][j] += (1.0f / (float)(tms[i].lineup[j].size() + 1.0f)) * tms[i].lineup[0][0].defense;
            }

            teamValues_def[i][j] *= powf(positionCountPowerBasis, tms[i].lineup[j].size());

        }
    }
}

void calculateTechPositionValues(Club team1, Club team2) {
    Club tms[2] = { team1,team2 };
    float sum = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            sum = 0;
            for (int p = 0; p < tms[i].lineup[j].size(); p++) {
                sum += tms[i].lineup[j][p].technical;
            }
            teamValues_tech[i][j] = sum / (float)tms[i].lineup[j].size() + position_bonus[j][1];
            teamValues_tech[i][j] *= powf(positionCountPowerBasis, tms[i].lineup[j].size());
        }
    }

}

void calculateAttPositionValues(Club team1, Club team2) {
    Club tms[2] = { team1,team2 };
    float sum = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            sum = 0;
            for (int p = 0; p < tms[i].lineup[j].size(); p++) {
                sum += tms[i].lineup[j][p].offense;
            }
            teamValues_att[i][j] = sum / (float)tms[i].lineup[j].size() + position_bonus[j][2];
            teamValues_att[i][j] *= powf(positionCountPowerBasis, tms[i].lineup[j].size());

        }
    }

}

void calculatePossession() {
    float total_possessions[2];
    float lost[2], won[2];
    float tech[2] = { 0,0 }, def[2] = { 0,0 };
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            tech[i] += 0.01f * lostPossessions[j] * teamValues_tech[i][j];
            def[i] += 0.01f * wonPossessions[j] * teamValues_def[i][j];
        }
    }
    lost[0] = Calc::lostPossessions(tech[0] / def[1]);
    lost[1] = Calc::lostPossessions(tech[1] / def[0]);
    won[0] = Calc::wonPossessions(def[0] / tech[1]);
    won[1] = Calc::wonPossessions(def[1] / tech[0]);
    for (int i = 0; i < 2; i++) {
        total_possessions[i] = Calc::Max(lost[1 - i], won[i]);
        POSSESSION_TOTAL[i] = total_possessions[i];
    }
    for (int i = 0; i < 2; i++) {
        POSSESSION[i] = total_possessions[i]/(total_possessions[i] + total_possessions[1-i]);
    }
}

float attacker_Defender_Comparison(float att, float def) {
    return powf((att / def),2.15f) - 1.0f;
}

void calculateGoalsScored() {
    float shotsPos[2][4];
    sd[0] = 0;
    sd[1] = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            shotsPos[i][j] = shots[j] * (POSSESSION_TOTAL[i] / (float)possesions);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < activeClubs[i].lineup[j].size(); k++) {
                xG[i] += shotsPos[i][j] * Calc::Clamp((conversion_rate + attacker_Defender_Comparison(activeClubs[i].lineup[j][k].offense, teamValues_def[1 - i][1])), 0.04f, 0.4f);
                sd[i] += shotsPos[i][j] * Calc::Clamp((conversion_rate + attacker_Defender_Comparison(activeClubs[i].lineup[j][k].offense, teamValues_def[1 - i][1])), 0.04f, 0.4f) * (1.0f - Calc::Clamp((conversion_rate + attacker_Defender_Comparison(activeClubs[i].lineup[j][k].offense, teamValues_def[1 - i][1])), 0.04f, 0.4f));
                cout << activeClubs[i].lineup[j][k].name << " schiesst " << shotsPos[i][j] * Calc::Clamp((conversion_rate + attacker_Defender_Comparison(activeClubs[i].lineup[j][k].offense, teamValues_def[1 - i][1])), 0.04f, 0.4f) << " Tore." << endl;
            }
        }
    }
    //penalties
    xG[0] += pensPerGame;
    xG[1] += pensPerGame;
    cout << "SDs: " << sd[0] << ", " << sd[1] << endl;
}*/

void getResult() {
    for (int i = 0; i < 2; i++) {
        cout << "Tore Team " << i << ": " << endl;
        cin >> goals[i];
        currentMatch.result[i] = goals[i];
    }
    cout << sepLine << "Ergebnis:\n" << activeClubs[0].name << " " << goals[0] << " : " << goals[1] << " " << activeClubs[1].name << sepLine;
}


float normalDistributionProbability(float start, float end) {
    return 0.5f * erff(end / sqrt(2)) - 0.5f * erff(start / sqrt(2));
}

string calculateSDRangeandNormProb(float xg1 = 0.0f, float xg2 = 0.0f, int type = 0) {
    //--------------------------REGULAR CALCULATIONS--------------------
    
    //team with higher xG:
    string out = "";
    int betterTeamID, other;
    float sdStart, sdEnd;
    if (type == 0) {
        if (xG[0] > xG[1]) {
            betterTeamID = 0;
            other = 1;
        }
        else {
            betterTeamID = 1;
            other = 0;
        }
        //win for team with higher xG:
        sdStart = -1000.0f;
        sdEnd = (xG[betterTeamID] - xG[other] - 0.5f) / (sd[betterTeamID] + sd[other]);
        normProb[betterTeamID] = normalDistributionProbability(sdStart, sdEnd);
        //draw probability:
        sdStart = (xG[betterTeamID] - xG[other] - 0.5f) / (sd[betterTeamID] + sd[other]);
        sdEnd = (xG[betterTeamID] - xG[other] + 0.5f) / (sd[betterTeamID] + sd[other]);
        normProb[2] = normalDistributionProbability(sdStart, sdEnd);
        //win for team with lower xG:
        sdStart = (xG[betterTeamID] - xG[other] + 0.5f) / (sd[betterTeamID] + sd[other]);
        sdEnd = 10000.0f;
        normProb[other] = normalDistributionProbability(sdStart, sdEnd);
        //print probabilities:
        cout << "Wahrscheinlichkeiten:\n";
        cout << "Sieg " << activeClubs[0].name << ": " << 100 * normProb[0] << "\%" << endl;
        cout << "Unentschieden: " << 100 * normProb[2] << "\%" << endl;
        cout << "Sieg " << activeClubs[1].name << ": " << 100 * normProb[1] << "\%" << endl;
    }
    else {
        //-------------------PREVIOUS CALCULATIONS--------------------
        float sd1 = xg1 * (1.0f - Calc::ProbabilityfromxG(xg1,aExponentialFactor,kExponentialFactor,biasExponential));
        float sd2 = xg2 * (1.0f - Calc::ProbabilityfromxG(xg2,aExponentialFactor,kExponentialFactor,biasExponential));
        float xg[2] = { xg1, xg2 };
        float sd[2] = { sd1,sd2 };

        //team with higher xG:
        if (xg[0] > xg[1]) {
            betterTeamID = 0;
            other = 1;
        }
        else {
            betterTeamID = 1;
            other = 0;
        }
        //win for team with higher xG:
        sdStart = -1000.0f;
        sdEnd = (xg[betterTeamID] - xg[other] - 0.5f) / (sd[betterTeamID] + sd[other]);
        normalProbPrevious[betterTeamID] = normalDistributionProbability(sdStart, sdEnd);
        //draw probability:
        sdStart = (xg[betterTeamID] - xg[other] - 0.5f) / (sd[betterTeamID] + sd[other]);
        sdEnd = (xg[betterTeamID] - xg[other] + 0.5f) / (sd[betterTeamID] + sd[other]);
        normalProbPrevious[2] = normalDistributionProbability(sdStart, sdEnd);
        //win for team with lower xg:
        sdStart = (xg[betterTeamID] - xg[other] + 0.5f) / (sd[betterTeamID] + sd[other]);
        sdEnd = 10000.0f;
        normalProbPrevious[other] = normalDistributionProbability(sdStart, sdEnd);
        //goal probabilities::
        // 0 goals:
        sdStart = -1000.0f;
        for (int i = 0; i < 2; i++) {
            sdEnd = (xg[i] - 0.5f) / (-sd[i]);
            goalProbability[i][0] = normalDistributionProbability(sdStart, sdEnd);
        }
        // 1 goal:
        for (int i = 0; i < 2; i++) {
            sdStart = (xg[i] - 0.5f) / (-sd[i]);
            sdEnd = (xg[i] - 1.5f) / (-sd[i]);
            goalProbability[i][1] = normalDistributionProbability(sdStart, sdEnd);
        }
        // 2 goals:
        for (int i = 0; i < 2; i++) {
            sdStart = (xg[i] - 1.5f) / (-sd[i]);
            sdEnd = (xg[i] - 2.5f) / (-sd[i]);
            goalProbability[i][2] = normalDistributionProbability(sdStart, sdEnd);
        }
        // 3 goals:
        for (int i = 0; i < 2; i++) {
            sdStart = (xg[i] - 2.5f) / (-sd[i]);
            sdEnd = (xg[i] - 3.5f) / (-sd[i]);
            goalProbability[i][3] = normalDistributionProbability(sdStart, sdEnd);
        }
        // 4 goals:
        for (int i = 0; i < 2; i++) {
            sdStart = (xg[i] - 3.5f) / (-sd[i]);
            sdEnd = 1000.0f;
            goalProbability[i][4] = normalDistributionProbability(sdStart, sdEnd);
        }

        //print probabilities:
        out += "Wahrscheinlichkeiten:\n";
        out += "Tore: (Team1, Team2)\n";
        for (int j = 0; j < 4; j++) {
            out += j + ": " + to_string(goalProbability[0][j] * 100.0f) + "\%" + ", " + to_string(goalProbability[1][j] * 100.0f) + "\%\n";
        }
        out += "4+: " + to_string(goalProbability[0][4] * 100.0f) + "\%, " + to_string(goalProbability[1][4] * 100.0f) + "\%";
        out += sepLine;
        out += "Heimsieg: " + to_string(normalProbPrevious[0]) + "\n";
        out += "Unentschieden: " + to_string(normalProbPrevious[2]) + "\n";
        out += "Auswartssieg: " + to_string(normalProbPrevious[1]) + "\n";

    }
    return out;
}

void calculateWinProbabilities(bool neutralVenue = false) {

    winProb[0][0] = Calc::calc(clubs[activeClubs[0].id].homeAdv * probValue[0][0], probValue[1][3]);
    winProb[0][1] = Calc::calc(clubs[activeClubs[0].id].homeAdv * probValue[0][1], probValue[1][3]);
    winProb[0][2] = Calc::calc(clubs[activeClubs[0].id].homeAdv * probValue[0][2], probValue[1][2]);
    winProb[0][3] = Calc::calc(clubs[activeClubs[0].id].homeAdv * probValue[0][3], defValue[1]);
    winProb[1][0] = Calc::calc(probValue[1][0], clubs[activeClubs[0].id].homeAdv * probValue[0][3]);
    winProb[1][1] = Calc::calc(probValue[1][1], clubs[activeClubs[0].id].homeAdv * probValue[0][3]);
    winProb[1][2] = Calc::calc(probValue[1][2], clubs[activeClubs[0].id].homeAdv * probValue[0][2]);
    winProb[1][3] = Calc::calc(probValue[1][3], clubs[activeClubs[0].id].homeAdv * defValue[0]);
    if (neutralVenue) {
        winProb[0][0] = Calc::calc(probValue[0][0], probValue[1][3]);
        winProb[0][1] = Calc::calc(probValue[0][1], probValue[1][3]);
        winProb[0][2] = Calc::calc(probValue[0][2], probValue[1][2]);
        winProb[0][3] = Calc::calc(probValue[0][3], defValue[1]);
        winProb[1][0] = Calc::calc(probValue[1][0], probValue[0][3]);
        winProb[1][1] = Calc::calc(probValue[1][1], probValue[0][3]);
        winProb[1][2] = Calc::calc(probValue[1][2], probValue[0][2]);
        winProb[1][3] = Calc::calc(probValue[1][3], defValue[0]);

    }
    // expected goals
    xG[0] = Calc::xGfromProbability(winProb[0][3],aExponentialFactor,kExponentialFactor,biasExponential);
    sd[0] = sqrt(xG[0] * (1 - winProb[0][3]));
    xG[1] = Calc::xGfromProbability(winProb[1][3],aExponentialFactor, kExponentialFactor,biasExponential);
    sd[1] = sqrt(xG[1] * (1 - winProb[1][3]));
    xGdif[0] = xG[0] - xG[1];
    xGdif[1] = xG[1] - xG[0];
    cout << "Erwartetes Ergebnis:\n" << activeClubs[0].name << " " << xG[0] << " : " << xG[1] << " " << activeClubs[1].name << endl;
    //Confidence interval checks
    calculateSDRangeandNormProb();
}

int TotalPlayerCount() {
    int count = 0;
    for (int i = 0; i < clubs.size(); i++) {
        count += clubs[i].players.size();
    }
    return count;
}

void AddPointsToAllPlayers(float addition) {
    int count = 0;
    for (int i = 0; i < clubs.size(); i++) {
        for (int j = 0; j < clubs[i].players.size(); j++) {
            for (int k = 0; k < 4; k++) {
                clubs[i].players[j].value[k] += addition;
                count++;

            }
        }
    }
    cout << sepLine << "Punktangleichung: " << count << " Werte um " << addition << "veraendert." << sepLine;
}

void selectLineup(int activeTeam) {
    activeClubs[activeTeam].PrintPlayers();
    int tempPos = 0;
    int playersSelectedSoFar = 0;
    int choice;
    bool error = false;
nextpos:
    cout << positions[tempPos] << " fur " << activeClubs[activeTeam].name << " auswahlen: (\"99\" eingeben um zu nachster Position zu wechseln)" << endl;
    while (playersSelectedSoFar < 11) {
        cin >> choice;
        if (!cin) {
            cout << "Fehler: Unzulaessige eingabe. Beende Programm." << endl;
            error = true;
            goto End;
        }
        if (choice == 99) {
            tempPos += 1;
            goto nextpos;
        }
        activeClubs[activeTeam].lineup[tempPos].push_back(activeClubs[activeTeam].players[choice]);
        playersSelectedSoFar += 1;
    }
End:;
    if (error) {
        system("pause");
        terminate();
    }

}

void calculateMatchValues(bool previous = false) {
    float sumValues;
    for (int t = 0; t < 2; t++) {
        for (int p = 0; p < 4; p++) {
            sumValues = 0;
            for (int i = 0; i < activeClubs[t].lineup[p].size(); i++) {
                sumValues += activeClubs[t].lineup[p][i].value[p];
            }
            matchValuesTeamPosition[t][p] = sumValues / activeClubs[t].lineup[p].size();
            matchValuesTeamPosition[t][p] *= powf(positionCountPowerBasis, activeClubs[t].lineup[p].size());
            cout << "Speiltagswert " << clubs[activeClubs[t].id].name << ", " << positions[p] << ": " << matchValuesTeamPosition[t][p] << endl;
            Ranalysis_Values[t][p] = matchValuesTeamPosition[t][p] + clubs[activeClubs[t].id].Trainer.value * 0.008f;
        }
    }
    //R analytics data
    for (int i = 0; i < 4; i++) {
        Ranalysis_Values[0][i] *= clubs[activeClubs[0].id].homeAdv;
    }

    Club team1 = activeClubs[0];
    Club team2 = activeClubs[1];

    float coachEffect[2];
    coachEffect[0] = clubs[activeClubs[0].id].Trainer.value * 0.008f;
    coachEffect[1] = clubs[activeClubs[1].id].Trainer.value * 0.008f;

    //--------------------------|from doi:10.2478/hukin-2019-0015|--------------------------

    //team 1 final position values
    probValue[0][0] = matchValuesTeamPosition[0][0] * 0.6f + matchValuesTeamPosition[0][1] * 0.3f + matchValuesTeamPosition[0][2] * 0.1f;
    probValue[0][1] = matchValuesTeamPosition[0][0] * 0.2f + matchValuesTeamPosition[0][1] * 0.6f + matchValuesTeamPosition[0][2] * 0.2f;
    probValue[0][2] = matchValuesTeamPosition[0][1] * 0.2f + matchValuesTeamPosition[0][2] * 0.6f + matchValuesTeamPosition[0][3] * 0.2f;
    probValue[0][3] = matchValuesTeamPosition[0][1] * 0.057f + matchValuesTeamPosition[0][2] * 0.366f + matchValuesTeamPosition[0][3] * 0.577f;
    //team 2 final position values
    probValue[1][0] = matchValuesTeamPosition[1][0] * 0.6f + matchValuesTeamPosition[1][1] * 0.3f + matchValuesTeamPosition[1][2] * 0.1f;
    probValue[1][1] = matchValuesTeamPosition[1][0] * 0.2f + matchValuesTeamPosition[1][1] * 0.6f + matchValuesTeamPosition[1][2] * 0.2f;
    probValue[1][2] = matchValuesTeamPosition[1][1] * 0.2f + matchValuesTeamPosition[1][2] * 0.6f + matchValuesTeamPosition[1][3] * 0.2f;
    probValue[1][3] = matchValuesTeamPosition[1][1] * 0.057f + matchValuesTeamPosition[1][2] * 0.366f + matchValuesTeamPosition[1][3] * 0.577f;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            probValue[i][j] += coachEffect[i];
        }
    }

    defValue[0] = probValue[0][1];
    defValue[1] = probValue[1][1];

}

float calculateBestElevenPositionValue(Club team, int position) {
    vector<float> chosenPosValues;
    float posValue;
    chosenPosValues.clear();
    cout << "DEBUG: formation for " << team.name << " is " << Calc::round(team.averageFormation[0]) << "-" << Calc::round(team.averageFormation[1]) << "-" << Calc::round(team.averageFormation[2]) << "-" << Calc::round(team.averageFormation[3]) << endl;
    cout << team.players.size();
    for (int i = 0; i < team.players.size(); i++) {
        if (Calc::round(team.players[i].averagePos) == position && team.players[i].gamesPlayed != 0) {
            cout << "DEBUG: " << Calc::round(team.players[i].averagePos) << " = " << position  << ", with value " << team.players[i].value  << endl;
            chosenPosValues.push_back(team.players[i].value[position]);
            cout << "DEBUG: " << team.players[i].value << "hinzugefugt" << endl;
        }
    }
    posValue = Calc::getAveragefromHeader(chosenPosValues, Calc::round(team.averageFormation[position])) * powf(positionCountPowerBasis, Calc::round(team.averageFormation[position]));
    cout << "DEBUG: " << posValue << " is best mean for " << positions[position] << " of " << team.name << ", from " << Calc::getAveragefromHeader(chosenPosValues, Calc::round(team.averageFormation[position])) << endl;
    return posValue;
}

void calculateValueChange() {
    Gdif[0] = goals[0] - goals[1];
    Gdif[1] = goals[1] - goals[0];

    //team 1 tore vergleich
    if (xG[1] < goals[1]) {
        changeTeamPosition[0][0] = winProb[0][0] * changeConstant * Calc::clampRoot(xG[1] - goals[1]);
        changeTeamPosition[0][1] = winProb[0][1] * changeConstant * Calc::clampRoot(xG[1] - goals[1]);
        changeTeamPosition[1][3] = (1 - winProb[1][3]) * changeConstant * Calc::clampRoot(goals[1] - xG[1]);

    }
    else {
        changeTeamPosition[0][0] = (1 - winProb[0][0]) * changeConstant * Calc::clampRoot(xG[1] - goals[1]);
        changeTeamPosition[0][1] = (1 - winProb[0][1]) * changeConstant * Calc::clampRoot(xG[1] - goals[1]);
        changeTeamPosition[1][3] = winProb[1][3] * changeConstant * Calc::clampRoot(goals[1] - xG[1]);

    }
    //mid team 0
    if (Gdif[0] < xGdif[0]) {
        changeTeamPosition[0][2] = winProb[0][2] * changeConstant * Calc::clampRoot(Gdif[0] - xGdif[0]);

    }
    else {
        changeTeamPosition[0][2] = (1 - winProb[0][2]) * changeConstant * Calc::clampRoot(Gdif[0] - xGdif[0]);

    }
    //mid team 1
    if (Gdif[1] < xGdif[1]) {
        changeTeamPosition[1][2] = winProb[1][2] * changeConstant * Calc::clampRoot(Gdif[1] - xGdif[1]);

    }
    else {
        changeTeamPosition[1][2] = (1 - winProb[1][2]) * changeConstant * Calc::clampRoot(Gdif[1] - xGdif[1]);

    }
    //team 0 tore vergleich
    if (goals[0] < xG[0]) {
        changeTeamPosition[0][3] = winProb[0][3] * changeConstant * Calc::clampRoot(goals[0] - xG[0]);
        changeTeamPosition[1][0] = (1 - winProb[1][0]) * changeConstant * Calc::clampRoot(xG[0] - goals[0]);
        changeTeamPosition[1][1] = (1 - winProb[1][1]) * changeConstant * Calc::clampRoot(xG[0] - goals[0]);

    }
    else {
        changeTeamPosition[0][3] = (1 - winProb[0][3]) * changeConstant * Calc::clampRoot(goals[0] - xG[0]);
        changeTeamPosition[1][0] = winProb[1][0] * changeConstant * Calc::clampRoot(xG[0] - goals[0]);
        changeTeamPosition[1][1] = winProb[1][1] * changeConstant * Calc::clampRoot(xG[0] - goals[0]);

    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            changeTeamPosition[i][j] = Calc::Clamp(changeTeamPosition[i][j], -changeConstant, changeConstant);
        }
    }

    // save previous values for quick calculations
    clubs[activeClubs[0].id].previousValues[0] = defValue[0] + 0.2f * changeTeamPosition[0][0] + 0.6f * changeTeamPosition[0][1] + 0.2f * changeTeamPosition[0][2];
    clubs[activeClubs[0].id].previousValues[1] = probValue[0][3] + 0.057f * changeTeamPosition[0][2] + 0.366f * changeTeamPosition[0][2] + 0.577f * changeTeamPosition[0][3];
    clubs[activeClubs[1].id].previousValues[0] = defValue[1] + 0.2f * changeTeamPosition[1][0] + 0.6f * changeTeamPosition[1][1] + 0.2f * changeTeamPosition[1][2];
    clubs[activeClubs[1].id].previousValues[1] = probValue[1][3] + 0.057f * changeTeamPosition[1][2] + 0.366f * changeTeamPosition[1][2] + 0.577f * changeTeamPosition[1][3];

}

void applyValueChanges() {//Player values regress to position means by 5% of the difference each game
    float mean = 0;
    for (int t = 0; t < 2; t++) {
        for (int p = 0; p < 4; p++) {
            for (int i = 0; i < activeClubs[t].lineup[p].size(); i++) {//first loop to get means
                Player current = clubs[activeClubs[t].id].players[activeClubs[t].lineup[p][i].id];
                mean += current.value[Calc::round(current.averagePos)];
            }
            cout << "Mean = " << mean << " / " << (float)activeClubs[t].lineup[p].size() << endl;
            mean /= (float)activeClubs[t].lineup[p].size();
            
            for (int i = 0; i < activeClubs[t].lineup[p].size(); i++) {
                float playerVal = clubs[activeClubs[t].id].players[activeClubs[t].lineup[p][i].id].value[Calc::round(clubs[activeClubs[t].id].players[activeClubs[t].lineup[p][i].id].averagePos)];
                float diff = playerVal - mean;
                clubs[activeClubs[t].id].players[activeClubs[t].lineup[p][i].id].ChangeValue(changeTeamPosition[t][p] - 0.05f*diff,p);
                NetPoints += changeTeamPosition[t][p];
                cout << "Wert von " << activeClubs[t].players[activeClubs[t].lineup[p][i].id].name << " um " << changeTeamPosition[t][p] - 0.05f*diff << " auf " << clubs[activeClubs[t].id].players[activeClubs[t].lineup[p][i].id].value << " geandert." << endl;
            }
            mean = 0;
        }
    }
}

void downgradeSubstitutes() {
    cout << "Auswechselspieler:\n" << sepLine;
    int found;
    int index;
    for (int t = 0; t < 2; t++) {
        index = 0;
        for (int p = 0; p < 4; p++) {
            for (int i = 0; i < activeClubs[t].lineup[p].size(); i++) {
                //cout << "y of " << index << " = " << linedUpID[t][p][i] << endl;
                y[index] = activeClubs[t].lineup[p][i].id;
                index++;
            }
        }

        for (int i = 0; i < activeClubs[t].players.size(); i++) {
            found = 0;
            for (int j = 0; j < 11; j++) {
                if (i == y[j]) {
                    found++;
                    //cout << "\n found ++ auf " << found << endl;
                    goto endLoop;
                }
            }
            for (int kkk = 0; kkk < 4; kkk++) {
                if (found == 0 && goals[t] > goals[-t + 1]) {
                    float mmm = activeClubs[t].players[i].value[kkk] * 0.0005f;
                    float nnn = mmm * 1999;
                    clubs[activeClubs[t].id].players[i].value[kkk] *= 0.9995f;
                    NetPoints -= mmm;
                }
                else if (found == 0) {
                    float mmm = clubs[activeClubs[t].id].players[i].value[kkk] * 0.001f;
                    float nnn = mmm * 999;
                    clubs[activeClubs[t].id].players[i].value[kkk] *= 0.999f;
                    NetPoints -= mmm;
                }

            }
        endLoop:;
        }
        cout << "Verlorengegangene Punkte: " << NetPoints << endl << "Haelfte wiederherstellen..." << endl;
        AddPointsToAllPlayers((-0.125f * NetPoints) / (float)TotalPlayerCount());
        cout << "Alle Spielerwerte um " << (-0.125f * NetPoints)/(float)TotalPlayerCount() << " angeglichen." << endl;
    }
    //save games played and average positions
    for (int t = 0; t < 2; t++) {
        clubs[activeClubs[t].id].gamesPlayed++;
        cout << "DEBUG: " << clubs[activeClubs[t].id].gamesPlayed << endl;
        for (int p = 0; p < 4; p++) {
            clubs[activeClubs[t].id].averageFormation[p] = Calc::incrementAvg(clubs[activeClubs[t].id].averageFormation[p], activeClubs[t].lineup[p].size(), clubs[activeClubs[t].id].gamesPlayed);
            cout << "DEBUG: " << setprecision(6) << positions[p] << ", " << clubs[activeClubs[t].id].averageFormation[p] << ", " << activeClubs[t].lineup[p].size() << ", " << clubs[activeClubs[t].id].gamesPlayed << ", " << Calc::incrementAvg(clubs[activeClubs[t].id].averageFormation[p], activeClubs[t].lineup[p].size(), clubs[activeClubs[t].id].gamesPlayed) << endl;

            for (int i = 0; i < activeClubs[t].lineup[p].size(); i++) {
                clubs[activeClubs[t].id].players[activeClubs[t].lineup[p][i].id].gamesPlayed++;
                clubs[activeClubs[t].id].players[activeClubs[t].lineup[p][i].id].averagePos = Calc::incrementAvg(clubs[activeClubs[t].id].players[activeClubs[t].lineup[p][i].id].averagePos, p, clubs[activeClubs[t].id].players[activeClubs[t].lineup[p][i].id].gamesPlayed);;
            }
        }
    }
}

void CoachValueChange(bool neutralVenue = false) {
    //coach & home value team 0
    if (Gdif[0] < xGdif[0]) {
        clubs[activeClubs[0].id].Trainer.value += Calc::Clamp(normProb[0] * changeConstant * (Gdif[0] - xGdif[0]), -changeConstant, changeConstant);
        if (!neutralVenue) {
            clubs[activeClubs[0].id].SetHomeValue(clubs[activeClubs[0].id].homeValue + Calc::Clamp(normProb[0] * changeConstant * (Gdif[0] - xGdif[0]), -changeConstant, changeConstant));
        }
    }
    else {
        clubs[activeClubs[0].id].Trainer.value += Calc::Clamp((1 - normProb[0]) * changeConstant * (Gdif[0] - xGdif[0]), -changeConstant, changeConstant);
        if (!neutralVenue) {
            clubs[activeClubs[0].id].SetHomeValue(clubs[activeClubs[0].id].homeValue + Calc::Clamp((1 - normProb[0]) * changeConstant * (Gdif[0] - xGdif[0]), -changeConstant, changeConstant));
        }

    }
    //coach team 1
    if (Gdif[1] < xGdif[1]) {
        clubs[activeClubs[1].id].Trainer.value += Calc::Clamp(normProb[1] * changeConstant * (Gdif[1] - xGdif[1]), -changeConstant, changeConstant);

    }
    else {
        clubs[activeClubs[1].id].Trainer.value += Calc::Clamp((1 - normProb[1]) * changeConstant * (Gdif[1] - xGdif[1]), -changeConstant, changeConstant);

    }
}

string displayGoalProbabilities(vector<string> labels4 = { "Grenze", "Team 1", "Team 2", "Gesamt" }) {
    v1.clear();
    v2.clear();
    vComb.clear();
    string output;
    vector<string> left = { "<0.5", "<1.5", "<2.5", "<3.5", "<4.5" };
    float combProb[5];
    float newComb[5];
    for (int i = 0; i < 5; i++) {
        combProb[i] = 0;
        v1.push_back(goalProbability[0][i]);
        v2.push_back(goalProbability[1][i]);
    }
    combProb[0] = v1[0] * v2[0];
    combProb[1] = (v1[1] * v2[0]) + (v1[0] * v2[1]);
    combProb[2] = (v1[2] * v2[0]) + (v1[1] * v2[1]) + (v1[0] * v2[2]);
    combProb[3] = (v1[3] * v2[0]) + (v1[2] * v2[1]) + (v1[1] * v2[2]) + (v1[0] * v2[3]);
    combProb[4] = (v1[4] * v2[0]) + (v1[3] * v2[1]) + (v1[2] * v2[2]) + (v1[1] * v2[3]) + (v1[0] * v2[4]);
    for (int i = 0; i < 5; i++) {
        cout << sepLine << "Combined Goal Probability =" << i << ": " << combProb[i];
    }
    cout << sepLine;
    newComb[0] = combProb[0];
    newComb[1] = combProb[0] + combProb[1];
    newComb[2] = combProb[0] + combProb[1] + combProb[2];
    newComb[3] = combProb[0] + combProb[1] + combProb[2] + combProb[3];
    newComb[4] = combProb[0] + combProb[1] + combProb[2] + combProb[3] + combProb[4];
    for (int i = 1; i < 5; i++)
    {
        vComb.push_back(newComb[i]);
        v1[i] += v1[i - 1];
        v2[i] += v2[i - 1];
    }
    for (int i = 0; i < 5; i++) {
        vComb.push_back(combProb[i]);
    }
    output = Table::AsTable(left, v1, v2, vComb, labels4);
    return output;

}

string displayTopResults(int count) {
    string out = "";
    vector<string> results;
    vector<float> probs;
    vector<int> indices;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            probs.push_back(goalProbability[0][i] * goalProbability[1][j]);
            results.push_back(to_string(i) + " : " + to_string(j));
        }
    }
    for (int i = 0; i < probs.size(); i++) {
        indices.push_back(Calc::getSortedArrayIndices(probs, probs[i]));
    }
    vector<float> sortedProbs;
    vector<string> sortedResults;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < indices.size(); j++) {
            if (indices[j] == i) {
                int id = j;
                sortedProbs.push_back(probs[id]);
                sortedResults.push_back(results[id]);
            }
        }
        
    }
    return Table::AsTable(sortedResults, sortedProbs,"Ergebniswahrscheinlichkeiten");
}

void sortPlayers() {
    cout << "SORTING PLAYERS..." << endl;
    for (int i = 0; i < clubs.size(); i++) {
        vector<Player> PlayerVectorNew;
        vector<string> PlayerNamesVector;
        vector<int> sortedIndices;
        for (int j = 0; j < clubs[i].players.size(); j++) {
            PlayerNamesVector.push_back(clubs[i].players[j].name);
        }

        for (int j = 0; j < clubs[i].players.size(); j++) {
            sortedIndices.push_back(Calc::getSortedArrayIndices(PlayerNamesVector, clubs[i].players[j].name));
        }

        for (int ii = 0; ii < clubs[i].players.size(); ii++) {
            for (int j = 0; j < clubs[i].players.size(); j++) {
                if (sortedIndices[j] == ii) {
                    PlayerVectorNew.push_back(clubs[i].players[j]);
                }
            }
        }

        clubs[i].players = PlayerVectorNew;

        for (int k = 0; k < clubs[i].players.size(); k++) {
            PlayerVectorNew[k].id = k;
        }
    }
    cout << "!PLAYER SORTING COMPLETE!" << endl;

}

void GameDayOdds() {
    float temp1, temp2;
    float prev[2][2][9];
    float matchvals[2][4];
    int matches[9][2];
    float xGmatches[9][2];
    int numMatchesSelected = 0, tmp;
    int choice;
    for (int i = 0; i < clubs.size(); i++) {
        cout << i << ": " << clubs[i].name << endl;
    }
    for (int i = 0; i < 9; i++) {
        cout << "Begegnung " << i << " festlegen: (99 zum aufhoren)\n";
        cout << "Team 1:" << endl;
        cin >> tmp;
        if (tmp != 99) {
            matches[i][0] = tmp;
        }
        else {
            goto calculation;
        }
        cout << "Team 2:" << endl;
        cin >> tmp;
        if (tmp != 99) {
           matches[i][1] = tmp;
        }
        else {
            goto calculation;
        }
        numMatchesSelected++;
    }
calculation:;
    cout << "Letzte Aufstellung oder Beste 11?\n1:Letzte, 2:Beste" << endl;
    cin >> choice;
    //ITERATE THROUGH MATCHES
    for (int m = 0; m < numMatchesSelected; m++) {
        //previous lineups
        if (choice == 1) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    prev[i][j][m] = clubs[matches[m][i]].previousValues[j];
                }
            }
        }
        //best elevens
        else {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                    matchvals[i][j] = calculateBestElevenPositionValue(clubs[matches[m][i]], j);
                }
            }
            //team 1 final position values
            prev[0][0][m] = matchvals[0][0] * 0.2f + matchvals[0][1] * 0.6f + matchvals[0][2] * 0.2f;
            prev[0][1][m] = matchvals[0][1] * 0.057f + matchvals[0][2] * 0.366f + matchvals[0][3] * 0.577f;
            //team 2 final position values
            prev[1][0][m] = matchvals[1][0] * 0.2f + matchvals[1][1] * 0.6f + matchvals[1][2] * 0.2f;
            prev[1][1][m] = matchvals[1][1] * 0.057f + matchvals[1][2] * 0.366f + matchvals[1][3] * 0.577f;

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    prev[i][j][m] += clubs[matches[m][i]].Trainer.value * 0.008f;
                }
            }

        }
        //calculations for both scenarios

    }

    string output = ""; 
    cout << "Vorrausgesagte Ergebnisse:" << sepLine;
    for (int i = 0; i < numMatchesSelected; i++) {
        output = "";
        temp1 = Calc::xGfromProbability(Calc::calc(clubs[matches[i][0]].homeAdv * prev[0][1][i],prev[1][0][i]), aExponentialFactor, kExponentialFactor,biasExponential);
        temp2 = Calc::xGfromProbability(Calc::calc(prev[1][1][i], clubs[matches[i][0]].homeAdv * prev[0][0][i]), aExponentialFactor, kExponentialFactor,biasExponential);
        xGmatches[i][0] = temp1;
        xGmatches[i][1] = temp2;
        output += clubs[matches[i][0]].name + " " + to_string(xGmatches[i][0]) + " : " + to_string(xGmatches[i][1]) + " " + clubs[matches[i][1]].name + "\n";
        calculateSDRangeandNormProb(temp1, temp2, 1);
        output += "Wahrscheinlichkeiten: (" + to_string(Calc::oddsfromProbability(normalProbPrevious[0])) + "," + to_string(Calc::oddsfromProbability(normalProbPrevious[2])) + "," + to_string(Calc::oddsfromProbability(normalProbPrevious[1])) + ")" + "\n";
        output += "Sieg " + clubs[matches[i][0]].name + ": " + to_string(100.0f * normalProbPrevious[0]) + "\%" + "\n";
        output += "Unentschieden: " + to_string(100.0f * normalProbPrevious[2]) + "\%" + "\n";
        output += "Sieg " + clubs[matches[i][1]].name + ": " + to_string(100.0f * normalProbPrevious[1]) + "\%" + "\n";
        output += displayGoalProbabilities({ "Grenze", clubs[matches[i][0]].name,  clubs[matches[i][1]].name, "Gesamt" });
        output += sepLine;
        output += displayTopResults(5) + sepLine;
        cout << output;
    }
    cout << "In Textdatei speichern?\n" << "1: Ja, 2: Nein" << endl;
    int in;
    cin >> in;
    // save gameday data
    if (in == 1) {
        ofstream out("Data/GamedayPredictions.txt");
        out << output;
    }
}

void DisplayTableFromPersons(vector<Player> players, int amount, string title, bool write) {
    vector<string> names, teamNames;
    vector<float> values;
    for (int i = 0; i < players.size(); i++) {
        names.push_back(players[i].name);
        values.push_back(players[i].value[Calc::round(players[i].averagePos)]);
        teamNames.push_back(clubs[players[i].team].name);
    }
    if (write == true) {
        ofstream table("Data/Tabellen.txt",5);
        table << newBlock << endl << Table::writeableTable(names, values, teamNames, amount, title);
        table.close();
    }
    else {
        cout << Table::writeableTable(names, values, teamNames, amount, title);
    }
}

float getGrandMean() {
    float sum = 0;
    int count = 0;
    for (int i = 0; i < clubs.size(); i++) {
        for (int j = 0; j < clubs[i].players.size(); j++) {
            sum += clubs[i].players[j].value[Calc::round(clubs[i].players[j].averagePos)];
            count++;
        }
    }
    return sum / (float)count;
}

void DisplayTableFromPersons(vector<Coach> coaches, int amount, string title, bool write) {
    vector<string> names, teamNames;
    vector<float> values;
    for (int i = 0; i < coaches.size(); i++) {
        names.push_back(coaches[i].name);
        values.push_back(coaches[i].value);
        teamNames.push_back(clubs[coaches[i].team].name);
    }
    if (write == true) {
        ofstream table("Data/Tabellen.txt",5);
        table << newBlock << endl << Table::writeableTable(names, values, teamNames, amount, title);
        table.close();
    }
    else {
        cout << Table::writeableTable(names, values, teamNames, amount, title);
    }
}

string UserTextInput(string prompt = "Namen eingeben:") {
    cout << prompt << " 99 eingeben um Eingabe zu beenden." << endl;
    string output = "";
    int length = 0;
    string foo[8];
    for (int i = 0; i < 8; i++) {
        cin >> foo[i];
        if (foo[i] == "99") {
            foo[i] = "";
            foo[i-1].pop_back();
            goto TextInputEnd;
        }
        length++;
    }
TextInputEnd:;
    for (int i = 0; i < length; i++) {
        output += foo[i] + " ";
    }
    return output;
}

void displayClubRanking(int amount, bool write = false) {
    vector<float> clubvalues;
    vector<string> clubnames;
    vector<float> pVals;

    for (int i = 0; i < clubs.size(); i++) {
        for (int j = 0; j < clubs[i].players.size(); j++) {
            pVals.push_back(clubs[i].players[j].value[Calc::round(clubs[i].players[j].averagePos)]);
        }
        clubvalues.push_back(Calc::getAveragefromHeader(pVals, 11));
        clubnames.push_back(clubs[i].name);
        pVals.clear();
    }
    if (write) {
        ofstream table("Data/Tabellen.txt",5);
        table << newBlock << endl << Table::writeableTable(clubnames, clubvalues, amount, "Vereinsranking");
        table.close();
    }
    else {
        Table::displayTable(clubnames, clubvalues, amount, "Vereinsranking");

    }
}

void displayPlayerRanking(int amount, bool write = false) {
    vector<Player> Players;
    for (int i = 0; i < clubs.size(); i++) {
        for (int j = 0; j < clubs[i].players.size(); j++) {
            Players.push_back(clubs[i].players[j]);
        }
    }
    string label = "Rangliste der besten " + to_string(amount) + " Spieler";
    DisplayTableFromPersons(Players, amount, label, write);
}

void Settings() {
    int in;
    cout << "Einstellungen.\n";
    cout << "1. Torberechnungsformel optimieren (Nesterov accelerated gradient)\n" <<
        "2. Spielerwerte anpassen\n";
    cin >> in;

    if (in == 1) {
        int iter = 0;
        cout << "Anzahl Iterationen eingeben: (Empfohlen ~ 10000)" << endl;
        cout << "(Der Algorithmus stoppt automatisch, sollten Verbesserungen ausbleiben)" << endl;
        cin >> in;
        iter = in;
        optimizeFormula(iter);
        goto settingsEnd;
    }
    if (in == 2) {
        int league = 0;
        cout << "Angleichungsmethode auswaehlen:" << endl;
        cout << "1. Mittelwert-Standardabweichung-Standardisierung\n" <<
            "2. Quantilnormalisierung (WIP)\n";
        cin >> in;
        cout << "Alle Spieler (-1) oder nur bestimmte Liga (0=Bundesliga)?" << endl;
        cin >> league;
        if (in == 1) {
            float mu, sd;
            cout << "Gewuenschten Mittelwert eingeben" << endl;
            cin >> mu;
            cout << "Gewuenschte Standardabweichung eingeben" << endl;
            cin >> sd;
        }
        else if (in == 2) {
            

        }
    }
    
settingsEnd:;
}

void DisplayMenu() {
menuStart:;
    int inputX;
    int in;
    SetConsoleTextAttribute(hConsole, 14);
    cout << sepLine << "MENU" << sepLine;
    SetConsoleTextAttribute(hConsole, 7);

    cout << "Eine der Optionen auswahlen:\n";
    cout << "1. Spiel eintragen\n"
        << "11. Spiel mit neutralem Spielort eintragen\n"
        << "2. Spieltagswettquoten\n"
        << "3. Editor aufrufen\n"
        << "4. Statistiken aufrufen\n"
        << "5. Analysedaten anzeigen\n"
        << "6. Einstellungen\n";
    cin >> inputX;

    if (inputX == 1) {
        neutralVenue_game == false;
    }

    if (inputX == 11) {
        neutralVenue_game = true;
    }

    if (inputX == 2) {
        GameDayOdds();
        goto menuStart;
    }
    if (inputX == 3) {
        Editor();
        goto menuStart;
    }
    if (inputX == 4) {
        Statistics();
        goto menuStart;
    }

    if (inputX == 5) {
        //Spielerwertvektor erstellen:
        cout << sepLine;
        cout << "Durchschnittswert aller Bundesligaspieler: " << Calc::mean(allPlayerValues(-1,true)) << endl;
        cout << "Standardabweichung aller Bundesligaspieler: " << Calc::sd(allPlayerValues(-1,true)) << sepLine;


        cout << "Durchschnittswert aller Bundesligatorhueter: " << Calc::mean(allPlayerValues(0,true)) << endl;
        cout << "Standardabweichung aller Bundesligatorhueter: " << Calc::sd(allPlayerValues(0,true)) << sepLine;
        cout << "Durchschnittswert aller Bundesligaverteidiger: " << Calc::mean(allPlayerValues(1,true)) << endl;
        cout << "Standardabweichung aller Bundesligaverteidiger: " << Calc::sd(allPlayerValues(1,true)) << sepLine;
        cout << "Durchschnittswert aller Bundesligamittelfeldspieler: " << Calc::mean(allPlayerValues(2,true)) << endl;
        cout << "Standardabweichung aller Bundesligamittelfeldspieler: " << Calc::sd(allPlayerValues(2,true)) << sepLine;
        cout << "Durchschnittswert aller Bundesligastuermer: " << Calc::mean(allPlayerValues(3,true)) << endl;
        cout << "Standardabweichung aller Bundesligastuermer: " << Calc::sd(allPlayerValues(3,true)) << sepLine;


        cout << "RMSE: " << Analysis::getPerformanceIndex(Analysis_Probs, result, 0) << endl;
        cout << "Mean Bias: " << Analysis::getPerformanceIndex(Analysis_Probs, result, 1) << endl;
        cout << "Durchschnittliche Abweichung der Tore: " << Analysis::getAverageDeviationFromRealGoals(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1]) << endl;
        cout << "Anteil korrekter Tendenzen: " << Analysis::getCorrect1X2Percentage(predictedResult, result) << endl;
        cout << "Anteil korrekter Tendenzen ohne 0:0: " << Analysis::getCorrect1X2PercentageWithoutGoalless(predictedResult, Analysis_goals[0], Analysis_goals[1]) << endl;
        cout << "Anteil korrekter doppelter Chancen: " << Analysis::getCorrectDoublePercentage(Analysis_Probs, result) << endl;
        cout << "Durchschnittliche Wahrscheinlichkeit fur korrekte Tendenz: " << Analysis::getAveragePercentageToWin(Analysis_Probs, result) << sepLine;
        cout << "Gerundet: " << endl;
        cout << "Anteil korrekter Tore pro Team: " << Analysis::getCorrectGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 0) << endl;
        cout << "Anteil exakt korrekter Ergebnisse: " << Analysis::getCorrectGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 1) << endl;
        cout << "Abgerundet:" << endl;
        cout << "Anteil korrekter Tore pro Team: " << Analysis::getCorrectFloorGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 0) << endl;
        cout << "Anteil exakt korrekter Ergebnisse: " << Analysis::getCorrectFloorGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 1) << endl;
        cout << "Aufgerundet:" << endl;
        cout << "Anteil korrekter Tore pro Team: " << Analysis::getCorrectCeilingGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 0) << endl;
        cout << "Anteil exakt korrekter Ergebnisse: " << Analysis::getCorrectCeilingGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 1) << endl;
        cout << "Ab .75 aufgerundet:" << endl;
        cout << "Anteil korrekter Tore pro Team: " << Analysis::getCorrectGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 0, 0.75f) << endl;
        cout << "Anteil exakt korrekter Ergebnisse: " << Analysis::getCorrectGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 1, 0.75f) << endl;
        cout << "Ab .25 aufgerundet:" << endl;
        cout << "Anteil korrekter Tore pro Team: " << Analysis::getCorrectGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 0, 0.25f) << endl;
        cout << "Anteil exakt korrekter Ergebnisse: " << Analysis::getCorrectGoalsPercentage(Analysis_xG[0], Analysis_xG[1], Analysis_goals[0], Analysis_goals[1], 1, 0.25f) << endl;


        cout << sepLine;
        goto menuStart;
    }

    if (inputX == 6) {
        Settings();
        goto menuStart;
    }
}

void Statistics() {
statStart:;
    cout << sepLine << "STATISTIK" << sepLine;
    cout << "Eine der folgenden Optionen waehlen:\n";
    int inputX, in;
    cout << "Eine der folgenden Optionen waehlen:\n"
        << "1. Spieler-Rangliste anzeigen\n"
        << "2. Team-Rangliste anzeigen\n"
        << "3. Trainer-Rangliste anzeigen\n"
        << "4. Heimstaerke-Rangliste anzeigen\n"
        << "99. Zurueck zum Menu\n";
    cin >> inputX;
    if (inputX == 1) {
        cout << "Alle Spieler oder Positionsspezifisch?\n" << "1: Alle, 2: Positionen" << endl;
        cin >> in;
        if (in == 2) {
            vector<Player> all, pos[4];
            for (int i = 0; i < clubs.size(); i++) {
                for (int j = 0; j < clubs[i].players.size(); j++) {
                    Player currentPlayer = clubs[i].players[j];
                    if (currentPlayer.gamesPlayed != 0) {
                        all.push_back(currentPlayer);
                    }
                }
            }
            for (int i = 0; i < all.size(); i++) {
                for (int j = 0; j < 4; j++) {
                    if (round(all[i].averagePos) == j) {
                        pos[j].push_back(all[i]);
                    }

                }
            }
            cout << "Wie viele Spieler sollen angezeigt werden?" << endl;
            cin >> in;
            for (int i = 0; i < 4; i++) {
                DisplayTableFromPersons(pos[i], in, positions[i]);
            }
            cout << "Tabelle speichern?\n" << "1:Ja, 2:Nein" << endl;
            cin >> inputX;
            if (inputX == 1) {
                for (int i = 0; i < 4; i++) {
                    DisplayTableFromPersons(pos[i], in, positions[i], true);
                }
            }
        }
        else{
            cout << "Wie viele Spieler sollen angezeigt werden?" << endl;
            cin >> in;
            displayPlayerRanking(in);
            cout << "Tabelle speichern?\n" << "1:Ja, 2:Nein" << endl;
            cin >> inputX;
            if (inputX == 1) {
                displayPlayerRanking(in, true);
            }
        }
        goto menu;
    }

    if (inputX == 2) {
        cout << "Wie viele Teams sollen angezeigt werden?" << endl;
        cin >> in;
        displayClubRanking(in);
        cout << "Tabelle speichern?\n" << "1:Ja, 2:Nein" << endl;
        cin >> inputX;
        if (inputX == 1) {
            displayClubRanking(in, true);
        }
        goto menu;
    }

    if (inputX == 3) {
        vector<Coach> coaches;
        for (int i = 0; i < clubs.size(); i++) {
            coaches.push_back(clubs[i].Trainer);
        }
        DisplayTableFromPersons(coaches, clubs.size(), "Trainer-Rangliste", false);
        goto menu;
    }

    if (inputX == 4) {
        vector<string> names;
        vector<float> values;
        vector<string> snames;
        vector<float> svalues;
        vector<int> indices;
        for (int i = 0; i < clubs.size(); i++) {
            names.push_back(clubs[i].name);
            values.push_back(clubs[i].homeAdv);
        }
        for (int i = 0; i < names.size(); i++) {
            indices.push_back(Calc::getSortedArrayIndices(values, values[i]));
        }
        for (int i = 0; i < names.size(); i++) {
            for (int j = 0; j < names.size(); j++) {
                if (indices[j] == i) {
                    snames.push_back(names[j]);
                    svalues.push_back(values[j]);
                }

            }
        }
        cout << Table::AsTable(snames, svalues, "Heimvorteil-Rangliste");
        goto menu;
    }

    if (inputX == 99) {
        goto menu;
    }
    //end:
menu:;
    cout << "1: Weitermachen, 2: Zurueck zum Menu" << endl;
    cin >> in;
    if (in == 1) {
        goto statStart;
    }
}



void Editor() {
editorStart:;
    readData();
    int inputX, in;
    cout << sepLine << "EDITOR" << sepLine;
    cout << "Eine der folgenden Optionen waehlen:\n"
        << "1: Spieler hinzufuegen\n"
        << "2: Spieler entfernen\n"
        << "3: Trainer hinzufuegen/ersetzen\n"
        << "4: Spielertransfer durchfuehren\n"
        << "5: Trainertransfer durchfuehren\n"
        << "6: Verein hinzufuegen\n"
        << "7: Liga hinzufuegen\n"
        << "8: Ligazugehoerigkeit aendern\n";
    cin >> inputX;
    if (inputX == 1) {
        string pName;
        float pVal, PVAL[4];
        for (int i = 0; i < clubs.size(); i++) {
            cout << clubs[i].id << " " << clubs[i].name << endl;
        }
        cout << "Team auswahlen:" << endl;
        cin >> in;
        clubs[in].PrintPlayers();
        cout << "Namen des Spielers?" << endl;
        cin >> pName;
        vector<float> vals;
        for (int i = 0; i < clubs[in].players.size(); i++) {
            vals.push_back(clubs[in].players[i].value[Calc::round(clubs[in].players[i].averagePos)]);
        }
        float mean = Calc::getAveragefromHeader(vals, 18);
        cout << "Wert des Spielers? Empfehlung basiert auf Teamstaerke (Durchschnittlicher Wert der Top 18): " + to_string(mean) << endl;
        cout << "(Eine Suche nach bereits vorhandenen Spielern wird im naechsten Schritt ausgefuehrt)" << endl;
        cin >> pVal;
        for (int i = 0; i < 4; i++) {
            PVAL[i] = pVal;
        }
        clubs[in].AddPlayer(pName, PVAL);
        clubs[in].PrintPlayers();
        writeData();
    }
    if (inputX == 2) {
        int in2;
        for (int i = 0; i < clubs.size(); i++) {
            cout << clubs[i].id << " " << clubs[i].name << endl;
        }
        cout << "Team auswahlen:" << endl;
        cin >> in;
        clubs[in].PrintPlayers();
        cout << "Spieler auswaehlen:" << endl;
        cin >> in2;
        clubs[in].iRemovePlayer(in2);
        clubs[in].PrintPlayers();
        writeData();

    }
    if (inputX == 3) {
        string tName;
        cout << sepLine;
        for (int i = 0; i < clubs.size(); i++) {
            cout << setw(2) << left << clubs[i].id << " | " << setw(25) << left << clubs[i].name << " | " << clubs[i].Trainer.name << endl;
        }
        cout << "Team auswaehlen:" << endl;
        cin >> in;
        cout << "Namen des fuer " << clubs[in].name << " hinzuzufuegenden Trainers eingeben:" << endl;
        cin >> tName;
        clubs[in].NewCoach(tName);
        writeData();
    }
    if (inputX == 4) {
        cout << "Spielerauswahl" << sepLine;
        cout << "Auswahl nach aktuellem Verein (1) oder Namen (2)?" << endl;
        cin >> in;
        Player* pccc = nullptr;
        Club* ccc = nullptr;
        bool found = false;
        if (in == 1) {
            cout << "Team des zu transferierenden Spielers auswaehlen:" << endl;
            for (int i = 0; i < clubs.size(); i++) {
                cout << i << ": " << clubs[i].name << endl;
            }
            cin >> in;
            cout << "Zu transferierenden Spieler auswaehlen:" << endl;
            ccc = &clubs[in];
            ccc->PrintPlayers();
            cin >> in;
            pccc = &ccc->players[in];
            found = true;
        }
        else {
            cout << "Namen eingeben:" << endl;
            string name;
            cin >> name;
            for (int i = 0; i < clubs.size(); i++) {
                for (int j = 0; j < clubs[i].players.size(); j++) {
                    if (clubs[i].players[j].name == name) {
                        cout << "Spieler gefunden in " + clubs[i].name + " mit Wert " + to_string(clubs[i].players[j].value[Calc::round(clubs[i].players[j].averagePos)]) + ", Position " + positions[Calc::round(clubs[i].players[j].averagePos)] << endl;
                        cout << "Diesen Spieler transferieren? 1 = JA, 2 = NEIN" << endl;
                        int in;
                        cin >> in;
                        if (in == 1) {
                            ccc = &clubs[i];
                            pccc = &ccc->players[j];
                            found = true;
                        }
                        else {
                            cout << "Weitersuchen..." << endl;
                        }
                    }
                }
            }
        }
        if (found) {
            cout << "Ausgewaehlter Spieler: " << pccc->name << endl << endl;
            cout << "Neues Team fuer " << pccc->name << " auswaehlen:" << endl;
            for (int i = 0; i < clubs.size(); i++) {
                cout << i << ": " << clubs[i].name << endl;
            }
            cin >> in;
            Club* CCC = &clubs[in];
            cout << pccc->name << " wird zu " << CCC->name << " hinzugefuegt. Mit '1' bestaetigen, mit '2' abbrechen." << endl;
            cin >> in;
            if (in == 1) {
                CCC->AddPlayer(*pccc);
                ccc->iRemovePlayer(pccc->id, true);
                cout << pccc->name << " von " << ccc->name << " zu " << CCC->name << " transferiert" << endl;
                CCC->PrintPlayers();
                writeData();
            }
            else {
                cout << "Aufgabe abgebrochen, keine Aenderungen vorgenommen" << endl;
            }
        }
        else {
            cout << "Spieler nicht gefunden. Gross/Kleinschreibung beachtet? Kehre zurueck zum Menu." << endl;
        }

    }
    if (inputX == 5) {
        cout << "Trainerauswahl" << sepLine;
        cout << "Zu transferierenden Trainer auswaehlen:" << endl;
        for (int i = 0; i < clubs.size(); i++) {
            cout << i << ": " << clubs[i].Trainer.name << endl;
        }
        cin >> in;
        Club *ccc = &clubs[in];
        Coach *pccc = &clubs[in].Trainer;
        cout << "Ausgewaehlter Trainer: " << pccc->name << endl << endl;
        cout << "Neues Team fuer " << pccc->name << " auswaehlen:" << endl;
        for (int i = 0; i < clubs.size(); i++) {
            cout << i << ": " << clubs[i].name << endl;
        }
        cin >> in;
        Club *CCC = &clubs[in];
        cout << pccc->name << " wird zu " << CCC->name << " hinzugefuegt. Mit '1' bestaetigen, mit '2' abbrechen." << endl;
        cin >> in;
        if (in == 1) {
            CCC->NewCoach(*pccc,true);
            ccc->AssignEmptyCoach();
            
            cout << pccc->name << " von " << ccc->name << " zu " << CCC->name << " transferiert" << endl;
            writeData();
        }
        else {
            cout << "Aufgabe abgebrochen, keine Aenderungen vorgenommen" << endl;
        }

    }
    if (inputX == 6) {
        int lig;

    ClubAdditionStart:;
        cout << sepLine << "Bereits existierende Vereine:" << sepLine;
        for (int i = 0; i < clubs.size(); i++) {
            cout << clubs[i].id << " " << clubs[i].name << endl;
        }
        string tName = UserTextInput("Namen des hinzuzufugenden Teams eingeben:");
        cout << "Liga des Teams festlegen: (1.Liga = 0, 2.Liga = 1, etc.)" << endl;
        cin >> lig;
        Club tmp;
        tmp.id = clubs.size();
        tmp.name = tName;
        tmp.SetHomeValue(2000.0f);
        cout << "Nachnamen des Trainers eingeben:" << endl;
        cin >> tName;
        tmp.NewCoach(tName);
        tmp.league = lig;
        clubs.push_back(tmp);
        cout << "Auch Spieler hinzufugen?" << endl;
        cout << "1:Ja, 2:Nein" << endl;
        cin >> inputX;
        float val, VAL[4];
        if (inputX == 1) {
            vector<int> ids;
            vector<string> names;
            vector<float> values;
            cout << "Niveau des Teams festlegen: (z. B. 1950)" << endl;
            cin >> val;
            for (int ij = 0; ij < 4; ij++) {
                VAL[ij] = val;
            }
        AddPlayerStart:;
            cout << "Nachnamen des Spielers eingeben: (Doppelnamen mit \"_\")" << endl;
            cin >> tName;
            ids.push_back(clubs[tmp.id].players.size());
            names.push_back(tName);
            values.push_back(val);
            Player added(clubs[tmp.id].players.size(), tName, tmp.id, VAL, 0, 0);
            clubs[tmp.id].players.push_back(added);
            cout << "Weiteren Spieler hinzufugen?" << endl;
            cout << "1:Ja, 2:Nein" << endl;
            cin >> inputX;
            if (inputX == 1) {
                goto AddPlayerStart;
            }
            else {
                cout << "Zusammenfassung:" << endl;
                cout << Table::AsTable(ids, names, values, { "ID", "Name", "Wert" }, "Neue Spieler fur " + tmp.name);
                cout << clubs[tmp.id].players.size() << " Spieler hinzugefugt." << endl;
            }
        }
        cout << "Weiteren Verein hinzufugen?" << endl;
        cout << "1:Ja, 2:Nein" << endl;
        cin >> inputX;
        writeData();

        if (inputX == 1) {
            goto ClubAdditionStart;
        }
    }
    if (inputX == 7) {
        string ligName;
        vector<string> leagueNames, emptyStringVector;
        vector<float> leagueValues;
        for (int i = 0; i < leagues.size(); i++) {
            leagueNames.push_back(leagues[i].name);
            leagueValues.push_back(leagues[i].AveragePlayerValue());
            emptyStringVector.push_back("");
        }
        cout << Table::AsTable(leagueNames, leagueValues, "Aktuell existierende Ligen");
        ligName = UserTextInput("Namen der hinzuzufuegenden Liga eingeben:");
        League tmp;
        tmp.name = ligName;
        cout << "Teams hinzufuegen?\n1:Ja, 2:Nein" << endl;
        cin >> inputX;
        if (inputX == 1) {
            for (int i = 0; i < clubs.size(); i++) {
                cout << i << ": " << clubs[i].name << endl;
            }
        TeamInputStart:;
            cout << "Index des hinzuzufuegenden Teams eingeben:" << endl;
            cin >> in;
            tmp.teams.push_back(clubs[in]);
            tmp.DisplayLeagueMembers();
            cout << "Weiteres Team hinzufuegen?\n1:Ja, 2:Nein" << endl;
            cin >> in;
            if (in == 1) {
                goto TeamInputStart;
            }
        }
        leagues.push_back(tmp);
        cout << "Liga " << leagues[leagues.size()].name << "hinzugefuegt:" << endl;
        tmp.DisplayLeagueMembers();
    }
    if (inputX == 8) {
        int in;
        string ligs[2] = { "Bundesliga","2. Bundesliga" };
        cout << "Verein auswaehlen: " << endl;
        for (int i = 0; i < clubs.size(); i++) {
            cout << i << ": " << clubs[i].name << ": " << ligs[clubs[i].league] << endl;
        }
        int ligin;
        cin >> in;
        cout << "Verein " << clubs[in].name << " ausgewaehlt. Aktuell in " << ligs[clubs[in].league] << endl;
        cout << "Neue Liga (0 = Bundesliga, 1 = 2. Liga):" << endl;
        cin >> ligin;
        cout << "Verein " << clubs[in].name << " wird zu " << ligs[ligin] << " verschoben." << endl;
        Club* ref = &clubs[in];
        ref->league = ligin;
        cout << "Neue Ligazugehoerigkeiten: " << sepLine;
        for (int i = 0; i < clubs.size(); i++) {
            cout << i << ": " << clubs[i].name << ": " << ligs[clubs[i].league] << endl;
        }
        cout << "Speichern? 1: JA, 2:NEIN" << endl;
        cin >> in;
        if (in == 1) {
            writeData();
        }
        else {
            cout << "Rueckkehr zum Menu." << sepLine;
        }
    }
    cout << "Editor beenden?\n'1' um zu beenden, '2' um weiterzumachen\n";
    cin >> in;
    if (in == 2) {
        goto editorStart;
    }

}

void saveAnalyticdata() {
    ifstream an("Data/analytics.txt");
    if (!an.good()) {
        ofstream analytics("Data/analytics.txt");
        analytics << activeClubs[0].id << "," << activeClubs[1].id << "," << xG[0] << "," << xG[1] << "," << normProb[0] << "," << normProb[2] << "," << normProb[1] << "," << goals[0] << "," << goals[1] << endl;
        analytics.close();

    }
    else {
        an.close();
        ofstream analytics("Data/analytics.txt", 5);
        analytics << activeClubs[0].id << "," << activeClubs[1].id << "," << xG[0] << "," << xG[1] << "," << normProb[0] << "," << normProb[2] << "," << normProb[1] << "," << goals[0] << "," << goals[1] << endl;

    }
    an.close();
    ifstream Ran("Data/Ranalytics.txt");
    if (!Ran.good()) {
        ofstream ran("Data/Ranalytics.txt");
        ran.close();
    }
    Ran.close();
    ofstream Ranalytics("Data/Ranalytics.txt", 5);
    for (int run = 0; run < 2; run++) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 4; j++) {
                Ranalytics << Ranalysis_Values[abs(run-i)][j] << ",";
            }
        }
        Ranalytics << goals[run] << endl;
    }
    Ranalytics.close();
}

void overwriteAnalytics() {
    ofstream an("Data/analytics.txt", ios_base::trunc);
    for (int i = 0; i < Analysis_teams[0].size(); i++) {
        an << Analysis_teams[0][i] << "," << Analysis_teams[1][i] << "," << Analysis_xG[0][i] << "," << Analysis_xG[1][i] << "," << Analysis_Probs[0][i] << "," << Analysis_Probs[2][i] << "," << Analysis_Probs[1][i] << "," << Analysis_goals[0][i] << "," << Analysis_goals[1][i] << endl;
    }
}


int main() {
start:;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //color choices:
    /*for (int k = 0; k < 400; k++)
    {
        // pick the colorattribute k you want
        SetConsoleTextAttribute(hConsole, k);
        cout << k << " I want to be nice today!" << endl;
    }*/
    //Germany::
    int cols[3] = { 3, 72, 231 };
    for (int j = 0; j < 3; j++) {
        SetConsoleTextAttribute(hConsole, cols[j]);

        for (int i = 0; i < 15; i++) {
            cout << "." << endl;
        }
    }
    SetConsoleTextAttribute(hConsole, 7);
    cout << endl;

    /*cout << Calc::calc(100, 35) << endl << Calc::xGfromProbability(Calc::calc(100, 35),aExponentialFactor,kExponentialFactor) << endl;
    cout << Calc::calc(35,100) << endl << Calc::xGfromProbability(Calc::calc(35,100),aExponentialFactor,kExponentialFactor) << endl;*/
    readData();
    if (!correctVersion) {
        writeData();
    }

    /*
    for (int i = 0; i < clubs.size(); i++) {
        for (int j = 0; j < clubs[i].players.size(); j++) {
            clubs[i].players[j].offense = clubs[i].players[j].value[2];
            clubs[i].players[j].defense = clubs[i].players[j].value[2];
            clubs[i].players[j].technical = clubs[i].players[j].value[2];
        }
    } 
    */

    //cout << attacker_Defender_Comparison(2070, 2000) << endl; 
    //DisplayTableFromPersons(clubs[13].players, clubs[13].players.size(), "TEST-Titel");
    //displayClubRanking(clubs.size());
    DisplayMenu();
    currentMatch = newMatch();
    selectLineup(0);
    selectLineup(1);
    /*
    calculateDefPositionValues(activeClubs[0], activeClubs[1]);
    calculateTechPositionValues(activeClubs[0], activeClubs[1]);
    calculateAttPositionValues(activeClubs[0], activeClubs[1]);
    calculatePossession();
    calculateGoalsScored();
    cout << "Neue Formel:";
    cout << sepLine << xG[0] << " : " << xG[1] ;
    cout << sepLine << "Ballbesitz: " << sepLine << POSSESSION[0] << ", " << POSSESSION[1] << sepLine << endl;
    cout << calculateSDRangeandNormProb(xG[0], xG[1]) << endl;
    */
    calculateMatchValues();
    calculateWinProbabilities(neutralVenue_game);
    getResult();
    calculateValueChange();
    applyValueChanges();
    CoachValueChange(neutralVenue_game);
    downgradeSubstitutes();
    saveAnalyticdata();
    writeData();
    goto start;
}

vector<float> allPlayerValues(int position = -1, bool BundesligaOnly = true) {
    vector<float> ret;
    if (position == -1) {
        for (int i = 0; i < clubs.size(); i++) {
            if (BundesligaOnly) {
                if (clubs[i].league == 0) {
                    for (int j = 0; j < clubs[i].players.size(); j++) {
                        if (clubs[i].players[j].gamesPlayed > 0) {
                            ret.push_back(clubs[i].players[j].value[Calc::round(clubs[i].players[j].averagePos)]);
                        }
                    }
                }
            }
            else {
                for (int j = 0; j < clubs[i].players.size(); j++) {
                    if (clubs[i].players[j].gamesPlayed > 0) {
                        ret.push_back(clubs[i].players[j].value[Calc::round(clubs[i].players[j].averagePos)]);
                    }
                }
            }
        }
    }
    else {
        for (int i = 0; i < clubs.size(); i++) {
            if (BundesligaOnly) {
                if (clubs[i].league == 0) {
                    for (int j = 0; j < clubs[i].players.size(); j++) {
                        bool check1 = Calc::round(clubs[i].players[j].averagePos) == position ? true : false;
                        bool check2 = clubs[i].players[j].gamesPlayed > 0 ? true : false;
                        if (check1 && check2) {
                            ret.push_back(clubs[i].players[j].value[Calc::round(clubs[i].players[j].averagePos)]);
                        }
                    }
                }
            }
            else {
                for (int j = 0; j < clubs[i].players.size(); j++) {
                    bool check1 = Calc::round(clubs[i].players[j].averagePos) == position ? true : false;
                    bool check2 = clubs[i].players[j].gamesPlayed > 0 ? true : false;
                    if (check1 && check2) {
                        ret.push_back(clubs[i].players[j].value[Calc::round(clubs[i].players[j].averagePos)]);
                    }
                }
            }            
        }
    }
    
    return ret;
}


float accuracy(vector<float> probs, vector<float> actual, float a, float k, float b, vector<bool> use) {
    float corrects = 0;
    float totals = 0;
    for (int i = 0; i < probs.size(); i++) {
        if (use[i]) {
            if (Calc::round(Calc::xGfromProbability(probs[i], a, k, b)) == actual[i]) {
                corrects++;
            }
            totals++;
        }
    }
    return corrects / totals;
}

void refreshAnalytics(vector<float> inputProbs) {
    int index = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < Analysis_xG[i].size(); j++) {
            Analysis_xG[i][j] = Calc::xGfromProbability(inputProbs[index], aExponentialFactor, kExponentialFactor, biasExponential);
            index++;
        }
    }
}

float calculateBias(vector<float> expected, vector<float> actual, float a, float k, float bias, vector<bool> use) {
    float sum = 0;
    int count = 0;
    for (int i = 0; i < expected.size(); i++) {
        if (use[i]) {
            sum += Calc::xGfromProbability(expected[i], a, k, bias) - actual[i];
            count++;
        }
    }
    sum /= (float)count;
    return sum;
}

void optimizeFormula(int iterations = 1000) {
    cout << "Optimierung gestartet...\nSammle Daten..." << endl;
    vector<float> inputs, targets;
    vector<bool> usable;
    float lambda = 0.0000001f;
    float a = aExponentialFactor;
    float k = kExponentialFactor;
    float b = biasExponential;
    float ac = 0, kc = 0, bc = 0;
    float minXG = 100;
    float alpha = 0.00001f;
    float currentAlpha = alpha;
    float momentum = 0.9f;
    int patience = 5;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < Analysis_goals[i].size(); j++) {
            usable.push_back(Analysis_goals[i][j] < 5);
            float in = Calc::ProbabilityfromxG(Analysis_xG[i][j], a, k, b);
            float xgggg = Analysis_xG[i][j];
            if (xgggg < minXG)minXG = xgggg;
            float tar = static_cast<float>(Analysis_goals[i][j]);
            inputs.push_back(in);
            targets.push_back(tar);
            cout << "log(" << xgggg << "/" << a << ")/" << k << " -> " << in << ", " << tar << " using: " << usable.back() << endl;
            cin;
        }
    }
    float previousError = error(inputs, targets, a, k, b, usable);
    cout << sepLine << "Daten gesammelt. Aktuell Error: " << previousError << ", Accuracy: " << accuracy(inputs, targets, a, k, b, usable) << sepLine;
    vector<float> losses;
    vector<float> accuracies;
    vector<float> as;
    vector<float> ks;
    vector<float> bs;
    float bestA = a, bestK = k, bestB = b;
    float minError = previousError;
    for (int i = 0; i < iterations; i++) {
        currentAlpha = alpha / sqrtf(i + 1);
        gradient(inputs, targets, a, k, b,usable);

        float atp = a - currentAlpha * aGrad;//Gradient stage
        float at = momentum * (atp - a);//Nesterov momentum
        a = atp + at;

        float ktp = k - currentAlpha * kGrad;
        float kt = momentum * (ktp - k);
        k = ktp + kt;

        float btp = b - currentAlpha * bGrad;
        float bt = momentum * (btp - b);
        b = btp + bt;
        /* Regular Momentum
        a += -currentAlpha * aGrad + momentum*ac - lambda*a;
        ac = -currentAlpha * aGrad;
        k += -currentAlpha * kGrad + momentum*kc - lambda * k;
        kc = -currentAlpha * kGrad;
        b += -currentAlpha * bGrad + momentum * bc;
        bc = -currentAlpha * bGrad;
        */
        float currentError = error(inputs, targets, a, k, b, usable);
        float acc = accuracy(inputs, targets, a, k, b, usable);
        cout << "Epoch " << i+1 << ": " << "Error: " << currentError << ", Accuracy: " << acc << ", Bias: " << calculateBias(inputs,targets,a,k,b,usable) << endl;
        losses.push_back(currentError);
        accuracies.push_back(acc);
        as.push_back(a);
        ks.push_back(k);
        bs.push_back(b);

        if (currentError >= previousError) {
            patience--;
            if (patience < 0)break;
        }
        else { patience = 5; }
        if (currentError < minError) {
            minError = currentError;
            bestA = a;
            bestK = k;
            bestB = b;
        }
        previousError = currentError;
    }
    ofstream log("Logs/OptimierungsLog.txt");
    log << "Loss,Accuracy,a,k,b\n";
    for (int i = 0; i < losses.size(); i++) {
        log << losses[i] << "," << accuracies[i] << "," << as[i] << "," << ks[i] << "," << bs[i] << "\n";
    }
    log.close();
    cout << sepLine << "Optimierung abgeschlossen mit besten Parametern\na = " + to_string(bestA) + "\nk = " + to_string(bestK) + "\nb = " + to_string(bestB) << endl;
    cout << "Der Optimierungsverlauf (Diagnosedaten) sind in \"OptimierungsLog.txt\" zu finden." << endl;
    cout << "Aktuell - MSE: " << error(inputs,targets,bestA,bestK,bestB,usable) << ", Genauigkeit: " << accuracy(inputs, targets, bestA, bestK, bestB, usable) << endl;
    cout << "Vor Training - MSE: " << error(inputs, targets, aExponentialFactor, kExponentialFactor, biasExponential, usable) << ", Genauigkeit: " << accuracy(inputs, targets, aExponentialFactor, kExponentialFactor, biasExponential, usable) << sepLine;
    cout << "Behalten? Vorhandene Analysedaten werden angepasst." << endl << "1 = JA, 2 = NEIN" << endl;
    int choice = 0;
    cin >> choice;
    if (choice == 1) {
        aExponentialFactor = bestA;
        kExponentialFactor = bestK;
        biasExponential = bestB;
        refreshAnalytics(inputs);
        overwriteAnalytics();
        writeData();
        cout << "Optimierung fertiggestellt, neue Parameter gespeichert." << endl;
    }
    else {
        cout << "Keine Aenderungen vorgenommen. Kehre zurueck zum Menu." << endl;
    }
}

void gradient(vector<float> probs, vector<float> actual, float a, float k, float b, vector<bool> use) {
    aGrad = 0;
    kGrad = 0;
    bGrad = 0;
    int used = 0;
    for (int i = 0; i < probs.size(); i++) {
        if (use[i]) {
            float x = probs[i];
            float y = actual[i];
            aGrad += a * expf(2.0f * k * x) + b * expf(k * x) + y * expf(k * x);
            kGrad += a * x * (a * expf(2 * k * x) + expf(k * x) - y * expf(k * x));
            bGrad += a * expf(k * x) + b - y;
            used++;
        }
    }
    aGrad /= (float)used;
    kGrad /= (float)used;
    bGrad /= (float)used;
}

float error(vector<float> probs, vector<float> actual, float a, float k, float bias, vector<bool> use) {
    float sum = 0;
    int count = 0;
    for (int i = 0; i < probs.size(); i++) {
        if (use[i]) {
            sum += powf(actual[i] - Calc::xGfromProbability(probs[i], a, k, bias), 2);
            count++;
        }
    }
    return sum / (float)count;
}

