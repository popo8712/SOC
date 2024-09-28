#include <stdio.h>
#include <stdlib.h>

#define WIDTH 352   // Image width
#define HEIGHT 288  // Image height

// Calculate the size of each frame
#define FRAME_SIZE (WIDTH * HEIGHT * 3 / 2)

unsigned char buffer[FRAME_SIZE];  // Used to store one frame of YUV data

int main() {
    // Open the YUV file
    FILE *file = fopen("C:/Users/admin/Desktop/soc/HW1/akiyo_cif.yuv", "rb");
    if (!file) {
        printf("Error: Unable to open input file.\n");
        return -1;
    }

    // Open the output YUV file
    FILE *output_file = fopen("output_luminance.yuv", "wb");
    if (!output_file) {
        printf("Error: Unable to open output file.\n");
        fclose(file);
        return -1;
    }

    // Process the video data frame by frame
    while (fread(buffer, sizeof(unsigned char), FRAME_SIZE, file) == FRAME_SIZE) {
        // Split the YUV data
        unsigned char *Y = buffer;                             // Y plane starts from buffer
        unsigned char *Cb = Y + WIDTH * HEIGHT;                // Cb plane follows the Y plane
        unsigned char *Cr = Cb + (WIDTH * HEIGHT / 4);         // Cr plane follows the Cb plane

        // Set Cb and Cr to 128, indicating gray
        for (int i = 0; i < WIDTH * HEIGHT / 4; i++) {
            Cb[i] = 128;
            Cr[i] = 128;
        }

        // Write the processed data to the new YUV file
        fwrite(buffer, sizeof(unsigned char), FRAME_SIZE, output_file);
    }

    // Close the files
    fclose(file);
    fclose(output_file);

    printf("YUV luminance processing complete. Output saved to 'output_luminance.yuv'.\n");

    return 0;
}
