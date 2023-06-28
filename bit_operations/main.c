#include <stdio.h>


int main() {
    int a = 0x00CCAABB;
    int b = a;
    printf("Number: %x\n", a);

    for (int i = 0; i < 4; i++) {
        printf("%x\n", (a & 0xFF));
        a = a >> 8;
    }
    // меняем 5-ый и 6-ой байт на DD
    b = b | 0x10000;
    b = b | 0x100000;
    printf("Number: %x\n", b);

    return 0;
}