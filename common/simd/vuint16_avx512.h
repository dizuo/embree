// ======================================================================== //
// Copyright 2009-2016 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

namespace embree
{ 
  /* 16-wide AVX-512 unsigned integer type */
  template<>
    struct vuint<16>
  {
    typedef vboolf16 Bool;
    typedef vuint16  UInt;
    typedef vfloat16 Float;

    enum  { size = 16 }; // number of SIMD elements
    union {              // data
      __m512i v; 
      unsigned int i[16]; 
    };
    
    ////////////////////////////////////////////////////////////////////////////////
    /// Constructors, Assignment & Cast Operators
    ////////////////////////////////////////////////////////////////////////////////
       
    __forceinline vuint() {}
    __forceinline vuint(const vuint16& t) { v = t.v; }
    __forceinline vuint16& operator=(const vuint16& f) { v = f.v; return *this; }

    __forceinline vuint(const __m512i& t) { v = t; }
    __forceinline operator __m512i () const { return v; }
    __forceinline operator __m256i () const { return _mm512_castsi512_si256(v); }

    __forceinline vuint(const unsigned int i) {
      v = _mm512_set_1to16_epi32(i);
    }
    
    __forceinline vuint(const unsigned int a, const unsigned int b, const unsigned int c, const unsigned int d) {
      v = _mm512_set_4to16_epi32(d,c,b,a);      
    }

    __forceinline vuint(const unsigned int a0 , const unsigned int a1 , const unsigned int a2 , const unsigned int a3, 
                        const unsigned int a4 , const unsigned int a5 , const unsigned int a6 , const unsigned int a7, 
                        const unsigned int a8 , const unsigned int a9 , const unsigned int a10, const unsigned int a11, 
                        const unsigned int a12, const unsigned int a13, const unsigned int a14, const unsigned int a15)
    {
      v = _mm512_set_16to16_epi32(a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,a0);
    }
   
    __forceinline explicit vuint(const __m512 f) {
      // round to nearest is standard
      v = _mm512_cvt_roundps_epu32(f,_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC); 
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    /// Constants
    ////////////////////////////////////////////////////////////////////////////////
    
    __forceinline vuint( ZeroTy   ) : v(_mm512_setzero_epi32()) {}
    __forceinline vuint( OneTy    ) : v(_mm512_set_1to16_epi32(1)) {}
    __forceinline vuint( StepTy   ) : v(_mm512_set_16to16_epi32(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)) {}
    __forceinline vuint( ReverseStepTy )   : v(_mm512_setr_epi32(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)) {}

    __forceinline static vuint16 zero() { return _mm512_setzero_epi32(); }
    __forceinline static vuint16 one () { return _mm512_set_1to16_epi32(1); }
    __forceinline static vuint16 neg_one () { return _mm512_set_1to16_epi32(-1); }

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads and Stores
    ////////////////////////////////////////////////////////////////////////////////

    static __forceinline void store_nt(void *__restrict__ ptr, const vuint16& a) {
      _mm512_stream_si512(ptr,a);
    }

    static __forceinline vuint16 loadu(const void* addr)
    {
      return _mm512_loadu_si512(addr);
    }

    static __forceinline vuint16 load(const vuint16* addr) {
      return _mm512_load_si512(addr);
    }

    static __forceinline vuint16 load(const unsigned int* addr) {
      return _mm512_load_si512(addr);
    }

    static __forceinline void store(void* ptr, const vuint16& v) {
      _mm512_store_si512(ptr,v);
    }

    static __forceinline void storeu(void* ptr, const vuint16& v ) {
      _mm512_storeu_si512(ptr,v);
    }

    static __forceinline void storeu(const vboolf16& mask, int* ptr, const vuint16& f ) {
      _mm512_mask_storeu_epi32(ptr,mask,f);
    }

    static __forceinline void store(const vboolf16& mask, void* addr, const vuint16& v2) {
      _mm512_mask_store_epi32(addr,mask,v2);
    }

  /* pass by value to avoid compiler generating inefficient code */
    static __forceinline void storeu_compact(const vboolf16 mask,void * addr, const vuint16 reg) {
      _mm512_mask_compressstoreu_epi32(addr,mask,reg);
    }

    static __forceinline vuint16 compact64bit(const vboolf16& mask, vuint16 &v) {
      return _mm512_mask_compress_epi64(v,mask,v);
    }

    static __forceinline vuint16 compact(const vboolf16& mask, vuint16 &v) {
      return _mm512_mask_compress_epi32(v,mask,v);
    }

    static __forceinline vuint16 compact(const vboolf16& mask, const vuint16 &a, vuint16 &b) {
      return _mm512_mask_compress_epi32(a,mask,b);
    }

    static __forceinline vuint16 broadcast64bit(size_t v) {
      return _mm512_set1_epi64(v);
    }

    static __forceinline size_t extract64bit(const vuint16& v)
    {
      return _mm_cvtsi128_si64(_mm512_castsi512_si128(v));
    }


    ////////////////////////////////////////////////////////////////////////////////
    /// Array Access
    ////////////////////////////////////////////////////////////////////////////////
    
    __forceinline       unsigned int& operator[](const size_t index)       { assert(index < 16); return i[index]; }
    __forceinline const unsigned int& operator[](const size_t index) const { assert(index < 16); return i[index]; }

    __forceinline unsigned int uint    (const size_t index) const { assert(index < 16); return ((unsigned int*)i)[index]; }
    __forceinline size_t&      uint64_t(const size_t index) const { assert(index < 8);  return ((size_t*)i)[index]; }
  };
  
  ////////////////////////////////////////////////////////////////////////////////
  /// Unary Operators
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const vuint16 asUInt    ( const __m512& a ) { return _mm512_castps_si512(a); }
  __forceinline const vuint16 operator +( const vuint16& a ) { return a; }
  __forceinline const vuint16 operator -( const vuint16& a ) { return _mm512_sub_epi32(_mm512_setzero_epi32(), a); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Binary Operators
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const vuint16 operator +( const vuint16& a, const vuint16& b )     { return _mm512_add_epi32(a, b); }
  __forceinline const vuint16 operator +( const vuint16& a, const unsigned int b ) { return a + vuint16(b); }
  __forceinline const vuint16 operator +( const unsigned int a, const vuint16& b ) { return vuint16(a) + b; }

  __forceinline const vuint16 operator -( const vuint16& a, const vuint16& b     ) { return _mm512_sub_epi32(a, b); }
  __forceinline const vuint16 operator -( const vuint16& a, const unsigned int b ) { return a - vuint16(b); }
  __forceinline const vuint16 operator -( const unsigned int a, const vuint16& b ) { return vuint16(a) - b; }

  __forceinline const vuint16 operator *( const vuint16& a, const vuint16& b )     { return _mm512_mul_epu32(a, b); }
  __forceinline const vuint16 operator *( const vuint16& a, const unsigned int b ) { return a * vuint16(b); }
  __forceinline const vuint16 operator *( const unsigned int a, const vuint16& b ) { return vuint16(a) * b; }

  __forceinline const vuint16 operator &( const vuint16& a, const vuint16& b )     { return _mm512_and_epi32(a, b); }
  __forceinline const vuint16 operator &( const vuint16& a, const unsigned int b ) { return a & vuint16(b); }
  __forceinline const vuint16 operator &( const unsigned int a, const vuint16& b ) { return vuint16(a) & b; }

  __forceinline const vuint16 operator |( const vuint16& a, const vuint16& b )     { return _mm512_or_epi32(a, b); }
  __forceinline const vuint16 operator |( const vuint16& a, const unsigned int b ) { return a | vuint16(b); }
  __forceinline const vuint16 operator |( const unsigned int a, const vuint16& b ) { return vuint16(a) | b; }

  __forceinline const vuint16 operator ^( const vuint16& a, const vuint16& b )     { return _mm512_xor_epi32(a, b); }
  __forceinline const vuint16 operator ^( const vuint16& a, const unsigned int b ) { return a ^ vuint16(b); }
  __forceinline const vuint16 operator ^( const unsigned int a, const vuint16& b ) { return vuint16(a) ^ b; }

  __forceinline const vuint16 operator <<( const vuint16& a, const unsigned int n ) { return _mm512_slli_epi32(a, n); }
  __forceinline const vuint16 operator >>( const vuint16& a, const unsigned int n ) { return _mm512_srai_epi32(a, n); } // FIXME: why not srl?

  __forceinline const vuint16 operator <<( const vuint16& a, const vuint16& n ) { return _mm512_sllv_epi32(a, n); }
  __forceinline const vuint16 operator >>( const vuint16& a, const vuint16& n ) { return _mm512_srav_epi32(a, n); } // FIXME: why not srl?

  __forceinline const vuint16 sll ( const vuint16& a, const unsigned int b ) { return _mm512_slli_epi32(a, b); }
  __forceinline const vuint16 sra ( const vuint16& a, const unsigned int b ) { return _mm512_srai_epi32(a, b); }
  __forceinline const vuint16 srl ( const vuint16& a, const unsigned int b ) { return _mm512_srli_epi32(a, b); }
  
  __forceinline const vuint16 min( const vuint16& a, const vuint16& b )     { return _mm512_min_epu32(a, b); }
  __forceinline const vuint16 min( const vuint16& a, const unsigned int b ) { return min(a,vuint16(b)); }
  __forceinline const vuint16 min( const unsigned int a, const vuint16& b ) { return min(vuint16(a),b); }

  __forceinline const vuint16 max( const vuint16& a, const vuint16& b )     { return _mm512_max_epu32(a, b); }
  __forceinline const vuint16 max( const vuint16& a, const unsigned int b ) { return max(a,vuint16(b)); }
  __forceinline const vuint16 max( const unsigned int a, const vuint16& b ) { return max(vuint16(a),b); }
  
  __forceinline const vuint16 mask_add(const vboolf16& mask, vuint16& c, const vuint16& a, const vuint16& b) { return _mm512_mask_add_epi32(c,mask,a,b); }
  __forceinline const vuint16 mask_sub(const vboolf16& mask, vuint16& c, const vuint16& a, const vuint16& b) { return _mm512_mask_sub_epi32(c,mask,a,b); }

  __forceinline const vuint16 mask_and(const vboolf16& m,vuint16& c, const vuint16& a, const vuint16& b) { return _mm512_mask_and_epi32(c,m,a,b); }
  __forceinline const vuint16 mask_or (const vboolf16& m,vuint16& c, const vuint16& a, const vuint16& b) { return _mm512_mask_or_epi32(c,m,a,b); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Assignment Operators
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline vuint16& operator +=( vuint16& a, const vuint16& b ) { return a = a + b; }
  __forceinline vuint16& operator +=( vuint16& a, const unsigned int     b ) { return a = a + b; }
  
  __forceinline vuint16& operator -=( vuint16& a, const vuint16& b ) { return a = a - b; }
  __forceinline vuint16& operator -=( vuint16& a, const unsigned int     b ) { return a = a - b; }

  __forceinline vuint16& operator *=( vuint16& a, const vuint16& b ) { return a = a * b; }
  __forceinline vuint16& operator *=( vuint16& a, const unsigned int     b ) { return a = a * b; }
  
  __forceinline vuint16& operator &=( vuint16& a, const vuint16& b ) { return a = a & b; }
  __forceinline vuint16& operator &=( vuint16& a, const unsigned int     b ) { return a = a & b; }
  
  __forceinline vuint16& operator |=( vuint16& a, const vuint16& b ) { return a = a | b; }
  __forceinline vuint16& operator |=( vuint16& a, const unsigned int     b ) { return a = a | b; }
  
  __forceinline vuint16& operator <<=( vuint16& a, const unsigned int b ) { return a = a << b; }
  __forceinline vuint16& operator >>=( vuint16& a, const unsigned int b ) { return a = a >> b; }


  ////////////////////////////////////////////////////////////////////////////////
  /// Comparison Operators + Select
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const vboolf16 operator ==( const vuint16& a, const vuint16& b ) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_EQ); }
  __forceinline const vboolf16 operator ==( const vuint16& a, const unsigned int b ) { return a == vuint16(b); }
  __forceinline const vboolf16 operator ==( const unsigned int a, const vuint16& b ) { return vuint16(a) == b; }
  
  __forceinline const vboolf16 operator !=( const vuint16& a, const vuint16& b ) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_NE); }
  __forceinline const vboolf16 operator !=( const vuint16& a, const unsigned int b ) { return a != vuint16(b); }
  __forceinline const vboolf16 operator !=( const unsigned int a, const vuint16& b ) { return vuint16(a) != b; }
  
  __forceinline const vboolf16 operator < ( const vuint16& a, const vuint16& b ) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_LT); }
  __forceinline const vboolf16 operator < ( const vuint16& a, const unsigned int b ) { return a <  vuint16(b); }
  __forceinline const vboolf16 operator < ( const unsigned int a, const vuint16& b ) { return vuint16(a) <  b; }
  
  __forceinline const vboolf16 operator >=( const vuint16& a, const vuint16& b ) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_GE); }
  __forceinline const vboolf16 operator >=( const vuint16& a, const unsigned int b ) { return a >= vuint16(b); }
  __forceinline const vboolf16 operator >=( const unsigned int a, const vuint16& b ) { return vuint16(a) >= b; }

  __forceinline const vboolf16 operator > ( const vuint16& a, const vuint16& b ) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_GT); }
  __forceinline const vboolf16 operator > ( const vuint16& a, const unsigned int b ) { return a >  vuint16(b); }
  __forceinline const vboolf16 operator > ( const unsigned int a, const vuint16& b ) { return vuint16(a) >  b; }

  __forceinline const vboolf16 operator <=( const vuint16& a, const vuint16& b ) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_LE); }
  __forceinline const vboolf16 operator <=( const vuint16& a, const unsigned int b ) { return a <= vuint16(b); }
  __forceinline const vboolf16 operator <=( const unsigned int a, const vuint16& b ) { return vuint16(a) <= b; }

  __forceinline vboolf16 eq(                     const vuint16& a, const vuint16& b) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_EQ); }
  __forceinline vboolf16 eq(const vboolf16 mask, const vuint16& a, const vuint16& b) { return _mm512_mask_cmp_epu32_mask(mask,a,b,_MM_CMPINT_EQ); }
  
  __forceinline vboolf16 ne(                     const vuint16& a, const vuint16& b) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_NE); }
  __forceinline vboolf16 ne(const vboolf16 mask, const vuint16& a, const vuint16& b) { return _mm512_mask_cmp_epu32_mask(mask,a,b,_MM_CMPINT_NE); }

  __forceinline vboolf16 lt(                     const vuint16& a, const vuint16& b) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_LT); }
  __forceinline vboolf16 lt(const vboolf16 mask, const vuint16& a, const vuint16& b) { return _mm512_mask_cmp_epu32_mask(mask,a,b,_MM_CMPINT_LT); }
 
  __forceinline vboolf16 ge(                     const vuint16& a, const vuint16& b) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_GE); }
  __forceinline vboolf16 ge(const vboolf16 mask, const vuint16& a, const vuint16& b) { return _mm512_mask_cmp_epu32_mask(mask,a,b,_MM_CMPINT_GE); }
  
  __forceinline vboolf16 gt(                     const vuint16& a, const vuint16& b) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_GT); }
  __forceinline vboolf16 gt(const vboolf16 mask, const vuint16& a, const vuint16& b) { return _mm512_mask_cmp_epu32_mask(mask,a,b,_MM_CMPINT_GT); }
  
  __forceinline vboolf16 le(                     const vuint16& a, const vuint16& b) { return _mm512_cmp_epu32_mask(a,b,_MM_CMPINT_LE); }
  __forceinline vboolf16 le(const vboolf16 mask, const vuint16& a, const vuint16& b) { return _mm512_mask_cmp_epu32_mask(mask,a,b,_MM_CMPINT_LE); }
    
 
  __forceinline const vuint16 select( const vboolf16& m, const vuint16& t, const vuint16& f ) {
    return _mm512_mask_or_epi32(f,m,t,t); 
  }

  __forceinline void xchg(const vboolf16& m, vuint16& a, vuint16& b) {
    const vuint16 c = a; a = select(m,b,a); b = select(m,c,b);
  }

  __forceinline vboolf16 test(const vboolf16& m, const vuint16& a, const vuint16& b) {
    return _mm512_mask_test_epi32_mask(m,a,b);
  }

  __forceinline vboolf16 test(const vuint16& a, const vuint16& b) {
    return _mm512_test_epi32_mask(a,b);
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Movement/Shifting/Shuffling Functions
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline vuint16 shuffle (const vuint16& x,_MM_SWIZZLE_ENUM perm32 ) { return _mm512_swizzle_epi32(x,perm32); }
  __forceinline vuint16 shuffle4(const vuint16& x,_MM_PERM_ENUM    perm128) { return _mm512_permute4f128_epi32(x,perm128); }
  
  template<int D, int C, int B, int A> __forceinline vuint16 shuffle   (const vuint16& v) { return _mm512_shuffle_epi32(v,_MM_SHUF_PERM(D,C,B,A)); }
  template<int A>                      __forceinline vuint16 shuffle   (const vuint16& x) { return shuffle<A,A,A,A>(v); }
  template<>                           __forceinline vuint16 shuffle<0>(const vuint16& x) { return shuffle(x,_MM_SWIZ_REG_AAAA); }
  template<>                           __forceinline vuint16 shuffle<1>(const vuint16& x) { return shuffle(x,_MM_SWIZ_REG_BBBB); }
  template<>                           __forceinline vuint16 shuffle<2>(const vuint16& x) { return shuffle(x,_MM_SWIZ_REG_CCCC); }
  template<>                           __forceinline vuint16 shuffle<3>(const vuint16& x) { return shuffle(x,_MM_SWIZ_REG_DDDD); }

  template<int D, int C, int B, int A> __forceinline vuint16 shuffle4(const vuint16& v) { return shuffle4(v,_MM_SHUF_PERM(D,C,B,A)); }
  template<int A>                      __forceinline vuint16 shuffle4(const vuint16& x) { return shuffle4<A,A,A,A>(x); }

  __forceinline vuint16 shuffle(const vuint16& x,_MM_PERM_ENUM    perm128, _MM_SWIZZLE_ENUM perm32) { return shuffle(shuffle4(x,perm128),perm32); }
  
  __forceinline vuint16 shuffle(const vboolf16& mask, vuint16& v, const vuint16& x,_MM_PERM_ENUM perm128, _MM_SWIZZLE_ENUM perm32)  {
    return _mm512_mask_swizzle_epi32(_mm512_mask_permute4f128_epi32(v,mask,x,perm128),mask,x,perm32);  
  }

  __forceinline vuint16 swAAAA(const vuint16& x) {
    return shuffle(x,_MM_SWIZ_REG_AAAA);
  }

  __forceinline vuint16 swBBBB(const vuint16& x) {
    return shuffle(x,_MM_SWIZ_REG_BBBB);
  }

  __forceinline vuint16 swCCCC(const vuint16& x) {
    return shuffle(x,_MM_SWIZ_REG_CCCC);
  }

  __forceinline vuint16 swDDDD(const vuint16& x) {
    return shuffle(x,_MM_SWIZ_REG_DDDD);
  }

  template<int i>
    __forceinline vuint16 align_shift_right(const vuint16& a, const vuint16& b)
  {
    return _mm512_alignr_epi32(a,b,i); 
  };

  __forceinline unsigned int toScalar(const vuint16& a)
  {
    return _mm_cvtsi128_si32(_mm512_castsi512_si128(a));
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Reductions
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline unsigned int reduce_add(vuint16 a) { return _mm512_reduce_add_epi32(a); }
  __forceinline unsigned int reduce_min(vuint16 a) { return _mm512_reduce_min_epu32(a); }
  __forceinline unsigned int reduce_max(vuint16 a) { return _mm512_reduce_max_epu32(a); }
  __forceinline unsigned int reduce_and(vuint16 a) { return _mm512_reduce_and_epi32(a); }
  
  __forceinline vuint16 vreduce_min2(vuint16 x) {                      return min(x,shuffle(x,_MM_SWIZ_REG_BADC)); }
  __forceinline vuint16 vreduce_min4(vuint16 x) { x = vreduce_min2(x); return min(x,shuffle(x,_MM_SWIZ_REG_CDAB)); }
  __forceinline vuint16 vreduce_min8(vuint16 x) { x = vreduce_min4(x); return min(x,shuffle4(x,_MM_SHUF_PERM(2,3,0,1))); }
  __forceinline vuint16 vreduce_min (vuint16 x) { x = vreduce_min8(x); return min(x,shuffle4(x,_MM_SHUF_PERM(1,0,3,2))); }

  __forceinline vuint16 vreduce_max2(vuint16 x) {                      return max(x,shuffle(x,_MM_SWIZ_REG_BADC)); }
  __forceinline vuint16 vreduce_max4(vuint16 x) { x = vreduce_max2(x); return max(x,shuffle(x,_MM_SWIZ_REG_CDAB)); }
  __forceinline vuint16 vreduce_max8(vuint16 x) { x = vreduce_max4(x); return max(x,shuffle4(x,_MM_SHUF_PERM(2,3,0,1))); }
  __forceinline vuint16 vreduce_max (vuint16 x) { x = vreduce_max8(x); return max(x,shuffle4(x,_MM_SHUF_PERM(1,0,3,2))); }

  __forceinline vuint16 vreduce_and2(vuint16 x) {                      return x & shuffle(x,_MM_SWIZ_REG_BADC); }
  __forceinline vuint16 vreduce_and4(vuint16 x) { x = vreduce_and2(x); return x & shuffle(x,_MM_SWIZ_REG_CDAB); }
  __forceinline vuint16 vreduce_and8(vuint16 x) { x = vreduce_and4(x); return x & shuffle4(x,_MM_SHUF_PERM(2,3,0,1)); }
  __forceinline vuint16 vreduce_and (vuint16 x) { x = vreduce_and8(x); return x & shuffle4(x,_MM_SHUF_PERM(1,0,3,2)); }

  __forceinline vuint16 vreduce_or2(vuint16 x) {                     return x | shuffle(x,_MM_SWIZ_REG_BADC); }
  __forceinline vuint16 vreduce_or4(vuint16 x) { x = vreduce_or2(x); return x | shuffle(x,_MM_SWIZ_REG_CDAB); }
  __forceinline vuint16 vreduce_or8(vuint16 x) { x = vreduce_or4(x); return x | shuffle4(x,_MM_SHUF_PERM(2,3,0,1)); }
  __forceinline vuint16 vreduce_or (vuint16 x) { x = vreduce_or8(x); return x | shuffle4(x,_MM_SHUF_PERM(1,0,3,2)); }

  __forceinline vuint16 vreduce_add2(vuint16 x) {                      return x + shuffle(x,_MM_SWIZ_REG_BADC); }
  __forceinline vuint16 vreduce_add4(vuint16 x) { x = vreduce_add2(x); return x + shuffle(x,_MM_SWIZ_REG_CDAB); }
  __forceinline vuint16 vreduce_add8(vuint16 x) { x = vreduce_add4(x); return x + shuffle4(x,_MM_SHUF_PERM(2,3,0,1)); }
  __forceinline vuint16 vreduce_add (vuint16 x) { x = vreduce_add8(x); return x + shuffle4(x,_MM_SHUF_PERM(1,0,3,2)); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Memory load and store operations
  ////////////////////////////////////////////////////////////////////////////////


  __forceinline vuint16 broadcast4to16ui(const unsigned int *const ptr) {
    return _mm512_extload_epi32(ptr,_MM_UPCONV_EPI32_NONE,_MM_BROADCAST_4X16,_MM_HINT_NONE);
  }

  __forceinline vuint16 gather16i_4i(const unsigned int *__restrict__ const ptr0,
                                     const unsigned int *__restrict__ const ptr1,
                                     const unsigned int *__restrict__ const ptr2,
                                     const unsigned int *__restrict__ const ptr3)
  {
    vuint16 v =  broadcast4to16ui(ptr0);
    v = select((vboolf16)0xf0  , broadcast4to16ui(ptr1),v);
    v = select((vboolf16)0xf00 , broadcast4to16ui(ptr2),v);
    v = select((vboolf16)0xf000, broadcast4to16ui(ptr3),v);
    return v;
  }


  __forceinline vuint16 gather16ui_4ui_align(const void *__restrict__ const ptr0,
                                             const void *__restrict__ const ptr1,
                                             const void *__restrict__ const ptr2,
                                             const void *__restrict__ const ptr3)
  {
    vuint16 v = broadcast4to16ui((const unsigned int*)ptr3);
    v = align_shift_right<12>(v,broadcast4to16ui((const unsigned int*)ptr2));
    v = align_shift_right<12>(v,broadcast4to16ui((const unsigned int*)ptr1));
    v = align_shift_right<12>(v,broadcast4to16ui((const unsigned int*)ptr0));
    return v;
  }

  __forceinline vuint16 gather16i_4i_align(const vuint16& v0,
                                           const vuint16& v1,
                                           const vuint16& v2,
                                           const vuint16& v3)
  {
    vuint16 v = v3;
    v = align_shift_right<12>(v,v2);
    v = align_shift_right<12>(v,v1);
    v = align_shift_right<12>(v,v0);
    return v;
  }

  
  __forceinline vuint16 gather16i(const vboolf16& mask, const unsigned int *const ptr, const vuint16& index,const _MM_INDEX_SCALE_ENUM scale) {
    return _mm512_mask_i32extgather_epi32(_mm512_undefined_epi32(),mask,index,ptr,_MM_UPCONV_EPI32_NONE,scale,0);
  }
  
  __forceinline vuint16 gather16i(const vboolf16& mask, vuint16& dest, const unsigned int *const ptr, const vuint16& index,const _MM_INDEX_SCALE_ENUM scale) {
    return _mm512_mask_i32extgather_epi32(dest,mask,index,ptr,_MM_UPCONV_EPI32_NONE,scale,0);
  }
  
  __forceinline void scatter16i(const vboolf16& mask,int *const ptr, const vuint16& index,const vuint16& v, const _MM_INDEX_SCALE_ENUM scale) {
    _mm512_mask_i32extscatter_epi32((int*)ptr,mask,index,v,_MM_DOWNCONV_EPI32_NONE,scale,0);
  }
    

  __forceinline void compactustore16i_low(const vboolf16 mask, void *addr, const vuint16& reg) {
    _mm512_mask_compressstoreu_epi32(addr,mask,reg);
  }
  

  __forceinline vuint16 permute(vuint16 v,vuint16 index)
  {
    return _mm512_permutev_epi32(index,v);  
  }

  __forceinline vuint16 reverse(const vuint16 &a)
  {
    return permute(a,vuint16(reverse_step));
  }

  __forceinline vuint16 prefix_sum(const vuint16& a)
  {
    vuint16 v = a;
    v = mask_add(0xaaaa,v,v,shuffle<2,2,0,0>(v));
    v = mask_add(0xcccc,v,v,shuffle<1,1,1,1>(v));
    const vuint16 shuf_v0 = shuffle(v,(_MM_PERM_ENUM)_MM_SHUF_PERM(2,2,0,0),_MM_SWIZ_REG_DDDD);
    v = mask_add(0xf0f0,v,v,shuf_v0);
    const vuint16 shuf_v1 = shuffle(v,(_MM_PERM_ENUM)_MM_SHUF_PERM(1,1,0,0),_MM_SWIZ_REG_DDDD);
    v = mask_add(0xff00,v,v,shuf_v1);
    return v;  
  }

  __forceinline vuint16 reverse_prefix_sum(const vuint16& a)
  {
    vuint16 v = a;
    v = mask_add(0x5555,v,v,shuffle<3,3,1,1>(v));
    v = mask_add(0x3333,v,v,shuffle<2,2,2,2>(v));
    const vuint16 shuf_v0 = shuffle(v,(_MM_PERM_ENUM)_MM_SHUF_PERM(3,3,1,1),_MM_SWIZ_REG_AAAA);
    v = mask_add(0x0f0f,v,v,shuf_v0);
    const vuint16 shuf_v1 = shuffle(v,(_MM_PERM_ENUM)_MM_SHUF_PERM(2,2,2,2),_MM_SWIZ_REG_AAAA);
    v = mask_add(0x00ff,v,v,shuf_v1);

    return v;  
  }

  __forceinline vuint16 sortNetwork(const vuint16& v)
  {
    const vuint16 a0 = v;
    const vuint16 b0 = shuffle<1,0,3,2>(a0);
    const vuint16 c0 = min(a0,b0);
    const vuint16 d0 = max(a0,b0);
    const vuint16 a1 = select(0x99 /* 0b10011001 */,c0,d0);
    const vuint16 b1 = shuffle<2,3,0,1>(a1);
    const vuint16 c1 = min(a1,b1);
    const vuint16 d1 = max(a1,b1);
    const vuint16 a2 = select(0xc3 /* 0b11000011 */,c1,d1);
    const vuint16 b2 = shuffle<1,0,3,2>(a2);
    const vuint16 c2 = min(a2,b2);
    const vuint16 d2 = max(a2,b2);
    const vuint16 a3 = select(0xa5 /* 0b10100101 */,c2,d2);
    const vuint16 b3 = shuffle4<1,0,1,0>(a3);
    const vuint16 c3 = min(a3,b3);
    const vuint16 d3 = max(a3,b3);
    const vuint16 a4 = select(0xf /* 0b00001111 */,c3,d3);
    const vuint16 b4 = shuffle<2,3,0,1>(a4);
    const vuint16 c4 = min(a4,b4);
    const vuint16 d4 = max(a4,b4);
    const vuint16 a5 = select(0x33 /* 0b00110011 */,c4,d4);
    const vuint16 b5 = shuffle<1,0,3,2>(a5);
    const vuint16 c5 = min(a5,b5);
    const vuint16 d5 = max(a5,b5);
    const vuint16 a6 = select(0x55 /* 0b01010101 */,c5,d5);
    return a6;
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  /// Output Operators
  ////////////////////////////////////////////////////////////////////////////////
  
  __forceinline std::ostream& operator<<(std::ostream& cout, const vuint16& v)
  {
    cout << "<" << v[0];
    for (int i=1; i<16; i++) cout << ", " << v[i];
    cout << ">";
    return cout;
  }
}
