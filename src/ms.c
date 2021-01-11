//  Small command line utility allowing the user to calculate the duration of a note in milliseconds by providing a BPM and note value.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>

const char *argp_program_version = "ms 0.7";
const char *argp_program_bug_address = "<bruno.pezer@tutanota.com>";

//  Program documentation
static char doc[] = "\nThis program calculates the duration of a beat or note value in milliseconds.\nThe BPM value is required for the program to run correctly and can be entered as a whole or decimal number.\nThe options below are available to calculate specific note durations for the provided BPM.\nOptions can be concatenated (eg. \"-ehqd\"), however only the last of the note-length options will be considered. As such, concatenation is intended to specify that the note should be dotted (eg. write \"-sd\" for a dotted sixteenth note rather than \"-s -d\").\nUsage example: write \"./ms 120 -ed\" to receive the duration in milliseconds of a dotted eigth note at 120BPM, or \"./ms 100\" to get a quarter note at 100BPM, etc.\nThis calculation can be useful for setting time-based effect (delay, reverb, etc.) or plugin parameters in a DAW to be in sync with a song's tempo.\n";

//  Description of the accepted arguments
static char args_doc[] = "bpm";

//  Accepted options
static struct argp_option options[] = {
    {"whole", 'w', 0, 0, "Whole note", 0},
    {"half", 'h', 0, 0, "Half note"},
    {"quarter", 'q', 0, 0, "Quarter note"},
    {"eighth", 'e', 0, 0, "Eighth note"},
    {"sixteenth", 's', 0, 0, "Sixteenth note"},
    {"thirty-second", 't', 0, 0, "Thirty-second note"},
    {"sixty-fourth", 'x', 0, 0, "Sixty-fourth note"},
    {"hundred-twenty-eighth", 128, 0, 0, "Hundred twenty-eighth note"},
    {"two-hundred-fifty-sixth", 256, 0, 0, "Two hundred fifty-sixth note"},
    {"dot", 'd', 0, 0, "Dotted", 1},
    {0}
};

//  Used by main to communicate with parse_opt
//  Holds all the data provided by the command 
//  line and used by the program.
struct arguments {
    char *args;
    int arg_count;
    double bpm, subdivs;
    int dotFlag;
};

//  Return the note value as a string based on the numeric subdivision of the measure
char* subdiv_to_note(double subdiv) {
    char *note;
    int key;
    
    //  Special cases for half and whole notes
    //  These cases require a satisfying short option in the options array
    if (subdiv == 0.5) key = 5;
    else if (subdiv == 0.25) key = 25;
    else key = (int) subdiv; 

    switch (key) {
        case 25:
            note = "whole";
            break;
        case 5:
            note = "half";
            break;
        case 1:
            note = "quarter";
            break;
        case 2:
            note = "eighth";
            break;
        case 4:
            note = "sixteenth";
            break;
        case 8:
            note = "thirty-second";
            break;
        case 16:
            note = "sixty-fourth";
            break;
        case 32:
            note = "hundred-twenty-eighth";
            break;
        case 64:
            note = "two hundred fifty-sixth";
            break;
        default:
            printf("Unknown subdivision. ");
            note = "unknown";
            break;
    }

    return note;
}

//  Calculates the length of the note in milliseconds
double note_dur(double bpm, double subdivs) {
    return (60000 / bpm) / subdivs;
}

//  Parse a single option
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    char *eptr;
   
    switch (key) {
        case 'w':
            arguments->subdivs = 0.25;
            break;
        case 'h':
            arguments->subdivs = 0.5;
            break;           
        case 'q':
            arguments->subdivs = 1;
            break;
        case 'e':
            arguments->subdivs = 2;
            break;
        case 's':
            arguments->subdivs = 4;
            break;
        case 't':
            arguments->subdivs = 8;
            break;
        case 'x':
            arguments->subdivs = 16;
            break;
        case 128:
            arguments->subdivs = 32;
            break;
        case 256:
            arguments->subdivs = 64;
            break;
        case 'd':
            arguments->dotFlag = 1;
            break;
        case ARGP_KEY_ARG:
            //  Reads the BPM value from the input
            arguments->bpm = strtod(arg, &eptr);
            break;
        case ARGP_KEY_INIT:
            //  Default values
            arguments->args = arg;
            arguments->subdivs = 1;
            arguments->dotFlag = 0;
            break;
        case ARGP_KEY_END:
            //  Handles situations in which the user omits a required argument (the BPM) or adds too many options
            if (arguments->arg_count >= 5) argp_failure(state, 1, 0, "Too many arguments");
            else if (arguments->arg_count < 2) argp_failure(state, 1, 0, "Too few arguments");
            break;
    }

    return 0;
}

//  argp parser
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[]) {
    struct arguments arguments;
    arguments.arg_count = argc;
    //printf("%s %d\n", "count", arguments.arg_count);  //  For assistance in establishing command line argument limits during development

    //  Parses the user input from the command line
    if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0) {
        if (!arguments.dotFlag) {
            printf("%.2fms: %s note at %.2fBPM\n", note_dur(arguments.bpm, arguments.subdivs), subdiv_to_note(arguments.subdivs), arguments.bpm);
        } else printf("%.2fms: dotted %s note at %.2fBPM\n", note_dur(arguments.bpm, arguments.subdivs) + (note_dur(arguments.bpm, arguments.subdivs) / 2), subdiv_to_note(arguments.subdivs), arguments.bpm);
    }       
}
