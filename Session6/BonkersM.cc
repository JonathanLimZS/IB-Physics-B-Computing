// BonkersM.cc
// one wall moves

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string.h>

using namespace std;

#define D  1  // number of dimensions
struct particle {
  double x[D] ; // (x,y) coordinates
  double p[D] ; // momentum
  double im   ; // inverse mass
  double v[D] ; // velocity
  double T    ; // kinetic energy
  double a    ; // radius of particle
} ; // Note the definition of a structure ends with a semicolon

struct control {
  int verbose    ; // program verbosity
  int printing   ; // period with which to print
  int commanding ; // period with which to issue commands
  int time_in_ms ; // time between frames (commands)
  clock_t latest_time;
  ofstream fout  ; 
};

// Implement     x += v dt for one particle
void PositionStep ( particle &a , double dt )
{ 
  for ( int i = 0 ; i < D ; i++ ) 
    a.x[i] += dt * a.v[i] ; 
}

void v2p( particle &a )
  // propagate the changed velocity into the momentum vector
{
  for ( int i = 0 ; i < D ; i++ ) 
    a.p[i] =  a.v[i] / a.im ;
}

void p2v ( particle &a ) {
  for ( int i = 0 ; i < D ; i++ ) 
    a.v[i]  = a.p[i] * a.im ;
}

void pv2T ( particle &a ) {
  // compute the kinetic energy and angular momentum
  a.T=0.0;
  for ( int i = 0 ; i < D ; i++ ) { 
    a.T    += 0.5*a.v[i] * a.p[i] ; 
  }
}

void collide ( particle & a , particle & b , double d[D] )
  // two particles collide elastically; the line joining
  // their centres is in direction d, which should be normalized
{
  // find the relative velocity 
  double velocity_along_line = 0.0 ; 
  for ( int i = 0 ; i < D ; i++ ) {
    velocity_along_line += ( a.v[i] - b.v[i] ) * d[i] ;
  }
  if(velocity_along_line<0.0) {
    cerr << "warning: particle collided but velocity "
	 << "along alleged line joining centres "
	 << "is negative: " << velocity_along_line
	 << endl ;
  }
  // find a's mass fraction
  double af = a.im / ( a.im + b.im ) ; 
  double bf = b.im / ( a.im + b.im ) ; 
  // reverse the c.o.m. velocity of each along the line of collision
  for ( int i = 0 ; i < D ; i++ ) {
    a.v[i] -= 2.0 * af * velocity_along_line * d[i] ;
    b.v[i] += 2.0 * bf * velocity_along_line * d[i] ;
  }
  v2p( a ) ; 
  v2p( b ) ; 
}

// find next collision time is next
double nct( particle *a , int NN , int &whichn ) {
  double dt = 1e100 ; 
  // examine all adjacent pairs from 0,1   to  NN-2,NN-1
  for ( int n = 0 ; n < NN-1 ; n ++ ) {
    double relative_velocity = a[n].v[0] - a[n+1].v[0] ;
    if(relative_velocity > 0.0) {
      double collision_time =  ((a[n+1].x[0]-a[n+1].a) - (a[n].x[0]+a[n].a))
	                        /relative_velocity ;
      if ( collision_time < dt ) {
	dt = collision_time ;
	whichn = n ;
      }
    }
  }
  return dt ;
}

void leapForward(  particle *a , int NN , double dt ) {
  for( int n = 0 ; n < NN ; n ++ ) 
    PositionStep( a[n] , dt ) ; 
}

void showState ( particle *a , int n0 ,  int NN, ostream &fout )
{
  for( int n = n0 ; n < NN ; n ++ ) {
    for ( int i = 0 ; i < D ; i++ ) { 
      fout << "\t"<<a[n].x[i];
      fout << "\t"<<a[n].v[i];
    }
  }
  fout << endl;
}

double kineticEnergy ( particle *a , int NN )
{
  double E = 0.0 ; 
  for( int n=0 ; n < NN ; n ++ ) {
    for ( int i = 0 ; i < D ; i++ ) { 
      if ( a[n].im > 0.0 ) // avoid infinitely massive objects
	E+=0.5*a[n].v[i]*a[n].v[i]/a[n].im;
    }
  }
  return E ;
}

void  simulateBonkers( particle *a , int NN , double &t , double dt , double T , control &c ) {
  double next_print_dt = 0.0, next_collision_dt ;
  int whichn ;
  double d[D];   d[0]=1.0;
  int we_are_printing_this_time = 1 ; 
  for(;t<=T;) {
    if( we_are_printing_this_time ) {
      c.fout << t << "\t" << kineticEnergy(a,NN) ;
      showState ( a , 0 , NN , c.fout ) ;
      next_print_dt = dt ;
    }
    // find the next event
    next_collision_dt = nct( a , NN , whichn ) ; // returns the time, and 'whichn'
    if ( next_collision_dt < next_print_dt ) {
      // advance time to that event, have a collision
      leapForward( a ,  NN , next_collision_dt ) ;
      t += next_collision_dt ;
      next_print_dt -= next_collision_dt ;
      collide( a[whichn] , a[whichn+1] , d ) ;
      we_are_printing_this_time = 0 ; 
    } else {
      leapForward( a ,  NN , next_print_dt ) ;
      t += next_print_dt ;
      we_are_printing_this_time = 1 ; 
    }
  }
}

int main(int argc, char* argv[])
{ // with 4 particles the avoided volume is 0.8.
  // starting separation is 5.0
  double radius     = 0.1 ;
  double rightWallV = -1.0 ; 
  int N = 4     ; // number of particles
  double T =4.0 ; // target time
  particle   *a ;
  control    c ;
  // set defaults
  double dt = 0.01 ;
  double t = 0.0 ;
  c.verbose = 1 ;
  c.time_in_ms = 100; // real time between plots (in ms)

  // read in any command-line arguments
  if(argc>1)   {
    sscanf( argv[1], "%d", &N ) ; // put the first command-line argument in N
  }
  if(argc>2) {
    sscanf( argv[2], "%lf", &dt ) ; // put the 2nd argument in dt
  }
  if(argc>3) {
    sscanf( argv[3], "%d", &(c.time_in_ms) ) ;
  }

  // try to write output file
  char fileName[]="output/M";
  c.fout.open(fileName);
  if(c.fout.good()==false){
    cerr << "can't write to file " << fileName << endl;
    exit(0);
  }
  
  a = new particle[N+2] ; // 0 (wall) , 1...N , N+1 (wall)
  for ( int n=1 ; n <= N ; n ++ ) {
    a[n].im    = 1.0/(10.0*n + 1.23)  ; 
    a[n].v[0]  = 1.0*n ;
    a[n].x[0]  = 1.0*n ;
    a[n].a     = radius;
  }
  // two walls, right hand one moving
  double vvv = 0.0 ; 
  for ( int n=0 ; n <= N+1 ; n += N+1 , vvv+=rightWallV ) {
    a[n].im    = 0.0 ; 
    a[n].v[0]  = vvv ;   
    a[n].x[0]  = 1.0*n ;
    a[n].a     = 0.0   ; // radius
  }
  for ( int n=0 ; n <= N+1 ; n ++ ) {
    v2p(a[n]);
  }
  simulateBonkers( a , N+2 , t , dt , T , c ) ; 
  
  return 0;
}

