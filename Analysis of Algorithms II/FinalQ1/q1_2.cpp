/*
Beraat Buz
150160002
I compiled with "g++ -std=c++11 q1_2.cpp -o q1_2"
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

void print_arr(vector<int>& arr, int first, int last)
{
	for (int i = first; i <= last; i++)
	{
		string if_eol = (i == last) ? "\n" : ", ";

		cout << arr[i] << if_eol;
	}
}
void MinMaxSort(vector<int>& L, int first,int last)
{ 
    cout<<"Min_max unsorted : ";
    print_arr(L,first,last);
    int n=last-first+1;
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        arr[i]=L[first+i];
    }
    for (int i = 0, j = n - 1; i < j; i++, j--) { 
        int min = arr[i];
        int max = arr[i]; 
        int min_index = i;
        int max_index = i; 
        for (int k = i; k <= j; k++)  { 
            if (arr[k] > max) { 
                max = arr[k]; 
                max_index = k; 
            }
            else if (arr[k] < min) { 
                min = arr[k]; 
                min_index = k; 
            } 
        } 
        cout<<"Min: "<<arr[min_index]<<" Max: "<<arr[max_index]<<endl;
        swap(arr[i], arr[min_index]); 
        if (arr[min_index] == max)  
            swap(arr[j], arr[min_index]); 
        else
            swap(arr[j], arr[max_index]); 
    } 
    for (int i = 0; i < n; i++)
    {
        L[first+i]=arr[i];
    }
    cout<<"Min_max sorted : ";
    print_arr(L,first,last);
    
}
void Merge(vector<int>& L, int first, int mid,int oneplus ,int last){
    int i = first, j= oneplus, k=0;

    int temp[L.size()];
    while (i<=mid && j<=last)
    {
        if(L[i]<L[j])
            temp[k++]=L[i++];
        else
            temp[k++]=L[j++];
    }
    while (i<=mid)
    {
        temp[k++]=L[i++];
    }
    while (j<=last)
    {
        temp[k++]=L[j++];
    }
    i=first,j=0;
    while (i<=last)
    {
        L[i]=temp[j];
        i++;
        j++;
    }
    
}
void func(vector<int>& L, int first, int last){
    int m=0,k=first;
    while(k<=last){
        if(L[k]>L[k+1]){
            int temp=L[k];
            L[k]=L[k+1];
            L[k+1]=temp;
        }
        k=k+2;
    }
    k=first+1;
    int A[L.size()];
    while(k<=last){
        A[m]=L[k]; 
        m=m+1;
        k=k+2;
    }
    k=first;
    int x=first;
    while (k<=last)
    {
        L[x]=L[k];
        x=x+1;
        k=k+2;
    }
    k=0;
    while(k<m){
        L[x]=A[k];
        x=x+1;
        k=k+1;
    }

}

void sort(vector<int>& arr, int first, int last){
    int n = last-first+1;
    if(n<=1)
    {   
        return;
    }
    if(first<last){
        print_arr(arr,first,last);
        func(arr,first,last);
        int Mid=(first+last)/2;
        MinMaxSort(arr,first,Mid);
        sort(arr,Mid+1,last);
        Merge(arr,first,Mid,Mid+1,last);

    }
    
    return;
}
int main(int argc, char *argv[])
{

	vector<int> all_values;


	try {
		
		string filename = argv[1];

		ifstream myfile(filename);

		
		if (myfile.is_open())
		{
			string line;
			while (getline(myfile, line))
			{
				int number = stoi(line);
				all_values.push_back(number);
			}

			sort(all_values, 0, all_values.size() - 1);

			print_arr(all_values, 0, all_values.size() - 1);
		}
		else
		{
			cout << "File is not available." << endl;
			throw;
		}

	}
	catch (const ifstream::failure& e)
	{
		cerr << "Exception opening the file!" << endl;
		return -1;
	}
	catch (...)
	{
		cerr << "Something's wrong!" << endl;
		return -1;
	}



	return 0;
}