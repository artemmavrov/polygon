#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <string>
#include <cmath>
#include <utility>
#include <cctype>
#include <cstring>
#include <queue>
#include <stack>
#include <set>

using namespace std;

vector<long double> parse_line(const string& line) {
    vector<long double> numbers;
    string token;
    istringstream stream(line);
    char delimiters[] = ":;,";
    char currentChar;

    while (stream.get(currentChar)) {
        if (isdigit(currentChar) || currentChar == '.' || currentChar == '-' || currentChar == 'e' || currentChar == 'E') {
            token += currentChar;
        } else if (strchr(delimiters, currentChar) && !token.empty()) {
            numbers.push_back(stold(token));
            token.clear();
        }
    }

    if (!token.empty()) {
        numbers.push_back(stold(token));
    }

    return numbers;
}

void read_graf(const string& filename, vector<vector<long double>>& graf) {
    ifstream file(filename);

    string line;
    while (getline(file, line)) {
        auto numbers = parse_line(line);
        if (!numbers.empty()) {
            graf.push_back(numbers);
        }
    }

    file.close();
}


int get_nearby(long double x, long double y, vector<vector<long double>>& graf, int& n) {
    int ind_nearby;
    long double mn_l = 1e9;
    long double l;
    for (int i = 0; i < n; i++) {
        l = sqrt(pow(x - graf[i][0], 2) + pow(y - graf[i][1], 2));
        if (mn_l > l) {
            mn_l = l;
            ind_nearby = i;
        }
    }

    return ind_nearby;
} 

void to_map(vector<vector<long double>>& graf, unordered_map<string, int>& graf_map, int& n) {
    for (int i = 0; i < n; i++) {
        string key = to_string(graf[i][0]) + to_string(graf[i][1]);
        graf_map.insert(make_pair(key, i));
    }
}

int get_index(unordered_map<string, int>& graf_map, long double& x, long double& y) {
    string key = to_string(x) + to_string(y);
    return graf_map[key];
}

long double BFS(int& start, int& end, vector<vector<long double>>& graf, unordered_map<string, int>& graf_map, int& n) {
    vector<long double> distances(n, numeric_limits<long double>::max());
    queue<int> q;

    distances[start] = 0;
    q.push(start);

    while (!q.empty()) {  //обойдёт все вершины и рёбра O(V + E), но может обходить граф несколько раз для поиска лучшего пути(тк граф взвешен), 
        int node = q.front();//так что фактическая сложность будет куда выше и O((V + E) * k), где k количество полных обходов графа
        q.pop();

        for (int i = 2; i < graf[node].size(); i += 3) {
            int neighborNode = get_index(graf_map, graf[node][i], graf[node][i + 1]);
            long double weight = graf[node][i + 2];

            if (distances[node] + weight < distances[neighborNode]) {
                distances[neighborNode] = distances[node] + weight;
                q.push(neighborNode);
            }
        }
    }

    return distances[end];
}
//Итого асимтотика O((V + E) * k), где V - количество вершин, E - рёбер, k - количество полных обходов графа
//Пространсвтенная сложность: хранит graf, graf_map и distances, итого O(N * 32 * 8 + N * 32 * 2 + 8 + N * 32) = O(N * 360)байт, N - колчечтво строк в исходном файле

long double Dijkstra(int& start, int& end, vector<vector<long double>>& graf, unordered_map<string, int>& graf_map, int& n) {
    vector<long double> distances(n, numeric_limits<long double>::max());
    set<pair<long double, int>> pq;

    distances[start] = 0;
    pq.insert({0, start});

    while (!pq.empty()) { //Обойдёт все вершины O(V + E)
        auto [dist, node] = *pq.begin();
        pq.erase(pq.begin());

        if (node == end) break;

        for (int i = 2; i < graf[node].size(); i += 3) {
            int neighborNode = get_index(graf_map, graf[node][i], graf[node][i + 1]);
            long double weight = graf[node][i + 2];

            if (distances[node] + weight < distances[neighborNode]) { //Извлечение минимального из кучи O(logV)
                pq.erase({distances[neighborNode], neighborNode});
                distances[neighborNode] = distances[node] + weight;
                pq.insert({distances[neighborNode], neighborNode});
            }
        }
    }

    return distances[end];
}
//Итого O((V + E) * logV)
//Пространсвтенная сложность: хранит graf, graf_map, pq и distances, итого O(N * 32 * 8 + N * 32 * 2 + 8 + N * 32) = O(N * 360)байт, N - колчечтво строк в исходном файле,
//pq можно не учитывать, тк редко бывает заполнена

long double calculate_heuristic(long double x1, long double y1, long double x2, long double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

long double AStar(int& start, int& end, vector<vector<long double>>& graf, unordered_map<string, int>& graf_map, int& n) {
    vector<long double> distances(n, numeric_limits<long double>::max());
    vector<long double> f_score(n, numeric_limits<long double>::max());
    set<pair<long double, int>> open_set;

    distances[start] = 0;
    f_score[start] = calculate_heuristic(graf[start][0], graf[start][1], graf[end][0], graf[end][1]);
    open_set.insert({f_score[start], start});

    while (!open_set.empty()) {
        auto [current_f, current] = *open_set.begin();
        open_set.erase(open_set.begin());

        if (current == end) {
            return distances[end];
        }

        for (int i = 2; i < graf[current].size(); i += 3) {
            int neighbor = get_index(graf_map, graf[current][i], graf[current][i + 1]);
            long double weight = graf[current][i + 2];

            long double tentative_g_score = distances[current] + weight;
            if (tentative_g_score < distances[neighbor]) {
                open_set.erase({f_score[neighbor], neighbor});
                distances[neighbor] = tentative_g_score;
                f_score[neighbor] = distances[neighbor] + calculate_heuristic(graf[neighbor][0], graf[neighbor][1], graf[end][0], graf[end][1]);
                open_set.insert({f_score[neighbor], neighbor});
            }
        }
    }

    return -1;
}

//Итого O((V + E) * logV) в худшеми O(E) в лучшем
//Пространсвтенная сложность: хранит graf, graf_map, open_set, f_score и distances, итого O(N * 32 * 8 + N * 32 * 2 + 8 + N * 32 + N * 32) = O(N * 392)байт, N - колчечтво строк в исходном файле,
//open_set можно не учитывать, тк редко бывает заполнена

long double DFS_Util(int node, int end, vector<vector<long double>>& graf, unordered_map<string, int>& graf_map, vector<bool>& visited, vector<long double>& distances, long double current_distance) {
    if (node == end) {
        return current_distance; 
    }

    visited[node] = true;
    long double shortest_path = numeric_limits<long double>::max();

    for (int i = 2; i < graf[node].size(); i += 3) {
        int neighborNode = get_index(graf_map, graf[node][i], graf[node][i + 1]);
        long double weight = graf[node][i + 2];

        if (!visited[neighborNode]) {
            long double path_distance = DFS_Util(neighborNode, end, graf, graf_map, visited, distances, current_distance + weight);
            shortest_path = min(shortest_path, path_distance);
        }
    }

    visited[node] = false;
    return shortest_path;
}

long double DFS(int& start, int& end, vector<vector<long double>>& graf, unordered_map<string, int>& graf_map, int& n) {
    vector<bool> visited(n, false); 
    vector<long double> distances(n, numeric_limits<long double>::max());

    return DFS_Util(start, end, graf, graf_map, visited, distances, 0.0);
}