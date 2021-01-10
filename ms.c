//  This program allows the user to calculate the duration of a beat in milliseconds
//  given a certain BPM value. Additional parameters can be added for calculating the
//  durations of finer subdivisions of the beat and dotted notes. 
//  The program states the note values for standard subdivisions (quarter note,
//  eighth note, etc.) but can calculate nonstandard subdivisions as well.
//  This calculation can be useful for setting time-based effects or plugins (delay,
//  reverb, etc.) in a DAW to be in sync with a song's tempo.
//
//  1 measure  (Whole Note)     = 60000 / bpm   / 0.25     
//             (Half Note)      = (60000 / bpm) / 0.5                                                        
//  1 beat     (Quarter Note)   = (60000 / bpm)                              
//             (8th)            = (60000 / bpm) / 2     
//             (16th)           = (60000 / bpm) / 4     
//             (32nd)           = (60000 / bpm) / 8     
//             (64th)           = (60000 / bpm) / 16    
//
//             (Dotted note)    = ((60000 / bpm) + (60000 / bpm) / 2) 
//  v. 0.6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>

const char *argp_program_version = "ms 0.6";
const char *argp_program_bug_address = "<bruno.pezer@tutanota.com>";

//  Program documentation
static char doc[] = "This program calculates the duration of a beat or note value in milliseconds.\nThe BPM value is required for the program to run correctly and can be entered as a whole or decimal number.\nThe options below are available to calculate specific note durations for the provided BPM.\nOptions can be concatenated (eg. -ehqd), however only the last of the note-length options will be considered\nin the calculation. Use example: write \"ms 120 -ed\" to receive the duration in milliseconds of a dotted eigth note at 120BPM.\n";

//  Description of the accepted arguments
static char args_doc[] = "BPM";

//  Accepted options
static struct argp_option options[] = {
    {"whole", 'w', 0, 0, "Whole note", 0},
    {"half", 'h', 0, 0, "Half note"},
    {"quarter", 'q', 0, 0, "Quarter note"},
    {"eighth", 'e', 0, 0, "Eighth note"},
    {"sixteenth", 's', 0, 0, "Sixteenth note"},
    {"thirtysecond", 't', 0, 0, "Thirtysecond note"},
    {"sixtyfourth", 'x', 0, 0, "Sixtyfourth note"},
    {"dot", 'd', 0, 0, "Dotted", 1},
    {0}
};

//  Used by main to communicate with parse_opt
struct arguments {
    char *args;
    int arg_count;
    double bpm, subdivs;
    int dotFlag;
};

int subdivToNote(double subdiv) {
    int note, key;
    
    //  Special cases for half and whole notes
    //if (subdiv == 0.5) key = 5;
    //else if (subdiv == 0.25) key = 25;
    else key = (int) subdiv; 

    switch (key) {
        case 1:
            note = 4;
            break;
        case 2:
            note = 8;
            break;
        case 4:
            note = 16;
            break;
        case 8:
            note = 32;
            break;
        case 16:
            note = 64;
            break;
        /*
        case 32:
            note = 128;
            break;
        case 64:
            note = 256;
            break;
        */
        default:
            printf("Unknown subdivision. ");
            note = -1;
            break;
    }

    return note;
}

//  Calculates the length of the beats parameter in milliseconds
double calculateMs(double bpm, double subdivs) {
    return (double) ((60000 / bpm) / subdivs);
}

//  Parse a single option
static error_t
parse_opt(int key, char *arg, struct argp_state *state) {
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

        case 'd':
            arguments->dotFlag = 1;
            break;
        case ARGP_KEY_ARG:
            arguments->bpm = strtod(arg, &eptr);
            break;
        case ARGP_KEY_INIT:
            //  Default values
            arguments->args = arg;
            arguments->subdivs = 1;
            arguments->dotFlag = 0;
            break;
        case ARGP_KEY_END:
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
    char *eptr;

    //printf("%s %d\n", "count", arguments.arg_count);  //  For assistance in establishing command line argument limits during development

    if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0) {
        if (!arguments.dotFlag) {
            printf("%.2fms: %dth note at %.2fBPM\n", calculateMs(arguments.bpm, arguments.subdivs), subdivToNote(arguments.subdivs), arguments.bpm);
        } else printf("%.2fms: dotted %dth note at %.2fBPM\n", calculateMs(arguments.bpm, arguments.subdivs) + (calculateMs(arguments.bpm, arguments.subdivs) / 2), subdivToNote(arguments.subdivs), arguments.bpm);
    }       
}
