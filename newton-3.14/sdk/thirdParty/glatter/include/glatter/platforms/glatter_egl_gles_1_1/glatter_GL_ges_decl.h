/*
Copyright 2018 Ioannis Makris

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// This file was generated by glatter.py script.




typedef union glatter_extension_support_status_union_GL
{
    int inexed_extensions[74];
    struct {
        int has_GL_AMD_compressed_3DC_texture;
        int has_GL_AMD_compressed_ATC_texture;
        int has_GL_APPLE_copy_texture_levels;
        int has_GL_APPLE_framebuffer_multisample;
        int has_GL_APPLE_sync;
        int has_GL_APPLE_texture_2D_limited_npot;
        int has_GL_APPLE_texture_format_BGRA8888;
        int has_GL_APPLE_texture_max_level;
        int has_GL_ARM_rgba8;
        int has_GL_EXT_blend_minmax;
        int has_GL_EXT_debug_marker;
        int has_GL_EXT_discard_framebuffer;
        int has_GL_EXT_map_buffer_range;
        int has_GL_EXT_multi_draw_arrays;
        int has_GL_EXT_multisampled_render_to_texture;
        int has_GL_EXT_read_format_bgra;
        int has_GL_EXT_robustness;
        int has_GL_EXT_sRGB;
        int has_GL_EXT_texture_compression_dxt1;
        int has_GL_EXT_texture_filter_anisotropic;
        int has_GL_EXT_texture_format_BGRA8888;
        int has_GL_EXT_texture_lod_bias;
        int has_GL_EXT_texture_storage;
        int has_GL_IMG_multisampled_render_to_texture;
        int has_GL_IMG_read_format;
        int has_GL_IMG_texture_compression_pvrtc;
        int has_GL_IMG_texture_env_enhanced_fixed_function;
        int has_GL_IMG_user_clip_plane;
        int has_GL_KHR_debug;
        int has_GL_NV_fence;
        int has_GL_OES_EGL_image;
        int has_GL_OES_EGL_image_external;
        int has_GL_OES_blend_equation_separate;
        int has_GL_OES_blend_func_separate;
        int has_GL_OES_blend_subtract;
        int has_GL_OES_byte_coordinates;
        int has_GL_OES_compressed_ETC1_RGB8_sub_texture;
        int has_GL_OES_compressed_ETC1_RGB8_texture;
        int has_GL_OES_compressed_paletted_texture;
        int has_GL_OES_depth24;
        int has_GL_OES_depth32;
        int has_GL_OES_draw_texture;
        int has_GL_OES_element_index_uint;
        int has_GL_OES_extended_matrix_palette;
        int has_GL_OES_fbo_render_mipmap;
        int has_GL_OES_fixed_point;
        int has_GL_OES_framebuffer_object;
        int has_GL_OES_mapbuffer;
        int has_GL_OES_matrix_get;
        int has_GL_OES_matrix_palette;
        int has_GL_OES_packed_depth_stencil;
        int has_GL_OES_point_size_array;
        int has_GL_OES_point_sprite;
        int has_GL_OES_query_matrix;
        int has_GL_OES_read_format;
        int has_GL_OES_required_internalformat;
        int has_GL_OES_rgb8_rgba8;
        int has_GL_OES_single_precision;
        int has_GL_OES_stencil1;
        int has_GL_OES_stencil4;
        int has_GL_OES_stencil8;
        int has_GL_OES_stencil_wrap;
        int has_GL_OES_surfaceless_context;
        int has_GL_OES_texture_cube_map;
        int has_GL_OES_texture_env_crossbar;
        int has_GL_OES_texture_mirrored_repeat;
        int has_GL_OES_texture_npot;
        int has_GL_OES_vertex_array_object;
        int has_GL_QCOM_driver_control;
        int has_GL_QCOM_extended_get;
        int has_GL_QCOM_extended_get2;
        int has_GL_QCOM_perfmon_global_mode;
        int has_GL_QCOM_tiled_rendering;
        int has_GL_QCOM_writeonly_rendering;
    };
} glatter_extension_support_status_GL_t;


#define glatter_GL_AMD_compressed_3DC_texture glatter_get_extension_support_GL().has_GL_AMD_compressed_3DC_texture
#define glatter_GL_AMD_compressed_ATC_texture glatter_get_extension_support_GL().has_GL_AMD_compressed_ATC_texture
#define glatter_GL_APPLE_copy_texture_levels glatter_get_extension_support_GL().has_GL_APPLE_copy_texture_levels
#define glatter_GL_APPLE_framebuffer_multisample glatter_get_extension_support_GL().has_GL_APPLE_framebuffer_multisample
#define glatter_GL_APPLE_sync glatter_get_extension_support_GL().has_GL_APPLE_sync
#define glatter_GL_APPLE_texture_2D_limited_npot glatter_get_extension_support_GL().has_GL_APPLE_texture_2D_limited_npot
#define glatter_GL_APPLE_texture_format_BGRA8888 glatter_get_extension_support_GL().has_GL_APPLE_texture_format_BGRA8888
#define glatter_GL_APPLE_texture_max_level glatter_get_extension_support_GL().has_GL_APPLE_texture_max_level
#define glatter_GL_ARM_rgba8 glatter_get_extension_support_GL().has_GL_ARM_rgba8
#define glatter_GL_EXT_blend_minmax glatter_get_extension_support_GL().has_GL_EXT_blend_minmax
#define glatter_GL_EXT_debug_marker glatter_get_extension_support_GL().has_GL_EXT_debug_marker
#define glatter_GL_EXT_discard_framebuffer glatter_get_extension_support_GL().has_GL_EXT_discard_framebuffer
#define glatter_GL_EXT_map_buffer_range glatter_get_extension_support_GL().has_GL_EXT_map_buffer_range
#define glatter_GL_EXT_multi_draw_arrays glatter_get_extension_support_GL().has_GL_EXT_multi_draw_arrays
#define glatter_GL_EXT_multisampled_render_to_texture glatter_get_extension_support_GL().has_GL_EXT_multisampled_render_to_texture
#define glatter_GL_EXT_read_format_bgra glatter_get_extension_support_GL().has_GL_EXT_read_format_bgra
#define glatter_GL_EXT_robustness glatter_get_extension_support_GL().has_GL_EXT_robustness
#define glatter_GL_EXT_sRGB glatter_get_extension_support_GL().has_GL_EXT_sRGB
#define glatter_GL_EXT_texture_compression_dxt1 glatter_get_extension_support_GL().has_GL_EXT_texture_compression_dxt1
#define glatter_GL_EXT_texture_filter_anisotropic glatter_get_extension_support_GL().has_GL_EXT_texture_filter_anisotropic
#define glatter_GL_EXT_texture_format_BGRA8888 glatter_get_extension_support_GL().has_GL_EXT_texture_format_BGRA8888
#define glatter_GL_EXT_texture_lod_bias glatter_get_extension_support_GL().has_GL_EXT_texture_lod_bias
#define glatter_GL_EXT_texture_storage glatter_get_extension_support_GL().has_GL_EXT_texture_storage
#define glatter_GL_IMG_multisampled_render_to_texture glatter_get_extension_support_GL().has_GL_IMG_multisampled_render_to_texture
#define glatter_GL_IMG_read_format glatter_get_extension_support_GL().has_GL_IMG_read_format
#define glatter_GL_IMG_texture_compression_pvrtc glatter_get_extension_support_GL().has_GL_IMG_texture_compression_pvrtc
#define glatter_GL_IMG_texture_env_enhanced_fixed_function glatter_get_extension_support_GL().has_GL_IMG_texture_env_enhanced_fixed_function
#define glatter_GL_IMG_user_clip_plane glatter_get_extension_support_GL().has_GL_IMG_user_clip_plane
#define glatter_GL_KHR_debug glatter_get_extension_support_GL().has_GL_KHR_debug
#define glatter_GL_NV_fence glatter_get_extension_support_GL().has_GL_NV_fence
#define glatter_GL_OES_EGL_image glatter_get_extension_support_GL().has_GL_OES_EGL_image
#define glatter_GL_OES_EGL_image_external glatter_get_extension_support_GL().has_GL_OES_EGL_image_external
#define glatter_GL_OES_blend_equation_separate glatter_get_extension_support_GL().has_GL_OES_blend_equation_separate
#define glatter_GL_OES_blend_func_separate glatter_get_extension_support_GL().has_GL_OES_blend_func_separate
#define glatter_GL_OES_blend_subtract glatter_get_extension_support_GL().has_GL_OES_blend_subtract
#define glatter_GL_OES_byte_coordinates glatter_get_extension_support_GL().has_GL_OES_byte_coordinates
#define glatter_GL_OES_compressed_ETC1_RGB8_sub_texture glatter_get_extension_support_GL().has_GL_OES_compressed_ETC1_RGB8_sub_texture
#define glatter_GL_OES_compressed_ETC1_RGB8_texture glatter_get_extension_support_GL().has_GL_OES_compressed_ETC1_RGB8_texture
#define glatter_GL_OES_compressed_paletted_texture glatter_get_extension_support_GL().has_GL_OES_compressed_paletted_texture
#define glatter_GL_OES_depth24 glatter_get_extension_support_GL().has_GL_OES_depth24
#define glatter_GL_OES_depth32 glatter_get_extension_support_GL().has_GL_OES_depth32
#define glatter_GL_OES_draw_texture glatter_get_extension_support_GL().has_GL_OES_draw_texture
#define glatter_GL_OES_element_index_uint glatter_get_extension_support_GL().has_GL_OES_element_index_uint
#define glatter_GL_OES_extended_matrix_palette glatter_get_extension_support_GL().has_GL_OES_extended_matrix_palette
#define glatter_GL_OES_fbo_render_mipmap glatter_get_extension_support_GL().has_GL_OES_fbo_render_mipmap
#define glatter_GL_OES_fixed_point glatter_get_extension_support_GL().has_GL_OES_fixed_point
#define glatter_GL_OES_framebuffer_object glatter_get_extension_support_GL().has_GL_OES_framebuffer_object
#define glatter_GL_OES_mapbuffer glatter_get_extension_support_GL().has_GL_OES_mapbuffer
#define glatter_GL_OES_matrix_get glatter_get_extension_support_GL().has_GL_OES_matrix_get
#define glatter_GL_OES_matrix_palette glatter_get_extension_support_GL().has_GL_OES_matrix_palette
#define glatter_GL_OES_packed_depth_stencil glatter_get_extension_support_GL().has_GL_OES_packed_depth_stencil
#define glatter_GL_OES_point_size_array glatter_get_extension_support_GL().has_GL_OES_point_size_array
#define glatter_GL_OES_point_sprite glatter_get_extension_support_GL().has_GL_OES_point_sprite
#define glatter_GL_OES_query_matrix glatter_get_extension_support_GL().has_GL_OES_query_matrix
#define glatter_GL_OES_read_format glatter_get_extension_support_GL().has_GL_OES_read_format
#define glatter_GL_OES_required_internalformat glatter_get_extension_support_GL().has_GL_OES_required_internalformat
#define glatter_GL_OES_rgb8_rgba8 glatter_get_extension_support_GL().has_GL_OES_rgb8_rgba8
#define glatter_GL_OES_single_precision glatter_get_extension_support_GL().has_GL_OES_single_precision
#define glatter_GL_OES_stencil1 glatter_get_extension_support_GL().has_GL_OES_stencil1
#define glatter_GL_OES_stencil4 glatter_get_extension_support_GL().has_GL_OES_stencil4
#define glatter_GL_OES_stencil8 glatter_get_extension_support_GL().has_GL_OES_stencil8
#define glatter_GL_OES_stencil_wrap glatter_get_extension_support_GL().has_GL_OES_stencil_wrap
#define glatter_GL_OES_surfaceless_context glatter_get_extension_support_GL().has_GL_OES_surfaceless_context
#define glatter_GL_OES_texture_cube_map glatter_get_extension_support_GL().has_GL_OES_texture_cube_map
#define glatter_GL_OES_texture_env_crossbar glatter_get_extension_support_GL().has_GL_OES_texture_env_crossbar
#define glatter_GL_OES_texture_mirrored_repeat glatter_get_extension_support_GL().has_GL_OES_texture_mirrored_repeat
#define glatter_GL_OES_texture_npot glatter_get_extension_support_GL().has_GL_OES_texture_npot
#define glatter_GL_OES_vertex_array_object glatter_get_extension_support_GL().has_GL_OES_vertex_array_object
#define glatter_GL_QCOM_driver_control glatter_get_extension_support_GL().has_GL_QCOM_driver_control
#define glatter_GL_QCOM_extended_get glatter_get_extension_support_GL().has_GL_QCOM_extended_get
#define glatter_GL_QCOM_extended_get2 glatter_get_extension_support_GL().has_GL_QCOM_extended_get2
#define glatter_GL_QCOM_perfmon_global_mode glatter_get_extension_support_GL().has_GL_QCOM_perfmon_global_mode
#define glatter_GL_QCOM_tiled_rendering glatter_get_extension_support_GL().has_GL_QCOM_tiled_rendering
#define glatter_GL_QCOM_writeonly_rendering glatter_get_extension_support_GL().has_GL_QCOM_writeonly_rendering

