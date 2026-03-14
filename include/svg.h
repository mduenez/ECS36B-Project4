#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void* svg_context_ptr;
typedef void* svg_user_context_ptr;
typedef int svg_return_t;

// Callback types
typedef svg_return_t (*svg_write_callback)(svg_user_context_ptr ctx, const char* data);
typedef svg_return_t (*svg_cleanup_callback)(svg_user_context_ptr ctx);

// Error codes
#define SVG_OK 0
#define SVG_ERR_NULL 1
#define SVG_ERR_STATE 2

// Context creation / destruction
svg_context_ptr svg_create(svg_write_callback write_cb,
                           svg_cleanup_callback cleanup_cb,
                           svg_user_context_ptr user_ctx,
                           int width,
                           int height);

svg_return_t svg_destroy(svg_context_ptr ctx);

// Drawing functions
svg_return_t svg_text(svg_context_ptr ctx, double x, double y, const char* text);
svg_return_t svg_circle(svg_context_ptr ctx, double cx, double cy, double r);
svg_return_t svg_rectangle(svg_context_ptr ctx, double x, double y, double width, double height);
svg_return_t svg_line(svg_context_ptr ctx, double x1, double y1, double x2, double y2);

// Groups
svg_return_t svg_group_begin(svg_context_ptr ctx);
svg_return_t svg_group_end(svg_context_ptr ctx);

// Path
svg_return_t svg_path(svg_context_ptr ctx, const char* path_data);

#ifdef __cplusplus
}
#endif