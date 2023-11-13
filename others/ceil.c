#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

// #define inline 1
#ifdef inline
#define attr __attribute__((noinline))
#else
#define attr /**/
#endif

static attr int ceil0(int a, int b) {
/*
ceil0:
        mov     eax, edi
        cdq
        idiv    esi
        cmp     edx, 1
        sbb     eax, -1
        ret
*/
  int r = a / b;
  if (a % b != 0) {
    ++r;
  }
  return r;
}

static attr int ceil1(int a, int b) {
/*
ceil1:
        lea     eax, [rsi-1+rdi]
        cdq
        idiv    esi
        ret
*/
  return (a + b - 1) / b;
}

static attr int ceil2(int a, int b) {
/*
ceil2:
        lea     eax, [rdi-1]
        cdq
        idiv    esi
        add     eax, 1
        ret
*/
  return (a - 1) / b + 1;
}

static double differtime(struct timespec* t1, struct timespec* t2) {
  return (t1->tv_sec - t2->tv_sec) * 1000. +
         (t1->tv_nsec - t2->tv_nsec) / 1000. / 1000.;
}

int main() {
  int n = 1000000 + (rand() % 100), m = 1000 + (rand() % 10), s = 0;
  struct timespec t1, t2;
  clock_gettime(CLOCK_MONOTONIC, &t1);
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < m; j++) {
        s += ceil0(i, j);
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &t2);
  printf("time %0.3lf ms -> %d\n", differtime(&t2, &t1), s);

  s = 0;
  clock_gettime(CLOCK_MONOTONIC, &t1);
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < m; j++) {
        s += ceil1(i, j);
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &t2);
  printf("time %0.3lf ms -> %d\n", differtime(&t2, &t1), s);

  s = 0;
  clock_gettime(CLOCK_MONOTONIC, &t1);
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < m; j++) {
        s += ceil2(i, j);
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &t2);
  printf("time %0.3lf ms -> %d\n", differtime(&t2, &t1), s);
}

/**
static int ceil0(int a, int b) {
  int r = a / b;
  if (a % b != 0) {
    ++r;
  }
  return r;
}

static int ceil1(int a, int b) {
  return (a + b - 1) / b;
}
#include <stdlib.h>
int main() {
  int i = rand(), j = rand(), i2 = rand(), j2 = rand(), s1 = 0, s2 = 0;
  s2 = ceil1(i2, j2);
  s1 = ceil0(i, j);
  return s1+s2;
}

// https://godbolt.org/
// gcc 13.2 -O2

main:
        push    r12
        push    rbp
        push    rbx
        call    rand
        mov     ebx, eax ; ebx i
        call    rand
        mov     ebp, eax ; ebp j
        call    rand
        mov     r12d, eax ; r12d i2
        call    rand
        mov     ecx, eax  ; ecx  j2
        lea     eax, [r12-1+rax] ; i2 - 1 + j2
        cdq
        idiv    ecx ; (i2 - 1 + j2) / j2
        mov     ecx, eax ; ecx = s2 = ceil1(i2, j2)

        mov     eax, ebx ; eax = i
        pop     rbx ; recover
        cdq
        idiv    ebp ; i / j
        pop     rbp ; recover
        pop     r12 ; revocer
        cmp     edx, 1 ; (a % b - 1) ;  CF carry
        sbb     eax, -1 ; a / b + 1 - CF
        add     eax, ecx ; s1 + s2
        ret

本文源码

static int ceil0(int a, int b) {
  int r = a / b;
  if (a % b != 0) {
    ++r;
  }
  return r;
}

        mov     eax, ecx
        cdq
        idiv    ebx
        cmp     edx, 1
        sbb     eax, -1
        add     ebp, eax

static int ceil1(int a, int b) {
  return (a + b - 1) / b;
}

        lea     esi, [rdi-1]
.L6:
        lea     eax, [rsi+rcx]
        cdq
        idiv    ecx
*/