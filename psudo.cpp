#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <climits>

using namespace std;

pair<vector<int>, vector<int>> solve_subset_sum_equality(vector<int>& nums) {
    int n = nums.size();
    int B = accumulate(nums.begin(), nums.end(), 0);
    vector<vector<int>> t(n + 1, vector<int>(B + 1, 0));
    vector<unordered_map<int, vector<int>>> c(n + 1, unordered_map<int, vector<int>>());

    // Initialize tables
    t[0][0] = 1;
    c[0][0] = {};

    for (int i = 1; i <= n; ++i) {
        t[i][0] = 0;
        c[i][0] = {};
    }

    for (int j = 1; j <= B; ++j) {
        t[0][j] = 0;
        c[0][j] = {};
    }

    // Fill out the tables
    int result = -1;
    pair<vector<int>, vector<int>> optimal_solution = make_pair(vector<int>(), vector<int>());

    for (int j = 1; j <= B; ++j) {
        for (int i = 1; i <= n; ++i) {
            if (j >= nums[i - 1] && t[i - 1][j - nums[i - 1]] == 1) {
                t[i][j] = 1;
                c[i][j] = c[i - 1][j - nums[i - 1]];
                c[i][j].push_back(i);
            } else {
                t[i][j] = 0;
                c[i][j] = {};
            }
        }
    }

    // Identify candidate sums and find the optimal solution
    int min_ratio = INT_MAX;
    for (int j = 1; j <= B / 2; ++j) {
        if (t[n][j] == 1) {
            int kj = -1;
            for (int k = j - 1; k >= 1; --k) {
                if (!c[n][k].empty() && (c[n][j].size() < c[n][k].size())) {
                    kj = k;
                    break;
                }
            }
            if (kj != -1) {
                int ratio = j / kj;
                if (ratio < min_ratio) {
                    min_ratio = ratio;
                    optimal_solution.first = c[n][j];
                    optimal_solution.second = c[n][kj];
                }
            }
        }
    }

    return optimal_solution;
}

int main() {
    vector<int> nums = {1, 2, 3, 4};
    pair<vector<int>, vector<int>> ans = solve_subset_sum_equality(nums);

    cout << "Subset 1: ";
    for(auto it : ans.first){
        cout << it << " ";
    }
    cout << endl;

    cout << "Subset 2: ";
    for(auto it : ans.second){
        cout << it << " ";
    }
    cout << endl;

    return 0;
}