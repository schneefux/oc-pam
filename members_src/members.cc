// members is the complement of groups: whereas groups shows the groups a
// specified user belongs to, members shows users belonging to a specified
// group.

// PRESENTLY WORKING ON FULL OPTION PARSING (search for /**/ for work pt)

// Copyright (c) 1997 by Jim Lynch.
// This software comes with NO WARRANTY WHATSOEVER.
//
// This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; version 2 dated June, 1991, or, at your
//    option, any LATER version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program;  if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// On Debian Linux systems, the complete text of the GNU General
// Public License can be found in `/usr/doc/copyright/GPL' (on some
// installations) or /usr/share/common-licenses/GPL (on newer 
// ones).

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <cstdio>
#include <iostream>

 using std::cerr;
 using std::ostream;
 using std::endl;
 using std::cout;
 using std::hex;
 using std::endl;

#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <getopt.h>

#define ALL_OPT        'a'
#define PRIMARY_OPT    'p'
#define SECONDARY_OPT  's'
#define TWO_LINES_OPT  't'
#define HELP_OPT       'h'

#define END_SHORT_OPT  '\0'
#define END_LONG_OPT   {(char *) 0, 0, (int *) 0, '\0'}

const char short_blank = END_SHORT_OPT;
const struct option long_blank = END_LONG_OPT;

struct option longopts[] =
{
  {"all",         no_argument,  (int *) 0,  ALL_OPT       },
  {"primary",     no_argument,  (int *) 0,  PRIMARY_OPT   },
  {"secondary",   no_argument,  (int *) 0,  SECONDARY_OPT },
  {"two-lines",   no_argument,  (int *) 0,  TWO_LINES_OPT },
  {"help",        no_argument,  (int *) 0,  HELP_OPT      },
  END_LONG_OPT
};

char opt_string[] =
{
  ALL_OPT,
  PRIMARY_OPT,
  SECONDARY_OPT,
  TWO_LINES_OPT,
  HELP_OPT,
  END_SHORT_OPT
};

int optionlen(struct option *option_array)
{
  int result = 0;

  while(option_array[result].name != NULL)
    result++;

  return result;
}

void remove_short_option(char option, char *option_array)
{
  int i;
  int found_index = -1; /* index of found index !or! -1 if none */
  int size = strlen(option_array);

  for(i = 0; i < size; i++)
    {
      if(option_array[i] == option)
	{
	  found_index = i;
	  break;
	}
    }

  if(found_index != -1)
    {
      if(found_index != size - 1) /* not last item? */
	{
	  /* replace found item with last item */
	  
	  option_array[found_index] = option_array[size - 1];
	}

      /* blank out last item */
      option_array[size - 1] = short_blank;
    }
}

void remove_long_option(char option, struct option *option_array)
{
  int i;
  int found_index = -1; /* index of found index !or! -1 if none */
  int size = optionlen(option_array);

  for(i = 0; i < size; i++)
    {
      if(option_array[i].val == option)
	{
	  found_index = i;
	  break;
	}
    }

  if(found_index != -1)
    {
      if(found_index != size - 1) /* not last item? */
	{
	  /* replace found item with last item */
	  
	  option_array[found_index] = option_array[size - 1];
	}

      /* blank out last item */
      option_array[size - 1] = long_blank;
    }
}

void print_option_array(struct option *option_array)
{
  int i = 0;

  while(option_array[i].val != short_blank)
    {
      if(option_array[i].flag != NULL)
	{
	  cout 
	    << "{" 
	    << option_array[i].name << ", "
	    << option_array[i].has_arg << ", "
	    << (*option_array[i].flag) << ", '"
	    << ( (char) option_array[i].val ) << "'=0x"
	    << std::hex << option_array[i].val << "}" << std::dec << endl;
	}
      else
	{
	  cout 
	    << "{"
	    << option_array[i].name << ", "
	    << option_array[i].has_arg << ", NULL, '"
	    << ( (char) option_array[i].val ) << "'=0x"
	    << std::hex << option_array[i].val << "}" << std::dec << endl;
	}
      
      i++;
    }
}

void disable_option(char *opt_chars)
{
  while(*opt_chars)
    {
      remove_short_option(*opt_chars, opt_string);
      remove_long_option(*opt_chars, longopts);

      opt_chars++;
    }
}

void usage(ostream &out, char *name)
{
  out << endl << "Usage: " << name;
  out << "  [ -apst --all --primary --secondary --two-lines ] <group>" << endl;
}

//receives:
// - C string containing group name
// - pointer to int containing (!assumed initialized!) flag
//     indicating whether a member name was printed on this line.
//     (if true, at least one member had been printed)
// returns true if group found.

int display_primaries(const char *groupString, int *printedOne)
{
  int result = 0;
  struct group *theGroup;

  theGroup = getgrnam(groupString);
  
  if(theGroup)
    {
      struct passwd *thePasswd;

      setpwent();

      thePasswd = getpwent();

      if(thePasswd)
	{	
	  thePasswd = getpwent();
	  
	  while(thePasswd)
	    {
	      if(thePasswd->pw_gid == theGroup->gr_gid)
		{
		  // if there is at least one, print it without any space
		  // if there is at least one more, print space then it
		  
		  if(*printedOne)
		    cout << " ";
		  else
		    *printedOne = 1;
		  
		  cout << thePasswd->pw_name;
		}
	      
	      thePasswd = getpwent();
	    }
	}

      endpwent();

      result = 1;
    }

  return result;
}

//receives:
// - C string containing group name
// - pointer to int containing (!assumed initialized!) flag
//     indicating whether a member name was printed on this line.
//     (if true, at least one member had been printed)
// returns true if group found.

int display_secondaries(const char *groupString, int *printedOne)
{
  int result = 0;
  struct group *theGroup;

  theGroup = getgrnam(groupString);
  
  if(theGroup)
    {
      char **member = theGroup->gr_mem;
      
      result = 1;
      
      // if there is at least one, print it without any space
      // if there is at least one more, print space then it
      
      while(*member) 
	{
	  if(*printedOne)
	    cout << " ";
	  else
	    *printedOne = 1;

	  cout << *member++;
	}
    }

  return result;
}

int main(int argc, char *argv[])
{
  int result = 1;    // pessimistic since we do something if success
  int first_nonopt = 1; /* index of the first non-option in argv */
  int is_bad_opt = 0; /* true if unrecognized option */
  int getopt_result;
  int longindex = 0;

  int wants_help = 0; /* boolean from options */
  int wants_all_members = 0;
  int wants_primary = 0;
  int wants_secondary = 0;
  int wants_two_lines = 0;
  
  getopt_result = 
    getopt_long
    (
      argc, 
      argv, 
      opt_string,
      longopts, 
      &longindex
    );
  
  while(getopt_result != EOF)
    {
      /* process opts here */
      
      switch(getopt_result)
	{
	case ALL_OPT:
	  wants_all_members = 1;
	  
	  disable_option("apst");
	  
	  break;
	  
	case PRIMARY_OPT:
	  wants_primary = 1;
	  
	  disable_option("apst");
	  
	  break;
	  
	case SECONDARY_OPT:
	  wants_secondary = 1;
	  
	  disable_option("apst");
	  
	  break;
	  
	case TWO_LINES_OPT:
	  wants_two_lines = 1;
	  
	  disable_option("apst");
	  
	  break;
	  
	case HELP_OPT:
	  wants_help = 1;
	  
	  disable_option("apst");
	  
	  break;
	  
	case '?':
	default:
	  is_bad_opt = 1;
	  break;
	}
      
      if(is_bad_opt) 
	break;
      else
	getopt_result = 
	  getopt_long
	  (
	    argc, 
	    argv, 
	    opt_string, 
	    longopts, 
	    &longindex
	  );
    }
  
  first_nonopt = optind;
  
#ifdef SKIP  
  // display results of option parsing  
  cout << "wants_help: " << ( wants_help  ? "yes" : "no" ) << endl;
  cout << "wants_all_members: " << ( wants_all_members  ? "yes" : "no" ) << endl;
  cout << "wants_primary: " << ( wants_primary  ? "yes" : "no" ) << endl;
  cout << "wants_secondary: " << ( wants_secondary  ? "yes" : "no" ) << endl;
  cout << "wants_two_lines: " << ( wants_two_lines  ? "yes" : "no" ) << endl;
  cout << "argc: " << argc << "; first_nonopt: " << first_nonopt << endl;
#endif
  
  if(wants_help)
    {
       usage(cout, argv[0]);
       
       cout << "You can use -one- of -a, -p, -s, or -t." << endl;
       
       cout << endl;
       cout << endl;
       cout << "OPTIONS" << endl;
       cout << endl;
       cout << "  -a or --all" << endl;
       cout << "       [default] show all members, both primary and secondary." << endl;
       cout << endl;
       cout << "  -p or --primary" << endl;
       cout << "       show only primary members" << endl;
       cout << "  -s or --secondary" << endl;
       cout << "       show only secondary members" << endl;
       cout << "" << endl;
       cout << "  -t or --two-lines" << endl;
       cout << "       output two lines, first is primary group members," << endl;
       cout << "       secondary members on the second line." << endl;
       cout << "" << endl;
       cout << "  -h or --help" << endl;
       cout << "       print this message and exit successfully" << endl;
       cout << "" << endl;
       cout << "ARGUMENTS" << endl;
       cout << "" << endl;
       cout << "  group (required)" << endl;
       cout << "       the group to find members in." << endl;
       cout << "" << endl;
       cout << "SEE ALSO: man members for details." << endl;
       cout << "" << endl;
       
       return 0;
    }
  else if(argc != first_nonopt + 1) 
    {
      usage(cerr, argv[0]);
      
      return 1;
    }

  if(argc == first_nonopt + 1)
    {
      int bad = 0;
      int sbad = 0;
      int printedOne;
      
      if // user wants default (i.e., no option selected)?
	(
	 ! 
           (
	     wants_all_members || 
	     wants_primary || 
	     wants_secondary || 
	     wants_two_lines
	   )
	)
	{
	  wants_all_members = 1; // the default for this app
	}
      
      // fill in responses to errors
      
      if(wants_all_members)
	{
	  printedOne = 0;
	  
	  if(display_primaries(argv[first_nonopt], &printedOne))
	    {
	      if(display_secondaries(argv[first_nonopt], &printedOne))
		if(printedOne)
		  cout << endl;
	      else
		sbad = 1;
	    }
	  else
	    bad = 1;
	}
      else if(wants_primary)
	{
	  printedOne = 0;
	  
	  bad = ! display_primaries(argv[first_nonopt], &printedOne);
	  
	  if(! bad)
	    cout << endl;
	}
      else if(wants_secondary)
	{
	  printedOne = 0;
	  
	  bad = ! display_secondaries(argv[first_nonopt], &printedOne);
	  
	  if( (! bad) && printedOne)
	    cout << endl;
	}
      else if(wants_two_lines)
	{
	  printedOne = 0;
	  
	  bad = ! display_primaries(argv[first_nonopt], &printedOne);
	  
	  if(! bad)
	    {
	      cout << endl;
	      
	      printedOne = 0;
	      
	      sbad = ! display_secondaries(argv[first_nonopt], &printedOne);
	      
	      cout << endl;
	    }
	}
      
      if(bad) // presumably, group does not exist
	{
	  cerr << "members: group " << argv[first_nonopt] << " does not exist";
	  cerr << endl;
	}
      
      result = bad || sbad;
    }
  else 
    result = 1;
  
  return result;
}

