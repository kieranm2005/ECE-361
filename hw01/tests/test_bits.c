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
	/* Check the exact output for the minimum and maximum supported widths. */
	check("print_binary output",
		  strcmp(output,
				 "1\n1111 0000 1010 0101 0101 1010 0000 1111\n") == 0);
}

static void test_get_field(void) {
	/* A one-bit field at position 31 exercises the highest valid bit. */
	check("get_field width 1 at pos 31", get_field(0x80000000u, 31, 1) == 1);
	/* Width 32 must return the entire word without an invalid shift. */
	check("get_field width 32", get_field(0xdeadbeefu, 0, 32) == 0xdeadbeefu);
	/* A field extending past bit 31 is invalid and must return zero. */
	check("get_field rejects field past bit 31", get_field(0xffffffffu, 31, 2) == 0);
}

static void test_set_field(void) {
	/* Setting a one-bit field at position 31 must set the unsigned sign bit. */
	check("set_field width 1 at pos 31", set_field(0, 31, 1, 1) == 0x80000000u);
	/* Values wider than the field are truncated to the field width. */
	check("set_field masks a value too wide", set_field(0, 4, 3, 0xff) == 0x70);
	/* Width 32 must replace the complete word. */
	check("set_field width 32", set_field(0, 0, 32, 0xdeadbeefu) == 0xdeadbeefu);
}

static void test_sign_extend(void) {
	/* The single-bit value 1 is the most negative one-bit signed value. */
	check("sign_extend width 1 most negative", sign_extend(1, 1) == -1);
	/* 0x80 is the most negative value representable in eight bits. */
	check("sign_extend width 8 most negative", sign_extend(0x80, 8) == -128);
	/* Width 32 preserves the full two's-complement value. */
	check("sign_extend width 32", sign_extend(0x80000000u, 32) == INT32_MIN);
}

static void test_status_unpack(void) {
	status_t zero = status_unpack(0x0000);
	/* An all-zero word should decode to the inactive default status. */
	check("status_unpack zero word",
		  !zero.heat && !zero.cool && !zero.fan && !zero.fault &&
			  zero.mode == MODE_OFF && !zero.reserved && zero.setpoint == 0);

	status_t all_set = status_unpack(0xffff);
	/* All flags set also exercises invalid modes and a negative setpoint. */
	check("status_unpack all bits set",
		  all_set.heat && all_set.cool && all_set.fan && all_set.fault &&
			  all_set.mode == MODE_INVALID && all_set.reserved &&
			  all_set.setpoint == -1);

	status_t example = status_unpack(0x1631);
	/* This representative word checks the documented mixed status fields. */
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
