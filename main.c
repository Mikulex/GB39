#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vcruntime.h>

int main(int argc, const char **argv) {
  FILE *boot_file;
  uint8_t *boot_rom;

  if (argc != 2) {
    fprintf(stderr, "Incorrect usage\n");
    fprintf(stderr, "Usage: GB39.exe <path/to/boot_rom>");
    return 1;
  }

  fopen_s(&boot_file, argv[1], "rb");
  if (boot_file == NULL) {
    fprintf(stderr, "Failed to load boot rom %s", argv[1]);
    return 1;
  }

  fseek(boot_file, 0, SEEK_END);
  long f_size = ftell(boot_file);
  rewind(boot_file);
  printf("file size: %ld B\n", f_size);

  boot_rom = (uint8_t *)malloc(f_size);
  if (boot_rom == NULL) {
    fprintf(stderr, "failed to allocate memory\n");
    return 1;
  }

  size_t result = fread(boot_rom, sizeof(uint8_t), f_size, boot_file);
  printf("read: %zu B \n", result);
  if (result != f_size) {
    fprintf(stderr, "failed to read file");
  }

  fclose(boot_file);

  for (int i = 0; i < f_size; i++) {
    printf("%02x ", boot_rom[i]);
  }
  printf("\n");
  free(boot_rom);
}
