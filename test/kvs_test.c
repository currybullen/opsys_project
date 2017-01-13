#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "kvs.h"

static int TEST_KEY_1 = 55;
static int TEST_VAL_1 = 76;
static int TEST_KEY_2 = 5255;
static int TEST_VAL_2 = 1976;
static int TEST_KEY_3 = 6464;
static int TEST_VAL_3 = 1515;

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Missing device driver location!\n");
        exit(1);
    }

    int fd = open(argv[1], 0);
    if (fd < 0) {
        printf("Bad device driver path '%s'!\n", argv[1]);
        exit(2);
    }

    int response;

    printf("Attempting to store value %d behind key %d\n", TEST_KEY_1, TEST_VAL_1);
    response = put(fd, TEST_KEY_1, TEST_VAL_1);
    if (response == FAIL) {
        printf("Could not store value %d behind key %d, something went wront, exiting\n", TEST_KEY_1, TEST_VAL_1);
        exit(3);
    }
    printf("Successfully stored value %d behind key %d\n", TEST_KEY_1, TEST_VAL_1);
    printf("\n");

    printf("Attempting to store value %d behind key %d\n", TEST_KEY_2, TEST_VAL_2);
    response = put(fd, TEST_KEY_2, TEST_VAL_2);
    if (response == FAIL) {
        printf("Could not store value %d behind key %d, something went wront, exiting\n", TEST_KEY_2, TEST_VAL_2);
        exit(4);
    }
    printf("Successfully stored value %d behind key %d\n", TEST_KEY_2, TEST_VAL_2);
    printf("\n");

    int val;

    printf("Attempting to retrieve value behind key %d\n", TEST_KEY_1);
    response = get(fd, TEST_KEY_1, &val);
    if (response == FAIL) {
        printf("Could not retrieve value behind key %d, something went wrong, exiting\n", TEST_KEY_1);
        exit(5);
    } 
    printf("Found value %d behind key %d\n", val, TEST_KEY_1);
    printf("\n");

    printf("Attempting to retrieve value behind key %d.\n", TEST_KEY_2);
    response = get(fd, TEST_KEY_2, &val);
    if (response == FAIL) {
        printf("Could not retrieve value behind key %d, something went wrong, exiting\n", TEST_KEY_2);
        exit(5);
    } 
    printf("Found value %d behind key %d\n", val, TEST_KEY_2);
    printf("\n");

    
    printf("Attempting to delete value behind key %d.\n", TEST_KEY_1);
    response = delete(fd, TEST_KEY_1);
    if (response == FAIL) {
        printf("Could not delete value behind key %d, something went wrong, exiting\n", TEST_KEY_1);
        exit(6);
    } 
    printf("Deleted value behind key %d\n", TEST_KEY_1);
    printf("\n");

    printf("Attempting to retrieve value behind key %d\n", TEST_KEY_1);
    response = get(fd, TEST_KEY_1, &val);
    if (response == SUCCESS) {
        printf("Was able to retrieve value behind key %d, something went wrong, exiting\n", TEST_KEY_1);
        exit(7);
    } 
    printf("Found no value behind key %d\n", TEST_KEY_1);
    printf("\n");

    printf("Attempting to delete value behind key %d.\n", TEST_KEY_2);
    response = delete(fd, TEST_KEY_2);
    if (response == FAIL) {
        printf("Could not delete value behind key %d, something went wrong, exiting\n", TEST_KEY_2);
        exit(8);
    } 
    printf("Deleted value behind key %d\n", TEST_KEY_2);
    printf("\n");

    printf("Attempting to retrieve value behind key %d\n", TEST_KEY_2);
    response = get(fd, TEST_KEY_2, &val);
    if (response == SUCCESS) {
        printf("Was able to retrieve value behind key %d, something went wrong, exiting\n", TEST_KEY_2);
        exit(9);
    } 
    printf("Found no value behind key %d\n", TEST_KEY_2);
    printf("\n");

    printf("Attempting to store value %d behind key %d\n", TEST_KEY_3, TEST_VAL_3);
    response = put(fd, TEST_KEY_3, TEST_VAL_3);
    if (response == FAIL)
        printf("Could not store value %d behind key %d, was the value stored previously?\n", TEST_KEY_3, TEST_VAL_3);
    else
        printf("Successfully stored value %d behind key %d\n", TEST_KEY_3, TEST_VAL_3);
    printf("\n");

    printf("Attempting to retrieve value behind key %d\n", TEST_KEY_3);
    response = get(fd, TEST_KEY_3, &val);
    if (response == FAIL) {
        printf("Could not retrieve value behind key %d, something went wrong, exiting\n", TEST_KEY_3);
        exit(5);
    } 
    printf("Found value %d behind key %d\n", val, TEST_KEY_3);
    printf("\n");
}
