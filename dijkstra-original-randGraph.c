// A C / C++ program for Dijkstra's single source shortest path algorithm.
// The program is for adjacency matrix representation of the graph


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/time.h>

// Number of vertices in the graph
//#define V 200

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
int minDistance(int dist[], bool sptSet[], int V)
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// A utility function to print the constructed distance array
int printSolution(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; i++)
        printf("%d \t\t %d\n", i, dist[i]);
    return 0;
}

// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(struct Graph *graph, int src, int V)
{
    int dist[V];     // The output array.  dist[i] will hold the shortest
    // distance from src to i

    bool sptSet[V]; // sptSet[i] will true if vertex i is included in shortest
    // path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V-1; count++)
    {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in first iteration.
        int u = minDistance(dist, sptSet, V);

        // Mark the picked vertex as processed
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++){

            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph->w[u][v] && dist[u] != INT_MAX
                && dist[u]+graph->w[u][v] < dist[v]){
                dist[v] = dist[u] + graph->w[u][v];
            }

        }
    }

    // print the constructed distance array
    //printSolution(dist, V);
}

// driver program to test above function
int main(int argc, char** argv){


    int i = 1, v;
    int nVertices = atoi(argv[1]);
    int nArestas  = (nVertices*nVertices)/2;//nVertices*10;
    int seed = i;

    //printf("creating graph\n");

    struct Graph *graph = createRandomGraph(nVertices, nArestas, seed);
    struct timeval tv;

    //printf("starting Dijkstra\n");
    //gettimeofday(&tv, 0);
    //long t1 = tv.tv_sec*1000 + tv.tv_usec/1000;
    struct timeval t1;
    gettimeofday(&t1, 0);

    dijkstra(graph, 0, nVertices);

    struct timeval t2;
    gettimeofday(&t2, 0);
    printf("%f\n", (t2.tv_sec*1000. + t2.tv_usec/1000.) - (t1.tv_sec*1000. + t1.tv_usec/1000.));

    //gettimeofday(&tv, 0);
    //long t2 = tv.tv_sec*1000 + tv.tv_usec/1000;
    //printf ("Test finished. %ldms.\n", t2-t1);
    //printf ("%ld\n", t2-t1);

    for (v=0; v<nVertices; v++)
	     free(graph->w[v]);
	  free(graph->w);
	  free(graph);

    //free(dist);

    return 0;
}
