/*
Beraat BUZ
150160002
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <climits>
using namespace std;

struct Edge{
    int sourceNode;
    int destinationNode;
    int weight;
};

class Graph{
    int numberOfNodes;
    int numberOfEdges;
    vector<Edge> edges;
    
public:
    void setNumberOfNodes(int n){
        numberOfNodes = n;
    }
    int getNumberOfNodes(){
        return numberOfNodes;
    }
    void setNumberOfEdges(int n){
        numberOfEdges = n;
    }
    int getNumberOfEdges(){
        return numberOfEdges;
    }
    
    void AddEdge(int sourceNode, int destinationNode, int weight){
        Edge e = Edge();
        e.sourceNode = sourceNode;
        e.destinationNode = destinationNode;
        e.weight = weight;
        
        edges.push_back(e);
    }
    vector<Edge> getAllEdges(){
        return edges;
    }
    
    vector<Edge> getOutgoingEdges(int src){
        vector<Edge> outgoingEdges;
        
        for (Edge e : edges){
            if(e.sourceNode == src){
                outgoingEdges.push_back(e);
            }
        }
        
        return outgoingEdges;
    }
    
    int getWeightOfEdge(int src, int dst){
       int weight = 0;
       for (Edge e : edges){
           if(e.sourceNode == src && e.sourceNode == dst){
               cout << "weigth: " << e.weight << endl;
               weight = e.weight;
           }
       }
       
       return weight;
    }
    
    void printGraph(){
        cout << "************* Graph data *************" << endl;
        cout << "Number of total edges: " << numberOfEdges << endl;
        cout << "Number of total nodes: " << numberOfNodes << endl << endl;
        
        cout << "Source node --> Destination node : Edge weigth" << endl;
        for (std::vector<Edge>::const_iterator i = edges.begin(); i != edges.end(); ++i){
            std::cout << i->sourceNode << " --> " ;
            std::cout << i->destinationNode << " : " ;
            std::cout << setw(2) << i->weight << endl ;
        }
        cout << "**************************************" << endl << endl;
    }
};

class BellmanFord{
    Graph G;
    int sourceNode;
    int destinationNode;
    int** M;
    bool graphContainsNegativeWeightCycle;
        
public:
    void setGraph(Graph g){
        G = g;
    }
    Graph getGraph(){
        return G;
    }
    void setSourceNode(int s){
        sourceNode = s;
    }
    int getSourceNode(){
        return sourceNode;
    }
    void setDestinationNode(int t){
        destinationNode = t;
    }
    int getDestinationNode(){
        return destinationNode;
    }
    bool buildDPTable(){
        // Fill here
        int dest=getDestinationNode();
        int numb_of_nodes=G.getNumberOfNodes();
        M = new int*[numb_of_nodes+1];   // I created 1 more row to hold the path.
        for(int i = 0; i < numb_of_nodes+1; ++i)
            M[i] = new int[numb_of_nodes];

        // Step 1: fill the first row of distance array 
        for (int i = 0; i < numb_of_nodes; i++){
            M[0][i] = INT_MAX;
        }
        M[0][dest] = 0;
        //Computing by using equation 1
        vector<int> path (numb_of_nodes, -1); // for path
        vector<Edge> e = G.getAllEdges(); // all edges
        for (int i = 1; i <= numb_of_nodes - 1; i++) {
            for (int a = 0; a < numb_of_nodes; a++)
                M[i][a]=M[i-1][a];
            for (int j = 0; j < e.size(); j++) {
                int src = e[j].sourceNode;
                int dst = e[j].destinationNode;
                int weight = e[j].weight;
                if (M[i-1][dst] != INT_MAX && M[i-1][dst] + weight < M[i-1][src]){
                    M[i][src] = M[i-1][dst] + weight;
                    path[src] = dst;
                }
            }
        }
        //Check negative weight cycle
        for (int i = 0; i < e.size(); i++) {
            int src = e[i].sourceNode;
            int dst = e[i].destinationNode;
            int weight = e[i].weight;
            if (M[numb_of_nodes-1][dst] != INT_MAX && M[numb_of_nodes-1][dst] + weight < M[numb_of_nodes-1][src]){
                cout<<"Graph contains negative weight cycle!"<<endl;
                return false;
            }
        }
        //Copy the path to last row of M.
        for(int a = 0; a < numb_of_nodes; a++)
            M[numb_of_nodes][a]=path[a];
        return true;
      
    }
    
    void printDPTable(){
        if(!graphContainsNegativeWeightCycle){
            cout << endl << "Dynamic programming table: (shortest paths from each node to destination node): " << endl << endl;
            
            int numberOfNodes = G.getNumberOfNodes();
            
            for(int i = 0; i <= numberOfNodes; i++){
                for(int j = 0; j <= numberOfNodes; j++){
                    if(i == 0 && j == 0)
                        cout << "\t";
                    else if(i != 0 && j == 0)
                        cout << setw(10) << "Iteration-" << i - 1;
                    else if(i == 0 && j != 0)
                        cout << "\t" << "Node-" << j - 1 << " ";
                    else{
                        if(M[i - 1][j - 1] == INT_MAX)
                            cout << "\t " << "\u221E";
                        else
                            cout << "\t" << setw(2) << M[i - 1][j - 1];
                    }
                }
                cout << endl;
            }
            cout << endl;
        }
    }
    void printShortestPathBetweenSourceAndDestination(){
        // Fill here
        int dest=getDestinationNode();
        int src=getSourceNode();
        int numb_of_nodes=G.getNumberOfNodes();
        cout<<endl;
        cout<<"Shortest path value from "<<src<<" to "<<dest<<": "<<M[numb_of_nodes-1][src]<<endl;
        cout<<"Shortest path from "<<src<<" to "<<dest<<": "<<src;
        while(src>=0){
            src=M[numb_of_nodes][src];
            if(src>=0)
                cout <<" -> "<< src ;
        }
        cout<<endl;
    }
};

int main(int argc, char* argv[])
{
    if(argc > 1){
        //cout<<"\u221E"<<endl;
        Graph graph;
        
        //read graph data from file
        string inputFileName = argv[1];
        string line;
        ifstream infile(inputFileName);
        
        getline(infile, line);
        graph.setNumberOfNodes(stoi(line));
        getline(infile, line);
        graph.setNumberOfEdges(stoi(line));

        int src, dst, w;
        while (infile >> src >> dst >> w) {
           graph.AddEdge(src, dst, w);
        }
        
        graph.printGraph();
        
        BellmanFord bf;
        bf.setGraph(graph);
        
        int dstNode;
        int srcNode;
        
        cout << "Enter the destination node: ";
        cin >> dstNode;
        
        bf.setDestinationNode(dstNode);
    
        if(bf.buildDPTable()){
            bf.printDPTable();
            
            cout << "Enter a source node: ";
            cin >> srcNode;
            bf.setSourceNode(srcNode);
            
            bf.printShortestPathBetweenSourceAndDestination();
        }
    }
    else{
        cout << "Error: please provide a test file!" << endl;
    }
    
    return 0;
}
