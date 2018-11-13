#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <ctime>

#include <unistd.h>

char map(int convert){
  if (convert < 10) {
    return (char) convert + 48;
  } else {
    return (char) convert + 87;
  }
}


int main() {

    //Create a file

    char password[] = "aacba";

    const float possibleLen = strlen(password);

    //Set size is 36 characters and one blank character
    const float setSize = 36;

    printf("-Starting Non-Parallel Password Cracker-\n");

    // Start Timer
    std:: clock_t start;
    double duration;
    start = std::clock();

    // Loop through len 1 - possible len
    for (int currLen = 1; currLen <= possibleLen; ++currLen) {
    // Loop for all possible combinations
      for (int currChar = 0; currChar <= (pow(setSize, (float) currLen)); ++currChar) {
        char guess[5];
        // Set guess
        for (int guessIndex = 0; guessIndex < currLen; ++guessIndex) {
          char temp = map((currChar / (int) pow(setSize, guessIndex)) % (int) setSize);
          guess[guessIndex] = temp;
        }

        //printf("Iteration: %d\tGuess: %s\n", currChar, guess);

        // Check if it compares
        if (strcmp(password, guess) == 0) {
          printf("Match Found Single!! \nLen: %d\tIteration: %d\tGuess: %s\n",currLen, currChar, guess);
          break;
        }
      }
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    printf("Time: %f\n", duration);
    
    return 0;
}
