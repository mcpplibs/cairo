# What is missing from this `upstream/`, and why

This is cairo 1.18.2's release tree with **two directories removed**:

| removed | size | why |
|---|---|---|
| `test/` | 61 MB | cairo's reference-image suite — thousands of PNGs |
| `perf/` | 1.3 MB | the performance harness |

Neither is compiled by this package: `mcpp/cairo/mcpp.toml` builds `src/` and
nothing else. Keeping them made the published archive **47 MB**, which every
consumer would download to use 6 MB of source.

Everything a build touches is here and byte-identical to upstream. The fork's
CI checks exactly that — it diffs this tree against the release tarball with
those two directories excluded, so a change to any file that *does* matter is
still caught.

If you need the test suite, it is in cairo's own release:
<https://gitlab.freedesktop.org/cairo/cairo/-/archive/1.18.2/cairo-1.18.2.tar.gz>
