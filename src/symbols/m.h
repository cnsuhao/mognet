#pragma once

// Contains all the `libm` symbols required by the library
// (there may be extras)
const char* m_symbol_table[] = {
    "acos",   "acosh",      "asin",       "asinf",      "asinh",      "atan2",
    "atan2f", "atan",       "atanf",      "atanh",      "cbrt",       "ceil",
    "ceilf",  "cos",        "cosf",       "cosh",       "erf",        "erfc",
    "exp2",   "exp",        "expf",       "expm1",      "floor",      "floorf",
    "fmod",   "fmodf",      "fputs",      "fwrite",     "hypot",      "ldexp",
    "lgamma", "llrint",     "log10",      "log1p",      "log",        "logb",
    "lrint",  "modf",       "nearbyint",  "nextafter",  "nextafterf", "pow",
    "powf",   "qsort",      "remainder",  "remquo",     "rint",       "roundf",
    "scalbn", "sin",        "sinf",       "sinh",       "sqrt",       "tan",
    "tanf",   "tanh",       "tgamma",     "logf",       "isnanf",     "isnan",
    "lrintf", "tanhf",      "sqrtf",      "remainderf", "fminf",      "fmaxf",
    "round",  "fegetround", "fesetround", "hypotf",     NULL};
