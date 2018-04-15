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

#include <stdio.h>
#include "kiwi.h"

static sink *sinks[];

static int add_sink(FILE *fd) {

}

sink *kl_create_sink (FILE *fd, ...){
	va_list options;
	add_sink(fd);
}



// Reset all filters. Starts logging everything to this sink.
void kl_sink_everything(sink *s) {

}


// XXX remove it
static void add_to_record (kl_pair *kv){
	// XXX формировать запись, учесть доп форматы (JSON etc)
	switch (kv->type) {
	case KL_PAIR_STRING:
		printf("%s STR %s\n", kv->key, kv->sval);
		break;
	case KL_PAIR_INT:
		printf("%s INT %i\n", kv->key, kv->ival);
		break;
	case KL_PAIR_FLOAT:
		printf("%s FLOAT %f\n", kv->key, kv->fval);
		break;
	case KL_PAIR_DOUBLE:
		printf("%s DOUBLE %f\n", kv->key, kv->dval);
		break;
	case KL_PAIR_TIME:
		printf("%s TIME %s\n", kv->key, kv->sval);
		break;
	}
}

void kl__sink_this(record *rec) {
	// создается копия со списком синков (1)
	// создается пустой массив под активные выходы (2)
	for (int i = 0; i<rec->used; i++) {
		// прогон по фильтрам, в какой синк подходит, он переносится из 1 в 2
		// далее проверка идет по 1 уже без учета отфильтрованных (уже активированных) синков
		// если попадет в запрещающий фильтр, то синк выкидывается из 1 и 2

		printf("%i ", i); // XXX
		add_to_record(rec->array[i]); // XXX debug time

	}

	// если список активных синков не пустой, подготовка строки из записи
	// цикл по активным синкам, выдача
}
