#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_ELEMENTS 100

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <device_path> <data_file>\n", argv[0]);
        return -1;
    }

    const char *device_path = argv[1];
    const char *data_file = argv[2];

    int fd = open(device_path, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        return -1;  // Return immediately on error
    }

    FILE *file = fopen(data_file, "r");
    if (!file) {
        perror("Failed to open data file");
        close(fd);
        return -1;
    }

    size_t n_elements = N_ELEMENTS;
    size_t size = n_elements * sizeof(int);
    int *inbuf = malloc(size);
    if (!inbuf) {
        perror("Failed to allocate memory for input buffer");
        fclose(file);
        close(fd);
        return -1;
    }

    // Read data from file into inbuf
    size_t idx = 0;
    while (idx < n_elements && fscanf(file, "%d", &inbuf[idx]) == 1) {
        idx++;
    }

    if (idx != n_elements) {
        fprintf(stderr,
                "Failed to read enough data from file. Expected %zu elements, "
                "but got %zu.\n",
                n_elements, idx);
        free(inbuf);
        fclose(file);
        close(fd);
        return -1;
    }

    fclose(file);  // Close the file after reading

    // Send data to the device for sorting
    ssize_t r_sz = read(fd, inbuf, size);
    if (r_sz != size) {
        perror("Failed to process data through character device");
        free(inbuf);
        close(fd);
        return -1;
    }

    // Verify the result of sorting
    bool pass = true;
    for (size_t i = 1; i < n_elements; i++) {
        if (inbuf[i] < inbuf[i - 1]) {
            pass = false;
            break;
        }
    }

    printf("Sorting %s for file %s!\n", pass ? "succeeded" : "failed",
           data_file);

    free(inbuf);
    close(fd);
    return 0;  // Return 0 on successful execution
}
