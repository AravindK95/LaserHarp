packet structure:

2 bytes, first byte has MSB 0, second byte has MSB 1.

byte 0:

0    [_ _ _]  [_ _ _ _]
MSB  POS      high part of reading

byte 1:

1    [_ _ _ _ _ _ _]
MSB  low part of reading