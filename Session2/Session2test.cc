//Name: Jonathan Lim
//CRSid: zsjl2
//Session 2
//Program Title: PrimeNumbers.cc
//Description: This program prints out all prime numbers less than or equal to N.


#include <iostream>
using namespace std;


//Helper method to print out all prime numbers less than or equal to N.

void PrimeNumbers(int input){
    for (int i = 2; i <= input; i++){
        bool isPrime = true;

        //Following loop repeatedly divides i by j, until it finds a factor or j reaches i.
        //Initialises  j=2 as i % 1 ==0 is true for all values of i
        for (int j = 2; j < i; j++){                    
            if (i % j == 0){                
                isPrime = false;
                break;    // A factor has been found so i is not prime.
            }
        }

        if (isPrime){
            cout << i << endl;
        }
    }
    return;
}


//Main function that takes the input from the command line terminal and calls the helper function.
int main(){

    int N;

    while(true){
        cout << "This program prints the series of prime numbers upto N. What is the value of N?" << endl;
        cin >> N;
        if (N <= 0){
            cout << "Term invalid! Your input has to be greater than 1. Please try again." << endl;
            continue;
        }

        cout << "These are the prime numbers from 2 to " << N << ":" << endl;
        PrimeNumbers(N);
        break;
    }
    
    cout << "The series has ended." << endl;
    return 0;
}

