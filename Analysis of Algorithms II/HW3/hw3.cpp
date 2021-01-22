/* @Author
 * Student Name: <Beraat BUZ>
 * Student ID: <150160002>
 * Date: <27.05.2020> */
#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list> 
#include <algorithm>

using namespace std;

//Test Suite struct
struct test
{
    string name;
    int bug;
    int time;
    list<string> freq;
};
// Knapsack Algorithm opt funtion
int opt(int i, int w,int wi,int vi,int**arr){
    if(i==0)
        return 0;
    else if(wi>w)
        return arr[i-1][w];
    else
        return max(arr[i-1][w],vi+arr[i-1][w-wi]);
}
/*Calculates distance between two string */
int distance_function(string str1, string str2, int len1, int len2) 
{ 
    int results[len1 + 1][len2 + 1]; 
    for (int i = 0; i <= len1; i++) { 
        for (int j = 0; j <= len2; j++) { 
            if (i == 0) 
                results[i][j] = j; 
            else if (j == 0) 
                results[i][j] = i;  
            else if (str1[i - 1] == str2[j - 1]) 
                results[i][j] = results[i - 1][j - 1]; 
            else
                results[i][j] = 1 + min(min(results[i][j - 1],results[i - 1][j]), results[i - 1][j - 1]); // Insert ,Remove ,Replace
        } 
    } 
  
    return results[len1][len2];  // last element
} 

int main(int argc, char *argv[]){

    ifstream  testfile (argv[1]);
    list <test> suite; // list of test suites
    test temp; //temporary test suite
    string a,b; // needed to hold values from txt
    int max; // max allowed time
    if (testfile.is_open())
    {
        getline(testfile,a);
        string delimiter=": ";
        a.erase(0, a.find(delimiter)+delimiter.length());
        istringstream(a) >> max; // read max running time

        while(testfile)
        {
            getline(testfile,a);
            if(a=="\0" || (a[0]!='T' && a[1]!='S'))
                continue;
    
            istringstream iss(a);
            iss>>temp.name; // read test suite's name
            iss>>temp.bug;  // read test suite's bug count
            iss>>temp.time; // read test suite's time 
            
            while(iss){
                /* Read Frequency */
                iss>>b;
                if(iss=='\0')
                    break;
                temp.freq.push_back(b);
            }
            
            suite.push_front(temp);
            while(!temp.freq.empty()) // delete from temp previous frequencies list 
                temp.freq.pop_front();
        }
    }


    // Create opt array for knapsack algorithm
    int** opt_array = new int*[suite.size()+1];
    for(int i = 0; i < suite.size()+1; ++i)
        opt_array[i] = new int[max+1];

    //our suites in list goes to array to implement easily
    int size=suite.size()+1;
    test test_suites[size];
    for (unsigned int k=0; k<size-1; k++)
    {
        test_suites[k]=suite.back();
        suite.pop_back();
    }
    
    //implement knapsack algorithm
    for(int i=0;i<size;i++){
        if(i>0)
            temp=test_suites[i-1];
        for (int j = 0; j <= max; j++)
            opt_array[i][j]=opt(i,j,temp.time,temp.bug,opt_array);
            
    }
    
    int answer = opt_array[size-1][max];  // total amont is the last element in array
    cout<<endl;
    cout<<"Total amount of running time: "<<answer<<endl<<endl; 
    
    // To find which test suites are selected
    int weight = max; 
    list<int> chosen; //selecet one is holded in this list, for example ; for TS1 -> only integer 1 value
    for (int i = size-1; i > 0 && answer > 0; i--) {  
        if (answer == opt_array[i - 1][weight])  
            continue;         
        else{ 
            answer = answer - test_suites[i - 1].bug; 
            weight = weight - test_suites[i - 1].time; 
            chosen.push_back(int(test_suites[i - 1].name[2])-int('0')-1);
        } 
    }
    /*   SECOND PART*/     

    for (int i = 0; i < size; i++)
    {   
        if(!(find(chosen.begin(), chosen.end(), i) != chosen.end())) // Find selected test cases
            continue;
        string sequence[test_suites[i].freq.size()];  //needed for each freq seq in a Test Suite

        int covered_list[test_suites[i].freq.size()]; 
        int count=0; 
        list<string>::iterator it;

        for (it = test_suites[i].freq.begin(); it != test_suites[i].freq.end(); ++it){ //for each freq sequence in a test suite
            string temp_freq = *it;

            stringstream  linestream(temp_freq);
            list<int>frequ; // string to integer for frequencies
            while(linestream){
                string data;
                if(!getline(linestream, data, '-'))
                    break;
                int intfreq;
                istringstream(data)>>intfreq;
                frequ.push_back(intfreq);
            }

            int freq_seq[frequ.size()]; // create a int array, it holds freqs without '-'
            int size=frequ.size();
            for (int i = 0; i < size; i++)
            {
                freq_seq[i]=frequ.front();
                frequ.pop_front();       
            }

            int covered=0;
            for(int j=0;j<size;j++){ // Find covered value
                if(freq_seq[j]!=0)
                    covered++;
            }
            covered_list[count]=covered; // add covered value to list
            string empty (size,'0');
            sequence[count]=empty; // add sequence to sequence list
            
            int k=0,l=0;
            while(l!=size){
                for(int j=0;j<size;j++){

                    if(freq_seq[j]==k){ // search from 0 to n

                        sequence[count][l]=int('0')+j+1;
                        l++; 
                        continue;
                    }   
                }
                
                k++; //for searching
            }
            count++;
        }
        int max=0;
        int first=0;
        int difference[test_suites[i].freq.size()]; // distance array
        for (int j = 0; j < test_suites[i].freq.size(); j++)
        {
            difference[j]==-1;
            if(covered_list[j]>max) // find, covered value is highest 
            {
                max=covered_list[j];
                first=j; //it is first order
            }
        }

        list<int> order; //final order
        order.push_back(first);

        int sorted_array[test_suites[i].freq.size()-1];
        int inc=0;
        for (int j = 0; j < test_suites[i].freq.size(); j++)
        {
            
            if(first==j)
                continue;
            difference[j]=distance_function(sequence[first], sequence[j], sequence[first].length(), sequence[j].length()); //find distance compare to first one
            sorted_array[inc]=difference[j];
            inc++;
        }
        int n = sizeof(sorted_array)/sizeof(sorted_array[0]);
        sort(sorted_array,sorted_array+n,greater<int>());   // sort distance's
        // add them to order list
        for (int j = 0; j < test_suites[i].freq.size()-1; j++)
        {
            for (int k = 0; k < test_suites[i].freq.size(); k++)
            {
                if(sorted_array[j]==difference[k]&&sorted_array[j]!=-1&&difference[k]!=-1){
                    order.push_back(k);
                    difference[k]=-1;
                }
            }
        }
        // Print the solution
        cout<< "TS" << (chosen.back()+1)<<" ";
        while(!order.empty()){
            cout<<order.front()+1<<" ";
            order.pop_front();
        }
        cout<<endl;
        chosen.pop_back();
    }
    
    // Delete  Opt array
    for( int i = 0; i < size; i++) {
        delete [] opt_array[i];
    }
    delete [] opt_array;
    return 0;
} 

