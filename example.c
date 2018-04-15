#include "kiwi.h"

int main(int argc, char *argv[])
{
	kl_create_sink(stderr);

	// Just logs key="sample value" key2="another value"
	kl_log(kl_s("key", "sample value"), kl_s("key2", "another value"));

	// From this moment use timestamp for the records
	kl_with_ts("at");

	// Short alias for kl_log(kl_sv())
	kl_logs("key", "first value", "key2", "second value");

	// Output different types in the record
	kl_log(kl_s("key", "string value"), kl_i("key2", 12345678), kl_f("key3", 3.14159265359));

	return 0;
}
