## x11-calc - Another RPN (Reverse Polish) calculator.

### Registers

    A, B, C:   General purpose registers.  The C register is used to access
               the M register and or memory as well as holding the value of
               the 'X' register. 
    D, E, F:   Stack registers used to hold the values of 'Y', 'Z', and 'T'.
    M, N:      Memory registers.

    P:         A  4-bit register that is used to select which part of  each
               register should be used.
    DATA:      An 8-bit register holding the memory address used to read or
               write to memory from the C register.

    S0-S11:    Processor status bits.

      S0       Not used.
      S1       Scientific notation (clear for fixed point notation).
      S2       Auto Enter (if set entering digit will push 'X').
      S3       Set for radians clear for degrees.
      S4       Power OK (clear for lower power).
      S5       Set if decimal point has already been entered.
      S6       ?
      S7       ?
      S8       ?
      S9       ?
      S10      ?
      S11      ?
      S12      ?
      S13      Set if function key has been pressed.
      S14      ?
      S15      Set if any key is pressed.


### Special operations.

    0 000 000 000  nop
    0 000 001 000  clear regs
    0 000 010 000  keys -> rom address
    0 001 001 000  clear s
    0 010 001 000  display toggle
    0 011 001 000  display off
    0 100 001 000  m exch c
    0 101 001 000  m -> c
    0 110 001 000  n exch c
    0 111 001 000  n -> c
    1 000 001 000  stack -> a
    1 001 001 000  down rotate
    1 010 001 000  y -> a
    1 011 001 000  c -> stack
    1 100 001 000  decimal
    1 110 001 000  f -> a, f -> a[x]
    1 111 001 000  f exch a, f exch a[x]
    0 001 010 000  keys -> a
    0 010 010 000  a -> rom address
    0 011 010 000  reset twf
    0 100 010 000  binary
    0 101 010 000  rotate a left
    0 110 010 000  p - 1 -> p                 (dec p)
    0 111 010 000  p + 1 -> p                 (inc p)
    1 000 010 000  return
    1 000 110 000  bank switch
    0 000 111 000  c -> addr
    1 010 110 000  clear data registers
    1 011 110 000  c -> data
    1 100 110 000  rom checksum
    1 111 110 000  hi i'm woodstock

    n nnn 000 100  1 -> s(n)
    n nnn 001 100  0 -> s(n)
    n nnn 010 100  if 1 = s(n)
    n nnn 011 000  load n
    n nnn 011 100  if 0 = s(n)
    n nnn 100 000  select rom n
    n nnn 100 100  if p = n
    n nnn 101 000  c -> data register(n)
    n nnn 101 100  if p # n
    n nnn 110 100  delayed rom n
    n nnn 111 000  data register(n)-> c
    n nnn 111 100  p = n

    x xxx xxx x01  jsb
    x xxx xxx x11  if n/c 
    n nnn nnn nnn  then go to


### Arithmetic operations.

      9   8   7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+---+---+
    | n | n | n | n | n | f | f | f | 1 | 0 |
    +---+---+---+---+---+---+---+---+---+---+

   Where fff is the field modifier.

     13   12  11  10  9   8   7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    | s | m | m | m | m | m | m | m | m | m | m | s | e | e |
    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+

    000   P  : determined by P register             ([P])
    001  WP  : word up to and including P register  ([0 .. P])
    010  XS  : exponent and sign                    ([0 .. 2])
    011   X  : exponent                             ([0 .. 1])
    100   S  : sign                                 ([13])
    101   M  : mantissa                             ([3 .. 12])
    110   W  : word                                 ([0 .. 13])
    111  MS  : mantissa and sign                    ([3 .. 13])



    0 000 0ff f10  0 -> a
    0 000 1ff f10  0 -> b
    0 001 0ff f10  a exchange b                    ???
    0 001 0ff f10  a -> b                          ???
    0 001 1ff f10  a exchange c
    0 010 0ff f10  c -> a
    0 010 1ff f10  b -> c
    0 011 0ff f10  b exchange c
    0 011 1ff f10  0 -> c
    0 100 0ff f10  a + b -> a
    0 100 1ff f10  a + c -> a
    0 101 0ff f10  c + c -> c
    0 101 1ff f10  a + c -> c
    0 110 0ff f10  a + 1 -> a
    0 110 1ff f10  shift left a
    0 111 0ff f10  c + 1 -> c
    0 111 1ff f10  a - b -> a
    1 000 1ff f10  a - c -> c
    1 001 0ff f10  a - 1 -> a
    1 001 1ff f10  c - 1 -> c
    1 010 0ff f10  0 - c -> c
    1 010 1ff f10  0 - c - 1 -> c                  (negate)
    1 011 0ff f10  if b = 0
    1 011 1ff f10  if c = 0
    1 100 0ff f10  if a >= c
    1 100 1ff f10  if a >= b
    1 101 0ff f10  if a # 0
    1 101 1ff f10  if c # 0
    1 110 0ff f10  a - c -> a
    1 110 1ff f10  shift right a
    1 111 0ff f10  shift right b
    1 111 1ff f10  shift right c
