#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXTRA_SIZE 256
#define BLOCK_SIZE 128
#define BUF_SIZE 1024

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];

  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);

  if (len < 0) {
    handle_error("snprintf");
  }

  write(STDOUT_FILENO, buf, len);
}

int main(void) {
  // Increase heap by 256 bytes
  void *heap_start = sbrk(EXTRA_SIZE);

  if (heap_start == (void *)-1) {
    handle_error("sbrk");
  }

  // First block starts at the beginning of new heap space
  struct header *first_block = (struct header *)heap_start;

  // Second block starts 128 bytes after the first block
  struct header *second_block = (struct header *)((char *)heap_start + BLOCK_SIZE);

  // Initialize first block header
  first_block->size = BLOCK_SIZE;
  first_block->next = NULL;

  // Initialize second block header
  second_block->size = BLOCK_SIZE;
  second_block->next = first_block;

  // Data starts right after each header
  char *first_data = (char *)first_block + sizeof(struct header);
  char *second_data = (char *)second_block + sizeof(struct header);

  // Data size excludes the header
  size_t data_size = BLOCK_SIZE - sizeof(struct header);

  // Fill first block data with 0s
  memset(first_data, 0, data_size);

  // Fill second block data with 1s
  memset(second_data, 1, data_size);

  // Print block addresses
  print_out("first block:       %p\n", &first_block, sizeof(first_block));
  print_out("second block:      %p\n", &second_block, sizeof(second_block));

  // Print first block header values
  print_out("first block size:  %lu\n", &first_block->size, sizeof(first_block->size));
  print_out("first block next:  %p\n", &first_block->next, sizeof(first_block->next));

  // Print second block header values
  print_out("second block size: %lu\n", &second_block->size, sizeof(second_block->size));
  print_out("second block next: %p\n", &second_block->next, sizeof(second_block->next));

  // Print first block data
  for (size_t i = 0; i < data_size; i++) {
    uint64_t value = first_data[i];
    print_out("%lu\n", &value, sizeof(value));
  }

  // Print second block data
  for (size_t i = 0; i < data_size; i++) {
    uint64_t value = second_data[i];
    print_out("%lu\n", &value, sizeof(value));
  }

  return 0;
}
