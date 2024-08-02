//������
#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <string>

using namespace std;

// �萔
const int NUM_INNINGS = 9; // �����̃C�j���O��
const int MAX_HITS = 3;    // �q�b�g���̏��

// �����_��������
random_device rd;
uniform_real_distribution<> dis(0.0, 1.0);

// 0.0����1.0�܂ł̃����_���Ȓl�𐶐�����֐�
double getRandom() {
    return dis(rd);
}

// �`�[���N���X�̒�`
class Team {
private:
    string name;            // �`�[����
    double hitRate;         // �ŗ�
    double homerunRate;     // �z�[��������
    vector<int> scores;     // �e�C�j���O�̓��_
    vector<int> hits_Inning; // �e�C�j���O�̃q�b�g��
    vector<int> homeruns_Inning; // �e�C�j���O�̃z�[��������

public:
    Team(string name, double hitRate, double homerunRate)
        : name(name), hitRate(hitRate), homerunRate(homerunRate), scores(NUM_INNINGS, 0), hits_Inning(NUM_INNINGS, 0), homeruns_Inning(NUM_INNINGS, 0) {}

    // �e�C�j���O�̃V�~�����[�V�������s���֐�
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

// ��������͂��擾����֐�
string getValidStringInput(const string& prompt) {
    string value;
    cout << prompt;
    cin >> value;
    return value;
}

// �L���ȓ��͂��ǂ���
double getValidDoubleInput(const string& prompt, double min, double max) {
    double value;
    cout << prompt;
    cin >> value;
    if (cin.fail() || value < min || value > max) {
        cerr << "�G���[�F" << min << " ���� " << max << " �͈̔͂œ��͂��Ă��������B" << endl;
        exit(EXIT_FAILURE);
    }
    return value;
}

// �z�[�����������ŗ��𒴂���ꍇ�A�v���O�������I������֐�
void validateRates(double hitRate, double homerunRate) {
    if (homerunRate > hitRate) {
        cerr << "�G���[: �z�[���������͑ŗ��𒴂��邱�Ƃ͂ł��܂���B" << endl;
        exit(EXIT_FAILURE);
    }
}

// �V�~�����[�V����
void simulateGames(Team& team1, Team& team2, int trials, vector<pair<Team, Team>>& gameResults) {
    for (int i = 0; i < trials; ++i) {
        team1.simulateInning();
        team2.simulateInning();
        gameResults.emplace_back(team1, team2);
    }
}

int main() {
    string team1Name = getValidStringInput("�`�[��1�̖��O����͂��Ă�������: ");
    double team1HitRate = getValidDoubleInput("�`�[��1�̑ŗ�����͂��Ă������� (0.0����1.0�͈̔�): ", 0.0, 1.0);
    double team1HomerunRate = getValidDoubleInput("�`�[��1�̃z�[������������͂��Ă������� (0.0����1.0�͈̔�): ", 0.0, 1.0);
    validateRates(team1HitRate, team1HomerunRate);

    string team2Name = getValidStringInput("�`�[��2�̖��O����͂��Ă�������: ");
    double team2HitRate = getValidDoubleInput("�`�[��2�̑ŗ�����͂��Ă������� (0.0����1.0�͈̔�): ", 0.0, 1.0);
    double team2HomerunRate = getValidDoubleInput("�`�[��2�̃z�[������������͂��Ă������� (0.0����1.0�͈̔�): ", 0.0, 1.0);
    validateRates(team2HitRate, team2HomerunRate);

    // �V�~�����[�V�������R��s��
    int trials = 3;

    Team prototype1(team1Name, team1HitRate, team1HomerunRate);
    Team prototype2(team2Name, team2HitRate, team2HomerunRate);

    vector<pair<Team, Team>> gameResults;
    simulateGames(prototype1, prototype2, trials, gameResults);

    for (int i = 0; i < trials; ++i) {
        const Team& team1 = gameResults[i].first;
        const Team& team2 = gameResults[i].second;

        cout << "���� " << i + 1 << ": " << endl;
        cout << team1.getName() << " vs " << team2.getName() << endl;

        for (int inning = 0; inning < NUM_INNINGS - 1; ++inning) {
            cout << "�C�j���O " << inning + 1 << ": " << endl;
            cout << team1.getName() << " - ���_: " << team1.getScores()[inning]
                << ", �q�b�g: " << team1.getHits_Inning()[inning]
                << ", �z�[������: " << team1.getHomeruns_Inning()[inning] << endl;
            cout << team2.getName() << " - ���_: " << team2.getScores()[inning]
                << ", �q�b�g: " << team2.getHits_Inning()[inning]
                << ", �z�[������: " << team2.getHomeruns_Inning()[inning] << endl;
        }

        //9�񗠂�����K�v���Ȃ��Ƃ����l���邩��?1
        int team1Total = accumulate(team1.getScores().begin(), team1.getScores().end(), 0);
        int team2Total = accumulate(team2.getScores().begin(), team2.getScores().end() - 1, 0);

        cout << "�C�j���O 9: " << endl;
        cout << team1.getName() << " - ���_: " << team1.getScores().back()
            << ", �q�b�g: " << team1.getHits_Inning().back()
            << ", �z�[������: " << team1.getHomeruns_Inning().back() << endl;

        team1Total += team1.getScores().back();

        //9�񗠂�����K�v���Ȃ��Ƃ�
        if (team2Total > team1Total) {
            cout << "9�񗠂Ȃ��� " << team2.getName() << " �̏���" << endl;
        }
        else {
            cout << team2.getName() << " - ���_: " << team2.getScores().back()
                << ", �q�b�g: " << team2.getHits_Inning().back()
                << ", �z�[������: " << team2.getHomeruns_Inning().back() << endl;
            team2Total += team2.getScores().back();

            if (team1Total > team2Total) {
                cout << team1.getName() << " �̏���" << endl;
            }
            else if (team1Total < team2Total) {
                cout << team2.getName() << " �̏���" << endl;
            }
            else {
                cout << "��������" << endl;
            }
        }

        cout << team1.getName() << " �̍��v���_: " << team1Total << endl;
        cout << team2.getName() << " �̍��v���_: " << team2Total << endl;
        cout << endl;
    }

    return 0;
}
