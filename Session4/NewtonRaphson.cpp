//Name: Jonathan Lim
//CRSid: zsjl2
//Session 4
//Program Title: NewtonRaphson.cpp


#include <iostream>
#include <cmath>

using namespace std;

//Function prototypes:
float prompt_loop_c();
float prompt_initial();
int prompt_loop_iteration();
float NRRoot(float square, float initial, int N);


//Driver function
int main(){
    float c, result, initial;
    int iteration;

    c = prompt_loop_c();
    initial = prompt_initial();
    iteration = prompt_loop_iteration(); 

    //Printing out test lines (for debugging purposes)
    cout << "Now finding the square root of " << c << " through the Newton-Raphson method." << endl;
    cout << "Initial guess = " << initial << " ; Number of iterations = " << iteration << endl;


    result = NRRoot(c, initial, iteration);
    cout<< "The square root of " << c << " is " << result << endl;
    return 0;
}


//Prompt-loop to retrieve and sanitise the input number
float prompt_loop_c(){
    float c_input;

    cout << "Welcome! This program uses the Newton-Raphson method to estimate the root of any number c, where c>=0. What is the value of c?" << endl;
    while (true) {
        cin >> c_input;
        if (c_input >= 0){
            break;
        }
        cout << "Input invalid! c must be greater than or equal to 0. Please try again." << endl;
    }
    return c_input;
}


//Prompts the user for an initial guess (real number)
float prompt_initial(){
    float initial;

    cout << "Please provide a real number as an initial guess." << endl;
    cin >> initial;
    return initial;
}


//Prompt-loop to retrieve and sanitise the input number of iterations.
int prompt_loop_iteration(){
    int iteration;

    cout << "What is the number of iterations that you would like? Note that this must be an integer that is greater than 0." << endl;
    while (true) {
        cin >> iteration;
        if (iteration > 0){
            break;
        }
        cout << "Input invalid! The number of iterations must be greater than 0. Please try again." << endl;
    }
    return iteration;
}



// Function that calculates the square root via the NR method
float NRRoot(float square, float initial, int N){
    float root, f_root, f_prime;

    root = initial;                          // Using the initial guess

    for (int i = 0; i < N; i++){             // N iterations (user-input defined)
        f_root = square - pow(root, 2);       // f(x) = c - x^2
        f_prime = -2*root;                   // f'(x) = -2x
        root = root - (f_root/f_prime);     // p(i+1) = p(i) - (f(p(i))/f'(p(i)))
        cout<< "Current iteration: " << i+1 << " ; Current estimate: " << root << endl;
    }
    return root;
}

