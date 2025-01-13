#include <chrono>
#include <windows.h>
#include "lab_8_shortest_ways.cpp"

using namespace std;
using namespace std::chrono;

int main() {
    char temp_buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, temp_buffer);
    string buffer = string(temp_buffer);
    vector<vector<long double>> graf;
    string file_way = "C:/Users/artem/Desktop/labs/algoritms/lab-8/custom_format.txt";

    read_graf(file_way, graf);
    int n = graf.size();

    cout << "Start: ";
    long double x_start, y_start;
    cin >> x_start >> y_start;
    int start = get_nearby(x_start, y_start, graf, n);

    cout << "End: ";
    long double x_end, y_end;
    cin >> x_end >> y_end;
    int end = get_nearby(x_end, y_end, graf, n);    

    cout << "Choose algorithm to find shortest way(cin number):" << endl
    << "1) Dijkstra" << endl 
    << "2) A*" << endl 
    << "3) BFS" << endl 
    << "4) DFS" << endl;
    int number_algorithm;
    cin >> number_algorithm;
    
    unordered_map<string, int> graf_map;
    to_map(graf, graf_map, n);

    auto start_time = chrono::high_resolution_clock::now();
    if (number_algorithm == 1) {
        cout << "Dijkstra: " << Dijkstra(start, end, graf, graf_map, n) << endl;
    }else if (number_algorithm == 2) {
        cout << "A*: " << AStar(start, end, graf, graf_map, n) << endl;
    }else if (number_algorithm == 3) {
        cout << "BFS: " << BFS(start, end, graf, graf_map, n) << endl;
    }else {
        cout << "DFS: " << DFS(start, end, graf, graf_map, n) << endl;
    }
    auto end_time = chrono::high_resolution_clock::now();
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << " ms" << endl;

    return 0;
}
