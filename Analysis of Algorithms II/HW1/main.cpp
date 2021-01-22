#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <sys/time.h> 
#include <cmath>   
#include <stack>
using namespace std;
struct pokemon
{
    string attack_name[4];
    int HP,PP[4],Accuracy[4],Damage[4],FirstUsage[4];
};
class Poke_Node {    
public:
    string attack_name;
    int pika_hp,pika_pp,blast_hp,blast_pp,turn,level,isleaf,effective;
    float prob;
    Poke_Node* next=NULL; 
    Poke_Node* child=NULL; 
    Poke_Node* parent=NULL; 
    Poke_Node(){
        this->isleaf=0;
        this->effective=1;
    }
};
void equal(Poke_Node* a1,Poke_Node* a2){
    a1->blast_hp=a2->blast_hp;
    a1->blast_pp=a2->blast_pp;
    a1->isleaf=a2->isleaf;
    a1->level=a2->level;
    a1->pika_hp=a2->pika_hp;
    a1->pika_pp=a2->pika_pp;
    a1->prob=a2->prob;
}

void bfs_part3(Poke_Node* node,stack<string>* s1,stack<string>* s2,float* prob1,float* prob2) 
{ 
    if (node == NULL) 
        return;
    if(node->blast_hp==0 && node->prob>*prob1){ // Pikachu wins
        while(!s1->empty())
            s1->pop();
        *prob1=node->prob;
        ostringstream ss,ss2;
        ss << node->prob;
        s1->push(ss.str());
        ss2 << node->level;
        s1->push(ss2.str());
        while (node->parent)
        {
            
            if(node->effective==0)
                s1->push("noneffective");
            else
                s1->push("effective");
            s1->push(node->attack_name);
            node=node->parent;

        }
        
    }
    if(node->pika_hp==0 && node->prob>*prob2){ //Blastoise wins
        while(!s2->empty())
            s2->pop();
        *prob2=node->prob;
        
        ostringstream ss,ss2;
        ss << node->prob;
        s2->push(ss.str());
        ss2 << node->level;
        s2->push(ss2.str());
        while (node->parent)
        {
            
            if(node->effective==0)
                s2->push("noneffective");
            else
                s2->push("effective");
            s2->push(node->attack_name);
            node=node->parent;

        }
        
    }
    
    bfs_part3(node->next,s1,s2,prob1,prob2); 
    bfs_part3(node->child,s1,s2,prob1,prob2); 
}   

void Dfs(Poke_Node* node,int* count) 
{ 
    if (node == NULL) 
        return;
    (*count)++;
    Dfs(node->child,count); 
    Dfs(node->next,count);
}   
void Bfs(Poke_Node* node,int* count) 
{ 
    if (node == NULL) 
        return;
    (*count)++;
    Bfs(node->next,count); 
    Bfs(node->child,count);
}
void Print_MAX_level(Poke_Node* node,int max) //DEĞİŞTİR PREDORDER
{ 
    if (node == NULL) 
        return;
    if(node->level==max)
        cout<<"P_HP:"<<node->pika_hp<<" "<<"P_PP:"<<node->pika_pp<<" "<<"B_HP:"<<node->blast_hp<<" "<<"B_PP:"<<node->blast_pp<<" "<<"PROB:"<<node->prob<<endl; 
    Print_MAX_level(node->child,max); 
    Print_MAX_level(node->next,max); 
}   

void add(Poke_Node* node,int numb,pokemon blast,pokemon pika) 
{ 
    if (node == NULL) 
        return; 
    
    add(node->next,numb,blast,pika); 
    float divide=3.0; //If no skip
    if(numb==2)
        divide=4.0;  // If there is skip
    if(node->isleaf==1)
        return;
    if(node->level==numb){
        if(numb%2==0){ //Pikachu's attacks
            for (int j = 0; j < divide; j++)
            {
                    if(j==0){
                            Poke_Node* newnode=new Poke_Node();
                            node->child=newnode;
                            equal(newnode,node);
                            if(node->pika_pp+pika.PP[j]>=0 ){
                                newnode->pika_pp+=pika.PP[j];
                                newnode->blast_hp -=pika.Damage[j];
                                if(newnode->blast_hp<=0){
                                    newnode->blast_hp=0;
                                    newnode->isleaf=1;
                                }
                                newnode->attack_name=pika.attack_name[j];
                                newnode->level=numb+1;
                                newnode->prob=((node->prob)/divide);
                                node->child->parent=node;
                            }
        
                    }
                    else if(pika.Accuracy[j]!=100){
                            if(node->pika_pp+pika.PP[j]>=0 ){
                                Poke_Node* temp=node->child;
                                for (int k = 1; k < j; k++)
                                {
                                    temp=temp->next->next;
                                }
                                Poke_Node* nextnode=new Poke_Node();
                                temp->next=nextnode;
                                equal(nextnode,node);
                                nextnode->pika_pp+=pika.PP[j];
                                nextnode->blast_hp -=pika.Damage[j];
                                if(nextnode->blast_hp<=0){
                                        nextnode->blast_hp=0;
                                        nextnode->isleaf=1;
                                }
                                nextnode->level=numb+1;
                                nextnode->attack_name=pika.attack_name[j];
                                nextnode->prob=(node->prob/divide)*(pika.Accuracy[j]/100.0);
                                temp->next->parent=node;
                                Poke_Node* nextnode2=new Poke_Node();
                                temp->next->next=nextnode2;
                                equal(nextnode2,node);
                                nextnode2->pika_pp+=pika.PP[j];
                                nextnode2->level=numb+1;
                                nextnode2->effective=0;
                                nextnode2->attack_name=pika.attack_name[j];
                                nextnode2->prob=(node->prob/divide)*(1-(pika.Accuracy[j]/100.0));
                                temp->next->next->parent=node;
                            }
                    }
                    else{//For skip
                        if(node->pika_pp+pika.PP[j]>=0 ){
                            Poke_Node* temp=node->child;
                            for (int k = 1; k < j; k++)
                            {
                                temp=temp->next->next;
                            }
                            Poke_Node* nextnode=new Poke_Node();
                            temp->next=nextnode;
                            equal(nextnode,node);
                            nextnode->pika_pp+=pika.PP[j];
                            nextnode->blast_hp -=pika.Damage[j];
                            if(nextnode->blast_hp<=0){
                                        nextnode->blast_hp=0;
                                        nextnode->isleaf=1;
                            }
                            nextnode->level=numb+1;
                            nextnode->attack_name=pika.attack_name[j];
                            nextnode->prob=(node->prob/divide)*(pika.Accuracy[j]/100.0);
                            temp->next->parent=node;
                        }
                    }
            }
        }
        else // Blastoise's attacks
        {
            for (int j = 0; j < divide; j++)
                { 
                    if(j==0){
                        if(node->blast_pp+blast.PP[j]>=0 ){
                            Poke_Node* newnode=new Poke_Node();
                            node->child=newnode;
                            equal(newnode,node);
                            newnode->blast_pp+=blast.PP[j];
                            newnode->pika_hp -=blast.Damage[j];
                            if(newnode->pika_hp<=0){
                                newnode->pika_hp=0;
                                newnode->isleaf=1;
                            }
                            newnode->level=numb+1;
                            newnode->attack_name=blast.attack_name[j];
                            newnode->prob=((node->prob)/divide);
                            node->child->parent=node;
                        }
                    }
                    else if (j==1)
                    {
                        if(node->blast_pp+blast.PP[j]>=0 ){
                            Poke_Node* newnode=new Poke_Node();
                            node->child->next=newnode;
                            equal(newnode,node);
                            newnode->blast_pp+=blast.PP[j];
                            newnode->pika_hp -=blast.Damage[j];
                            if(newnode->pika_hp<=0){
                                    newnode->pika_hp=0;
                                    newnode->isleaf=1;
                            }
                            newnode->level=numb+1;
                            newnode->attack_name=blast.attack_name[j];
                            newnode->prob=((node->prob)/divide);
                            node->child->next->parent=node;
                        }
                    }
                    else if(j==2)
                    {
                        if(node->blast_pp+blast.PP[j]>=0 ){
                            Poke_Node* newnode=new Poke_Node();
                            node->child->next->next=newnode;
                            equal(newnode,node);
                            newnode->blast_pp+=blast.PP[j];
                            newnode->pika_hp -=blast.Damage[j];
                            if(newnode->pika_hp<=0){
                                    newnode->pika_hp=0;
                                    newnode->isleaf=1;
                            }
                            newnode->level=numb+1;
                            newnode->attack_name=blast.attack_name[j];
                            newnode->prob=((node->prob)/divide);
                            node->child->next->next->parent=node;
                        }
                    }
                    else{ // For skip
                        if(node->blast_pp+blast.PP[j]>=0 ){
                            Poke_Node* newnode=new Poke_Node();
                            node->child->next->next->next=newnode;
                            equal(newnode,node);
                            newnode->blast_pp+=blast.PP[j];
                            newnode->pika_hp -=blast.Damage[j];
                            if(newnode->pika_hp<=0){
                                        newnode->pika_hp=0;
                                        newnode->isleaf=1;
                                }
                            newnode->level=numb+1;
                            newnode->attack_name=blast.attack_name[j];
                            newnode->prob=((node->prob)/divide);
                            node->child->next->next->parent=node;
                        }
                    }
                }
        }
    }
    add(node->child,numb,blast,pika); 
}   
void Delete(Poke_Node* node){
    if(node==NULL)
        return;
    Delete(node->child);
    delete node;
    Delete(node->next);
}

int main(int argc, char *argv[]) {
    
    if(argc<3)
        return 1;
    string line,name;
    pokemon pika;
    pokemon blast;
    //READ PİKACHU FİLE
    ifstream  pikafile ("pikachu.txt");
    if (pikafile.is_open())
    {
        int i=0;
        getline(pikafile, line);
        while (getline(pikafile, line)) //read 3 of them for a line
        {
            stringstream ss(line);
            getline(ss, pika.attack_name[i],',');
            getline(ss, name,',');
            pika.PP[i]=atoi(name.c_str());
            getline(ss, name,',');
            pika.Accuracy[i]=atoi(name.c_str());
            getline(ss, name,',');
            pika.Damage[i]=atoi(name.c_str());
            getline(ss, name,',');
            pika.FirstUsage[i]=atoi(name.c_str());
            i++;
        }
    }
    pikafile.close();
    //READ Blastoise FİLE
    ifstream  Blastoise_file ("blastoise.txt");
    if (Blastoise_file.is_open())
    {
        int i=0;
        getline(Blastoise_file, line);
        while (getline(Blastoise_file, line)) //read 3 of them for a line
        {
            stringstream ss(line);
            getline(ss, blast.attack_name[i],',');
            getline(ss, name,',');
            blast.PP[i]=atoi(name.c_str());
            getline(ss, name,',');
            blast.Accuracy[i]=atoi(name.c_str());
            getline(ss, name,',');
            blast.Damage[i]=atoi(name.c_str());
            getline(ss, name,',');
            blast.FirstUsage[i]=atoi(name.c_str());
            //cout<<blast.attack_name[i]<<" "<<blast.PP[i]<<" "<<blast.Accuracy[i]<<" "<<blast.Damage[i]<<" "<<blast.FirstUsage[i]<<endl;
            i++;
        }
    }
    Poke_Node* head=new Poke_Node();
   // Poke_Node* now=new Poke_Node();
 
    head->pika_hp=200;
    head->pika_pp=100;
    head->blast_hp=200;
    head->blast_pp=100;
    head->prob=1;
    head->level=0;
    head->isleaf=0;
    head->parent=NULL;

    string part_1="part1";
    string part_2="part2";
    string part_3="part3";
    string bfs="bfs";
    string dfs="dfs";
    //PART1
    if(argv[1]==part_1){
        int count=atoi(argv[2]);
        for (int i = 0; i <= count; i++)
            add(head,i-1,blast,pika);
        Print_MAX_level(head,atoi(argv[2]));
    }
    //PART 2
    else if(argv[1]==part_2){
        int count=atoi(argv[2]);
        for (int i = 0; i <= count; i++)
            add(head,i-1,blast,pika);
        if(argv[3]==bfs){
            int node_count=0;
            timeval start ,end; 
            gettimeofday(&start, NULL);
            Bfs(head,&node_count);
            gettimeofday(&end, NULL);
            cout<<"Node count in BFS: "<<node_count<<endl;
        	long seconds = (end.tv_sec - start.tv_sec);
	        long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            cout << "Time taken by function: "<<micros << " microseconds" << endl; 
        }
        else if(argv[3]==dfs)
        {
            int node_count=0;
            timeval start ,end; 
            gettimeofday(&start, NULL);
            Dfs(head,&node_count);
            gettimeofday(&end, NULL);
            cout<<"Node count in DFS: "<<node_count<<endl;
        	long seconds = (end.tv_sec - start.tv_sec);
	        long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            cout << "Time taken by function: "<<micros << " microseconds" << endl; 
        }
        
    }
    //PART3
    else if(argv[1]==part_3){
        for (int i = 0; i <= 8; i++) // 9 layers are enough for two of them.
            add(head,i-1,blast,pika);
        stack<string> pika_stack,blast_stack; //pika_stack holds the road in which pikachu wins.
        float prob1=0,prob2=0;
        bfs_part3(head,&pika_stack,&blast_stack,&prob1,&prob2); //Used bfs algorithm with highest probability
        string pikachu="pikachu";
        string blastoise="blastoise";
        if(argv[2]==pikachu){// Pikachu wins
            while(!pika_stack.empty()){
                cout<<"Pikachu used "<<pika_stack.top()<<". ";
                pika_stack.pop();
                cout<<"It's "<<pika_stack.top()<<"."<<endl;
                pika_stack.pop();
                if(int(pika_stack.top()[0])<60) // If it is number
                {
                    cout<<"level : "<<pika_stack.top()<<endl;
                    pika_stack.pop();
                    cout<<"prob : " <<pika_stack.top()<<endl;
                    pika_stack.pop();
                }
                else{
                    cout<<"Blastoise used "<<pika_stack.top()<<". ";
                    pika_stack.pop();
                    cout<<"It's "<<pika_stack.top()<<"."<<endl;
                    pika_stack.pop();
                }
            }

        }
        else if(argv[2]==blastoise){ //Blastoise wins
            while(!blast_stack.empty()){

                cout<<"Pikachu used "<<blast_stack.top()<<". ";
                blast_stack.pop();
                cout<<"It's "<<blast_stack.top()<<"."<<endl;
                blast_stack.pop();

                cout<<"Blastoise used "<<blast_stack.top()<<". ";
                blast_stack.pop();
                cout<<"It's "<<blast_stack.top()<<"."<<endl;
                blast_stack.pop();
                if(int(blast_stack.top()[0])<60) //If it is number
                {
                    cout<<"level : "<<blast_stack.top()<<endl;
                    blast_stack.pop();
                    cout<<"prob : " <<blast_stack.top()<<endl;
                    blast_stack.pop();
                }
            }
        }
    }
    Delete(head);
    return 0;
}

