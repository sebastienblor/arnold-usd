#pragma once

// http://programmers.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed

union DJB2HashUnion{
   unsigned int hash;
   int hashInt;
};

int DJB2Hash(unsigned char *str)
{
   DJB2HashUnion hashUnion;
   hashUnion.hash = 5381;
   int c;

   while ((c = *str++))
      hashUnion.hash = ((hashUnion.hash << 5) + hashUnion.hash) + c; /* hash * 33 + c */

   return hashUnion.hashInt;
}