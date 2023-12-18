/*****************************************************************************/
/*
 * NTSC/CRT - integer-only NTSC video signal encoding / decoding emulation
 *
 *   by EMMIR 2018-2023
 *
 *   YouTube: https://www.youtube.com/@EMMIR_KC/videos
 *   Discord: https://discord.com/invite/hdYctSmyQJ
 */
/*****************************************************************************/
#ifndef _PPM_RW_
#define _PPM_RW_

/* ppm_rw.h
 *
 * Routines to read and write non-ASCII 24-bit PPM images.
 *
 */

extern int ppm_read24(char *file,
        unsigned char **out_color,
        int *out_w, int *out_h,
        void *(*calloc_func)(size_t, size_t));

extern int ppm_write24(char *name, unsigned char *color, int w, int h);

#endif
