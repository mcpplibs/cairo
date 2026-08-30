/* cairo-features.h — which backends this build has.
 *
 * NOT generated: upstream's meson writes it from `feature_conf`, and the values
 * are decisions about what to build rather than a transform over any file. The
 * ALWAYS-ON set is here; everything optional arrives as a `-D` from the feature
 * that provides it, so this header and `[features]` in mcpp.toml cannot drift.
 *
 * The five below are not features because turning them off produces a cairo
 * that cannot do its job: image surfaces are where the software rasterizer
 * draws, recording surfaces are how `cairo_push_group` works, and the user-font
 * backend is what a toolkit installs its own glyph callbacks through.
 */
#ifndef CAIRO_FEATURES_H
#define CAIRO_FEATURES_H

#define CAIRO_HAS_IMAGE_SURFACE 1
#define CAIRO_HAS_RECORDING_SURFACE 1
#define CAIRO_HAS_MIME_SURFACE 1
#define CAIRO_HAS_OBSERVER_SURFACE 1
#define CAIRO_HAS_USER_FONT 1

#endif
