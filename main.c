#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "string.h"


typedef struct {
	int number_of_nodes; //number of nodes
	int deg; //degree
	int* A; // entries d*(n-1)+1 to 3n return the nodes, which are connected to node n
} d_graph;

typedef struct {
	d_graph d_graph; //
  double* X; // cartesian coordinates of every node n
  double* Y; // cartesian coordinates of every node n
} c_graph;


double pi = 3.1415; 
double cool(int i, int n);
double min(double x, double y);
d_graph d_minigraph();
c_graph set_nodes(d_graph g, int k);
c_graph c_minigraph();
void output(c_graph g );
void graphit(int a, int b);
void move(c_graph g, int n);
double dist(double x1, double x2, double y1, double y2);

int main() {
  
	c_graph g;
	g = c_minigraph();

	for (int i = 1; i<100; i++){; 
						move(g, i);
	}
	output(g);
	return 1;
}

// single move routine ---------------------------------------------------------------

void move(c_graph g, int n){
	int degree = g.d_graph.deg;
	int node_length = g.d_graph.number_of_nodes;
	double constant;
	constant = pow(node_length/pi, 0.5);
	
	double* force_x;  // force_x[i] returns the force in x-direction for node i, see algorithm.pdf
	double* force_y;  // force_y[i] returns the force in y-direction for node i, see algorithm.pdf
	
	force_x = malloc(node_length*sizeof(double));
	force_y = malloc(node_length*sizeof(double));
	
	// Step I:
	memset(force_x, 0, node_length * sizeof(double));
	memset(force_y, 0, node_length * sizeof(double)); 

	// Step II:
	for (int i = 0; i < node_length; i++){
		for (int j = 0; j < degree; j++){
			
			double distance = dist(g.X[i], g.Y[i], g.X[g.d_graph.A[degree*i+j]], g.Y[g.d_graph.A[degree*i+j]]);
			printf("%i, %i, %f , %f, %f, loop, distance: %f \n", i, j, constant, force_x[i], force_y[i], distance);
			force_x[i] -= constant * (g.X[i]- g.X[g.d_graph.A[degree*i+j]]) * distance * distance;
			force_y[i] -= constant * (g.Y[i]- g.Y[g.d_graph.A[degree*i+j]]) * distance * distance;
		}
	}

	// Step III:

	double c = cool(n, node_length);
	
	for (int i = 3; i < node_length; i++){

		double norm = dist(0, 0, force_x[i], force_y[i]);
		
		g.X[i] +=  min(norm, c) * force_x[i]/norm; 
		g.Y[i] +=  min(norm, c) * force_y[i]/norm; 
		printf("Iteration %i \n", i);
	}

	return;
}

// initializing ---------------------------------------------------------
// k: number of nodes on the unit circle
c_graph set_nodes(d_graph g, int k){
	c_graph h;
	h.X = malloc(g.number_of_nodes*sizeof(double));
	h.Y = malloc(g.number_of_nodes*sizeof(double));	

	for (int i = 0; i < k; i++ ){
		h.X[i] = cos(2*pi*i/k);
		h.Y[i] = sin(2*pi*i/k);
	}
	for (int i = k; i < g.number_of_nodes; i++){
		h.X[i] = 0;
		h.Y[i] = 0;

	}
	h.d_graph = g;
	return h;
}

// utilities ------------------------------------------------------------

double min(double x, double y){
	if (x >= y) {
		return y;
	} else {
		return x;
	}
}

double dist(double x1, double x2, double y1, double y2){
	double d;
	d = pow((x1 - y1)*(x1 - y1) + (x2 - y2)*(x2 - y2),0.5);
	return d;
}

double cool(int i, int n){
	double result;
	result = pow(pi/n, 0.5)/(1 + pi/n*pow(i, 1.5));
	return result;
}

// Example ---------------------------------------------------------------------------

d_graph d_minigraph(){ 
	d_graph g;
	g.number_of_nodes = 6;
	g.deg = 3;
	// d_graph* h = &g; h->A is the same as (*g).A, h[i] is the same as *(h + i)
	g.A = malloc(g.deg*g.number_of_nodes*sizeof(int)); // allocate(A[d*n]), if A(:) :: int
	for (int i = 0; i < g.number_of_nodes; i++) {	
		g.A[g.deg*i+0] = (i+1)%(g.number_of_nodes);
		g.A[g.deg*i+1] = (i+2)%(g.number_of_nodes);
		g.A[g.deg*i+2] = (i+3)%(g.number_of_nodes);		
	}
	g.A[0] = 1;
	g.A[1] = 2;
	g.A[2] = 3;
	g.A[3] = 0;
	g.A[4] = 2;
	g.A[5] = 4;
	g.A[6] = 0;
	g.A[7] = 1;
	g.A[8] = 5;
	g.A[9] = 0;
	g.A[10] = 4;
	g.A[11] = 5;
	g.A[12] = 1;
	g.A[13] = 3;
	g.A[14] = 5;
	g.A[15] = 3;
	g.A[16] = 4;
	g.A[17] = 2;
	return g;
}

c_graph c_minigraph(){
	c_graph tetrahedron;
	tetrahedron.d_graph = d_minigraph();
	tetrahedron.X = malloc(tetrahedron.d_graph.number_of_nodes*sizeof(double));
	tetrahedron.Y = malloc(tetrahedron.d_graph.number_of_nodes*sizeof(double));

	/* for (int i = 0; i < tetrahedron.d_graph.number_of_nodes; i++) {	 */
	/* 	tetrahedron.X[i] = ((double)i)/((double)tetrahedron.d_graph.number_of_nodes ); */
	/* 	tetrahedron.Y[i] = (double)(i+1)/(double)(tetrahedron.d_graph.number_of_nodes*i+1); */
	/* } */
	
	tetrahedron.X[0] = -0.5;
	tetrahedron.Y[0] = -0.5*sqrt(3.0);
	tetrahedron.X[1] = 1;
	tetrahedron.Y[1] = 0;
	tetrahedron.X[2] = -0.5;
	tetrahedron.Y[2] = 0.5*sqrt(3.0);

	double eps;
	eps = -0.1;
	
	tetrahedron.X[3] = -0.5*eps;
	tetrahedron.Y[3] = -0.5*sqrt(3.0)*eps;
	tetrahedron.X[4] = 1*eps;
	tetrahedron.Y[4] = 0*eps;
	tetrahedron.X[5] = -0.5*eps;
	tetrahedron.Y[5] = 0.5*sqrt(3.0)*eps;

	//tetrahedron = set_nodes(tetrahedron.d_graph, 3);
	return tetrahedron;
}


// Output routine ------------------------------------------------------------------

void output(c_graph g ){
	FILE *f = fopen("output_graph.tex", "w");
	if (f == NULL)
		{
			printf("Error opening file output_graph!\n");
			exit(1);
		}
	// print latex-code 
	fprintf(f,
					"\\documentclass[a4paper]{article}\n"
					"\\usepackage{tikz}\n"
					"\\begin{document}\n"
					"\\begin{figure}[h!]\n"
					"\\centering\n"
					"\\begin{tikzpicture}[scale = 4]\n"
					);
	// print tikz-code for nodes
	for (int i = 0; i<g.d_graph.number_of_nodes; i++){
		fprintf(f, "\\coordinate (x%i) at (%f, %f);\n", i, g.X[i], g.Y[i]);
	}
	// print tikz-code for connection lines
	for (int i = 0; i<g.d_graph.number_of_nodes; i++){
		for (int j = 0; j < g.d_graph.deg; j++) {
		fprintf(f,"\\draw (x%i) -- (x%i);\n", i, g.d_graph.A[g.d_graph.deg*i + j]);
		}		
	}
	// print latex-code 
	fprintf(f, "\\end{tikzpicture}\n"
					"\\caption{Graph}\n"
					"\n"
					"\\end{figure}\n"
					"\\end{document}\n"
					);

	fclose(f);
	// create pdf
	//system("pdflatex output_graph.tex");
}
