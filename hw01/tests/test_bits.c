#include "bits.h"
#include "status.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int failures;

static void check(const char *name, int passed) {
	if (passed) {
		printf("PASS: %s\n", name);
	} else {
		printf("FAIL: %s\n", name);
		failures++;
	}
}

static void test_print_binary(void) {
	FILE *stream = tmpfile();
	char output[128];
	size_t length;

	if (stream == NULL) {
		check("print_binary output setup", 0);
		return;
	}

	print_binary_to(stream, 1, 1);
	print_binary_to(stream, 0xf0a55a0f, 32);
	fflush(stream);
	rewind(stream);
	length = fread(output, 1, sizeof(output) - 1, stream);
	output[length] = '\0';
	fclose(stream);
	check("print_binary output",
		  strcmp(output,
				 "1\n1111 0000 1010 0101 0101 1010 0000 1111\n") == 0);
}

static void test_get_field(void) {
	check("get_field width 1 at pos 31", get_field(0x80000000u, 31, 1) == 1);
	check("get_field width 32", get_field(0xdeadbeefu, 0, 32) == 0xdeadbeefu);
	check("get_field rejects field past bit 31", get_field(0xffffffffu, 31, 2) == 0);
}

static void test_set_field(void) {
	check("set_field width 1 at pos 31", set_field(0, 31, 1, 1) == 0x80000000u);
	check("set_field masks a value too wide", set_field(0, 4, 3, 0xff) == 0x70);
	check("set_field width 32", set_field(0, 0, 32, 0xdeadbeefu) == 0xdeadbeefu);
}

static void test_sign_extend(void) {
	check("sign_extend width 1 most negative", sign_extend(1, 1) == -1);
	check("sign_extend width 8 most negative", sign_extend(0x80, 8) == -128);
	check("sign_extend width 32", sign_extend(0x80000000u, 32) == INT32_MIN);
}

static void test_status_unpack(void) {
	status_t zero = status_unpack(0x0000);
	check("status_unpack zero word",
		  !zero.heat && !zero.cool && !zero.fan && !zero.fault &&
			  zero.mode == MODE_OFF && !zero.reserved && zero.setpoint == 0);

	status_t all_set = status_unpack(0xffff);
	check("status_unpack all bits set",
		  all_set.heat && all_set.cool && all_set.fan && all_set.fault &&
			  all_set.mode == MODE_INVALID && all_set.reserved &&
			  all_set.setpoint == -1);

	status_t example = status_unpack(0x1631);
	check("status_unpack 0x1631",
		  example.heat && !example.cool && !example.fan && !example.fault &&
			  example.mode == MODE_AUTO && !example.reserved &&
			  example.setpoint == 22);
}

int main(void) {
	test_print_binary();
	test_get_field();
	test_set_field();
	test_sign_extend();
	test_status_unpack();

	if (failures == 0) {
		printf("SUMMARY: all tests passed\n");
	} else {
		printf("SUMMARY: %d test(s) failed\n", failures);
	}
	return failures == 0 ? 0 : 1;
}
