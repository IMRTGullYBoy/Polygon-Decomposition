#include <bits/stdc++.h>
#include <fstream>
using namespace std;
 
const double PI = 3.14;
 
ifstream fin("input.txt");
ofstream fout("output.txt");
ofstream poly("polygon.txt");
class Vertex;
class Edge;
class Face;
 
class Vertex
{
public:
    Edge *inc_edge;
    int id;
    int x, y;
    Vertex(int num, int x, int y, Edge *e)
    {
        this->id = num;
        this->x = x;
        this->y = y;
        this->inc_edge = e;
    }
};
 
class Face
{
public:
    Edge *inc_edge;
    Face(Edge *ed)
    {
        this->inc_edge = ed;
    }
};
 
class Edge
{
 
public:
    Edge *twin, *next, *prev; // pointing from this->twin->org to org;
    Vertex *org;              // destination can be obtained using this->twin->org;
 
    Face *left; // right can be obtained using this->twin->left;
 
    Edge(Vertex *org, Edge *tw, Edge *nex, Edge *pre, Face *l)
    {
        this->org = org;
        this->twin = tw;
        this->next = nex;
        this->prev = pre;
        this->left = l;
    }
};
 
class DCEL
{
 
public:
    vector<Vertex *> vertices;
    vector<Edge *> edges;
    vector<Face *> faces;
 
    DCEL(int n)
    {
        vertices.resize(n);
        edges.resize(n);
        faces.resize(1);
    }
 
    void enumerate_vertices(Face *f)
    {
        cout << "Enumterating Vertices from Face \n";
        Edge *e1 = f->inc_edge;
        Edge *e = e1;
        do
        {
            Vertex *v = e->org;
            cout << "Vertex ";
            cout << v->id << ": (" << v->x << "," << v->y << ")"
                 << "\n";
            e = e->next;
        } while (e != e1);
    }
};
 
void summary(DCEL* g)
{
    int n = g->vertices.size();
    fout<<"Number of Vertices = "<<n<<endl;
    fout<<"Printing Vertices info\n";
    for(int i = 0; i < n; i++){
        fout<<"Point "<<i+1<<" ("<< g->vertices[i]->x<<","<< g->vertices[i]->y  <<")"<<endl;
    }
 
    int es = g->edges.size();
    fout<<"Number of Edges = "<<es<<endl;
 
    for(int i = 0; i < es; i++){
        fout<<"Edges between point "<<g->edges[i]->org->id<<" and "<<g->edges[i]->next->org->id<<endl;
        poly<<g->edges[i]->org->x<<" "<<g->edges[i]->org->y<<" "<<g->edges[i]->next->org->x<<" "<<g->edges[i]->next->org->y;
        if( i != es-1){
            poly<<"\n";
        }
    }
    fout<<endl;
 
}
 
 
 
 
bool isreflex(Vertex* A, Vertex* B, Vertex* C) {
    int x1 = B->x - A->x;
    int y1 = B->y - A->y;
    int x2 = B->x - C->x ;
    int y2 = B->y - C->y;
    int a = x1*x2 + y1*y2;
    int b = x1*y2 - x2*y1;
 
    double  ang = ( atan2(b,a) * 180 / PI) ;
    double max_angle = 180.00;
    if(ang < 0){
        return true;
    }
    return false;
}
 
 
bool isnotch(Vertex* point){
    // cout<<"lolz\n";
    Vertex* aft = point->inc_edge->twin->org;
    Vertex* bef = point->inc_edge->prev->org;
    return isreflex(bef,point,aft);
}
bool isinRect(Vertex* v,int xmin,int xmax,int ymin,int ymax){
    return !(v->x > xmax or v->x < xmin or v->y > ymax or v->y < ymin) ;
}
 
bool isInsidePolygon(deque<Vertex*>vertices,Vertex* v){
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
    } 
    return false;
}
 
 
bool lieonsameside(Vertex* v1, Vertex* v2,Vertex* v3,Vertex* v4){
 
    int c = (v1->x) * (v2->y) - (v2->x)*(v1->y);
    int y21 = v2->y - v1->y;
    int x21 = v2->x - v1->x;
    int val1 = (v3->x)*y21 + (v3->y)*x21;
    int val2 = (v4->x)*y21 + (v4->y)*x21; 
    int res = val1*val2;
 
    if( res > 0){
        return true;
    }
    return false;
 
 
}
 
 
int main()
{
 
        Vertex* p = new Vertex(0,2,2,NULL);
        Vertex* q = new Vertex(1,1,1,NULL);
        Vertex* r = new Vertex(2,2,0,NULL);
        // cout<<"ANS = ";
        // cout<<isreflex(p,q,r);
        // cout<<endl;
 
    int n;
    fin >> n;
    // cout<<"n="<<n<<endl;
    DCEL *root = new DCEL(n);
    for (int i = 0; i < n; i++)
    {
        int x, y;
        fin >> x >> y;
        // cout << x << " " << y << endl;
        Vertex *v = new Vertex(i, x, y, NULL);
        root->vertices[i] = v;
    }
 
    for (int i = 0; i < n; i++)
    {
 
        Edge *e1 = new Edge(root->vertices[i], NULL, NULL, NULL, NULL);
 
        Edge *e2 = new Edge(root->vertices[(i+1)%n], NULL, NULL, NULL, NULL);
 
        e1->twin = e2;
        e2->twin = e1;
        root->edges[i] = e1;
 
        root->vertices[i]->inc_edge = e1;
        //   cout<<"here\n";
        if (i == 0)
        {
            Face *f1 = new Face(e1);
            root->faces[0] = f1;
        }
 
        root->edges[i]->left = root->faces[0];
    }
 
    // setting previous and next edges
    for (int i = 0; i < n; ++i)
    {
 
        root->edges[i]->next = root->edges[(i + 1)%n];
        root->edges[(i + 1)%n]->prev = root->edges[i];
        
    }
 
    // root->enumerate_vertices(root->faces[0]);
    //  summary(root);
    // Algorithm implementation
    deque<Vertex*>L,P;
 
    // Step 1 ->
    
    for(int i=0;i<n;i++){
        P.push_back(root->vertices[i]);
    }
  
 
    // Step 1 
 
    // Step 2 ->
    int m = 1;
    int sz = n;
    L.push_back(root->vertices[0]);
    // Step 2 
    
 
    // Step 3 ->
    while(sz > 3){
        // 3.1
        Vertex* v1 = *(prev(L.end()));
        Vertex* v2;
        deque<Vertex*> ptemp = P;
        Vertex* saved = ptemp.front();
        Vertex* frontt;
        bool gh = false;
        while(ptemp.size()){
            frontt = ptemp.front();
            ptemp.pop_front();
            if(frontt == v1){
                v2 = ptemp.front();
                gh= true;
                break;
            }
 
            
        }
        if(!gh){
            v2 = saved;
        }
       
 
        // 3.2 
        L = {v1,v2};
        // cout<<"V1 = "<<v1->id<<" V2 = "<<v2->id<<endl;
        int i = 2;
        Vertex* va = v1;
        Vertex* vb = v2;
        Vertex* vc;
        for(int i=0;i<n;++i){
            if(root->vertices[i] == v2){
                vc = root->vertices[i+1];
                break;
            }
        }
 
        // 3.2
          
 
        // 3.3
        // cout<<"Checking ";
       
        // cout<<va->id<<" "<<vb->id<<" "<<vc->id<<endl;
 
         if(isreflex(va,vb,vc)){
            // cout<<"im culprit with "<<"v1 ="<<va->id<<" v2 ="<<vb->id<<" v3 = "<<vc->id<<endl;
            // cout<<"Cuplrit details\n";
            // cout<<va->x<<" "<<va->y<<"\n";
            // cout<<vb->x<<" "<<vb->y<<"\n";
            // cout<<vc->x<<" "<<vc->y<<"\n";
            // cout<<va->inc_edge->next->org->id<<endl;
            //  cout<<vb->inc_edge->next->org->id<<endl;
 
        }
        
        while(!isreflex(va,vb,vc) and !isreflex(vb,vc,v1) and !isreflex(vc,v1,v2) and L.size()<n)
        {
            // cout<<"Adding vertex "<<vc->id<<endl;
            L.push_back(vc);
            i ++ ;
 
            va = vb;
            vb = vc;
            // v3 = next();
            for(int i=0;i<n;++i){
                if(root->vertices[i] == vc)
                {
                    vc = root->vertices[(i+1)%n];
                    break;
                }
            }
 
            
 
        }
 
       
 
 
        // cout<<"L size = "<<L.size()<<endl;
        // cout<<"V3 val = "<<vc->id<<endl;
 
        // 3.3
    
    // from here
 
        // 3.4
 
        if(L.size() != P.size()){
            // cout<<"L size = "<<L.size()<<" ,P size = "<<P.size()<<endl;
 
            // 3.4.1
            unordered_set<Vertex*>LPVS;
            for(auto it:P){
                // cout<<"bro, value is "<<it->id<<endl;
                bool found = false;
                for(auto e:L){
                    if(it == e){
                        found = true;
                        // cout<<"we found "<<it->id<<endl;
                        break;
                    }
                }
 
 
                if(!found and isnotch(it)){
 
                    // cout<<"Notch is "<<it->id<<endl;
                    LPVS.insert(it);
                    
                }
            // 3.4.1
 
            // 3.4.2
            while(LPVS.size()){
                int xmin,xmax,ymin,ymax;
                deque<Vertex*>temp = L;
                xmin = temp.front()->x;
                xmax = xmin;
                ymin = temp.front()->y;
                ymax = ymin;
                temp.pop_front();
                while(temp.size()){
                    Vertex* ff = temp.front();
                    temp.pop_front();
                    xmin = min(xmin,ff->x);
                    xmax = max(xmax,ff->x);
                    ymin = min(ymin,ff->y);
                    ymax = max(ymax,ff->y);
                }
 
                bool backward = false;
                while(!backward and LPVS.size()>0){
 
                    Vertex* v = *(LPVS.begin());
                    if(!isinRect(v,xmin,xmax,ymin,ymax)){
                        LPVS.erase(v);
                    }
                    
                    while(!(LPVS.size() == 0 or isinRect(v,xmin,xmax,ymin,ymax))){
                        v = *(LPVS.begin());
                        if(!isinRect(v,xmin,xmax,ymin,ymax)){
                        LPVS.erase(v);
                        }
                    }
 
                    if(LPVS.size()){
                        if(isInsidePolygon(L,v)){ // v is inside polygon of L(m)
                            unordered_set<Vertex*>VTR;
                            Vertex* last = L.back();
 
 
                            // obtain vtr
                            unordered_set<Vertex*>unwanted;
 
 
                            for(auto e:L){
                                // remove e from L 
                                // remove if these 2 points lie on same side
                                if(lieonsameside(v1,v,e,last)){
                                    unwanted.insert(e);
                                }
                            }
                            deque<Vertex*>LL;
                            for(auto e:L){
                                if(!unwanted.count(e)){
                                    LL.push_back(e);
                                }
                            }
                            L = LL;
 
 
 
 
                            backward = true;
                            LPVS.erase(v);
                        // MP1 ends
 
                        }
                    }
 
                }
            } 
 
 
            // 3.4.2
 
 
 
            }
        }
        // cout<<"L size and P size -> "<<L.size()<<" "<<P.size()<<"\n" ;
 
        // 3.4
 
        // 3.5
 
        // cout<<"L back = "<<L.back()->id<<" and v2 val = "<<v2->id<<endl;
        if(L.back() != v2){
 
            
            
          
 
            Vertex* Front = L.front();
            Vertex* Back = L.back();
            bool pq = false;
            // cout<<"frontval = "<<Front->id<<" "<<"backval = "<<Back->id<<endl;
          
 
            if(Front->inc_edge->prev->org == Back){
                pq = true;
   
            }
 
            if(!pq){
            
 
                    
                // cout<<" i should be coming\n";
                Vertex* nexttofront = Front->inc_edge->org;
 
                // 3.5.1 write Lm as partition
                Edge* rev = new Edge(Front,NULL,NULL,NULL,NULL);
                Edge* diag = new Edge(Back,NULL,NULL,NULL,NULL);
 
 
 
                diag->twin = rev;
                rev->twin = diag;
 
 
 
                Edge* ea = Front->inc_edge;
                Edge* eb = Front->inc_edge->prev;
                Edge* ec = Back->inc_edge;
                Edge* ed = Back->inc_edge->prev;
 
                // ea->next = diag;
                // diag->prev = ea;
                // diag->next = ed;
                // ed->prev = diag;
                ea->prev = diag;
                ed->next = diag;
                diag->next = ea;
                diag->prev = ed;
 
 
 
                // eb->prev = rev;
                // rev->next = eb;
                // ec->next = rev;
                // rev->prev = ec;
 
                eb->next = rev;
                ec->prev = rev;
                rev->next = ec;
                rev->prev = eb;
 
 
                root->edges.push_back(diag);
                root->edges.push_back(rev);
 
 
                // Front->inc_edge = diag;
                
 
 
 
                // 3.5.1
                // 3.5.2
                
               
            }
 
             deque<Vertex*>PP;
                for(auto e1:P){
                    bool flag = false;
                    for(auto e2:L){
                        if(e1 == e2){
                            flag = true;
                            break;
                        }
                    }
 
                    if(!flag){
                        PP.push_back(e1);
                    }
 
                }
                unordered_set<Vertex*> PPset;
                while(PP.size()){
                    Vertex* ff = PP.front();
                    PPset.insert(ff);
                    PP.pop_front();
                }
                P.clear();
                for(auto e:PPset){
                    P.push_back(e);
                }
 
 
 
                sz = sz - L.size() + 2;
                // cout<<"Now the size is :"<<sz<<endl;
 
            // cout<<"End of loop with sz = "<<sz<<"\n";
 
           
            // 3.5.2
 
        }
 
       
 
        // 3.5
 
 
 
        // 3.6
        m = m + 1;
        // 3.6
 
 
    }
    // Step 3
 
 
    
    // for(int i = 0; i < n; ++i){
 
    // }
    
 
 
     summary(root);
    // summary(root->vertices);
 
    return 0;
}