#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>

#include "magnification_map.hpp"


class LightCurve {
public:
  int Nsamples;
  double* t;
  double* m;
  double* dm;

  void writeData(const std::string filename);
  // some functions doing statistics with light curves
};


class LightCurveCollection {
public:
  int Ncurves;
  point* A; // initial location of the light curves in pixels
  point* B; // final location of the light curves in pixels
  LightCurve* lightCurves;
  double pixSizePhys;
  int Nx; // width of the effective map from which the light curves will be exracted
  int Ny; // height of the effective map from which the light curves will be exracted
  EffectiveMap* emap;

  LightCurveCollection(int Ncurves,EffectiveMap* emap);
  LightCurveCollection(const LightCurveCollection& other);
  ~LightCurveCollection(){
    for(int i=0;i<Ncurves;i++){
      free(lightCurves[i].t);
      free(lightCurves[i].m);
      free(lightCurves[i].dm);
    }
    free(lightCurves);
    free(A);
    free(B);
  }

  void setEmap(EffectiveMap* emap);
  void createRandomLocations(int seed,int maxLen);

  std::vector<int> checkLengthFull();
  std::vector<int> checkLength(double v,double tmax);
  std::vector<int> checkSampling(double v,double dt);
  std::vector<int> checkSampling(double v,std::vector<double> t);

  void extractFull();
  void extractSampled(double v,double dt,double tmax);
  void extractStrategy(double v,std::vector<double> t);

  void writeLocations(const std::string filename);
  void writeCurves(const std::string prefix);

private:
  const double factor = 8.64*1.e-5; // conversion from [day*km/s] to [pixels]
  void sampleLightCurve(int index,std::vector<double> length,double phi);
  void interpolatePlane(double xk,double yk,double& m,double& dm);

  const int full_sampling_lower_limit = 10;
  const int sampling_lower_limit = 3;  
};
