//Name: Jonathan Lim
//CRSid: zsjl2
//Session 1
//Program Title: Fibonacci.cc
//Description: This program prints out the nth term of the Fibonacci Sequence, which goes like 1,1,2,3,5...


#include <iostream>
#include <chrono>
using namespace std;

//Helper method that computes the nth term of the Fibonacci Sequence
int FibSequence(int input){
    if (input == 1 || input == 2) {
        return 1;
    }
    int term = 0;
    int previous1 = 1;
    int previous2 = 1;
    for (int i=3; i <= input; i++){
        term = previous1 + previous2 ;
        previous2 = previous1;
        previous1 = term;
    }
    return term;
}


//Main function that takes the input from the command line terminal and calls the helper function.
int main(){

    int a, input, answer;

    while(true){
        cout << "Which term of the Fibonacci sequence would you like?" << endl;
        cin >> a;
        if (a <= 0){
            cout << "Term invalid! Your input has to be greater than 0. Please try again." << endl;
            _sleep(2000);     //Pausing the program for 2 seconds to make it more user-friendly
            continue;
        }
        answer = FibSequence(a);
        break;
    }
    cout << "Term " << a << " of the Fibonacci Sequence is " << answer << endl; 
    return answer;
}

