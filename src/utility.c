unsigned int rand_r(int *seed) {
    // Constants are from the POSIX specification for rand_r
    *seed = *seed * 1103515245 + 12345;
    return (*seed >> 16) & 0x7FFF;
}