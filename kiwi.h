/*
   This file is part of Kiwi Logger (KL) — the library for logging.

   Copyright (c) 2018, Alexander I.Grafov <grafov@gmail.com>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of kvlog nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   Usage example:
     kl_log("key1", kl_s("sample"), "key2", kl_i(123), "key3", kl_f(3.141592));

   It will output something like:
     key1="sample" key2=123 key3=3.141592

   If you would output it in traditional way with levels:
     kl_info("key", "string value")

   Output will look like:
     level=INFO key="string value"

   ॐ तारे तुत्तारे तुरे स्व
 */

#ifndef KIWI_H
#define KIWI_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "kiwi-internal.h"

/* Internal data structures used by logger functions but they are not
 * required for using logger in your application. It is enough use
 * functions declared in "kiwi.h". So right way is include "kiwi.h"
 * for using `kiwi` in the application. */

#if __STRICT_ANSI__ // by recipe from https://snai.pe/c/preprocessor/varargs/
 #define kl_log(pair, ...) kl__log_varg(pair, __VA_ARGS__, (kl_pair *)NULL)
 #define kl_logs(k, v, ...) kl__logs_varg(k, v, __VA_ARGS__, (char *)NULL)
#else
 #define kl_log(pair, pairs ...) kl__log_varg(pair, ## pairs, (kl_pair *)NULL)
 #define kl_logs(k, v, pairs ...) kl__logs_varg(k, v, ## pairs, (char *)NULL)
#endif

/* Helpers for adding key-value pairs to the log record.
 */
kl_pair *kl_s(char *key, char *val);
kl_pair *kl_i(char *key, int val);
kl_pair *kl_f(char *key, float val);
kl_pair *kl_d(char *key, double val);

/* Setup timestamp output as additional context for the log records.
 * Second argument is time kl_format. Default kl_format is %H:%M:%S.
 */
void kl_with_ts(char *key, ...);
void kl_without_ts();

/* Emulate levels for compatibility with traditional loggers.
 */
void kl_level_key(char *key);
void kl_trace(kl_pair *kv, ...);
void kl_debug(kl_pair *kv, ...);
void kl_info(kl_pair *kv, ...);
void kl_warn(kl_pair *kv, ...);
void kl_error(kl_pair *kv, ...);
void kl_fatal(kl_pair *kv, ...);

/*
** Sink interface
*/
enum match_type {FILTER_MATCH_KEY, FILTER_RANGE_INT};

typedef struct kl_filter {
	enum match_type   type;
	char *            name;
	char *            val;
	int               from;
	int               to;
	struct kl_filter *prev;
	struct kl_filter *next;
} kl_filter;

typedef struct kl_sink {
	FILE *            fd;
	kl_filter *       filter_set;
	struct kl_format *fmt;
	struct kl_sink *  linked;
} kl_sink;

/* Formatting
 */
typedef struct kl_format {
	int (*start)(FILE *, kl_record *); // at start of the record
	int (*pair)(FILE *, kl_pair *); // format single pair
	int (*finish)(FILE *, kl_record *); // at the end of the record
} kl_format;

int *kl_logfmt_start(FILE *, kl_record *);
int *kl_logfmt_pair(FILE *, kl_pair *);
int *kl_logfmt_finish(FILE *, kl_record *);

typedef struct {
	kl_filter *cond;
	kl_format *fmt;
} kl_option;

/* Create a new kl_sink. By default the kl_sink will pass everything until
 * the filters applied. Arbitrary number of options could be passed
 * for modifying default kl_sink settings. It could be filtering options
 * or reference to alternative formatter (all options are references
 * to `option` type instances. */
kl_sink *kl_create_sink(FILE *fd, ...);

/* Change kl_sink settings by adding or removing the filters or changing
 * the output kl_format. Arbitrary number of options could be passed for
 * modifying default kl_sink settings. It could be filtering options or
 * reference to alternative formatter (all options are references to
 * `option` type instances. */
void kl_modify_sink(kl_sink *s, ...);

/* Reset all the filters for the kl_sink. By default the kl_sink will output
 * all passed records. Filtering options will not reset. */
void kl_sink_everything(kl_sink *s);

/* Closes the kl_sink and underlaying file. Return 0 on success or error code. */
int kl_close_sink(kl_sink *s);

/*
** XXX Sink interface
   kl_filter must_key(char* key);
   kl_filter must_sval(char* key, char* val);
   kl_filter must_irange(char* key, int from, int to);

   kl_filter with_key(char* key);
   kl_filter without_key(char* key);
   kl_filter with_sval(char* key, char* val);
   kl_filter without_sval(char* key, char* sval);
   kl_filter with_irange(char* key, int from, int to);
   kl_filter without_irange(char* key, int from, int to);
*/

#endif
