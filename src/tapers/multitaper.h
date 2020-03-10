#ifndef SAP_MULTITAPER_H__
#define SAP_MULTITAPER_H__

namespace sap
{

class MultiTaper
{
public:
  MultiTaper(int size);
  virtual ~MultiTaper();
protected:
private:
  static void proTapers(int WindowSize, int tpr, float wd, float TapersOut[]);
  static void multitap(int num_points, int nwin, double *lam, float npi,
      double *tapers, double *tapsum, float TapersOut[]);

  static void jtinvit_(int *nm, int *n, double *d, double *e, double *e2,
      int *m, double *w, int *ind, double *z, int *ierr, double *rv1,
      double *rv2, double *rv3, double *rv4, double *rv6);

  static void jtridib_(int *n, double *eps1, double *d, double *e, double *e2,
      double *lb, double *ub, int *m11, int *m, double *w, int *ind, int *ierr,
      double *rv4, double *rv5);

  int size_;
  float* taper1_;
  float* taper2_;
};

} // namespace

#endif // SAP_MULTITAPER_H__

/* vim: set cindent sw=2 expandtab : */

