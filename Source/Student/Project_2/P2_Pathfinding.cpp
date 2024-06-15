#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"
#include "cassert"

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
constexpr auto sqrt2 = 1.41421356f;

AStarPather::AStarPather() : preprocessing(true), debug_prints(false)
{
    Callback cb = std::bind(&AStarPather::PreCheckForValidNeighbors, this);
    Messenger::listen_for_message(Messages::MAP_CHANGE, cb);


    for (int x = 0; x < MAX_GRID_SIZE; ++x)
    {
        for (int y = 0; y < MAX_GRID_SIZE; ++y)
        {
            Node* node = &grid[x][y];
            node->xPos = x;
            node->yPos = y;
        }
    }
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
        // Initialize everything.Clear Open / Closed Lists.
        initialize();
        start = terrain->get_grid_position(request.start);
        goal = terrain->get_grid_position(request.goal);
        if (debug_prints) printf("Going from col: %d row: %d to col: %d row %d\n", start.col, start.row, goal.col, goal.row);
        if (request.settings.debugColoring)
        {
            terrain->set_color(start, Colors::Orange);
            terrain->set_color(goal, Colors::Red);
        }
        Node* startNode = &grid[start.col][start.row];
        startNode->CalculateValues(ComputeHeuristicCost(start, goal, request.settings.heuristic, request.settings.weight));
        startNode->parent = nullptr;
        //return PathResult::IMPOSSIBLE;
        // Push Start Node onto the Open List with cost of f(x) = g(x) + h(x).​
        openList.Push(startNode);
    }
    // While(Open List is not empty) {
    while (openList.Size())
    {
        // parentNode = Pop cheapest node off Open List.​
        Node* parentNode = openList.Pop();
        // If parentNode is the Goal Node, then path found(return PathResult::COMPLETE).​
        if (*parentNode == goal)
        {
            FormPath(*parentNode, request);
            if (request.settings.debugColoring) terrain->set_color(goal, Colors::Green);
            return PathResult::COMPLETE;
        }
        // Place parentNode on Closed List
        parentNode->listStatus = ListStatus::Closed;
        if (request.settings.debugColoring) terrain->set_color(parentNode->GetGridPos(), Colors::Yellow);
        // for all neighboring child nodes
        int index = 1;
        if(debug_prints) printf("%d %d\n", parentNode->xPos, parentNode->yPos);
        for (int x = -1; x < 2; ++x)
        for (int y = -1; y < 2; ++y)
        {
            if (!(x || y)) continue;    // If both are 0, it's self. skip
            if(debug_prints) printf("\t%d/%d neighbor %d valid: %d\n", parentNode->xPos + x, parentNode->yPos + y, index, parentNode->validNeighbors & index);
            if (~parentNode->validNeighbors & index) // if not a valid neighbor, skip
            {
                index = index << 1;
                continue;
            }
            GridPos newPos { static_cast<int>(parentNode->yPos + y), static_cast<int>(parentNode->xPos + x) };
            assert(terrain->is_valid_grid_position(newPos));
            // Compute cost
            float hCost = ComputeHeuristicCost(newPos, goal, request.settings.heuristic, request.settings.weight);
            //Node neighbor = Node(x,y,hCost,&parentNode);
            Node* neighbor = &grid[newPos.col][newPos.row];
            // if child node isn't on open or closed list, put on open list
            if (neighbor->listStatus == ListStatus::None)
            {
                neighbor->parent = parentNode;
                neighbor->CalculateValues(hCost, parentNode);
                openList.Push(neighbor);
                if (request.settings.debugColoring) terrain->set_color(newPos, Colors::Blue);
            }
            else
            {
                // Else if child node is on Open or Closed List, AND this new one is cheaper
                float distanceToParent = x && y ? sqrt2 : 1;
                float gCost = parentNode->givenCost + distanceToParent;
                if (neighbor->finalCost > gCost + hCost)
                {
                    // then take the old expensive one off both lists
                    if (neighbor->listStatus == ListStatus::Open) openList.Remove(neighbor);
                    // and put this new​ cheaper one on the Open List.​
                    neighbor->parent = parentNode;
                    neighbor->finalCost = gCost + hCost;
                    openList.Push(neighbor);
                    if (request.settings.debugColoring) terrain->set_color(neighbor->GetGridPos(), Colors::Blue);
                }
            }
            index = index << 1;
        }


        // if request.settings.singleStep == true, ​
        // abort search for now and resume next frame(return PathResult::PROCESSING).​
        if (request.settings.singleStep == true) return PathResult::PROCESSING;
    }
    //        Open List empty, thus no path possible(return PathResult::IMPOSSIBLE).
    return PathResult::IMPOSSIBLE;
}

void AStarPather::FormPath(Node goalNode, PathRequest& request)
{
    request.path.push_back(request.goal);
    Node* currentNode = goalNode.parent;
    while (currentNode != nullptr)
    {
        request.path.push_back(terrain->get_world_position(currentNode->GetGridPos()));
        currentNode = currentNode->parent;
    }
    request.path.reverse();
}

float AStarPather::ComputeHeuristicCost(GridPos start, GridPos goal, Heuristic heuristic, float weight)
{
    float hCost;
    switch (heuristic)
    {
    case (Heuristic::OCTILE):       hCost = Octile(start, goal); break;
    case (Heuristic::EUCLIDEAN):    hCost = Euclidean(start, goal); break;
    case (Heuristic::MANHATTAN):    hCost = Manhattan(start, goal); break;
    case (Heuristic::CHEBYSHEV):    hCost = Chebyshev(start, goal); break;
    case (Heuristic::INCONSISTENT): hCost = Inconsistent(start, goal); break;
    default: return 0;
    }
    return hCost * weight;

}

float AStarPather::Octile(GridPos start, GridPos goal)
{
    unsigned int xDiff = abs(goal.col - start.col);
    unsigned int yDiff = abs(goal.row - start.row);
    return std::min(xDiff, yDiff) * sqrt2 + std::max(xDiff, yDiff) - std::min(xDiff, yDiff);
}

float AStarPather::Euclidean(GridPos start, GridPos goal)
{
    unsigned int xDiff = abs(goal.col - start.col);
    unsigned int yDiff = abs(goal.row - start.row);
    return static_cast<float>(sqrt((xDiff << 1) + (yDiff << 1)));
}

float AStarPather::Manhattan(GridPos start, GridPos goal)
{
    unsigned int xDiff = abs(goal.col - start.col);
    unsigned int yDiff = abs(goal.row - start.row);
    return static_cast<float>(xDiff + yDiff);
}

float AStarPather::Chebyshev(GridPos start, GridPos goal)
{
    unsigned int xDiff = abs(goal.col - start.col);
    unsigned int yDiff = abs(goal.row - start.row);
    return static_cast<float>(std::max(xDiff,yDiff));
}

float AStarPather::Inconsistent(GridPos start, GridPos goal)
{
    if ((start.col + start.row) % 2 > 0) return Euclidean(start, goal);
    return 0;
}

void AStarPather::PreCheckForValidNeighbors()
{
    if (!preprocessing) return;
    bool unknown_size = true;
    int max_grid_size = MAX_GRID_SIZE;
    for (int x = 0; x < max_grid_size; ++x)
    for (int y = 0; y < max_grid_size; ++y)
    {
        if (unknown_size && !terrain->is_valid_grid_position(y, x))
        {
            max_grid_size = std::max(y, x);
            unknown_size = false;
            continue;
        }
        Node* node = &grid[x][y];
        node->validNeighbors = 0;          // Reset neighbors
        if (terrain->is_wall(y,x)) continue;   // No neighbors if this is a wall

        byte index = 1;
        for (int j = -1; j < 2; ++j)
        for (int k = -1; k < 2; ++k)
        {
            if (j == 0 && k == 0) continue;     // Don't check self
            bool valid = true;
            GridPos newPos { y + k, x + j };
            // if this position falls outside of the world
            if (!terrain->is_valid_grid_position(newPos)) valid = false;
            // If this neighbor is a wall
            else if (terrain->is_wall(newPos)) valid = false;
            // if neighbors shared by new and old are walls, no diagonal moves allowed
            else if (j && k)
                if (terrain->is_wall(y, newPos.col) || terrain->is_wall(newPos.row, x))
                    valid = false;
            
            if (valid)
            {
                node->validNeighbors += index;   // Set the value 
                if(debug_prints) printf("\tadding %d\t%d %d\t%d %d\n", index, newPos.col,newPos.row,j,k);
            }
            index = index << 1;                        // shift that bit
        }
        if(debug_prints) printf("%d %d Valid neighbors: %d\n", x,y,node->validNeighbors);
    }
}
