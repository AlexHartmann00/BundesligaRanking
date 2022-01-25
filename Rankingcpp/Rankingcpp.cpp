/*// Rankingcpp.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//


#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <tgmath.h>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

float winProb[2][4];

int gkID[1], defID[6], midID[6], attID[6];
int linedUpID[2][4][6];
// looping necessities
int y[11];
int found = 0;
int index = 0;
float temp1, temp2;
float probValue[2][4];
float normProb[3], sdRange, sdStart, sdEnd;
int betterTeamID, other;

string clubs[18] = {"FC Bayern Munchen", "RB Leipzig", "VfL Wolfsburg", "Eintracht Frankfurt", "Bayer 04 Leverkusen", "Borussia Dortmund", "1.FC Union Berlin", "SC Freiburg", "VfB Stuttgart", "Borussia Monchengladbach", "TSG 1899 Hoffenheim", "SV Werder Bremen", "FC Augsburg", "1.FC Koln", "Hertha BSC Berlin", "Arminia Bielefeld", "1.FSV Mainz 05", "FC Schalke 04"};
int activeTeamID[2];
vector<string> playerName[18];
int teamPlayers[18];
vector<float> playerValue[18];
float teamValues[18];
float matchValues[2];
float matchValuesTeamPosition[2][4];
float sumValues = 0;
float average = 0;
int teamCount = 2;
int playerCount = 30;
int lineupCount = 11;
string positions[4] = { "Torhuter", "Verteidiger", "Mittelfeldspieler", "Angreifer" };
int positionPlayers[18][30];

float xG[2];
float sd[2];
float xGdif[2];
float Gdif[2];

int linedUpA[11];
int linedUpB[11];

int goals[2];
int result; //0,1,2(home,draw,away)
float probA, probB;
float changeA, changeB;

float x;
float test[18*30];
string n;
string tempNames[18 * 30];
string sepLine = "\n-------------------- \n";

string check;
int tempPos = 0;
int playersSelectedSoFar;
int positionSelections[2][4];
int equals = 0;
int input;
float changeTeamPosition[2][4];
float defValue[2];
float debugArray[2][4];
double debugDoubleArray[2][4];

// new file system arrays
vector<int> uniqueID;
vector<string> names;
vector<int> team;
vector<float> values;
int squadIDs[18][100];
int playerCountperTeam[100];
int inputX;
string nameInput;
float valueInput;
float previousValues[18][2];
float normalProbPrevious[3];
//coaches
vector<int> cuniqueID;
vector<string> cnames;
vector<int> cteam;
vector<float> cvalues;
string cnameInput;

//settings:
float homeAdv = 0.075f;
float homeFac = 1.0000f + homeAdv;
float awayFac = 1.0000f - homeAdv;
float changeConstant = 20.0f;

class Coach {
public:
    int id;
    string name;
    int team;
    float value;

};

class Player {
public:
    int id;
    string name;
    int team;
    float value;
};

class Club {
public:
    int id;
    string name;
    vector<Player> players;
    Coach Trainer;
    

    void AddPlayer(string name, float value) {
        Player p;
        p.id = players.size();
        p.name = name;
        p.team = id;
        p.value = value;
        players.push_back(p);
    }

    void RemovePlayer(int ID) {
        players.erase(players.begin() + ID);
    }

    void NewCoach(string name) {
        Coach c;
        c.team = id;
        c.name = name;
        c.value = 2000;
        Trainer = c;
    }
};

void WriteNewDataSystem() {
    int kk = 0;
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < playerCountperTeam[i]; j++) {
            if (playerName[i][j] != "null") {
                uniqueID.push_back(kk);
                names.push_back(playerName[i][j]);
                team.push_back(i);
                values.push_back(playerValue[i][j]);
                kk++;
            }
        }
    }
    ofstream cdata("coachdata.txt", 5);
    for (int i = 0; i < cnames.size(); i++) {
        cdata << cuniqueID[i] << "," << cnames[i] << "," << cteam[i] << "," << cvalues[i] << endl;
    }
    cdata.close();
    ofstream data("data.txt");
    for (int i = 0; i < team.size(); i++) {
        data << uniqueID[i] << "," << names[i] << "," << team[i] << "," << values[i] << endl;
    }
    data.close();
    ofstream pCount("playerCounts.txt");
    for (int i = 0; i < 18; i++) {
        pCount << playerCountperTeam[i] << endl;
    }
    pCount.close();
    ofstream prevValues("previousValues.txt");
    for (int i = 0; i < 18; i++) {
        prevValues << previousValues[i][0] << "," << previousValues[i][1] << endl;
    }
    prevValues.close();
}

void readData() {
    ifstream check("coachdata.txt");
    if (!check.good()) {
        ofstream checkk("coachdata.txt");
        checkk.close();
    }
    check.close();
    ifstream data("data.txt");
    int kk = 0;
    string x;
    string id, nam, tm, val;
    while (getline(data,x)) {
        stringstream ss(x);
        getline(ss, id, ',');
        getline(ss, nam, ',');
        getline(ss, tm, ',');
        getline(ss, val, ',');
        uniqueID.push_back(stoi(id));
        names.push_back(nam);
        team.push_back(stoi(tm));
        values.push_back(stof(val));
        kk++;
    }
    data.close();
    ifstream cdata("coachdata.txt");
    while (getline(data,x))
    {
        stringstream ss(x);
        getline(ss, id, ',');
        getline(ss, nam, ',');
        getline(ss, tm, ',');
        getline(ss, val, ',');
        cuniqueID.push_back(stoi(id));
        cnames.push_back(nam);
        cteam.push_back(stoi(tm));
        cvalues.push_back(stof(val));

    }
    cdata.close();

    for (int i = 0; i < team.size(); i++) {
        playerName[team[i]].push_back(names[i]);
        playerValue[team[i]].push_back(values[i]);
    }
    ifstream pCount("playerCounts.txt");
    for (int i = 0; i < 18; i++) {
        pCount >> playerCountperTeam[i];
    }
    pCount.close();
    uniqueID.clear();
    names.clear();
    team.clear();
    values.clear();
    ifstream prevValues("previousValues.txt");
    kk = 0;
    string frst, scnd;
    while (getline(prevValues, x)) {
        stringstream ss(x);
        getline(ss, frst, ',');
        getline(ss, scnd, ',');
        previousValues[kk][0] = stof(frst);
        previousValues[kk][1] = stof(scnd);
        kk++;
    }
    prevValues.close();
}

void printCoaches() {
    for (int i = 0; i < cnames.size(); i++) {
        cout << cuniqueID[i] << " " << cnames[i] << " " << clubs[cteam[i]] << " " << cvalues[i] << endl;
    }
}


void addCoaches() {
coachStart:;
    cout << "Team auswahlen, dem ein Trainer hinzugefugt werden soll:\n";
    for (int i = 0; i < 18; i++) {
        cout << i << ": " << clubs[i] << endl;
    }
    cin >> inputX;
    cout << "\nNamen des hinzuzufugenden Trainers eingeben:\n";
    cin >> cnameInput;
    cuniqueID.push_back(cuniqueID.size());
    cnames.push_back(cnameInput);
    cteam.push_back(inputX);
    cvalues.push_back(2000);
    cout << "Alle neuen aktuellen Trainer:" << sepLine;
    printCoaches();
    cout << "Weiteren Trainer hinzufugen?\n 1:Ja, 2:Speichern und Ende";
    cin >> inputX;
    if (inputX == 1) {
        goto coachStart;
    }
}


float normalDistributionProbability(float start, float end) {
    return 0.5f * erff(end / sqrt(2)) - 0.5f * erff(start / sqrt(2));
}
void calculateSDRangeandNormProb(float xg1 = 0.0f, float xg2 = 0.0f, int type = 0) {
    //--------------------------REGULAR CALCULATIONS--------------------
    //team with higher xG:
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
        cout << "Sieg " << clubs[activeTeamID[0]] << ": " << 100*normProb[0] << "\%" << endl;
        cout << "Unentschieden: " << 100*normProb[2] << "\%" << endl;
        cout << "Sieg " << clubs[activeTeamID[1]] << ": " << 100*normProb[1] << "\%" << endl;
    }
    else {
        //-------------------PREVIOUS CALCULATIONS--------------------
        float sd1 = xg1 * (1.0f - xg1 / 3.5f);
        float sd2 = xg2 * (1.0f - xg2 / 3.5f);
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
        //print probabilities:
        cout << "Wahrscheinlichkeiten:\n";
        cout << "Heimsieg " << ": " << normalProbPrevious[0] << endl;
        cout << "Unentschieden: " << normalProbPrevious[2] << endl;
        cout << "Auswartssieg " << ": " << normalProbPrevious[1] << endl;

    }
    
}


/*
void initializePlayers(int club) {
    string input;
    cout << "Spieler fur Verein -" << clubs[club] << "- hinzufugen. \"ende\" eingeben zum Beenden." << endl;
    int y = 0;
    while (true)
    {
        cin >> input;
        if (input == "ende") {
            teamPlayers[club] = y;

            goto initTwo;
            y = 0;
        }
        playerName[club][y] = input;
        y++;
    }
initTwo:
    for (int i = teamPlayers[club]; i < 30; i++) {
        playerName[club][i] = "null";
    }
    for (int i = 0; i <= y; i++) {
        cout << playerName[club][i] << endl;
    }
    cout << teamPlayers[club] << endl;
    ofstream out("teams.txt", 5);
    for (int i = 0; i < 30; i++) {
        out << playerName[club][i] << endl;
    }
    out.close();
    ofstream outP("playerValues.txt", 5);
    fill((float*)playerValue, (float*)playerValue + sizeof(playerValue) / sizeof(float), 2000);
    for (int i = 0; i < 30; i++) {
        outP << playerValue[club][i] << endl;
    }
    outP.close();

}

void Load() {
    ifstream inputFile("playerValues.txt");
    int u = 0;
    while (inputFile >> x)
    {
        test[u] = x;
        u++;
    }
    //cout << sum << "test: " << test[0] << ", " << test[1];
    for (int j = 0; j < 18; j++) {
        for (int i = 0; i < 30; i++) {
            playerValue[j].push_back(test[30 * j + i]);
        }
    }
    //for (int j = 0; j < playerCount * teamCount; j++) {
    //    playerValues[j] = test[j];
    //}
    inputFile.close();

    ifstream inputNames("teams.txt");
    int k = 0;
    while (inputNames >> n)
    {
        tempNames[k] = n;
        k++;
    }
    //cout << sum << "test: " << test[0] << ", " << test[1];
    for (int j = 0; j < 18; j++) {
        for (int i = 0; i < 30; i++) {
            playerName[j].push_back(tempNames[30 * j + i]);
        }
    }
    inputNames.close();
    for (int i = 0; i < 18; i++) {
        cout << sepLine << clubs[i] << ":" << sepLine;
        for (int j = 0; j < teamPlayers[i]; j++) {
            cout << playerName[i][j] << ", " << playerValue[i][j] << endl;
        }
    }
}




void printPlayers(int team) {
    cout << clubs[team] << ": \n";
    cout << "ID " << "Wert  " << " Name" << endl;
    for (int i = 0; i < playerCountperTeam[team]; i++) {
        cout << i << " " << setprecision(4) << playerValue[team][i] << " " << playerName[team][i] << endl;
    }
    cout << sepLine;
}

void saveAndQuit() {
    //ofstream  out2("playerValuesByClub.txt", 5);
    //out2.clear();
    //for (int i = 0; i < 30; i++) {
    //    for (int j = 0; j < 17; j++) {
    //        out2 << setprecision(6) << playerValue[j][i] << ", ";
    //    }
    //    out2 << setprecision(6) << playerValue[17][i] << endl;
    //}
    //out2.close();


    ofstream  out2("playerValuesByClub.txt", 5);
    out2.clear();
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 17; j++) {
            out2 << setprecision(6) << playerValue[j][i] << ", ";
        }
        out2 << setprecision(6) << playerValue[17][i] << endl;
    }
    out2.close();

    ofstream outPos("playerPositionsByClub.txt", 5);
    for(int i = 0; i < 30; i++){
        for (int j = 0; j < 17; j++) {
            outPos << positionPlayers[j][i] << ", ";
        }
        outPos << positionPlayers[17][i] << endl;
    }
    outPos.close();

    terminate();
}

void saveToAppendixFile() {
    ofstream  out2("playerValuesByClub.txt", 5);
    out2.clear();
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 1; j++) {
            out2 << setprecision(6) << playerValue[j][i] << ", ";
        }
        out2 << setprecision(6) << playerValue[17][i] << endl;
    }
    out2.close();

    ofstream outPos("playerPositionsByClub.txt", 5);
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 17; j++) {
            outPos << positionPlayers[j][i] << ", ";
        }
        outPos << positionPlayers[17][i] << endl;
    }
    outPos.close();

}



void getResult() {
    for (int i = 0; i < 2; i++) {
        cout << "Tore Team " << (i + 1) << ": " << endl;
        cin >> goals[i];
    }
    if (goals[0] > goals[1]) {
        result = 0;
    }
    else if (goals[0] == goals[1]) {
        result = 1;
    }
    else {
        result = 2;
    }
    cout << sepLine << "Ergebnis:\n" << clubs[activeTeamID[0]] << " " << goals[0] << " : " << goals[1] << " " << clubs[activeTeamID[1]] << sepLine;
}

float calc(float a, float b) {
    temp1 = b - a;
    temp2 = temp1 / 150.0;
    return 1.0f / (1.0f + pow(10.0f, temp2));
}

void calculateWinProbabilities() {

    winProb[0][0] = calc(probValue[0][0], probValue[1][3]);
    winProb[0][1] = calc(probValue[0][1], probValue[1][3]);
    winProb[0][2] = calc(probValue[0][2], probValue[1][2]);
    winProb[0][3] = calc(probValue[0][3], defValue[1]);
    winProb[1][0] = calc(probValue[1][0], probValue[0][3]);
    winProb[1][1] = calc(probValue[1][1], probValue[0][3]);
    winProb[1][2] = calc(probValue[1][2], probValue[0][2]);
    winProb[1][3] = calc(probValue[1][3], defValue[0]);


    // expected goals
    xG[0] = winProb[0][3] * 3.5f * homeFac;
    sd[0] = sqrt(xG[0] * (1 - winProb[0][3]));
    xG[1] = winProb[1][3] * 3.5f * awayFac;
    sd[1] = sqrt(xG[1] * (1 - winProb[1][3]));
    xGdif[0] = xG[0] - xG[1];
    xGdif[1] = xG[1] - xG[0];
    cout << "Erwartetes Ergebnis:\n" << clubs[activeTeamID[0]] << " " << xG[0] << " : " << xG[1] << " " << clubs[activeTeamID[1]] << endl;

    //Confidence interval checks
    calculateSDRangeandNormProb();
}

void calculateValueChange(int result) {
    /*if (goals[0] < 1.5f) {
        changeTeamPosition[0][3] = -(winProb[0][3]) * changeConstant * (1 - 0.5f * goals[0]);
        changeTeamPosition[1][0] = (1 - winProb[1][0]) * changeConstant * (1 - 0.5f * goals[0]);
        changeTeamPosition[1][1] = (1 - winProb[1][1]) * changeConstant * (1 - 0.5f * goals[0]);
    }
    if (goals[1] < 1.5f) {
        changeTeamPosition[1][3] = -(winProb[1][3]) * changeConstant * (1 - 0.5f * goals[1]);
        changeTeamPosition[0][0] = (1 - winProb[0][0]) * changeConstant * (1 - 0.5f * goals[1]);
        changeTeamPosition[0][1] = (1 - winProb[0][1]) * changeConstant * (1 - 0.5f * goals[1]);
    }
    if (goals[0] > 1.5f) {
        changeTeamPosition[0][3] = (1 - winProb[0][3]) * changeConstant * (0.5f * (goals[0] - 1));
        changeTeamPosition[1][0] = (winProb[1][0]) * changeConstant * (-0.5f * (goals[0] - 2));
        changeTeamPosition[1][1] = (winProb[1][1]) * changeConstant * (-0.5f * (goals[0] - 2));
    }
    if (goals[1] > 1.5f) {
        changeTeamPosition[1][3] = (1 - winProb[1][3]) * changeConstant * (0.5f * (goals[1] - 1));
        changeTeamPosition[0][0] = (winProb[0][0]) * changeConstant * (-0.5f * (goals[1] - 2));
        changeTeamPosition[0][1] = (winProb[0][1]) * changeConstant * (-0.5f * (goals[1] - 2));
    }
    int goalDif = goals[0] - goals[1];
    if (result == 0) {
        changeTeamPosition[1][2] = -(winProb[1][2]) * changeConstant * (0.5f * goalDif);
        changeTeamPosition[0][2] = (1 - winProb[0][2]) * changeConstant * (0.5f * goalDif);
    }
    else if (result == 2) {
        changeTeamPosition[1][2] = -(1 - winProb[1][2]) * changeConstant * (0.5f * goalDif);
        changeTeamPosition[0][2] = (winProb[0][2]) * changeConstant * (0.5f * goalDif);
    }

    //NEW VERSION::

    Gdif[0] = goals[0] - goals[1];
    Gdif[1] = goals[1] - goals[0];

    //team 1 tore vergleich
    if (xG[1] < goals[1]) {
        changeTeamPosition[0][0] = winProb[0][0] * changeConstant * (xG[1] - goals[1]);
        changeTeamPosition[0][1] = winProb[0][1] * changeConstant * (xG[1] - goals[1]);
        changeTeamPosition[1][3] = (1 - winProb[1][3]) * changeConstant * (goals[1] - xG[1]);

    }
    else {
        changeTeamPosition[0][0] = (1 - winProb[0][0]) * changeConstant * (xG[1] - goals[1]);
        changeTeamPosition[0][1] = (1 - winProb[0][1]) * changeConstant * (xG[1] - goals[1]);
        changeTeamPosition[1][3] = winProb[1][3] * changeConstant * (goals[1] - xG[1]);

    }
    //mid team 0
    if (Gdif[0] < xGdif[0]) {
        changeTeamPosition[0][2] = winProb[0][2] * changeConstant * (Gdif[0] - xGdif[0]);

    }
    else {
        changeTeamPosition[0][2] = (1 - winProb[0][2]) * changeConstant * (Gdif[0] - xGdif[0]);

    }
    //mid team 1
    if (Gdif[1] < xGdif[1]) {
        changeTeamPosition[1][2] = winProb[1][2] * changeConstant * (Gdif[1] - xGdif[1]);

    }
    else {
        changeTeamPosition[1][2] = (1 - winProb[1][2]) * changeConstant * (Gdif[1] - xGdif[1]);

    }
    //team 0 tore vergleich
    if (goals[0] < xG[0]) {
        changeTeamPosition[0][3] = winProb[0][3] * changeConstant * (goals[0] - xG[0]);
        changeTeamPosition[1][0] = (1 - winProb[1][0]) * changeConstant * (xG[0] - goals[0]);
        changeTeamPosition[1][1] = (1 - winProb[1][1]) * changeConstant * (xG[0] - goals[0]);

    }
    else {
        changeTeamPosition[0][3] = (1 - winProb[0][3]) * changeConstant * (goals[0] - xG[0]);
        changeTeamPosition[1][0] = winProb[1][0] * changeConstant * (xG[0] - goals[0]);
        changeTeamPosition[1][1] = winProb[1][1] * changeConstant * (xG[0] - goals[0]);

    }

    // save previous values for quick calculations
    previousValues[activeTeamID[0]][0] = defValue[0] + 0.2f * changeTeamPosition[0][0] + 0.6f * changeTeamPosition[0][1] + 0.2f * changeTeamPosition[0][2];
    previousValues[activeTeamID[0]][1] = probValue[0][3] + 0.3f * changeTeamPosition[0][2] + 0.7f * changeTeamPosition[0][3];
    previousValues[activeTeamID[1]][0] = defValue[1] + 0.2f * changeTeamPosition[1][0] + 0.6f * changeTeamPosition[1][1] + 0.2f * changeTeamPosition[1][2];
    previousValues[activeTeamID[1]][1] = probValue[1][3] + 0.3f * changeTeamPosition[1][2] + 0.7f * changeTeamPosition[1][3];





    //DEBUG:
    for (int t = 0; t < 2; t++) {
        for (int p = 0; p < 4; p++) {
            //cout << sepLine << changeTeamPosition[t][p] << sepLine;
        }
    }



}

void selectLineup(int team) {
    playersSelectedSoFar = 0;
    tempPos = 0;
    for (int i = 0; i < 6; i++) {
        positionSelections[team][i] = 0;
    }
    for (int i = 0; i < playerCountperTeam[activeTeamID[team]]; i++) {
        positionPlayers[activeTeamID[team]][i] = 0;
    }
nextpos:
    cout << positions[tempPos] << " fur Team " << team << " auswahlen: (\"99\" eingeben um zu nachster Position zu wechseln)" << endl;
    while (playersSelectedSoFar < 11) {
        cin >> input;
        if (input == 99) {
            tempPos += 1;
            goto nextpos;
        }
        linedUpID[team][tempPos][positionSelections[team][tempPos]] = input;
        positionSelections[team][tempPos] += 1;
        playersSelectedSoFar += 1;
        positionPlayers[activeTeamID[team]][input] = tempPos + 1;
    }

}

void printLineups() {
    cout << sepLine;
    cout << "Aufstellung fur " << clubs[activeTeamID[0]] << ": " << endl;
    for (int p = 0; p < 4; p++) {
        for (int i = 0; i < positionSelections[0][p]; i++) {
            cout << playerName[activeTeamID[0]][linedUpID[0][p][i]] << endl;
        }
    }

    cout << "Aufstellung fur " << clubs[activeTeamID[1]] << ": " << endl;
    for (int p = 0; p < 4; p++) {
        for (int i = 0; i < positionSelections[1][p]; i++) {
            cout << playerName[activeTeamID[1]][linedUpID[1][p][i]] << endl;
        }
    }

}

void calculateMatchValues() {
    for (int t = 0; t < 2; t++) {
        matchValues[t] = 0;
        for (int p = 0; p < 4; p++) {
            sumValues = 0;
            for (int i = 0; i < positionSelections[t][p]; i++) {
                sumValues += playerValue[activeTeamID[t]][linedUpID[t][p][i]];
            }
            matchValuesTeamPosition[t][p] = sumValues / positionSelections[t][p];
            cout << "matchValues team " << t << ", Position " << p << ": " << matchValuesTeamPosition[t][p] << endl;
            matchValues[t] += matchValuesTeamPosition[t][p] * (positionSelections[t][p] / (float)11);

        }
    }


    //team 1 final position values
    probValue[0][0] = matchValuesTeamPosition[0][0] * 0.6f + matchValuesTeamPosition[0][1] * 0.3f + matchValuesTeamPosition[0][2] * 0.1f;
    probValue[0][1] = matchValuesTeamPosition[0][0] * 0.2f + matchValuesTeamPosition[0][1] * 0.6f + matchValuesTeamPosition[0][2] * 0.2f;
    probValue[0][2] = matchValuesTeamPosition[0][1] * 0.2f + matchValuesTeamPosition[0][2] * 0.6f + matchValuesTeamPosition[0][3] * 0.2f;
    probValue[0][3] = matchValuesTeamPosition[0][2] * 0.3f + matchValuesTeamPosition[0][3] * 0.7f;
    defValue[0] = probValue[0][1];
    //team 2 final position values
    probValue[1][0] = matchValuesTeamPosition[1][0] * 0.6f + matchValuesTeamPosition[1][1] * 0.3f + matchValuesTeamPosition[1][2] * 0.1f;
    probValue[1][1] = matchValuesTeamPosition[1][0] * 0.2f + matchValuesTeamPosition[1][1] * 0.6f + matchValuesTeamPosition[1][2] * 0.2f;
    probValue[1][2] = matchValuesTeamPosition[1][1] * 0.2f + matchValuesTeamPosition[1][2] * 0.6f + matchValuesTeamPosition[1][3] * 0.2f;
    probValue[1][3] = matchValuesTeamPosition[1][2] * 0.3f + matchValuesTeamPosition[1][3] * 0.7f;
    defValue[1] = probValue[1][1];

    //DEBUG:
    cout << "Value debug: " << sepLine;
    for (int i = 0; i < 2; i++) {
        cout << "Werte fur " << clubs[activeTeamID[i]] << sepLine;
        for (int j = 0; j < 4; j++) {
            cout << positions[j] << ": " << probValue[i][j] << endl;
        }
    }

    //for (int j = 0; j < lineupCount; j++) {
    //    sumValues += playerValue[activeTeamID[0]][linedUpA[j]];
    //}
    //matchValues[0] = sumValues / lineupCount;

    //sumValues = 0;
    //for (int j = 0; j < lineupCount; j++) {
    //    sumValues += playerValue[activeTeamID[1]][linedUpB[j]];
    //}
    //matchValues[1] = sumValues / lineupCount;

    for (int i = 0; i < 2; i++) {
        cout << endl << "Spieltagswert " << clubs[activeTeamID[i]] << ": " << matchValues[i] << endl;
    }
}

void applyValueChanges() {
    for (int t = 0; t < 2; t++) {
        for (int p = 0; p < 4; p++) {
            for (int i = 0; i < positionSelections[t][p]; i++) {
                playerValue[activeTeamID[t]][linedUpID[t][p][i]] += changeTeamPosition[t][p];
                cout << "Wert von " << playerName[activeTeamID[t]][linedUpID[t][p][i]] << " um " << changeTeamPosition[t][p] << " auf " << playerValue[activeTeamID[t]][linedUpID[t][p][i]] << " geandert." << endl;
            }
        }
    }
}

void writeToFile() {
    ofstream out("playerValues.txt");
    out.clear();
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 30; j++) {
            out << setprecision(6) << playerValue[i][j] << endl;
        }

    }

    //for (int i = 0; i < playerCount * teamCount; i++) {
    //    out << setprecision(6) << playerValues[i] << endl;
    //}
    out.close();
}

void downgradeSubstitutes() {
    cout << "Auswechselspieler:\n" << sepLine;
    for (int t = 0; t < 2; t++) {
        index = 0;
        for (int p = 0; p < 4; p++) {
            for (int i = 0; i < positionSelections[t][p]; i++) {
                //cout << "y of " << index << " = " << linedUpID[t][p][i] << endl;
                y[index] = linedUpID[t][p][i];
                index++;
            }
        }

        //for (int i = 0; i < 11; i++) {
            //cout << "index " << i << " = " << y[i] << endl;
        //}

        for (int i = 0; i < playerCountperTeam[activeTeamID[t]]; i++) {
            found = 0;
            for (int j = 0; j < 11; j++) {
                if (i == y[j]) {
                    found++;
                    //cout << "\n found ++ auf " << found << endl;
                    goto endLoop;
                }
            }
            if (found == 0 && goals[t] > goals[-t + 1]) {
                float mmm = playerValue[activeTeamID[t]][i] * 0.0005f;
                float nnn = mmm * 1999;
                playerValue[activeTeamID[t]][i] *= 0.9995f;

                cout << "Wert von " << playerName[activeTeamID[t]][i] << " um " << mmm << " auf " << nnn << " verringert\n";
            }
            else if (found == 0) {
                float mmm = playerValue[activeTeamID[t]][i] * 0.001f;
                float nnn = mmm * 999;
                playerValue[activeTeamID[t]][i] *= 0.999f;

                cout << "Wert von " << playerName[activeTeamID[t]][i] << " um " << mmm << " auf " << nnn << " verringert\n";

            }
        endLoop:;
        }

    }
}

void saveAnalyticdata() {
    ofstream analytics("analytics.txt", 5);
    analytics << activeTeamID[0] << ", " << activeTeamID[1] << ", " << xG[0] << ", " << xG[1] << ", " << normProb[0] << ", " << normProb[2] << ", " << normProb[1] << ", " << goals[0] << ", " << goals[1] << endl;
    analytics.close();
}

void AddPlayer() {
    cout << "Team auswahlen:\n";
    for (int i = 0; i < 18; i++) {
        cout << i << ": " << clubs[i] << endl;
    }
    cin >> inputX;
    printPlayers(inputX);
    cout << "\nNamen des hinzuzufugenden Spielers eingeben:";
    cin >> nameInput;
    cout << "\nWert des hinzuzufugenden Spielers eingeben;";
    cin >> valueInput;
    playerCountperTeam[inputX] ++;
    playerName[inputX].push_back(nameInput);
    playerValue[inputX].push_back(valueInput);
    WriteNewDataSystem();
    printPlayers(inputX);
}

void RemovePlayer() {
rmPlayerStart:;
    cout << "Team auswahlen:\n";
    int temptm;
    for (int i = 0; i < 18; i++) {
        cout << i << ": " << clubs[i] << endl;
    }
    cin >> temptm;
    printPlayers(temptm);
    cout << "Index des zu entfernenden Spielers eingeben:\n";
    cin >> inputX;
    uniqueID.erase(uniqueID.begin() + inputX);
    names.erase(names.begin() + inputX);
    team.erase(team.begin() + inputX);
    values.erase(values.begin() + inputX);
    playerCountperTeam[temptm] --;
    cout << "Spieler entfernt\n";
    WriteNewDataSystem();
    printPlayers(inputX);
    cout << "Weiteren Spieler entfernen?\ 1:Ja, 2:Ende\n";
    cin >> inputX;
    if (inputX == 1) {
        goto rmPlayerStart;
    }
}

void GameDayOdds() {
    int matches[9][2];
    float xGmatches[9][2];
    for (int i = 0; i < 18; i++) {
        cout << i << ": " << clubs[i] << endl;
    }
    for (int i = 0; i < 9; i++) {
        cout << "Begegnung " << i << " festlegen:\n";
        cout << "Team 1:" << endl;
        cin >> matches[i][0];
        cout << "Team 2:" << endl;
        cin >> matches[i][1];
    }
    cout << "Vorrausgesagte Ergebnisse:" << sepLine;
    for (int i = 0; i < 9; i++) {
        xGmatches[i][0] = round(homeFac * 3.5f * calc(previousValues[matches[i][0]][1], previousValues[matches[i][1]][0]));
        xGmatches[i][1] = round(awayFac * 3.5f * calc(previousValues[matches[i][1]][1], previousValues[matches[i][0]][0]));
        cout << clubs[matches[i][0]] << " " << xGmatches[i][0] << " : " << xGmatches[i][1] << " " << clubs[matches[i][1]] << endl;
        calculateSDRangeandNormProb(homeFac * 3.5f * calc(previousValues[matches[i][0]][1], previousValues[matches[i][1]][0]), awayFac * 3.5f * calc(previousValues[matches[i][1]][1], previousValues[matches[i][0]][0]), 1);
        cout << sepLine;
    }
    cout << "In Textdatei speichern?\n" << "1: Ja, 2: Nein";
    int in;
    cin >> in;
    // save gameday data
    if (in == 1) {
        ofstream out("GamedayPredictions.txt");
        for (int i = 0; i < 9; i++) {
            xGmatches[i][0] = round(homeFac * 3.5f * calc(previousValues[matches[i][0]][1], previousValues[matches[i][1]][0]));
            xGmatches[i][1] = round(awayFac * 3.5f * calc(previousValues[matches[i][1]][1], previousValues[matches[i][0]][0]));
            out << clubs[matches[i][0]] << " " << xGmatches[i][0] << " : " << xGmatches[i][1] << " " << clubs[matches[i][1]] << endl;
            calculateSDRangeandNormProb(homeFac * 3.5f * calc(previousValues[matches[i][0]][1], previousValues[matches[i][1]][0]), awayFac * 3.5f * calc(previousValues[matches[i][1]][1], previousValues[matches[i][0]][0]), 1);
            out << "Wahrscheinlichkeiten:\n";
            out << "Sieg " << clubs[matches[i][0]] << ": " << setprecision(6) << 100.0f * normalProbPrevious[0] << "\%" << endl;
            out << "Unentschieden: " << setprecision(6) << 100.0f * normalProbPrevious[2] << "\%" << endl;
            out << "Sieg " << clubs[matches[i][1]] << ": " << setprecision(6) << 100.0f * normalProbPrevious[1] << "\%" << sepLine;
        }

    }
}


void DisplayMenu() {
menuStart:;
    cout << sepLine << "MENU" << sepLine;
    cout << "Eine der Optionen auswahlen:\n";
    cout << "1. Spiel eintragen\n"
        << "2. Spieler hinzufugen\n"
        << "3. Spieler entfernen\n"
        << "4. Trainer hinzufugen\n"
        << "5. Trainer entfernen\n"
        << "6. Spieltagswettquoten\n";
    cin >> inputX;
    if (inputX == 2) {
        AddPlayer();
        WriteNewDataSystem();
        goto menuStart;
    }
    if (inputX == 3) {
        RemovePlayer();
        WriteNewDataSystem();
        goto menuStart;
    }
    if (inputX == 4) {
        addCoaches();
        WriteNewDataSystem();
        goto menuStart;
    }
    if (inputX == 6) {
        GameDayOdds();
        goto menuStart;
    }
}

int main()
{
start:

    readData();
    DisplayMenu();
    newMatch();
    printPlayers(activeTeamID[0]);
    printPlayers(activeTeamID[1]);
    selectLineup(0);
    selectLineup(1);
    printLineups();
    calculateMatchValues();
    calculateWinProbabilities();
    getResult();//last user input
    calculateValueChange(result);
    applyValueChanges();
    saveAnalyticdata();
    downgradeSubstitutes();
    writeToFile();
    WriteNewDataSystem();

    cout << endl << "finished" << sepLine;
    goto start;
}
*/