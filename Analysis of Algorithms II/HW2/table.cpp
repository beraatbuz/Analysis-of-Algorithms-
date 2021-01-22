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
#include "table.h"
#include "dijkstra.cpp"
using namespace std;

void table::add_to_table(int shortest_path[], int j) 
{ 
    if (shortest_path[j] == - 1) 
        return; 
    this->add_to_table(shortest_path, shortest_path[j]); 
    this->name.push_back(j);
} 
  

void table::create_table(int dist[], int shortest_path[],int src,int targ) 
{ 
    if(dist[targ] ==INT_MAX || dist[targ]<0){
        while(!this->name.empty())
            this->name.pop_back();
        this->name.push_back(-1);
        return;
    }
        this->name.push_back(src);
        this->add_to_table(shortest_path, targ); 
} 

//Fill the time in the table
void table::go_come_table(int start,int** adj){

    for (unsigned int i = 0; i <this->name.size(); ++i) {
        if(i==0)
            this->time.push_back(start);
        else
        {
            this->time.push_back(this->time[i-1]+adj[this->name[i-1]][this->name[i]]);
        }
    }
    
}
//Change (Update) time in the table 
void table::change_time(int start,int** adj){

    for (unsigned int i = 0; i <this->name.size(); ++i) {
        if(i==0)
            this->time[i]=start;
        else
        {
            this->time[i]=(this->time[i-1]+adj[this->name[i-1]][this->name[i]]);
        }
    }
    
}
//Find all alternative paths
void table::Find_Alt_Paths(int**adj,int number_of_nodes,list<table> *alt_path,int *min,int start,int end,list<int>* my_list){
    if(this->name.back()==-1){
        return;
    }
    this->go_come_table(0,adj);
    
    //if this path's distance is not in list then add
    if(!(find(my_list->begin(), my_list->end(), this->time.back()-this->time.front()) != my_list->end())){
        my_list->push_back(this->time.back()-this->time.front());
        if(this->time.back()-this->time.front()>=*min){
            alt_path->push_front(*this);
            *min=this->time.back()-this->time.front();
        }
        else
            alt_path->push_back(*this);     
    }
    int size=this->name.size();
    for(int i=1;i<size;i++)
    {
        int a,b;
        a=this->name.back();
        this->name.pop_back();
        b=this->name.back();
        int temp=adj[b][a];
        adj[b][a]=0; 
        table t1;
        dijkstra(adj, start,number_of_nodes,end,&t1);
        if(t1.name.back()!=-1){
            //Until finding all paths 
            t1.Find_Alt_Paths(adj,number_of_nodes,alt_path,min,start,end,my_list);
        }
        adj[b][a]=temp;
    }
}
//If two table intersect,then returns true
bool table::is_intersection(table*t2){
    for(int i=0;i<this->time.size();i++)
        for(int j=0;j<t2->time.size();j++)
            if(this->time[i]==t2->time[j] && this->name[i]==t2->name[j])
                return true;
    for(int i=0;i<this->time.size();i++)  
        if( this->time[i]>(t2->time[t2->time.size()-1]) && this->time[i]<(t2->time[t2->time.size()-1]+30) && this->name[i]==t2->name[t2->name.size()-1])
            return true;
    for(int i=0;i<t2->time.size();i++)
        if( t2->time[i]>(this->time[this->time.size()-1]) && t2->time[i]<(this->time[this->time.size()-1]+30) && t2->name[i]==this->name[this->name.size()-1])
            return true;
    return false;
}

//  If main tables intersect, we can find alternative solutions in this function if there is.
bool table::solution_(list<table>*alt_path1,list<table>*alt_path2,table* t1,table* t2,int**adj,table* ch1,table* ch2,table* ch3,table* ch4)
{
    while(1){
        if((!(alt_path1->empty()))&&(!(alt_path2->empty()))){ // For 2 table has alter. paths
            if(t1->time.back()+alt_path2->back().time.back()<=t2->time.back()+alt_path1->back().time.back() &&t1->time.back()+alt_path2->back().time.back()<=alt_path1->back().time.back()+alt_path2->back().time.back()){
                if(!t1->is_intersection(&alt_path2->back())){
                    
                    *t2=alt_path2->back();
                    ch2->change_time(ch4->time.back()+30,adj);
                    alt_path2->pop_back();
                    if(t1->is_intersection(t2))
                        continue;
                    return true;
                }
                alt_path2->pop_back();
            }
            else if(t1->time.back()+alt_path2->back().time.back()>=t2->time.back()+alt_path1->back().time.back() &&t2->time.back()+alt_path1->back().time.back()<=alt_path1->back().time.back()+alt_path2->back().time.back()){
                if(!t2->is_intersection(&alt_path1->back())){
                    *t1=alt_path1->back();
                    ch1->change_time(ch3->time.back()+30,adj);
                    alt_path1->pop_back();
                    if(t1->is_intersection(t2))
                        continue;
                    return true;
                }
                alt_path1->pop_back();
            }
            else
            {
                if(!alt_path2->back().is_intersection(&alt_path1->back())){
                    *t1=alt_path1->back();
                    *t2=alt_path2->back();    
                    ch2->change_time(ch4->time.back()+30,adj);
                    ch1->change_time(ch3->time.back()+30,adj);
                    alt_path2->pop_back();
                    alt_path1->pop_back();
                    if(t1->is_intersection(t2))
                        continue;
                    return true;
                }
                if(alt_path1->back().time.back()<=alt_path2->back().time.back())
                    alt_path2->pop_back(); 
                else
                    alt_path1->pop_back();
                if(alt_path1->empty())
                    alt_path2->pop_back();
                if(alt_path2->empty())
                    alt_path1->pop_back();
                if(alt_path1->empty()&&alt_path2->empty())
                {
                    return false;
                }
                    
                
            }
            
        }
        else if(!(alt_path1->empty())){ //If only table 1 has alternative paths
            if(!t2->is_intersection(&alt_path1->back())){
                *t1=alt_path1->back();
                ch1->change_time(ch3->time.back()+30,adj);
                alt_path1->pop_back();
                if(t1->is_intersection(t2))
                    continue;
                return true;
            }
            alt_path1->pop_back();
            if(alt_path1->empty())
            {
                return false;
            }
        }
        else if(!(alt_path2->empty())){ //If only table 2 has alternative paths
            if(!t1->is_intersection(&alt_path2->back())){
                *t2=alt_path2->back();
                ch2->change_time(ch4->time.back()+30,adj);
                alt_path2->pop_back();
                if(t1->is_intersection(t2))
                    continue;
                return true;
            }
            alt_path2->pop_back();
            if(alt_path2->empty())
            {
                return false;
            }
        }
        else{
            return false;
        }
    }
}
//Find All tables' paths and fill the node-time table
void table::fill_table(int* min_t1,int**adj,int number_of_nodes,list<table>*alt_path1,int JH,int JD,list<int>*my_list1){
    my_list1->push_back(*min_t1);
    table temp;
    temp=*this;
    this->Find_Alt_Paths(adj,number_of_nodes,alt_path1,min_t1,JH,JD,my_list1); //Find all paths
    *this=temp;
    list<table> temp_list;
    temp_list=*alt_path1;
    while(!alt_path1->empty())
    {
        alt_path1->back().go_come_table(0,adj);
        alt_path1->pop_back();
    }
    *alt_path1=temp_list;
}