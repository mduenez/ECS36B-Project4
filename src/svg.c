#include "svg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Concrete context struct
typedef struct {
    svg_write_callback write_cb;
    svg_cleanup_callback cleanup_cb;
    svg_user_context_ptr user_ctx;
    int destroyed;
    int width;
    int height;
} svg_context_t;

// Helper function to write SVG elements
static svg_return_t write_element(svg_context_t* ctx, const char* data) {
    if (!ctx) return SVG_ERR_NULL;
    if (ctx->destroyed) return SVG_ERR_STATE;
    return ctx->write_cb(ctx->user_ctx, data);
}

// Create SVG context
svg_context_ptr svg_create(svg_write_callback write_cb,
                           svg_cleanup_callback cleanup_cb,
                           svg_user_context_ptr user_ctx,
                           int width,
                           int height) {
    if (!write_cb) return NULL;

    svg_context_t* ctx = (svg_context_t*)malloc(sizeof(svg_context_t));
    if (!ctx) return NULL;

    ctx->write_cb = write_cb;
    ctx->cleanup_cb = cleanup_cb;
    ctx->user_ctx = user_ctx;
    ctx->destroyed = 0;
    ctx->width = width;
    ctx->height = height;

    char buf[256];
    snprintf(buf, sizeof(buf),
             "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n",
             width, height);
    write_element(ctx, buf);

    return (svg_context_ptr)ctx;
}

// Destroy SVG context
svg_return_t svg_destroy(svg_context_ptr ctx_ptr) {
    svg_context_t* ctx = (svg_context_t*)ctx_ptr;
    if (!ctx) return SVG_ERR_NULL;
    if (ctx->destroyed) return SVG_ERR_STATE;

    write_element(ctx, "</svg>\n");
    svg_return_t ret = SVG_OK;
    if (ctx->cleanup_cb) ret = ctx->cleanup_cb(ctx->user_ctx);

    ctx->destroyed = 1;
    free(ctx);
    return ret;
}

// Draw functions
svg_return_t svg_text(svg_context_ptr ctx_ptr, double x, double y, const char* text) {
    svg_context_t* ctx = (svg_context_t*)ctx_ptr;
    if (!ctx || !text) return SVG_ERR_NULL;

    char buf[512];
    snprintf(buf, sizeof(buf), "<text x=\"%.2f\" y=\"%.2f\">%s</text>\n", x, y, text);
    return write_element(ctx, buf);
}

svg_return_t svg_circle(svg_context_ptr ctx_ptr, double cx, double cy, double r) {
    svg_context_t* ctx = (svg_context_t*)ctx_ptr;
    if (!ctx) return SVG_ERR_NULL;

    char buf[256];
    snprintf(buf, sizeof(buf), "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" />\n", cx, cy, r);
    return write_element(ctx, buf);
}

svg_return_t svg_rectangle(svg_context_ptr ctx_ptr, double x, double y, double width, double height) {
    svg_context_t* ctx = (svg_context_t*)ctx_ptr;
    if (!ctx) return SVG_ERR_NULL;

    char buf[256];
    snprintf(buf, sizeof(buf), "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" />\n",
             x, y, width, height);
    return write_element(ctx, buf);
}

svg_return_t svg_line(svg_context_ptr ctx_ptr, double x1, double y1, double x2, double y2) {
    svg_context_t* ctx = (svg_context_t*)ctx_ptr;
    if (!ctx) return SVG_ERR_NULL;

    char buf[256];
    snprintf(buf, sizeof(buf), "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" />\n",
             x1, y1, x2, y2);
    return write_element(ctx, buf);
}

// Groups
svg_return_t svg_group_begin(svg_context_ptr ctx_ptr) {
    svg_context_t* ctx = (svg_context_t*)ctx_ptr;
    if (!ctx) return SVG_ERR_NULL;

    return write_element(ctx, "<g>\n");
}

svg_return_t svg_group_end(svg_context_ptr ctx_ptr) {
    svg_context_t* ctx = (svg_context_t*)ctx_ptr;
    if (!ctx) return SVG_ERR_NULL;

    return write_element(ctx, "</g>\n");
}

// Path
svg_return_t svg_path(svg_context_ptr ctx_ptr, const char* path_data) {
    svg_context_t* ctx = (svg_context_t*)ctx_ptr;
    if (!ctx || !path_data) return SVG_ERR_NULL;

    char buf[512];
    snprintf(buf, sizeof(buf), "<path d=\"%s\" />\n", path_data);
    return write_element(ctx, buf);
}