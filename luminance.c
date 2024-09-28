#include <stdio.h>
#include <stdlib.h>

#define WIDTH 352   // 影像寬度
#define HEIGHT 288  // 影像高度

// 計算每一幀的大小
#define FRAME_SIZE (WIDTH * HEIGHT * 3 / 2)

// YUV 轉換為 RGB 的函數
void YUVtoRGB(unsigned char Y, unsigned char Cb, unsigned char Cr, unsigned char* R, unsigned char* G, unsigned char* B) {
    int C = Y - 16;
    int D = Cb - 128;
    int E = Cr - 128;

    int R_val = (298 * C + 409 * E + 128) >> 8;
    int G_val = (298 * C - 100 * D - 208 * E + 128) >> 8;
    int B_val = (298 * C + 516 * D + 128) >> 8;

    // 限制範圍在 0-255
    *R = (R_val < 0) ? 0 : (R_val > 255) ? 255 : R_val;
    *G = (G_val < 0) ? 0 : (G_val > 255) ? 255 : G_val;
    *B = (B_val < 0) ? 0 : (B_val > 255) ? 255 : B_val;
}

// RGB 轉換為 YUV 的函數
void RGBtoYUV(unsigned char R, unsigned char G, unsigned char B, unsigned char* Y, unsigned char* Cb, unsigned char* Cr) {
    int Y_val = (77 * R + 150 * G + 29 * B) >> 8;
    int Cb_val = ((-44 * R - 87 * G + 131 * B) >> 8) + 128;
    int Cr_val = ((131 * R - 110 * G - 21 * B) >> 8) + 128;

    // 限制範圍在 0-255
    *Y = (Y_val < 0) ? 0 : (Y_val > 255) ? 255 : Y_val;
    *Cb = (Cb_val < 0) ? 0 : (Cb_val > 255) ? 255 : Cb_val;
    *Cr = (Cr_val < 0) ? 0 : (Cr_val > 255) ? 255 : Cr_val;
}

int main() {
    unsigned char buffer[FRAME_SIZE];  // 用來儲存一幀的YUV資料

    // 打開YUV檔案
    FILE *file = fopen("C:/Users/admin/Desktop/soc/HW1/akiyo_cif.yuv", "rb");
    if (!file) {
        printf("Error: Unable to open input file.\n");
        return -1;
    }

    // 讀取單個影格的YUV資料
    fread(buffer, sizeof(unsigned char), FRAME_SIZE, file);
    fclose(file);  // 讀取完畢，關閉檔案

    // 分解YUV資料
    unsigned char *Y = buffer;                             // Y平面從buffer開始
    unsigned char *Cb = Y + WIDTH * HEIGHT;                // Cb平面緊接著Y平面
    unsigned char *Cr = Cb + (WIDTH * HEIGHT / 4);         // Cr平面在Cb平面後面

    // 創建 RGB 緩衝區
    unsigned char R, G, B;

    // 創建一個新的 YUV 緩衝區以保存轉換回來的數據
    unsigned char new_Y[WIDTH * HEIGHT];
    unsigned char new_Cb[WIDTH * HEIGHT / 4];
    unsigned char new_Cr[WIDTH * HEIGHT / 4];

    // 將 YUV 轉換為 RGB，然後再轉換回 YUV
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        YUVtoRGB(Y[i], Cb[i / 4], Cr[i / 4], &R, &G, &B);   // 每 4 個像素共用一組 Cb 和 Cr
        RGBtoYUV(R, G, B, &new_Y[i], &new_Cb[i / 4], &new_Cr[i / 4]);
    }

    // 將處理後的資料寫入新的 YUV 檔案
    FILE *output_file = fopen("output_yuv_rgb.yuv", "wb");
    if (!output_file) {
        printf("Error: Unable to open output file.\n");
        return -1;
    }

    // 寫入轉換後的YUV資料
    fwrite(new_Y, sizeof(unsigned char), WIDTH * HEIGHT, output_file);
    fwrite(new_Cb, sizeof(unsigned char), WIDTH * HEIGHT / 4, output_file);
    fwrite(new_Cr, sizeof(unsigned char), WIDTH * HEIGHT / 4, output_file);
    fclose(output_file);  // 關閉檔案

    printf("YUV to RGB and back conversion complete. Output saved to 'output_yuv_rgb.yuv'.\n");

    return 0;
}
