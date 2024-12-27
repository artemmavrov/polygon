#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findMinMoves(vector<int>& machines) {
        int n = machines.size();
        int sm = 0;

        for (int i = 0; i < n; i++) {
            sm += machines[i];
        }

        if (sm % n != 0) {
            return -1;
        }

        int amt_dresses = sm / n;

        int total_balance = 0;
        int max_moves = 0;

        for (int i = 0; i < n; i++) {
            int diff = machines[i] - amt_dresses;
            total_balance += diff;
            max_moves = max(max_moves, max(abs(total_balance), diff));
        }

        return max_moves;
    }
};

