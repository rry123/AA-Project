#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <climits>
#include <set>
#include <cmath>

using namespace std;

void psuedo_solve(int n, vector<int>& a) {

    double B =0;
    for(auto it : a){
        B+=it;
    }

    B = floor(B/2);
    // Initialize tables t and c
    vector<vector<bool>> t(n + 1, vector<bool>(B + 1, false)); // 2D table to store boolean values
    vector<vector<set<int>>> c(n + 1, vector<set<int>>(B + 1)); // 2D table to store sets
    
    // Initialization
    t[0][0] = true;
    c[0][0] = {};

    // Base Cases
    for (int i = 1; i <= n; ++i) {
        t[i][0] = false;
        c[i][0] = {};
    }

    for (int j = 1; j <= B; ++j) {
        t[0][j] = false;
        c[0][j] = {};
    }

    // Main Loop
    for (int j = 1; j <= B; ++j) {
        for (int i = 1; i <= n; ++i) {
            // Update t[i][j] and c[i][j] based on previous values
            for (int k = 0; k < i; ++k) {
                int prev_j = j - a[i - 1]; // Adjust for zero-based indexing
                if (prev_j >= 0 && t[k][prev_j]) {
                    t[i][j] = true; // Update t[i][j] based on some condition
                    c[i][j] = c[k][prev_j]; // Update c[i][j] based on some condition
                    c[i][j].insert(a[i - 1]); // Insert the current element to the set
                }
            }
        }
    }

    // Termination Condition
    for (int i1 = 0; i1 <= n; ++i1) {
        for (int i2 = i1 + 1; i2 <= n; ++i2) {
            if (t[i1][B] && t[i2][B]) {
                cout << "Solution found!\n";
                cout << "Elements in subset 1: ";
                for (auto elem : c[i1][B]) {
                    cout << elem << " ";
                }
                cout << "\nElements in subset 2: ";
                for (auto elem : c[i2][B]) {
                    cout << elem << " ";
                }
                cout << endl;
                return;
            }
        }
    }

    cout << "No solution found!\n";
}


void solve_instance_case_2b(const vector<int>& nums, const vector<int>& Im) {
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

    cout<<"Subset 1 : ";
    for(auto it: min_P){
        cout<<it<<" ";
    }
    cout<<endl;
    cout<<"Subset : 2 ";

    for(auto it : min_Q){
        cout<<it<<" ";
    }
    cout<<endl;
}

// Function to solve the algorithm for a given instance Im
void solve_instance(const vector<int>& nums, double e, int n) {
    int m = nums.size();

    // Sorting the input numbers in increasing order
    vector<int> sorted_nums = nums;
    sort(sorted_nums.begin(), sorted_nums.end());

    // Calculate k(m)
    double km = (e * e * sorted_nums[m - 1]) / (2 * m);

    // Calculate n0
    int n0 = 0;
    while (n0 <= n ) {
        double kn0 = (e*e*sorted_nums[m-1])/(2*m);
        if(kn0<1){
            n0++;
        }
    }

    if (m <= n0) {
        // Apply pseudo-polynomial algorithm
        cout << "Applying pseudo-polynomial algorithm for instance I_" << m << endl;
        psuedo_solve(sorted_nums.size(), sorted_nums);
    } else {
        // Transform the instance to contain only polynomial-size numbers
        vector<double> transformed_nums(m);
        for (int i = 0; i < m; ++i) {
            transformed_nums[i] =  floor((double(sorted_nums[i]) ) / km);
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
            cout<<"Subset 1 :";
            cout<<Im[0];
            cout<<endl;
            cout<<"Subset 2: ";
            for(int k =1; k<t; k++){
                cout<<Im[k]<<" ";
            }
        } else {
            // Case 2: t > 1
            cout << "Case 2: t > 1" << endl;
            
            double sum = 0;

            for(auto it : Im){
                sum += it;
            }

            if(int(sum) %2 == 0){
                psuedo_solve(t, Im);
            }
            else{
                solve_instance_case_2b(sorted_nums, Im);
            }
        }
    }

    // Placeholder return value
    
}

int main() {
    vector<int> nums = {1, 2, 3, 4, 5, 6, 8}; // Example input
    double e = 0.1; // Example value of e
    int n = nums.size();
    for (int m = 2; m <= nums.size(); ++m) {
        cout << "Instance I_" << m << ":" << endl;

        vector<int> alpha;
        for(int k=0; k<m ; k++){
            alpha.push_back(nums[k]);
        }
        solve_instance(alpha, e, n);

    }

    return 0;
}
