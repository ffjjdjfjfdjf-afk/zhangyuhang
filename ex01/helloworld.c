#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    
    char input[100];
    printf("请输入内容: ");
    scanf("%s", &input);
    
    printf("您输入的内容是: %s\n", input);
    
    return 0;
}
