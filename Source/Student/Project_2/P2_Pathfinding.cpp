#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

#define MAX_GRID_SIZE 40
#define sqrt2 1.14121356

AStarPather::AStarPather() : preprocessing(false)
{
}

bool AStarPather::initialize()
{
    // handle any one-time setup requirements you have

    /*
        If you want to do any map-preprocessing, you'll need to listen
        for the map change message.  It'll look something like this:

        Callback cb = std::bind(&AStarPather::your_function_name, this);
        Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

        There are other alternatives to using std::bind, so feel free to mix it up.
        Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
        object that std::function can wrap will suffice.
    */
    for (int x = 0; x < MAX_GRID_SIZE; ++x)
    {
        for (int y = 0; y < MAX_GRID_SIZE; ++y)
        {
            grid[x][y].listStatus = ListStatus::None;
        }
    }
    if (preprocessing)
    {
        PreCheckForValidNeighbors();
    }
    openList.Clear();

    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown()
{
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */
}

PathResult AStarPather::compute_path(PathRequest &request)
{
    /*
        This is where you handle pathing requests, each request has several fields:

        start/goal - start and goal world positions
        path - where you will build the path upon completion, path should be
            start to goal, not goal to start
        heuristic - which heuristic calculation to use
        weight - the heuristic weight to be applied
        newRequest - whether this is the first request for this path, should generally
            be true, unless single step is on

        smoothing - whether to apply smoothing to the path
        rubberBanding - whether to apply rubber banding
        singleStep - whether to perform only a single A* step
        debugColoring - whether to color the grid based on the A* state:
            closed list nodes - yellow
            open list nodes - blue

            use terrain->set_color(row, col, Colors::YourColor);
            also it can be helpful to temporarily use other colors for specific states
            when you are testing your algorithms

        method - which algorithm to use: A*, Floyd-Warshall, JPS+, or goal bounding,
            will be A* generally, unless you implement extra credit features

        The return values are:
            PROCESSING - a path hasn't been found yet, should only be returned in
                single step mode until a path is found
            COMPLETE - a path to the goal was found and has been built in request.path
            IMPOSSIBLE - a path from start to goal does not exist, do not add start position to path
    */

    // WRITE YOUR CODE HERE

    if (request.newRequest)
    {
        // Initialize everything.Clear Open / Closed Lists.​
        initialize();
        start = terrain->get_grid_position(request.start);
        goal = terrain->get_grid_position(request.goal);
        if (request.settings.debugColoring)
        {
            terrain->set_color(start, Colors::Orange);
            terrain->set_color(goal, Colors::Orange);
        }
        Node startNode(start, ComputeHeuristicCost(start, goal, request.settings.heuristic));
        // Push Start Node onto the Open List with cost of f(x) = g(x) + h(x).​
        openList.Push(startNode);
    }
    // While(Open List is not empty) {
    while (openList.Size())
    {
        // parentNode = Pop cheapest node off Open List.​
        Node parentNode = openList.Pop();
        // If parentNode is the Goal Node, then path found(return PathResult::COMPLETE).​
        if (parentNode == goal)
        {
            FormPath(parentNode, request);
            return PathResult::COMPLETE;
        }
        // Place parentNode on Closed List
        // for all neighboring child nodes
            // Compute cost
            // if child node isn't on open or closed list, put on open list
            // Else if child node is on Open or Closed List, AND this new one is cheaper, ​
            // then take the old expensive one off both lists and put this new​
            // cheaper one on the Open List.​


    // if request.settings.singleStep == true, ​
    // abort search for now and resume next frame(return PathResult::PROCESSING).​
    }
    //        Open List empty, thus no path possible(return PathResult::IMPOSSIBLE).
    return PathResult::IMPOSSIBLE;
}

void AStarPather::FormPath(Node goalNode, PathRequest request)
{
    request.path.push_back(Vec3(goalNode.xPos, goalNode.yPos, 0));
    Node* currentNode = goalNode.parent;
    while (currentNode != nullptr)
    {
        request.path.push_back(Vec3(currentNode->xPos, currentNode->yPos, 0));
        currentNode = currentNode->parent;
    }
    request.path.reverse();
}

float AStarPather::ComputeHeuristicCost(GridPos start, GridPos goal, Heuristic heuristic)
{
    switch (heuristic)
    {
    case (Heuristic::OCTILE):       return Octile(start, goal);
    case (Heuristic::EUCLIDEAN):    return Euclidean(start, goal);
    case (Heuristic::MANHATTAN):    return Manhattan(start, goal);
    case (Heuristic::CHEBYSHEV):    return Chebyshev(start, goal);
    case (Heuristic::INCONSISTENT): return Inconsistent(start, goal);
    default: return 0;
    }

}

float AStarPather::Octile(GridPos start, GridPos goal)
{
    float xDiff = abs(goal.col - start.col);
    float yDiff = abs(goal.row - start.row);
    return std::min(xDiff, yDiff) * sqrt2 + std::max(xDiff, yDiff) - std::min(xDiff, yDiff);
}

float AStarPather::Euclidean(GridPos start, GridPos goal)
{
    float xDiff = abs(goal.col - start.col);
    float yDiff = abs(goal.row - start.row);
    return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}

float AStarPather::Manhattan(GridPos start, GridPos goal)
{
    float xDiff = abs(goal.col - start.col);
    float yDiff = abs(goal.row - start.row);
    return xDiff + yDiff;
}

float AStarPather::Chebyshev(GridPos start, GridPos goal)
{
    float xDiff = abs(goal.col - start.col);
    float yDiff = abs(goal.row - start.row);
    return std::max(xDiff,yDiff);
}

float AStarPather::Inconsistent(GridPos start, GridPos goal)
{
    if ((start.row + start.col) % 2 > 0) return Euclidean(start, goal);
    return 0;
}

void AStarPather::PreCheckForValidNeighbors()
{
    for (int x = 0; x < MAX_GRID_SIZE; ++x)
    for (int y = 0; y < MAX_GRID_SIZE; ++y)
    {
        Node node = grid[x][y];
        node.validNeighbors = 0;          // Reset neighbors
        if (terrain->is_wall(x, y)) continue;   // No neighbors if this is a wall

        byte index = 1;
        for (int j = -1; j < 2; ++j)
        for (int k = -1; k < 2; ++k)
        {
            if (j == 0 && k == 0) continue;     // Don't check self
            bool valid = true;
            GridPos newPos;
            newPos.col = x + j;
            newPos.row = y + k;
            if (terrain->is_wall(newPos))       // If this neighbor is a wall
            {
                valid = false;
            }
            // if neighbors shared by new and old are walls, no diagonal moves allowed
            else if (terrain->is_wall(newPos.col, y) || terrain->is_wall(x, newPos.row))   
            {
                valid = false;
            }
            
            if (valid) node.validNeighbors += index;   // Set the value 
            index << 1;                                // shift that bit
        }
    }
}
