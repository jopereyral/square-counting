#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>

using namespace std;

typedef map<int, map<int,int> > track2userScore;

track2userScore getHashmap(ifstream &file)
{
    track2userScore tracks;
    string line;
    int userId, nRatings, i, trackId, score;
    while(getline(file, line))
    {
        sscanf(line.c_str(), "%d|%d", &userId, &nRatings);

        for(i = 0; i < nRatings; i++) {
            getline(file, line);
            sscanf(line.c_str(), "%d %d", &trackId, &score);
            tracks[trackId][userId] = score;
        }
    }

    return tracks;
}

int getLoveRatingValue(int score) {
    if (score > 80) {
        return 1;
    }
    return 0;
}

void squareCounting(ifstream &file, track2userScore tracks)
{
    int confSquares[10] = {0,0,0,0,0,0,0,0,0,0};
    string line;
    queue<int> q;
    char squareConfInBinary[3];
    ofstream instanceMatrix("./data/instance-test.txt", ios::out);

    int userTarget, nRatings, i, itemTarget, score, j, item,a,b,c, confValue;

    while(getline(file, line))
    {
        sscanf(line.c_str(), "%d|%d", &userTarget, &nRatings);
        int items[nRatings], scores[nRatings];
        for (i = 0; i < nRatings; i++) {
            getline(file, line);
            sscanf(line.c_str(), "%d %d", &item, &score);
            items[i] = item;
            scores[i] = score;
        }
        confSquares[0] = userTarget;
        // cout << nRatings << endl;
        //this for is for generate every row of the instance matrix
        for (i = 0; i < nRatings; i++) {
            itemTarget = items[i];
            confSquares[9] = getLoveRatingValue(scores[i]);
            //count squares
            for (i = 0; i < nRatings; i++) {
                if (itemTarget != items[i]) {
                    a = (char) getLoveRatingValue(tracks[items[i]][userTarget]);
                    for (auto userScore: tracks[items[i]]) {
                        if ( userScore.first != userTarget) {
                            b = (char) getLoveRatingValue(userScore.second);
                            if(tracks[itemTarget].count(userScore.first)) {
                                c = (char) getLoveRatingValue(tracks[itemTarget][userScore.first]);
                                sprintf(squareConfInBinary, "%d%d%d", a,b,c);
                                confValue = stoi(squareConfInBinary, nullptr, 2);
                                confSquares[confValue + 1] ++;
                            }
                        }
                    }
                }
            }

            //print lLnes in file
            for(j= 0; j < 10; j++) {
                instanceMatrix << confSquares[j] << " ";
            }
            instanceMatrix << endl;
        }
        memset(confSquares, 0, 10*sizeof(int));
    }
    file.close();
    instanceMatrix.close();
}

int main ()
{
    ifstream testFile("./data/small-test/test.txt", ios::in);
    // ifstream testFile("./data/yahoo-track2/testIdx2.txt", ios::in);
    track2userScore tracksMap = getHashmap(testFile);
    testFile.clear();
    testFile.seekg(0, ios::beg);
    squareCounting(testFile, tracksMap);
}