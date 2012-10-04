#ifndef THREE_HASH_HPP
#define THREE_HASH_HPP

#include <cstdint>

namespace three {

#define JENKINS_MIX(a,b,c) {    \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8);  \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12); \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5);  \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

#define JENKINS_32(d) (*((const std::uint32_t *) (d)))

template < typename T >
static unsigned jenkins_hash(const T& value, unsigned initval = 0) {

  typedef std::uint32_t ub4;

  int length = sizeof(T);
  unsigned char* k = (unsigned char*)(&value);

  register ub4 a,b,c,len;

  /* Set up the internal state */
  len = length;
  a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
  c = initval;         /* the previous hash value */

  /*---------------------------------------- handle most of the key */
  while (len >= 12) {
    a += JENKINS_32(k);
    b += JENKINS_32(k+4);
    c += JENKINS_32(k+8);
    JENKINS_MIX(a,b,c);
    k += 12; len -= 12;
  }

  /*------------------------------------- handle the last 11 bytes */
  c += length;
  switch(len) {            /* all the case statements fall through */
    case 11: c+=((ub4)k[10]<<24);
    case 10: c+=((ub4)k[9]<<16);
    case 9 : c+=((ub4)k[8]<<8);
    /* the first byte of c is reserved for the length */
    case 8 : b+=((ub4)k[7]<<24);
    case 7 : b+=((ub4)k[6]<<16);
    case 6 : b+=((ub4)k[5]<<8);
    case 5 : b+=k[4];
    case 4 : a+=((ub4)k[3]<<24);
    case 3 : a+=((ub4)k[2]<<16);
    case 2 : a+=((ub4)k[1]<<8);
    case 1 : a+=k[0];
    /* case 0: nothing left to add */
  };

  JENKINS_MIX(a,b,c);

  return c;
}

#undef JENKINS_MIX
#undef JENKINS_32

}

#endif // THREE_HASH_HPP