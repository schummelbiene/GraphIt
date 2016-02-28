#include "stdio.h"
#include "stdlib.h"
#include "main.h"

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
	int n; //number of nodes
	int d; //degree
	int* A; // entries d*(n-1)+1 to 3n return the nodes, which are connected to node n
} d_graph;

typedef struct {
	d_graph dg; //
	float* X; // cartesian coordinates of every node n
} c_graph;


d_graph d_minigraph();
c_graph c_minigraph();
void output(c_graph g );
void graphit(int a, int b);

int main() {
  
	c_graph g;
	g = c_minigraph();
	//	write(*,'I') g%n;
	printf("%i, %i \n", g.dg.n, g.dg.d);
	for (int i = 0; i < g.dg.n; i++){
		printf("%i, %i, %i, ", g.dg.A[g.dg.d*i], g.dg.A[g.dg.d*i+1], g.dg.A[g.dg.d*i+2]);
		printf("%f, %f, \n", g.X[2*i], g.X[2*i+1]); 
	}
	
	output(g);
	return 1;
}

// tetrahedron :)

d_graph d_minigraph(){ 
	d_graph g;
	g.n = 4;
	g.d = 3;
	// d_graph* h = &g; h->A is the same as (*g).A, h[i] is the same as *(h + i)
	g.A = malloc(g.d*g.n*sizeof(int)); // allocate(A[d*n]), if A(:) :: int
	for (int i = 0; i < g.n; i++) {	
		g.A[g.d*i]   = (i+1)%(g.n);
		g.A[g.d*i+1] = (i+2)%(g.n);
		g.A[g.d*i+2] = (i+3)%(g.n);
		
	}
	return g;
}

c_graph c_minigraph(){
	c_graph tetrahedron;
	tetrahedron.dg = d_minigraph();
	tetrahedron.X = malloc(tetrahedron.dg.n*sizeof(float)*2);
	for (int i = 0; i < tetrahedron.dg.n; i++) {	
		tetrahedron.X[i*2]   = ((float)i)/((float)tetrahedron.dg.n );
		tetrahedron.X[i*2+1] = (float)(i+1)/(float)(tetrahedron.dg.n*i+1);
	}
	return tetrahedron;
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
					"\\begin{tikzpicture}\n"
					);
	// print tikz-code for nodes
	for (int i = 0; i<g.dg.n; i++){
		fprintf(f, "\\coordinate (x%i) at (%f, %f);\n", i, g.X[i*2], g.X[i*2+1]);
	}
	// print tikz-code for connection lines
	for (int i = 0; i<g.dg.n; i++){
		for (int j = 0; j < g.dg.d; j++) {
		fprintf(f,"\\draw (x%i) -- (x%i);\n", i, g.dg.A[g.dg.d*i + j]);
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
	system("pdflatex output_graph.tex");
}
