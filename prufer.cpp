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

// void generate_sister(vector<int> prufer,vector<int> &sister){

// }
void Prufer_to_tree(vector<int> prufer){
    int n = prufer.size(); // N
    int graph_size = n+2;
    vector<int> degree(graph_size);
    Graph(graph_size);
   // cout << n<< "\n";
    priority_queue <int, vector<int>, greater<int> > leaf; //leaves <- MIN_HEAP

    for(int i=0;i<graph_size;i++){
       degree[i] = 0;
    }
    for(int i=0;i<graph_size;i++){ //for i from 0 to graph_size
        degree[i]++;
    }

    for(int i=0;i<n;i++){
        int x = prufer[i];
        degree[x]++;        //degree of all the vertex
    }

    // for(int i=0;i<n+2;i++){
    //     cout << degree[i] << " ";
    // }

    for(int i=0;i<graph_size;i++){
        if(degree[i]==1)
            leaf.push(i);         // create a MIN_HEAP of leaves
    }

    for(int i=0;i<n;i++){
        int leaves = leaf.top();    // pop out nodes with min val
        leaf.pop();
        int prufer_node = prufer[i];
        //cout << leaves << " ";
        degree[prufer_node]--;      
        degree[leaves]--;
        addEdge(prufer_node,leaves);

        if(degree[prufer_node]==1){
            leaf.push(prufer_node);
        }
    }

    int leaves = leaf.top();
    addEdge(leaves,graph_size-1);
}

int main(){
    
    // addEdge(0, 1);
    // addEdge(1, 2);
    // addEdge(2, 3);
    // addEdge(3, 4);
    vector<int> prufer = {2,2,5,5,5,6,6};
    Prufer_to_tree(prufer);
    int n = prufer.size();
    for(int i=0;i<n+2;i++){
        cout << i << " ";
        for (auto j = graph[i].begin(); j != graph[i].end(); ++j){
            cout << *j << " ";
        }
        cout << "\n";
    }
        
}