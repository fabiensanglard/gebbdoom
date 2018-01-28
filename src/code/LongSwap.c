long LongSwap (long dat) {
  return  ( dat>>24)            |
          ((dat>>8) & 0xff00)   |
          ((dat<<8) & 0xff0000) |
          ( dat<<24);
}