#include <stdio.h>
#include <stdlib.h>
#include <math.h>  // For floating-point operations

#define WIDTH 352   // Image width
#define HEIGHT 288  // Image height
#define FRAME_SIZE (WIDTH * HEIGHT * 3 / 2)  // YUV 4:2:0 frame size

// Function to convert YUV to RGB with floating-point precision
void YUVtoRGB(unsigned char Y, unsigned char Cb, unsigned char Cr, unsigned char* R, unsigned char* G, unsigned char* B) {
    float C = (float)(Y - 16);
    float D = (float)(Cb - 128);
    float E = (float)(Cr - 128);

    // YUV to RGB conversion
    float Rf = 1.164 * C + 1.596 * E;
    float Gf = 1.164 * C - 0.392 * D - 0.813 * E;
    float Bf = 1.164 * C + 2.017 * D;

    // Clamp the values to the range 0-255
    *R = (unsigned char)(fminf(fmaxf(Rf, 0.0), 255.0));
    *G = (unsigned char)(fminf(fmaxf(Gf, 0.0), 255.0));
    *B = (unsigned char)(fminf(fmaxf(Bf, 0.0), 255.0));
}

// Function to convert RGB to YUV with floating-point precision
void RGBtoYUV(unsigned char R, unsigned char G, unsigned char B, unsigned char* Y, unsigned char* Cb, unsigned char* Cr) {
    float Rf = (float)R;
    float Gf = (float)G;
    float Bf = (float)B;

    // RGB to YUV conversion
    float Yf = 0.299 * Rf + 0.587 * Gf + 0.114 * Bf;
    float Cbf = -0.168736 * Rf - 0.331264 * Gf + 0.5 * Bf + 128.0;
    float Crf = 0.5 * Rf - 0.418688 * Gf - 0.081312 * Bf + 128.0;

    // Clamp the values to the range 0-255
    *Y = (unsigned char)(fminf(fmaxf(Yf, 0.0), 255.0));
    *Cb = (unsigned char)(fminf(fmaxf(Cbf, 0.0), 255.0));
    *Cr = (unsigned char)(fminf(fmaxf(Crf, 0.0), 255.0));
}

int main() {
    unsigned char buffer[FRAME_SIZE];  // Used to store one frame of YUV data

    // Open the input YUV file
    FILE *file = fopen("C:\\Users\\admin\\Desktop\\soc\\HW1 R2\\akiyo_cif.yuv", "rb");
    if (!file) {
        perror("Error opening input file");
        return -1;
    }

    // Open the output YUV file
    FILE *output_file = fopen("C:\\Users\\admin\\Desktop\\soc\\HW1 R2\\output_yuv_rgb.yuv", "wb");
    if (!output_file) {
        perror("Error opening output file");
        fclose(file);
        return -1;
    }

    // Process video frame by frame
    while (fread(buffer, sizeof(unsigned char), FRAME_SIZE, file) == FRAME_SIZE) {
        // Decompose YUV data
        unsigned char *Y = buffer;
        unsigned char *Cb = Y + WIDTH * HEIGHT;
        unsigned char *Cr = Cb + (WIDTH * HEIGHT / 4);

        // Create new YUV buffers to store converted data
        unsigned char new_Y[WIDTH * HEIGHT];
        unsigned char new_Cb[WIDTH * HEIGHT / 4];
        unsigned char new_Cr[WIDTH * HEIGHT / 4];

        // Convert YUV to RGB, then back to YUV
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                int chroma_index = (i / 2) * (WIDTH / 2) + (j / 2);
                unsigned char R, G, B;

                // Convert YUV to RGB
                YUVtoRGB(Y[i * WIDTH + j], Cb[chroma_index], Cr[chroma_index], &R, &G, &B);

                // Convert RGB back to YUV
                RGBtoYUV(R, G, B, &new_Y[i * WIDTH + j], &new_Cb[chroma_index], &new_Cr[chroma_index]);
            }
        }

        // Write the processed YUV data to the output file
        fwrite(new_Y, sizeof(unsigned char), WIDTH * HEIGHT, output_file);
        fwrite(new_Cb, sizeof(unsigned char), WIDTH * HEIGHT / 4, output_file);
        fwrite(new_Cr, sizeof(unsigned char), WIDTH * HEIGHT / 4, output_file);
    }

    // Close the files
    fclose(file);
    fclose(output_file);

    printf("YUV to RGB and back conversion complete. Output saved to 'output_yuv_rgb.yuv'.\n");

    return 0;
}
