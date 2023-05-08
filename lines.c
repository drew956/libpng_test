#include <stdio.h>
#include <stdlib.h>

#define PNG_SETJMP_NOT_SUPPORTED
#include <png.h>

#define WIDTH 256
#define HEIGHT 32
#define COLOR_DEPTH 8

struct Pixel {
	png_byte r, g, b, a;
};

void fillImageWithRandomVeritcalBars(struct Pixel** pixels, int width, int height);
int main(int argc, char *argv[]) {
	srand(time(NULL)); //seed based on time to make it random each time
    char* file_name = "out.png";

    if(argc > 1)
       file_name = argv[1];

	/* open PNG file for writing */
	FILE *f = fopen(file_name, "wb");
	if (!f) {
		fprintf(stderr, "could not open out.png\n");
		return 1;
	}

	/* initialize png data structures */
	png_structp png_ptr;
	png_infop info_ptr;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fprintf(stderr, "could not initialize png struct\n");
		return 1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(f);
		return 1;
	}

	/* begin writing PNG File */
	png_init_io(png_ptr, f);
	png_set_IHDR(png_ptr, info_ptr, WIDTH, HEIGHT, COLOR_DEPTH,
	             PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
	             PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	/* allocate image data */
	struct Pixel *row_pointers[HEIGHT];
	for (int row = 0; row < HEIGHT; row++) {
		row_pointers[row] = calloc(WIDTH*2, sizeof(struct Pixel)); //allocates space for width*2 pixels
	}

	/* draw a bunch of vertical lines */
    fillImageWithRandomVeritcalBars(row_pointers, WIDTH, HEIGHT);
	
    /* write image data to disk */
	png_write_image(png_ptr, (png_byte **)row_pointers);

	/* finish writing PNG file */
	png_write_end(png_ptr, NULL);

	/* clean up PNG-related data structures */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* close the file */
	fclose(f);
	f = NULL;

	for (int row = 0; row < HEIGHT; row++) {
		free(row_pointers[row]);
	}
	return 0;
}

void fillImageWithRandomVeritcalBars(struct Pixel** pixels, int width, int height){
	for (int col = 0; col < width; col++) {
		int bar_height = rand() % height;
        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;
        int a = rand() % 256;
        
        for (int row = 0; row < height; row++) {
			if (height - row <= bar_height) {
				pixels[row][col].r = r; // red
				pixels[row][col].g = g; // green
				pixels[row][col].b = b; // blue
				pixels[row][col].a = a; // alpha (opacity)
			} else {
				pixels[row][col].r = 0; // red
				pixels[row][col].g = 0; // green
				pixels[row][col].b = 0; // blue
				pixels[row][col].a = 0; // alpha (opacity)
			}
		}
	}

}
