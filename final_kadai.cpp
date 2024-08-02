//完成版
#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <string>

using namespace std;

// 定数
const int NUM_INNINGS = 9; // 試合のイニング数
const int MAX_HITS = 3;    // ヒット数の上限

// ランダム数生成
random_device rd;
uniform_real_distribution<> dis(0.0, 1.0);

// 0.0から1.0までのランダムな値を生成する関数
double getRandom() {
    return dis(rd);
}

// チームクラスの定義
class Team {
private:
    string name;            // チーム名
    double hitRate;         // 打率
    double homerunRate;     // ホームラン率
    vector<int> scores;     // 各イニングの得点
    vector<int> hits_Inning; // 各イニングのヒット数
    vector<int> homeruns_Inning; // 各イニングのホームラン数

public:
    Team(string name, double hitRate, double homerunRate)
        : name(name), hitRate(hitRate), homerunRate(homerunRate), scores(NUM_INNINGS, 0), hits_Inning(NUM_INNINGS, 0), homeruns_Inning(NUM_INNINGS, 0) {}

    // 各イニングのシミュレーションを行う関数
    void simulateInning() {
        scores.assign(NUM_INNINGS, 0);
        hits_Inning.assign(NUM_INNINGS, 0);
        homeruns_Inning.assign(NUM_INNINGS, 0);

        for (int i = 0; i < NUM_INNINGS; ++i) {
            int hits = 0;
            int hitsc = 0;
            int homeruns = 0;
            int outs = 0;

            while (outs < 3) {
                double c = getRandom();
                if (c <= hitRate) {
                    if (c <= homerunRate) {
                        scores[i] += hits + 1;
                        homeruns++;
                        hits = 0;
                    }
                    else {
                        hits++;
                        hitsc++;
                        if (hits > MAX_HITS) {
                            scores[i]++;
                            hits--;
                        }
                    }
                }
                else {
                    outs++;
                }
            }
            hits_Inning[i] = hitsc;
            homeruns_Inning[i] = homeruns;
        }
    }

    int getTotalScore() const {
        return accumulate(scores.begin(), scores.end(), 0);
    }

    string getName() const {
        return name;
    }

    const vector<int>& getScores() const {
        return scores;
    }

    const vector<int>& getHits_Inning() const {
        return hits_Inning;
    }

    const vector<int>& getHomeruns_Inning() const {
        return homeruns_Inning;
    }
};

// 文字列入力を取得する関数
string getValidStringInput(const string& prompt) {
    string value;
    cout << prompt;
    cin >> value;
    return value;
}

// 有効な入力かどうか
double getValidDoubleInput(const string& prompt, double min, double max) {
    double value;
    cout << prompt;
    cin >> value;
    if (cin.fail() || value < min || value > max) {
        cerr << "エラー：" << min << " から " << max << " の範囲で入力してください。" << endl;
        exit(EXIT_FAILURE);
    }
    return value;
}

// ホームラン率が打率を超える場合、プログラムを終了する関数
void validateRates(double hitRate, double homerunRate) {
    if (homerunRate > hitRate) {
        cerr << "エラー: ホームラン率は打率を超えることはできません。" << endl;
        exit(EXIT_FAILURE);
    }
}

// シミュレーション
void simulateGames(Team& team1, Team& team2, int trials, vector<pair<Team, Team>>& gameResults) {
    for (int i = 0; i < trials; ++i) {
        team1.simulateInning();
        team2.simulateInning();
        gameResults.emplace_back(team1, team2);
    }
}

int main() {
    string team1Name = getValidStringInput("チーム1の名前を入力してください: ");
    double team1HitRate = getValidDoubleInput("チーム1の打率を入力してください (0.0から1.0の範囲): ", 0.0, 1.0);
    double team1HomerunRate = getValidDoubleInput("チーム1のホームラン率を入力してください (0.0から1.0の範囲): ", 0.0, 1.0);
    validateRates(team1HitRate, team1HomerunRate);

    string team2Name = getValidStringInput("チーム2の名前を入力してください: ");
    double team2HitRate = getValidDoubleInput("チーム2の打率を入力してください (0.0から1.0の範囲): ", 0.0, 1.0);
    double team2HomerunRate = getValidDoubleInput("チーム2のホームラン率を入力してください (0.0から1.0の範囲): ", 0.0, 1.0);
    validateRates(team2HitRate, team2HomerunRate);

    // シミュレーションを３回行う
    int trials = 3;

    Team prototype1(team1Name, team1HitRate, team1HomerunRate);
    Team prototype2(team2Name, team2HitRate, team2HomerunRate);

    vector<pair<Team, Team>> gameResults;
    simulateGames(prototype1, prototype2, trials, gameResults);

    for (int i = 0; i < trials; ++i) {
        const Team& team1 = gameResults[i].first;
        const Team& team2 = gameResults[i].second;

        cout << "試合 " << i + 1 << ": " << endl;
        cout << team1.getName() << " vs " << team2.getName() << endl;

        for (int inning = 0; inning < NUM_INNINGS - 1; ++inning) {
            cout << "イニング " << inning + 1 << ": " << endl;
            cout << team1.getName() << " - 得点: " << team1.getScores()[inning]
                << ", ヒット: " << team1.getHits_Inning()[inning]
                << ", ホームラン: " << team1.getHomeruns_Inning()[inning] << endl;
            cout << team2.getName() << " - 得点: " << team2.getScores()[inning]
                << ", ヒット: " << team2.getHits_Inning()[inning]
                << ", ホームラン: " << team2.getHomeruns_Inning()[inning] << endl;
        }

        //9回裏をする必要がないときを考えるから?1
        int team1Total = accumulate(team1.getScores().begin(), team1.getScores().end(), 0);
        int team2Total = accumulate(team2.getScores().begin(), team2.getScores().end() - 1, 0);

        cout << "イニング 9: " << endl;
        cout << team1.getName() << " - 得点: " << team1.getScores().back()
            << ", ヒット: " << team1.getHits_Inning().back()
            << ", ホームラン: " << team1.getHomeruns_Inning().back() << endl;

        team1Total += team1.getScores().back();

        //9回裏をする必要がないとき
        if (team2Total > team1Total) {
            cout << "9回裏なしで " << team2.getName() << " の勝利" << endl;
        }
        else {
            cout << team2.getName() << " - 得点: " << team2.getScores().back()
                << ", ヒット: " << team2.getHits_Inning().back()
                << ", ホームラン: " << team2.getHomeruns_Inning().back() << endl;
            team2Total += team2.getScores().back();

            if (team1Total > team2Total) {
                cout << team1.getName() << " の勝利" << endl;
            }
            else if (team1Total < team2Total) {
                cout << team2.getName() << " の勝利" << endl;
            }
            else {
                cout << "引き分け" << endl;
            }
        }

        cout << team1.getName() << " の合計得点: " << team1Total << endl;
        cout << team2.getName() << " の合計得点: " << team2Total << endl;
        cout << endl;
    }

    return 0;
}
