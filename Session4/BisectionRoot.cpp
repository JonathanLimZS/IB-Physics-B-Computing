//Name: Jonathan Lim
//CRSid: zsjl2
//Session 4
//Program Title: BisectionRoot.cpp


#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

using namespace std;

//Function prototypes for call by reference:
float prompt_loop(float &c);
float MySquareRoot(float square);



int main(){
    float c, result;

    c = prompt_loop(c);

    //Printing out a test line (for debugging purposes)
    cout << "Now finding the square root of " << c << " ..." << endl;

    result = MySquareRoot(c);
    cout<< "The square root of " << c << " is " << result << endl;
    return 0;
}



float prompt_loop(float &c){

    //Prompt-loop for to retrieve and sanitise the input
    cout << "Welcome! This program estimates the square root of the number c, where 1<=c<=100. What is the value of c?" << endl;
    while (true) {
        cin >> c;
        if (c >= 1 && c <= 100){
            break;
        }
        cout << "Input invalid! c must be a number within the domain of 1 to 100. Please try again." << endl;
    }
    return c;
}


float MySquareRoot(float square){
    float lower, upper, root, sign, f_lower, f_estimate;
    lower = 0.1f;
    upper = 10.1f;
    for (int i = 0; i < 30; i++){
        root = (lower + upper) / 2.0f;
        f_lower = square - pow(lower, 2);
        f_estimate = square - pow(root, 2);
        sign = f_lower * f_estimate;
        if (sign < 0){
            upper = root;
        }
        else {
            lower = root;
        }
    }
    return root;
}