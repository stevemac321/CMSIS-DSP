#include "graph.h"
// graph operations


void create_graph(struct Graph *g, const int sz) {
    g->numVertices = 0;
    g->graph_capacity=sz;

}
// Internal function to add a vertex
void add_vertex(struct Graph* g, const float mag) {
    // Ensure we don't exceed the array size
    if (g->numVertices >= g->graph_capacity) {
        // Handle full graph (e.g., return or log error)
        return;
    }
	g->vertices[g->numVertices].magnitude = mag;
	g->vertices[g->numVertices].color = WHITE;
	g->vertices[g->numVertices].distance = 0;
	g->vertices[g->numVertices].parent = 0;
    // Initialize the adjacency list to 0 (no connections initially)
    g->vertices[g->numVertices].adjList = 0;

    // Increment the number of vertices after initialization
    g->numVertices++;
}

// Internal function to add an edge between two vertices
void add_edge(struct Graph* g, size_t v, size_t w) {
    if (v >= g->numVertices || w >= g->numVertices) return;


    // Set the edge bit in the adjacency list
    g->vertices[v].adjList  |= (1 << (w % g->graph_capacity));
}
//

