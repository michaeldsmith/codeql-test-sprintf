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

void split_filename_with_poundsigns_into_parts( char* input_string_with_pound_signs, char* prefix, char* suffix, unsigned int* number_of_digits)
{
  // this function splits an input string with sequential pound signs into prefix, suffix and number of digits
  // for example test.####.tif will be split into prefix = test. suffix = .tif and number_of_digits = 4
  // find pound (#) signs in input string
  const char pound_signs[] = "################";

  *number_of_digits = 0;
  bool keep_searching = true;
  char* found_search_result = NULL;
  while (true == keep_searching)
  {
    char pound_signs_to_find[MAX_PATH_LENGTH] = { '\0' };
    strncpy(pound_signs_to_find, pound_signs, (size_t)(*number_of_digits) + 1);
    char* search_result = strstr(input_string_with_pound_signs, pound_signs_to_find);
    if (search_result)
    {
      //fprintf(stdout, "found %s in %s\n", pound_signs_to_find, search_result);
      found_search_result = search_result;
      *number_of_digits = *number_of_digits + 1;
    }
    else
    {
      //fprintf(stdout, "%s not found in %s\n", pound_signs_to_find, input_string_with_pound_signs);
      keep_searching = false;
    }

  };

  if (NULL != found_search_result)
  {
    //fprintf(stdout, "number_of_digits = %d in %s\n", *number_of_digits, found_search_result);
    // find prefix
    size_t number_of_prefix_chars = found_search_result - input_string_with_pound_signs;
    strncpy(prefix, input_string_with_pound_signs, number_of_prefix_chars);
    //fprintf(stdout, "prefix = %s\n", prefix);
    // find suffix
    size_t input_string_length = strlen(input_string_with_pound_signs);
    strncpy(suffix, &found_search_result[*number_of_digits], (size_t)(input_string_length - *number_of_digits));
    //fprintf(stdout, "suffix = %s\n", suffix);
  }
  
  return;
}

void make_filename(char* output_string, const char* prefix, const char* suffix, const unsigned int number_of_digits, const unsigned int frame_number)
{
  //char string_formatting[MAX_PATH_LENGTH] = { '\0' };
  //snprintf(string_formatting, sizeof(string_formatting), "%%s%%0%dd%%s", number_of_digits);

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
    split_filename_with_poundsigns_into_parts(args.input_filename, args.prefix, args.suffix, &args.number_of_digits);
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

