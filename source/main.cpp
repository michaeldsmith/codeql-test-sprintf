#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _commandline_args
{
  char input_filename[512];
  unsigned int start_frame;
  unsigned int end_frame;
  bool is_process_framerange;
} commandline_args_t;

void print_usage(int argc, char* argv[])
{
  fprintf(stderr, "USAGE: %s input_filename\n", argv[0]);
  fprintf(stderr, "OR\n");
  fprintf(stderr, "USAGE: %s input_filename start_frame end_frame\n", argv[0]);
  fprintf(stderr, " - use printf() style formating for input sequence - i.e. input.%%06d.tif\n");

  return;
}

int main( int argc, char* argv[])
{
    if( argc < 2 )
    {
      print_usage(argc, argv);
      exit(-1);
    }
    commandline_args_t args;
    memset( &args, 0, sizeof(commandline_args_t) );

    if( argc == 2)
    {
        strcpy(args.input_filename,argv[1]);
    }
    else if( argc == 4)
    {
        strcpy(args.input_filename,argv[1]);
        args.start_frame = atoi( argv[2] );
        args.end_frame = atoi( argv[3] );
        args.is_process_framerange = true;
    }
    else
    {
      print_usage(argc, argv);
      exit(-1);
    }
    
    char input_filename[512] = { '\0' };
    if( true == args.is_process_framerange )
    {
        unsigned int number_of_frames_to_process = args.end_frame - args.start_frame + 1;
        for( unsigned int frame_index = 0; frame_index < number_of_frames_to_process; frame_index++)
        {
            sprintf(input_filename, args.input_filename, frame_index + args.start_frame);
            fprintf( stdout, "input_filename = %s\n", input_filename );
        }
        
    }
    else
    {
        strcpy( input_filename, args.input_filename );
        fprintf( stdout, "input_filename = %s\n", input_filename );
    }
    
    return 0;
}

