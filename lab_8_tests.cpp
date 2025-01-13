#include <iostream>
#include <random>
#include <iomanip>
#include <chrono>
#include <windows.h>
#include "lab_8_shortest_ways.cpp"

using namespace std;
using namespace std::chrono;

string buffer;

struct Vertex {
    double x, y;
};

struct Edge {
    int to;
    double weight;
};

void generateGraph(int vertexCount, const string& incidenceFile, const string& customFile) {
    vector<Vertex> vertices(vertexCount);
    vector<vector<Edge>> adjList(vertexCount);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> coordDist(1.0, 100.0);
    uniform_real_distribution<> weightDist(0.1, 1.0);
    uniform_int_distribution<> neighborDist(2, 5);

    for (auto& vertex : vertices) {
        vertex.x = coordDist(gen);
        vertex.y = coordDist(gen);
    }

    set<pair<int, int>> addedEdges;
    for (int i = 0; i < vertexCount; ++i) {
        int connections = neighborDist(gen);
        for (int j = 0; j < connections; ++j) {
            int neighbor = gen() % vertexCount;
            while (neighbor == i || addedEdges.count({i, neighbor}) || addedEdges.count({neighbor, i})) {
                neighbor = gen() % vertexCount;
            }

            double weight = weightDist(gen);
            adjList[i].push_back({neighbor, weight});
            adjList[neighbor].push_back({i, weight});
            addedEdges.insert({i, neighbor});
        }
    }

    ofstream incFile(incidenceFile);
    incFile << fixed << setprecision(2);
    for (int i = 0; i < vertexCount; ++i) {
        for (int j = 0; j < vertexCount; ++j) {
            double weight = 0.0;
            for (const auto& edge : adjList[i]) {
                if (edge.to == j) {
                    weight = edge.weight;
                    break;
                }
            }
            incFile << weight;
            if (j != vertexCount - 1) {
                incFile << ", ";
            }
        }
        incFile << "\n";
    }
    incFile.close();

    ofstream custFile(customFile);
    custFile << fixed << setprecision(2);
    for (int i = 0; i < vertexCount; ++i) {
        custFile << vertices[i].x << "," << vertices[i].y << ":";
        for (const auto& edge : adjList[i]) {
            custFile << vertices[edge.to].x << "," << vertices[edge.to].y << "," << edge.weight << ";";
        }
        custFile << "\n";
    }
    custFile.close();
}

void generateCustomGraph(int vertexCount, const string& customFile){
    vector<Vertex> vertices(vertexCount);
    vector<vector<Edge>> adjList(vertexCount);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> coordDist(1.0, 100.0);
    uniform_real_distribution<> weightDist(0.1, 1.0);
    uniform_int_distribution<> neighborDist(2, 5);

    for (auto& vertex : vertices) {
        vertex.x = coordDist(gen);
        vertex.y = coordDist(gen);
    }

    set<pair<int, int>> addedEdges;
    for (int i = 0; i < vertexCount; ++i) {
        int connections = neighborDist(gen);
        for (int j = 0; j < connections; ++j) {
            int neighbor = gen() % vertexCount;
            while (neighbor == i || addedEdges.count({i, neighbor}) || addedEdges.count({neighbor, i})) {
                neighbor = gen() % vertexCount;
            }

            double weight = weightDist(gen);
            adjList[i].push_back({neighbor, weight});
            adjList[neighbor].push_back({i, weight});
            addedEdges.insert({i, neighbor});
        }
    }

    ofstream custFile(customFile);
    custFile << fixed << setprecision(2);
    for (int i = 0; i < vertexCount; ++i) {
        custFile << vertices[i].x << "," << vertices[i].y << ":";
        for (const auto& edge : adjList[i]) {
            custFile << vertices[edge.to].x << "," << vertices[edge.to].y << "," << edge.weight << ";";
        }
        custFile << "\n";
    }
    custFile.close();
}

void testingWithStep(int start_size, int end_size, int step) {
    ofstream result;
    result.open("result.txt");

    for (int i = start_size, number = 1; i < end_size; i += step, number++) {
        generateCustomGraph(i, "custom_format" + to_string(number) + ".txt");
        vector<vector<long double>> graf;
        string file_way = "C:/Users/artem/Desktop/labs/algoritms/lab-8/output/custom_format" + to_string(number) +  ".txt";
        read_graf(file_way, graf);
        int n = graf.size();
        unordered_map<string, int> graf_map;
        to_map(graf, graf_map, n);
        int start = get_nearby(1, 1, graf, n);
        int end = get_nearby(100, 100, graf, n);

        auto start_time = chrono::high_resolution_clock::now();
        BFS(start, end, graf, graf_map, n);
        auto end_time = chrono::high_resolution_clock::now();
        result << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << ' ';

        start_time = chrono::high_resolution_clock::now();
        Dijkstra(start, end, graf, graf_map, n);
        end_time = chrono::high_resolution_clock::now();
        result << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << ' ';

        start_time = chrono::high_resolution_clock::now();
        AStar(start, end, graf, graf_map, n);
        end_time = chrono::high_resolution_clock::now();
        result << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << endl;
    }

    result.close();
}

int main() {
    char temp_buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, temp_buffer);
    buffer = string(temp_buffer);

    cout << "1) Generate graph (2 types of storage)" << endl
    << "2) Testing with step" << endl;
    int function;
    cin >> function;
    if (function == 1) {
        int vertexCount;
        cout << "Amounts vertex: ";
        cin >> vertexCount;
        cout << "Graph have been generated" << endl;
    }else {
        int step, start_size, end_size;
        cout << "Start size: ";
        cin >> start_size;
        cout << "End size: ";
        cin >> end_size;
        cout << "Step: ";
        cin >> step;
        testingWithStep(start_size, end_size, step);
        cout << "Testing have been ended";
    }

    return 0;
}
