// A C / C++ program for Dijkstra's single source shortest path algorithm.
// The program is for adjacency matrix representation of the graph
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <mpi.h>
#include<math.h>

// Number of vertices in the graph and number of threads of pthread
#define V 9
#define NUM_THREADS 2

int dist[V];     // The output array.  dist[i] will hold the shortest distance from src to i

bool sptSet[V]; // sptSet[i] will true if vertex i is included in shortest path tree or shortest distance from src to i is finalized

int i, v, count,my_rank, p;

//Graph
int graph[V][V] = {{0, 4, 0, 0, 0, 0, 0, 8, 0},
{4, 0, 8, 0, 0, 0, 0, 11, 0},
{0, 8, 0, 7, 0, 4, 0, 0, 2},
{0, 0, 7, 0, 9, 14, 0, 0, 0},
{0, 0, 0, 9, 0, 10, 0, 0, 0},
{0, 0, 4, 14, 10, 0, 2, 0, 0},
{0, 0, 0, 0, 0, 2, 0, 1, 6},
{8, 11, 0, 0, 0, 0, 1, 0, 7},
{0, 0, 2, 0, 0, 0, 6, 7, 0}
};

//Create pthread
pthread_t threads[NUM_THREADS];

//Struct to be passed as an argument to the threads
struct data{
int u;
int tid;

};

//Function to be performed by each thread to update the distance vector
void *update(void *param){

//Get the values passed as arguments
struct data *val= (struct data *)param;
int u=val->u;
int tid=val->tid;
int current;

// Update dist[current] only if is not in sptSet, there is an edge from
// u to current, and total weight of path from src to  current through u is
// smaller than value of dist[current]
for(current=tid; current<V;current+=NUM_THREADS){
    if (!sptSet[current] && graph[u][current] && dist[u] != INT_MAX){
        if(dist[u]+graph[u][current] < dist[current]){
            dist[current] = dist[u] + graph[u][current];
        }
    }
}
pthread_exit(NULL);
}

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(int dist[], bool sptSet[])
{
// Initialize min value
int min = INT_MAX, min_index;
//Get the minimum distance and the vertex that has the minimum distance
for (v = 0; v < V; v++)
    if (sptSet[v] == false && dist[v] <= min)
        min = dist[v], min_index = v;
return min_index;
}

// A utility function to print the constructed distance array
int printSolution(int dist[], int n)
{
printf("Vertex   Distance from Source\n");
for (int i = 0; i < V; i++)
    printf("%d \t\t %d\n", i, dist[i]);

printf(" \n");
return 0;

}

// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(int graph[V][V], int src)
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
for(count=0;count<V;count++)
{
    //Process zero performs two broadcasts to send the dist and sptSet to other processes
    MPI_Bcast(dist, V, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(sptSet, V, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Pick the minimum distance vertex from the set of vertices not
    // yet processed. u is always equal to src in first iteration.
    //int u = minDistance(dist, sptSet);
    int min = INT_MAX, min_index;
    int u; //u will receive the global minimum distance
    
    //Find the local minimum distance
    for (v = my_rank; v < V; v+=p){
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
    MPI_Reduce(&in, &out, 1,MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
    
    //Critical code snippet
    if(my_rank==0){
    u=out.node;
        
    // Mark the picked vertex as processed
    sptSet[u] = true;
    
    //Create structure
    struct data *val= (struct data *)malloc(V * sizeof(struct data));
    
        //Set val values and create threads
        for (v = 0; v < NUM_THREADS; v++){
            val[v].tid=v;
            val[v].u=u;
            pthread_create(&threads[v], NULL, update,  val+v);
            
        }
        //Join
        for(v=0;v<NUM_THREADS;v++){
            pthread_join(threads[v], NULL);
        }
    }
}

// Print the constructed distance array just once
if(my_rank==0)
printSolution(dist, V);
}

//Main
int main(int argc, char *argv[])
{
struct timeval tv;
gettimeofday(&tv, 0);
long t1 = tv.tv_sec*1000 + tv.tv_usec/1000;

//MPI components
MPI_Init (&argc, &argv);
MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
MPI_Comm_size (MPI_COMM_WORLD, &p);

//Apply algorithm
dijkstra(graph, 0);

//Calculate execution time
if(my_rank==0){
gettimeofday(&tv, 0);
long t2 = tv.tv_sec*1000 + tv.tv_usec/1000;
printf ("Test finished. %ldms.\n", t2-t1);
}

MPI_Finalize();

return 0;
}
