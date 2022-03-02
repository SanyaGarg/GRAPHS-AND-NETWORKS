#include <bits/stdc++.h>
#define NIL -1
using namespace std;

set<int> *graph;
vector<int> component;
bool *killed;
set<int> *blocks;


void Graph(int V){
    graph = new set<int>[V];
    blocks = new set<int>[V];
    killed = new bool[V];
    for(int i=0;i<V;i++){
        killed[i] = false;
    }
}

//if (v==w)
//	return NULL;			//STEP3 : checks if self loop, and does not add in the graph

void addEdge(int v,int w){

    if(v==w){
        return;
    }
    // std::set<int>::iterator it;
    // it = std::find (graph[w].begin(), graph[w].end(), v);
    // if(it!=graph[w].end()){
    //     return;
    // }
    graph[v].insert(w);
    graph[w].insert(v);
}

void printGraph(int n){
    for(int i=0;i<n;i++){
        cout << i << " ";
        for(auto j = graph[i].begin();j!=graph[i].end();j++){
            cout << *j << " ";
        }
        cout << "\n";
    }
}

//STEP1
void find_connected_util(int i,vector<bool> &vis,int k){
    vis[i] = true;
    for(auto j = graph[i].begin();j!=graph[i].end();j++){
        int node = *j;
        if(vis[node] == false){
            blocks[k].insert(node);
            find_connected_util(node,vis,k);
        }
    }
}

void find_connected(int n){

    vector<bool> vis(n,false);
    int j = 0;
    for(int i=0;i<n;i++){
        if(!vis[i]){
           // cout << i;
            //component.push_back(i);
            blocks[j].insert(i);
            find_connected_util(i,vis,j);
            j++;
        }
    }
}


//STEP 2
//check cut vertex
void delEdge(int u, int v){
    graph[u].erase(v);
    graph[v].erase(u);
}

void cutVertex_util(int u, bool visited[], int disc[],int low[], int parent[], vector<bool> &ap)
{
    static int time = 0;
    int children = 0;
 
    visited[u] = true;
    disc[u] = low[u] = ++time;

    for (auto i = graph[u].begin(); i != graph[u].end(); ++i)
    {
        int v = *i;  // v is current adjacent of u
 
        // If v is not visited yet, then make it a child of u
        // in DFS tree and recur for it
        if (!visited[v])
        {
            children++;
            parent[v] = u;
            cutVertex_util(v, visited, disc, low, parent, ap);
 
            // Check if the subtree rooted with v has a connection to
            // one of the ancestors of u
            low[u]  = min(low[u], low[v]);
 
            // u is an articulation point in following cases
 
            // (1) u is root of DFS tree and has two or more chilren.
            if (parent[u] == -1 && children > 1)
               ap[u] = true;
 
            // (2) If u is not root and low value of one of its child is more
            // than discovery value of u.
            if (parent[u] != -1 && low[v] >= disc[u])
               ap[u] = true;
        }
 
        // Update low value of u for parent function calls.
        else if (v != parent[u])
            low[u]  = min(low[u], disc[v]);
    }
}

vector<bool> cutVertex(vector<bool> &ap,int V){
    bool *visited = new bool[V];
    int *disc = new int[V];
    int *low = new int[V];
    int *parent = new int[V];
 
    // Initialize parent and visited, and ap(articulation point) arrays
    for (int i = 0; i < V; i++)
    {
        parent[i] = NIL;
        visited[i] = false;
        ap[i] = false;
    }
 
    // Call the recursive helper function to find articulation points
    // in DFS tree rooted with vertex 'i'
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            cutVertex_util(i, visited, disc, low, parent, ap);
 
    // Now ap[] contains articulation points, print them
    for (int i = 0; i < V; i++)
       if (ap[i] == true){
           //cout << i;
           killed[i] = true;
       }
           return ap;
}

void remove_cutvertex(int n){
    vector<bool> ap(n);
    cutVertex(ap,n);
    cout << "\n";

    for(int i=0;i<n;i++){
        if(ap[i]){
            for(auto j = graph[i].begin();j!=graph[i].end();j++){
                delEdge(i,*j);
            }
        }
    }
} 

//STEP 5
void eliminate_vertices(int n){
    for(int i=0;i<n;i++){
        if(graph[i].size()==2){
            set<int>::iterator it1 = std::next(graph[i].begin(), 0);
            set<int>::iterator it2 = std::next(graph[i].begin(), 1);
            int left = *it1;
            int right = *it2;
            //cout << i << " " << left << " " << right << "\n";
            addEdge(left,right);
            delEdge(i,left);
            delEdge(i,right);
            killed[i] = true;
        }
        // printGraph(n);
    }
}

//STEP6
//A
bool check_K2(int i){
    int no_vertices = 0;
    for(auto j = blocks[i].begin();j!= blocks[i].end();j++){
       if(!killed[*j])no_vertices++;
    }

    if(no_vertices==2 || no_vertices==1)return true;
    return false;
}

//B
bool check_fourVertices(int i){
   int no_vertices = 0;
   for(auto j = blocks[i].begin();j!= blocks[i].end();j++){
       if(!killed[*j])no_vertices++;
   }

   // cout << no_vertices;
   if(no_vertices==4 || no_vertices==1)return true;
    return false;
}

//C
//SHORTEST CYCLE
int shortest_cycle(int i){

    int ans = INT_MAX;

    for(auto j = blocks[i].begin();j!=blocks[i].end();j++){
        if(killed[*j])continue;
        int n= blocks[i].size();
        vector<int> dist(n, (int)(1e9));
        vector<int> parent(n, -1);

        // Distance of source to source is 0
        dist[*j] = 0;
        queue<int> q;

        // Push the source element
        q.push(*j);

        // Continue until queue is not empty
        while (!q.empty()) {
 
            // Take the first element
            int x = q.front();
            q.pop();

            for(auto child= graph[*j].begin();child!=graph[*j].end();child++){
                // If it is not visited yet
                if (dist[*child] == (int)(1e9)) {
 
                    // Increase distance by 1
                    dist[*child] = 1 + dist[*j];
 
                    // Change parent
                    parent[*child] = *j;
 
                    // Push into the queue
                    q.push(*child);
                }// If it is already visited
                else if (parent[*j] != *child and parent[*child] != *j)
                    ans = min(ans, dist[*j] + dist[*child] + 1);

            }
        }
    }
        //If graph contains no cycle
    if (ans == INT_MAX)
        return -1;
 
    // If graph contains cycle
    else
        return ans;
}

bool check_step3(int i){
    int degree = 0;
    for(auto j = blocks[i].begin();j!= blocks[i].end();j++){
        degree+=  graph[*j].size();
    }

    int n = blocks[i].size();
    int m = degree/2;
    int k = shortest_cycle(i);

    if(n==1)return true;
    if(n>=5 && m>=7 && m<=k*(n-2)/(k-2)){
        return true;
    }

    return false;
}

//Kwartoski
bool check_K5(set<int> *gr,int n){

    for(int i = 0;i<n;i++){
        int degree = 0;
        for(auto j = blocks[i].begin();j!= blocks[i].end();j++){
            degree+=  graph[*j].size();
        }

        int n = blocks[i].size();
        int m = degree/2;

        if(n==5 && m==10)return true;
    }
    return false;
}

bool check_K33(set<int> *gr,int y){
    for(int i = 0;i<y;i++){
        int degree = 0;
        for(auto j = blocks[i].begin();j!= blocks[i].end();j++){
            degree+=  graph[*j].size();
        }

        int n = blocks[i].size();
        int m = degree/2;

        if(n==6 && m==9){
            set<int> p1,p2;
            bool visited[n] = {false};

            for(auto j = blocks[i].begin();j!= blocks[i].end();j++){
                if(!visited[*j]){
                    p1.insert(*j);
                    visited[*j] = true;

                    for(auto k =graph[*j].begin();k!=graph[*j].end();k++){
                        visited[*k] = true;
                        p2.insert(*k);
                    }
                }
            }
            if(p1.size()==3 && p2.size()==3)return true;
        }
    }
    return false;
}

bool subgraphs(set<int> *gr,int n){
     
     for(int i=0;i<n;i++){
        for(auto j = gr[i].begin();j!=gr[i].end();j++){
            delEdge(i,*j);
            if(check_K5(gr,n) || check_K33(gr,n))return false;
            subgraphs(gr,n);
            addEdge(i,*j);
        }
        cout << "\n";
    }
    return true;
}

int main(){
    int n = 6;
    Graph(n);
    //Petersen Graph
    // addEdge(0, 1);
    // addEdge(0, 5);
    // addEdge(1, 2);
    // addEdge(1, 6);
    // addEdge(2, 3);
    // addEdge(2, 7);
    // addEdge(3, 8);
    // addEdge(3, 4);
    // addEdge(4, 9);
    // addEdge(4, 0);
    // addEdge(5, 7);
    // addEdge(5, 8);
    // addEdge(7, 9);
    // addEdge(8, 6);
    // addEdge(6, 9);

    //K33 
    // addEdge(0, 5);
    // addEdge(0, 3);
    // addEdge(0, 4);
    // addEdge(1, 5);
    // addEdge(1, 3);
    // addEdge(1, 4);
    // addEdge(2, 5);
    // addEdge(2, 3);
    // addEdge(2, 4);

    // addEdge(0,1);
    // addEdge(0,2);
    // addEdge(0,5);
    // addEdge(0,4);
    // addEdge(1,2);
    // addEdge(0,2);
    // addEdge(0,2);
    // addEdge(1,2);
    // addEdge(2,3);
    // addEdge(3,0);
    // addEdge(1,3);
    
    //STEP2
    remove_cutvertex(n);

    //STEP1
    find_connected(n);
    // for(int i=0;i<component.size();i++){
    //     cout << component[i] << " ";
    //     cout << "\n";
    // }
    
   
    //STEP5
    eliminate_vertices(n);

    int planarity = 1;
    for(int i = 0;i<n;i++){
        if(blocks[i].size()>0 && !check_K2(i) && !check_fourVertices(i) && !check_step3(i)) planarity = 0;
    }

    printGraph(n);

    set<int> duplicate[n];
    for(int i=0;i<n;i++){
        for(auto j = graph[i].begin();j!=graph[i].end();j++){
            duplicate[i].insert(*j);
        }
        cout << "\n";
    }

     for(int i=0;i<n;i++){
        cout << i << " ";
        for(auto j = duplicate[i].begin();j!=duplicate[i].end();j++){
            cout << *j << " ";
        }
        cout << "\n";
    }
   // cout <<subgraphs(duplicate,n);
    //cout << check_K33(duplicate,n);
    if(planarity == 1){
        cout << "Yes, it is planar.";
    }else{
        cout << "No, it is not planar.";
    }

    
    
    //printGraph(n);
}
