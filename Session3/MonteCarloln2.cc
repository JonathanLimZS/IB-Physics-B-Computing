//Name: Jonathan Lim
//CRSid: zsjl2
//Session 3
//Program Title: MonteCarloln2


// Usage: ./MonteCarloln2 [<N> [<Seed>]]
// e.g. ./MonteCarloln2 (run with default value of N and seed)
// ./MonteCarloln2 30 9237832 (Run with N=30 and seed=9237832)
// *gets parameters from the command line

// Uses a y=1/x function to estimate ln2. This is the area under graph from x=2 to x=4. 
// Bounding rectangular area for Monte Carlo integration will be x=2 to x=4, y=0 to y=0.5. This has an area of 1.
// N needs to be at least ~1000 to get a precision of 2 d.p. (rounded)

#include <iostream>
#include <iomanip>      // std::setprecision
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <unistd.h>

using namespace std;

#define ranf() \ 
    ((double)rand()/(1.0+RAND_MAX)) // Uniform from interval [0,1)

int main(int argc, char* argv[]){
    int outcome, N=10, count_in=0, seed = 123;
    double fraction_in, estimate;

    auto start = chrono::steady_clock::now();    //Added functionality of showing the elapsed time, just for the fun of it

    if (argc > 1) {
        sscanf(argv[1], "%d", &N) ; // put the first command-line argument in N
    }

    if (argc > 2) {
        sscanf(argv[2], "%d", &seed) ; // put the 2nd argument in seed
    }
    //Write out a summary of parameters
    cout << "# " << argv[0] << " N=" << N << " seed=" << seed << endl;

    //Initialise random number generator
    srand(seed);

    //Perform N experiments.
    for(int n=1; n<=N; n++){
        double x = 2 + 2*ranf();                //ranf() returns a real number in [0,1), so the expression yields a random number in [2, 4)
        double y = ranf() / 2;                  // range [0, 0.5)
        outcome = (x*y < 1.0) ? 1 : 0;
        count_in += outcome;
        //Integer variables must be converted (cast) for correct division
        fraction_in = static_cast<double>(count_in)/n;
        estimate = 1.0 * fraction_in;           //scale factor is 1.0 as the bounding box is a 0.5x2 rectangle

        double threshold = ranf();
        double prob = 1.0 - ((float)n/(float)N);    // casting to floats to do a floating-point division
        if (prob > threshold) {                  // prints a gradually decreasing fraction of the lines as n increases.
            cout << "Location" << outcome << "\t" << x << "\t" << y << "\t"
            << count_in << "\t" << n << "\t" << estimate << endl;   
        }
    }
    cout << setprecision(2) << "Final estimate of ln(2) is " << estimate << '\n';
    auto end = chrono::steady_clock::now();
    cout << "Elapsed time in seconds : " << chrono::duration_cast<chrono::seconds>(end - start).count() << " sec";
    return 0;
}