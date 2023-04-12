#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
#include <omp.h>

using namespace std;

// Pair of vertex and weight
typedef pair<int, int> iPair;

// Graph class
class Graph
{
    int V;                     // number of vertices
    vector<vector<iPair>> adj; // adjacency list

public:
    Graph(int V) // constructor
    {
        this->V = V;
        adj.resize(V);
    }

    // add an edge to the graph
    void addEdge(int u, int v, int w)
    {
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
    }

    // Dijkstra algorithm
    void dijkstra(int src)
    {
        // Create a priority queue to store vertices that are being processed
        priority_queue<iPair, vector<iPair>, greater<iPair>> pq;

        // Create a vector for distances and initialize all distances as infinite (INT_MAX)
        vector<int> dist(V, INT_MAX);

        // Insert source vertex into the priority queue and initialize its distance as 0
        pq.push(make_pair(0, src));
        dist[src] = 0;

        // Loop until the priority queue is empty
        while (!pq.empty())
        {
            // Extract the minimum distance vertex from the priority queue
            int u = pq.top().second;
            pq.pop();

            // Iterate through all adjacent vertices of u
            for (auto i = adj[u].begin(); i != adj[u].end(); ++i)
            {
                int v = i->first;
                int weight = i->second;

                // If there is a shorter path to v through u
                if (dist[v] > dist[u] + weight)
                {
                    // Update distance of v and push it into the priority queue
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        // Print the shortest distances from the source vertex
        cout << "Vertex\tDistance from Source\n";
        for (int i = 0; i < V; ++i)
            cout << i << "\t" << dist[i] << "\n";
    }
};

// Main function
int main()
{

    // Graph g(9); // create a graph with 9 vertices

    // // add edges to the graph
    // g.addEdge(0, 1, 4);
    // g.addEdge(0, 7, 8);
    // g.addEdge(1, 0, 4);
    // g.addEdge(1, 7, 11);
    // g.addEdge(1, 2, 8);
    // g.addEdge(2, 1, 8);
    // g.addEdge(2, 8, 2);
    // g.addEdge(2, 5, 4);
    // g.addEdge(2, 3, 7);
    // g.addEdge(3, 2, 7);
    // g.addEdge(3, 5, 14);
    // g.addEdge(3, 4, 9);
    // g.addEdge(4, 3, 9);
    // g.addEdge(4, 5, 10);
    // g.addEdge(5, 2, 4);
    // g.addEdge(5, 3, 14);
    // g.addEdge(5, 4, 10);
    // g.addEdge(6, 7, 1);
    // g.addEdge(6, 8, 6);
    // g.addEdge(6, 5, 2);
    // g.addEdge(7, 0, 8);
    // g.addEdge(7, 1, 11);
    // g.addEdge(7, 8, 7);
    // g.addEdge(7, 6, 1);
    // g.addEdge(8, 2, 2);
    // g.addEdge(8, 7, 7);
    // g.addEdge(8, 6, 6);

    Graph g(15); // create a graph with 15 vertices

    // add edges to the graph
    g.addEdge(0, 1, 6);
    g.addEdge(0, 2, 4);
    g.addEdge(0, 3, 3);
    g.addEdge(1, 0, 6);
    g.addEdge(1, 4, 5);
    g.addEdge(1, 5, 9);
    g.addEdge(2, 0, 4);
    g.addEdge(2, 5, 6);
    g.addEdge(2, 6, 8);
    g.addEdge(3, 0, 3);
    g.addEdge(3, 6, 2);
    g.addEdge(3, 7, 4);
    g.addEdge(4, 1, 5);
    g.addEdge(4, 8, 3);
    g.addEdge(4, 9, 7);
    g.addEdge(5, 1, 9);
    g.addEdge(5, 2, 6);
    g.addEdge(5, 10, 5);
    g.addEdge(6, 2, 8);
    g.addEdge(6, 3, 2);
    g.addEdge(6, 11, 6);
    g.addEdge(7, 3, 4);
    g.addEdge(7, 11, 3);
    g.addEdge(7, 12, 1);
    g.addEdge(8, 4, 3);
    g.addEdge(8, 9, 6);
    g.addEdge(8, 13, 9);
    g.addEdge(9, 4, 7);
    g.addEdge(9, 8, 6);
    g.addEdge(9, 14, 2);
    g.addEdge(10, 5, 5);
    g.addEdge(10, 11, 4);
    g.addEdge(10, 12, 3);
    g.addEdge(11, 6, 6);
    g.addEdge(11, 7, 3);
    g.addEdge(11, 10, 4);
    g.addEdge(12, 7, 1);
    g.addEdge(12, 10, 3);
    g.addEdge(12, 13, 2);
    g.addEdge(13, 8, 9);
    g.addEdge(13, 12, 2);
    g.addEdge(13, 14, 8);
    g.addEdge(14, 9, 2);
    g.addEdge(14, 13, 8);


    // Start the timer
    double t_start = omp_get_wtime();

    // Find the shortest paths from vertex 0
    g.dijkstra(0);

    // Calculate the time taken
    double time_taken = omp_get_wtime() - t_start;

    // Print the time taken
    printf("Time taken for the main part: %f\n", time_taken);


    return 0;
}
