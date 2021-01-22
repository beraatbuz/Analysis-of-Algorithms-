/* @Author
 * Student Name: <Beraat BUZ>
 * Student ID: <150160002>
 * Date: <22.04.2020> */
#include <iostream>
#include <string>
#include <fstream>
#include <limits.h> 
#include <vector> 
#include <list>
#include <algorithm>
#include "dijkstra.h"
using namespace std;
//Needed for dijkstra
int minumum_distance(int dist[],bool isVertex[],int Numb_of_nodes) 
{ 
    int min = INT_MAX, min_index; 
    for (int i = 0; i < Numb_of_nodes; i++) 
        if (isVertex[i] == false && dist[i] <= min) 
        {
            min = dist[i] ;
            min_index = i; 
        }

    return min_index; 
} 
// dijkstra function must be here because it is used by below functions
void dijkstra(int** graph, int src,int Numb_of_nodes, int targ,table* t) 
{ 

    int distance[Numb_of_nodes],shortest_path[Numb_of_nodes];   
    bool isVertex[Numb_of_nodes]; 
    for (int i = 0; i < Numb_of_nodes; i++) 
    { 
        shortest_path[src] = -1;
        distance[i] = INT_MAX; 
        isVertex[i] = false; 
        
    } 
    distance[src] = 0; 
    for (int i = 0; i < Numb_of_nodes - 1; i++) 
    { 
        int min_dist = minumum_distance(distance, isVertex,Numb_of_nodes); 
        isVertex[min_dist] = true; 
        for (int i = 0; i < Numb_of_nodes; i++) 
            if (!isVertex[i] && graph[min_dist][i] && distance[min_dist] + graph[min_dist][i] < distance[i]) 
            { 
                shortest_path[i] = min_dist; 
                distance[i] = distance[min_dist] + graph[min_dist][i]; 
            }  
    } 
    t->create_table(distance, shortest_path,src,targ); 
}