// sse_sample1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"

ostream &operator<<(ostream &os, __m128 m) {
  os << m.m128_f32[0] << " " << m.m128_f32[1] << " " << m.m128_f32[2] << " " << m.m128_f32[3];
  return os;
}
void add1() {
  __m128 s1 = { 1.2, 2, 3, 4 };
  __m128 s2 = { 1, 1, 1, 1 };

  float __declspec(16) myfloat4[4] = { 1.0f, 2.0f, 3.0f, 4.0f, }; // out in global scope
  __m128 s3 = _mm_add_ps(s1, s2);
  //cout << "s1=" << s1 << endl;
  //cout << "s2=" << s2 << endl;
  //cout << "s3=" << s3 << endl;
}

void add2() {
  __m128 s1, s2, s3;
  __declspec(16) float a[4] = { 1.2f, 2.0f, 3.0f, 4.0f, };
  __declspec(16) float b[4] = { 1.0f, 2.0f, 3.0f, 4.0f, };
  s1 = _mm_load_ps(a);
  s2 = _mm_load_ps(b);
  s3 = _mm_add_ps(s1, s2);

  cout << "s1=" << s1 << endl;
  cout << "s2=" << s2 << endl;
  cout << "s3=" << s3 << endl;
}

__m128 add2(__declspec(16) float a[4], __declspec(16) float b[4]) {
  __m128 s1, s2, s3;

  s1 = _mm_load_ps(a);
  s2 = _mm_load_ps(b);
  s3 = _mm_add_ps(s1, s2);

  return s3;
}

void test_asm() {
  float f1, f2, f3;
  f1 = 4.0; f2 = 4.0; f3 = 4;
  cout << "f1=" << f1 << " f2=" << f2 << " f3=" << f3 << endl;
  __asm{
    movss xmm4, f1    // 将f1存储到xmm0寄存器中
    movss f2, xmm4    // 将xmm0寄存器中的值存储到f2中
    movss  dword ptr [f3], xmm4
  }
  cout << "f1=" << f1 << " f2=" << f2 <<" f3="<<f3<< endl;
}
void asm_copy() {
  char str1[50] = "Hello, world!";
  char str2[50];
  __asm{
    lea esi, str1    // 将str1的地址存储到esi中
    lea edi, str2    // 将str2的地址存储到edi中
    mov ecx, 50      // 将50存储到ecx中，表示要移动50个字节
    cld             // 将方向标志DF清零，表示向前移动字符串
    rep movsb       // 执行movs指令
  }
  strcpy(str2, str1);
  cout << str2 << endl;
}

void mul_matrxi_1(float A[4][4], float B[4][4], float C[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      C[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j] + A[i][3] * B[3][j];
    }
  }
}

void mul_matrxi_2(__declspec(16) float A[4][4], __declspec(16) float B[4][4], __declspec(16) float C[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      C[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j] + A[i][3] * B[3][j];
    }
  }
}
void print(float c[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cout << c[i][j] << " ";
    }
    cout << endl;
  }
}
int main() {
  add1();
  add2();
  __declspec(16) float a[4] = { 1.2f, 2.0f, 3.0f, 4.0f, };
  __declspec(16) float b[4] = { 1.0f, 2.0f, 3.0f, 4.0f, };
  cout << add2(a, b) << endl;
  test_asm();
  asm_copy();

  __declspec(16) float ma[4][4] = {
    1, 2, 3, 4,
    1, 2, 3, 4,
    1, 2, 3, 4,
    1, 2, 3, 4
  },
  mb[4][4] = {
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1
  },
  mc[4][4];
  mul_matrxi_1(ma, mb, mc);
  print(mc);
  return 0;
}

