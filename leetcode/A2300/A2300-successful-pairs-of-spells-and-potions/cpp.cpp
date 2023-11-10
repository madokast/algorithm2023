#include<vector>
#include<algorithm>
#include<iostream>
#include<cstring>
using namespace std;

static int suffix_sum[100001];

class Solution3 { // 前缀和
public:
    vector<int> successfulPairs(vector<int>& spells, vector<int>& potions, long long success) {
        memset(suffix_sum, 0, sizeof suffix_sum);
        vector<int> pairs(spells.size());
        int max_potion = 0;
        for (int e : potions) {
            max_potion = max(max_potion, e);
            ++suffix_sum[e];
        }
        for (size_t i = max_potion; i > 0; i--) suffix_sum[i-1] += suffix_sum[i];
        for (size_t i = 0; i < spells.size(); i++) {
            long long target = (success + spells[i] - 1) / spells[i];
            if (target > max_potion) pairs[i] = 0;
            else pairs[i] = suffix_sum[target];
        }
        
        return pairs;
    }
};

class Solution2 { // 都排序，双指针
public:
    vector<int> successfulPairs(vector<int>& spells, vector<int>& potions, long long success) {
        vector<int> pairs(spells.size());
        vector<pair<int, int>> spellIndexs(spells.size());
        for (size_t i = 0; i < spells.size(); i++) {
            spellIndexs[i].first = spells[i];
            spellIndexs[i].second = i;
        }
        sort(spellIndexs.begin(), spellIndexs.end(), [](pair<int, int> a, pair<int, int> b){return a.first > b.first;});
        sort(potions.begin(), potions.end());
        for (size_t i = 0, j = 0; i < spells.size(); i++) {
            long long spell = spellIndexs[i].first;
            int index = spellIndexs[i].second;
            while (j < potions.size() && spell * potions[j] < success) j++;
            pairs[index] = potions.size() - j;
        }
        
        return pairs;
    }
};

class Solution { // 排序 二分查找
public:
    vector<int> successfulPairs(vector<int>& spells, vector<int>& potions, long long success) {
        vector<int> pairs(spells.size());
        sort(potions.begin(), potions.end());
        for (size_t i = 0; i < spells.size(); i++) {
            long long target = (success + spells[i] - 1) / spells[i];
            if (target > potions[potions.size() - 1]) {
                pairs[i] = 0;
            } else {
                auto lb = lower_bound(potions.begin(), potions.end(), target);
                pairs[i] = potions.end() - lb;
            }
        }
        
        return pairs;
    }
};


int main() {
    Solution3 s;
    vector<int> s1{5, 1, 3};
    vector<int> p1{1,2,3,4,5};
    for (int e : s.successfulPairs(s1, p1, 7)) {
        cout << e << " ";
    }
    cout << "\n";
    
    vector<int> s2{3,1,2};
    vector<int> p2{8,5,8};
    for (int e : s.successfulPairs(s2, p2, 16)) {
        cout << e << " ";
    }
    cout << "\n";
}