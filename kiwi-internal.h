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

   ॐ तारे तुत्तारे तुरे स्व
 */

/* Internal data structures used by logger functions but they are not
 * required for using logger in your application. It is enough use
 * functions declared in "kiwi.h". So right way is include "kiwi.h"
 * for using `kiwi` in the application. Anyway due library logic the
 * "kiwi-internal.h" will be included implicitely. */

/** Logger interface **
 */
enum kl_pairval {KL_PAIR_STRING, KL_PAIR_INT, KL_PAIR_FLOAT, KL_PAIR_DOUBLE, KL_PAIR_TIME};
typedef struct {
	enum kl_pairval type;
	char *          key;
	char *          sval;
	int             ival;
	float           fval;
	double          dval;
} kl_pair;

/* For avoid varargs limitations each function that allows variadic
 * parameters redefined with NULL at the end. You should use functions
 * from #defines and don't use `_varg` functions!
 */
void kl__log_varg(kl_pair *kv, ...); // use kl_log() instead!
void kl__logs_varg(char *key, char *val, ...); // use kl_logs() instead!

/*
** Log record
*/
typedef struct {
	kl_pair **array;
	size_t    used;
	size_t    size;
	time_t *  at;
} kl_record;

int kl__record_init(kl_record *a, size_t initial_size);
int kl__record_append(kl_record *a, kl_pair *pair);
void kl__record_free(kl_record *a);

/* Internal function. */
void kl__sink_this(kl_record *rec);


