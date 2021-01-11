# ms
<i>ms</i> is a simple command line program which calculates musical note duration in milliseconds.
The program is intended to work like a typical command, with the user specifying a mandatory BPM value (as an integer or double)
and two optional commands: one for a note value (whole note through two hundred fifty-sixth note) and one to
specify that the note should be dotted. The syntax follows standard Linux commands with short and long options.
Type <b>./ms --help</b> or <b>./ms -?</b> to view the full list of options. If only the BPM value is provided the
program will calculate the duration of a standard quarter note.

## Purpose
The purpose of this program is to provide a quick and easy reference to the durations of standard
note values, for example to set parameters of time-based effects such as reverb and delay so that
the tails and echoes stay in sync with the tempo of one's composition (the greater your use of these effects the more
this helps keep a clear mix). Although many time-based effects (especially VST plugins) have options
to sync to a track's tempo, sometimes you might want to fine tune to a specific amount for finer control.

## Implementation
The program calculates the duration of a note length with a function that accepts a BPM value and a <i>measure subdivision</i>
as parameters. It then returns the result of the following calculation as a double value, rounded to 2 decimal points:

<ul>
<li>(60000 / BPM) / subdivision</li>
</ul>

If no subdivision parameter is provided, it calculates a quarter note (or one beat) by default: 
<br>
<ul>
<li>(60000 / BPM)<li>
</ul>
<br>
For shorter notes the subdivision parameter assumes values that are multiples of 2. For half and whole notes,
subdivision assumes the value 0.5 and 0.25 respectively.

## Installation

<b>gcc</b> is required to compile the program. After compiling, navigate to the directory where you compiled it
and you can use the program (type <b>./ms --help</b>) to view information about the available commands.

For the program to be truly more useful than simply performing the calculation yourself or searching for answers online
you should add it to your <i>PATH</i> variable:

<ol>
<li>Navigate to the directory where you compiled the binary.</li>
<li>Type <b>echo $PATH</b> in your terminal and choose the most appropriate folder to copy the program to (my recommendation is <b>/usr/local/bin</b>)</li>
<li>Type <b>sudo cp ms [target directory]</b> to add the program to your path</li>
</ol>

Now you can just bring up a terminal any time and use it like all other Linux commands (<i>./</i> no longer required: <b>ms --help</b> will get you started).
