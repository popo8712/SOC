#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Include the math library to use more precise numerical operations

#define WIDTH 352   // Image width
#define HEIGHT 288  // Image height

// Calculate the size of each frame
#define FRAME_SIZE (WIDTH * HEIGHT * 3 / 2)

unsigned char buffer[FRAME_SIZE];  // Used to store one frame of YUV data

// Use floating-point operations for color conversion, increasing precision
void process_frame_with_precision(unsigned char* Y, unsigned char* Cb, unsigned char* Cr) {
    // Perform color operations using floating-point arithmetic for better precision
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        float Yf = (float)Y[i];
        float Cbf = (float)Cb[i / 4];
        float Crf = (float)Cr[i / 4];

        // Use the standard ITU-R BT.601 color formula for more precise calculations
        float Rf = Yf + 1.402 * (Crf - 128);
        float Gf = Yf - 0.3441 * (Cbf - 128) - 0.7141 * (Crf - 128);
        float Bf = Yf + 1.772 * (Cbf - 128);

        // Clamp R, G, B values to the range 0-255 and round to integers
        unsigned char R = (unsigned char)(fminf(fmaxf(Rf, 0.0), 255.0));
        unsigned char G = (unsigned char)(fminf(fmaxf(Gf, 0.0), 255.0));
        unsigned char B = (unsigned char)(fminf(fmaxf(Bf, 0.0), 255.0));

        // Update Y value to grayscale based on RGB values
        Y[i] = (unsigned char)(0.299 * R + 0.587 * G + 0.114 * B);

        // Keep Cb and Cr set to 128
        if (i % 4 == 0) {
            Cb[i / 4] = 128;
            Cr[i / 4] = 128;
        }
    }
}

int main() {
    // Open the YUV file
    FILE *file = fopen("C:/Users/admin/Desktop/soc/HW1 R2/akiyo_cif.yuv", "rb");
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
        // Decompose the YUV data
        unsigned char *Y = buffer;                             // Y plane starts from buffer
        unsigned char *Cb = Y + WIDTH * HEIGHT;                // Cb plane follows the Y plane
        unsigned char *Cr = Cb + (WIDTH * HEIGHT / 4);         // Cr plane follows the Cb plane

        // Process each frame with precision
        process_frame_with_precision(Y, Cb, Cr);

        // Write the processed data to the new YUV file
        fwrite(buffer, sizeof(unsigned char), FRAME_SIZE, output_file);
    }

    // Close the files
    fclose(file);
    fclose(output_file);

    printf("YUV luminance processing complete. Output saved to 'output_luminance.yuv'.\n");

    return 0;
}
