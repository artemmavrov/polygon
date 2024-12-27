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

        int sm_l;
        bool flag = false;
        int step = 0;
        while (!flag) {
            sm_l = machines[0];
            flag = true;

            if (machines[0] > amt_dresses) {
                machines[1] += 1;
                machines[0] -= 1;
            }
            for (int i = 1; i < n; i++) {
                if (machines[i] > 0){
                    if (sm_l < i * amt_dresses) {
                        machines[i - 1] += 1;
                        machines[i] -= 1;
                    }else if (sm - sm_l - machines[i] < i * amt_dresses && i != n - 1) {
                        machines[i + 1] += 1;
                        machines[i] -= 1;
                    }
                }

                sm_l += machines[i];
            }
            step += 1;

            for (int i = 0; i < n; i++) {
                if (machines[i] != amt_dresses) {
                    flag = false;
                    break;
                }
            }
        }

        return step;
    }
};

