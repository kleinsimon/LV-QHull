#pragma once

#include "libqhull_r/qhull_ra.h"


#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)


int get_hull(coordT* points, int dim, int numpoints, int** simplices_ptr, int* numsimplices, double* volume, double* area) {
	#pragma EXPORT

	char flags[250];
	int exitcode = 0;
	vertexT* vertex;
	int curlong, totlong;
	boolT ismalloc = False;
	FILE* outfile = NULL;
	FILE* errfile = NULL;

	qhT qh_qh;
	qhT* qh = &qh_qh;

	sprintf(flags, "qhull FA");

	QHULL_LIB_CHECK

	qh_zero(qh, NULL);

	exitcode = qh_new_qhull(qh, dim, numpoints, points, ismalloc, flags, outfile, errfile);

	if (!exitcode) {

		size_t memsize = qh->num_vertices * sizeof(int);
		int* simplices = (int*) malloc(memsize);
		if (simplices == NULL)
			return 1;

		*simplices_ptr = simplices;

		int i = 0;
		FORALLvertices{
			simplices[i] = qh_pointid(qh, vertex->point);
			i++;
		}

		*numsimplices = i;
		*volume = qh->totvol;
		*area = qh->totarea;
	}

	qh_freeqhull(qh, !qh_ALL);
	qh_memfreeshort(qh, &curlong, &totlong);

	return exitcode;
}

int get_delaunay(coordT* points, int dim, int numpoints, int** simplices_ptr, int* numsimplices, coordT** v_centers_ptr, boolT calcVoronoi) {
	#pragma EXPORT

	boolT		ismalloc = False;
	facetT		*facet;
	vertexT		*vertex, **vertexp;
	int			curlong, totlong, exitcode;
	char		options[200];
	qhT			qh_qh;
	qhT			*qh = &qh_qh;

	QHULL_LIB_CHECK

	if (numpoints <= 0 || dim <= 0)
		return 0;

	qh_zero(qh, NULL);
	qh_init_A(qh, NULL, NULL, NULL, 0, NULL);
	exitcode = setjmp(qh->errexit);

	if (!exitcode) {
		qh->NOerrexit = False;
		sprintf(options, "qhull d");
		qh_initflags(qh, options);

		qh->PROJECTdelaunay = True;
		
		qh_init_B(qh, points, numpoints, dim, ismalloc);
		
		qh_qhull(qh);
		
		if (calcVoronoi) {
			coordT* center;
			size_t vmemsize = qh->num_facets * qh->input_dim * sizeof(coordT);
			coordT* vcenters = (coordT*) malloc(vmemsize);
			if (vcenters == NULL)
				return 1;
			*v_centers_ptr = vcenters;

			int i_facet = 0;

			qh_vertexneighbors(qh);

			FORALLfacets{
				if (facet->good && !facet->upperdelaunay) {
					center = qh_facetcenter(qh, facet->vertices);
					for (int i_dim = 0; i_dim < qh->input_dim; i_dim++) {
						vcenters[i_facet + i_dim] = center[i_dim];
					}
					i_facet++;
				}
			}
		}

		qh_check_output(qh);
		
		if (qh->VERIFYoutput && !qh->FORCEoutput && !qh->STOPadd && !qh->STOPcone && !qh->STOPpoint)
			qh_check_points(qh);
		
		int facet_i = 0;
		int numfacets = 0;

		size_t memsize = qh->num_facets * qh->hull_dim * sizeof(int);
		int *simplices = (int*) malloc(memsize);
		if (simplices == NULL)
			return 1;

		*simplices_ptr = simplices;
		
		FORALLfacets{
			if (facet->good && !facet->upperdelaunay) {
				FOREACHvertex_(facet->vertices){
					simplices[facet_i] = qh_pointid(qh, vertex->point);
					facet_i++;
				}
				numfacets++;
			}
		}
		*numsimplices = numfacets;
	}

	qh->NOerrexit = True;
	qh_freeqhull(qh, !qh_ALL);
	qh_memfreeshort(qh, &curlong, &totlong);
	
	return exitcode;
}

void free_ptr(size_t* simplices_ptr) {
	#pragma EXPORT

	free(simplices_ptr);
}