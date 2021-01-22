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
#include "table.cpp"
using namespace std;

//Needed for sorting alternative paths
bool compare(table&t1,table&t2){
    if(t1.get_back() >=t2.get_back())
        return true;
    else
    {
        return false;
    }
    
}

int main(int argc, char *argv[]){
    ifstream  nodefile (argv[1]);
    int number_of_nodes;
    int  JH,JD,LH,LD ;
    int source_node, target_node, weight ;
    int** adj;
    //Read file
    if (nodefile.is_open())
    {
        nodefile>>number_of_nodes;
        adj = new int*[number_of_nodes];
        for(int i = 0; i < number_of_nodes; ++i)
            adj[i] = new int[number_of_nodes];
        for (int i = 0; i < number_of_nodes; i++)
            for (int j = 0; j < number_of_nodes; j++)
                adj[i][j]=0;
        nodefile>>JH>>JD>>LH>>LD; 
        while ( nodefile>>source_node>>target_node>>weight ) 
            adj[source_node][target_node]=weight;
    }
    table t1,t2,t3,t4; //For result tables
    list<table> alt_path1,alt_path2,alt_path3,alt_path4; //List of alternative tables
    list<int> my_list1,my_list2,my_list3,my_list4; // Hold the shortest distances for alternative paths (its needed for sorting)
    //First find shortest paths
    dijkstra(adj, JH,number_of_nodes,JD,&t1);
    dijkstra(adj, JD,number_of_nodes,JH,&t2);
    dijkstra(adj, LH,number_of_nodes,LD,&t3);
    dijkstra(adj, LD,number_of_nodes,LH,&t4);
    //Fill the times in the table
    t1.go_come_table(0,adj);
    t2.go_come_table(t1.get_back()+30,adj);
    t3.go_come_table(0,adj);
    t4.go_come_table(t3.get_back()+30,adj);
    //Find Tables' Alternative paths and Fill the table's
    int min=t1.min();
    t1.fill_table(&min,adj,number_of_nodes,&alt_path1,JH,JD,&my_list1);
    min=t2.min();
    t2.fill_table(&min,adj,number_of_nodes,&alt_path2,JD,JH,&my_list2);
    min=t3.min();
    t3.fill_table(&min,adj,number_of_nodes,&alt_path3,LH,LD,&my_list3);
    min=t4.min();
    t4.fill_table(&min,adj,number_of_nodes,&alt_path4,LD,LH,&my_list4);
    
    //Sort Alternative Paths
    alt_path1.sort(compare);
    alt_path2.sort(compare);
    alt_path3.sort(compare);
    alt_path4.sort(compare);

    bool solution=true;
    //If they intersect,then find  solution
    if(t1.is_intersection(&t3))
        solution=t1.solution_(&alt_path1,&alt_path3,&t1,&t3,adj,&t2,&t4,&t1,&t3);

    
    if(t2.is_intersection(&t4) && solution==true)
        solution=t2.solution_(&alt_path2,&alt_path4,&t2,&t4,adj,&t2,&t4,&t1,&t3);
    
    if(t1.is_intersection(&t4) && solution==true)
        solution=t1.solution_(&alt_path1,&alt_path4,&t1,&t4,adj,&t2,&t4,&t1,&t3);
    
    if(t2.is_intersection(&t3) && solution==true)
        solution=t2.solution_(&alt_path2,&alt_path3,&t2,&t3,adj,&t2,&t4,&t1,&t3);

    if(solution)
    {
        cout<<"Joseph's Path, duration: "<<t2.get_back()<<endl;
        for (int i = 0; i < t1.get_size(); i++)
        {
            cout<<"Node: "<<t1.get_name(i)<<"  Time: "<<t1.get_time(i)<<endl;
        }
        cout<<"-- return --"<<endl;
        for (int i = 0; i < t2.get_size(); i++)
        {
            cout<<"Node: "<<t2.get_name(i)<<"  Time: "<<t2.get_time(i)<<endl;
        }
        cout<<endl;
        cout<<"Lucy's Path, duration: "<<t4.get_back()<<endl;
        for (int i = 0; i < t3.get_size(); i++)
        {
            cout<<"Node: "<<t3.get_name(i)<<"  Time: "<<t3.get_time(i)<<endl;
        }
        cout<<"-- return --"<<endl;
        for (int i = 0; i < t4.get_size(); i++)
        {
            cout<<"Node: "<<t4.get_name(i)<<"  Time: "<<t4.get_time(i)<<endl;
        }
    }
    else 
        cout<<"No Solution!"<<endl;
    cout<<endl;
    return 0;
}
