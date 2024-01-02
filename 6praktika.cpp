#include <iostream>
#include <limits>
#include <vector>
#include <list>

using namespace std;


struct Edge {
    int target;
    int weight;

    Edge(int t, int w) : target(t), weight(w) {}
};

class Graph {
private:
    int vertices;
    vector<list<Edge>> adjacencyList;

public:
    Graph(int v) : vertices(v), adjacencyList(v) {}

    void addEdge(int from, int target, int weight) {
        adjacencyList[from].emplace_back(target, weight);
    }

    void outputGraph() {
        for (int i = 0; i < vertices; ++i) {
            cout << "Вершина " << i << ": ";
            for (const auto& edge : adjacencyList[i]) {
                cout << "(" << edge.target << ", " << edge.weight << ") ";
            }
            cout << std::endl;
        }
    }

    int getDegree(int vertex) const {
        return adjacencyList[vertex].size();
    }

    void dijkstra(int from, int target) {
        vector<int> distance(vertices, numeric_limits<int>::max());
        distance[from] = 0;

        vector<bool> visited(vertices, false);

        for (int count = 0; count < vertices - 1; ++count) {
            int minDistance = numeric_limits<int>::max();
            int minIndex = -1;

            for (int v = 0; v < vertices; ++v) {
                if (!visited[v] && distance[v] <= minDistance) {
                    minDistance = distance[v];
                    minIndex = v;
                }
            }

            visited[minIndex] = true;

            for (const auto& edge : adjacencyList[minIndex]) {
                int newDistance = distance[minIndex] + edge.weight;
                if (!visited[edge.target] && newDistance < distance[edge.target]) {
                    distance[edge.target] = newDistance;
                }
            }
        }

        cout << "Кратчайший путь из " << from << " в " << target << ": ";
        printPath(from, target, distance);
        cout << "Длина пути: " << distance[target] << endl;
    }

    void printPath(int from, int target, const vector<int>& distance) {
        if (distance[target] == numeric_limits<int>::max()) {
            cout << "Несвязанный граф" << endl;
            return;
        }

        vector<int> path;
        int current = target;

        while (current != from) {
            path.push_back(current);
            current = getParent(current, distance);
        }

        path.push_back(from);

        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            cout << *it;
            if (it + 1 != path.rend()) {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    int getParent(int vertex, const vector<int>& target) {
        for (int v = 0; v < vertices; ++v) {
            for (const auto& edge : adjacencyList[v]) {
                if (edge.target == vertex && target[v] + edge.weight == target[vertex]) {
                    return v;
                }
            }
        }
        return -1;
    }
};

int main() {
    setlocale(LC_ALL, "ru");

    int v, e;
    cout << "Введите количество вершин и ребер: " << endl;
    cin >> v >> e;

    Graph graph(v);

    cout << "Введите ребра (откуда куда вес)" << endl;
    for (int i = 0; i < e; ++i) {
        int source, destination, weight;
        cin >> source >> destination >> weight;
        graph.addEdge(source, destination, weight);
    }

    cout << "Граф:" << endl;
    graph.outputGraph();

    int vertex;
    cout << "Введите вершину, для которой необходимо найти степень: ";
    cin >> vertex;
    cout << "Степень вершины " << vertex << ": " << graph.getDegree(vertex) << endl;

    int from, target;
    cout << "Введите вершины, между которыми необходимо найти кратчайший путь: " << endl;
    cin >> from >> target;

    graph.dijkstra(from, target);

    return 0;
}