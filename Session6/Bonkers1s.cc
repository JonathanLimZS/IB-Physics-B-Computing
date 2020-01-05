// Bonkers1.cc
//   bonkers bonking in a one-dimensional box.
//      For more general versions see Bonkers.cc and BonkersM.cc
//      in which some of the functions (eg collide) can do D dimensions.
//
// uses the string class, but this is not recommended.

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <string.h>

using namespace std;

struct particle {
  double x    ; // (x,y) coordinates
  double p    ; // momentum
  double im   ; // inverse mass
  double v    ; // velocity
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
  a.x += dt * a.v ; 
}

void v2p( particle &a )
  // propagate the changed velocity into the momentum vector
{
  a.p =  a.v / a.im ;
}

void p2v ( particle &a ) {
  a.v  = a.p * a.im ;
}

void pv2T ( particle &a ) {
  a.T = 0.5*a.v * a.p ; 
}

void collide ( particle & a , particle & b  )
  // two particles collide elastically; the line joining
  // their centres is in direction d, which should be normalized
{
  // find the relative velocity 
  double velocity_along_line =  a.v - b.v;
  // find a's mass fraction
  double af = a.im / ( a.im + b.im ) ; 
  double bf = b.im / ( a.im + b.im ) ; 
  // reverse the c.o.m. velocity of each along the line of collision
  a.v -= 2.0 * af * velocity_along_line ;
  b.v += 2.0 * bf * velocity_along_line ;
  v2p( a ) ; 
  v2p( b ) ; 
}

// find next collision time 
double nct( particle *a , int NN , int &whichn ) {
  double dt = 1e100 ; 
  // examine all adjacent pairs from 0,1   to  NN-2,NN-1
  for ( int n = 0 ; n < NN-1 ; n ++ ) {
    double relative_velocity = a[n].v - a[n+1].v ;
    if(relative_velocity > 0.0) {
      double collision_time =  ((a[n+1].x-a[n+1].a) - (a[n].x+a[n].a))
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
      fout << "\t"<<a[n].x;
      fout << "\t"<<a[n].v;
  }
  fout << endl;
}

double kineticEnergy ( particle *a , int NN )
{
  double E = 0.0 ; 
  for( int n=0 ; n < NN ; n ++ ) {
      if ( a[n].im > 0.0 ) // avoid infinitely massive objects
	E+=0.5*a[n].v*a[n].v/a[n].im;
  }
  return E ;
}

void  simulateBonkers( particle *a , int NN , double &t , double dt , double T , control &c ) {
  double next_print_dt = 0.0, next_collision_dt ;
  int whichn ;
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
      collide( a[whichn] , a[whichn+1] ) ;
      we_are_printing_this_time = 0 ; 
    } else {
      leapForward( a ,  NN , next_print_dt ) ;
      t += next_print_dt ;
      we_are_printing_this_time = 1 ; 
    }
  }
}

int main(int argc, char* argv[])
{
  int N = 4    ; // number of particles
  double T =510.0  ; // target time
  particle   *a ;
  control    c ;
  // set defaults
  double dt = 0.03 ;
  double t = 0.0 ;
  c.verbose = 1 ;
  c.time_in_ms = 100; // real time between plot events (in ms)
  string fileName = "/tmp/1";

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
  if(argc>4) {
    fileName = argv[4] ; // put argv[4] in the string fileName
  }

  // try to write output file
  c.fout.open(fileName.c_str()); // it's a faff to get the character string itself
                                 // out of the 'string' fileName !
  if(c.fout.good()==false){
    cerr << "can't write to file " << fileName << endl;
    exit(0);
  }
  
  a = new particle[N+2] ; // 0 (wall) , 1...N , N+1 (wall)
  // place two walls
  for ( int n=0 ; n <= N+1 ; n += N+1 ) {
    a[n].im = 0.0 ; 
    a[n].v  = 0.0 ;
    a[n].x  = 1.0*n ;
    a[n].a  = 0.0   ; // radius
  }
  // place N particles equally spaced and give them
  // steadily increasing velocities and masses too
  for ( int n=1 ; n <= N ; n ++ ) {
    a[n].im    = 1.0/(10.0*n + 1.23)  ; 
    a[n].v     = 1.0*n ;
    a[n].x     = 1.0*n ;
    a[n].a     = 0.1   ; // radius
    cerr<<"mass: "<<1.0/a[n].im<<endl;
  }
  for ( int n=0 ; n <= N+1 ; n ++ ) {
    v2p(a[n]);
  }
  simulateBonkers( a , N+2 , t , dt , T , c ) ; 
  
  return 0;
}

