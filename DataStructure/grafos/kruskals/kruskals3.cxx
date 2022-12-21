#include <fstream>
#include <iostream>


/************************************************************

-> This Program is to implement Kruskal algorithm.

-> This program is to find minimum spanning tree
for undirected weighted graphs

-> Data Structers used:
Graph:Adjacency Matrix

-> This program works in microsoft vc++ 6.0 environment.

**************************************************************/

struct kruskal
{
  int read_graph();
  void initialize_span_t();
  void sort_edges();
  void algorithm();
  int find_node(int );
  void print_min_span_t();
  
  private:
  
  int n; //no of nodes
  int noe; //no edges in the graph
  int top[100];
  int tree[10][10];
  int sets[100][10];
  int graph_edge[100][4];  
};

int kruskal::read_graph()
{
 std::cout<<"\n\tThis program implements the kruskal algorithm"
          <<"\n\tEnter the no. of nodes in the undirected weighted graph: ";
 std::cin>>n;
 noe=0;

 std::cout<<"\n\tEnter the weights for the following edges::";

   for(int i=1;i<=n;i++)
    {
     for(int j=i+1;j<=n;j++)
      {
       std::cout<<"\n\t"<<i<<" , "<<j<<": ";
       int w;
       std::cin>>w;
       
       if(w!=0)
        {
         noe++;
         graph_edge[noe][1]=i;
         graph_edge[noe][2]=j;
         graph_edge[noe][3]=w;
        }
      }
    }

 // print the graph edges

 std::cout<<"\n\tThe edges in the given graph are:";
 for(int i=1;i<=noe;i++)
  {
   std::cout<<"\n\t < "<<graph_edge[i][1]
            <<" , "<<graph_edge[i][2]
            <<" > "<<graph_edge[i][3]<<"\n";
  }
}

void kruskal::sort_edges()
{
 /**** Sort the edges using bubble sort in increasing order**************/

 for(int i=1;i<=noe-1;i++)
  {
    for(int j=1;j<=noe-i;j++)
    {
     if(graph_edge[j][3]>graph_edge[j+1][3])
      {
        int t=graph_edge[j][1];
        graph_edge[j][1]=graph_edge[j+1][1];
        graph_edge[j+1][1]=t;

        t=graph_edge[j][2];
        graph_edge[j][2]=graph_edge[j+1][2];
        graph_edge[j+1][2]=t;

        t=graph_edge[j][3];
        graph_edge[j][3]=graph_edge[j+1][3];
        graph_edge[j+1][3]=t;
      }
    }
  }

 // print the graph edges
 std::cout<<"\n\tAfter sorting the edges in the given graph are:";
 for(int i=1;i<=noe;i++)
  std::cout<<"\n\t"<< graph_edge[i][1]
           <<" , "<<graph_edge[i][2]
           <<" > ::"<<graph_edge[i][3]<<"\n";
}

void kruskal::algorithm()
{
    // ->make a set for each node
    for(int i=1;i<=n;i++)
    {
    sets[i][1]=i;
    top[i]=1;
    }

    std::cout<<"\n\tThe algorithm starts";

    for(int i=1;i<=noe;i++)
    {
    int p1=find_node(graph_edge[i][1]);
    int p2=find_node(graph_edge[i][2]);

        if(p1!=p2)
        {
         std::cout<<"\n\n\tThe edge included in the tree is: "
                  <<"< "<<graph_edge[i][1]<<" , "
                  <<graph_edge[i][2]<<" >";

            tree[graph_edge[i][1]][graph_edge[i][2]]=graph_edge[i][3];
            tree[graph_edge[i][2]][graph_edge[i][1]]=graph_edge[i][3];

            // Mix the two sets

            for(int j=1;j<=top[p2];j++)
            {
                top[p1]++;
                sets[p1][top[p1]]=sets[p2][j];
            }

            top[p2]=0;
        }
        else
        {
         std::cout<<"\n\n\tInclusion of the edge: "
                  <<"< "<<graph_edge[i][1]<<" , "
                  <<graph_edge[i][2]<<" > "
                  <<"\n\n\tforms a cycle so it is removed:\n";
        }
    }
}

int kruskal::find_node(int n)
{
 for(int i=1;i<=noe;i++)
  {
   for(int j=1;j<=top[i];j++)
    {
     if(n==sets[i][j])
     return i;
    }
  }
 return -1;
}

void kruskal::print_min_span_t()
{
 for(int i=1;i<=n;i++)
  {
    std::cout<<"\t";  
   for(int j=1;j<=n;j++)
    std::cout<<tree[i][j]<<"   ";
    std::cout<<"\n\n";
  }
}

int main()
{
 kruskal kr;
 
 kr.read_graph();
 kr.sort_edges();
 kr.algorithm();
 kr.print_min_span_t();
 return 0;
}