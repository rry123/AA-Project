#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <climits>

using namespace std;

// Function to solve the subset sum equality preoblem using the pseudo-polynomial algorithm
pair<vector<int>, vector<int>> solve_subset_sum_equality(const vector<int>& nums) {
    int n = nums.size();
    int B = accumulate(nums.begin(), nums.end(), 0);
    vector<vector<int>> t(n + 1, vector<int>(B + 1, 0));
    vector<unordered_map<int, vector<int>>> c(n + 1, unordered_map<int, vector<int>>());

    // Step 1: Initialize tables
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

    // Step 2: Fill out the tables
    int result = -1;
    pair<vector<int>, vector<int>> optimal_solution;

    for (int j = 1; j <= B; ++j) {
        for (int i = 1; i <= n; ++i) {
            if (j >= nums[i - 1] && t[i - 1][j - nums[i - 1]] == 1) {
                t[i][j] = 1;
                c[i][j] = c[i - 1][j - nums[i - 1]];
                c[i][j].push_back(i);

                // Check for termination condition
                if (t[i][j] == 1 && t[i - 1][j] == 1) {
                    result = j;
                    optimal_solution.first = c[i][j];
                    optimal_solution.second = c[i - 1][j];
                    return optimal_solution;
                }
            } else {
                t[i][j] = 0;
                c[i][j] = {};
            }
        }
    }

    // Step 3: Identify candidate sums and find the optimal solution
    int min_ratio = INT_MAX;
    for (int j = 1; j <= result / 2; ++j) {
        if (t[n][j] == 1) {
            int kj = -1;
            for (int k = j - 1; k >= 1; --k) {
                if (!c[n][j].empty() && (c[n][j].size() < c[n][k].size())) {
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

// Function to solve instance I_-m for Case 2b
pair<vector<int>, vector<int>> solve_instance_case_2b(const vector<int>& nums, const vector<int>& Im) {
    int m = nums.size();

    // Generate pairs of subsets P(v, m) and Q(v, m)
    vector<vector<int>> P, Q;
    int t = Im.size();
    for (int v = 0; v < (1 << (t - 1)); ++v) {
        vector<int> P_subset, Q_subset;
        for (int i = 0; i < t; ++i) {
            if (v & (1 << i)) {
                P_subset.push_back(nums[Im[i]]);
            } else {
                Q_subset.push_back(nums[Im[i]]);
            }
        }
        P.push_back(P_subset);
        Q.push_back(Q_subset);
    }

    // Choose the pair of subsets for which the ratio C_i * S1 / C_i * S2 is minimized
    double min_ratio = numeric_limits<double>::infinity();
    vector<int> min_P, min_Q;
    for (int i = 0; i < P.size(); ++i) {
        double sum_P = accumulate(P[i].begin(), P[i].end(), 0);
        double sum_Q = accumulate(Q[i].begin(), Q[i].end(), 0);
        double ratio = sum_P / sum_Q;
        if (ratio < min_ratio) {
            min_ratio = ratio;
            min_P = P[i];
            min_Q = Q[i];
        }
    }

    return make_pair(min_P, min_Q);
}

// Function to solve the algorithm for a given instance Im
pair<vector<int>, vector<int>> solve_instance(const vector<int>& nums, double e) {
    int m = nums.size();

    // Sorting the input numbers in increasing order
    vector<int> sorted_nums = nums;
    sort(sorted_nums.begin(), sorted_nums.end());

    // Calculate k(m)
    double km = (e * e * sorted_nums[m - 1]) / (2 * m);

    // Calculate n0
    int n0 = 0;
    while (n0 <= m && km * n0 < 1) {
        n0++;
    }

    if (m <= n0) {
        // Apply pseudo-polynomial algorithm
        cout << "Applying pseudo-polynomial algorithm for instance I_" << m << endl;
        pair<vector<int>, vector<int>> solution = solve_subset_sum_equality(sorted_nums);
        return solution;
    } else {
        // Transform the instance to contain only polynomial-size numbers
        vector<double> transformed_nums(m);
        for (int i = 0; i < m; ++i) {
            transformed_nums[i] = (double(sorted_nums[i]) * m) / km;
        }

        // Define I_-m
        vector<int> Im;
        for (int i = 0; i < m; ++i) {
            if (transformed_nums[i] >= m / e) {
                Im.push_back(i);
            }
        }

        // Handle cases based on the value of t
        int t = Im.size();
        if (t == 1) {
            // Case 1: t = 1
            cout << "Case 1: t = 1" << endl;
            // Implement solution for Case 1
        } else {
            // Case 2: t > 1
            cout << "Case 2: t > 1" << endl;
            pair<vector<int>, vector<int>> solution = solve_instance_case_2b(sorted_nums, Im);
            return solution;
        }
    }

    // Placeholder return value
    return make_pair(vector<int>(), vector<int>());
}

int main() {
    vector<int> nums = {1, 2, 3, 4, 5}; // Example input
    double e = 0.8; // Example value of e

    for (int m = 2; m <= nums.size(); ++m) {
        cout << "Instance I_" << m << ":" << endl;
        pair<vector<int>, vector<int>> solution = solve_instance(nums, e);
        // Output the solution here

        cout<<"Printing the subset1"<<endl;
        cout<<"{";
        for(int i : solution.first){
            cout<<i<<" ";
        }
        cout<<"}";


        cout<<endl<<endl;
        cout<<"Printing thhe subset2"<<endl;
        cout<<"{";
        for(int i: solution.second){
            cout<<i<<" ";
        }
        cout<<"}";
        cout<<endl;
    }

    return 0;
}
