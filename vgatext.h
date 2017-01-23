#include "util/io.h"

namespace vgatext{

   static inline void writechar(int loc, uint8_t c, uint8_t bg, uint8_t fg, addr_t base){
     //your code goes here
	mmio::write16(base,loc << 1,(fg << 12) | (bg << 8) | c);
   }

}//namespace vgatext
