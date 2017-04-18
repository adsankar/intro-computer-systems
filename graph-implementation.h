/* This is the header file which describes how the graph
   object is stored. This graph contains a linked list of
   linked lists to store the vertices and the edges for each
   vertex.
   The vertex contains a pointer to the next vertex, a linked
   list describing the edges coming from it and a string
   containing its name.
   The edge stores the cost and name of the destination
   vertex along with a pointer to the next edge.
   The graph struct has a pointer to the head of the linked
   list of vertices and stores the total number of vertices.
*/
typedef struct vertex {
  struct vertex *next;
  struct Edge *edge_list; 
  char *name;
} Vertex;

typedef struct Edge {
  Vertex *source;
  char *dest;
  struct Edge *next;
  int cost;
} Edge;

typedef struct Graph {
  int num_vertices;
  Vertex *head;
} Graph;
