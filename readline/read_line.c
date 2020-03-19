/* Alex Gavin
 * Winter 2020
 * Quarantine, Day 5
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>


/* Macros */
#define HANDLE_ERROR(str)               \
    do {int __errsv = errno;            \
        fprintf(stderr, "%s\n", str);   \
        exit(__errsv);                  \
    } while(0)


/* Globals */
static int max_len = -1;
static char *history_file = (char *) NULL;


/* Prototypes */
int init();
int run();
void save_history();
void check_file();


/* Functions */
int main(int argc, char **argv) {
    if (argc != 1 && argc != 3) {

    }
    else if (argc == 3) {
        history_file = argv[1];
        max_len = atoi(argv[2]);
        check_file();
    }

    init();

    return run();
}

/* Description: 
 *      Initialize requiste information for program.
 *
 * Returns;
 *      void
 */
int init() {
    using_history();

    if (max_len > 0) {
        stifle_history(max_len);
    }
}

/* Description:
 *      Run readline until user ends program.
 *
 * Returns:
 *      0 on success.
 *      Exit on failure.
 */
int run() {
    int line_len = -1;
    char *line_read = (char *) NULL;
    HISTORY_STATE *hist_state = (HISTORY_STATE *) NULL;

    while (1) {
        if (line_read) {
            free(line_read);
        }

        line_read = readline("> ");

        if (line_read && *line_read != '\0') {
            line_len = strlen(line_read);

            if (line_len == 4) {
                if (line_len == 4 && strncmp("quit", line_read, 4) == 0) {
                    fprintf(stderr, "read_line: Quitting.\n"); 
                    exit(0);
                }
            }

            // Only save when history stifled and at max_len
            if (history_is_stifled()) {
                hist_state = history_get_history_state();

                if (!hist_state) {
                    HANDLE_ERROR("read_line: HISTORY_STATE corrupted.");
                }
                else if (hist_state->length == max_len) {
                    save_history();
                }
            }

            printf("%s", line_read);
            add_history(line_read);
        }
        printf("\n");
    } 

    return 0;
}

/* Description: 
 *      Save and clear history to given history_file if at max capacity.
 *      History must be stifled to save.
 *
 * Returns: 
 *      void on success
 *      Exit on failure.
 */
void save_history() {
    if (append_history(max_len, history_file) != 0) {
        HANDLE_ERROR("read_line: Unable to save to file.");
    }
   
    clear_history();
}

/* Description: Ensure history_file specified is valid.
 *
 * Returns:
 *      void on success.
 *      Exit on failure.
 */
void check_file() {
    int err;
    FILE* fd;

    fd = fopen(history_file, "r");

    // Unable to open file for reading
    if (fd == NULL) {
        err = errno;

        // File doesn't exist or path error
        if (err == ENOENT) {
            fd = fopen(history_file, "w");

            if (fd == NULL) {
                HANDLE_ERROR("read_line: Pathname refers to a nonexistent directory.");
            }
        }
        // Unable to access
        else if (err == EACCES) {
            HANDLE_ERROR("read_line: Insufficient permissions to edit file.");
        }
        else {
            HANDLE_ERROR("read_line: Unable to access file.");
        }
    }

    if (fd != NULL) {
        fclose(fd);
    }
}
