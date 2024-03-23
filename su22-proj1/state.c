#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_x(unsigned int cur_x, char c);
static unsigned int get_next_y(unsigned int cur_y, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);


/* Task 1 */
game_state_t* create_default_state() {
  // snake state
  snake_t* snake_state = (snake_t *)malloc(sizeof(snake_t));
  snake_state->head_x = 4;
  snake_state->head_y = 2;
  snake_state->tail_x = 2;
  snake_state->tail_y = 2;
  snake_state->live = true;

  // game state
  game_state_t* default_state = (game_state_t *)malloc(sizeof(game_state_t));
  default_state->num_rows = 18;
  default_state->num_snakes = 1;
  default_state->snakes = snake_state;

  // board state
  uint32_t num_row = default_state->num_rows;
  uint32_t num_col = 20;
  default_state->board = (char **)malloc(sizeof(char *) * num_row);
  for (int i = 0; i < num_row; i++)
  {
    default_state->board[i] = (char *)malloc(sizeof(char) * num_col);
    for (int j = 0; j < num_col; j++)
    {
      if (i == 0 || i == num_row - 1 || j == 0 || j == num_col - 1)
      {
        default_state->board[i][j] = '#';
      }
      else
      {
        default_state->board[i][j] = ' ';
      }
    }
  }

  default_state->board[2][9] = '*'; //set fruit
  default_state->board[2][2] = 'd'; //set snake tail
  default_state->board[2][4] = 'D'; //set snake head
  default_state->board[2][3] = '>'; //set snake body

  return default_state;
}


/* Task 2 */
void free_state(game_state_t* state) {
    for (int i = 0; i < state->num_rows; i++)
  {
    free(state->board[i]);
  }
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
    for (int i = 0; i < state->num_rows; i++)
  {
      fprintf(fp, "%s\n", state->board[i]);
    }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}


/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int x, unsigned int y) {
  return state->board[y][x];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch) {
  state->board[y][x] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
    return true;
  } else {
    return false;
  }
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  if (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x'){
    return true;
  } else {
    return false;
  }
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<>vWASDx"
*/
static bool is_snake(char c) {
  if (is_tail(c) || is_head(c) || c == '^' || c == '<' || c == '>' || c == 'v') {
    return true;
  } else {
    return false;
  }
}

/*
  Converts a character in the snake's body ("^<>v")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  switch(c) {
    case '^':
      return 'w';
    case '<':
      return 'a';
    case '>':
      return 'd';
    case 'v':
      return 's';
  }
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<>v").
*/
static char head_to_body(char c) {
  switch(c){
    case 'W':
      return '^';
    case 'A':
      return '<';
    case 'S':
      return 'v';
    case 'D':
      return '>';
  }
  return '?';
}

/*
  Returns cur_x + 1 if c is '>' or 'd' or 'D'.
  Returns cur_x - 1 if c is '<' or 'a' or 'A'.
  Returns cur_x otherwise.
*/
static unsigned int get_next_x(unsigned int cur_x, char c) {
  if (c == '>' || c == 'd' || c == 'D') {
    return cur_x + 1;
  } else if (c == '<' || c == 'a' || c == 'A') {
    return cur_x - 1;
  } else {
    return cur_x;
  }
}
  
/*
  Returns cur_y + 1 if c is '^' or 'w' or 'W'.
  Returns cur_y - 1 if c is 'v' or 's' or 'S'.
  Returns cur_y otherwise.
*/
static unsigned int get_next_y(unsigned int cur_y, char c) {
  if (c == '^' || c == 'w' || c == 'W') {
    return cur_y - 1;
  } else if (c == 'v' || c == 's' || c == 'S') {
    return cur_y + 1;
  } else {
    return cur_y;
  }
}

/*
  Task 4.2
  Helper function for update_state. Return the character in the cell the snake is moving into.
  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  unsigned int head_x = state->snakes[snum].head_x;
  unsigned int head_y = state->snakes[snum].head_y;
  char snake_head = get_board_at(state, head_x, head_y);
  unsigned int next_head_x = get_next_x(head_x, snake_head);
  unsigned int next_head_y = get_next_y(head_y, snake_head);
  return get_board_at(state,next_head_x,next_head_y);
}

/*
  Task 4.3
  Helper function for update_state. Update the head
  on the board: add a character where the snake is moving
  in the snake struct: update the x and y coordinates of the head
  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  unsigned int head_x = state->snakes[snum].head_x;
  unsigned int head_y = state->snakes[snum].head_y;
  char snake_head = get_board_at(state, head_x, head_y);
  set_board_at(state, head_x, head_y, head_to_body(snake_head));
  unsigned int next_head_x = get_next_x(head_x, snake_head);
  unsigned int next_head_y = get_next_y(head_y, snake_head);
  state->snakes[snum].head_x = next_head_x;
  state->snakes[snum].head_y = next_head_y;
  set_board_at(state, next_head_x, next_head_y, snake_head);
  return;
}


/*
  Task 4.4
  Helper function for update_state. Update the tail
  on the board: blank out the current tail, and change the new
  tail from a body character (^v<>) into a tail character (wasd)
  in the snake struct: update the x and y coordinates of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  unsigned int tail_x = state->snakes[snum].tail_x;
  unsigned int tail_y = state->snakes[snum].tail_y;
  char snake_tail = get_board_at(state, tail_x, tail_y);
  state->board[tail_y][tail_x] = ' ';
  unsigned int next_tail_x = get_next_x(tail_x, snake_tail);
  unsigned int next_tail_y = get_next_y(tail_y, snake_tail);
  state->snakes[snum].tail_x = next_tail_x;
  state->snakes[snum].tail_y = next_tail_y;
  char next_tail = body_to_tail(get_board_at(state, next_tail_x, next_tail_y));
  set_board_at(state, next_tail_x, next_tail_y, next_tail);
  return;
}


/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  for (unsigned int i = 0; i < state->num_snakes; i++) {
    if (state->snakes[i].live) {
      char next_char = next_square(state, i);
      if (is_snake(next_char) || next_char == '#') {
        set_board_at(state, state->snakes[i].head_x, state->snakes[i].head_y, 'x');
        state->snakes[i].live = false;
      } else {
        update_head(state, i);
        if (next_char == ' ') {
          update_tail(state, i);
        } else if (next_char == '*') {
          add_food(state);
        }
      }
    }
  }
  return;
}


/* Task 5 */
game_state_t* load_board(char* filename) {
  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    return NULL;
  }
  game_state_t *game_state = (game_state_t*)malloc(sizeof(game_state_t));
  game_state->board = (char**)malloc(sizeof(char*));
  int num_row = 0;
  int num_col = 0;
  int num_col_helper = 0;
  char character = getc(f);
  while (!feof(f)) {
    if (character == '\n') {
      num_row += 1;
      num_col_helper = 0;
    } else {
      num_col_helper += 1;
    }
    if (num_col_helper >= num_col) {
      num_col = num_col_helper;
    }
    character = getc(f);
  }
  fclose(f);
  game_state->num_rows = num_row;
  game_state->board = (char **)malloc(sizeof(char *) * num_row);
  f = fopen(filename, "r");
  for (int i = 0; i < num_row; i++)
  {
      char *row = (char *)malloc(sizeof(char)*(num_col + 1));
      fscanf(f, "%[^\n]%*c", row);
      game_state->board[i] = (char *)malloc(sizeof(char)*(num_col + 1));
      strcpy(game_state->board[i], row);
      free(row);
  }
  fclose(f);
  return game_state;
}


/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail coordinates filled in,
  trace through the board to find the head coordinates, and
  fill in the head coordinates in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  unsigned int snake_square_x = state->snakes[snum].tail_x;
  unsigned int snake_square_y = state->snakes[snum].tail_y;
  char snake_square = get_board_at(state, snake_square_x, snake_square_y);
  while (is_snake(snake_square) && !is_head(snake_square))
  {
    unsigned int helper_x = get_next_x(snake_square_x, snake_square);
    unsigned int helper_y = get_next_y(snake_square_y, snake_square);
    snake_square = get_board_at(state, helper_x, helper_y);
    if (is_snake(snake_square)) {
      snake_square_x = helper_x;
      snake_square_y = helper_y;
    }
  }
  state->snakes[snum].head_x = snake_square_x;
  state->snakes[snum].head_y = snake_square_y;
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  unsigned int snake = 0;
  state->snakes = (snake_t *)malloc(sizeof(snake_t));
  for (unsigned int i = 0; i < state->num_rows; i++) {
    for (unsigned int j = 0; j < strlen(state->board[i]); j++) {
      if (is_tail(state->board[i][j])) {
        state->snakes = (snake_t *)realloc(state->snakes, sizeof(snake_t)*(snake+1));
        state->snakes[snake].tail_x = j;
        state->snakes[snake].tail_y = i;
        state->snakes[snake].live = true;
        find_head(state, snake);
        snake += 1;
      }
    }
  }
  state->num_snakes = snake;
  return state;
}