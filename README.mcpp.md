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
import cairo;
```

470 names, grouped behind the same `CAIRO_HAS_*` macros the features define — so
the module and the manifest cannot disagree about what is in the build.

## ⚠️ The module is named for the project, not for its host

`import cairo;` — **not** `freedesktop.cairo`. cairo is its own project;
freedesktop.org hosts the git and does not own the interface. The index
namespace (`freedesktop.cairo`) is a *shelf label* and never enters the module
name — the same reason `freedesktop.egl` exports `khronos.egl`.

This changed in this release. A consumer on the old name gets a compile error
naming the module, which is the right place to find out.

## ⭐ The module exported 470 names and was missing the two that mattered

Found from *outside*, by `gnome.pangocairo` — which cannot mix the two routes
and therefore had to ask the module for `cairo_t` and got nothing:

| | |
|---|---|
| `cairo_t` | `typedef struct _cairo cairo_t;`. The typedef regex required at least one character between `cairo_` and `_t`, so the single most-used type in the library was the one name it could not match. |
| 192 enumerators | every `CAIRO_FORMAT_*`, `CAIRO_STATUS_*`, `CAIRO_OPERATOR_*`. The scan never looked inside a `typedef enum` body. |

**Nobody noticed because the only test that imported the module also wrote
`#include <cairo.h>`,** so the header supplied whatever the module lacked. An
import that is never asked to stand on its own is not tested, it is decorated.

The tests are now two files — `tests/cairo.cpp` includes and does not import,
`tests/module.cpp` imports and includes nothing — and the export count is 697.
