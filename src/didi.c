#include "didi.h"

static int debug;

int
usage()
{ 
  fprintf(stderr, "Usage: didiwiki [options]\n");
  fprintf(stderr, "   -d, --debug           : debug mode, listens to requests from stdin\n");
  fprintf(stderr, "   -h, --home   <path>   : specify didiwiki's home directory\n");
  fprintf(stderr, "   -l, --listen <ipaddr> : specify IP address\n");
  fprintf(stderr, "   -p, --port   <port>   : specify port number \n");
  fprintf(stderr, "       --help            : display this help message\n");
  exit(1);
}

int 
main(int argc, char **argv)
{
  HttpRequest    *req  = NULL;
  int             port = 8000;
  int             c;
  char           *didiwiki_home = NULL;
  struct in_addr address;


  debug = 0;

  /* by default bind server to "0.0.0.0" */
  address.s_addr = inet_addr("0.0.0.0");

  while (1)
    {
      static struct option long_options[] = 
	{
	  {"debug",  no_argument,       0, 'd'},
      {"listen", required_argument, 0, 'l'},
	  {"port",   required_argument, 0, 'p'},
	  {"home",   required_argument, 0, 'h'},
      {"help",   no_argument,       0,  10 },
	  {0, 0, 0, 0}
	};

      /* getopt_long stores the option index here */
      int option_index = 0;
      
      c = getopt_long (argc, argv, "dl:p:h:", long_options, &option_index);

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
	  
	case 'p':
	  port = atoi(optarg);
	  break;
	  
	case 'h':
	  didiwiki_home = optarg;
	  break;

    case 'l':
      if(inet_aton(optarg,&address) == 0) {
        fprintf(stderr, "didiwiki: invalid ip address \"%s\"\n", optarg);
        exit(1);
      } else
        address.s_addr = inet_addr(optarg);
      break;

    case 10:
      usage();
      break;

	default:
	  usage();
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
