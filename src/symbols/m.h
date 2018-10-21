/*
 * Optical Character Recognition for Latin-based languages
 * Copyright (C) 2018  meme
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
