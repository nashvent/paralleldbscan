#include <bits/stdc++.h>

using namespace std;

typedef vector<float> fvector;


struct Punto{
    bool visited,noise,hasCluster;
    int index;
    vector<float>data;
    Punto(fvector n_data,int n_index=-1){
        data=n_data;
        visited=false;
        noise=false;
        hasCluster=false;
        index=n_index;
    }
    void print(){
        for(int x=0;x<data.size();x++){
            cout<<data[x]<<" ";
        }
        cout<<endl;
    }
};
typedef vector<Punto*> PLista;

struct Cluster{
    PLista pnts;
    Cluster(){
    }

    void addPunto(Punto * p){
        pnts.push_back(p);
        p->hasCluster=true;
    }

    void print(){
        cout<<"#### Cluster #####"<<endl;
        for(int i=0;i<pnts.size();i++){
            //cout<<pnts[i]->index<<" ";
            pnts[i]->print();
        }
        cout<<endl;
    }
};

typedef vector<Cluster*> CLista;


float distEuclidiana(fvector a,fvector b){
    float dist=0;
    if(a.size()==b.size()){
        for(int x=0;x<a.size();x++){
            dist+=(pow(a[x]-b[x],2));
        }
    }
    return sqrt(dist);
}

PLista query(Punto* p,PLista X, float eps){
    PLista qList;
    for(int x=0;x<X.size();x++){
        if(distEuclidiana(p->data,X[x]->data)<eps){
            qList.push_back(X[x]);
        }
    }
    return qList;
}
bool ordenar (Punto* i, Punto* j) {
  for(int x=0;x<i->data.size();x++){
      if(i->data[x]>j->data[x])
        return false;
  }
  return true;
  //return (i->data[0]==j->data[0] and i->data[1]==j->data[1]);
}



void expand(Punto* p,PLista Np,PLista X, Cluster* C,float eps,int minpts){
    C->addPunto(p);
    for(int x=0;x<Np.size();x++){
        Punto *o=Np[x];
        if(!o->visited){
            o->visited=true;
            PLista No=query(o,X,eps);
            if(No.size()>=minpts){
                Np.insert(Np.end(), No.begin(), No.end());
            }
                
        }
        if(!o->hasCluster)
            C->addPunto(o);
    }
}

CLista DBSCAN(PLista X,float eps,int minpts){
    CLista clusters;
    for(int xi=0;xi<X.size();xi++){
        Punto *p=X[xi];
        if(!p->visited){
            p->visited=true;
            PLista Np=query(p,X,eps);
            if(Np.size()<minpts){
                p->noise=true;
            }
            else{
                Cluster* C=new Cluster();
                clusters.push_back(C);
                expand(p,Np,X,C,eps,minpts);
            }
        }
    }
    return clusters;
}


int main(int argc, char** argv){
    PLista puntos;       
    int index;
    float pnt1,pnt2;
    
    int cont=0;
    string epsilon=argv[2];
    string maxPuntos=argv[1];
    
    while(cin>>pnt1>>pnt2){
        vector<float>tvect{pnt1,pnt2};
        Punto *p=new Punto(tvect,cont);
        cont++;
        puntos.push_back(p);
    }
    sort (puntos.begin(), puntos.end(), ordenar);

    float eps=std::stof(epsilon);
    int maxPnts=stoi(maxPuntos);
    

    clock_t begin = clock();

    CLista C=DBSCAN(puntos,eps,maxPnts);
  
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    int noisePuntos=0; 
    for(int x=0;x<puntos.size();x++){
        if(puntos[x]->noise){
            noisePuntos++;
        }
    }
    
    
    std::cout << "\t" << 2 << "\tDimension " << std::endl;
    std::cout << "\t" << puntos.size() << "\tPuntos " << std::endl;
    std::cout << "\t" << C.size() << "\tClusters " << std::endl;
    std::cout << "\t" << noisePuntos << "\tNoise Points " << std::endl;
    
    cout<<"Tiempo: "<<elapsed_secs<<endl;
    //glutMainLoop();
    return 0;
}