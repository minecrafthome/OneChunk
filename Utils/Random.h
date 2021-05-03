#ifndef JAVARND_H
#define JAVARND_H

#include <cstdint>

class JavaRnd
{
private:
    uint64_t seed;

public:
    JavaRnd(int64_t seed) {
      setSeed(seed);
    }

  inline void setSeed(int64_t seed) {
    this->seed = (uint64_t)((uint64_t)seed ^ 0x5deece66du) & ((1LLu << 48) - 1);
  }

    inline int nextInt(int n)
    {
        //if (n <= 0) throw new ArgumentException("n must be positive");

        if ((n & -n) == n)  // i.e., n is a power of 2
            return (int)((n * (int64_t)Next(31)) >> 31u);

        long bits, val;
        do
        {
            bits = Next(31);
            val = bits % (int32_t)n;
        }
        while (bits - val + (n - 1) < 0);

        return (int)val;
    }

    inline bool nextBoolean() {
        return Next(1) != 0;
    }

    inline double NextDouble()
    {
        return (((int64_t)Next(26) << 27u) + Next(27))
          / (double)(1LLu << 53u);
    }

    inline int32_t Next(int bits)
    {
        seed = (seed * 0x5deece66du + 0xBu) & ((1LLu << 48u) - 1);

        return (int32_t)(seed >> (48u - bits));
    }
    
    inline int64_t nextLong()
    {
        return ((int64_t) Next(32) << 32) + Next(32);
    }
};
#endif