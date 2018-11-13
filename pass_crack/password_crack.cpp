#include "password_crack.h"

int main() {
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
      char guess[currLen];
      for (int currChar = 0; currChar <= (pow(setSize, (float) currLen)); ++currChar) {
        // Set guess
        for (int guessIndex = 0; guessIndex < currLen; ++guessIndex) {
          char temp = map((currChar / (int) pow(setSize, guessIndex)) % (int) setSize);
          guess[guessIndex] = temp;
        }
        //printf("Iteration: %d\tGuess: %s\n", currChar, guess);

        // Check if it compares
        if (strcmp(password, guess) == 0) {
          printf("Match Found Single!! \nLen: %d\tGuess: %s\n",currLen, guess);
          break;
        }
      }
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    printf("Time: %f\n", duration);
    
    return 0;
}


char map(int convert){
  if (convert < 10) {
    return (char) convert + 48;
  } else {
    return (char) convert + 87;
  }
}
