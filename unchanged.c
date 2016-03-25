#include <sys/inotify.h>
#include <unistd.h>      // close
#include <stdio.h>       // fprintf
#include <stdlib.h>      // malloc
#include <string.h>      // memset

#define FAILURE 0

int main(int argc, char * argv[argc]){
  if (argc < 2){
    char * s = argv[0];
    
    fprintf(stderr, "%s - waits until file(s) change.\n", s);
    fprintf(stderr, "Usage: %s FILE [FILES...]\n", s);
    fprintf(stderr, "  Waits until the inotify kernel facility indicates one of FILE changed.\n");
    fprintf(stderr, "  Exit Status: index of changed file in argument list (starts with 1).\n");
    
    return FAILURE;
  }

  // first element is program name
  argv++;  argc--;

  
  const int fd = inotify_init();
  
  int wd[argc];  // watch-ids for all args
  int n = 0;     // successes

  for (int i = 0; i < argc; i++){
    wd[i] = 0;
    wd[i] = inotify_add_watch(fd, argv[i], IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF);
    if (wd[i] > 0){
      n++;
      /* Could use env var to control verbosity: UNCHANGED_VERBOSE*/
      //fprintf(stderr, "Watching %s\n", argv[i]);
    }
  }

  if (!n){
    fprintf(stderr, "No watchable files listed. Invoke without arguments for usage.\n");
    return FAILURE;
  }

  /*
    Ignoring the file name field; inotify(7) recommends:
      sizeof(struct inotify_event) + NAME_MAX + 1
   */
  const size_t len = sizeof(struct inotify_event);
  struct inotify_event * e = malloc(len);
  if(!e){
    fprintf(stderr, "No memory left.\n");
    return FAILURE;
  }
  memset(e, 0, len);

  // read blocks read; catch signal
  if (-1 == read(fd, e, len)){
    fprintf(stderr, "Interrupted.\n");
    free(e);
    return FAILURE;
  }
  close(fd);

  int idx = 0;
  for(; (idx < argc) && (wd[idx] != e->wd); idx++);
  free(e);

  // 1-indexed return
  return idx + 1;
}
