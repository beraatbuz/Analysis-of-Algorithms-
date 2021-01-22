/*
Beraat Buz
150160002
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

class Graph{
    vector <vector<int> > capacity;
    vector <vector<int> > flow;
    int nodeCount;
    vector <int> excess;
    vector <int> height; 
    public:
    void addCapacity(int, int, int);
    void push(int, int);
    void relabel(int);
    void initializePreflow(int);
    int overflowingVertex();
    int findMaxFlow();
    Graph(int);
    ~Graph();
};

Graph::Graph(int n){
    nodeCount = n;
    capacity.assign(nodeCount, vector<int>(nodeCount, 0));
}

void Graph::addCapacity(int from, int to, int cap){
    capacity[from][to] = cap;
}

int Graph::overflowingVertex(){
   for (int i = 1; i < excess.size()-1; i++) 
       if (excess[i]> 0){
           for (int v = 0; v < nodeCount; v++)
            { 
                if(capacity[i][v]>0 &&!(capacity[i][v]==flow[i][v]))
                    return i;  //then push or relabel can be applied.
            }

       }
    return -1;  // means no overflowing Vertex 
}

void Graph::initializePreflow(int source){
    //Given pseudo code
    vector<int> temp;
    for (int u = 0; u < nodeCount; u++)
    {
        temp.push_back(0);
    }
    for (int v = 0; v < nodeCount; v++) {
        height.push_back(0);
        excess.push_back(0);
        flow.push_back(temp);
    }
    height[source]=nodeCount;
    for (int v = 0; v < nodeCount; v++)
    {
        flow[source][v]=capacity[source][v];
        excess[v]=capacity[source][v];
        excess[source]-=capacity[source][v];
    }    
}

void Graph::push(int u, int v){
    //Given pseudo
    int delta = min(excess[u], capacity[u][v] - flow[u][v]);
    flow[u][v] += delta;
    flow[v][u] -= delta;
    excess[u] -= delta;
    excess[v] += delta;
    
}

void Graph::relabel(int u){
    // Given pseudo
    int maximum=numeric_limits<int>::max();
    int minimum=numeric_limits<int>::max();
    for (int v = 0; v < nodeCount; v++) {
        int difference=capacity[u][v] - flow[u][v];
        if (difference > 0)
            minimum= min(minimum, height[v]);
    }
    if(minimum<maximum)
        height[u]=minimum+1;
}

int Graph::findMaxFlow(){
    //until there is no overflow, push or relabel nodes or vertices.
    while (overflowingVertex() != -1) 
    { 
        int u = overflowingVertex();
        for (int v = 0; v < nodeCount; v++)
        { 
            if(height[u]>height[v]&& capacity[u][v]>0 &&!(capacity[u][v]==flow[u][v])){
                push(u,v);
            }
            else if (capacity[u][v]>0&&!(capacity[u][v]==flow[u][v]))
            {
                relabel(u);
            }
             
        } 
    }
    int max_flow=0;
    for (int i = 0; i < nodeCount; i++)
        max_flow -= flow[nodeCount-1][i];
    return max_flow;  // or we can return the last excess.
}

Graph::~Graph(){

}

int main(int argumentNumber, char *arguments[]){
    int nodeCount;
    string filename = arguments[1];
    string tempString;
    int temp, from, to, flow;
    ifstream file;
    file.open(arguments[1]);
    file >> nodeCount;
    Graph graph(nodeCount);
    file >> tempString >> tempString >> tempString;
    while(file.good()){
        file >> from >> to >> flow;
        graph.addCapacity(from, to, flow);
    }
    graph.initializePreflow(0); // 0th node is source
    cout << "Maximum flow: " << graph.findMaxFlow() << endl; 
    return 0;
}