#include "didi.h"

static int debug;

void 
usage(char *progname)
{
  fprintf(stderr, "usage..\n");
}

int 
main(int argc, char **argv)
{
  HttpRequest  *req  = NULL;
  int           port = 8000;
  int           c;
  unsigned int  address = INADDR_ANY;
  char          *didiwiki_home = NULL;

  debug = 0;

  while (1)
    {
      static struct option long_options[] = 
	{
	  {"debug", no_argument,       0, 'd'},
	  {"loopback", no_argument,       0, 'l'},
	  {"port",  required_argument, 0, 'p'},
	  {"home",  required_argument, 0, 'h'},
	  {0, 0, 0, 0}
	};

      /* getopt_long stores the option index here */
      int option_index = 0;
      
      c = getopt_long (argc, argv, "dlp:h:", long_options, &option_index);

      /* detect the end of the options */
      if (c == -1)
	break;

      switch (c)
	{
	case 0:
	  break;
	  
	case 'd':
	  debug = 1;
	  break;

	case 'l':
	  address = INADDR_LOOPBACK;
	  break;

	case 'p':
	  port = atoi(optarg);
	  break;
	  
	case 'h':
	  didiwiki_home = optarg;
	  break;

	default:
		abort();
	}
    }

  wiki_init(didiwiki_home);

  if (debug)
    {
      req = http_request_new(); 	/* reads request from stdin */
    }
  else {
    req = http_server(address, port);    /* forks here */
  }

  wiki_handle_http_request(req);

  return 0;
}
