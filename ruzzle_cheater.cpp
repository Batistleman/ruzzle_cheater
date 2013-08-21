#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Struct to hold a point
struct Point2D{
    int x;
    int y;
};

// Global vars
char _fixedMap[4][4];
Point2D _pointMap[4][4];

// Function defs
void Init(string values);
vector<Point2D> Find(char* word);
vector<Point2D> FindLetter(char letter);
vector<Point2D> FindNext(vector<Point2D> pathSoFar, char* word);
vector<Point2D> FindNextLetter(Point2D currentPos, char letter, char localMap[4][4]);
void MoveOnPath(vector<Point2D> path);
string StartMovement(int x, int y);
string InterMovement(int x, int y);
string EndMovement();

// this function compares two strings and sorts them
bool myComparer (string a,string b) { return (a.size()>b.size()); }

int main()
{

    string input;
    cout << "please type your to crack key" << endl;
    cin >> input;

    string values = "gbbeieodslertjnr";

    cout << " >" << input << "< "<< endl;


    cout << "Checking the input value and copying" << endl;
    cout << "Length of the input string: " << input.size() << endl;
    if(input.size() == 16)
        values = input.c_str();

    //gbbe
    //ieod
    //sler
    //tjnr

    cout << "Initializing" << endl;
    Init(values);
    ifstream fin("dic.txt");

    string worda;

    vector<string> found_words;

    cout << "Hello" << endl;
    int words_found = 0;
    while(getline(fin, worda))
    {

        unsigned pos = worda.find("/");

        if(pos > 0 && pos < worda.size()){
            worda.resize(pos);

            vector<Point2D> path;
            path = Find((char*)worda.c_str());

            if(path.size() != 0)
            {
                if(std::find(found_words.begin(), found_words.end(), worda) != found_words.end()) {
                        cout << "double found" << endl;
                    } else {
                        found_words.push_back(worda);
                    }


                //cout << worda;// << endl;
                //MoveOnPath(path);
                //cout << endl;
                words_found++;
                //break;
            }
        }
    }
    cout << "Total words found:" << words_found << endl;

    sort(found_words.begin(), found_words.end(), myComparer);

    for(int i = 0; i< found_words.size();i++)
    {
        cout << found_words[i];// << endl;

        vector<Point2D> path;
        path = Find((char*)found_words[i].c_str());
        MoveOnPath(path);
        cout << endl;
    }


    cin.get();

    return 0;
}

// Send the commands to your android phone --------------------------------------------------------------------------------------------
void SendCommands(string comm)
{
    stringstream commands;

    commands << "adb shell \"" << comm << "\"";

    system(commands.str().c_str());
}

// To initialize the fixed map of characters -----------------------------------------------------------------------------------------
void Init(string values)
{
    for(int i = 0; i < 4; i++){
        for(int j = 0;j<4;j++)
        {
            _fixedMap[i][j] = values[j+4*i];
        }
    }

    // And initialize the coordinate map of your phone.
    // These are the coords for a S4. (This is a big screen, so your coordinates might be different.)
    int offset_x = 650;
    int offset_y = 150;
    int span_x = 925-offset_x;
    int span_y = 400 -offset_y;

    for(int i = 0; i < 4; i++){
        for(int j = 0;j<4;j++)
        {
            _pointMap[i][j].x = offset_x + span_x*i;
            _pointMap[i][j].y = offset_y + span_y*j;
        }
    }
}

// The 'Find' commands ----------------------------------------------------------------------------------------------------------------
vector<Point2D> Find(char* word)
{
    vector<Point2D> startpoints;

    startpoints = FindLetter(word[0]);

    for(int i = 0; i<startpoints.size(); i++)
    {
        vector<Point2D> path;
        path.push_back(startpoints[i]);

        vector<Point2D> finalPath;
        finalPath = FindNext(path, word);

        if (finalPath.size() > 0)
            return finalPath;
    }

    vector<Point2D> n;
    return n;
}

vector<Point2D> FindLetter(char letter)
{
    vector<Point2D> points;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (_fixedMap[i][j] == letter){
                Point2D pt;
                pt.x = i;
                pt.y = j;
                points.push_back(pt);
            }
        }
    }

    return points;
}

vector<Point2D> FindNext(vector<Point2D> pathSoFar, char* word)
{
    char localMap[4][4];
    for(int i = 0; i < 4; i++){
        for(int j = 0;j<4;j++)
        {
            localMap[i][j] = _fixedMap[i][j];
        }
    }


    for (int i = 0; i < pathSoFar.size(); i++)
    {
        localMap[pathSoFar[i].x][pathSoFar[i].y] = '#';
    }

    vector<Point2D> nexts = FindNextLetter(pathSoFar.back(), word[pathSoFar.size()], localMap);

    if (nexts.size() == 0)
        return nexts;

    pathSoFar.push_back(nexts[0]);

    if (pathSoFar.size() == strlen(word))
        return pathSoFar;

    return FindNext(pathSoFar, word);
}

vector<Point2D> FindNextLetter(Point2D currentPos, char letter, char localMap[4][4])
{
    vector<Point2D> points;
    for (int i = currentPos.x - 1; i <= currentPos.x + 1; i++)
    {
        if (i < 0 || i > 3)
            continue;

        for (int j = currentPos.y - 1; j <= currentPos.y + 1; j++)
        {
            if (j < 0 || j > 3)
                continue;

            if (localMap[i][j] == letter){
                Point2D pt;
                pt.x = i;
                pt.y = j;
                points.push_back(pt);
            }
        }
    }

    return points;
}


// Movement commands ------------------------------------------------------------------------------------------------------------------
void MoveOnPath(vector<Point2D> path)
{
    stringstream commands;

    for(int i = 0; i< path.size();i++)
    {
        if(i==0){
            commands << StartMovement(_pointMap[path[i].x][path[i].y].y,_pointMap[path[i].x][path[i].y].x);

            SendCommands(commands.str().c_str());
            commands.str(std::string());
        }
        else
        {
            // Due to limited lenght of the adb shell commands we need to send this now and then in between
            if(commands.str().size() > 800){
                SendCommands(commands.str().c_str());
                commands.str(std::string());
            }

            commands <<  InterMovement(_pointMap[path[i].x][path[i].y].y,_pointMap[path[i].x][path[i].y].x);

        }
        cout << "(" << path[i].x << ", " << path[i].y << ")";
    }

    SendCommands(commands.str().c_str());
    commands.str(std::string());

    commands << EndMovement();

    SendCommands(commands.str().c_str());
}

string StartMovement(int x, int y)
{
    stringstream commands;

    // These commands start the swipe movement
    commands << "sendevent /dev/input/event3 0003 57 50781;";
    commands << "sendevent /dev/input/event3 0001 330 1;";

    // These are the location commands
    commands << "sendevent /dev/input/event3 0003 53 " << x << ";";
    commands << "sendevent /dev/input/event3 0003 54 " << y << ";";

    // These are related to the thickness of the push or something like that
    commands << "sendevent /dev/input/event3 0003 50 3;";
    commands << "sendevent /dev/input/event3 0003 60 45;";

    // This is the 'end command train command'
    commands << "sendevent /dev/input/event3 0000 0 0;";

    return commands.str();
}

string InterMovement(int x, int y)
{
    stringstream commands;

    // These are the location commands
    commands << "sendevent /dev/input/event3 0003 53 " << x << ";";
    commands << "sendevent /dev/input/event3 0003 54 " << y << ";";

    // These are related to the thickness of the push or something like that
    commands << "sendevent /dev/input/event3 0003 48 5;";
    commands << "sendevent /dev/input/event3 0003 50 5;";

    // This is the 'end command train command'
    commands << "sendevent /dev/input/event3 0000 0 0;";

    return commands.str();
}

string EndMovement()
{
    stringstream commands;

    // These are the commands to finish the swipe gesture
    commands << "sendevent /dev/input/event3 0003 57 4294967295;";
    commands << "sendevent /dev/input/event3 0001 330 0;";

    // This is the 'end command train command'
    commands << "sendevent /dev/input/event3 0000 0 0;";

    return commands.str();
}