#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <set>
#include <memory>
#include <cmath> // for sqrt function
#include <omp.h>

using namespace std;

const int INF = numeric_limits<int>::max();

void relax(int u, int v, int weight, vector<int> &distances, int delta, vector<set<int>> &buckets)
{
    int old_distance = distances[v];
    int new_distance = distances[u] + weight;
    if (new_distance < old_distance)
    {
        distances[v] = new_distance;
        int old_bucket = old_distance / delta;
        int new_bucket = new_distance / delta;
        if (old_distance != INF)
        {
            buckets[old_bucket].erase(v);
        }
        buckets[new_bucket].insert(v);
    }
}


void dijkstra(int source, vector<vector<pair<int, int>>> &graph, vector<int> &distances)
{
    int n = graph.size();
    distances.assign(n, INF);
    distances[source] = 0;

    // Use a set to keep track of unprocessed vertices
    set<pair<int, int>> unprocessed_vertices;
    for (int i = 0; i < n; ++i)
    {
        unprocessed_vertices.insert({distances[i], i});
    }

    while (!unprocessed_vertices.empty())
    {
        // Get the vertex with the smallest distance from the set
        int u = unprocessed_vertices.begin()->second;
        unprocessed_vertices.erase(unprocessed_vertices.begin());

        // Relax all the outgoing edges from the vertex
        for (pair<int, int> &edge : graph[u])
        {
            int v = edge.first;
            int weight = edge.second;
            int new_distance = distances[u] + weight;
            if (new_distance < distances[v])
            {
                unprocessed_vertices.erase({distances[v], v});
                distances[v] = new_distance;
                unprocessed_vertices.insert({distances[v], v});
            }
        }
    }
}

void delta_stepping(int source, vector<vector<pair<int, int>>> &graph, vector<int> &distances, int delta)
{
    int n = graph.size();
    distances.assign(n, INF);
    distances[source] = 0;

    int max_buckets = 3 * n;

    vector<set<int>> buckets(max_buckets);
    buckets[0].insert(source);

    for (int i = 0; i < max_buckets; ++i)
    {
        while (!buckets[i].empty())
        {
            set<int> S = buckets[i];
            buckets[i].clear();

            // Parallelize light edge relaxation loop
            #pragma omp parallel for schedule(dynamic)
            for (int u_index = 0; u_index < S.size(); u_index++)
            {
                int u = *next(S.begin(), u_index);
                for (pair<int, int> &edge : graph[u])
                {
                    int v = edge.first;
                    int weight = edge.second;
                    if (weight <= delta) // Relax light edges first
                    {
                        #pragma omp critical
                        relax(u, v, weight, distances, delta, buckets);
                    }
                }
            }

            // Parallelize heavy edge relaxation loop
            #pragma omp parallel for schedule(dynamic)
            for (int u_index = 0; u_index < S.size(); u_index++)
            {
                int u = *next(S.begin(), u_index);
                for (pair<int, int> &edge : graph[u])
                {
                    int v = edge.first;
                    int weight = edge.second;
                    if (weight > delta) // Relax heavy edges after light edges
                    {
                        #pragma omp critical
                        relax(u, v, weight, distances, delta, buckets);
                    }
                }
            }
        }
    }
}



int main()
{
    omp_set_num_threads(4);

    vector<vector<pair<int, int>>> graph = {
    {{1, 6}, {2, 4}, {3, 3}},
    {{0, 6}, {4, 5}, {5, 9}},
    {{0, 4}, {5, 6}, {6, 8}},
    {{0, 3}, {6, 2}, {7, 4}},
    {{1, 5}, {8, 3}, {9, 7}},
    {{1, 9}, {2, 6}, {10, 5}},
    {{2, 8}, {3, 2}, {11, 6}},
    {{3, 4}, {11, 3}, {12, 1}},
    {{4, 3}, {9, 6}, {13, 9}},
    {{4, 7}, {8, 6}, {14, 2}},
    {{5, 5}, {11, 4}, {12, 3}},
    {{6, 6}, {7, 3}, {10, 4}},
    {{7, 1}, {10, 3}, {13, 2}},
    {{8, 9}, {12, 2}, {14, 8}},
    {{9, 2}, {13, 8}}
    };

    vector<vector<pair<int, int>>> graph1 = {
        {{1, 4}, {7, 8}},
        {{0, 4}, {7, 11}, {2, 8}},
        {{1, 8}, {8, 2}, {5, 4}, {3, 7}},
        {{2, 7}, {5, 14}, {4, 9}},
        {{3, 9}, {5, 10}},
        {{6, 2}, {2, 4}, {3, 14}, {4, 10}},
        {{7, 1}, {8, 6}, {5, 2}},
        {{0, 8}, {1,11}, {8, 7}, {6, 1}},
        {{2, 2}, {7,7}, {6, 6}}
    };

    vector<int> distances;
    int delta = 1;
    
    // Start the timer
    double t_start = omp_get_wtime();

    // Call the delta_stepping() function
    delta_stepping(0, graph, distances, delta);

    // Call the dijkstra() function
    //dijkstra(0, graph1, distances);

    // Calculate the time taken
    double time_taken = omp_get_wtime() - t_start;

    // Print the time taken
    printf("Time taken for the main part: %f\n", time_taken);

    // Print the shortest distances from the source vertex
    cout << "Vertex\tDistance from Source\n";
    for (int i = 0; i < distances.size(); ++i)
            cout << i << "\t" << distances[i] << "\n";

    return 0;
}
