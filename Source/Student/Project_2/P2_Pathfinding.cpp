#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"
#include <cassert>
#include <cmath>

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

AStarPather::AStarPather() : preprocessing(true)
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
            grid[x][y].parent = nullptr;
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
        if (request.settings.debugColoring)
        {
            terrain->set_color(start, Colors::Orange);
            terrain->set_color(goal, Colors::Red);
        }
        Node* startNode = &grid[start.col][start.row];
        startNode->CalculateValues(ComputeHeuristicCost(start, goal, request.settings.heuristic, request.settings.weight));
        // Push Start Node onto the Open List with cost of f(x) = g(x) + h(x).​
        openList.Push(startNode);
    }
    // While(Open List is not empty) {
    while (openList.Size())
    {
        // parentNode = Pop cheapest node off Open List.​
        Node* parentNode = openList.Pop();
        // If parentNode is the Goal Node, then path found (return PathResult::COMPLETE).​
        if (*parentNode == goal)
        {
            //openList.PrintMostNodes();
            FormPath(*parentNode, request);
            if (request.settings.debugColoring) terrain->set_color(goal, Colors::Green);
            return PathResult::COMPLETE;
        }
        //if (!preprocessing) parentNode->validNeighbors =  CheckValidNeighbors(parentNode->xPos,parentNode->yPos);
        // Place parentNode on Closed List
        parentNode->listStatus = ListStatus::Closed;
        if (request.settings.debugColoring) terrain->set_color(parentNode->GetGridPos(), Colors::Yellow);
        // for all neighboring child nodes
        int index = 1;
        for (int colChange = -1; colChange < 2; ++colChange)
        for (int rowChange = -1; rowChange < 2; ++rowChange)
        {
            if (colChange == 0 && rowChange == 0) continue;    // If both are 0, it's self. skip
            if (~parentNode->validNeighbors & index) // if not a valid neighbor, skip
            {
                index = index << 1;
                continue;
            }

            int newCol = parentNode->xPos + colChange;
            int newRow = parentNode->yPos + rowChange; 

            // Compute cost
            float hCost = ComputeHeuristicCost(GridPos{newRow,newCol}, goal, request.settings.heuristic, request.settings.weight);
            Node* neighbor = &grid[newCol][newRow];
            // if child node isn't on open or closed list, put on open list
            if (neighbor->listStatus == ListStatus::None)
            {
                neighbor->parent = parentNode;
                neighbor->CalculateValues(hCost, parentNode);
                openList.Push(neighbor);
                neighbor->listStatus = ListStatus::Open;
                if (request.settings.debugColoring) terrain->set_color(newRow,newCol, Colors::Blue);
            }
            else
            {
                // Else if child node is on Open or Closed List, AND this new one is cheaper
                float distanceToParent = colChange && rowChange ? sqrt2 : 1;    // if both col and row changed, this is diagonal
                float gCost = parentNode->givenCost + distanceToParent;
                if (neighbor->finalCost > gCost + hCost)
                {
                    // then take the old expensive one off both lists
                    if (neighbor->listStatus == ListStatus::Open) openList.Remove(neighbor);
                    // and put this new​ cheaper one on the Open List.​
                    neighbor->parent = parentNode;
                    neighbor->givenCost = gCost;
                    neighbor->finalCost = gCost + hCost;
                    openList.Push(neighbor);
                    neighbor->listStatus = ListStatus::Open;
                    if (request.settings.debugColoring) terrain->set_color(neighbor->GetGridPos(), Colors::Blue);
                }
            }
            index = index << 1;
        }

        // if request.settings.singleStep == true, ​
        // abort search for now and resume next frame(return PathResult::PROCESSING).​
        if (request.settings.singleStep == true) return PathResult::PROCESSING;
    }
    // Open List empty, thus no path possible(return PathResult::IMPOSSIBLE).
    return PathResult::IMPOSSIBLE;
}

void AStarPather::FormPath(Node goalNode, PathRequest& request)
{
    std::vector<Vec3> path;
    request.path.push_back(request.goal);
    Node* currentNode = goalNode.parent;
    while (currentNode != nullptr)
    {
        request.path.push_back(terrain->get_world_position(currentNode->yPos,currentNode->xPos));
        currentNode = currentNode->parent;
    }
    if (request.settings.rubberBanding) RubberbandPath(request.path);
    request.path.reverse();
    if (request.settings.smoothing)
    {
        AddIntermediatePoints(request.path);
        SmoothPath(request.path);
    }
}

int intSign(int num)
{
    int result = 0;
    if (num > 0) result = 1;
    else if (num < 0) result = -1;
    return result;
}

void AStarPather::RubberbandPath(WaypointList& path)
{
    WaypointList::iterator first  = path.begin();
    WaypointList::iterator second = path.begin();
    std::advance(second, 2);
    while(second != path.end())
    {
        GridPos firstPos  = terrain->get_grid_position(*first);
        GridPos secondPos = terrain->get_grid_position(*second);
        int colStart = std::min(firstPos.col, secondPos.col);
        int colEnd   = std::max(firstPos.col, secondPos.col);
        int rowStart = std::min(firstPos.row, secondPos.row);
        int rowEnd   = std::max(firstPos.row, secondPos.row);
        bool canCut = true;
        // if the two positions are in the same col or row, this for loop skips and cuts
        for (int col = colStart; col <= colEnd; ++col)
        {
            for (int row = rowStart; row <= rowEnd; ++row)
            {
                if (terrain->is_wall(row, col))
                {
                    canCut = false;
                    break;
                }
            }
            if (!canCut) break;
        }
        first++;
        if (canCut)
        {
            path.erase(first);
            first = path.begin();
            second = first;
            second++;
        }
        second++;
    }
}

void AStarPather::AddIntermediatePoints(WaypointList& path)
{
    bool changed;
    WaypointList result;
    do
    {
        changed = false;
        result.clear();
        WaypointList::iterator p1 = path.begin();
        WaypointList::iterator p2 = path.begin();
        p2++;

        result.push_back(*p1);
        while (p2 != path.end())
        {
            GridPos pos1 = terrain->get_grid_position(*p1);
            GridPos pos2 = terrain->get_grid_position(*p2);
            double distance = sqrt(pow((pos2.col - pos1.col), 2) + pow((pos2.row - pos1.row), 2));
            double worldDistance = Vec3::Distance(*p1, *p2) / 2.50;
            //printf("distance %d,%d -> %d,%d  %f\n",pos1.col,pos1.row,pos2.col,pos2.row, distance);
            //printf("distance %f,%f,%f -> %f,%f,%f  %f\n", (*p1).x, (*p1).y, (*p1).z, (*p2).x, (*p2).y, (*p2).z, Vec3::Distance(*p1, *p2)/2.50);
            
            if (worldDistance > 1.5)
            {
                Vec3 newPoint = *p1 + (*p2 - *p1) * 0.5;
                GridPos gridPoint = terrain->get_grid_position(newPoint);
                //printf("\tnew grid point: %d,%d\n", gridPoint.col, gridPoint.row);
                //printf("\tnew world point: %f,%f,%f\n", newPoint.x, newPoint.y, newPoint.z);
                result.push_back(newPoint);
                changed = true;
            }
            result.push_back(*p2);
            p1++;
            p2++;
        }
        path = result;
    } while (changed);
}

void AStarPather::SmoothPath(WaypointList& path)
{
    if (path.size() < 3)
    {
        return;
    }
    WaypointList result;
    WaypointList::iterator p1 = path.begin();
    WaypointList::iterator p2 = path.begin();
    std::advance(p2, 1);
    WaypointList::iterator p3 = path.begin();
    std::advance(p3, 2);
    WaypointList::iterator p4 = path.begin();
    std::advance(p4, 3);
    result.push_back(Vec3::CatmullRom(*p1, *p1, *p2, *p3,.20f));
    result.push_back(Vec3::CatmullRom(*p1, *p1, *p2, *p3,.50f));
    result.push_back(Vec3::CatmullRom(*p1, *p1, *p2, *p3,.80f));
    size_t limit = path.size() - 3;
    for (int i = 0; i < limit; i++)
    {
        result.push_back(Vec3::CatmullRom(*p1, *p2, *p3, *p4, .20f));
        result.push_back(Vec3::CatmullRom(*p1, *p2, *p3, *p4, .50f));
        result.push_back(Vec3::CatmullRom(*p1, *p2, *p3, *p4, .80f));
        if (i < limit - 1)
        {
            p1++;
            p2++;
            p3++;
            p4++;
        }
    }
    result.push_back(Vec3::CatmullRom(*p2, *p3, *p4, *p4, .20f));
    result.push_back(Vec3::CatmullRom(*p2, *p3, *p4, *p4, .50f));
    result.push_back(Vec3::CatmullRom(*p2, *p3, *p4, *p4, .80f));
    result.push_back(*p4);

    path = result;
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
    return static_cast<float>(sqrt((xDiff * xDiff) + (yDiff * yDiff)));
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

byte AStarPather::CheckValidNeighbors(int parentCol, int parentRow)
{
    if (terrain->is_wall(parentRow,parentCol)) return 0;
    byte index = 1;
    byte validNeighbors = 0;
    for (int col = -1; col < 2; ++col)
    for (int row = -1; row < 2; ++row)
    {
        if (col == 0 && row == 0) continue;     // Don't check self
        bool valid = true;
        int newCol = parentCol + col;
        int newRow = parentRow + row;
        // if this position falls outside of the world
        if (!terrain->is_valid_grid_position(newRow, newCol)) valid = false;
        // If this neighbor is a wall
        else if (terrain->is_wall(newRow, newCol)) valid = false;
        // if neighbors shared by new and old are walls, no diagonal moves allowed
        else if (col && row)
            if (terrain->is_wall(parentRow, newCol) || terrain->is_wall(newRow, parentCol))
                valid = false;

        if (valid)
        {
            validNeighbors += index;   // Set the value 
        }
        index = index << 1;            // shift that bit
    }
    return validNeighbors;
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
        node->validNeighbors = CheckValidNeighbors(node->xPos, node->yPos);
    }
}
