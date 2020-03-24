#include "multitaper.h"
#include <math.h>

namespace sap
{

#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define PI 3.14159265358979

MultiTaper::MultiTaper(int size) : size_(size)
{
  buffer_ = new float[100 + size_ * 4];
  proTapers(size_, 4, 1.5, buffer_);
  if (buffer_[2] < 0)
  {
    for (int i = 0; i < size_; ++i)
    {
      buffer_[i] *= -1;
    }
  }
  if (buffer_[size_ + 2] < 0)
  {
    for (int i = size_; i < size_ * 2; ++i)
    {
      buffer_[i] *= -1;
    }
  }
  taper1_ = buffer_;
  taper2_ = &buffer_[size_];
}

MultiTaper::~MultiTaper()
{
  delete[] buffer_;
  taper1_ = taper2_ = nullptr;
}

void MultiTaper::proTapers(int WindowSize, int tpr, float wd, float TapersOut[])
{
    double tapsum[10], lambda[10], *tapers;
    tapers =new double[WindowSize*8];
    multitap(WindowSize, 4, lambda, wd, tapers, tapsum, TapersOut);
    delete[] tapers;
}

// {{{ multitap
void MultiTaper::multitap(int num_points, int nwin, double *lam, float npi, double *tapers,
    double *tapsum, float TapersOut[])
{
  /*
   get the multitaper slepian functions:
   num_points = number of points in data stream
   nwin = number of windows
   lam= vector of eigenvalues
   npi = order of slepian functions
   tapsum = sum of each taper, saved for use in adaptive weighting
   tapers =  matrix of slepian tapers, packed in a 1D double array
  */
  int    i, k, kk, *ip, ierr, m11;
  double  ww, cs, ai, an, eps, rlu, rlb, aa ,dfac, drat, gamma, bh, tapsq
  ,TWOPI, DPI,*diag, *offdiag, *offsq, *scratch1, *scratch2, *scratch3,
   *scratch4 , *scratch6, anpi, *ell, *evecs;
  /* need to initialize iwflag = 0 */
  ip=new int[num_points];
  diag=new double[num_points];
  offdiag=new double[num_points];
  offsq=new double[num_points];
  scratch1=new double[num_points];
  scratch2=new double[num_points];
  scratch3=new double[num_points];
  scratch4=new double[num_points];
  scratch6=new double[num_points];
  evecs=new double[num_points* nwin];
  ell=new double[10];

  
  DPI = (double) PI;
  TWOPI = (double) 2 *DPI;
  anpi = npi;
  an = (double) (num_points);
  ww = (double) (anpi) / an;	/* this corresponds to P&W's W value  */
  cs = cos(TWOPI * ww);


  /* make the diagonal elements of the tridiag matrix  */
  for (i = 0; i < num_points; i++) 
  {
          ai = (double) (i);
          diag[i] = -cs * (((an - 1.) / 2. - ai)) * (((an - 1.) / 2. - ai));
          offdiag[i] = -ai * (an - ai) / 2.;
          offsq[i] = offdiag[i] * offdiag[i];
  }

  eps = 1.0e-13;
  m11 = 1;


  /* call the eispac routines to invert the tridiagonal system */
  jtridib_(&num_points, &eps, diag, offdiag, offsq, &rlb, &rlu, &m11, &nwin, lam,
           ip, &ierr, scratch1, scratch2);


  jtinvit_(&num_points, &num_points, diag, offdiag, offsq, &nwin, lam, ip, evecs, 			&ierr, scratch1, scratch2, scratch3, scratch4, scratch6);

  delete[]scratch1;
  delete[]scratch2;
  delete[]scratch3;
  delete[]scratch4;
  delete[]scratch6;

  /*
   * we calculate the eigenvalues of the dirichlet-kernel problem i.e.
   * the bandwidth retention factors from slepian 1978 asymptotic
   * formula, gotten from thomson 1982 eq 2.5 supplemented by the
   * asymptotic formula for k near 2n from slepian 1978 eq 61 more
   * precise values of these parameters, perhaps useful in adaptive
   * spectral estimation, can be calculated explicitly using the
   * rayleigh-quotient formulas in thomson (1982) and park et al (1987)
   *
   */
  dfac = (double) an *DPI * ww;
  drat = (double) 8. *dfac;
  dfac = (double) 4. *sqrt(DPI * dfac) * exp((double) (-2.0) * dfac);

  for (k = 0; k < nwin; k++)
  {
          lam[k] = (double) 1.0 - (double) dfac;
          dfac = dfac * drat / (double) (k + 1);
          /* fails as k -> 2n */
  }

  gamma = log((double) 8. * an * sin((double) 2. * DPI * ww)) + (double) 	0.5772156649;

  for (k = 0; k < nwin; k++)
  {
          bh = -2. * DPI * (an * ww - (double) (k) /
            (double) 2. - (double) .25) / gamma;
          ell[k] = (double) 1. / ((double) 1. + exp(DPI * (double) bh));
  }

  for (i = 0; i < nwin; i++) lam[i] = max(ell[i], lam[i]);

  /************************************************************
  c   normalize the eigentapers to preserve power for a white process
  c   i.e. they have rms value unity
          c  tapsum is the average of the eigentaper, should be near zero for
  c  antisymmetric tapers
  ************************************************************/

  for (k = 0; k < nwin; k++) {
          kk = (k) * num_points;
          tapsum[k] = 0.;
          tapsq = 0.;
          for (i = 0; i < num_points; i++) {
                  aa = evecs[i + kk];
                  tapers[i + kk] = aa;
      tapsum[k] = tapsum[k] + aa;
                  tapsq = tapsq + aa * aa;
          }
          aa = sqrt(tapsq / (double) num_points);
          tapsum[k] = tapsum[k] / aa;


          for (i = 0; i < num_points; i++) {
      TapersOut[i+kk]=tapers[i + kk] / aa;
      tapers[i + kk] = tapers[i + kk] / aa;

          }
  }

  /* Free Memory */

  delete[]ell;
  delete[]diag;
  delete[]offdiag;
  delete[]offsq;
  delete[]ip;
  delete[]evecs;


  return ;
}
// }}}

 // {{{ jtinvit_
void MultiTaper::jtinvit_(int *nm, int *n, double *d, double *e, double *e2, int *m, double *w, int *ind, double *z, int *ierr, double *rv1, double *rv2,
	 double *rv3, double *rv4, double *rv6)
{
	static double   machep = 1.25e-15, norm, u, v, order, x0, x1, uk, xu
	, eps2, eps3, eps4, rtem;
	int   z_dim1, z_offset, i1, i2, i3;
	double  d1, d2;
	static int   i, j, p, q, r, s, group, ii, jj, ip, tag, its;

	/* this subroutine is a translation of the inverse iteration tech- */
	/* nique in the algol procedure tristurm by peters and wilkinson. */
	/* handbook for auto. comp., vol.ii-linear algebra, 418-439(1971). */
	/* this subroutine finds those eigenvectors of a tridiagonal */
	/* symmetric matrix corresponding to specified eigenvalues, */
	/* using inverse iteration. */

	--rv6;	--rv4;	--rv3;	--rv2;	--rv1;	--e2;	--e;	--d;
	z_dim1 = *nm;
	z_offset = z_dim1 + 1;
	z -= z_offset;
	--ind;	--w;
	*ierr = 0;
	if (*m == 0) goto L1001;
	tag = 0;
	order = 1. - e2[1];
	q = 0;

L100:
	p = q + 1;
	i1 = *n;
	for (q = p; q <= i1; ++q)
	{
		if (q == *n) goto L140;
		if (e2[q + 1] == 0.) goto L140;
	}

L140:
	++tag;
	s = 0;
	i1 = *m;
	for (r = 1; r <= i1; ++r)
	{
		if (ind[r] != tag) goto L920;
		its = 1;
		x1 = w[r];
		if (s != 0) goto L510;
		xu = 1.;
		if (p != q) goto L490;
		rv6[p] = 1.;
		goto L870;
L490:
		norm = (d1 = d[p], abs(d1));
		ip = p + 1;

		i2 = q;
		for (i = ip; i <= i2; ++i)
			norm = norm + (d1 = d[i], abs(d1)) + (d2 = e[i], abs(d2));
		eps2 = norm * 0.001;
		eps3 = machep * norm;
		uk = (double) (q - p + 1);
		eps4 = uk * eps3;
		uk = eps4 / sqrt(uk);
		s = p;
L505:
		group = 0;
		goto L520;

L510:
		if ((d1 = x1 - x0, abs(d1)) >= eps2) goto L505;
		++group;
		if (order * (x1 - x0) <= 0.) x1 = x0 + order * eps3;

L520:
		v = 0.;
		i2 = q;
		for (i = p; i <= i2; ++i)
		{
			rv6[i] = uk;
			if (i == p) goto L560;
			if ((d1 = e[i], abs(d1)) < abs(u)) goto L540;
			xu = u / e[i];
			rv4[i] = xu;
			rv1[i - 1] = e[i];
			rv2[i - 1] = d[i] - x1;
			rv3[i - 1] = 0.;
			if (i != q) rv3[i - 1] = e[i + 1];
			u = v - xu * rv2[i - 1];
			v = -xu * rv3[i - 1];
			goto L580;
L540:
			xu = e[i] / u;
			rv4[i] = xu;
			rv1[i - 1] = u;
			rv2[i - 1] = v;
			rv3[i - 1] = 0.;
L560:
			u = d[i] - x1 - xu * v;
			if (i != q) v = e[i + 1];

L580:
			;
		}

		if (u == 0.) u = eps3;
		rv1[q] = u;	rv2[q] = 0.;	rv3[q] = 0.;

L600:
		i2 = q;
		for (ii = p; ii <= i2; ++ii)
		{
			i = p + q - ii;
			rtem = rv6[i] - u * rv2[i] - v * rv3[i];
			rv6[i] = (rtem) / rv1[i];
			v = u;
			u = rv6[i];
			/* L620: */
		}

		if (group == 0) goto L700;
		j = r;
		i2 = group;
		for (jj = 1; jj <= i2; ++jj)
		{
L630:
			--j;
			if (ind[j] != tag) goto L630;
			xu = 0.;
			i3 = q;
			for (i = p; i <= i3; ++i) xu += rv6[i] * z[i + j * z_dim1];
			i3 = q;
			for (i = p; i <= i3; ++i) rv6[i] -= xu * z[i + j * z_dim1];
		}

L700:
		norm = 0.;
		i2 = q;
		for (i = p; i <= i2; ++i) norm += (d1 = rv6[i], abs(d1));
		if (norm >= 1.) goto L840;
		if (its == 5) goto L830;
		if (norm != 0.)	goto L740;
		rv6[s] = eps4;
		++s;
		if (s > q) s = p;
		goto L780;
L740:
		xu = eps4 / norm;
		i2 = q;
		for (i = p; i <= i2; ++i) rv6[i] *= xu;
L780:
		i2 = q;
		for (i = ip; i <= i2; ++i) 
		{
			u = rv6[i];
			if (rv1[i - 1] != e[i]) goto L800;
			u = rv6[i - 1];
			rv6[i - 1] = rv6[i];
L800:
			rv6[i] = u - rv4[i] * rv6[i - 1];
			/* L820: */
		}

		++its;
		goto L600;

L830:
		*ierr = -r;
		xu = 0.;
		goto L870;
		
L840:
		u = 0.;
		i2 = q;
		for (i = p; i <= i2; ++i) 
		{
			d1 = rv6[i];
			u += d1 * d1;
		}

		xu = 1. / sqrt(u);

L870:
		i2 = *n;
		for (i = 1; i <= i2; ++i) z[i + r * z_dim1] = 0.;
		i2 = q;
		for (i = p; i <= i2; ++i) z[i + r * z_dim1] = rv6[i] * xu;
		x0 = x1;
L920:
		;
	}

	if (q < *n) goto L100;
L1001:
	return;
	
}
// }}} 

// {{{ jtridib_
void MultiTaper::jtridib_(int *n, double *eps1, double *d, double *e, double *e2, double *lb, double *ub, int *m11, int *m, double *w, int *ind, int *ierr,
	 double *rv4, double *rv5)
{
	static double   machep = 1.25e-15;
	int             i1, i2;
	double          d1, d2, d3;
	static int      i, j, k, l, p, q, r, s, m1, m2, m22, ii, isturm, tag;
	static double   u, v, t1, t2, x0, x1, xu;
	/* this subroutine is a translation of the algol procedure bisect, */
	/* num. math. 9, 386-393(1967) by barth, martin, and wilkinson. */
	/* handbook for auto. comp., vol.ii-linear algebra, 249-256(1971). */
	/* this subroutine finds those eigenvalues of a tridiagonal */
	/* symmetric matrix between specified boundary indices, */
	/* using bisection. */
	--rv5;	--rv4;	--e2;	--e;	--d;	--ind;	--w;
	*ierr = 0;	tag = 0;	xu = d[1];	x0 = d[1];	u = 0.;	
	i1 = *n;
	for (i = 1; i <= i1; ++i) 
	{
		x1 = u;
		u = 0.;
		if (i != *n) u = (d1 = e[i + 1], abs(d1));
		d1 = d[i] - (x1 + u);
		xu = min(d1, xu);
		d1 = d[i] + (x1 + u);
		x0 = max(d1, x0);
		if (i == 1) goto L20;
		if ((d1 = e[i], abs(d1)) > machep * ((d2 = d[i], abs(d2)) + (
			d3 = d[i - 1], abs(d3)))) goto L40;
L20:
		e2[i] = 0.;
L40:
		;
	}

	/* Computing MAX */
	d1 = abs(xu), d2 = abs(x0);
	x1 = max(d1, d2) * machep * (double) (*n);
	xu -= x1;	t1 = xu;	x0 += x1;	t2 = x0;
	p = 1;
	q = *n;
	m1 = *m11 - 1;
	if (m1 == 0)	goto L75;
	isturm = 1;
L50:
	v = x1;
	x1 = xu + (x0 - xu) * 0.5;
	if (x1 == v)	goto L980;
	goto L320;
L60:
	if ((i1 = s - m1) < 0)	goto L65;
	else if (i1 == 0) goto L73;
	else goto L70;
L65:
	xu = x1;
	goto L50;
L70:
	x0 = x1;
	goto L50;
L73:
	xu = x1;
	t1 = x1;
L75:
	m22 = m1 + *m;
	if (m22 == *n) goto L90;
	x0 = t2;
	isturm = 2;
	goto L50;
L80:
	if ((i1 = s - m22) < 0)	goto L65;
	else if (i1 == 0) goto L85;
	else goto L70;
L85:
	t2 = x1;
L90:
	q = 0;	r = 0;
L100:
	if (r == *m) goto L1001;
	++tag;
	p = q + 1;	xu = d[p];	x0 = d[p];	u = 0.;		i1 = *n;
	for (q = p; q <= i1; ++q) 
	{
		x1 = u;		u = 0.;		v = 0.;
		if (q == *n) goto L110;
		u = (d1 = e[q + 1], abs(d1));
		v = e2[q + 1];
L110:
		d1 = d[q] - (x1 + u);
		xu = min(d1, xu);
		d1 = d[q] + (x1 + u);
		x0 = max(d1, x0);
		if (v == 0.) goto L140;
	}

L140:
	d1 = abs(xu), d2 = abs(x0);
	x1 = max(d1, d2) * machep;
	if (*eps1 <= 0.) *eps1 = -x1;
	if (p != q) goto L180;
	if (t1 > d[p] || d[p] >= t2) goto L940;
	m1 = p;
	m2 = p;
	rv5[p] = d[p];
	goto L900;
L180:
	x1 *= (double) (q - p + 1);
	/* Computing MAX */
	d1 = t1, d2 = xu - x1;
	*lb = max(d1, d2);
	/* Computing MIN */
	d1 = t2, d2 = x0 + x1;
	*ub = min(d1, d2);
	x1 = *lb;
	isturm = 3;
	goto L320;
L200:
	m1 = s + 1;
	x1 = *ub;
	isturm = 4;
	goto L320;
L220:
	m2 = s;
	if (m1 > m2)	goto L940;
	x0 = *ub;
	isturm = 5;
	i1 = m2;
	for (i = m1; i <= i1; ++i)
	{
		rv5[i] = *ub;
		rv4[i] = *lb;
	}
	k = m2;
L250:
	xu = *lb;
	i1 = k;
	for (ii = m1; ii <= i1; ++ii) 
	{
		i = m1 + k - ii;
		if (xu >= rv4[i]) goto L260;
		xu = rv4[i];
		goto L280;
L260:
		;
	}

L280:
	if (x0 > rv5[k]) x0 = rv5[k];
	
L300:
	x1 = (xu + x0) * 0.5;
	if (x0 - xu <= machep * 2. * (abs(xu) + abs(x0)) + abs(*eps1)) goto L420;
	
L320:
	s = p - 1;
	u = 1.;

	i1 = q;
	for (i = p; i <= i1; ++i) 
	{
		if (u != 0.) goto L325;
		v = (d1 = e[i], abs(d1)) / machep;
		if (e2[i] == 0.)	v = 0.;
		goto L330;
L325:
		v = e2[i] / u;
L330:
		u = d[i] - x1 - v;
		if (u < 0.) ++s;
		
	}

	switch ((int) isturm) 
	{
	case 1:
		goto L60;
	case 2:
		goto L80;
	case 3:
		goto L200;
	case 4:
		goto L220;
	case 5:
		goto L360;
	}
	/* :::::::::: refine intervals :::::::::: */
L360:
	if (s >= k) goto L400;
	xu = x1;
	if (s >= m1) goto L380;
	rv4[m1] = x1;
	goto L300;
L380:
	rv4[s + 1] = x1;
	if (rv5[s] > x1) rv5[s] = x1;
	goto L300;
L400:
	x0 = x1;
	goto L300;
	/* :::::::::: k-th eigenvalue found :::::::::: */
L420:
	rv5[k] = x1;
	--k;
	if (k >= m1) goto L250;
	
L900:
	s = r;
	r = r + m2 - m1 + 1;
	j = 1;
	k = m1;
	i1 = r;
	for (l = 1; l <= i1; ++l) 
	{
		if (j > s) goto L910;
		if (k > m2) goto L940;
		if (rv5[k] >= w[l]) goto L915;
		i2 = s;
		for (ii = j; ii <= i2; ++ii) 
		{
			i = l + s - ii;
			w[i + 1] = w[i];
			ind[i + 1] = ind[i];
			/* L905: */
		}

L910:
		w[l] = rv5[k];
		ind[l] = tag;
		++k;
		goto L920;
L915:
		++j;
L920:
		;
	}

L940:
	if (q < *n) goto L100;
	goto L1001;
	
L980:
	*ierr = *n * 3 + isturm;
L1001:
	*lb = t1;
	*ub = t2;
	return ;
}
// }}}

} // namespace

/* vim: set cindent sw=2 expandtab foldmethod=marker : */

