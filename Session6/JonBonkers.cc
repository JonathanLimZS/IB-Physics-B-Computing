//Name: Lim Zheng Sheng, Jonathan
//CRSid: zsjl2
//Program name: JonBonkers.cc

//   bonkers bonking in a one-dimensional box
//   usage:
//      Bonkers1 [N [dt [tframe(ms) [filename]]]]
//
//   by default, writes to /tmp/1
//   movie can be made by gnuplot
//         load 'gnu10'

#include <iostream>
#include <fstream> 
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <time.h>

using namespace std;
 
 //OOP representation of a particle
struct particle {
  double x    ; // (x,y) coordinates
  double p    ; // momentum
  double im   ; // inverse mass
  double v    ; // velocity
  double T    ; // kinetic energy
  double a    ; // radius of particle
} ; 

//OOP approach to controlling the print
struct control {
  int verbose    ; // program verbosity
  int printing   ; // period with which to print
  int commanding ; // period with which to issue commands
  int time_in_ms ; // time between frames (commands)
  clock_t latest_time;
  ofstream fout  ; 
};

// Implement x += v dt for one particle via pass by reference
void PositionStep ( particle &a , double dt )
{ 
  a.x += dt * a.v ; 
}

//Propagate the changed velocity into the momentum vector
void v2p( particle &a )
{
  a.p =  a.v / a.im ;
}

//Propagate the changed momentum into the velocity vector
void p2v ( particle &a ) {
  a.v  = a.p * a.im ;
}

//Propagate the changed momentum & velocity into KE
void pv2T ( particle &a ) {
  a.T = 0.5*a.v * a.p ; 
}

// 2 particles collide elastically (pass by ref)
void collide ( particle & a , particle & b  )
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


//Finding the next collision time. Input has an array of particles
double nct( particle *a , int NN , int &whichn ) {
  double dt = 1e100 ; 
  // examine all adjacent pairs from 0,1   to  NN-2,NN-1
  for ( int n = 0 ; n < NN-1 ; n ++ ) {
    double relative_velocity = a[n].v - a[n+1].v ;
    if(relative_velocity > 0.0) {
      double collision_time =  ((a[n+1].x-a[n+1].a) - (a[n].x+a[n].a))  //Separation between the surfaces of the particles/rel velocity
	                        /relative_velocity ;
      if ( collision_time < dt ) {
	dt = collision_time ;
	whichn = n ;    //Particle that is gonna collide with the one next to it 
      }
    }
  }
  return dt ;
}

//Moving all particles in the array forward in time
void leapForward(  particle *a , int NN , double dt ) {
  for( int n = 0 ; n < NN ; n ++ ) 
    PositionStep( a[n] , dt ) ; 
}

//Printing the coordinates and velocity of all of the particles
void showState ( particle *a , int n0 ,  int NN, ostream &fout )
{
  for( int n = n0 ; n < NN ; n ++ ) {
      fout << "\t"<<a[n].x;    // \t corresponds to a horizontal tab. This prints position
      fout << "\t"<<a[n].v;    // This prints velocity
  }
  fout << endl;
}


//Calculating the total KE of the system of particles
double kineticEnergy ( particle *a , int NN )
{
  double E = 0.0 ; 
  for( int n=0 ; n < NN ; n ++ ) {
      if ( a[n].im > 0.0 ) // avoid infinitely massive objects
	      E+=0.5*a[n].v*a[n].v/a[n].im;
  }
  return E ;
}


// Simulating the system
void  simulateBonkers( particle *a , int NN ,
		       double &t , double dt , double T ,
		       control &c ) {
  double next_print_dt = 0.0, next_collision_dt ;
  int whichn ;
  int we_are_printing_this_time = 1 ; 
  for(;t<=T;) {
    if( we_are_printing_this_time ) {
      c.fout << t << "\t" << kineticEnergy(a,NN) ; //t is the time elapsed
      showState ( a , 0 , NN , c.fout ) ;
      next_print_dt = dt ;
    }
    // find the next event
    next_collision_dt = nct( a , NN , whichn ) ;
    // ^^ this returns the time, and sets 'whichn'
    if ( next_collision_dt < next_print_dt ) {
      // advance time to that event, have a collision
      leapForward( a ,  NN , next_collision_dt ) ;
      t += next_collision_dt ;
      next_print_dt -= next_collision_dt ;
      collide( a[whichn] , a[whichn+1] ) ;
      we_are_printing_this_time = 0 ; 
    } else {
      leapForward( a , NN , next_print_dt ) ;
      t += next_print_dt ;
      we_are_printing_this_time = 1 ; 
    }
  }
}


//Main function that calls the simulating function
int main(int argc, char* argv[])
{
  int N = 4    ; // number of particles
  double T =50.0  ; // target time
  particle   *a ; // Declaring an array of particles
  control    c ; // Declaring a control structure
  char filename[256]="/tmp/1" ;
  // set defaults
  double dt = 0.03 ;
  double t = 0.0 ;
  c.verbose = 1 ;
  c.time_in_ms = 100; // real time between plots (in ms)

  // read in any command-line arguments
  if(argc>1)   {
    sscanf( argv[1], "%d", &N ) ; // put the first command-line argument in N
  }
  if(argc>2) {
    sscanf( argv[2], "%lf", &dt ) ; // put the 2nd argument in dt    (%lf refers to a long float/double)
  }
  if(argc>3) {
    sscanf( argv[3], "%d", &(c.time_in_ms) ) ;
  }
  if(argc>4) {
    strncpy(filename, argv[4], 256 );
  }

  // try to write output file
  c.fout.open(filename);
  if(c.fout.good()==false){
    cerr << "can't write to file " << filename << endl;
    exit(0);
  }
  
  a = new particle[N+2] ; // 0 (wall) , 1...N , N+1 (wall)
  // two walls
  for ( int n=0 ; n <= N+1 ; n += N+1 ) {
    a[n].im = 0.0 ; 
    a[n].v  = 0.0 ;
    a[n].x  = 1.0*n ;
    a[n].a  = 0.0   ; // radius
  }

  // define some masses, positions, and velocities of the particles (excluding the walls)
  for ( int n=1 ; n <= N ; n ++ ) {
    // a[n].im    = 1.0/n;
    if (n < 5){ a[n].im    = n * 1.0 / 100.0 ; }
    else {a[n].im = 1.0 * n; }   // right side are light compared to LHS
    a[n].v     = 1.0 ;    //all particles start with a different velocity
    a[n].x     = 1.0*n ;
    a[n].a     = 0.1   ; // radius
    cerr<<"mass: "<<1.0/a[n].im<<endl;
  }
  for ( int n=0 ; n <= N+1 ; n ++ ) {  //Initial momentum for all particles
    v2p(a[n]);
  }
  simulateBonkers( a , N+2 , t , dt , T , c ) ; 
  
  return 0;
}

