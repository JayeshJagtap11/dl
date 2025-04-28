#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

const int MAX = 1000;
vector<int> graph[MAX];
bool visited[MAX];

void parallel_bfs(int start_node, int n) {
    queue<int> q;
    q.push(start_node);
    visited[start_node] = true;

    while (!q.empty()) {
        int qSize = q.size();
        vector<int> currentLevel;

        for (int i = 0; i < qSize; i++) {
            int u = q.front();
            q.pop();
            currentLevel.push_back(u);
        }

        #pragma omp parallel for
        for (int i = 0; i < currentLevel.size(); i++) {
            int u = currentLevel[i];
            cout << u << " ";

            for (int v : graph[u]) {
                #pragma omp critical
                {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    }
}

int main() {
    int n, m, start_node;
    cout << "Enter number of nodes, edges, and start node: ";
    cin >> n >> m >> start_node;

    cout << "Enter the edges (u v):\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // Assuming undirected graph
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    double start = omp_get_wtime();
    parallel_bfs(start_node, n);
    double end = omp_get_wtime();

    cout << "\nExecution Time: " << (end - start) << " seconds\n";

    return 0;
}
