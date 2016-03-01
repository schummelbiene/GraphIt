#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "string.h"


//Typen:
// ganzzahlig:
/* int i = -5; */
/* unsigned int ui = 5; */
/* long l = -123523952244; */
/* unsigned long ul = 0x32AB; //hexadezimal */
/* // gebrochenzahlig */
/* float f = 0.5f; */
/* double d = 0.424345345d; */
//mehr in stdint.h, z.B. uint8_t (unsigned 8bit-Typ)

typedef struct {
	int number_of_nodes; //number of nodes
	int deg; //degree
	int* A; // entries d*(n-1)+1 to 3n return the nodes, which are connected to node n
} d_graph;

typedef struct {
	d_graph d_graph; //
  float* X; // cartesian coordinates of every node n
  float* Y; // cartesian coordinates of every node n
} c_graph;


float pi = 3.1415; 
float cool(int i, int n);
//float abs(float x);
float min(float x, float y);
d_graph d_minigraph();
c_graph c_minigraph();
void output(c_graph g );
void graphit(int a, int b);
void move(c_graph g, int n);
float dist(float x1, float x2, float y1, float y2);

int main() {
  
	c_graph g;
	g = c_minigraph();
	//	write(*,'I') g%n;
	printf("%i, %i \n", g.d_graph.number_of_nodes, g.d_graph.deg);
	for (int i = 0; i < g.d_graph.number_of_nodes; i++){
		int degree = g.d_graph.deg;
		printf("%i, %i, %i, ", g.d_graph.A[degree*i], g.d_graph.A[degree*i+1], g.d_graph.A[degree*i+2]);
		printf("%f, %f, \n", g.X[i], g.Y[i]); 
	}

	for (int i = 1; i<2000; i++){; 
				move(g, i);
	}
	output(g);
	return 1;
}

// tetrahedron :)

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
	g.A[15] = 2;
	g.A[16] = 3;
	g.A[17] = 5;
	return g;
}

c_graph c_minigraph(){
	c_graph tetrahedron;
	tetrahedron.d_graph = d_minigraph();
	tetrahedron.X = malloc(tetrahedron.d_graph.number_of_nodes*sizeof(float));
	tetrahedron.Y = malloc(tetrahedron.d_graph.number_of_nodes*sizeof(float));

	/* for (int i = 0; i < tetrahedron.d_graph.number_of_nodes; i++) {	 */
	/* 	tetrahedron.X[i] = ((float)i)/((float)tetrahedron.d_graph.number_of_nodes ); */
	/* 	tetrahedron.Y[i] = (float)(i+1)/(float)(tetrahedron.d_graph.number_of_nodes*i+1); */
	/* } */
	
	tetrahedron.X[0] = -0.5;
	tetrahedron.Y[0] = -0.866;
	tetrahedron.X[1] = 1;
	tetrahedron.Y[1] = 0;
	tetrahedron.X[2] = -0.5;
	tetrahedron.Y[2] = 0.866;

	float eps;
	eps = 0.1;
	
	tetrahedron.X[3] = -0.5*eps;
	tetrahedron.Y[3] = -0.866*eps;
	tetrahedron.X[4] =  2*eps;
	tetrahedron.Y[4] = 0*eps;
	tetrahedron.X[5] = -0.5*eps;
	tetrahedron.Y[5] = 0.866*eps;
	
	return tetrahedron;
}

float min(float x, float y){
	if (x >= y) {
		return y;
	} else {
		return x;
	}
}

float dist(float x1, float x2, float y1, float y2){
	float d;
	d = powf((x1 - y1)*(x1 - y1) + (x2 - y2)*(x2 - y2),0.5);
	return d;
}

float cool(int i, int n){
	float result;
	result = powf(n/pi, 0.5)/(1 + pi/n*powf(i, 1.5));
	return result;
}

// n: iteration cycle
void move(c_graph g, int n){
	int degree = g.d_graph.deg;
	int node_length = g.d_graph.number_of_nodes;
	float constant;
	constant = node_length/pi;
	
	float* force_x;  // force_x[i] returns the force in x-direction for node i, see algorithm.pdf
	float* force_y;  // force_y[i] returns the force in y-direction for node i, see algorithm.pdf
	
	force_x = malloc(node_length*sizeof(float));
	force_y = malloc(node_length*sizeof(float));
	
	// Step I:
	memset(force_x, 0, node_length * sizeof(float));
	memset(force_y, 0, node_length * sizeof(float));

	// Step II:
	
	for (int i = 0; i < node_length; i++){
		
		for (int j = 0; j < degree; j++){
			

			printf("%i, %i, %f , %f, %f, loop \n", i, j, constant, force_x[i], force_y[i]);
			
			force_x[i] = force_x[i] - constant *
				(g.X[i]- g.X[g.d_graph.A[3*i+j]]) *
				(g.X[i]- g.X[g.d_graph.A[3*i+j]]) *
				(g.X[i]- g.X[g.d_graph.A[3*i+j]]);

			force_y[i] = force_y[i] - constant *
				(g.Y[i]- g.Y[g.d_graph.A[3*i+j]]) *
				(g.Y[i]- g.Y[g.d_graph.A[3*i+j]]) *
				(g.Y[i]- g.Y[g.d_graph.A[3*i+j]]);

		}
	}

	// Step III:

	
	
	for (int i = 3; i < node_length; i++){
		
		printf("%f,  %f  before\n", g.X[i],g.Y[i]);

		float norm = dist(0, 0, force_x[i], force_y[i]);
		float c    = cool(n, node_length);
		
		g.X[i] = g.X[i] + 0.1*min(norm, c) * force_x[i]/norm; 
		g.Y[i] = g.Y[i] + 0.1*min(norm, c) * force_y[i]/norm; 

		printf("%f,  %f after \n", g.X[i],g.Y[i]);
	}

	return;
}

void graphit(int a, int b) {
  printf("Huhu: %i, %i", a, a + b);

  return;
}

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
