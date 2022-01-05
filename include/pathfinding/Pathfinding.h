/**
 * @file Pathfinding.h
 * @author Ryan Purse
 * @date 01/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include <functional>
#include <set>
#include <queue>

/**
 * @brief Creates a path based on the current node and where it came from recursively.
 * @tparam Node - The type of Node.
 * @param cameFrom - A map of nodes that can be traversed.
 * @param current - The starting node within the map.
 * @returns A node vector with a path that can be traveled.
 */
template<typename Node>
std::vector<Node> reconstructPath(
        const std::unordered_map<Node, Node> &cameFrom,
        Node current
        )
{
    std::vector<Node> path { current };
    auto it = cameFrom.find(current);
    while(it != cameFrom.end())
    {
        current = it->second;
        path.push_back(current);
        it = cameFrom.find(current);
    }
    // The path traced is backwards, so flip it before returning it.
    return std::vector<Node>(path.rbegin(), path.rend());
}

/**
 * @brief A node with an F-Score attached to it. Used for a priority queue.
 * @tparam T The type of data that you want to store.
 */
template<typename T>
struct FNode
{
    T node;
    float fScore;
    
    // Priority queues needs < to be defined.
    bool operator<(const FNode &rhs) const { return fScore > rhs.fScore; }
};

/**
 * @brief Finds and creates a path of nodes between the start node and the end node.
 * @tparam Node - The type of Node. It must have an equality operator.
 * @param start - The node the you want to start searching from.
 * @param end - The node that you are searching for.
 * @param adjacentFunc - A function that returns all adjacent nodes (Node -> std::vector\<Node\>)
 * @param heuristicFunc - A function to define the distance between two nodes. (Node, Node -> float)
 * @returns A path between [start, end], nothing if end was not reached.
 */
template<typename Node>
std::vector<Node> AStarGrid(
        const Node &start,
        const Node &end,
        const std::function<std::vector<Node>(Node)> &adjacentFunc,
        const std::function<float(Node, Node)> &heuristicFunc
        )
{
    std::unordered_map<Node, Node> cameFrom;
    std::unordered_map<Node, float> gScore { { start, 0.f } };  // How far it's travelled from start.
    
    std::priority_queue<FNode<Node>> pFScore;
    pFScore.push( { start, heuristicFunc(start, end) } );  // gScore + heuristicFunc Score.
    
    while (!pFScore.empty())
    {
        Node currentNode = pFScore.top().node;  // Get the next Node in the open set with the lowest fScore.
        
        if (currentNode == end)
            return reconstructPath(cameFrom, currentNode);
        
        pFScore.pop();
        
        for (Node adjacent : adjacentFunc(currentNode))
        {
            float adjacentScore = gScore[currentNode] + 1.f;  // All grid cells are equidistant. Add one to the Score.
    
            // Create a score of infinity if the node has never been discovered.
            if (gScore.find(adjacent) == gScore.end())
                gScore.insert({ adjacent, std::numeric_limits<float>::infinity() });
            
            if (adjacentScore < gScore[adjacent])
            {
                cameFrom[adjacent] = currentNode;
                gScore[adjacent] = adjacentScore;
                pFScore.push( { adjacent, adjacentScore + heuristicFunc(adjacent, end) });
            }
        }
    }
    
    debug::log("Failed to find path between the two points", debug::severity::Minor);
    return { };
}

/**
 * @brief Identical to AStarGrid\<\> but returns the path, OpenSet and the fScore.
 * @tparam Node - The type of Node. It must have an equality operator.
 * @param start - The node the you want to start searching from.
 * @param end - The node that you are searching for.
 * @param adjacentFunc - A function that returns all adjacent nodes (Node -> std::vector\<Node\>)
 * @param heuristicFunc - A function to define the distance between two nodes. (Node, Node -> float)
 * @returns tuple(path, OpenSet, fScore)
 */
template<typename Node>
std::tuple<std::vector<Node>, std::unordered_map<Node, float>> AStarGridAndInfo(
        const Node &start,
        const Node &end,
        const std::function<std::vector<Node>(Node)> &adjacentFunc,
        const std::function<float(Node, Node)> &heuristicFunc
)
{
    std::unordered_map<Node, Node> cameFrom;
    std::unordered_map<Node, float> gScore { { start, 0.f } };  // How far it's travelled from start.
    
    std::priority_queue<FNode<Node>> pFScore;
    pFScore.push( { start, heuristicFunc(start, end) } );  // gScore + heuristicFunc Score.
    
    while (!pFScore.empty())
    {
        Node currentNode = pFScore.top().node;  // Get the next Node in the open set with the lowest fScore.
        
        if (currentNode == end)
            return { reconstructPath(cameFrom, currentNode), gScore };
        
        pFScore.pop();
        
        for (Node adjacent : adjacentFunc(currentNode))
        {
            float adjacentScore = gScore[currentNode] + 1.f;  // All grid cells are equidistant. Add one to the Score.
            
            // Create a score of infinity if the node has never been discovered.
            if (gScore.find(adjacent) == gScore.end())
                gScore.insert({ adjacent, std::numeric_limits<float>::infinity() });
            
            if (adjacentScore < gScore[adjacent])
            {
                cameFrom[adjacent] = currentNode;
                gScore[adjacent] = adjacentScore;
                pFScore.push( { adjacent, adjacentScore + heuristicFunc(adjacent, end) });
            }
        }
    }
    
    debug::log("Failed to find path between the two points", debug::severity::Minor);
    return { };
}




// OLD COPY OF DEBUG PATHFINDING ALGORITHM
// template<typename Node>
// std::tuple<std::vector<Node>, std::set<Node>, std::unordered_map<Node, float>> AStarGridAndInfo(
//         const Node &start,
//         const Node &end,
//         const std::function<std::vector<Node>(Node)> &adjacentFunc,
//         const std::function<float(Node, Node)> &heuristicFunc
// )
// {
//     std::set<Node> openSet { start };  // The set of nodes that are next to be queried.
//     std::unordered_map<Node, Node> cameFrom;
//     std::unordered_map<Node, float> gScore { { start, 0.f } };  // How far it's travelled from start.
//     std::unordered_map<Node, float> fScore { { start, heuristicFunc(start, end) } };  // gScore + heuristicFunc Score.
//
//     while (!openSet.empty())
//     {
//         // Get the next Node in the open set with the lowest fScore.
//         Node currentNode = *std::min_element(openSet.begin(),  openSet.end(),
//                                              [&fScore](const Node &a, const Node &b) {
//                                                  return fScore[a] < fScore[b];
//                                              });
//
//         if (currentNode == end)
//             return { reconstructPath(cameFrom, currentNode), openSet, fScore };
//
//         openSet.erase(currentNode);  // It doesn't need to be queried again.
//
//         for (Node adjacent : adjacentFunc(currentNode))
//         {
//             float adjacentScore = gScore[currentNode] + 1.f;  // All grid cells are equidistant. Add one to the Score.
//
//             // Create a score of infinity if the node has never been discovered.
//             if (gScore.find(adjacent) == gScore.end())
//                 gScore.insert({ adjacent, std::numeric_limits<float>::infinity() });
//
//             if (adjacentScore < gScore[adjacent])
//             {
//                 cameFrom[adjacent] = currentNode;
//                 gScore[adjacent] = adjacentScore;
//                 fScore[adjacent] = adjacentScore + heuristicFunc(adjacent, end);
//                 openSet.insert(adjacent);  // An if statement isn't needed since we are using a set.
//             }
//         }
//     }
//
//     debug::log("Failed to find path between the two points", debug::severity::Minor);
//     return { };
// }
