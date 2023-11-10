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

void shuffle1() {
  __m128 a = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
  __m128 b = _mm_set_ps(5.0f, 6.0f, 7.0f, 8.0f);

  cout << "input:\n";
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;

  // 交换 a 和 b 中的值
  _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 2, 0, 3));

  cout << "after _mm_shuffle_ps(a, b, _MM_SHUFFLE(0, 1, 2, 3):\n";
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
}

void sum_m128() {
  //__m128 a = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
  __m128 a = { 1, 2, 3, 4 };
  cout << "input 4 floats: ";
  cin >> a.m128_f32[0] >> a.m128_f32[1] >> a.m128_f32[2] >> a.m128_f32[3];

  cout << "a=" << a << endl;

  // 将两个相邻的成员相加
  __m128 b = _mm_hadd_ps(a, a);
  cout << "b=" << b << endl;

  __m128 c = _mm_hadd_ps(b, b);

  // 求和
  float sum = _mm_cvtss_f32(c);
  cout << "sum=" << sum << endl;

  float sum2 = a.m128_f32[0] + a.m128_f32[1] + a.m128_f32[2] + a.m128_f32[3];
  cout << "sum2=" << sum2 << endl;
}

void test_asm() {
  float f1, f2, f3;
  f1 = 4.0; f2 = 4.0; f3 = 4;
  cout << "f1=" << f1 << " f2=" << f2 << " f3=" << f3 << endl;


#ifdef _M_X64
  // x64 platform
#else
  // x86 platform
  __asm {
    movss xmm4, f1    // 将f1存储到xmm0寄存器中
    movss f2, xmm4    // 将xmm0寄存器中的值存储到f2中
    movss  dword ptr [f3], xmm4
  }
  cout << "f1=" << f1 << " f2=" << f2 <<" f3="<<f3<< endl;
#endif
}

void asm_copy() {
  char str1[50] = "Hello, world!";
  char str2[50];

#ifdef _M_X64
#else
  __asm {
    lea esi, str1    // 将str1的地址存储到esi中
    lea edi, str2    // 将str2的地址存储到edi中
    mov ecx, 50      // 将50存储到ecx中，表示要移动50个字节
    cld             // 将方向标志DF清零，表示向前移动字符串
    rep movsb       // 执行movs指令
  }
#endif
  strcpy(str2, str1);
  cout << str2 << endl;
}

void mul_matrxi_native(__declspec(16) float A[4][4], __declspec(16) float B[4][4], __declspec(16) float C[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float sum = 0;
      for (int k = 0; k < 4; k++) {
        sum += A[i][k] * B[k][j];
      }
      C[i][j] = sum;
    }
  }
}

void mul_matrxi_0(__declspec(16) float A[4][4], __declspec(16) float B[4][4], __declspec(16) float C[4][4]) {
  for (int i = 0; i < 4; i++) {
    C[i][0] = A[i][0] * B[0][0] + A[i][1] * B[1][0] + A[i][2] * B[2][0] + A[i][3] * B[3][0];
    C[i][1] = A[i][0] * B[0][1] + A[i][1] * B[1][1] + A[i][2] * B[2][1] + A[i][3] * B[3][1];
    C[i][2] = A[i][0] * B[0][2] + A[i][1] * B[1][2] + A[i][2] * B[2][2] + A[i][3] * B[3][2];
    C[i][3] = A[i][0] * B[0][3] + A[i][1] * B[1][3] + A[i][2] * B[2][3] + A[i][3] * B[3][3];
  }
}

void mul_matrxi_1(__declspec(16) float A[4][4], __declspec(16) float B[4][4], __declspec(16) float C[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      C[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j] + A[i][3] * B[3][j];
    }
  }
}


inline float sum_m128_1(__m128 a) {
  // 将两个相邻的成员相加
  __m128 b = _mm_hadd_ps(a, a);
  __m128 c = _mm_hadd_ps(b, b);
  float sum = _mm_cvtss_f32(c);
  return sum;
  cout << "sum=" << sum << endl;

  float sum2 = a.m128_f32[0] + a.m128_f32[1] + a.m128_f32[2] + a.m128_f32[3];
  cout << "sum2=" << sum2 << endl;
}

inline float sum_m128_2(__m128 a) {
  float sum2 = a.m128_f32[0] + a.m128_f32[1] + a.m128_f32[2] + a.m128_f32[3];
  return sum2;
}

void mul_matrxi_sse_1(__declspec(16) float A[4][4], __declspec(16) float B[4][4], __declspec(16) float C[4][4]) {
  __m128 b0 = { B[0][0], B[1][0], B[2][0], B[3][0] };
  __m128 b1 = { B[0][1], B[1][1], B[2][1], B[3][1] };
  __m128 b2 = { B[0][2], B[1][2], B[2][2], B[3][2] };
  __m128 b3 = { B[0][3], B[1][3], B[2][3], B[3][3] };
  for (int i = 0; i < 4; i++) {
    __m128 ai = _mm_load_ps(A[i]);

    __m128 c0 = _mm_mul_ps(ai, b0);
    __m128 c1 = _mm_mul_ps(ai, b1);
    __m128 c2 = _mm_mul_ps(ai, b2);
    __m128 c3 = _mm_mul_ps(ai, b3);

    C[i][0] = sum_m128_1(c0);
    C[i][1] = sum_m128_1(c1);
    C[i][2] = sum_m128_1(c2);
    C[i][3] = sum_m128_1(c3);
  }
}

void mul_matrxi_sse_2(__declspec(16) float A[4][4], __declspec(16) float B[4][4], __declspec(16) float C[4][4]) {
  __m128 b0 = { B[0][0], B[1][0], B[2][0], B[3][0] };
  __m128 b1 = { B[0][1], B[1][1], B[2][1], B[3][1] };
  __m128 b2 = { B[0][2], B[1][2], B[2][2], B[3][2] };
  __m128 b3 = { B[0][3], B[1][3], B[2][3], B[3][3] };

  //cout << "b0=" << b0 << endl;
  //cout << "b1=" << b1 << endl;
  //cout << "b2=" << b2 << endl;
  //cout << "b3=" << b3 << endl;
  for (int i = 0; i < 4; i++) {
    __m128 ai = _mm_load_ps(A[i]);
    //cout << "ai=" << ai << endl;
    for (int j = 0; j < 4; j++) {
      __m128 c0 = _mm_mul_ps(ai, b0);
      __m128 c1 = _mm_mul_ps(ai, b1);
      __m128 c2 = _mm_mul_ps(ai, b2);
      __m128 c3 = _mm_mul_ps(ai, b3);

      //cout << "c0=" << c0 << endl;
      //cout << "c1=" << c1 << endl;
      //cout << "c2=" << c2 << endl;
      //cout << "c3=" << c3 << endl;

      C[i][0] = sum_m128_2(c0);
      C[i][1] = sum_m128_2(c1);
      C[i][2] = sum_m128_2(c2);
      C[i][3] = sum_m128_2(c3);
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
  cout << endl;
}

inline static unsigned long long sum4(int *a, int n) {
#ifdef _M_X64
  return accumulate(a, a + n, 0);
#else
  int r = n % 4;
  int q = n / 4;

  ////

  unsigned long long res;
  __m128i *f4 = (__m128i *) a;
  __m128i vec_sum = _mm_setzero_si128();
  for (int i = 0; i < q; i++) {
    vec_sum = _mm_hadd_epi32(vec_sum, f4[i]);
  }
  vec_sum = _mm_hadd_epi32(vec_sum, vec_sum);
  vec_sum = _mm_hadd_epi32(vec_sum, vec_sum);
  res = _mm_cvtsi128_si32(vec_sum);


  ////

  if (r == 3) {
    res = res + a[n - 1] + a[n - 2] + a[n - 3];
  } else if (r == 2) {
    res = res + a[n - 1] + a[n - 2];
  } else if (r == 1) {
    res = res + a[n - 1];
  } else {

  }

  return res;
#endif
}

int test_sum4() {
  __declspec(16) int a0[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
  printf("%llu\n", sum4(&a0[0], 8));
  return 0;
}

typedef void(*mul_matrxi_func)(__declspec(16) float A[4][4], __declspec(16) float B[4][4], __declspec(16) float C[4][4]);
void TestTime(mul_matrxi_func func, __declspec(16) float A[4][4], __declspec(16) float B[4][4], __declspec(16) float C[4][4]) {
  double t0 = omp_get_wtime();
  for (int i = 0; i < 4; i++) {
    A[i][i] = 1;
  }
  for (int i = 0; i < 1000000; i++) {
    func(A, B, C);
    A[i % 4][i % 4] += 1;
  }
  double t1 = omp_get_wtime();
  printf("used time %.3f ms\n", (t1 - t0) * 1000);
}
int main() {
  //return test_sum4();
  //add1();
  //add2();
  //sum_m128(); //return 0;
  //__declspec(16) float a[4] = { 1.2f, 2.0f, 3.0f, 4.0f, };
  //__declspec(16) float b[4] = { 1.0f, 2.0f, 3.0f, 4.0f, };
  //cin >> a[0];
  //__m128 c = add2(a, b);
  //cout << c << endl;
  //test_asm();
  //asm_copy();

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
  mc0[4][4], mc1[4][4], mc2[4][4], mc3[4][4], mc4[4][4];
  //cin >> ma[0][0];
  //for (int i = 0; i < 20000; i++) {
  //  mul_matrxi_1(ma, mb, mc1);
  //  //print(mc1);

  //  mul_matrxi_sse_1(ma, mb, mc2);
  //  //print(mc2);

  //  mul_matrxi_sse_2(ma, mb, mc3);
  //  //print(mc3);
  //  ma[0][0] += 1;
  //}
  /*
  used time 12.105 ms
  used time 3.639 ms mul_matrxi_1最快
  used time 14.097 ms
  used time 11.861 ms
  */
  TestTime(mul_matrxi_native, ma, mb, mc0);
  TestTime(mul_matrxi_0, ma, mb, mc1);
  TestTime(mul_matrxi_1, ma, mb, mc2);
  TestTime(mul_matrxi_sse_1, ma, mb, mc3);
  TestTime(mul_matrxi_sse_2, ma, mb, mc4);

  print(mc0);
  print(mc1);
  print(mc2);
  print(mc3);
  print(mc4);

  return 0;
}

