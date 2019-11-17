As the given exercise was on root approximation through the bisection method, I've decided to refactor my code to adopt the Newton-Raphson method instead, with added functionalities.

Firstly, control of the initial guess and the number of iterations is given to the user, to make this code more flexible instead of having a fixed number of iterations. Also, allowing the user to specify a clever guess is better than having a fixed guess, which could be far from the actual root.

Next, the estimate of the root after each iteration is printed to the screen, to demonstrate the speed of convergence of the Newton-Raphson method. Also, this allows the user to check which root that the method is converging towards, as there will be two roots (positive & negative) for every positive number. 

My solution is in a file called NewtonRaphson.cc