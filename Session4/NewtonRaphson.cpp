//Name: Jonathan Lim
//CRSid: zsjl2
//Session 4
//Program Title: NewtonRaphson.cpp


#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

using namespace std;

//Function prototypes for call by reference:
float prompt_loop(float &c);
float NRRoot(float square);



int main(){
    float c, result;

    c = prompt_loop(c);

    //Printing out a test line (for debugging purposes)
    cout << "Now finding the square root of " << c << " through the Newton-Raphson method..." << endl;

    result = NRRoot(c);
    cout<< "The square root of " << c << " is " << result << endl;
    return 0;
}



float prompt_loop(float &c){

    //Prompt-loop for to retrieve and sanitise the input
    cout << "Welcome! This program uses the Newton-Raphson method to estimate the root of any number c, where c>=0. What is the value of c?" << endl;
    while (true) {
        cin >> c;
        if (c >= 0){
            break;
        }
        cout << "Input invalid! c must be greater than or equal to 0. Please try again." << endl;
    }
    return c;
}


float NRRoot(float square){
    float root, f_root, f_prime;

    root = 1 + 0.5*square;                     //From the binomial expansion approximation (1+x)^n ~= 1 + nx for small x

    for (int i = 0; i < 30; i++){             // 100 iterations
        f_root = square - pow(root, 2);       // f(x) = c - x^2
        f_prime = -2*root;                   // f'(x) = -2x
        root = root - (f_root/f_prime);     // p(i+1) = p(i) - (f(p(i))/f'(p(i)))
        cout<< "Current iteration: " << i+1 << " ; Current estimate: " << root << endl;
    }
    return root;
}