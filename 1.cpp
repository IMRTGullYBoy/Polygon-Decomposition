#include<bits/stdc++.h>
#include<fstream>
using namespace std;
 
ifstream fin("input.txt");
ofstream fout("output.txt");
class Vertex;
class Edge;
class Face;
 
 
class Vertex{
    public:
 
    Edge* inc_edge;
    int id;
    int x,y;
    Vertex(int num,int x,int y,Edge* e){
        this->id = num;
        this->x = x;
        this->y = y;
        this->inc_edge = e;
    }
    
};
 
class Face{
    public:
 
    Edge* inc_edge;
    Face(Edge* ed){
        this->inc_edge = ed;
    }
 
};
 
 
 
 
class Edge{
 
    public:
 
    Edge* twin,*next,*prev;//pointing from this->twin->org to org;
    Vertex* org;//destination can be obtained using this->twin->org;
 
    Face* left;//right can be obtained using this->twin->left;
 
    Edge(Vertex* org,Edge* tw,Edge* nex,Edge* pre,Face* l){
        this->org = org;
        this->twin = tw;
        this->next = nex;
        this->prev = pre;
        this->left = l;
    }
 
};
 
class DCEL{
 
    public:
 
    vector<Vertex*> vertices;
    vector<Edge*> edges;
    vector<Face*> faces;
 
    DCEL(int n){
        vertices.resize(n);
        edges.resize(n);
        faces.resize(1);
    }
 
 
 
};
 
 
 
 
void summary(DCEL* d){
    
    int n = d->vertices.size();
    fout<<"Number of vertices: "<<n<<endl;
    //fout<<"Number of Edges: "<<n+1<<endl;
 
    for(int i=0;i<n;i++){
        fout<<"For vertex "<<i<<" , next vertex "<< d->vertices[i]->inc_edge->twin->org->id<<endl;
    }

}

bool isReflex(Vertex* A, Vertex* B, Vertex* C) {

    // Check the sign of the cross product between each adjacent pair of edges

        int cross_product = (B->x - A->x) * (C->y - B->y) - (B->y - A->y) * (C->x - B->x);

        if (cross_product < 0) {
            return false; // Found a reflex angle
        }
    

    return true; // No reflex angles found
}

bool isInsidePolygon(deque<Vertex*> vertices, Vertex * v) {
    int n = vertices.size();
    int count = 0;

    // Iterate over each edge of the polygon
    for (int i = 0; i < n; i++) {
        int j = (i+1) % n;
        int x1 = vertices[i]->x, y1 = vertices[i]->y;
        int x2 = vertices[j]->x, y2 = vertices[j]->y;

        // Check if the point lies on the edge
        if ((v->y == y1) && (v->x >= x1) && (v->x <= x2)) {
            return true;
        }

        // Check if the edge intersects the ray from the point to the right
        if ((y1 > v->y) != (y2 > v->y)) {
            int x_intersection = x1 + (x2-x1) * (v->y - y1) / (y2-y1);
            if (v->x <= x_intersection) {
                count++;
            }
        }
    }

    // If the number of intersections is odd, the point is inside the polygon
    if (count % 2 == 1) {
        return true;
    } else {
        return false;
    }
}
 
int main(){
 
 
    int n;
    fin>>n;
    // cout<<"n="<<n<<endl;
    DCEL* root= new DCEL(n);
    for(int i=0;i<n;i++){
        int x,y;
        fin>>x>>y;
        //fout<<x<<" "<<y<<endl;
        Vertex* v = new Vertex(i,x,y,NULL);
        root->vertices[i] =  v;
 
 
 
    }
 
 
    for(int i=0;i<n;i++){
 
        Edge* e1 = new Edge(root->vertices[i],NULL,NULL,NULL,NULL);
       
        Edge* e2;
 
        if( i == n-1){
           e2 = new Edge(root->vertices[0],NULL,NULL,NULL,NULL);
        }else{
           e2 = new Edge(root->vertices[i+1],NULL,NULL,NULL,NULL);
        }
        
        e1->twin = e2;
        e2->twin = e1;
        root->edges[i] = e1;
     
 
        root->vertices[i]->inc_edge = e1;
        //   cout<<"here\n";
        if(i == 0){
            Face* f1 = new Face(e1);
            root->faces[0] = f1;
        }
        
        root->edges[i]->left = root->faces[0];
      
    }
 
    // fout<<"Printing edges..."<<endl;
    // for(int i=0;i<n;++i){
    //     fout<<root->edges[i]->org->id<<" ";
    // }
 
    // setting previous and next edges
    for(int i=0;i<n;++i){
        
        if( i == n-1){
            root->edges[i]->next = root->edges[0];
            root->edges[0]->prev = root->edges[i];
        }
        else{
 
            root->edges[i]->next = root->edges[i+1];
            root->edges[i+1]->prev = root->edges[i];
 
        }
    }
 
 
 
 
      
    fout<<isReflex(root->vertices[0], root->vertices[1], root->vertices[2]);
    //summary(root);
 
 
    return 0;
}

