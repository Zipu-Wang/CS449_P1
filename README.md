# CS449_P1CS 449
Luis OliveiraGeneral InfoSyllabusScheduleResourcesFor FunLabsProjects
Project 1 - Hiding data: Reading files
Released: 23:59 Monday, February 5th, 2024.

Due: 17:59 Monday, February 19th, 2024.

Let's hide images within images!

Steganography
The art of secret writing; cryptography. Also, cryptographic script, cipher.

OED - https://www.oed.com/view/Entry/189651
Introduction
Can you tell the difference between these two images?

Pepper dark

Pepper light

Perhaps one is over-exposed (too bright)? And the other under-exposed (too dark)? Well, it may be obvious by comparison, but alone they would just be normal pictures. And that’s something we can take advantage of :) Small changes to image data, while visually obvious, don’t really affect the subject captured significantly. Both images above have had 50% of the image data changed from the original. And depite all of thoses changes you can still clearly see the image!

Perhaps we can cleverly change images to hide data! Sounds fun?

Ok, let’s do it!

BMP images
A Bitmap Image File (typical extension: .bmp) is a container format for a big array of pixels (picture cells). There are a variety of ways that BMP files can encode image data, but we will focus on one particular form and write a program that performs two simple image transformations: Hide an image within another, and reveal that image.

Bitmaps can have many different formats, but in this assignment, we will be assuming Windows Bitmap files whose contents are 24-bit RGB color. This means that each pixel is represented by a 24-bit number, split into three 8-bit parts. The first part is the intensity of the color blue, the second is the intensity of the color green, and the third is the intensity of the color red, each expressed as an integer value from 0-255.

Structure of a BMP file
Your first step is to read and validate that the specified file is a BMP in a format we can handle. The first 14 bytes of all valid bitmap files begin with a “header” described in the table below:

Offset	Length	Description
0	2	Format identifier (magic number)
2	4	Size of the file in bytes
6	2	A two-byte reserved value
8	2	Another two-byte reserved value
10	4	Offset to the start of the pixel array
In your program, this header should be represented as a structure, which can then be read from the file using fread.

For the project, make sure that your format is BM by verifying that the format contains those two upper-case letters. Be careful, there is no NUL terminator since it’s not a proper string! If the value in the format is not BM, display an error message that we do not support the file format, and exit.

After the file header, you’ll find another header! called “DIB header” (DIB - Device Independent Bitmap). This bitmap describes how the actual image is stored in the file, for example how each pixel is represented.

Offset	Length	Description
0	4	Size of this DIB header in bytes
4	4	Width of the image in pixels
8	4	Height of the image in pixels
12	2	Number of color planes (don’t worry)
14	2	Number of bits per pixel
16	4	Compression scheme used
20	4	Image size in bytes
24	4	Horizontal resolution
28	4	Vertical resolution
32	4	Number of colors in the palette
36	4	Number of important colors
Again, this header can be represented as a C struct and read from the file. Most of these fields will not be used in our program, so don’t worry about understanding all of them. Just make the struct, create an instance, and fread it in.

What you need to check:

Check that the size of the DIB header (that first field in this struct) is 40. It could be other values and if so, once again, display an error message that we do not support the file format, and exit.
If our program gets to this point, we have a BMP that we can read. Let us ensure that the pixel data is encoded in 24-bit RGB by testing the bits per pixel field of our DIB header against 24. If it is any other value, display an error message that we don’t support the file format, and exit.
Now, we can be confident our file has the data we understand and can work with.

Use fseek to move the file pointer to the location specified in the original BMP header as the offset of the start of the pixel array. We are now ready to process the individual pixels.

The Pixel Array
Each pixel is three 8-bit integers, representing blue, green, and red intensity on a scale of 0-255. Create a pixel struct with the appropriate fields:

Offset	Length	Description
0	1	Blue intensity
1	1	Green intensity
2	1	Red intensity
The pixels are laid out in the file row by row (in upside-down, bottom row of the image first, order, for reasons we don’t care about nor will it affect our program).

The number of rows is the height as specified in the DIB header and the number of pixels per row is the width from the DIB header.

Use fread to read in each pixel, transforming it according to the project requirements, and use fwrite to write it back to the file. Note that in doing the fread, the file pointer is advanced, so if you do an fwrite you’ll actually be overwriting the neighboring pixel. To fix this, you can rewind the file backwards using fseek relative to the current position.

We do it this way so that we do not need to worry about creating a new, valid BMP file. We will just reuse the original one’s headers without modification, since our transformations never alter the size of the image or anything else recorded in the header.

There is one major quirk with reading the pixel array. The BMP format requires that a single row of pixels be a multiple of 4 bytes. But each pixel is 3 bytes, and 3 * width is not guaranteed to be a multiple of 4. If that’s the case, extra empty padding bytes are used to force the next pixel row to start at an offset that is a multiple of 4. At the end of reading a row, calculate how many padding bytes are present, and use fseek to skip them, before looping back to process the next row.

Pixel layout on a BMP file

Revealing the secret!
How does it work?
RGB (or BGR) is a color representation that stores a mix of three colors, allowing for the representation if many colors all across the visible spectrum. How many slight variations we can represent depends on how much detail we allow the image to store. If we store more bits, we can represent more color variations using more space; conversely, if we store less bits, we have coarser grained color representations but save space. We call the number bits per pixel bit-depth, for example a 24-bit color, is one where each pixel is represented with 24 bits. And if each pixel is composed by 3 colors, that means that each color gets 8 bits! E.g., orange can be represented by R: 0xFF, G: 0x80, B: 0x00, or merged into a single value 0xFF8000, or if you are a web-page #FF8000.

Now, back to those images. The dark image had the 4 LSB (Least significant bits) of each color set to 0x0. The light image had the 4 LSB (Least significant bits) of each color set to 0xF.

Here is an example with orange text:

Hello (R: 0xFF, G: 0x80, B: 0x00)

Hello (R: 0xF0, G: 0x80, B: 0x00)

Hello (R: 0xFF, G: 0x8F, B: 0x0F)

If such a significant change of the 4 LSB produces such a small difference in perceived color, then we can hide something more interesting in those bits. For example a whole other image!!!!

Pepper secret

Can you see it? No? Well, let’s write a program that can reveal the secret!

What are you programming?
For your project, you will build a C program that reads a BMP file, and reveals any secrets stored in them!! :)

Then, you will extend your program to be able to hide an image in another.

The secret image will be stored as the 4 LSB of a 24-bit BMP file. Such that for each pixel stored, the first half of the number are the 4MSB of the decoy image (the one you actually see), and the second half of the number are the 4MSB of the secret message. As we saw above, the secret message can go unnoticed, at least for images with multiple colors! Check the example below. Each color is 8bits!

    Mixed image: B: 0xIJ, G: 0xKL, R: 0xMN
    Decoy image: B: 0xI0, G: 0xK0, R: 0xM0
    Secret image: B: 0xJ0, G: 0xL0, R: 0xN0
Your turn
Step 1. I suggest you complete it this week!

A.1 (2 points): Implement a C program that can be called with the following arguments and displays both headers. Check the example output for the error messages you should print (EXACTLY! after all the autotester is dumb :)
NOTE: The values are just an example, and the files provided may or may not print the same values!!

$ ./bmp_steganography --info file_that_doesnt_exist.bmp
ERROR: File not found.
$ ./bmp_steganography --info wrong_file_type.bmp
ERROR: The format is not supported.
$ ./bmp_steganography --info wrong_file_type.bmp
ERROR: The format is not supported.
$ ./bmp_steganography --info
ERROR: Missing arguments.
$ ./bmp_steganography --info supported_bmp_file.bmp
=== BMP Header ===
Type: BM
Size: 2073654
Reserved 1: 0
Reserved 2: 0
Image offset: 54

=== DIB Header ===
Size: 40
Width: 960
Height: 720
# color planes: 1
# bits per pixel: 24
Compression scheme: 0
Image size: 2073600
Horizontal resolution: 7559
Vertical resolution: 7559
# colors in palette: 0
# important colors: 0
Step 2. I suggest you complete this AT LEAST 2 days before the deadline, to give you time to complete the last step.

A.2 (4 points): Extend your C program so that it can be called with the following arguments and reveals the hidden image
Again, make sure you print the appropriate error messages (same as in the previous question!)

$ ./bmp_steganography --reveal FILENAME
Make your program read each BGR color stored in the BMP file, and swap its 4MSB with its 4LSB (swap the decoy and hidden images). If you run your program multiple times on the same image, it will flip back-and-forth between those 2 images.

Note that the program modifies the input file!! This is expected.

A.3 (2 points): Extend a C program that can be called with the following arguments and hides FILENAME2 in the image in FILENAME1
Again, make sure you print the appropriate error messages (same as in the previous question!)

$ ./bmp_steganography --hide FILENAME1 FILENAME2
For the last part, your program needs to hide FILENAME2 in the 4LSB of each pixel in FILENAME1. To make the implementation simpler, make sure both images have the same width and height and throw an error otherwise.

Note that the program modifies FILENAME1! Again, this is expected, so make sure you have a backup copy of the original.

A.4 (1 points): Create a Makefile whose default behaviour when called (i.e. without arguments) is compile your code into an executable named bmp_steganography
Note: CAPITAL M in Makefile !!

If you don’t provide a Makefile, you cannot use multiple files in your solution! All code must be in a single file named bmp_steganography.c

A.5 (1 points): [Manually graded!] Use a good programming style with all code indented, code commented, using functions appropriately, and naming functions and variables in a legible way.
Hints
Arguments for a C program
The main function on your C programs can accept 2 arguments:

int main(int argc, char *argv[]);
The first, is called the “argument count” and it tells you how many arguments your program received. The second, is called the “argument vector” and is an array of strings! each being a different argument!

The first argument you receive is always the program being invoked, why? (We’ll explore that in a later project). So argc is expected to be greater or equal to 1.

Write a simple program that prints all arguments (a for loop with a printf is enough). That will help you understand how to access the values.

For the project, you can use your str* functions to parse those inputs.

On the compactness of structs
Remember how the struct sizes were weird in Lab 1? Well you can make sure there is no padding between elements by writting this above the struct declaration:

#pragma pack(1)
// Declare your structs here
#pragma pack(4)
This will force the alignment to be a multiple of 1B. It will likely make memory access slower, but the struct will be compact. This allows you to read a struct from a file in one go, instead of element-by-element.

Image with a secret
Use these images to test your code.

Peppers with a secret:

wget https://cs0449.gitlab.io/sp2024/projects/01/pepper_secret.bmp
Peppers without a secret:

wget https://cs0449.gitlab.io/sp2024/projects/01/pepper.bmp
BMP alignment
Don’t forget each row of the image must have a number of Bytes multiple of 4! So if the 3*width is not a multiple of 4, you need to move forwards. Ya know the REMAINDER of the way to 4!

Don’t implement EVERYTHING before testing!
Step 1 is already a simple enough one that allows you to check if you are parsing data correctly. IF any of the numbers printed make no sense, then you’ll know something is wrong. For example, I forgot one of the struct elements for the header… so the width was TOO large.

Before you attempt to finish Step 2 try something simpler that helps you understand if you are reading the file correctly. I did a bitwise-not (~) on the blue pixel. Because the whole image turned MORE blue where it was not, and less blue where it was, I knew I had the right data! If the image had turned more green, then I would know I had something wrong.

Step 3 should be VERY similar to Step 2, but you’ll have to open and read 2 files. And then you have to merge the data according to the description.

Submission
When you’re done, submit your work WITHOUT any sub-folders!

If you have a makefile: Submit the Makefile and any other files needed to build your code!
If you don’t have a makefile: Submit a single file named bmp_steganography.c to Gradescope.
When you’re done, submit a file named bmp_steganography.c to Gradescope. The testing program will be made available at least 3 days before the deadline. This is to force you to test your code in thoth!

Testing your code on Gradescope is a terrible practice, DON’T DO IT! Once submissions are open, you are free to submit as many times as you want.

CS 449
CS 449
loliveira@pitt.edu
 luisfnqoliveira
CS 449;
