#include<stdio.h>

int findTheLongestBalancedSubstring(char * s){
    int i = 0, z0 = 0, z1 = 0, length0, length1;
    char c;
    int longest = 0;
    int zeroMod = 0;
    while((c=*s) != '\0') {
        if (c == '0') {
            if (zeroMod) {
                
            } else {
                zeroMod = 1;
                
                length0 = z1 - z0;
                length1 = i - z1;
                if (length0 > length1) {
                    length0 = length1;
                }
                length0 *= 2;
                if (length0 > longest) {
                    longest = length0;
                }

                z0 = i;
            }            
        } else {
            if (zeroMod) {
                zeroMod = 0;
                z1 = i;
            } else {
                
            }
        }
        ++i;
        ++s;
    }
    length0 = z1 - z0;
    length1 = i - z1;
    if (length0 > length1) {
        length0 = length1;
    }
    length0 *= 2;
    if (length0 > longest) {
        longest = length0;
    }
    return longest;
}


int main() {
    printf("%s %d\n", "01000111", findTheLongestBalancedSubstring("01000111"));
    printf("%s %d\n", "00111", findTheLongestBalancedSubstring("00111"));
    printf("%s %d\n", "111", findTheLongestBalancedSubstring("111"));
}