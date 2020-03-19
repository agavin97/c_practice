/* Alex Gavin
 * Docusign Coding Challenge
 * 12/5/2019
 *
 * Purpose:
 *      Find and print all strings in with characters in alphabetical order.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 255


/* Protoypes */
int find_alphabetical(FILE *file);
int check_line(FILE *file, char *string_read);


/* Functionality */
int main(int argc, char *argv[]) {
    int retval;
    FILE *file;

    if (argc != 2) {
        fprintf(stderr, "Error: invalid invocation.\n"
                "Format: ./find_ordered file_name.\n");
        return -1;
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error: unable to open file \"%s\".\n", argv[1]);
        return -1;
    }

    retval = find_alphabetical(file);

    return retval;
}

/* Description: Iterate through each string printing only if alphabetically ordered
 *
 * Returns: 0 on success, -1 on error.
 */
int find_alphabetical(FILE *file) {
    char *string_read;
    int alphabetical = 0;

    string_read = malloc(sizeof(char)*MAX_SIZE);
    if (string_read == NULL) {
        fprintf(stderr, "Error: malloc failed\n");
        return -1;
    }

    // Run algorithm until EOF condition (set in check_line)
    while (alphabetical != -1) {
        alphabetical = check_line(file, string_read);

        if (alphabetical == 1) {
            printf("%s\n", string_read);
        }
    }

    return 0;
}

/* Description: Read characters from a string checking previously read
 *              characters higher in alphabet. Stops at newline or end of file.
 *
 * Returns: 1 if alphabetically ordered, -1 on EOF, 0 otherwise.
 */
int check_line(FILE *file, char *string_read) {
    int counter = 0;    // Index to copy read chars into string_read
    int retval = 1;     // Assume string alphabetical until find that not
    char cur_char = '\0';
    char prev_char = '\0';

    // Iterate until newline reached or end of file
    cur_char = (char) fgetc(file);
    while (cur_char != EOF && cur_char != '\n') {

        if (prev_char != '\0') {
            // If prev_char not before cur_char in alphabet, not alphabetical; exit loop
            if ((int) prev_char > (int) cur_char) {
                retval = 0;
                break;
            }
        }
        string_read[counter] = cur_char;
        counter++;
        prev_char = cur_char;

        cur_char = (char) fgetc(file);
    }
    string_read[counter] = '\0';

    // String non-alphabetical, read until next line starts
    if (retval == 0) {
        while (((char) fgetc(file)) != '\n');
    }
    // No bytes read, set EOF condition
    else if (counter == 0) {
        retval = -1;
    }

    return retval;
}
