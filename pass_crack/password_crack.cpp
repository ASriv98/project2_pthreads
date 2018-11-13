#include "password_crack.h"

//Set size is 36 characters and one blank character
float setSize = 36;

int main() {
    char password[] = "aacbab";

    int possibleLen = strlen(password);

    printf("-Starting Non-Parallel Password Cracker-\n");

    // Start Timer
    std:: clock_t start;
    double duration;
    start = std::clock();

    // Loop through len 1 - possible len
    for (int currLen = 1; currLen <= possibleLen; ++currLen) {
    // Loop for all possible combinations
        char guess[currLen + 1];
  
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

    /*
    // Test crack function
    printf("-Testing function-\n");
    start = std::clock();

    struct params* args = new struct params;
    args->password = password;
    args->len = 5;

    char* pass_guess = (char*) crack((void*) args);

    printf("Returned Value: %s\n", pass_guess);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    printf("Time: %f\n", duration);

    */

    // Use Pthreads
    printf("-Starting Parallel PThread Cracker-\n");
    start = std::clock();

    pthread_t thread[possibleLen];

    for (int i = 0; i < possibleLen; i++){
      struct params* args = new struct params;
      args -> password = password;
      args -> len = i+1;
      pthread_create( &thread[i], NULL, crack, (void*) args);
    }

    //printf("All threads running\n");

    for (int i = 0; i < possibleLen; i++){
      char* pass_guess;
      pthread_join(thread[i], (void**) &pass_guess);
      if(pass_guess != NULL){
        printf("Returned Value: %s\n", pass_guess);
      }
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    printf("Time: %f\n", duration);

    return 0;
}

void* crack(void* args){
  // Get arguments
  struct params *params = (struct params*) args;
  char* password = params -> password;
  int currLen = params -> len;

  //printf("Values: %d\t %s\n", currLen, password);

  char* guess = new char[currLen+1];
  memset(guess, '\0', currLen+1);

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
      return (void*) guess;
    }
  }
  return NULL;
}


char map(int convert){
  if (convert < 10) {
    return (char) convert + 48;
  } else {
    return (char) convert + 87;
  }
}
