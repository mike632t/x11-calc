# Registers

## Program Counter

Logically this is a 13-bit counter in the range 000000 - 017777 (octal)  or
0x0000 - 0x1fff (hexadecimal).

### Classic

The program counter is comprised of a bank number, a group  number, a 3-bit
ROM address and an 8-bit memory address.

     12  11  10   9   8   7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+---+---+---+---+---+
    | b | g | r | r | r | n | n | n | n | n | n | n | n |
    +---+---+---+---+---+---+---+---+---+---+---+---+---+

The program counter is incremented after the current instruction is fetched
from memory but before it is executed.

Incrementing the program counter only changes the 8-bit address field which
is reset to zero if it overflows.

Selecting an address in ROM 0 resets the bank number to zero.

### Woodstock and later

The program counter is comprised of a bank number, a 4-bit ROM address, and
an 8-bit memory address.

     12  11  10   9   8   7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+---+---+---+---+---+
    | b | r | r | r | r | a | a | a | a | a | a | a | a |
    +---+---+---+---+---+---+---+---+---+---+---+---+---+

The program counter is incremented after the current instruction is fetched
from memory but before it is executed.

Incrementing  the  program counter adds one to the combined 12-bit  address
field and is reset to zero if it overflows or the end or memory is
reached.

Selecting an address in ROM 0 resets the bank number to zero.

## Arithmetic Registers

     13  12  11  10   9   8   7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    | s | m | m | m | m | m | m | m | m | m | m | s | e | e |
    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+

    A, B, C:   General purpose registers.  The C register is used to access
               the M register and or memory as well as holding the value of
               the 'X' register.
    Y, Z, T:   Stack registers.
    M, N:      Memory registers.

## Special purpose registers

    F:         F register.
    P:         A  4-bit register that is used to select which part of  each
               register should be used.
    DATA:      An 8-bit register holding the memory address used to read or
               write to memory from the C register.
    SP:        Stack pointer

## Processor flags

Ten processor flags are available (F 0 - F 9)

    F 0        Run / Program mode.
    F 1        Carry.
    F 2        Prev Carry.
    F 3        Delayed ROM select.

    F 4        ROM select
    F 5        Display enabled
    F 6        ???
    F 7        ???

    F 8        Timer.
    F 9        ???

## Processor status word

Comprises of 16 status bits (S 0 - S 15)

    S 0        Not used.
    S 1  *     Scientific notation (clear for fixed point notation).
    S 2  *     Auto Enter (if set entering digit will push 'X').
    S 3  *     Switch position (WOODSTOCK/SPICE) / ? (HP67)

    S 4
    S 5  *     Power OK (WOODSTOCK) / Self test (SPICE)
    S 6        ?
    S 7        ?

    S 8        ?
    S 9        f prefix pressed
    S 10       g prefix pressed
    S 11       Switch position (set if off) (HP67)

    S 12       ?
    S 13       Set if function key has been pressed.
    S 14       Set if EEX has been pressed?
    S 15 *     Set if any key is pressed.

# Instruction set

There are some similarities between the instruction set used in the Classic
and Woodstock series, but they use different sets of opcodes for the Type 0
and Type 2 instructions.

## Type 1 - Jump subroutine (n nnn nnn n01)

            x xxx xxx x01     jsb

## Type 3 - Conditional branch (n nnn nnn n11)

            n nnn nnn n11  if n/c goto nnnnnnnn

## Type 0 - Special operations (n nnn nnn n00)

       9   8   7   6   5   4   3   2   1   0
     +---+---+---+---+---+---+---+---+---+---+
     | n | n | n | n | n | n | n | n | 0 | 0 |
     +---+---+---+---+---+---+---+---+---+---+

All the Type 0 instructions can be further sub-categorised by splitting the
least significant six bits of each opcode into three groups as shown below.

### Woodstock (and Spice) series

    00000   0 000 00 00 00    no operation

    00020   0 000 01 00 00    keys -> rom address
    00120   0 001 01 00 00    keys -> a
    00220   0 010 01 00 00    a -> rom address
    00320   0 011 01 00 00    reset twf
    00420   0 100 01 00 00    binary
    00520   0 101 01 00 00    rotate a left
    00620   0 110 01 00 00    p - 1 -> p
    00720   0 111 01 00 00    p + 1 -> p
    01020   1 000 01 00 00    return

    0nn40   n nnn 10 00 00    select rom n

    01060 * 1 000 11 00 00    bank switch
    01260 * 1 010 11 00 00    clear data registers
    01360 * 1 011 11 00 00    c -> data
    01460 * 1 100 11 00 00    rom checksum
    01760   1 111 11 00 00    hi i'm woodstock

    0nn04   n nnn 00 01 00    1 -> s(n)
    0nn24   n nnn 01 01 00    if 1 = s(n)
    0nn44   n nnn 10 01 00    if p = n
    0nn64   n nnn 11 01 00    delayed select rom n       (where n > 0)

    00010   0 000 00 10 00    clear registers
    00110   0 001 00 10 00    clear status
    00210   0 010 00 10 00    display toggle
    00310   0 011 00 10 00    display off
    00410   0 100 00 10 00    m1 exch c
    00510   0 101 00 10 00    m1 -> c
    00610   0 110 00 10 00    m2 exch c
    00710   0 111 00 10 00    m2 -> c

    01010   1 000 00 10 00    stack -> a
    01110   1 001 00 10 00    down rotate
    01210   1 010 00 10 00    y -> a
    01310   1 011 00 10 00    c -> stack
    01410   1 100 00 10 00    decimal
    01610   1 110 00 10 00    f -> a, f -> a[x]
    01710   1 111 00 10 00    f exch a, f exch a[x]

    0nn30   n nnn 01 10 00    load constant n
    0nn50   n nnn 10 10 00    c -> data register(n)
    01160 * 1 001 11 00 00    c -> data address
    00070 * 0 000 11 10 00    data -> c
    0nn70 * n nnn 11 10 00    data register(n)-> c       (where n > 0)

    0nn14   n nnn 00 11 00    0 -> s(n)
    0nn34   n nnn 01 11 00    if 0 = s(n)
    0nn54   n nnn 10 11 00    if p != n
    (where  nnnn = 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
            p  =  14,  4,  7,  8, 11,  2, 10, 12,  1,  3, 13,  6,  0,  9,  5, 14)
    0nn74   n nnn 11 11 00    n -> p
    (where  nnnn = 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
               p = 4,  8, 12,  2,  9,  1,  6,  3,  1, 13,  5,  0, 11, 10,  7,  4)

Card Reader Circuit instructions

    00060   0 000 11 00 00    Set display digits
    00100   0 000 00 10 00    Test ready
    00160   0 001 11 00 00    Test display digits
    00260   0 010 11 00 00    Motor On
    00300   0 011 00 00 00    Test W/PGM switch
    00360   0 011 11 00 00    Motor Off
    00400   0 100 00 00 00    A key was pressed
    00500   0 101 00 00 00    Test of a key was pressed
    00560   0 101 11 00 00    Test if card inserted
    00660   0 110 11 00 00    Set card write mode
    00760   0 111 11 00 00    Set card read mode
    01000   1 000 00 00 00    Set default function keys
    01100   1 001 00 00 00    Test if default function keys set
    01200   1 010 00 00 00    Set merge flag
    01300   1 011 00 00 00    Test merge flag
    01400   1 100 00 00 00    Set waiting for card side 2 flag
    01500   1 101 00 00 00    Test waiting for card side 2 flag
    01700   1 111 00 00 00    Read/Write data to/from card via RAM $99 and $9B

### Classic series

    00000   0 000 00 00 00    no operation

    00100   0 001 00 00 00    buffer -> rom address
    00200   0 010 00 00 00    memory insert
    00400   0 100 00 00 00    mark and search
    00600   0 110 00 00 00    memory delete

    01200   1 010 00 00 00    search for label
    01400   1 100 00 00 00    pointer advance
    01600   1 110 00 00 00    memory initialize

    0nx20   n nn0 01 00 00    select rom n
    00320   0 011 01 00 00    keys -> rom address

    0nx40   n nn0 10 00 00    1 -> f(n)
    0nx40   n nn1 10 00 00    0 -> f(n)
    00060   0 000 11 00 00    return
    01360 * 1 011 11 00 00    c -> data
    01160 * 1 001 11 00 00    c -> data address

    0nn04   n nnn 00 01 00    1 -> s(n)
    0nn24   n nnn 01 01 00    if 0 = s(n)
    0nn44   n nnn 10 01 00    0 -> s(0)
    00064   0 000 11 01 00    clear status
    0nn64   n nnn 11 01 00    delayed select rom n

    0nn30   n nnn 01 10 00    load constant n

    00050   0 000 10 10 00    display toggle
    00250   0 010 10 10 00    m exch c
    00450   0 100 10 10 00    c -> stack
    00650   0 110 10 10 00    stack -> a
    00750   0 111 10 10 00    bcd -> c
    01050   1 000 10 10 00    display off
    01250   1 010 10 10 00    m -> c
    01450   1 100 10 10 00    down rotate
    01650   1 110 10 10 00    clear registers

    01370   1 011 11 10 00    data -> c

    0nn14   n nnn 00 11 00    n -> p
    00034   0 000 01 11 00    p - 1 -> p
    0nn54   n nnn 10 11 00    if p != n
    00074   0 000 11 11 00    p + 1 -> p

    01064   1 000 11 01 00    delayed select group 0
    01264   1 010 11 01 00    delayed select group 1

    0nx64   n nn1 11 01 00    delayed select rom n

## Type 2 - Arithmetic operations (n nnn nnn n10)

      9   8   7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+---+---+
    | n | n | n | n | n | f | f | f | 1 | 0 |
    +---+---+---+---+---+---+---+---+---+---+

Where fff is the field modifier.

     13   12  11  10  9   8   7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    | s | m | m | m | m | m | m | m | m | m | m | s | x | x |
    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+

The field modifier is used to select which part of the register to use.

### Woodstock (and Spice) series

    000   P : determined by P register             ([P])
    001  WP : word up to and including P register  ([0 .. P])
    010  XS : exponent and sign                    ([0 .. 2])
    011   X : exponent                             ([0 .. 1])
    100   S : sign                                 ([13])
    101   M : mantissa                             ([3 .. 12])
    110   W : word                                 ([0 .. 13])
    111  MS : mantissa and sign                    ([3 .. 13])

            0 000 0 fff 10    0 -> a[f]
            0 000 1 fff 10    0 -> b[f]
            0 001 0 fff 10    a exch b[f]
            0 001 1 fff 10    a -> b[f]
            0 010 0 fff 10    a exch c[f]
            0 010 1 fff 10    c -> a[f]
            0 011 0 fff 10    b -> c[f]
            0 011 1 fff 10    b exch c[f]
            0 100 0 fff 10    0 -> c[f]
            0 100 1 fff 10    a + b -> a[f]
            0 101 0 fff 10    a + c -> a[f]
            0 101 1 fff 10    c + c -> c[f]
            0 110 0 fff 10    a + c -> c[f]
            0 110 1 fff 10    a + 1 -> a[f]
            0 111 0 fff 10    shift left a[f]
            0 111 1 fff 10    c + 1 -> c[f]
            1 000 0 fff 10    a - b -> a[f]
            1 000 1 fff 10    a - c -> c[f]
            1 001 0 fff 10    a - 1 -> a[f]
            1 001 1 fff 10    c - 1 -> c[f]
            1 010 0 fff 10    0 - c -> c[f]
            1 010 1 fff 10    0 - c - 1 -> c[f]
            1 011 0 fff 10    if b[f] = 0
            1 011 1 fff 10    if c[f] = 0
            1 100 0 fff 10    if a >= c[f]
            1 100 1 fff 10    if a >= b[f]
            1 101 0 fff 10    if a[f] != 0
            1 101 1 fff 10    if c[f] != 0
            1 110 0 fff 10    a - c -> a[f]
            1 110 1 fff 10    shift right a[f]
            1 111 0 fff 10    shift right b[f]
            1 111 1 fff 10    shift right c[f]

### Classic series

    000   P : determined by P register             ([P])
    001   M : mantissa                             ([3 .. 12])
    010   X : exponent                             ([0 .. 1])
    011   W : word                                 ([0 .. 13])
    100  WP : word up to and including P register  ([0 .. P])
    101  MS : mantissa and sign                    ([3 .. 13])
    110  XS : exponent and sign                    ([0 .. 2])
    111   S : sign                                 ([13])

            0 000 0 fff 10    if b[f] = 0
            0 000 1 fff 10    0 -> b[f]
            0 001 0 fff 10    if a >= c[f]
            0 001 1 fff 10    if c[f] != 0
            0 010 0 fff 10    b -> c[f]
            0 010 1 fff 10    0 - c -> c[f]
            0 011 0 fff 10    0 -> c[f]
            0 011 1 fff 10    0 - c - 1 -> c[f]
            0 100 0 fff 10    shift left a[f]
            0 100 1 fff 10    a -> b[f]
            0 101 0 fff 10    a - c -> c[f]
            0 101 1 fff 10    c - 1 -> c[f]
            0 110 0 fff 10    c -> a[f]
            0 110 1 fff 10    if c[f] = 0
            0 111 0 fff 10    a + c -> c[f]
            0 111 1 fff 10    c + 1 -> c[f]
            1 000 0 fff 10    if a >= b[f]
            1 000 1 fff 10    b exch c[f]
            1 001 0 fff 10    shift right c[f]
            1 001 1 fff 10    if a[f] != 0
            1 010 0 fff 10    shift right b[f]
            1 010 1 fff 10    c + c -> c[f]
            1 011 0 fff 10    shift right a[f]
            1 011 1 fff 10    0 -> a[f]
            1 100 0 fff 10    a - b -> a[f]
            1 100 1 fff 10    a exch b[f]
            1 101 0 fff 10    a - c -> a[f]
            1 101 1 fff 10    a - 1 -> a[f]
            1 110 0 fff 10    a + b -> a[f]
            1 110 1 fff 10    a exch c[f]
            1 111 0 fff 10    a + c -> a[f]
            1 111 1 fff 10    a + 1 -> a[f]
