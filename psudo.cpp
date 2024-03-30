#include <iostream>
#include <vector>
#include <set>
#include <cmath>
using namespace std;
void solve(int n, int B, vector<int>& a) {
    vector<vector<bool>> t(n + 1, vector<bool>(B + 1, false));
    vector<vector<set<int>>> c(n + 1, vector<set<int>>(B + 1)); 
    c[0][0] = {};
    for (int i = 1; i <= n; ++i) {
        t[i][0] = false;
        c[i][0] = {};
    }
    for (int j = 1; j <= B; ++j) {
        t[0][j] = false;
        c[0][j] = {};
    }
    for (int j = 1; j <= B; ++j) {
        for (int i = 1; i <= n; ++i) {
            for (int k = 0; k < i; ++k) {
                int prev_j = j - a[i - 1]; 
                if (prev_j >= 0 && t[k][prev_j]) {
                    t[i][j] = true; 
                    c[i][j] = c[k][prev_j]; 
                    c[i][j].insert(a[i - 1]);
                }
            }}}

    for (int i1 = 0; i1 <= n; ++i1) {
        for (int i2 = i1 + 1; i2 <= n; ++i2) {
            if (t[i1][B] && t[i2][B]) {
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

int main() {
    int n;
    double B;
    cout << "Enter the number of elements (n): ";
    cin >> n;
    vector<int> a(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    for(auto it : a){
        B+= it;
    }

    B = floor(B/2);
    solve(n, B, a);
    return 0;
}
