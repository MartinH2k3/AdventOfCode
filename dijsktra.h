#ifndef DIJSKTRA_H
#define DIJSKTRA_H
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <limits>
#include <utility>

class DijkstraSolver {
public:
    // Using a map for the adjacency. Each key: node, value: vector of (cost, neighbor) pairs.
    explicit DijkstraSolver(const std::map<size_t, std::vector<std::pair<size_t, size_t>>>& adjacency)
        : adjacency_list(adjacency) {}

    // Returns the shortest path cost from start to end.
    // If no path exists, returns std::numeric_limits<size_t>::max().
    size_t shortest_path_cost(size_t start, size_t end) {
        auto [dist, parent] = run_dijkstra(start);
        if (dist.find(end) == dist.end() || dist[end] == std::numeric_limits<size_t>::max()) {
            return std::numeric_limits<size_t>::max();
        }
        return dist[end];
    }

    // Returns the shortest path as a vector of nodes from start to end.
    // If no path exists, returns an empty vector.
    std::vector<size_t> shortest_path(size_t start, size_t end) {
        auto [dist, parent] = run_dijkstra(start);

        // If no path
        if (dist.find(end) == dist.end() || dist[end] == std::numeric_limits<size_t>::max()) {
            return {};
        }

        // Reconstruct the path by going backwards from end to start using the parent map
        std::vector<size_t> path;
        for (size_t current = end; current != std::numeric_limits<size_t>::max(); current = parent[current]) {
            path.push_back(current);
        }

        // The path is currently from end to start, so reverse it
        std::reverse(path.begin(), path.end());
        return path;
    }

private:
    std::map<size_t, std::vector<std::pair<size_t, size_t>>> adjacency_list;

    // Runs Dijkstra from a given start node and returns the distance and parent maps.
    // dist[node] = shortest distance from start to node
    // parent[node] = predecessor of node in the shortest path
    std::pair<std::map<size_t, size_t>, std::map<size_t, size_t>> run_dijkstra(size_t start) {
        std::map<size_t, size_t> dist;
        std::map<size_t, size_t> parent;

        // Initialize distances and parent for all known nodes
        for (const auto& kv : adjacency_list) {
            dist[kv.first] = std::numeric_limits<size_t>::max();
            parent[kv.first] = std::numeric_limits<size_t>::max(); // no parent
        }

        // If start node doesn't exist in adjacency_list, return empty results
        if (dist.find(start) == dist.end()) {
            return {dist, parent};
        }

        dist[start] = 0;

        // Min-heap priority queue: (distance, node)
        auto comp = [](const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b) {
            return a.first > b.first;
        };
        std::priority_queue<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>, decltype(comp)> pq(comp);
        pq.push({0, start});

        while (!pq.empty()) {
            auto [current_dist, node] = pq.top();
            pq.pop();

            if (current_dist > dist[node]) {
                continue; // outdated entry
            }

            // Explore neighbors
            auto it = adjacency_list.find(node);
            if (it != adjacency_list.end()) {
                for (auto& edge : it->second) {
                    size_t edge_cost = edge.first;
                    size_t neighbor = edge.second;
                    size_t new_dist = dist[node] + edge_cost;

                    // If this neighbor is not in dist map (not a key in adjacency_list),
                    // we should add it.
                    if (dist.find(neighbor) == dist.end()) {
                        dist[neighbor] = std::numeric_limits<size_t>::max();
                        parent[neighbor] = std::numeric_limits<size_t>::max();
                    }

                    if (new_dist < dist[neighbor]) {
                        dist[neighbor] = new_dist;
                        parent[neighbor] = node;
                        pq.push({new_dist, neighbor});
                    }
                }
            }
        }

        return {dist, parent};
    }
};

#endif //DIJSKTRA_H
