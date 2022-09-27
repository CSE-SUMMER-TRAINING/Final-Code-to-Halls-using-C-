#include<bits/stdc++.h>
using namespace std;

const int BRN = 1, BN = 15, FN = 30, wrongFrorBuild = 1e6, wrongFrorFloor = 1e3, wrongFrorHall = 1;
const long long OO = 1e18;

vector<pair<string, int>>BRANCH[BRN][BN][FN];
int FLOOR[BRN][BN], BUILD[BRN];
map<string, int>mp;
map<int, string>mp2;

class Hall
{
    public:
            // atrributes
            int branchNum, buildNum, floorNum, volume;
            string name;
            // methods
            Hall(const int, const int, const int, const int, const string);
};

Hall::Hall(const int brn, const int bn, const int fn, const int v, const string s)
{
    branchNum = brn;
    buildNum = bn;
    floorNum = fn;
    volume = v;
    name = s;
}

class Floor
{
    public:
            // atrributes
            int branchNum, buildNum, floorNum, numOfHalls, volume;
            vector<Hall>hallsInFloor;
            // methods
            Floor(const int, const int, const int, const int, const int = 0);
            void getHalls();
};

Floor::Floor(const int brn, const int bn, const int fn, const int noh, const int v)
{
    branchNum = brn;
    buildNum = bn;
    floorNum = fn;
    numOfHalls = noh;
    volume = v;
    getHalls();
}

void Floor::getHalls()
{
    for (pair<string, int> hall : BRANCH[branchNum][buildNum][floorNum])
    {
        hallsInFloor.push_back(Hall(branchNum, buildNum, floorNum, hall.second, hall.first));
    }
}

class Build
{
    public:
            // atrributes
            int branchNum, buildNum, numOfFloors, volume;
            vector<Floor>floorsInBuild;
            // methods
            Build(const int, const int, const int, const int = 0);
            void getFloors();
};

Build::Build(const int brn, const int bn, const int nof, const int v)
{
    branchNum = brn;
    buildNum = bn;
    numOfFloors = nof;
    volume = v;
    getFloors();
}

void Build::getFloors()
{
    for (int i = 0; i < numOfFloors; i++)
    {
        floorsInBuild.push_back(Floor(branchNum, buildNum, i, BRANCH[branchNum][buildNum][i].size()));
    }
}

class Branch
{
    public:
            // attributes
            int branchNum, numOfBuilds, volume;
            string name;
            vector<Build>buildsInBranch;
            vector<Hall>hallsInBranch;
            vector<int>groupsInBranch;
            // methods
            Branch(const string, const int, const int, const int = 0);
            void getBuilds();
};

Branch::Branch(const string s, const int brn, const int nob, const int v)
{
    name = s;
    branchNum = brn;
    volume = v;
    numOfBuilds = nob;
    getBuilds();
}

void Branch::getBuilds()
{
    for (int i = 0; i < numOfBuilds; i++)
    {
        buildsInBranch.push_back(Build(branchNum, i, FLOOR[branchNum][i] + 1));
    }
}

class Group
{
    public:
            // attributes
            int id, volume, from, to, branchNum;
            string name;
            // methods
            Group(const int, const int, const int, const int, const int, const string);
};

Group::Group(const int d, const int v, const int f, const int t, const int brn, const string s)
{
    id = d;
    volume = v;
    from = f;
    to = t;
    branchNum = brn;
    name = s;
}

vector<Branch>college;
vector<Group>group;

void getInputData(int brn)
{
    ifstream File;
    File.open("hallsData.csv");
    if (File.fail())
    {
        cerr << "Error Opening file" << endl;
        exit(1);
    }
    while (File.good())
    {
        string name, volumeStr, buildStr, floorStr;
        int volume, build, floor;
        getline(File, name, ',');
        if (name == "end")return;
        getline(File, volumeStr, ',');
        getline(File, buildStr, ',');
        getline(File, floorStr);
        stringstream geek(volumeStr);
        stringstream geek2(buildStr);
        stringstream geek3(floorStr);
        geek >> volume;
        geek2 >> build;
        geek3 >> floor;
        BRANCH[brn][build][floor].push_back({name, volume});
        FLOOR[brn][build] = max(FLOOR[brn][build], floor);
        BUILD[brn] = max(BUILD[brn], build);
        // cout << name << ' ' << volume << ' ' << build << ' ' << floor << '\n';
    }
    File.close();
    return;
}

void getHalls()
{
    int numOfBranches = college.size();
    for (int i = 0; i < numOfBranches; i++)
    {
        vector<Build> & thisBranch = college[i].buildsInBranch;
        for (int j = 0; j < college[i].numOfBuilds; j++)
        {
            Build & thisBuild = thisBranch[j];
            for (int k = 0; k < thisBuild.numOfFloors; k++)
            {
                Floor & thisFloor = thisBuild.floorsInBuild[k];
                for (int z = 0; z < thisFloor.numOfHalls; z++)
                {
                    college[i].hallsInBranch.push_back(thisFloor.hallsInFloor[z]);
                    // cout << college[i].hallsInBranch.back().first << ' ';
                }
            }
        }
        cout << '\n';
    }
}

void getGroups()
{
    int ID = 0;
    ifstream File;
    File.open("studyGroupsData.csv");
    if (File.fail())
    {
        cerr << "Error Opening file" << endl;
        exit(1);
    }
    while (File.good())
    {
        string name, branch, volumeStr, fromStr, toStr;
        int volume, from, to;
        getline(File, name, ',');
        if (name == "end")return;
        getline(File, branch, ',');
        getline(File, volumeStr, ',');
        getline(File, fromStr, ',');
        getline(File, toStr);
        stringstream geek(volumeStr);
        stringstream geek2(fromStr);
        stringstream geek3(toStr);
        geek >> volume;
        geek2 >> from;
        geek3 >> to;
        int brn = mp[branch];
        group.push_back(Group(ID, volume, from, to, brn, name));
        college[brn].groupsInBranch.push_back(ID);
        ID++;
        // cout << name << ' ' << branch << ' ' << volume << ' ' << from << ' ' << to << '\n';
    }
    File.close();
}

void ClacluateVolume()
{
    int numOfBranches = college.size();
    for (int i = 0; i < numOfBranches; i++)
    {
        int volumeOfBranch = 0;
        vector<Build> & thisBranch = college[i].buildsInBranch;
        for (int j = 0; j < college[i].numOfBuilds; j++)
        {
            int volumeOfBuild = 0;
            Build & thisBuild = thisBranch[j];
            for (int k = 0; k < thisBuild.numOfFloors; k++)
            {
                int volumeOfFloor = 0;
                Floor & thisFloor = thisBuild.floorsInBuild[k];
                for (int z = 0; z < thisFloor.numOfHalls; z++)
                {
                    volumeOfFloor += thisFloor.hallsInFloor[z].volume;
                }
                thisFloor.volume = volumeOfFloor;
                volumeOfBuild += volumeOfFloor;
            }
            thisBuild.volume = volumeOfBuild;
            volumeOfBranch += volumeOfBuild;
        }
        college[i].volume = volumeOfBranch;
    }
}

vector<int>g;
vector<Hall>h;
vector<vector<long long>>mem;
set<set<vector<int>>>uniqeGroups;
int N, M;

void initialize()
{
    N = h.size(), M = g.size();
    int sz1 = N + 5, sz2 = (1 << M) + 5;
    mem.clear();
    mem.resize(sz1, vector<long long>(sz2, -1));
}

long long dp(int idx, int msk)
{
    if (idx == N)
    {
        if ((msk != (1 << M) - 1))return OO;
        return 0;
    }

    long long &ret = mem[idx][msk];
    if (~ret)return ret;

    ret = OO;

    for (int i = 0; i < M; i++)
    {
        if (msk & (1 << i))continue;
        int sm = h[idx].volume, j;
        long long weight = 0;
        for (j = idx + 1; j < N && sm < group[g[i]].volume; j++)
        {
            sm += h[j].volume;
            if (h[j].buildNum != h[j - 1].buildNum)
            {
                weight += wrongFrorBuild;
                continue;
            }
            if (h[j].floorNum != h[j - 1].floorNum)
            {
                weight += wrongFrorFloor;
                continue;
            }
            weight += wrongFrorHall;
        }
        if (sm < group[g[i]].volume)continue;
        ret = min({ret, weight + dp(j, msk | (1 << i)), OO});
    }

    return ret = min({ret, dp(idx + 1, msk), OO});
}

deque<pair<int, int>>toPrint;

void buildDp(int idx, int msk)
{
    if (idx == N)
    {
        for (pair<int, int> s : toPrint)
            cout << h[s.first].name << ' ' << (s.second == -1 ? "free" :  group[g[s.second]].name) << '\n';
        cout << "\n\n-------------------------------------------------\n\n";
        toPrint.clear();
        return;
    }

    long long optimal = dp(idx, msk);

    for (int i = 0; i < M; i++)
    {
        if (msk & (1 << i))continue;
        int sm = h[idx].volume, j;
        long long weight = 0;
        for (j = idx + 1; j < N && sm < group[g[i]].volume; j++)
        {
            sm += h[j].volume;
            if (h[j].buildNum != h[j - 1].buildNum)
            {
                weight += wrongFrorBuild;
                continue;
            }
            if (h[j].floorNum != h[j - 1].floorNum)
            {
                weight += wrongFrorFloor;
                continue;
            }
            weight += wrongFrorHall;
        }
        if (sm < group[g[i]].volume)continue;
        if ((weight + dp(j, msk | (1 << i))) == optimal)
        {
            sm = 0;
            for (j = idx; j < N && sm < group[g[i]].volume; j++)
            {
                sm += h[j].volume;
                toPrint.push_back({j, i});
            }
            return void(buildDp(j, msk | (1 << i)));
        }
    }

    if (dp(idx + 1, msk) == optimal)
    {
        toPrint.push_back({idx, -1});
        return void(buildDp(idx + 1, msk));
    }

    return;
}

void solve(Branch branch)
{
    uniqeGroups.clear();
    vector<int>groups = branch.groupsInBranch;
    vector<Hall>halls = branch.hallsInBranch;
    for (int i = 1; i < (1 << int(groups.size())); i++)
    {
        vector<int>iHave, g1;
        for (int j = 0; j < int(groups.size()); j++)
        {
            iHave.push_back(j);
        }
        int cnt = 0;
        for (int j = 0; j < int(groups.size()); j++)
        {
            if (i & (1 << j))
            {
                g1.push_back(iHave[j - cnt]);
                iHave.erase(next(iHave.begin(), j - cnt++));
            }
        }
        for (int ii = 0; ii < (1 << int(iHave.size())); ii++)
        {
            vector<int>g2, g3;
            for (int j = 0; j < int(iHave.size()); j++)
            {
                if (ii & (1 << j))
                {
                    g2.push_back(iHave[j]);
                }
                else
                {
                    g3.push_back(iHave[j]);
                }
            }
            h = halls;
            g = g1, initialize();
            if (dp(0, 0) == OO)continue;
            g = g2, initialize();
            if (dp(0, 0) == OO)continue;
            g = g3, initialize();
            if (dp(0, 0) == OO)continue;
            set<vector<int>>s;
            s.insert(g1);
            s.insert(g2);
            s.insert(g3);
            if (uniqeGroups.find(s) != uniqeGroups.end())continue;
            uniqeGroups.insert(s);
            // cout << "=======================================================================\n\n\n";
            // cout << "g1 >> ";
            // for (auto x : g1)cout << x << ' ';
            // cout << "\ng2 >> ";
            // for (auto x : g2)cout << x << ' ';
            // cout << "\ng3 >> ";
            // for (auto x : g3)cout << x << ' ';
            // cout << '\n';
            // g = g1, initialize(), buildDp(0, 0);
            // g = g2, initialize(), buildDp(0, 0);
            // g = g3, initialize(), buildDp(0, 0); 
            // cout << "\n\n";
            // cout << "=======================================================================\n\n\n";
        }
    }
}

void display()
{
    cout << "\t\t>> Valid Options << \n";
    int cnt = 1;
    for (auto x : uniqeGroups)
    {
        cout << '\n' << cnt << ". \n";
        int cnt2 = 1;
        for (auto y : x)
        {
            cout << "day " << cnt2 << ": ";
            for (auto z : y)cout << group[z].name << ' ';
            cout << '\n';
            cnt2++;
        }
        cnt++;
    }

    cout << "\t\t==========================================================\n\n";

    int t;
    cin >> t;
    while (t--)
    {
        int choice;
        do
        {
            cin >> choice;
        } while (choice < 1 || choice > uniqeGroups.size());

        vector<int>gg[3];
        cnt = 1;
        
        for (auto x : uniqeGroups)
        {
            if (cnt < choice)
            {
                cnt++;
                continue;
            }
            cnt = 0;
            for (auto y : x)
            {
                gg[cnt++] = y;
            }
            break;
        }
        cout << "\n\n==========================================================\n\n";
        cout << "Day 1 :\n";
        g = gg[0], initialize(), buildDp(0, 0);
        cout << "\nDay 2 :\n";
        g = gg[1], initialize(), buildDp(0, 0);
        cout << "\nDay 3 :\n";
        g = gg[2], initialize(), buildDp(0, 0);
        cout << "\n\n==========================================================\n\n";
    }
}

void printCollege()
{
    for (int i = 0; i < BRN; i++)
    {
        cout << "\t\t>> Branch Num " << i << " <<\n\n";
        vector<Build>thisBranch = college[i].buildsInBranch;
        for (int j = 0; j < college[i].numOfBuilds; j++)
        {
            cout << "BUild Num " << j << '\n';
            Build thisBuild = thisBranch[j];
            for (int k = 0; k < thisBuild.numOfFloors; k++)
            {
                cout << "Floor Num " << k << " : \t";
                Floor thisFloor = thisBuild.floorsInBuild[k];
                for (int z = 0; z < thisFloor.numOfHalls; z++)
                {
                    cout << thisFloor.hallsInFloor[z].name << ' ';
                }
                cout << '\n';
            }
            cout << "\n\n";
        }
    }
}

void printGroups()
{
    for (Group gg : group)
    {
        cout << college[gg.branchNum].name << ' ' << gg.id << ' ' << gg.name << '\n';
    }
}

void printGroupsInBranches()
{
    for (Branch bb : college)
    {
        for (int id : bb.groupsInBranch)
        {
            cout << group[id].name << ' ' << group[id].volume << "  ";
        }
        cout << '\n';
    }
}

int main()
{
    int numOfBranches;
    cin >> numOfBranches;
    for (int i = 0; i < numOfBranches; i++)
    {
        string nameOfBranch;
        cin >> nameOfBranch;
        mp[nameOfBranch] = i;
        mp2[i] = nameOfBranch;
    }
    for (int i = 0; i < BRN; i++)
    {
        getInputData(i);
    }
    for (int i = 0; i < BRN; i++)
    {
        college.push_back(Branch(mp2[i], i, BUILD[i] + 1));
    }
    ClacluateVolume();
    getHalls();
    getGroups();
    // printGroupsInBranches();
    for (int i = 0; i < (int)college.size(); i++)
    {
        solve(college[i]);
        display();
    }
    // cout << '\n';
    // cout << '\n';
    // printCollege();
    // cout << '\n';
    // cout << '\n';
    // printGroups();
    // cout << '\n';
    // cout << '\n';

    return 0;
}
