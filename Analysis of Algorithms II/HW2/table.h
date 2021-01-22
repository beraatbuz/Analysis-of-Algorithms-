/* @Author
 * Student Name: <Beraat BUZ>
 * Student ID: <150160002>
 * Date: <22.04.2020> */
#ifndef TABLE_H
#define TABLE_H
using namespace std;
class table
{
protected:
    vector<int> name;
    vector<int> time;
public:
    int min(){return (this->time.back()-this->time.front());}
    int get_back(){return this->time.back();}
    int get_size(){return this->time.size();}
    int get_name(int i){return this->name[i];}
    int get_time(int i){return this->time[i];}
    void pushname(int j){this->name.push_back(j);}
    void pushtime(int j){this->time.push_back(j);}
    void go_come_table(int start,int** adj);
    void change_time(int start,int** adj);
    void Find_Alt_Paths(int**adj,int number_of_nodes,list<table> *alt_path,int *min,int start,int end,list<int>* my_list);
    bool is_intersection(table*t2);
    bool solution_(list<table>*alt_path1,list<table>*alt_path2,table* t1,table* t2,int**adj,table* ch1,table* ch2,table* ch3,table* ch4);
    void fill_table(int* min_t1,int**adj,int number_of_nodes,list<table>*alt_path1,int JH,int JD,list<int>*my_list1);
    void create_table(int dist[], int shortest_path[],int src,int targ);
    void add_to_table(int shortest_path[], int j);
};
#endif