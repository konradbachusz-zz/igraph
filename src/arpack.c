/* -*- mode: C -*-  */
/* 
   IGraph library.
   Copyright (C) 2007  Gabor Csardi <csardi@rmki.kfki.hu>
   MTA RMKI, Konkoly-Thege Miklos st. 29-33, Budapest 1121, Hungary
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
   02110-1301 USA

*/

#include "arpack.h"
#include "arpack_internal.h"
#include "memory.h"
#include "igraph.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* The ARPACK example file dssimp.f is used as a template */

int igraph_i_arpack_err_dsaupd(long int error) {
  switch (error) {
  case -1:      return IGRAPH_ARPACK_NPOS;
  case -2:      return IGRAPH_ARPACK_NEVNPOS;
  case -3:      return IGRAPH_ARPACK_NCVSMALL;
  case -4:      return IGRAPH_ARPACK_NONPOSI;
  case -5:      return IGRAPH_ARPACK_WHICHINV;
  case -6:      return IGRAPH_ARPACK_BMATINV;
  case -7:      return IGRAPH_ARPACK_WORKLSMALL;
  case -8:      return IGRAPH_ARPACK_TRIDERR;
  case -9:      return IGRAPH_ARPACK_ZEROSTART;
  case -10:     return IGRAPH_ARPACK_MODEINV;
  case -11:     return IGRAPH_ARPACK_MODEBMAT;
  case -12:     return IGRAPH_ARPACK_ISHIFT;
  case -13:     return IGRAPH_ARPACK_NEVBE;
  case -9999:   return IGRAPH_ARPACK_NOFACT;
  default:      return IGRAPH_ARPACK_UNKNOWN;
  }
}

int igraph_i_arpack_err_dseupd(long int error) {
  switch (error) {
  case -1:      return IGRAPH_ARPACK_NPOS;
  case -2:      return IGRAPH_ARPACK_NEVNPOS;
  case -3:      return IGRAPH_ARPACK_NCVSMALL;
  case -5:      return IGRAPH_ARPACK_WHICHINV;
  case -6:      return IGRAPH_ARPACK_BMATINV;
  case -7:      return IGRAPH_ARPACK_WORKLSMALL;
  case -8:      return IGRAPH_ARPACK_TRIDERR;
  case -9:      return IGRAPH_ARPACK_ZEROSTART;
  case -10:     return IGRAPH_ARPACK_MODEINV;
  case -11:     return IGRAPH_ARPACK_MODEBMAT;
  case -12:     return IGRAPH_ARPACK_NEVBE;
  case -14:     return IGRAPH_ARPACK_FAILED;
  case -15:     return IGRAPH_ARPACK_HOWMNY;
  case -16:     return IGRAPH_ARPACK_HOWMNYS;
  case -17:     return IGRAPH_ARPACK_EVDIFF;
  default:      return IGRAPH_ARPACK_UNKNOWN;
  }
  
}

int igraph_i_arpack_err_dnaupd(long int error) {
  switch (error) {
  case -1:      return IGRAPH_ARPACK_NPOS;
  case -2:      return IGRAPH_ARPACK_NEVNPOS;
  case -3:      return IGRAPH_ARPACK_NCVSMALL;
  case -4:      return IGRAPH_ARPACK_NONPOSI;
  case -5:      return IGRAPH_ARPACK_WHICHINV;
  case -6:      return IGRAPH_ARPACK_BMATINV;
  case -7:      return IGRAPH_ARPACK_WORKLSMALL;
  case -8:      return IGRAPH_ARPACK_TRIDERR;
  case -9:      return IGRAPH_ARPACK_ZEROSTART;
  case -10:     return IGRAPH_ARPACK_MODEINV;
  case -11:     return IGRAPH_ARPACK_MODEBMAT;
  case -12:     return IGRAPH_ARPACK_ISHIFT;
  case -9999:   return IGRAPH_ARPACK_NOFACT;
  default:      return IGRAPH_ARPACK_UNKNOWN;
  }  
}

int igraph_i_arpack_err_dneupd(long int error) {
  switch (error) {
  case -1:      return IGRAPH_ARPACK_NPOS;
  case -2:      return IGRAPH_ARPACK_NEVNPOS;
  case -3:      return IGRAPH_ARPACK_NCVSMALL;
  case -5:      return IGRAPH_ARPACK_WHICHINV;
  case -6:      return IGRAPH_ARPACK_BMATINV;
  case -7:      return IGRAPH_ARPACK_WORKLSMALL;
  case -8:      return IGRAPH_ARPACK_SHUR;
  case -9:      return IGRAPH_ARPACK_LAPACK;
  case -10:     return IGRAPH_ARPACK_MODEINV;
  case -11:     return IGRAPH_ARPACK_MODEBMAT;
  case -12:     return IGRAPH_ARPACK_HOWMNYS;
  case -13:     return IGRAPH_ARPACK_HOWMNY;
  case -14:     return IGRAPH_ARPACK_FAILED;
  case -15:     return IGRAPH_ARPACK_EVDIFF;
  default:      return IGRAPH_ARPACK_UNKNOWN;
  }    
}

/**
 * \function igraph_arpack_options_init
 * Initialize ARPACK options
 * 
 * Initializes ARPACK options, set them to default values. 
 * You can always pass the initialized \ref igraph_arpack_options_t 
 * object to built-in igraph functions without any modification. The
 * built-in igraph functions modify the options to perform their
 * calculation, e.g. \ref igraph_pagerank() always searches for the
 * eigenvalue with the largest magnitude, regardless of the supplied
 * value.
 * </para><para>
 * If you want to implement your own function involving eigenvalue
 * calculation using ARPACK, however, you will likely need to set up
 * the fields for yourself.
 * \param o The \ref igraph_arpack_options_t object to initialize.
 * 
 * Time complexity: O(1).
 */

void igraph_arpack_options_init(igraph_arpack_options_t *o) {
  o->bmat[0]='I';
  o->n=0;			/* needs to be updated! */
  o->which[0]='X'; o->which[1]='X';
  o->nev=1;
  o->tol=0;
  o->ncv=3;
  o->ldv=o->n;			/* will be updated to (real) n */
  o->ishift=1;
  o->mxiter=3000;
  o->nb=1;
  o->mode=1;
  o->start=0;
  o->lworkl=0;
  o->sigma=0;
  o->sigmai=0;
  o->info=o->start;
  
  o->iparam[0]=o->ishift; o->iparam[1]=0; o->iparam[2]=o->mxiter; o->iparam[3]=o->nb;
  o->iparam[4]=0; o->iparam[5]=0; o->iparam[6]=o->mode; o->iparam[7]=0;
  o->iparam[8]=0; o->iparam[9]=0; o->iparam[10]=0;
}

/**
 * \function igraph_arpack_storage_init
 * Initialize ARPACK storage
 * 
 * You only need this function if you want to run multiple eigenvalue
 * calculations using ARPACK, and want to spare the memory
 * allocation/deallocation between each two runs. Otherwise it is safe 
 * to supply a null pointer as the \c storage argument of both \ref
 * igraph_arpack_rssolve() and \ref igraph_arpack_rnsolve() to make
 * memory allocated and deallocated automatically.
 * 
 * </para><para>Don't forget to call the \ref
 * igraph_arpack_storage_destroy() function on the storage object if
 * you don't need it any more.
 * \param s The \ref igraph_arpack_storage_t object to initialize.
 * \param maxn The maximum order of the matrices.
 * \param maxncv The maximum NCV parameter intended to use.
 * \param maxldv The maximum LDV parameter intended to use.
 * \param symm Whether symmetric or non-symmetric problems will be
 *    solved using this \ref igraph_arpack_storage_t. (You cannot use
 *    the same storage both with symmetric and non-symmetric solvers.)
 * \return Error code.
 * 
 * Time complexity: O(maxncv*(maxldv+maxn)).
 */

int igraph_arpack_storage_init(igraph_arpack_storage_t *s, long int maxn,
			       long int maxncv, long int maxldv, 
			       igraph_bool_t symm) {
  
  /* TODO: check arguments */
  s->maxn=maxn; 
  s->maxncv=maxncv;
  s->maxldv=maxldv;

#define CHECKMEM(x) \
    if (!x) { \
      IGRAPH_ERROR("Cannot allocate memory for ARPACK", IGRAPH_ENOMEM); \
    } \
    IGRAPH_FINALLY(igraph_free, x);

  s->v=igraph_Calloc(maxldv * maxncv, igraph_real_t); CHECKMEM(s->v);
  s->workd=igraph_Calloc(3*maxn, igraph_real_t); CHECKMEM(s->workd);
  s->d=igraph_Calloc(2*maxncv, igraph_real_t); CHECKMEM(s->d);
  s->resid=igraph_Calloc(maxn, igraph_real_t); CHECKMEM(s->resid);
  s->ax=igraph_Calloc(maxn, igraph_real_t); CHECKMEM(s->ax);
  s->select=igraph_Calloc(maxncv, long int); CHECKMEM(s->select);

  if (symm) {
    s->workl=igraph_Calloc(maxncv*(maxncv+8), igraph_real_t); CHECKMEM(s->workl);
    s->di=0;
    s->workev=0;
  } else {
    s->workl=igraph_Calloc(3*maxncv*(maxncv+2), igraph_real_t); CHECKMEM(s->workl);
    s->di=igraph_Calloc(2*maxncv, igraph_real_t); CHECKMEM(s->di);
    s->workev=igraph_Calloc(3*maxncv, igraph_real_t); CHECKMEM(s->workev);
    IGRAPH_FINALLY_CLEAN(2);
  }

#undef CHECKMEM  
  
  IGRAPH_FINALLY_CLEAN(7);
  return 0;
}

/**
 * \function igraph_arpack_storage_destroy
 * Deallocate ARPACK storage
 * 
 * \param s The \ref igraph_arpack_storage_t object for which the
 *    memory will be deallocated.
 * 
 * Time complexity: operating system dependent.
 */

void igraph_arpack_storage_destroy(igraph_arpack_storage_t *s) {
  
  if (s->di) { 
    igraph_Free(s->di);
  }
  if (s->workev) {
    igraph_Free(s->workev);
  }
  
  igraph_Free(s->workl);
  igraph_Free(s->select);
  igraph_Free(s->ax);
  igraph_Free(s->resid);
  igraph_Free(s->d);
  igraph_Free(s->workd);
  igraph_Free(s->v);
}

/**
 * \function igraph_arpack_rssolve
 * \brief ARPACK solver for symmetric matrices
 * 
 * This is the ARPACK solver for symmetric matrices. Please use 
 * \ref igraph_arpack_rnsolve() for non-symmetric matrices.
 * \param fun Pointer to an \ref igraph_arpack_function_t object, 
 *     the function that performs the matrix-vector multiplication.
 * \param extra An extra argument to be passed to \c fun.
 * \param options An \ref igraph_arpack_options_t object.
 * \param storage An \ref igraph_arpack_storage_t object, or a null
 *     pointer. In the latter case memory allocation and deallocation
 *     is performed automatically.
 * \param values If not a null pointer, then it should be a pointer to an
 *     initialized vector. The eigenvalues will be stored here. The
 *     vector will be resized as needed.
 * \param vectors If not a null pointer, then it must be a pointer to
 *     an initialized matrix. The eigenvectors will be stored in the
 *     columns of the matrix. The matrix will be resized as needed.
 * \return Error code.
 * 
 * Time complexity: depends on the matrix-vector
 * multiplication. Usually a small number of iterations is enough, so
 * if the matrix is sparse and the matrix-vector multiplication can be
 * done in O(n) time (the number of vertices), then the eigenvalues
 * are found in O(n) time as well.
 */

int igraph_arpack_rssolve(igraph_arpack_function_t *fun, void *extra,
			  igraph_arpack_options_t *options, 
			  igraph_arpack_storage_t *storage,
			  igraph_vector_t *values, igraph_matrix_t *vectors) {
  
  igraph_real_t *v, *workl, *workd, *d, *resid, *ax;
  igraph_bool_t free_them=0;
  long int *select, i;

  long int ido=0;
  long int rvec= vectors || storage ? 1 : 0;	/* calculate eigenvectors? */
  char *all="All";

  long int origldv=options->ldv, origlworkl=options->lworkl;
  char origwhich[2]={ options->which[0], options->which[1] };
  igraph_real_t origtol=options->tol;
  
  /* Brush up options if needed */
  if (options->ldv == 0) { options->ldv=options->n; }
  if (options->lworkl == 0) { options->lworkl=options->ncv*(options->ncv+8); }
  if (options->which[0] == 'X') { options->which[0]='L'; options->which[1]='M'; }
  
  if (storage) {
    /* Storage provided */
    if (storage->maxn < options->n) {
      IGRAPH_ERROR("Not enough storage for ARPACK (`n')", IGRAPH_EINVAL);
    }
    if (storage->maxncv < options->ncv) {
      IGRAPH_ERROR("Not enough storage for ARPACK (`ncv')", IGRAPH_EINVAL);
    }
    if (storage->maxldv < options->ldv) {
      IGRAPH_ERROR("Not enough storage for ARPACK (`ldv')", IGRAPH_EINVAL);
    }

    v      = storage->v; 
    workl  = storage->workl;
    workd  = storage->workd;
    d      = storage->d;
    resid  = storage->resid;
    ax     = storage->ax;
    select = storage->select;
    
  } else {
    /* Storage not provided */
    free_them=1;
    
#define CHECKMEM(x) \
    if (!x) { \
      IGRAPH_ERROR("Cannot allocate memory for ARPACK", IGRAPH_ENOMEM); \
    } \
    IGRAPH_FINALLY(igraph_free, x);

    v=igraph_Calloc(options->ldv * options->ncv, igraph_real_t); CHECKMEM(v);
    workl=igraph_Calloc(options->lworkl, igraph_real_t); CHECKMEM(workl);
    workd=igraph_Calloc(3*options->n, igraph_real_t); CHECKMEM(workd);
    d=igraph_Calloc(2*options->ncv, igraph_real_t); CHECKMEM(d);
    resid=igraph_Calloc(options->n, igraph_real_t); CHECKMEM(resid);
    ax=igraph_Calloc(options->n, igraph_real_t); CHECKMEM(ax);
    select=igraph_Calloc(options->ncv, long int); CHECKMEM(select);

#undef CHECKMEM

  }

  /* Set final bits */
  options->iparam[0]=options->ishift;
  options->iparam[2]=options->mxiter;
  options->iparam[3]=options->nb;
  options->iparam[4]=0;
  options->iparam[6]=options->mode;
  options->info=options->start;
  if (options->start) { 
    for (i=0; i<options->n; i++) {
      resid[i]=MATRIX(*vectors, i, 0);
    }
  }

  /* Ok, we have everything */
  while (1) {
    igrapharpackdsaupd_(&ido, options->bmat, &options->n, options->which,
		  &options->nev, &options->tol, 
		  resid, &options->ncv, v, &options->ldv,
		  options->iparam, options->ipntr,
		  workd, workl, &options->lworkl, &options->info);

    if (ido==-1 || ido==1) {

      igraph_real_t *from=workd+options->ipntr[0]-1;
      igraph_real_t *to=workd+options->ipntr[1]-1;
      if (fun(to, from, options->n, extra) != 0) {
	IGRAPH_ERROR("Arpack error while evaluating matrix-vector product",
		     IGRAPH_ARPACK_PROD);
      }
      
    } else {
      break;
    }
  }
  
  if (options->info < 0) {
    IGRAPH_ERROR("ARPACK error", igraph_i_arpack_err_dsaupd(options->info));
  }
  
  igrapharpackdseupd_(&rvec, all, select, d, v, &options->ldv,
		&options->sigma, options->bmat, &options->n,
		options->which, &options->nev, &options->tol,
		resid, &options->ncv, v, &options->ldv, options->iparam,
		options->ipntr, workd, workl, &options->lworkl,
		&options->ierr);
  
  if (options->ierr < 0) {
    IGRAPH_ERROR("ARPACK error", igraph_i_arpack_err_dseupd(options->ierr));
  }    
  
  /* Save the result */
  
  options->noiter=options->iparam[2];
  options->nconv=options->iparam[4];
  options->numop=options->iparam[8];
  options->numopb=options->iparam[9];
  options->numreo=options->iparam[10];

  if (values) {
    long int i;
    IGRAPH_CHECK(igraph_vector_resize(values, options->nev));
    for (i=0; i<options->nev; i++) {
      VECTOR(*values)[i] = d[i];
    }
  }

  if (vectors) {
    long int i, j, ptr=0;
    IGRAPH_CHECK(igraph_matrix_resize(vectors, options->n, options->nev));
    for (j=0; j<options->nev; j++) {	
      for (i=0; i<options->n; i++) {
	MATRIX(*vectors, i, j) = v[ptr++];
      }
    }
  }

  options->ldv=origldv;
  options->lworkl=origlworkl;
  options->which[0] = origwhich[0]; options->which[1] = origwhich[1];
  options->tol=origtol;
    
  /* Clean up if needed */
  if (free_them) {
    igraph_Free(select);
    igraph_Free(ax);
    igraph_Free(resid);
    igraph_Free(d);
    igraph_Free(workd);
    igraph_Free(workl);
    igraph_Free(v);
    IGRAPH_FINALLY_CLEAN(7);
  }
  return 0;
}

/**
 * \function igraph_arpack_rnsolve
 * \brief ARPACK solver for non-symmetric matrices
 * 
 * Please always consider calling \ref igraph_arpack_rssolve() if your
 * matrix is symmetric, it is much faster.
 * \ref igraph_arpack_rnsolve() for non-symmetric matrices.
 * \param fun Pointer to an \ref igraph_arpack_function_t object, 
 *     the function that performs the matrix-vector multiplication.
 * \param extra An extra argument to be passed to \c fun.
 * \param options An \ref igraph_arpack_options_t object.
 * \param storage An \ref igraph_arpack_storage_t object, or a null
 *     pointer. In the latter case memory allocation and deallocation
 *     is performed automatically.
 * \param values If not a null pointer, then it should be a pointer to an
 *     initialized matrix. The (possibly complex) eigenvalues will be
 *     stored here. The matrix will have two columns, the first column
 *     contains the real, the second the imaginary parts of the
 *     eigenvalues.
 *     The matrix will be resized as needed.
 * \param vectors If not a null pointer, then it must be a pointer to
 *     an initialized matrix. The eigenvectors will be stored in the
 *     columns of the matrix. The matrix will be resized as needed.
 * \return Error code.
 * 
 * Time complexity: depends on the matrix-vector
 * multiplication. Usually a small number of iterations is enough, so
 * if the matrix is sparse and the matrix-vector multiplication can be
 * done in O(n) time (the number of vertices), then the eigenvalues
 * are found in O(n) time as well.
 */

int igraph_arpack_rnsolve(igraph_arpack_function_t *fun, void *extra,
			  igraph_arpack_options_t *options,
			  igraph_arpack_storage_t *storage,
			  igraph_matrix_t *values, igraph_matrix_t *vectors) {

  igraph_real_t *v, *workl, *workd, *dr, *di, *resid, *workev;
  igraph_bool_t free_them=0;
  long int *select, i;
  
  long int ido=0;
  long int rvec= vectors || storage ? 1 : 0;
  char *all="All";
  
  long int origldv=options->ldv, origlworkl=options->lworkl, 
    orignev=options->nev;
  char origwhich[2]={ options->which[0], options->which[1] };
  igraph_real_t origtol=options->tol;
  
  /* Brush up options if needed */
  if (options->ldv == 0) { options->ldv=options->n; }
  if (options->lworkl == 0) { options->lworkl=3*options->ncv*(options->ncv+2); }
  if (options->which[0] == 'X') { options->which[0]='L'; options->which[1]='M'; }
  
  if (storage) {
    /* Storage provided */
    if (storage->maxn < options->n) {
      IGRAPH_ERROR("Not enough storage for ARPACK (`n')", IGRAPH_EINVAL);
    }
    if (storage->maxncv < options->ncv) {
      IGRAPH_ERROR("Not enough storage for ARPACK (`ncv')", IGRAPH_EINVAL);
    }
    if (storage->maxldv < options->ldv) {
      IGRAPH_ERROR("Not enough storage for ARPACK (`ldv')", IGRAPH_EINVAL);
    }

    v      = storage->v; 
    workl  = storage->workl;
    workd  = storage->workd;
    workev = storage->workev;
    dr     = storage->d;
    di     = storage->di;
    resid  = storage->resid;
    select = storage->select;
    
  } else {
    /* Storage not provided */
    free_them=1;
    
#define CHECKMEM(x) \
    if (!x) { \
      IGRAPH_ERROR("Cannot allocate memory for ARPACK", IGRAPH_ENOMEM); \
    } \
    IGRAPH_FINALLY(igraph_free, x);

    v=igraph_Calloc(2*options->n * options->ncv, igraph_real_t); CHECKMEM(v);
    workl=igraph_Calloc(options->lworkl, igraph_real_t); CHECKMEM(workl);
    workd=igraph_Calloc(3*options->n, igraph_real_t); CHECKMEM(workd);
    dr=igraph_Calloc(options->nev+1, igraph_real_t); CHECKMEM(dr);
    di=igraph_Calloc(options->nev+1, igraph_real_t); CHECKMEM(di);
    resid=igraph_Calloc(options->n, igraph_real_t); CHECKMEM(resid);
    select=igraph_Calloc(options->ncv, long int); CHECKMEM(select);
    workev=igraph_Calloc(3*options->ncv, igraph_real_t); CHECKMEM(workev);
    
#undef CHECKMEM

  }
  
  /* Set final bits */
  options->iparam[0]=options->ishift;
  options->iparam[2]=options->mxiter;
  options->iparam[3]=options->nb;
  options->iparam[4]=0;
  options->iparam[6]=options->mode;
  options->info=options->start;
  if (options->start) { 
    for (i=0; i<options->n; i++) {
      resid[i]=MATRIX(*vectors, i, 0);
    }
  }
  
  /* Ok, we have everything */
  while (1) {
    
    igrapharpackdnaupd_(&ido, options->bmat, &options->n, options->which,
		  &options->nev, &options->tol,
		  resid, &options->ncv, v, &options->ldv,
		  options->iparam, options->ipntr,
		  workd, workl, &options->lworkl, &options->info);
    
    if (ido==-1 || ido==1) {
      igraph_real_t *from=workd+options->ipntr[0]-1;
      igraph_real_t *to=workd+options->ipntr[1]-1;
      if (fun(to, from, options->n, extra) != 0) {
	IGRAPH_ERROR("Arpack error while evaluating matrix-vector product",
		     IGRAPH_ARPACK_PROD);
      }      
      
    } else {
      break;
    }
  }

  if (options->info < 0) {
    IGRAPH_ERROR("ARPACK error", igraph_i_arpack_err_dnaupd(options->info));
  }

  options->ierr=0;
  igrapharpackdneupd_(&rvec, all, select, dr, di, v, &options->ldv,
		&options->sigma, &options->sigmai, workev, options->bmat,
		&options->n, options->which, &options->nev, &options->tol,
		resid, &options->ncv, v, &options->ldv, options->iparam,
		options->ipntr, workd, workl, &options->lworkl,
		&options->ierr);

  if (options->ierr < 0) {
    IGRAPH_ERROR("ARPACK error", igraph_i_arpack_err_dneupd(options->info));
  }    

  /* Save the result */
  
  options->noiter=options->iparam[2];
  options->nconv=options->iparam[4];
  options->numop=options->iparam[8];
  options->numopb=options->iparam[9];
  options->numreo=options->iparam[10];

  if (options->nconv < orignev) {
/*     IGRAPH_ERROR("ARPACK error, could not calculate all eigenvectors",  */
/* 		 IGRAPH_ARPACK_UNKNOWN); */
    IGRAPH_WARNING("Could not calculate all eigenpairs");
  }

  if (values) {
    long int i;
    IGRAPH_CHECK(igraph_matrix_resize(values, options->nconv, 2));
    for (i=0; i<options->nconv; i++) {
      MATRIX(*values, i, 0) = dr[i];
      MATRIX(*values, i, 1) = di[i];
    }
  }

  if (vectors) {
    long int i, j, ptr=0;
    IGRAPH_CHECK(igraph_matrix_resize(vectors, options->n, orignev+1));
    for (j=0; j<orignev+1; j++) {	
      for (i=0; i<options->n; i++) {
	MATRIX(*vectors, i, j) = v[ptr++];
      }
    }
  }

  options->ldv=origldv;
  options->lworkl=origlworkl;
  options->which[0] = origwhich[0]; options->which[1] = origwhich[1];
  options->tol=origtol;
  options->nev=orignev;
  
  /* Clean up if needed */
  if (free_them) {
    igraph_Free(workev);
    igraph_Free(select);
    igraph_Free(resid);
    igraph_Free(di);
    igraph_Free(dr);
    igraph_Free(workd);
    igraph_Free(workl);
    igraph_Free(v);
    IGRAPH_FINALLY_CLEAN(8);
  }
  return 0;  
}

/**
 * \function igraph_arpack_unpack_complex
 * \brief Make the result of the non-symmetric ARPACK solver more readable
 * 
 * This function works on the output of \ref igraph_arpack_rnsolve and 
 * brushes it up a bit: it only keeps \p nev eigenvalues/vectors and 
 * every eigenvector is stored in two columns of the \p vectors
 * matrix.
 * 
 * </para><para>
 * The output of the non-symmetric ARPACK solver is somewhat hard to
 * parse, as real eigenvectors occupy only one column in the matrix,
 * and the complex conjugate eigenvectors are not stored at all
 * (usually). The other problem is that the solver might return more
 * eigenvalues than requested. The common use of this function is to
 * call it directly after \ref igraph_arpack_rnsolve with its \p
 * vectors and \p values argument and \c options->nev as \p nev.
 * \param vectors The eigenvector matrix, as returned by \ref
 *   igraph_arpack_rnsolve. It will be resized, typically it will be
 *   larger. 
 * \param values The eigenvalue matrix, as returned by \ref
 *   igraph_arpack_rnsolve. It will be resized, typically extra,
 *   unneeded rows (=eigenvalues) will be removed.
 * \param nev The number of eigenvalues/vectors to keep. Can be less
 *   or equal than the number originally requested from ARPACK.
 * \param reverse Logical scalar, if \c TRUE, then the eigenvalues
 *   with the smallest magnitude will be kept, instead of the largest
 *   ones.
 * \return Error code.
 * 
 * Time complexity: linear in the number of elements in the \p vectors
 * matrix. 
 */

int igraph_arpack_unpack_complex(igraph_matrix_t *vectors, igraph_matrix_t *values, 
				 long int nev, igraph_bool_t reverse) {

  long int n=igraph_matrix_nrow(vectors);
  long int veccol=igraph_matrix_ncol(vectors);
  long int nconv=igraph_matrix_nrow(values);
  long int i, j;
  igraph_vector_t mod;
  igraph_vector_long_t order;
  igraph_matrix_t newvectors;
  igraph_matrix_t newvalues;
  igraph_vector_long_t index;
  long int colsize=n*sizeof(igraph_real_t);

  if (nev <0) {
    IGRAPH_ERROR("`nev' must be non-negative", IGRAPH_EINVAL);
  }
  if (nev > nconv) {
    printf("nev= %li, nconv= %li\n", nev, nconv);
    IGRAPH_ERROR("`nev' too big", IGRAPH_EINVAL);
  }
  if (igraph_matrix_ncol(values) != 2) {
    IGRAPH_ERROR("'values' must have two columns", IGRAPH_EINVAL);
  }
  if (veccol < nconv) {
    IGRAPH_ERROR("`vectors' and `values' don't match, "
		 "are they from the same ARPACK call?", IGRAPH_EINVAL);
  }

  /* Special case */
  if (nev==0) {
    IGRAPH_CHECK(igraph_matrix_resize(vectors, n, 0));
    IGRAPH_CHECK(igraph_matrix_resize(values, 0, 2));
    return 0;
  }

  /* There might be some "zero eigenvalues", we remove these */
  while (nconv > 0) {
    if (MATRIX(*values, nconv-1, 0) != 0 ||
	MATRIX(*values, nconv-1, 1) != 0) {
      break;
    }
    nconv--;
  }

  /* We need to order the eigenvalues, according to their modulus. */
  
  IGRAPH_CHECK(igraph_vector_long_init(&order, 0));
  IGRAPH_FINALLY(igraph_vector_long_destroy, &order);
  IGRAPH_VECTOR_INIT_FINALLY(&mod, nconv);
  for (i=0; i<nconv; i++) {
    igraph_real_t rp=MATRIX(*values, i, 0);
    igraph_real_t ip=MATRIX(*values, i, 1);
    VECTOR(mod)[i] = rp*rp + ip*ip;
  }
  IGRAPH_CHECK(igraph_vector_sort_order(&mod, &order, /*reverse=*/ reverse));
  igraph_vector_destroy(&mod);
  IGRAPH_FINALLY_CLEAN(1);
  
  /* In case of conjugates, the one with the positive imaginary part
     comes first. If 'reverse', then the order is the opposite. */
  
  for (i=0; i<nconv-1; i++) {
    long int oi=VECTOR(order)[i];
    long int oip1=VECTOR(order)[i+1];
    igraph_real_t ri=MATRIX(*values, oi, 0);
    igraph_real_t ii=MATRIX(*values, oi, 1);
    igraph_real_t rip1=MATRIX(*values, oip1, 0);
    igraph_real_t iip1=MATRIX(*values, oip1, 1);
    if (ri==rip1 && ii==-iip1 && ii < 0 && !reverse) {
      /* Switch them */
      VECTOR(order)[i]=oip1;
      VECTOR(order)[i+1]=oi;
    } else if (ri==rip1 && ii==-iip1 && ii > 0 && reverse) {
      /* Switch them */
      VECTOR(order)[i]=oip1;
      VECTOR(order)[i+1]=oi;
    }    
  }

  /* Create an index vector that points to the columns where the
     eigenvectors of the given eigenvalue begin. For complex
     conjugates, both pointers point to the same place. */

  IGRAPH_CHECK(igraph_vector_long_init(&index, nconv));
  IGRAPH_FINALLY(igraph_vector_long_destroy, &index);
  for (i=0, j=0; i<nconv; i++) {
    igraph_real_t ip=MATRIX(*values, i, 1);
    VECTOR(index)[i]=j;
    if (ip==0) {
      j++; 
    } else if (ip > 0) {
      /* nothing to do */
    } else {
      j+=2;
    }
  }

  IGRAPH_MATRIX_INIT_FINALLY(&newvalues, nev, 2);
  IGRAPH_MATRIX_INIT_FINALLY(&newvectors, n, nev*2);

  for (i=0; i<nev; i++) {
    long int idx=VECTOR(order)[i];
    igraph_real_t rp = MATRIX(*values, idx, 0);
    igraph_real_t ip = MATRIX(*values, idx, 1);
    long int colidx=VECTOR(index)[idx];

    MATRIX(newvalues, i, 0) = rp;
    MATRIX(newvalues, i, 1) = ip;

    if (ip==0) {
      /* real eigenvalue */
      memcpy(igraph_matrix_e_ptr(&newvectors, 0, 2*i), 
	     igraph_matrix_e_ptr(vectors, 0, colidx), colsize);
    } else if (ip>0) {
      /* complex eigenvalue, positive imaginary part */
      memcpy(igraph_matrix_e_ptr(&newvectors, 0, 2*i), 
	     igraph_matrix_e_ptr(vectors, 0, colidx), colsize*2);      
    } else { 
      /* complex eigenvalue, negative imaginary part */
      memcpy(igraph_matrix_e_ptr(&newvectors, 0, 2*i), 
	     igraph_matrix_e_ptr(vectors, 0, colidx), colsize*2);      
      for (j=0; j<n; j++) {
	MATRIX(newvectors, j, 2*i+1) *= (-1);
      }
    }
  }

  IGRAPH_CHECK(igraph_matrix_update(values, &newvalues));
  IGRAPH_CHECK(igraph_matrix_update(vectors, &newvectors));
  
  igraph_matrix_destroy(&newvectors);
  igraph_matrix_destroy(&newvalues);  
  igraph_vector_long_destroy(&index);
  igraph_vector_long_destroy(&order);
  IGRAPH_FINALLY_CLEAN(4);

  return 0;
}

/**
 */

int igraph_i_arpack_eigen_symmetric(igraph_arpack_function_t *fun,
				    long int mdim,
				    void *extra,
				    const igraph_vector_long_t *which,
				    igraph_matrix_t *values,
				    igraph_matrix_t *vectors,
				    igraph_arpack_options_t *options) {
  
  igraph_vector_long_t which_small, which_large;
  long int nev=igraph_vector_long_size(which);
  long int wmin, wmax;
  long int nlarge, nsmall;
  long int i;

  igraph_vector_long_t order;

  /* TODO: make values and vectors optional */
  
  igraph_vector_t myvalues_v, *myvalues=&myvalues_v;
  igraph_matrix_t myvectors_v, *myvectors=&myvectors_v;

  igraph_vector_long_minmax(which, &wmin, &wmax);
  if (wmin < 1 || wmax > mdim) {
    IGRAPH_ERROR("Invalid eigenvectors, must be between 1 and n", 
		 IGRAPH_EINVAL);
  }

  IGRAPH_CHECK(igraph_vector_long_init(&which_small, 0));
  IGRAPH_FINALLY(igraph_vector_long_destroy, &which_small);
  IGRAPH_CHECK(igraph_vector_long_init(&which_large, 0));
  IGRAPH_FINALLY(igraph_vector_long_destroy, &which_large);
  
  for (i=0; i<nev; i++) {
    long int ev=VECTOR(*which)[i];
    if (ev <= mdim/2) { 
      IGRAPH_CHECK(igraph_vector_long_push_back(&which_large, ev)); 
    } else {
      IGRAPH_CHECK(igraph_vector_long_push_back(&which_small, ev));
    }
  }
  nlarge=igraph_vector_long_size(&which_large);
  nsmall=igraph_vector_long_size(&which_small);  

  IGRAPH_VECTOR_INIT_FINALLY(&myvalues_v, 0);
  IGRAPH_MATRIX_INIT_FINALLY(&myvectors_v, 0, 0);

  IGRAPH_CHECK(igraph_matrix_resize(values, nev, 1));
  IGRAPH_CHECK(igraph_matrix_resize(vectors, mdim, nev));
  igraph_matrix_null(values);
  igraph_matrix_null(vectors);

  /* Beginning of the spectrum */

  if (nlarge > 0) {
    long int last_large=igraph_vector_long_max(&which_large);    
    options->n=mdim;
    options->which[0]='L'; options->which[1]='A';
    options->nev=last_large;
    options->ncv=2*last_large+1;
    if (options->ncv > mdim) { options->ncv=mdim; }
    igraph_arpack_rssolve(fun, extra,
			  options, /*storage=*/ 0, myvalues, myvectors);
    IGRAPH_CHECK(igraph_vector_long_init(&order, 0));
    IGRAPH_FINALLY(igraph_vector_long_destroy, &order);
    IGRAPH_CHECK(igraph_vector_sort_order(myvalues, &order, /*reverse=*/ 0));
    
    /* Save results */
    for (i=0; i<nev; i++) {
      long int j, pos, ev=VECTOR(*which)[i];
      if (ev > mdim/2) { continue; }
      pos=VECTOR(order)[ev-1];
      MATRIX(*values, i, 0) = VECTOR(*myvalues)[ev-1];

      for (j=0; j<mdim; j++) {
	MATRIX(*vectors, j, i) = MATRIX(*myvectors, j, pos);
      }
    }    

    igraph_vector_long_destroy(&order);
    IGRAPH_FINALLY_CLEAN(1);
      
  }

  /* End of the spectrum */

  if (nsmall > 0) {
    long int last_small=igraph_vector_long_min(&which_small);
    options->n=mdim;
    options->which[0]='S'; options->which[1]='A';
    options->nev=mdim-last_small+1;
    options->ncv=2*options->nev+1;
    if (options->ncv > mdim) { options->ncv=mdim; }
    igraph_arpack_rssolve(fun, extra,
			  options, /*storage=*/ 0, myvalues, myvectors);
    IGRAPH_CHECK(igraph_vector_long_init(&order, 0));
    IGRAPH_FINALLY(igraph_vector_long_destroy, &order);
    IGRAPH_CHECK(igraph_vector_sort_order(myvalues, &order, /*reverse=*/ 1));
    
    /* Save results */
    for (i=0; i<nev; i++) {
      long int j, pos, ev=VECTOR(*which)[i];
      if (ev <= mdim/2) { continue; }
      pos=VECTOR(order)[mdim-ev];
      MATRIX(*values, i, 0) = VECTOR(*myvalues)[mdim-ev];
      
      for (j=0; j<mdim; j++) {
	MATRIX(*vectors, j, i) = MATRIX(*myvectors, j, pos);
      }
    }
    
    igraph_vector_long_destroy(&order);
    IGRAPH_FINALLY_CLEAN(1);
  }

  igraph_matrix_destroy(myvectors);
  IGRAPH_FINALLY_CLEAN(1);
  igraph_vector_destroy(myvalues);
  IGRAPH_FINALLY_CLEAN(1);

  igraph_vector_long_destroy(&which_large);
  igraph_vector_long_destroy(&which_small);
  IGRAPH_FINALLY_CLEAN(2);

  return 0;
}

/**
 */

int igraph_i_arpack_eigen(igraph_arpack_function_t *fun,
			  long int mdim,
			  void *extra,
			  const igraph_vector_long_t *which,
			  igraph_matrix_t *values,
			  igraph_matrix_t *vectors,
			  igraph_arpack_options_t *options) {

  igraph_vector_long_t which_small, which_large;
  long int nev=igraph_vector_long_size(which);
  long int wmin, wmax;
  long int nlarge, nsmall;
  long int i;

  /* TODO: make values and vectors optional */
  
  igraph_matrix_t myvalues_v, *myvalues=&myvalues_v;
  igraph_matrix_t myvectors_v, *myvectors=&myvectors_v;

  igraph_vector_long_minmax(which, &wmin, &wmax);
  if (wmin < 1 || wmax > mdim) {
    IGRAPH_ERROR("Invalid eigenvectors, must be between 1 and n", 
		 IGRAPH_EINVAL);
  }
  
  IGRAPH_CHECK(igraph_vector_long_init(&which_small, 0));
  IGRAPH_FINALLY(igraph_vector_long_destroy, &which_small);
  IGRAPH_CHECK(igraph_vector_long_init(&which_large, 0));
  IGRAPH_FINALLY(igraph_vector_long_destroy, &which_large);
  
  for (i=0; i<nev; i++) {
    long int ev=VECTOR(*which)[i];
    if (ev <= mdim/2) { 
      IGRAPH_CHECK(igraph_vector_long_push_back(&which_large, ev)); 
    } else {
      IGRAPH_CHECK(igraph_vector_long_push_back(&which_small, ev));
    }
  }
  nlarge=igraph_vector_long_size(&which_large);
  nsmall=igraph_vector_long_size(&which_small);  

  IGRAPH_MATRIX_INIT_FINALLY(&myvalues_v, 0, 0);
  IGRAPH_MATRIX_INIT_FINALLY(&myvectors_v, 0, 0);  

  IGRAPH_CHECK(igraph_matrix_resize(values, nev, 2));
  IGRAPH_CHECK(igraph_matrix_resize(vectors, mdim, 2*nev));
  igraph_matrix_null(values);
  igraph_matrix_null(vectors);
  
  /* Beginning of the spectrum */

  if (nlarge > 0) {
    long int last_large=igraph_vector_long_max(&which_large);    
    options->n=mdim;
    options->which[0]='L'; options->which[1]='M';
    options->nev=last_large+2;
    options->ncv=2*options->nev+2;
    if (options->ncv < 4) { options->ncv=4; }
    if (options->ncv > mdim) { options->ncv=mdim; }
    igraph_arpack_rnsolve(fun, extra,
			  options, /*storage=*/ 0, myvalues, myvectors);
    IGRAPH_CHECK(igraph_arpack_unpack_complex(myvectors, myvalues, last_large, 
					      /*reverse=*/ 0));

    /* Save results */
    for (i=0; i<nev; i++) {
      long int j, ev=VECTOR(*which)[i];
      if (ev > mdim/2) { continue; }
      ev--;
      MATRIX(*values, i, 0) = MATRIX(*myvalues, ev, 0);
      MATRIX(*values, i, 1) = MATRIX(*myvalues, ev, 1);

      for (j=0; j<mdim; j++) {
	MATRIX(*vectors, j, 2*i) = MATRIX(*myvectors, j, 2*ev);
	MATRIX(*vectors, j, 2*i+1) = MATRIX(*myvectors, j, 2*ev+1);
      }
    }    
  }

  /* End of the spectrum */

  if (nsmall > 0) {
    long int last_small=igraph_vector_long_min(&which_small);
    options->n=mdim;
    options->which[0]='S'; options->which[1]='M';
    options->nev=mdim-last_small+1+2;
    options->ncv=2*options->nev+2;
    if (options->ncv < 4) { options->ncv=4; }
    if (options->ncv > mdim) { options->ncv=mdim; }
    igraph_arpack_rnsolve(fun, extra, 
			  options, /*storage=*/ 0, myvalues, myvectors);
    IGRAPH_CHECK(igraph_arpack_unpack_complex(myvectors, myvalues,
					      mdim-last_small+1, 
					      /*reverse=*/ 1));
    
    /* Save results */
    for (i=0; i<nev; i++) {
      long int j, ev=VECTOR(*which)[i];
      if (ev <= mdim/2) { continue; }
      ev=mdim-ev;
      MATRIX(*values, i, 0) = MATRIX(*myvalues, ev, 0);
      MATRIX(*values, i, 1) = MATRIX(*myvalues, ev, 1);
      
      for (j=0; j<mdim; j++) {
	MATRIX(*vectors, j, 2*i) = MATRIX(*myvectors, j, 2*ev);
	MATRIX(*vectors, j, 2*i+1) = MATRIX(*myvectors, j, 2*ev+1);
      }
    }
  }
  
  igraph_matrix_destroy(myvectors);
  IGRAPH_FINALLY_CLEAN(1);
  igraph_matrix_destroy(myvalues);
  IGRAPH_FINALLY_CLEAN(1);

  igraph_vector_long_destroy(&which_large);
  igraph_vector_long_destroy(&which_small);
  IGRAPH_FINALLY_CLEAN(2);

  return 0;
}

int igraph_arpack_eigen(igraph_bool_t symmetric,
			igraph_arpack_function_t *fun,
			long int mdim,
			void *extra,
			const igraph_vector_long_t *which,
			igraph_matrix_t *values,
			igraph_matrix_t *vectors,
			igraph_arpack_options_t *options) {

  if (symmetric) {
    return igraph_i_arpack_eigen_symmetric(fun, mdim, extra, which, 
					   values, vectors, options);
  } else {
    return igraph_i_arpack_eigen(fun, mdim, extra, which,
				 values, vectors, options);
  }
}

int igraph_i_arpack_eigen_matrix(igraph_real_t *to, 
				 const igraph_real_t *from,
				 long int n, void *extra) {
  
  igraph_matrix_t *mat=extra;
  igraph_vector_t vfrom, vto;
  
  igraph_vector_view(&vfrom, from, n);
  igraph_vector_view(&vto, to, n);
  
  igraph_matrix_vector_prod(mat, &vfrom, &vto);
  return 0;
} 

int igraph_arpack_eigen_matrix(const igraph_matrix_t *matrix,
			       igraph_matrix_symmetric_t symmetric,
			       const igraph_vector_long_t *which,
			       igraph_matrix_t *values,
			       igraph_matrix_t *vectors,
			       igraph_arpack_options_t *options) {

  long int nrow=igraph_matrix_nrow(matrix);
  igraph_bool_t sym;
  
  if (nrow != igraph_matrix_ncol(matrix)) {
    IGRAPH_ERROR("Non-square matrix for ARPACK", IGRAPH_NONSQUARE);
  }

  sym = symmetric==IGRAPH_MATRIX_SYMMETRIC ||
    (symmetric==IGRAPH_MATRIX_CHECKSYMMETRIC && 
     igraph_matrix_is_symmetric(matrix));
  
  return igraph_arpack_eigen(sym, igraph_i_arpack_eigen_matrix,
			     nrow, (void*) matrix, 
			     which, values, vectors, options);  
}

typedef struct igraph_i_arpack_eigen_graph_data_t {
  const igraph_t *graph;
  const igraph_adjlist_t *adjlist;
  const igraph_adjedgelist_t *adjedgelist;
  const igraph_vector_t *weights;
} igraph_i_arpack_eigen_graph_data_t;

int igraph_i_arpack_eigen_graph(igraph_real_t *to,
				const igraph_real_t *from,
				long int n, void *extra) {
  
  const igraph_i_arpack_eigen_graph_data_t *extra2=extra;
  const igraph_adjlist_t *adjlist=extra2->adjlist;
  igraph_vector_t *neis;
  long int i, j, nlen;
  
  for (i=0; i<n; i++) {
    neis=igraph_adjlist_get(adjlist, i);
    nlen=igraph_vector_size(neis);
    to[i]=0.0;
    for (j=0; j<nlen; j++) {
      long int nei=VECTOR(*neis)[j];
      to[i] += from[nei];
    }
  }

  return 0;
}

int igraph_i_arpack_eigen_graph_weighted(igraph_real_t *to,
					 const igraph_real_t *from,
					 long int n, void *extra) {

  const igraph_i_arpack_eigen_graph_data_t *extra2=extra;
  const igraph_t *graph=extra2->graph;
  const igraph_adjedgelist_t *adjedgelist=extra2->adjedgelist;
  const igraph_vector_t *weights=extra2->weights;
  igraph_vector_t *adj;
  long int i, j, nlen;
  
  for (i=0; i<n; i++) {
    adj=igraph_adjedgelist_get(adjedgelist, i);
    nlen=igraph_vector_size(adj);
    to[i]=0.0;
    for (j=0; j<nlen; j++) {
      long int edge=VECTOR(*adj)[i];
      long int nei=IGRAPH_OTHER(graph, edge, i);
      igraph_real_t w=VECTOR(*weights)[edge];
      to[i] += w*from[nei];
    }
  }
  
  return 0;
}

int igraph_arpack_eigen_graph(const igraph_t *graph,
			      const igraph_vector_t *weights,
			      const igraph_vector_long_t *which,
			      igraph_matrix_t *values,
			      igraph_matrix_t *vectors,
			      igraph_arpack_options_t *options) {
  
  igraph_i_arpack_eigen_graph_data_t extra;
  igraph_adjlist_t adjlist;
  igraph_adjedgelist_t adjedgelist;
  long int no_of_nodes=igraph_vcount(graph);
  long int no_of_edges=igraph_ecount(graph);
  igraph_bool_t sym= !igraph_is_directed(graph);
  
  if (weights) {

    if (igraph_vector_size(weights) != no_of_edges) {
      IGRAPH_ERROR("Invalid `weight vector size", IGRAPH_EINVAL);
    }

    IGRAPH_CHECK(igraph_adjedgelist_init(graph, &adjedgelist, IGRAPH_OUT));
    IGRAPH_FINALLY(igraph_adjedgelist_destroy, &adjedgelist);

    extra.graph=graph;
    extra.adjlist=0;
    extra.adjedgelist=&adjedgelist;
    extra.weights=weights;

    IGRAPH_CHECK(igraph_arpack_eigen(sym, igraph_i_arpack_eigen_graph_weighted,
				     no_of_nodes, (void*) &extra,
				     which, values, vectors, options));

    igraph_adjedgelist_destroy(&adjedgelist);
    IGRAPH_FINALLY_CLEAN(1);

  } else {

    IGRAPH_CHECK(igraph_adjlist_init(graph, &adjlist, IGRAPH_OUT));
    IGRAPH_FINALLY(igraph_adjlist_destroy, &adjlist);

    extra.graph=graph;
    extra.adjlist=&adjlist;
    extra.adjedgelist=0;
    extra.weights=0;

    IGRAPH_CHECK(igraph_arpack_eigen(sym, igraph_i_arpack_eigen_graph,
				     no_of_nodes, (void*) &extra,
				     which, values, vectors, options));
    
    igraph_adjlist_destroy(&adjlist);
    IGRAPH_FINALLY_CLEAN(1);
    
  }
      
  return 0;
}

