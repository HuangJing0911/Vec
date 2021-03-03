#ifndef XCRC_H
#define XCRC_H
#include <cassert>
#include <stdint.h>

#define XCRC32_KEY	0x04c11db7

class XCrc
{
public:
     typedef uint32_t Type;

XCrc(Type key)
     :_key(key)
     ,_register (0xFFFFFFFF)
     { }
     Type Done ()
     {
	  Type tmp = _register;
	  _register = 0xFFFFFFFF;
	  return tmp;
     }
protected:
     Type _key; // really 33-bit key, counting implicit 1 top-bit
     Type _register;
};

class XSlowCrc: public XCrc
{
public:
     XSlowCrc (XCrc::Type key)
	  : XCrc (key)
     { };
    
     void PutByte (unsigned char byte)
     {
	  unsigned char mask = 0x80; // leftmost bit
	  for (int j = 0; j < 8; ++j)
	  {
	       PutBit ((byte & mask) != 0);
	       mask >>= 1;
	  }
     }

private:
     void PutBit (bool bit)
     {
	  bool topBit = (_register & 0x80000000) != 0;
	  // shift bit into register from the right
	  _register <<= 1;
	  _register ^= (bit? 0x1: 0x0); // OR or XOR, same result
	  if (topBit)
	  {
	       // XOR the 32-bits of the key.
	       // The implicit high bit of the 33-bit key conceptually
	       // clears the topBit shifted out of the register
	       _register ^= _key;
	  }
     }
};

class XFastCrc: public XCrc
{
public:
     XFastCrc (XCrc::Type key) 
	  :XCrc(key) 
     {
	  _table.Init(key);
     }
     void PutByte (unsigned byte)
     {
	  _register = (_register << 8) ^
	       _table[((_register >> 24) ^ byte) & 0xff];
     };
private:
     class Table
     {
     public:
          Table() :_key(0)
          {};
	  void Init (XCrc::Type key)
	  {
	       assert (key != 0);
	       if (key == _key)
		    return;
	       _key = key;
		
	       unsigned int  i, j, k;

	       for( i = 0; i < 256; i++ )
	       {
		    k = 0;
		    for( j = (i << 24) | 0x800000; j != 0x80000000; j <<= 1 )
			 k = (k << 1) ^ (((k ^ j) & 0x80000000)
					 ? 0x04c11db7 : 0);
		    _table[i] = k;
	       }
	  };
	  XCrc::Type operator [] (unsigned i)
	  {
	       return _table [i];
	  }
     private:
	  XCrc::Type _table [256];
	  XCrc::Type _key;
     };

     Table _table;

};


#endif // XCRC_H
