/* list contents of a directory */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dirent.h"

int main (int argc, char *argv[]) {
  int i;

  /* check command line arguments */
  if (argc <= 1) {
    fprintf (stderr, "usage: printdir directory\n");
    return EXIT_FAILURE;
  }

  /* print contents of directories listed in command line */
  i = 1;
  while (i < argc) {
    DIR *dir;
    struct dirent *ent;

    /* open directory stream */
    dir = opendir (argv[i]);
    if (dir != NULL) {

      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        switch (ent->d_type) {
        case DT_REG:
          printf ("%*.*s\n", ent->d_namlen, ent->d_namlen, ent->d_name);
          break;

        case DT_DIR:
          printf ("%s (dir)\n", ent->d_name);
          break;

        default:
          printf ("%s:\n", ent->d_name);
        }
      }

      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }
    i++;
  }
  return EXIT_SUCCESS;
}
