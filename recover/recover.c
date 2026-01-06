#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool check_for_header(uint8_t buffer[512]);

int main(int argc, char *argv[])
{
    if (argc != 2) // if the user didn't run the program with the correct arguments
    {
        printf("Usage: ./recover file_name.extension"); // then show him how it's used
        return 1;                                       // and exit with error 1
    }

    char *file_name = argv[1];                  // set the file name to the first parameter
    FILE *sd_card_dump = fopen(file_name, "r"); // open the file chosen by the user
    FILE *temp_image = NULL; // this is going to be like a container for the image we are working on
    uint8_t buffer[512];     // this is going to be like a container for the 512 Byte chunks of the
                             // chosen image
    unsigned int image_number =
        0;              // number of the image starting from 0, needed for the recovered image name
    char image_name[7]; // stores the name of the image (for example 000.jpeg), since it's 0 padded
                        // it's always the same length of 7

    if (sd_card_dump == NULL) // if something's wrong with the file
    {
        printf("File corrupted, try another file."); // then tell the user
        return 2;                                    // and exit with error 2
    }

    while (fread(buffer, 1, 512, sd_card_dump) ==
           512) // keep reading the 512 Byte chunks untill it runs off (reach the end)
    {
        // printf("%s\n", buffer);
        if (check_for_header(buffer)) // detect when we are working on the first chunk of a new
                                      // image by checking for the header
        {
            if (temp_image != NULL) // if we were already working with an image
                fclose(temp_image); // close it to work on the new one
            sprintf(file_name, "%03i.jpg", image_number); // use sprintf (from stdio.h) to make a
                                                          // new file (for example 000.jpg)
            temp_image = fopen(file_name, "w"); // use temp_image to write into a new jpg file
            image_number++;
        }
        if (temp_image != NULL)                 // if currently working on an image
            fwrite(buffer, 1, 512, temp_image); // then save the current 512 Byte chunk
    }
    if (temp_image != NULL) // if an image is still opened
        fclose(temp_image); // then remember to close it
    fclose(sd_card_dump);   // before terminating remember to close the fictional sd card
    return 0;               // succesfully retrived the images
};

bool check_for_header(uint8_t buffer[512])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
        (buffer[3] & 0xf0) == 0xe0) // check for the jpg header Byte pattern
        return true;                // if found return true since it's a new jpg image initiating
    else
        return false; // else it's something else, most likely the subsequent chunks of Bytes
                      // composing the current image
};
