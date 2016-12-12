// A C / C++ program for Dijkstra's single source shortest path algorithm.
// The program is for adjacency matrix representation of the graph
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>

// Number of vertices in the graph and number of threads of pthread
//#define V 9

int *dist;     // The output array.  dist[i] will hold the shortest distance from src to i

bool *sptSet; // sptSet[i] will true if vertex i is included in shortest path tree or shortest distance from src to i is finalized

struct Graph *graph;

int V;

int i, v, count,my_rank, p, nElemProc;

int *line;

MPI_Status status;

struct Graph {
    int nVertices;
    int **w;
};

struct Graph *createRandomGraph(int nVertices, int nArestas, int seed) {

    int k,v;
    srandom(seed);

    struct Graph *graph = (struct Graph *) malloc( sizeof(struct Graph) );
    graph->nVertices = nVertices;
    graph->w = (int **) malloc( sizeof(int *)  * nVertices );
    for (v=0; v<nVertices; v++) {
        graph->w[v] = (int *) malloc( sizeof(int) * nVertices );
        for (k=0; k<nVertices; k++)
            graph->w[v][k] = 0; // Division prevents overflows
    }

    for (k=0; k<nArestas; k++) {
        int source = random() % nVertices;
        int dest   = random() % nVertices;
        while (source == dest)
            dest = random() % nVertices;

        int w      = 1 + (random() % 10);
        graph->w[source][ dest ] = w;
        graph->w[dest][ source ] = w;
    }

    return graph;
}


// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(int dist[], bool sptSet[])
{
// Initialize min value
int min = INT_MAX, min_index;
//Get the minimum distance and the vertex that has the minimum distance
for (int v = 0; v < V; v++)
    if (sptSet[v] == false && dist[v] <= min)
        min = dist[v], min_index = v;
return min_index;
}

// A utility function to print the constructed distance array
int printSolution(int dist[], int n)
{
printf("Vertex   Distance from Source\n");
for (i = 0; i < V; i++)
    printf("%d \t\t %d\n", i, dist[i]);

printf(" \n");
return 0;

}

// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(struct Graph *graph, int src, int V)
{

//Code snippet to be performed once
if(my_rank==0){
    // Initialize all distances as INFINITE and stpSet[] as false
    for (i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    // Distance of source vertex from itself is always 0
    dist[src] = 0;
}

// Find shortest path for all vertices
for(count=0;count<V;count++){
    //Process zero performs two broadcasts to send the dist and sptSet to other processes
    MPI_Bcast(dist, V, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(sptSet, V, MPI_C_BOOL, 0, MPI_COMM_WORLD);

    // Pick the minimum distance vertex from the set of vertices not
    // yet processed. u is always equal to src in first iteration.
    //int u = minDistance(dist, sptSet);
    int min = INT_MAX, min_index;
    int u; //u will receive the global minimum distance

    //Find the local minimum distance
    for ( v = my_rank; v < V; v+=p){
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    }

    //Barrier
    MPI_Barrier(MPI_COMM_WORLD);

    //Find the global minimum distance and the vertex that has the minimum distance
    struct{int val; int node;}in, out;
    in.val=min;
    in.node=min_index;

    //Reduction to find the global minimum distance and the vertex that has the minimum distance
    MPI_Allreduce(&in, &out, 1,MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);


    u=out.node;

    if(my_rank==0){
        for(v=0;v<p;v++){
            MPI_Send (graph->w[u], V, MPI_INT,v , 4, MPI_COMM_WORLD);
        }
    }
    MPI_Recv(line, V,MPI_INT, 0, 4, MPI_COMM_WORLD, &status);


    // Mark the picked vertex as processed
    sptSet[u] = true;

    // Update dist value of the adjacent vertices of the picked vertex.
    for ( v = my_rank*nElemProc; v < nElemProc+my_rank*nElemProc; v++){

        // Update dist[v] only if is not in sptSet, there is an edge from
        // u to v, and total weight of path from src to  v through u is
        // smaller than current value of dist[v]
        if (!sptSet[v] && line[v] && dist[u] != INT_MAX
            && dist[u]+line[v] < dist[v]){
            dist[v] = dist[u] + line[v];

        }
    }
    MPI_Send (dist + nElemProc*my_rank, nElemProc,
              MPI_INT, 0, 1, MPI_COMM_WORLD);


    if(my_rank==0){
      for(i=0;i<p;i++){
        MPI_Recv( dist + nElemProc*i, nElemProc,
                  MPI_INT, i,
                  1, MPI_COMM_WORLD, &status);

      }
    }

}

// Print the constructed distance array just once
//if(my_rank==0)
//    printSolution(dist, V);
}

//Main
int main(int argc, char *argv[]){

  //MPI components
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size (MPI_COMM_WORLD, &p);


  int i = 1, v;
  int nVertices = atoi(argv[1]);
  int nArestas  = (nVertices*nVertices)/2;
  int seed = i;


  //only process 0 will use graph
  if(my_rank==0){
    graph = createRandomGraph(nVertices, nArestas, seed);
  }
  //shared resources
  dist = (int *)malloc(nVertices*sizeof(int));
  line = (int *)malloc(nVertices*sizeof(int));
  sptSet = (bool *)malloc(nVertices*sizeof(bool));

  V = nVertices;

  nElemProc=V/p;

  struct timeval t1;
  gettimeofday(&t1, 0);

  //Apply algorithm
  dijkstra(graph, 0, nVertices);

  //Calculate execution time
  if(my_rank==0){
    struct timeval t2;
    gettimeofday(&t2, 0);
    printf("%f\n", (t2.tv_sec*1000. + t2.tv_usec/1000.) - (t1.tv_sec*1000. + t1.tv_usec/1000.));
  //}

    for (v=0; v<nVertices; v++)
	     free(graph->w[v]);
	  free(graph->w);
	  free(graph);
  }

  free(dist);
  free(sptSet);
  free(line);


MPI_Finalize();

return 0;
}
