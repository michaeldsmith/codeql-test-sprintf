#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define MAX_PATH_LENGTH 512

typedef struct _commandline_args
{
  char input_filename[MAX_PATH_LENGTH];
  unsigned int start_frame;
  unsigned int end_frame;
  bool is_process_framerange;
  char prefix[MAX_PATH_LENGTH];
  char suffix[MAX_PATH_LENGTH];
  unsigned int number_of_digits;
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

void split_filename_with_symbols_into_parts( char* input_string_with_symbols_to_find, char* prefix, char* suffix, unsigned int* number_of_digits)
{
  // this function splits an input string with sequential pound signs into prefix, suffix and number of digits
  // for example test.####.tif will be split into prefix = test. suffix = .tif and number_of_digits = 4
  // find pound (#) signs in input string
  const char* symbol_to_find = "#";

  *number_of_digits = 0;
  bool keep_searching = true;
  char* found_search_result = NULL;
  while (true == keep_searching && *number_of_digits < 10 )
  {
    char symbols_to_find[MAX_PATH_LENGTH] = { '\0' };
    for (size_t i = 1; i <= (size_t)*number_of_digits + 1; i++)
    {
      strncat(symbols_to_find, symbol_to_find, 1);
    }
    char* search_result = strstr(input_string_with_symbols_to_find, symbols_to_find);
    if (search_result)
    {
      found_search_result = search_result;
      *number_of_digits = *number_of_digits + 1;
    }
    else
    {
      keep_searching = false;
    }

  };

  if (*number_of_digits > 9)
  {
    fprintf(stderr, "WARNING on line %d of file %s in function %s:\n number_of_digits = %d is greater than 9\n",
      __LINE__, __FILE__, __FUNCTION__, *number_of_digits);
  }

  if (NULL != found_search_result)
  {
    // find prefix
    size_t number_of_prefix_chars = found_search_result - input_string_with_symbols_to_find;
    strncpy(prefix, input_string_with_symbols_to_find, number_of_prefix_chars);
    // find suffix
    size_t input_string_length = strlen(input_string_with_symbols_to_find);
    strncpy(suffix, &found_search_result[*number_of_digits], (size_t)(input_string_length - *number_of_digits));
  }
  else
  {
    // since no symbols were found, just copy the filename into prefix
    size_t number_of_prefix_chars = strnlen(input_string_with_symbols_to_find, MAX_PATH_LENGTH);
    strncpy(prefix, input_string_with_symbols_to_find, number_of_prefix_chars);
  }
  
  return;
}

void make_filename(char* output_string, const char* prefix, const char* suffix, const unsigned int number_of_digits, const unsigned int frame_number)
{
  size_t max_string_copy = sizeof(prefix) + sizeof(suffix) + (size_t)number_of_digits;

  if (number_of_digits > 0)
  {
    char string_formatting[MAX_PATH_LENGTH] = { '\0' };
    snprintf(string_formatting, sizeof(string_formatting), "%%s%%0%dd%%s", number_of_digits);

    snprintf(output_string, max_string_copy, string_formatting, prefix, frame_number, suffix);
  }
  else
  { 
    snprintf(output_string, max_string_copy, "%s%s", prefix, suffix);
  }

#if 0
  switch (number_of_digits)
  {
  case 0:
    sprintf(output_string, "%s%s", prefix, suffix); break;
  case 1:
    sprintf(output_string, "%s%01d%s", prefix, frame_number, suffix); break;
  case 2:
    sprintf(output_string, "%s%02d%s", prefix, frame_number, suffix); break;
  case 3:
    sprintf(output_string, "%s%03d%s", prefix, frame_number, suffix); break;
  case 4:
    sprintf(output_string, "%s%04d%s", prefix, frame_number, suffix); break;
  case 5:
    sprintf(output_string, "%s%05d%s", prefix, frame_number, suffix); break;
  case 6:
    sprintf(output_string, "%s%06d%s", prefix, frame_number, suffix); break;
  case 7:
    sprintf(output_string, "%s%07d%s", prefix, frame_number, suffix); break;
  case 8:
    sprintf(output_string, "%s%08d%s", prefix, frame_number, suffix); break;
  case 9:
    sprintf(output_string, "%s%09d%s", prefix, frame_number, suffix); break;
  default:
    fprintf(stderr, "ERROR on line %d of file %s in function %s: number_of_digits = %d, this should be between 0-9\n",
      __LINE__, __FILE__, __FUNCTION__, number_of_digits);
    break;
  }
#endif


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
    split_filename_with_symbols_into_parts(args.input_filename, args.prefix, args.suffix, &args.number_of_digits);
    fprintf(stdout, "input_filename = %s\n", args.input_filename);
    fprintf(stdout, "prefix = %s\n", args.prefix);
    fprintf(stdout, "suffix = %s\n", args.suffix);
    fprintf(stdout, "number_of_digits = %d\n", args.number_of_digits);
    
    char input_filename[512] = { '\0' };
    if( true == args.is_process_framerange )
    {
        unsigned int number_of_frames_to_process = args.end_frame - args.start_frame + 1;
        for( unsigned int frame_index = 0; frame_index < number_of_frames_to_process; frame_index++)
        {
            //snprintf(input_filename, sizeof(input_filename), args.input_filename, frame_index + args.start_frame);
            //make_filename_va(input_filename, sizeof(input_filename), args.input_filename, frame_index + args.start_frame);
            make_filename(input_filename, args.prefix, args.suffix, args.number_of_digits, frame_index);
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

