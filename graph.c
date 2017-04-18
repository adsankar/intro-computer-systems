/* Aleksander Sankar

   This project makes a graph that is stored in a linked list
   of linked lists, where the edges have weights and are directed. The graph
   has a linked list containing the vertices and each vertex has a linked 
   list containing all of the edges.
   Each vertex is identifiable by a string (char []). Repeat vertices are not
   allowed and edge costs cannot be lower than zero, It is allowable to have 
   an edge between a vertex and itself.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graph.h"


/*This function initializes the graph parameter so that other
  functions can call it. It sets the size of the graph to zero
  and sets the linked list head for the vertices to null
*/
void init_graph(Graph *graph) {

  /*set the number of vertices in this graph to zero*/
  graph -> num_vertices = 0;
  /* set the linked list of vertices to null*/
 
  graph -> head = NULL;
  return;
}

/* This function is a helper that prints out all of the relevant
   information about the graph. It prints out a representation of 
   the adjacency list so that the user can see what is inside it.
*/
void print_adj_list(Graph *graph){
  /*the relevant head nodes*/
  Vertex *h = graph -> head;
  Edge *e = NULL;
  if (graph == NULL) {
    printf("The graph is null\n");
  }
  printf("size: %d\n", graph->num_vertices);
  while (h != NULL) {
    e = h->edge_list;
    printf("%s: ", h->name);
    /*iterate and print the node's info*/
    while (e != NULL){
      printf("[%s, %d] ",e->dest, e->cost);
      e= e->next;
    }
    printf("\n");
    h = h->next;
  }
 
}


/*This function adds a vertex to the graph with the name given
  in the parameter. It allocates memory, checks to see that the
  allocation completed successfully, and increases the size of
  the graph. It adds the vertex in sorted order,
  lexicographically.
*/
int add_vertex(Graph *graph, const char new_vertex[]) {
  Vertex *v = NULL;
  Vertex *t = NULL;
  Vertex **w = NULL;
  if (graph == NULL ||  new_vertex == NULL || graph == NULL){
    /*incorrect parameters*/
    return 0;
  }

  /*if the vertex is already there*/
  if (has_vertex(*graph, new_vertex) == 1) {
    return 0;
  }

  /*allocate the appropriate memory and copy the 
    name to it*/
  v = malloc(sizeof(Vertex));

  /*check if the memory allocation succeeded, and 
    if not, exit*/
  if (!v) {
    printf("Error: Memory allocation of VERTEX failed; exiting\n");
    exit(EXIT_FAILURE);
  }
  
  v -> name = malloc((strlen(new_vertex) + 1) * sizeof(char));
  /*check if the memory allocation succeeded, and if not, exit*/
  if (! (v -> name)) {
    printf("Error: Memory allocation of VERTEX NAME failed; exiting\n");
    exit(EXIT_FAILURE);
  }

  /*add in the appropriate information*/
  strcpy(v -> name, new_vertex);
  v -> edge_list = NULL;

  /*special case if this is the first in the linked list*/
  if (graph -> head == NULL) {
    graph -> head = v;
    v -> next = NULL;
  }
  else {
    /*add in alphabetical order by comparing the names of the vertices */
    t = graph -> head;
    w = & graph -> head;
    while (t != NULL && strcmp(t -> name, v -> name) < 0) {
      w = & t -> next;
      t = t -> next;
     
    }
    *w = v;
    v -> next = t;
  }
  /*increment the size of the graph*/
  graph -> num_vertices++;

  return 1;
}

/*Returns the number of vertices in the Graph parameter 
 */
int num_vertices(Graph graph) {
  
  return graph.num_vertices;
}

/* Determines if the Graph parameter contains the vertex with the 
   name given by the second parameter. It does this by traversing 
   the adjacency list to see if another node has the same source name.
*/
int has_vertex(Graph graph, const char name[]) {
  Vertex *curr = graph.head;
  if (name == NULL)
    return 0;
  /*search array and if the parameter matches the name,
    then it is a vertex  in the graph*/
  while (curr != NULL){
    if (strcmp(name, curr->name) == 0)
      return 1;
    curr = curr->next;
  }
  /*if not found, return 0*/
  return 0;
}

/*This function adds an edge to the graph from the source to the destination
  (both given as parameters) and sets the cost of the one in the parameter.
  It adds the edges in alphabetical order.
  It will not modify the graph if one or both of the vertices is not in the
  graph or if an edge already exists between them.
*/
int add_edge(Graph *graph, const char source[], const char dest[], int cost) {
  /*declare the relevant pointers*/
  Vertex *h;
  Edge *new = NULL;
  Edge **w = NULL;
  Edge *t = NULL;

  /*if the parameters are incorrect, return 0*/
  if (graph == NULL || source == NULL || dest == NULL || (cost < 0))
    return 0;

  h = graph -> head;;
  /*allocate memory for the edge and its name*/
  new = malloc(sizeof(Edge));

  /*check if the memory allocation succeeded, and if not, exit*/
  if (!new) {
    printf("Error: Memory allocation of EDGE failed; exiting\n");
    exit(EXIT_FAILURE);
  }
  
  new -> dest = malloc((strlen(dest) + 1) * sizeof(char));

  /*check if the memory allocation succeeded, and if not, exit*/
  if (! (new -> dest)) {
    printf("Error: Memory allocation of VERTEX DEST failed; exiting\n");
    exit(EXIT_FAILURE);
  }
  
  /*set all of the fields appropriately*/
  strcpy(new -> dest, dest);
  new -> source = h;
  new -> cost = cost;
  new -> next = NULL;
  
  /*edge is already there or one of the vertices is not in the graph*/
  if  (has_vertex(*graph, source) == 0 || has_vertex(*graph, dest) == 0
       || get_edge_cost(*graph, source, dest) != -1 )
    return 0;

  /*find the appropriate vertex by name*/
  while (strcmp(h -> name, source) != 0) {
    h = h -> next;
  }

  /*special case if list is empty*/
  if (h -> edge_list == NULL){
    h -> edge_list = new;
    new -> next = NULL;
  }
  else {
    t = h -> edge_list;
    w = & h -> edge_list;
    /*add new edge in alphabetical order by the names of the destinations*/
    while (t != NULL && strcmp(t -> dest, new -> dest) < 0) {
      w = & t -> next;
      t = t -> next;
    }
    *w = new;
    new -> next = t;
  }
    
  return 1;
}
/*This function returns the cost of the edge between the two vertices
  given as parameters. If no edge exists between them, it returns -1.
*/
int get_edge_cost(Graph graph, const char source[], const char dest[]) {

  Vertex *h = graph.head;
  Edge *e = NULL;
  /*cases for invalid parameters*/
  if (source == NULL|| dest == NULL)
    return -1;
  if (has_vertex(graph, source) == 0 || has_vertex(graph, dest) == 0) {
    return -1;
  }

  /*go through list by name*/
  while (strcmp(h -> name, source) != 0) {
    
    h = h -> next;
  }
  /*get to the appropriate edge list*/
  e = h -> edge_list;
  if (e == NULL)
    return -1;
  /*find the edge you want and return its cost*/
  while (strcmp(e -> dest, dest) != 0) {
    e = e -> next;
    if (e == NULL)
      return -1;
  }

  return e -> cost;
}

/*This function changes the cost of an edge if the edge
  is found in the graph.
*/
int change_edge_cost(Graph *graph, const char source[], const char dest[],
		     int new_cost) {
  Vertex *h;
  Edge *e = NULL;

  /*cases for invalid parameters*/
  if (source == NULL|| dest == NULL || new_cost < 0 || graph == NULL)
    return 0;
  if (has_vertex(*graph, source) == 0 || has_vertex(*graph, dest) == 0) {
    return 0;
  }
  h = graph -> head;
  /*go through list by name*/
  while (strcmp(h -> name, source) != 0) {
    
    h = h -> next;
  }

  /*get to the appropriate edge list*/
  e = h -> edge_list;
  while (strcmp(e -> dest, dest) != 0) {
    e = e -> next;
    if (e == NULL)
      return 0;
  }
  /*set the new cost*/
  e -> cost = new_cost;
  return 1;
}


/*This function returns the number of neighbors that the vertex
  in the parameter has. This number comes from the length of the
  edge list of that vertex.
*/
int num_neighbors(Graph graph, const char vertex[]) {
  int i;
  Vertex *h = graph.head;
  Edge *e = NULL;
  i = 0;
  /*if the vertex is not there*/
  if (vertex == NULL)
    return -1;
  if (has_vertex(graph, vertex) == 0) {
    return -1;
  }

  /*find the correct vertex first*/
  while (strcmp(h -> name, vertex) != 0) {
    
    h = h -> next;
  }
  e = h -> edge_list;
  /*iterate and increment through the list*/
  while (e != NULL) {
    e = e -> next;
    i++;
  }
  return i;
}

/*This function clears the graph in the parameter by deallocating all
  of the memory that the graph points to.
*/
void clear_graph(Graph *graph) {
  Vertex *h;
  Vertex *j;
  Edge *e;
  Edge *t;

  /*cannot deallocate a null graph*/
  if (graph != NULL) {
  
    h = graph -> head;
    while (h != NULL) {
      /*iterate through each vertex, and through each edge*/
      e = h -> edge_list;
      while (e != NULL) {
	/*temporary pointers to safetly perform deallocations*/
	t = e;
	e = e -> next;
	/*free both the name of the edge and the edge itself, and then move
	  to the next one*/
	free(t -> dest);
	free(t);
      }
      j = h;
      h = h -> next;
      /*free both the name of the vertex and the vertex itself, and then
	move to the next one*/
      free(j -> name);
      free(j);
    }
  }
}

/*This function creates and returns a pointer to an array of strings 
  holding vertex names. It uses dynamically allocated memory to store them,
  and sets the last element to null so that the user knows where the end of
  the array is.
*/
char **get_vertices(Graph graph) {
  int j;
  char **vertices = NULL;
  Vertex *h = graph.head;

  /*allocate memory based on the size of the array*/
  vertices = malloc((num_vertices(graph)+1)*sizeof(char*));
  /*check if the memory allocation succeeded, and if not, exit*/
  if (! vertices) {
    printf("Error: Memory allocation of VERTICES failed; exiting\n");
    exit(EXIT_FAILURE);
  }

  for (j = 0; j < num_vertices(graph); j++) {
    /*allocate enough memory for each name*/
    vertices[j] = malloc((strlen(h -> name) + 1) * sizeof(char));
    /*check if the memory allocation succeeded, and if not, exit*/
    if (! vertices[j]) {
      printf("Error: Memory allocation of VERTEX failed; exiting\n");
      exit(EXIT_FAILURE);
    }
    /*copy the vertex name over and increment the array*/
    strcpy(vertices[j], h -> name);
    h = h -> next;
  }
  /*set the last element to null to signal the end of the array*/
  vertices[j] = NULL;

  
  return vertices;
}

/*Similar function to the one above, but it instead returns an array of the
  names of the neighbors to the vertex parameter*/
char **get_neighbors(Graph graph, const char vertex[]) {
  int j;
  char **neighbors = NULL;
  Edge *e = NULL;
  Vertex *h = graph.head;

  if (vertex == NULL || has_vertex(graph, vertex) == 0)
    return NULL;
  
  /*allocate memory based on the size of the array*/
  neighbors = malloc((num_neighbors(graph, vertex)+1)*sizeof(char*));
  /*check if the memory allocation succeeded, and if not, exit*/
  if (! neighbors) {
    printf("Error: Memory allocation of VERTICES failed; exiting\n");
    exit(EXIT_FAILURE);
  }

  /*find the correct vertex first*/
  while (strcmp(h -> name, vertex) != 0) {
    
    h = h -> next;
  }
  e = h -> edge_list;
  

  for (j = 0; j < num_neighbors(graph, vertex); j++) {
    /*allocate enough memory for each name*/
    neighbors[j] = malloc((strlen(e -> dest) + 1) * sizeof(char));
    /*check if the memory allocation succeeded, and if not, exit*/
    if (! neighbors[j]) {
      printf("Error: Memory allocation of VERTEX failed; exiting\n");
      exit(EXIT_FAILURE);
    }
    /*copy the neighbor name over and increment the array*/
    strcpy(neighbors[j], e -> dest);
    e = e -> next;
  }

  /*set the last element to null to signal the end of the array*/
  neighbors[j] = NULL;

  
  return neighbors;
}

/* Frees all of the allocated memory used by the string array parameter.
 */
void free_vertex_name_list(char **vertex_names) {
  int i;
  char *t;
  i = 0;
  /*cannot deallocate a null array*/
  if (vertex_names != NULL) {
    /*iterate though each element of the array and free it, using temporary
      pointers*/
    while (vertex_names[i] != NULL) {
      t = vertex_names[i];
      free(t);
      i++;
     
    }
    /*free the array itself*/
    free(vertex_names);
  }
}

/* Removes the edge going from source to dest (given in the parameters) from
   the graph. It also deallocates the memory that the edge uses.
*/
int remove_edge(Graph *graph, const char source[], const char dest[]) {
  Vertex *h;
  Edge *e = NULL;
  Edge **f = NULL;
  Edge *curr = NULL;
  Edge *prev = NULL;
  Edge *temp = NULL;

  /*invalid parameters or if there is no edge between source and dest*/
  if (graph == NULL || source == NULL || dest == NULL ||
      has_vertex(*graph, source) == 0 || has_vertex(*graph, dest) == 0 ||
      get_edge_cost(*graph, source, dest)== -1)
    return 0;
  
  h = graph -> head;
  /*go through list by name*/
  while (strcmp(h -> name, source) != 0) {
    
    h = h -> next;
  }

  /*get to the appropriate edge list*/
  e = h -> edge_list;
  f = & h -> edge_list;

  /*if the edge to remove is the first on the list*/
  if (strcmp(e -> dest, dest) == 0) {
    temp = *f;
    *f = (*f) -> next;
    /*free both the name of the edge and the edge itself because they are
      both dynamically allocated*/
    free(temp -> dest);
    free(temp);
    return 1;
  }
  curr = (*f) -> next;
  prev = *f;
  /*use current and previous nodes to iterate through the list of edges
    and delete the correct one*/
  while (curr != NULL && prev != NULL) {
    if (strcmp(curr -> dest, dest) == 0) {
      temp = curr;
      prev -> next = curr -> next;
      free(temp -> dest);
      free(temp);
      return 1;
    }
    prev = curr;
    curr = curr -> next;
  }
  
  return 0;
}

/*This function removes the vertex from the graph along with any edges
  that have it as a source or destination. It also deallocates any memory
  that the vertex uses
*/
int remove_vertex(Graph *graph, const char vertex[]) {
  Vertex *h;
  Vertex *curr = NULL;
  Vertex *prev = NULL;
  Vertex *temp = NULL;
  Vertex **g = NULL;
  Edge *f = NULL;
  Edge *t = NULL;
  /*invalid parameters or the graph does not contain the specified vertex*/
  if (graph == NULL || vertex == NULL || has_vertex(*graph, vertex) == 0 )
    return 0;
  
  /*remove all edges associated with the vertex first by iterating through all
    of the vertices and all of the edges*/
  h = graph -> head;
  while (h != NULL){
    f = h -> edge_list;
    while (f != NULL) {
      /*if such an edge exists, remove it*/

      if (strcmp(f -> dest, vertex) == 0 || strcmp(h -> name, vertex) == 0) {
	t = f;
	f = f->next;
	remove_edge(graph, h -> name, t -> dest);
       
        
      }
      else
	f = f -> next;
      
    }
    h = h -> next;
  }

  /*remove the actual vertex now*/
  h = graph -> head;
  g = & graph -> head;

  /*if the vertex to remove is the first on the list*/
  if (strcmp(h -> name, vertex) == 0) {
    temp = *g;
    *g = (*g) -> next;
    /*free the name of the vertex, the edge list of the vertex and the vertex
      itself*/
    free(temp -> name);
    free(temp -> edge_list);
    free(temp);
    /*decrement the number of vertices*/
    graph -> num_vertices--;
    return 1;
  }

  /*use current and previous nodes to iterate through the list of edges and
    delete the correct one*/
  curr = (*g) -> next;
  prev = *g;
  while (curr != NULL && prev != NULL) {
    if (strcmp(curr -> name, vertex) == 0) {
      temp = curr;
      prev -> next = curr -> next;
      free(temp -> name);
      
      free(temp -> edge_list);
      free(temp);
      graph -> num_vertices--;
      return 1;
    }
    prev = curr;
    curr = curr -> next;
  }
  
  return 0;
}

