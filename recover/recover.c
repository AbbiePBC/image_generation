#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    //repeat the following
    //open memory card
    if (argc > 2)
    {
        printf("One argument only please!\n");
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    char result[8];
    FILE *output = NULL;
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    //create buffer
    typedef uint8_t BYTE;
    BYTE buffer[512];
    int number_of_jpegs = 0;
   
    //check first digits are in buffer
    
    while (fread(&buffer, sizeof(BYTE), 512, input)) //feof(input) != 0
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            printf("jpeg has been opened\n");
            if (output != NULL)
            {
                fclose(output);
            }
            sprintf(result, "%03i.jpg", number_of_jpegs);
            output = fopen(result, "w");
            number_of_jpegs ++;
            
        }
        printf("writing %lu bytes to jpeg\n", sizeof(buffer));
        if (number_of_jpegs > 0)
        {
            fwrite(buffer, sizeof(buffer), 1, output);
        }
        
    }

    fclose(input);
    fclose(output);

}