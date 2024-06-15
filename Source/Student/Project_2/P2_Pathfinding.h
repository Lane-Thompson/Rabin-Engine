#pragma once
#include "Misc/PathfindingDetails.hpp"
#include "Node.h"
#include "UnorderedFastArray.h"

class AStarPather
{
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    AStarPather();
    //~AStarPather();

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */
    void FormPath(Node goalNode, PathRequest& request);

    float ComputeHeuristicCost(GridPos start, GridPos goal, Heuristic heuristic, float weight);
    float Octile(GridPos start, GridPos goal);
    float Euclidean(GridPos start, GridPos goal);
    float Manhattan(GridPos start, GridPos goal);
    float Chebyshev(GridPos start, GridPos goal);
    float Inconsistent(GridPos start, GridPos goal);

    void PreCheckForValidNeighbors();

    UnorderedFastArray openList;

    Node grid[40][40];
    bool preprocessing;

    GridPos start;
    GridPos goal;
    bool debug_prints;

};