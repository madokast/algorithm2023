#include<vector>
#include<unordered_map>
#include<cstdlib>
#include<iostream>
using namespace std;

class Solution {
public:
    vector<int> twoSum(const vector<int>& nums, int target) {
        vector<int> ret(2);
        unordered_map<int, int> m;
        for (size_t i = 0; i < nums.size(); i++) {
            auto it = m.find(target - nums[i]);
            if (it == m.end()) {
                m[nums[i]] = i;
            } else {
                ret[0] = it->second;
                ret[1] = i;
                return ret;
            }
        }
        exit(-1);
    }
};

int main() {
    Solution s;
    auto r = s.twoSum({3, 2, 4}, 6);
    cout << r[0] << " " << r[1] << endl;
}