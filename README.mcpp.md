# cairo, packaged for mcpp

2D vector drawing — paths, strokes, fills, text runs, and surfaces to put them
on. `pango` lays text out and cairo renders it; a compositor that only
composites does not need it, a desktop **shell** does.

`upstream/` is cairo 1.18.2, unmodified. `mcpp/` is the adaptation.

## Easier than its size suggests

104k lines of C and **no code generation at all**: upstream's meson emits
exactly two artifacts, `config.h` and `cairo-features.h`, and both are
`configure_file` — probe answers, not generated code.

So there is no generator to transliterate. `build.mcpp` exists only for the
module wrapper, because the export set depends on which features are on.

> Package difficulty is decided by GENERATORS, not by line count. fontconfig is
> a quarter the size and needed seven.

## The backends are features, and X11 is off

Upstream makes each backend a `get_option()`, which means a distribution decides
once for everyone. An index cannot: a Wayland compositor and an X11 application
want different cairos out of the same package.

```toml
default = ["ft", "fc", "png"]     # Wayland-first
```

`xlib` and `xcb` are off. Someone drawing a circle in a Wayland program should
not acquire libX11 to do it; someone who wants X11 writes
`cairo = { features = ["xlib"] }`.

Each feature's `sources` are listed **file by file**, never as a glob — feature
source selection matches literal entries, so a glob inside a feature block would
compile that backend into every consumer.

## Two probe answers worth knowing

**`WORDS_BIGENDIAN` and `FLOAT_WORDS_BIGENDIAN` must be ABSENT, not 0.** cairo
tests them with `#ifdef` (`cairoint.h:196`, `cairo-fixed-private.h:124`), so
`#define WORDS_BIGENDIAN 0` says big-endian. On x86-64 the result is that
everything compiles, links, reports success, `cairo_paint` works — and every
*path* silently gets garbage fixed-point coordinates:

```
cairo_rectangle(cr, 4, 4, 16, 16)  ->  path_extents  -8.03e+06 … 4.37e+06
cairo_in_fill(cr, 40, 40)          ->  1     (the point is outside)
cairo_fill / cairo_stroke          ->  0 pixels changed, status SUCCESS
```

**`HAVE_UINT128_T` must be absent too.** cairo's probe looks for the type spelled
`uint128_t`, which no C library provides — gcc and clang have `__uint128_t`, a
different identifier. Claiming it typedefs from a type that does not exist.

## Using it

```cpp
import freedesktop.cairo;
```

470 names, grouped behind the same `CAIRO_HAS_*` macros the features define — so
the module and the manifest cannot disagree about what is in the build.
