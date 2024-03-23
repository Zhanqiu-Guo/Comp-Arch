#include <stdio.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

int main(int argc, char* argv[]) {
  char* in_filename = NULL;
  char* out_filename = NULL;
  game_state_t* state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      in_filename = argv[i + 1];
      i++;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */

  // Read board from file, or create default board
  if (in_filename != NULL) {
    // TODO: Load the board from in_filename
    FILE *f = fopen(in_filename, "r");
    if (f) {
      state = load_board(in_filename);
      state = initialize_snakes(state);
    } else {
      return -1;
    }
    fclose(f);
    // TODO: If the file doesn't exist, return -1
    // TODO: Then call initialize_snakes on the state you made
  } else {
    state = create_default_state();
    // TODO: Create default state
  }

  // TODO: Update state. Use the deterministic_food function
  // (already implemented in state_utils.h) to add food.
  update_state(state, deterministic_food);

  // Write updated board to file or stdout
  if (out_filename != NULL) {
    // TODO: Save the board to out_filename
    FILE *f = fopen(out_filename, "w");
    for (int i = 0; i < state->num_rows; i++)
        {
            fprintf(f, "%s\n", state->board[i]);
        }
        fclose(f);
  } else {
    // TODO: Print the board to stdout
    for (int i = 0; i < state->num_rows; i++)
        {
            printf("%s\n", state->board[i]);
        }
  }
  free_state(state);
  return 0;
}
