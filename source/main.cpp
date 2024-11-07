#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

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

// https://www.ibm.com/docs/en/i/7.4?topic=functions-vsnprintf-print-argument-data-buffer
void make_filename_va(char* output_string, size_t output_string_length, char* formatted_string, ...)
{
  va_list arg_ptr;

  va_start(arg_ptr, formatted_string);
  vsnprintf(output_string, output_string_length, formatted_string, arg_ptr);
  va_end(arg_ptr);

  return;
}

// https://www.ibm.com/docs/en/i/7.4?topic=functions-vsnprintf-print-argument-data-buffer
void make_filename(char* output_string, char* input_string_with_pound_signs, const unsigned int frame_number )
{
  // find pound (#) signs in input string
  char pound_signs[] = "############";
  
  unsigned int number_of_digits = 0;
  bool keep_searching = true;
  while (true == keep_searching)
  {
    number_of_digits++;
    char pound_signs_to_find[32] = { '\0' };
    strncpy(pound_signs_to_find, pound_signs, number_of_digits);
    char* p = strstr(input_string_with_pound_signs, pound_signs_to_find);
    if (p)
    {
      fprintf(stdout, "found %s is %s\n", pound_signs_to_find, p);
    }
    else
    {
      fprintf(stdout, "%s not found in %s\n", pound_signs_to_find, input_string_with_pound_signs);
      break;
    }
    
  };

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
            //snprintf(input_filename, sizeof(input_filename), args.input_filename, frame_index + args.start_frame);
            make_filename_va(input_filename, sizeof(input_filename), args.input_filename, frame_index + args.start_frame);
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

