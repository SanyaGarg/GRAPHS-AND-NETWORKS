#include <bits\stdc++.h>
#define NIL -1
using namespace std;


list<int> *graph;

void Graph(int V){
    graph = new list<int>[V];
}

void addEdge(int v,int w){

    graph[v].push_back(w);
    graph[w].push_back(v);
}

vector<int> generate_prufer(int V){
    vector<int> degree(V);
    vector<bool> deleted(V);                          //Keep a track of deleted leaves
    priority_queue <int, vector<int>, greater<int> > leaves; //leaves <- MIN_HEAP

   for(int i=0;i<V;i++){
       deleted[i] = false;
   }

    for(int i=0;i<V;i++){
        degree[i] = graph[i].size();
        if(degree[i]==1)
            leaves.push(i);         // create a MIN_HEAP of leaves //STEP1
    }

    vector<int> prufer(V-2);
    for(int i=0;i<V-2;i++){     //STEP2
        int leaf = leaves.top();    //pop out the leaves<-instead write EXTRACT_MIN_HEAP
        leaves.pop();           //not necessary
        deleted[leaf] = true;

        int v;

        for(auto j= graph[leaf].begin();j!=graph[leaf].end();j++){  // for each vertex u= G.V
            if(!deleted[*j]){
                v = *j;
            }
        }

        prufer[i] = v;
       // cout << leaf << " " << v << "\n";
        degree[v]--; // degree[v] = degree[v]-1
        if(degree[v]==1){
            leaves.push(v);
        }
    }

    return prufer;
}

int main(){
    Graph(9);
    addEdge(0,2);
    addEdge(1, 2);
    addEdge(2, 5);
    addEdge(3, 5);
    addEdge(4, 5);
    addEdge(6, 5);
    addEdge(6, 7);
    addEdge(6, 8);

    vector<int> code = generate_prufer(9);
    for(int i =0;i<code.size();i++){
        cout << code[i];
    }
}