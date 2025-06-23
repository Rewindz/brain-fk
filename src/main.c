#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define CELL_LENGTH 512

uint8_t cells[CELL_LENGTH];
uint16_t cell_ptr;

void run_cmd(const unsigned char cmd)
{
  switch(cmd){
    case '+':
      cells[cell_ptr]++;
      break;
    case '-':
      cells[cell_ptr]--;
      break;
    case '<':
      if(cell_ptr > 0)cell_ptr--;
      break;
    case '>':
      if(cell_ptr < CELL_LENGTH)cell_ptr++;
      break;
    case '.':
      putchar(cells[cell_ptr]);
      break;
    case ',':
      cells[cell_ptr] = getchar();
      break;
    default:
      break;
    }

    return;
}

void run_loop(const char *cmds)
{
  uint16_t cmds_pos = 0;
  while(cmds[cmds_pos])
    run_cmd(cmds[cmds_pos++]);
  
  if(!cells[cell_ptr])
    return;
  else
    run_loop(cmds);
}

//TODO: NESTED LOOPS
void parse_brainfk(FILE *src)
{
  int res = fgetc(src);
  if(res == EOF)
    return;

  uint8_t in_loop = 0;
  uint8_t skip_loop = 0;

  char *loop_cmds = calloc(512, sizeof(char));
  uint16_t loop_pos = 0;
  
  do{
    unsigned char res_c = (unsigned char) res;

    if(in_loop){
      switch(res_c){
      case '+':
      case '-':
      case '<':
      case '>':
      case '.':
      case ',':
	loop_cmds[loop_pos++] = res_c;
	break;
      case ']':
	in_loop = 0;
	loop_pos = 0;
	run_loop(loop_cmds);
	memset(loop_cmds, 0, sizeof(char)*512);
	break;
      }
      continue;
    }

    if(skip_loop){
      if(res_c == ']')
	skip_loop = 0;
      else
	continue;
    }
    
    switch(res_c){
    case '+':
    case '-':
    case '<':
    case '>':
    case '.':
    case ',':
      run_cmd(res_c);
      break;
    case '[':
      if(cells[cell_ptr] > 0)
	in_loop = 1;
      else
	skip_loop = 1;
      break;
    default:
      break;
    }   

  }while( (res = fgetc(src)) != EOF);

  free(loop_cmds);
  
}



int main(int argc, char **argv)
{
  if(argc <= 1){
    printf("Usage: %s [source]\n", argv[0]);
    return 1;
  }
  
  FILE *src_file = fopen(argv[1], "r");
  if(!src_file){
    fprintf(stderr, "Could not open file %s!\n", argv[1]);
    return 1;
  }
  memset(cells, 0, CELL_LENGTH);
  cell_ptr = 0;
  parse_brainfk(src_file);

  fclose(src_file);
  return 0;
}
