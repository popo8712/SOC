#include <stdio.h>
#include <stdlib.h>

#define WIDTH 352   // Image width
#define HEIGHT 288  // Image height

// Calculate the size of each frame
#define FRAME_SIZE (WIDTH * HEIGHT * 3 / 2)

// Function to convert YUV to RGB
void YUVtoRGB(unsigned char Y, unsigned char Cb, unsigned char Cr, unsigned char* R, unsigned char* G, unsigned char* B) {
    int C = Y - 16;
    int D = Cb - 128;
    int E = Cr - 128;

    int R_val = (298 * C + 409 * E + 128) >> 8;
    int G_val = (298 * C - 100 * D - 208 * E + 128) >> 8;
    int B_val = (298 * C + 516 * D + 128) >> 8;

    // Limit the range to 0-255
    *R = (R_val < 0) ? 0 : (R_val > 255) ? 255 : R_val;
    *G = (G_val < 0) ? 0 : (G_val > 255) ? 255 : G_val;
    *B = (B_val < 0) ? 0 : (B_val > 255) ? 255 : B_val;
}

// Function to convert RGB to YUV
void RGBtoYUV(unsigned char R, unsigned char G, unsigned char B, unsigned char* Y, unsigned char* Cb, unsigned char* Cr) {
    int Y_val = (77 * R + 150 * G + 29 * B) >> 8;
    int Cb_val = ((-44 * R - 87 * G + 131 * B) >> 8) + 128;
    int Cr_val = ((131 * R - 110 * G - 21 * B) >> 8) + 128;

    // Limit the range to 0-255
    *Y = (Y_val < 0) ? 0 : (Y_val > 255) ? 255 : Y_val;
    *Cb = (Cb_val < 0) ? 0 : (Cb_val > 255) ? 255 : Cb_val;
    *Cr = (Cr_val < 0) ? 0 : (Cr_val > 255) ? 255 : Cr_val;
}

int main() {
    unsigned char buffer[FRAME_SIZE];  // Buffer to store one frame of YUV data

    // Open YUV file
    FILE *file = fopen("C:/Users/admin/Desktop/soc/HW1/akiyo_cif.yuv", "rb");
    if (!file) {
        printf("Error: Unable to open input file.\n");
        return -1;
    }

    // Read the YUV data of a single frame
    fread(buffer, sizeof(unsigned char), FRAME_SIZE, file);
    fclose(file);  // Finished reading, close the file

    // Split YUV data
    unsigned char *Y = buffer;                             // Y plane starts from buffer
    unsigned char *Cb = Y + WIDTH * HEIGHT;                // Cb plane follows the Y plane
    unsigned char *Cr = Cb + (WIDTH * HEIGHT / 4);         // Cr plane follows the Cb plane

    // Create an RGB buffer
    unsigned char R, G, B;

    // Create a new YUV buffer to store the converted data
    unsigned char new_Y[WIDTH * HEIGHT];
    unsigned char new_Cb[WIDTH * HEIGHT / 4];
    unsigned char new_Cr[WIDTH * HEIGHT / 4];

    // Convert YUV to RGB and then back to YUV
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        YUVtoRGB(Y[i], Cb[i / 4], Cr[i / 4], &R, &G, &B);   // Cb and Cr are shared by every 4 pixels
        RGBtoYUV(R, G, B, &new_Y[i], &new_Cb[i / 4], &new_Cr[i / 4]);
    }

    // Write the processed data to a new YUV file
    FILE *output_file = fopen("output_yuv_rgb.yuv", "wb");
    if (!output_file) {
        printf("Error: Unable to open output file.\n");
        return -1;
    }

    // Write the converted YUV data
    fwrite(new_Y, sizeof(unsigned char), WIDTH * HEIGHT, output_file);
    fwrite(new_Cb, sizeof(unsigned char), WIDTH * HEIGHT / 4, output_file);
    fwrite(new_Cr, sizeof(unsigned char), WIDTH * HEIGHT / 4, output_file);
    fclose(output_file);  // Close the file

    printf("YUV to RGB and back conversion complete. Output saved to 'output_yuv_rgb.yuv'.\n");

    return 0;
}
