As I have solved the ln(2) exercise by myself, this is the submission of my solution to this problem.

I've noticed that the integral of 1/x is ln(x), and hence the area under the y=1/x curve from x=2 to x=4 will be equal to ln(2). This is because
ln(2) = ln(4) - ln(2). Hence, a Monte Carlo integration of that region was performed, with the bounding rectangular box set from x=2 to x=4, y=0 to y=0.5. 

For added fun and functionality, I've utilised the <chrono> library to keep track of and print the time elapsed for my code to run. 
True enough, the time elapsed scaled roughly proportionally to the size of N, giving an O(N) time complexity for this algorithm.

I suspect that a faster-converging algorithm would be to chop up the rectangular box into thinner vertical strips, with the height of each rectangular strip set at the maximum value of (1/x) within that particular strip. This is so that the ratio of points 'hit' within each strip will converge towards the actual ratio quicker, in terms of decreasing the size of N required for a given precision. However, since Monte Carlo has to be done on every strip, I reckon that the time complexity will remain the same.  

My solution is in a file called MonteCarloln2.cc