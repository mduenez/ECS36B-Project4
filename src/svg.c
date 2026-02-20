/**
 * @file svg.c
 * @brief Implementation of simple SVG drawing interface.
 *
 * Implements the basic functions for creating SVG documents.
 */
#include "svg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define SVG_OK 0

/**
 * @brief Opaque SVG drawing context.
 *
 * Holds the necessary data to implement functions.
 */
struct SVG_CONTEXT{

};

svg_context_ptr svg_create(svg_write_fn write_fn, 
                           svg_cleanup_fn cleanup_fn, 
                           svg_user_context_ptr user, 
                           svg_px_t width, 
                           svg_px_t height){
    return NULL;
}

svg_return_t svg_destroy(svg_context_ptr context){
    return SVG_OK;
}

svg_return_t svg_circle(svg_context_ptr context,
                        const svg_point_t *center,
                        svg_real_t radius,
                        const char *style){
    return SVG_OK;
}


svg_return_t svg_rect(svg_context_ptr context,
                      const svg_point_t *top_left,
                      const svg_size_t *size,
                      const char* style){
    return SVG_OK;

}

svg_return_t svg_line(svg_context_ptr context,
                      const svg_point_t *start,
                      const svg_point_t *end,
                      const char* style){
    return SVG_OK;
}

svg_return_t svg_group_begin(svg_context_ptr context, 
                             const char* attrs){
    return SVG_OK;
}

svg_return_t svg_group_end(svg_context_ptr context){
    return SVG_OK;
}
