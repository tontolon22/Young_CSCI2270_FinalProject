#include "Graph.h"
#include <queue>
#include <stack>
#include <iostream>
#include <climits>

using namespace std;

Graph::Graph()
{

}

Graph::~Graph()
{

}
/*
Function prototype:
void createVertex(std::string)

Function description:
This method adds a vertex of the input name to the graph.

Example:
Graph cities;
cities.createVertex("New York");

Precondition:
The string is the vertex name. A vertex by that name does not already exist.

Post condition:
A vertex by the input name exists in the graph.

*/
void Graph::createVertex(string name) //creates a vertex
{
    vertices.push_back(vertex(name));
}

/*
Function prototype:
void createEdge(std::string,std::string,int)

Function description:
This method adds an edge from the first input string to the second input string, with a weight of the input int.

Example:
Graph cities;
cities.createEdge("New York", "Los Angeles", 123);

Precondition:
Both input vertices must exist in the graph. There must no already be an edge in that direction between the two vertices.

Post condition:
An edge from the first input string to the second input string with a weight of the input int is in the graph.

*/
void Graph::createEdge(string nameSource, string nameDestination, int distance) //creates a one way edge
{
    vertex* destination; // stores pointer for destination vertex
    for (int i = 0; i < vertices.size(); i++) // finds destination vertex
    {
        if (nameDestination == vertices.at(i).name)
        {
            destination = &vertices.at(i); // stores pointer to destination vertex
        }
    }
    for (int i = 0; i < vertices.size(); i++) // finds source vertex
    {
        if (nameSource == vertices.at(i).name)
        {
            vertices.at(i).edges.push_back(edge(destination, distance)); // creates the edge
        }
    }
}

/*
Function prototype:
void deleteVertex(std::string)

Function description:
Deletes a vertex and all edges from or to that vertex.

Example:
graph.cities;
cities.deleteVertex("New York");

Precondition:
A vertex by the input string must exist

Post condition:
There is no vertex by the input string, or and edges point to it, in the graph.

*/
void Graph::deleteVertex(string vertexName)
{
    int vertexLoc;
    for (int i = 0; i< vertices.size(); i++) // goes though all vertices
    {
        if (vertices.at(i).name != vertexName) // skips the vertex to be deleted
        {
            for (int j = 0; j < vertices.at(i).edges.size(); j++) // searches the current vertex for edges to the vertex to be deleted
            {
                if (vertices.at(i).edges.at(j).destination->name == vertexName)
                {
                    vertices.at(i).edges.erase(vertices.at(i).edges.begin() + j); // deletes the edge
                }
            }
        }
        else
        {
            vertexLoc = i; // stores location of vertex to be deleted
        }
    }
    vertices.erase(vertices.begin() + vertexLoc); // deletes vertex
}

/*
Function prototype:
void deleteEdge(std::string,std::string)

Function description:
Removes an edge from the graph.

Example:
graph.cities;
cities.deleteEdge("New York", "Los Angeles");

Precondition:
There must be vertices with of the input names, and an edge between them in the direction of the first name to the second name.

Post condition:
There is no edge going from the first input string vertex to the second input string vertex.

*/
void Graph::deleteEdge(string startVertex, string endVertex) // deletes an edge
{
    for (int i = 0; i < vertices.size(); i++) // searches for the start vertex
    {
        if (vertices.at(i).name == startVertex)
        {
            for (int j = 0; j < vertices.at(i).edges.size(); j++) // searches the start vertex for edges to the end vertex
            {
                if (vertices.at(i).edges.at(j).destination->name == endVertex)
                {
                    vertices.at(i).edges.erase(vertices.at(i).edges.begin() + j); // deletes the edge
                }
            }
        }
    }
}

/*
Function prototype:
std::vector<std::vector<edgeData> > getGraph()

Function description:
Gets the information about the layout of the entire graph. In the vector returned there is a vector for each vertex. These vectors contain the edgeData structure. The edge data Structure has a string called name, and an int called distance. The first edgeData in a vector has the name of a vertex and a distance of 0. Each subsequent edgeData contains the name of a vertex the first vertex has an edge to, and the distance is the weight of the vertex.

Example:
Graph cities;
vector<vector<edgeData> > citiesLayout = getGraph();

Precondition:
The graph must be declared.

Post condition:
Returns a vector of a vector of the edgeData structure.

*/
vector<vector<edgeData> > Graph::getGraph()
{
    vector<vector<edgeData> > information;
    for (int i = 0; i < vertices.size(); i++)
    {
        vector<edgeData> vertexInfo;
        vertexInfo.push_back(edgeData(vertices.at(i).name,0));
        for (int j = 0; j < vertices.at(i).edges.size(); j++)
        {
            vertexInfo.push_back(edgeData(vertices.at(i).edges.at(j).destination->name,vertices.at(i).edges.at(j).distance));
        }
        information.push_back(vertexInfo);
    }
    return information;
}

/*
Function prototype:
pathData getShortestNodePath(std::string,std::string)

Function description:
The method finds the shortest path from the first input string to the second input string, returning the path and distance as a pathData structure. The pathData structure has an integer distance which is the weight distance traversed, and a vector of strings which is the names of the vertices traversed.

Example:
graph.cities;
cities.getShortestNodePath("New York", "Los Angeles");

Precondition:
Both input strings must exist as vertices, and there must be a path between them.

Post condition:
Returns a pathData structure.

*/
pathData Graph::getShortestNodePath(string sourceVertex, string destinationVertex) // gets the shortest path from the start vertex to the end vertex, by vertex traversals
{
    queue<vertex*> que; // declares a queue to store vertex
    vertex* endVertex; // variable to store end vertex
    for (int i = 0; i < vertices.size(); i++) // goes through each vertex to set up initial conditions
    {
        vertices.at(i).visited = false;
        vertices.at(i).source = NULL;
        if (sourceVertex == vertices.at(i).name) // checks if the vertex is the starting vertex
        {
            que.push(&vertices.at(i));
            vertices.at(i).visited = true;
        }
        if (destinationVertex == vertices.at(i).name) // checks if the vertex is the ending vertex
        {
            endVertex = &vertices.at(i);
        }
    }
    while (! que.empty()) // proceeds through the queue
    {
        vertex* working = que.front(); // gets the value in the front of the queue
        que.pop();
        for (int i = 0; i < working->edges.size(); i++) // sets unnassigned vertices to have the current vertex as their source
        {
            if ( working->edges.at(i).destination->visited == false)
            {
                working->edges.at(i).destination->visited = true;
                working->edges.at(i).destination->source = working;
                que.push(working->edges.at(i).destination);
                //cout << working->name << " to " << working->edges.at(i).destination->name << endl; // debug line
            }
        }
    }
    stack<vertex*> route; // creates a stack to flip the order in
    vertex* working = endVertex;
    while (working != NULL) // goes from end to start, putting all vertices on the stack
    {
        route.push(working);
        //cout << working << endl; // debug line
        //cout << working->name << endl; // debug line
        working = working->source;
    }
    pathData shortestPath;
    shortestPath.distance = 0;
    while (! route.empty()) // goes through stack, removing vertices and putting them into path vertex, also calculates distance
    {
        working = route.top();
        route.pop();
        shortestPath.path.push_back(working->name);
        for (int i = 0; i < working->edges.size(); i++)
        {
            if ( working->edges.at(i).destination == route.top())
            {
                shortestPath.distance += working->edges.at(i).distance;
            }
        }
    }
    return shortestPath;
}

/*
Function prototype:
pathData getShortestDistancePathDijikstras(std::string,std::string)

Function description:
Finds the shortest path from the first input string to the second input string using Dijkstra's algorithm, and then returns the information as a pathData structure. The pathData structure has an integer distance which is the weight distance traversed, and a vector of strings which is the names of the vertices traversed.

Example:
graph.cities;
cities.getShortestDistancePathDijikstras("New York", "Los Angeles");

Precondition:
Both input strings must exist as vertices, and there must be a path between them.

Post condition:
Returns a pathData structure.

*/
pathData Graph::getShortestDistancePathDijikstras(string startVertex,string endVertex)
{
    vertex* startPoint;
    vertex* endPoint;
    vector<vertex*> solved;
    for (int i = 0; i <vertices.size(); i++) // goes through all vertices to establish inital conditions and find start and end vertices
    {
        vertices.at(i).visited = false;
        vertices.at(i).source = NULL;
        vertices.at(i).distance = INT_MAX;
        if (vertices.at(i).name == startVertex) // checks for start vertex
        {
            vertices.at(i).distance = 0;
            vertices.at(i).visited = true;
            solved.push_back(&vertices.at(i));
            startPoint = &vertices.at(i);
        }
        if (vertices.at(i).name == endVertex) // checks for end vertex
        {
            endPoint = &vertices.at(i);
        }
    }
    while(! endPoint->visited) // runs till solution for end vertex has been found
    {
        for (int i = 0; i < solved.size(); i++) // goes through each solved vertex
        {
            for (int j = 0; j < solved.at(i)->edges.size(); j++) // goes through all edges for current vertex
            {
                if ( (! solved.at(i)->edges.at(j).destination->visited) && ((solved.at(i)->edges.at(j).destination->distance > solved.at(i)->distance + solved.at(i)->edges.at(j).distance) ) ) // checks and updates distance for the vertex that the edge goes to
                    {
                        solved.at(i)->edges.at(j).destination->distance = solved.at(i)->distance + solved.at(i)->edges.at(j).distance;
                        solved.at(i)->edges.at(j).destination->source = solved.at(i);
                    }
            }
        }
        vertex* minVertex = NULL;
        for (int i = 0; i < vertices.size(); i++) // finds the lowest distance unsolved vertex
        {
            if (! vertices.at(i).visited)
            {
                if (minVertex == NULL)
                {
                    minVertex = &vertices.at(i);
                }
                else
                {
                    if (minVertex->distance > vertices.at(i).distance)
                    {
                        minVertex = &vertices.at(i);
                    }
                }
            }
        }
        minVertex->visited = true; // set the vertex as solved
        solved.push_back(minVertex);
    }
    stack<string> route; // puts the vertex names into a stack to flip the order
    vertex* working = endPoint;
    while (working != NULL)
    {
        route.push(working->name);
        working = working->source;
    }
    pathData shortestPath; // creates the return path
    shortestPath.distance = endPoint->distance; // puts the distance into the return path
    while (! route.empty()) // puts the vertex names into the return path
    {
        shortestPath.path.push_back(route.top());
        route.pop();
    }
    return shortestPath;
}

/*
Function prototype:
pathData getShortestVisitAllExhaustive(std::string)

Function description:
This function performs an exhaustive search to find all paths that originate and end at the input string while going through every other vertex once, and the returns the lowest weight distance path as a pathData structure. The pathData structure has an integer distance which is the weight distance traversed, and a vector of strings which is the names of the vertices traversed.

Example:
graph.cities;
cities.getShortestVisitAllExhaustive("New York", "Los Angeles");

Precondition:
There must be at-least one path that can go from the input string in a path to all other input strings and then back, visiting each vertex once.

Post condition:
Returns a pathData structure.

*/
pathData Graph::getShortestVisitAllExhaustive(string startVertex) // finds the shortest distance path that visits all vertices
{
    //cout << "finding path" << endl; // debug
    vertex* startPoint = NULL;
    for (int i = 0; i < vertices.size(); i++) // sets a pointer to the starting vertex
    {
        if (vertices.at(i).name == startVertex)
            startPoint = &vertices.at(i);
    }
    //cout << startPoint << endl; // debug
    stack<path> paths; // creates a stack to hold paths being computed
    vector<path> finishedPaths; // stack to hold finished paths
    path start;
    start.distance = 0;
    start.path.push_back(startPoint); // creates and adds to the stack a working path to start the loop
    paths.push(start);
    while (! paths.empty()) // runs as long as there are working paths
    {
        //cout << "loading path" << endl; // debug
        path working = paths.top(); // gets the path to expand
        paths.pop();
        bool newVertex = false; // tracks if the path was expanded
        for (int i = 0; i < working.path.back()->edges.size(); i++)
        {
            //cout << "checking and edge" << endl;
            bool visited = false; // tracks if vertex has been visited
            for (int j = 0; j < working.path.size(); j++) // goes through all visited vertices
            {
                if (working.path.at(j) == working.path.back()->edges.at(i).destination) // checks if vertex has been visited
                    visited = true;
            }
            if (! visited) // only runs if the vertex has not been visited before
            {
                newVertex = true; // sets that the path was expanded
                path temp = working; // create an extra path to expand
                temp.distance += working.path.back()->edges.at(i) .distance;
                temp.path.push_back(working.path.back()->edges.at(i).destination);
                paths.push(temp); // puts the new path onto the stack
                //cout << "path added" << endl; // debug
            }
        }
        if (! newVertex) // if the path could not be expanded, place in solutions
        {
            bool gotBack = false; // tracks if the path can be finished
            for (int i = 0; i < working.path.back()->edges.size(); i++)
            {
                if (working.path.back()->edges.at(i).destination == startPoint) // searches for edge back to start
                {
                    //cout << "full loop" << endl; // debug
                    gotBack = true; // track that the path could get back to the start
                    working.distance += working.path.back()->edges.at(i).distance; // adds distance back to start to the path
                    working.path.push_back(working.path.back()->edges.at(i).destination); // adds start node to end of path
                }
                if (gotBack) // if the path could return to the start
                {
                    //cout << vertices.size() << ":" << working.path.size() - 1 << endl; // debug
                    if (vertices.size() == (working.path.size() - 1)) // checks if all vertices were visited
                        finishedPaths.push_back(working); // adds the path to successful paths
                }
            }
        }
    }
    int shortestLoc = 0; // creates value to track location of shortest path
    for (int i = 1; i < finishedPaths.size(); i++) // runs through all completed paths
    {
        if (finishedPaths.at(i).distance < finishedPaths.at(shortestLoc).distance) // checks if current path is shorter than tracked shortest path
            shortestLoc = i; // sets not shortest path
    }
    pathData shortestPath; // creates pathData to hold path information
    for (int i = 0; i < finishedPaths.at(shortestLoc).path.size(); i++) // adds path names to pathData structure
    {
        shortestPath.path.push_back(finishedPaths.at(shortestLoc).path.at(i)->name);
    }
    shortestPath.distance = finishedPaths.at(shortestLoc).distance; // adds distance to pathData structure
    return shortestPath;
}

/*
Function prototype:
pathData getLongestDistanceVisitAllExhaustive(std::string)

Function description:
This function performs an exhaustive search to find all paths that originate and end at the input string while going through every other vertex once, and the returns the highest weight distance path as a pathData structure. The pathData structure has an integer distance which is the weight distance traversed, and a vector of strings which is the names of the vertices traversed.

Example:
graph.cities;
cities.getLongestDistanceVisitAllExhaustive("New York", "Los Angeles");

Precondition:
There must be at-least one path that can go from the input string in a path to all other input strings and then back, visiting each vertex once.

Post condition:
Returns a pathData structure.

*/
pathData Graph::getLongestDistanceVisitAllExhaustive(string startVertex) // finds longest path that visits all nodes, code is identical to getShortestVisitAllExhaustive() except where noted
{
    vertex* startPoint = NULL;
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices.at(i).name == startVertex)
            startPoint = &vertices.at(i);
    }
    stack<path> paths;
    vector<path> finishedPaths;
    path start;
    start.distance = 0;
    start.path.push_back(startPoint);
    paths.push(start);
    while (! paths.empty())
    {
        path working = paths.top();
        paths.pop();
        bool newVertex = false;
        for (int i = 0; i < working.path.back()->edges.size(); i++)
        {
            bool visited = false;
            for (int j = 0; j < working.path.size(); j++)
            {
                if (working.path.at(j) == working.path.back()->edges.at(i).destination)
                    visited = true;
            }
            if (! visited)
            {
                newVertex = true;
                path temp = working;
                temp.distance += working.path.back()->edges.at(i) .distance;
                temp.path.push_back(working.path.back()->edges.at(i).destination);
                paths.push(temp);
            }
        }
        if (! newVertex)
        {
            bool gotBack = false;
            for (int i = 0; i < working.path.back()->edges.size(); i++)
            {
                if (working.path.back()->edges.at(i).destination == startPoint)
                {
                    gotBack = true;
                    working.distance += working.path.back()->edges.at(i).distance;
                    working.path.push_back(working.path.back()->edges.at(i).destination);
                }
                if (gotBack)
                {
                    if (vertices.size() == (working.path.size() - 1))
                        finishedPaths.push_back(working);
                }
            }
        }
    }
    int shortestLoc = 0;
    for (int i = 1; i < finishedPaths.size(); i++)
    {
        if (finishedPaths.at(i).distance > finishedPaths.at(shortestLoc).distance) // only difference between find longest and find shortest, searches for greatest instead of smallest
            shortestLoc = i;
    }
    pathData shortestPath;
    for (int i = 0; i < finishedPaths.at(shortestLoc).path.size(); i++)
    {
        shortestPath.path.push_back(finishedPaths.at(shortestLoc).path.at(i)->name);
    }
    shortestPath.distance = finishedPaths.at(shortestLoc).distance;
    return shortestPath;
}

/*
Function prototype:
pathData getShortestDistancePathBellmanFord(std::string,std::string)

Function description:
Finds the shortest path from the first input string to the second input string using the Bellman-Ford algorithm, and then returns the information as a pathData structure. The pathData structure has an integer distance which is the weight distance traversed, and a vector of strings which is the names of the vertices traversed.

Example:
graph.cities;
cities.getShortestDistancePathBellmanFord("New York", "Los Angeles");

Precondition:
Both input strings must exist as vertices, and there must be a path between them.

Post condition:
Returns a pathData structure.

*/
pathData Graph::getShortestDistancePathBellmanFord(string startVertex, string endVertex) // finds shortest path, works with negative values
{
    vertex* startPoint;
    vertex* endPoint;
    vector<vertex*> solved;
    for (int i = 0; i < vertices.size(); i++) // goes through all vertices to establish inital conditions and find start and end vertices
    {
        vertices.at(i).source = NULL;
        vertices.at(i).distance = INT_MAX;
        if (vertices.at(i).name == startVertex) // checks for start vertex
        {
            vertices.at(i).distance = 0;
            solved.push_back(&vertices.at(i));
            startPoint = &vertices.at(i);
        }
        if (vertices.at(i).name == endVertex) // checks for end vertex
        {
            endPoint = &vertices.at(i);
        }
    }
    for (int i = 1; i < vertices.size(); i++) // run the procedure number of vertices minus 1 times
    {
        for (int j = 0; j < vertices.size(); j++) // go through every edge
        {
            for (int k = 0; k < vertices.at(j).edges.size(); k++)
            {
                if (vertices.at(j).edges.at(k).destination->distance > vertices.at(j).distance + vertices.at(j).edges.at(k).distance) // checks and updates distance
                {
                    vertices.at(j).edges.at(k).destination->distance = vertices.at(j).distance + vertices.at(j).edges.at(k).distance;
                    vertices.at(j).edges.at(k).destination->source = &vertices.at(j);
                }
            }
        }
    }
    stack<string> route; // puts the vertex names into a stack to flip the order
    vertex* working = endPoint;
    while (working != NULL)
    {
        route.push(working->name);
        working = working->source;
    }
    pathData shortestPath; // creates the return path
    shortestPath.distance = endPoint->distance; // puts the distance into the return path
    while (! route.empty()) // puts the vertex names into the return path
    {
        shortestPath.path.push_back(route.top());
        route.pop();
    }
    return shortestPath;
}

/*
Function prototype:
pathData getShortestDistancePathDAG(std::string,std::string)

Function description:
Finds the shortest path from the first input string to the second input string using the DAG algorithm, and then returns the information as a pathData structure. The pathData structure has an integer distance which is the weight distance traversed, and a vector of strings which is the names of the vertices traversed.

Example:
graph.cities;
cities.getShortestDistancePathDAG("New York", "Los Angeles");

Precondition:
Both input strings must exist as vertices, and there must be a path from the first vertex to the second. There can't be a cycle in the graph.

Post condition:
Returns a pathData structure.

*/
pathData Graph::getShortestDistancePathDAG(string startVertex, string endVertex) // finds shortest path, assuming no cycles
{
    vertex* startPoint;
    vertex* endPoint;
    vector<vertex*> sorted;
    for (int i = 0; i < vertices.size(); i++) // goes through each vertex
    {
        vertices.at(i).source = NULL;
        vertices.at(i).distance = INT_MAX;
        if (vertices.at(i).name == startVertex) // checks for start vertex
        {
            vertices.at(i).distance = 0;
            startPoint = &vertices.at(i);
        }
        if (vertices.at(i).name == endVertex) // checks for end vertex
        {
            endPoint = &vertices.at(i);
        }
        int location = INT_MAX;
        for (int j = 0; j < vertices.at(i).edges.size(); j++) //identifies where the vertex should be sorted
        {
            for (int k = 0; k < sorted.size(); k++)
            {
                if (sorted.at(k) == vertices.at(i).edges.at(j).destination)
                    location = min(k, location);
            }
        }
        if (location = INT_MAX) // sorts the vertex
        {
            sorted.push_back(&vertices.at(i));
        }
        else
        {
            sorted.insert(sorted.begin() + location, &vertices.at(i));
        }
    }
    for (int i = 0; i < sorted.size(); i++)
    {
        for (int j = 0; j < sorted.at(i)->edges.size(); j++)
        {
                if (sorted.at(i)->edges.at(j).destination->distance > sorted.at(i)->distance + sorted.at(i)->edges.at(j).distance) // checks and updates distance
                {
                    sorted.at(i)->edges.at(j).destination->distance = sorted.at(i)->distance + sorted.at(i)->edges.at(j).distance;
                    sorted.at(i)->edges.at(j).destination->source = sorted.at(i);
                }
        }
    }
    stack<string> route; // puts the vertex names into a stack to flip the order
    vertex* working = endPoint;
    while (working != NULL)
    {
        route.push(working->name);
        working = working->source;
    }
    pathData shortestPath; // creates the return path
    shortestPath.distance = endPoint->distance; // puts the distance into the return path
    while (! route.empty()) // puts the vertex names into the return path
    {
        shortestPath.path.push_back(route.top());
        route.pop();
    }
    return shortestPath;
}
