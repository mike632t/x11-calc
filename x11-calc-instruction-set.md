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

May be one or two word instructions!

      9   8   7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+---+---+
    | n | n | n | n | n | n | n | n | 0 | 0 |
    +---+---+---+---+---+---+---+---+---+---+

The can be further categorised by splitting the least significant six bits into three groups

   00000    0 000 00 00 00  nop

   00020    0 000 01 00 00  keys -> rom address
   00120    0 001 01 00 00  keys -> a
   00220    0 010 01 00 00  a -> rom address
   00320    0 011 01 00 00  reset twf
   00420    0 100 01 00 00  binary
   00520    0 101 01 00 00  rotate a left
   00620    0 110 01 00 00  p - 1 -> p                 (dec p)
   00720    0 111 01 00 00  p + 1 -> p                 (inc p)
   01020    1 000 01 00 00  return

   0nn40    n nnn 10 00 00  select rom n

   01060 *  1 000 11 00 00  bank switch
   01260 *  1 010 11 00 00  clear data registers
   01360 *  1 011 11 00 00  c -> data
   01460 *  1 100 11 00 00  rom checksum
   01760    1 111 11 00 00  hi i'm woodstock

   0nn04    n nnn 00 01 00  1 -> s(n)
   0nn24    n nnn 01 01 00  if 1 = s(n)
   0nn44    n nnn 10 01 00  if p = n
   0nn64 *  n nnn 11 01 00  delayed rom n

   00010    0 000 00 10 00  clear regs
   00110    0 001 00 10 00  clear s
   00210    0 010 00 10 00  display toggle
   00310    0 011 00 10 00  display off
   00410    0 100 00 10 00  m1 exch c
   00510    0 101 00 10 00  m1 -> c
   00610    0 110 00 10 00  m2 exch c
   00710    0 111 00 10 00  m2 -> c

   01010    1 000 00 10 00  stack -> a
   01110    1 001 00 10 00  down rotate
   01210    1 010 00 10 00  y -> a
   01310    1 011 00 10 00  c -> stack
   01410    1 100 00 10 00  decimal
   01610    1 110 00 10 00  f -> a, f -> a[x]
   01710    1 111 00 10 00  f exch a, f exch a[x]

   0nn30    n nnn 01 10 00  load n
   0nn50    n nnn 10 10 00  c -> data register(n)
   00070 *  0 000 11 10 00  data -> c 
   0nn70 *  n nnn 11 10 00  data register(n)-> c      (n > 0)

   0nn14    n nnn 00 11 00  0 -> s(n)
   0nn34    n nnn 01 11 00  if 0 = s(n)
   0nn54    n nnn 10 11 00  if p # n
            (where nn=   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
                    p=  14,  4,  7,  8, 11,  2, 10, 12,  1,  3, 13,  6,  0,  9,  5, 14)
   0nn74    n nnn 11 11 00  p = n
            (where nn=   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
                    p=   4,  8, 12,  2,  9,  1,  6,  3,  1, 13,  5,  0, 11, 10,  7,  4)

### Jump subroutine

    x xxx xxx x01  jsb


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

### Conditional

    n nnn nnn n11  if n/c goto nnnnnnnn




### Op Codes

                         HP-01     Classic   Woodstock
0 - c - 1 -> c[m]        0346      0346      1266
0 - c - 1 -> c[ms]       0366      0366      1276
0 - c - 1 -> c[p]        0342      0342      1242
0 - c - 1 -> c[s]        0376      0376      1262
0 - c - 1 -> c[w]        0356      0356      1272
0 - c - 1 -> c[wp]       0362      0362      1246
0 - c - 1 -> c[x]        0352      0352      1256
0 - c - 1 -> c[xs]       0372      0372      1252
0 - c -> c[m]            0306      0246      1226
0 - c -> c[ms]           0326      0266      1236
0 - c -> c[p]            0302      0242      1202
0 - c -> c[s]            0336      0276      1222
0 - c -> c[w]            0316      0256      1232
0 - c -> c[wp]           0322      0262      1206
0 - c -> c[x]            0312      0252      1216
0 - c -> c[xs]           0332      0272      1212
0 -> a[m]                1046      1346      0026
0 -> a[ms]               1066      1366      0036
0 -> a[p]                1042      1342      0002
0 -> a[s]                1076      1376      0022
0 -> a[w]                1056      1356      0032
0 -> a[wp]               1062      1362      0006
0 -> a[x]                1052      1352      0016
0 -> a[xs]               1072      1372      0012
0 -> b[m]                1406      0046      0066
0 -> b[ms]               1426      0066      0076
0 -> b[p]                1402      0042      0042
0 -> b[s]                1436      0076      0062
0 -> b[w]                1416      0056      0072
0 -> b[wp]               1422      0062      0046
0 -> b[x]                1412      0052      0056
0 -> b[xs]               1432      0072      0052
0 -> c[m]                0046      0306      0426
0 -> c[ms]               0066      0326      0436
0 -> c[p]                0042      0302      0402
0 -> c[s]                0076      0336      0422
0 -> c[w]                0056      0316      0432
0 -> c[wp]               0062      0322      0406
0 -> c[x]                0052      0312      0416
0 -> c[xs]               0072      0332      0412
0 -> f0                  N/A       0140      N/A
0 -> f1                  N/A       0340      N/A
0 -> f2                  N/A       0540      N/A
0 -> f3                  N/A       0740      N/A
0 -> f4                  N/A       1140      N/A
0 -> f5                  N/A       1340      N/A
0 -> f6                  N/A       1540      N/A
0 -> f7                  N/A       1740      N/A
0 -> p                   0314      0014      1474
0 -> s0                  0044      0044      0014
0 -> s1                  0144      0144      0114
0 -> s1-7                0020      N/A       N/A
0 -> s10                 1244      1244      1214
0 -> s11                 1344      1344      1314
0 -> s12                 1444      N/A       1414
0 -> s13                 1544      N/A       1514
0 -> s14                 1644      N/A       1614
0 -> s15                 1744      N/A       1714
0 -> s2                  0244      0244      0214
0 -> s3                  0344      0344      0314
0 -> s4                  0444      0444      0414
0 -> s5                  0544      0544      0514
0 -> s6                  0644      0644      0614
0 -> s7                  0744      0744      0714
0 -> s8                  1044      1044      1014
0 -> s8-15               0120      N/A       N/A
0 -> s9                  1144      1144      1114
1 -> f0                  N/A       0040      N/A
1 -> f1                  N/A       0240      N/A
1 -> f2                  N/A       0440      N/A
1 -> f3                  N/A       0640      N/A
1 -> f4                  N/A       1040      N/A
1 -> f5                  N/A       1240      N/A
1 -> f6                  N/A       1440      N/A
1 -> f7                  N/A       1640      N/A
1 -> p                   0714      0114      1074
1 -> s0                  0004      0004      0004
1 -> s1                  0104      0104      0104
1 -> s10                 1204      1204      1204
1 -> s11                 1304      1304      1304
1 -> s12                 1404      N/A       1404
1 -> s13                 1504      N/A       1504
1 -> s14                 1604      N/A       1604
1 -> s15                 1704      N/A       1704
1 -> s2                  0204      0204      0204
1 -> s3                  0304      0304      0304
1 -> s4                  0404      0404      0404
1 -> s5                  0504      0504      0504
1 -> s6                  0604      0604      0604
1 -> s7                  0704      0704      0704
1 -> s8                  1004      1004      1004
1 -> s9                  1104      1104      1104
10 -> p                  1614      1214      0674
11 -> p                  0114      1314      0474
12 -> p                  N/A       1414      0774
13 -> p                  N/A       1514      1274
14 -> p                  N/A       1614      N/A
15 -> p                  N/A       1714      N/A
2 -> p                   1114      0214      0574
3 -> p                   1314      0314      1174
4 -> p                   1014      0414      0174
5 -> p                   0414      0514      1674
6 -> p                   1514      0614      1374
7 -> p                   1414      0714      0274
8 -> p                   0214      1014      0374
9 -> p                   0614      1114      1574
a + 1 -> a[m]            1106      1746      0666
a + 1 -> a[ms]           1126      1766      0676
a + 1 -> a[p]            1102      1742      0642
a + 1 -> a[s]            1136      1776      0662
a + 1 -> a[w]            1116      1756      0672
a + 1 -> a[wp]           1122      1762      0646
a + 1 -> a[x]            1112      1752      0656
a + 1 -> a[xs]           1132      1772      0652
a + b -> a[m]            1306      1606      0466
a + b -> a[ms]           1326      1626      0476
a + b -> a[p]            1302      1602      0442
a + b -> a[s]            1336      1636      0462
a + b -> a[w]            1316      1616      0472
a + b -> a[wp]           1322      1622      0446
a + b -> a[x]            1312      1612      0456
a + b -> a[xs]           1332      1632      0452
a + c -> a[m]            0506      1706      0526
a + c -> a[ms]           0526      1726      0536
a + c -> a[p]            0502      1702      0502
a + c -> a[s]            0536      1736      0522
a + c -> a[w]            0516      1716      0532
a + c -> a[wp]           0522      1722      0506
a + c -> a[x]            0512      1712      0516
a + c -> a[xs]           0532      1732      0512
a + c -> c[m]            0706      0706      0626
a + c -> c[ms]           0726      0726      0636
a + c -> c[p]            0702      0702      0602
a + c -> c[s]            0736      0736      0622
a + c -> c[w]            0716      0716      0632
a + c -> c[wp]           0722      0722      0606
a + c -> c[x]            0712      0712      0616
a + c -> c[xs]           0732      0732      0612
a - 1 -> a[m]            1146      1546      1126
a - 1 -> a[ms]           1166      1566      1136
a - 1 -> a[p]            1142      1542      1102
a - 1 -> a[s]            1176      1576      1122
a - 1 -> a[w]            1156      1556      1132
a - 1 -> a[wp]           1162      1562      1106
a - 1 -> a[x]            1152      1552      1116
a - 1 -> a[xs]           1172      1572      1112
a - b -> a[m]            1346      1406      1026
a - b -> a[ms]           1366      1426      1036
a - b -> a[p]            1342      1402      1002
a - b -> a[s]            1376      1436      1022
a - b -> a[w]            1356      1416      1032
a - b -> a[wp]           1362      1422      1006
a - b -> a[x]            1352      1412      1016
a - b -> a[xs]           1372      1432      1012
a - c -> a[m]            0546      1506      1626
a - c -> a[ms]           0566      1526      1636
a - c -> a[p]            0542      1502      1602
a - c -> a[s]            0576      1536      1622
a - c -> a[w]            0556      1516      1632
a - c -> a[wp]           0562      1522      1606
a - c -> a[x]            0552      1512      1616
a - c -> a[xs]           0572      1532      1612
a - c -> c[m]            0746      0506      1066
a - c -> c[ms]           0766      0526      1076
a - c -> c[p]            0742      0502      1042
a - c -> c[s]            0776      0536      1062
a - c -> c[w]            0756      0516      1072
a - c -> c[wp]           0762      0522      1046
a - c -> c[x]            0752      0512      1056
a - c -> c[xs]           0772      0532      1052
a -> al                  0474      N/A       N/A
a -> b[m]                1206      0446      0166
a -> b[ms]               1226      0466      0176
a -> b[p]                1202      0442      0142
a -> b[s]                1236      0476      0162
a -> b[w]                1216      0456      0172
a -> b[wp]               1222      0462      0146
a -> b[x]                1212      0452      0156
a -> b[xs]               1232      0472      0152
a -> cl                  0274      N/A       N/A
a -> clrs                0174      N/A       N/A
a -> dsp                 1374      N/A       N/A
a -> rom address         N/A       N/A       0220
a -> sw                  0674      N/A       N/A
a exchange b[m]          1246      1446      0126
a exchange b[ms]         1266      1466      0136
a exchange b[p]          1242      1442      0102
a exchange b[s]          1276      1476      0122
a exchange b[w]          1256      1456      0132
a exchange b[wp]         1262      1462      0106
a exchange b[x]          1252      1452      0116
a exchange b[xs]         1272      1472      0112
a exchange c[m]          0406      1646      0226
a exchange c[ms]         0426      1666      0236
a exchange c[p]          0402      1642      0202
a exchange c[s]          0436      1676      0222
a exchange c[w]          0416      1656      0232
a exchange c[wp]         0422      1662      0206
a exchange c[x]          0412      1652      0216
a exchange c[xs]         0432      1672      0212
a[p] -> f                1334      N/A       N/A
a[p]=0                   0030      N/A       N/A
a[p]=1                   0130      N/A       N/A
a[p]=2                   0230      N/A       N/A
a[p]=3                   0330      N/A       N/A
a[p]=4                   0430      N/A       N/A
a[p]=5                   0530      N/A       N/A
a[p]=6                   0630      N/A       N/A
a[p]=7                   0730      N/A       N/A
a[p]=8                   1030      N/A       N/A
a[p]=9                   1130      N/A       N/A
a[p]=_dot                1230      N/A       N/A
a[p]=_neg                1330      N/A       N/A
a[p]=_cln                1430      N/A       N/A
a[p]=_lb                 1530      N/A       N/A
a[p]=_3b                 1630      N/A       N/A
a[p]=_bnk                1730      N/A       N/A
al -> a                  1474      N/A       N/A
altog                    1774      N/A       N/A
b -> c[m]                1506      0206      0326
b -> c[ms]               1526      0226      0336
b -> c[p]                1502      0202      0302
b -> c[s]                1536      0236      0322
b -> c[w]                1516      0216      0332
b -> c[wp]               1522      0222      0306
b -> c[x]                1512      0212      0316
b -> c[xs]               1532      0232      0312
b exchange c[m]          1546      1046      0366
b exchange c[ms]         1566      1066      0376
b exchange c[p]          1542      1042      0342
b exchange c[s]          1576      1076      0362
b exchange c[w]          1556      1056      0372
b exchange c[wp]         1562      1062      0346
b exchange c[x]          1552      1052      0356
b exchange c[xs]         1572      1072      0352
bank switch              N/A       N/A       1060
binary                   N/A       N/A       0420
blink                    1134      N/A       N/A
buffer -> rom address    N/A       0100      N/A
c + 1 -> c[m]            0106      0746      0766
c + 1 -> c[ms]           0126      0766      0776
c + 1 -> c[p]            0102      0742      0742
c + 1 -> c[s]            0136      0776      0762
c + 1 -> c[w]            0116      0756      0772
c + 1 -> c[wp]           0122      0762      0746
c + 1 -> c[x]            0112      0752      0756
c + 1 -> c[xs]           0132      0772      0752
c + c -> c[m]            0246      1246      0566
c + c -> c[ms]           0266      1266      0576
c + c -> c[p]            0242      1242      0542
c + c -> c[s]            0276      1276      0562
c + c -> c[w]            0256      1256      0572
c + c -> c[wp]           0262      1262      0546
c + c -> c[x]            0252      1252      0556
c + c -> c[xs]           0272      1272      0552
c - 1 -> c[m]            0146      0546      1166
c - 1 -> c[ms]           0166      0566      1176
c - 1 -> c[p]            0142      0542      1142
c - 1 -> c[s]            0176      0576      1162
c - 1 -> c[w]            0156      0556      1172
c - 1 -> c[wp]           0162      0562      1146
c - 1 -> c[x]            0152      0552      1156
c - 1 -> c[xs]           0172      0572      1152
c -> a[m]                0446      0606      0266
c -> a[ms]               0466      0626      0276
c -> a[p]                0442      0602      0242
c -> a[s]                0476      0636      0262
c -> a[w]                0456      0616      0272
c -> a[wp]               0462      0622      0246
c -> a[x]                0452      0612      0256
c -> a[xs]               0472      0632      0252
c -> data                N/A       1360      1360
c -> data address        N/A       1160      1160
c -> data register 0     N/A       N/A       0050
c -> data register 1     N/A       N/A       0150
c -> data register 10    N/A       N/A       1250
c -> data register 11    N/A       N/A       1350
c -> data register 12    N/A       N/A       1450
c -> data register 13    N/A       N/A       1550
c -> data register 14    N/A       N/A       1650
c -> data register 15    N/A       N/A       1750
c -> data register 2     N/A       N/A       0250
c -> data register 3     N/A       N/A       0350
c -> data register 4     N/A       N/A       0450
c -> data register 5     N/A       N/A       0550
c -> data register 6     N/A       N/A       0650
c -> data register 7     N/A       N/A       0750
c -> data register 8     N/A       N/A       1050
c -> data register 9     N/A       N/A       1150
c -> m                   0434      N/A       N/A
c -> stack               N/A       0450      1310
c exchange d             0134      N/A       N/A
c exchange m             N/A       0250      N/A
cl -> a                  0074      N/A       N/A
clear data registers     N/A       N/A       1260
clear registers          0034      1650      0010
clear status             N/A       0064      0110
crc 100                  N/A       N/A       0100
crc 1000                 N/A       N/A       1000
crc 1100                 N/A       N/A       1100
crc 1200                 N/A       N/A       1200
crc 1300                 N/A       N/A       1300
crc 1400                 N/A       N/A       1400
crc 1500                 N/A       N/A       1500
crc 160                  N/A       N/A       0160
crc 1700                 N/A       N/A       1700
crc 260                  N/A       N/A       0260
crc 300                  N/A       N/A       0300
crc 360                  N/A       N/A       0360
crc 400                  N/A       N/A       0400
crc 500                  N/A       N/A       0500
crc 560                  N/A       N/A       0560
crc 60                   N/A       N/A       0060
crc 660                  N/A       N/A       0660
crc 760                  N/A       N/A       0760
d -> c                   0334      N/A       N/A
data -> c                N/A       1370      0070
data register 0 -> c     N/A       N/A       0070
data register 1 -> c     N/A       N/A       0170
data register 10 -> c    N/A       N/A       1270
data register 11 -> c    N/A       N/A       1370
data register 12 -> c    N/A       N/A       1470
data register 13 -> c    N/A       N/A       1570
data register 14 -> c    N/A       N/A       1670
data register 15 -> c    N/A       N/A       1770
data register 2 -> c     N/A       N/A       0270
data register 3 -> c     N/A       N/A       0370
data register 4 -> c     N/A       N/A       0470
data register 5 -> c     N/A       N/A       0570
data register 6 -> c     N/A       N/A       0670
data register 7 -> c     N/A       N/A       0770
data register 8 -> c     N/A       N/A       1070
data register 9 -> c     N/A       N/A       1170
decimal                  N/A       N/A       1410
delayed select group 0   N/A       1064      N/A
delayed select group 1   N/A       1264      N/A
delayed select rom 0     0064      0164      0064
delayed select rom 1     0164      0364      0164
delayed select rom 10    N/A       N/A       1264
delayed select rom 11    N/A       N/A       1364
delayed select rom 12    N/A       N/A       1464
delayed select rom 13    N/A       N/A       1564
delayed select rom 14    N/A       N/A       1664
delayed select rom 15    N/A       N/A       1764
delayed select rom 2     0264      0564      0264
delayed select rom 3     0364      0764      0364
delayed select rom 4     0464      1164      0464
delayed select rom 5     0564      1364      0564
delayed select rom 6     0664      1564      0664
delayed select rom 7     0764      1764      0764
delayed select rom 8     1064      N/A       1064
delayed select rom 9     N/A       N/A       1164
display off              1034      1050      0310
display on               0734      N/A       N/A
display reset twf        N/A       N/A       0320
display toggle           N/A       0050      0210
down rotate              N/A       1450      1110
dsp -> a                 1734      N/A       N/A
dsp=al                   1174      N/A       N/A
dsp=cl                   0374      N/A       N/A
dsp=sw                   0774      N/A       N/A
dsscwp                   1534      N/A       N/A
enscwp                   1434      N/A       N/A
f -> a                   N/A       N/A       1610
f -> a[p]                1234      N/A       N/A
f exchange a             N/A       N/A       1710
go to $00                0003      0003      0003
go to $01                0007      0007      0007
go to $02                0013      0013      0013
go to $03                0017      0017      0017
go to $04                0023      0023      0023
go to $05                0027      0027      0027
go to $06                0033      0033      0033
go to $07                0037      0037      0037
go to $08                0043      0043      0043
go to $09                0047      0047      0047
go to $0A                0053      0053      0053
go to $0B                0057      0057      0057
go to $0C                0063      0063      0063
go to $0D                0067      0067      0067
go to $0E                0073      0073      0073
go to $0F                0077      0077      0077
go to $10                0103      0103      0103
go to $11                0107      0107      0107
go to $12                0113      0113      0113
go to $13                0117      0117      0117
go to $14                0123      0123      0123
go to $15                0127      0127      0127
go to $16                0133      0133      0133
go to $17                0137      0137      0137
go to $18                0143      0143      0143
go to $19                0147      0147      0147
go to $1A                0153      0153      0153
go to $1B                0157      0157      0157
go to $1C                0163      0163      0163
go to $1D                0167      0167      0167
go to $1E                0173      0173      0173
go to $1F                0177      0177      0177
go to $20                0203      0203      0203
go to $21                0207      0207      0207
go to $22                0213      0213      0213
go to $23                0217      0217      0217
go to $24                0223      0223      0223
go to $25                0227      0227      0227
go to $26                0233      0233      0233
go to $27                0237      0237      0237
go to $28                0243      0243      0243
go to $29                0247      0247      0247
go to $2A                0253      0253      0253
go to $2B                0257      0257      0257
go to $2C                0263      0263      0263
go to $2D                0267      0267      0267
go to $2E                0273      0273      0273
go to $2F                0277      0277      0277
go to $30                0303      0303      0303
go to $31                0307      0307      0307
go to $32                0313      0313      0313
go to $33                0317      0317      0317
go to $34                0323      0323      0323
go to $35                0327      0327      0327
go to $36                0333      0333      0333
go to $37                0337      0337      0337
go to $38                0343      0343      0343
go to $39                0347      0347      0347
go to $3A                0353      0353      0353
go to $3B                0357      0357      0357
go to $3C                0363      0363      0363
go to $3D                0367      0367      0367
go to $3E                0373      0373      0373
go to $3F                0377      0377      0377
go to $40                0403      0403      0403
go to $41                0407      0407      0407
go to $42                0413      0413      0413
go to $43                0417      0417      0417
go to $44                0423      0423      0423
go to $45                0427      0427      0427
go to $46                0433      0433      0433
go to $47                0437      0437      0437
go to $48                0443      0443      0443
go to $49                0447      0447      0447
go to $4A                0453      0453      0453
go to $4B                0457      0457      0457
go to $4C                0463      0463      0463
go to $4D                0467      0467      0467
go to $4E                0473      0473      0473
go to $4F                0477      0477      0477
go to $50                0503      0503      0503
go to $51                0507      0507      0507
go to $52                0513      0513      0513
go to $53                0517      0517      0517
go to $54                0523      0523      0523
go to $55                0527      0527      0527
go to $56                0533      0533      0533
go to $57                0537      0537      0537
go to $58                0543      0543      0543
go to $59                0547      0547      0547
go to $5A                0553      0553      0553
go to $5B                0557      0557      0557
go to $5C                0563      0563      0563
go to $5D                0567      0567      0567
go to $5E                0573      0573      0573
go to $5F                0577      0577      0577
go to $60                0603      0603      0603
go to $61                0607      0607      0607
go to $62                0613      0613      0613
go to $63                0617      0617      0617
go to $64                0623      0623      0623
go to $65                0627      0627      0627
go to $66                0633      0633      0633
go to $67                0637      0637      0637
go to $68                0643      0643      0643
go to $69                0647      0647      0647
go to $6A                0653      0653      0653
go to $6B                0657      0657      0657
go to $6C                0663      0663      0663
go to $6D                0667      0667      0667
go to $6E                0673      0673      0673
go to $6F                0677      0677      0677
go to $70                0703      0703      0703
go to $71                0707      0707      0707
go to $72                0713      0713      0713
go to $73                0717      0717      0717
go to $74                0723      0723      0723
go to $75                0727      0727      0727
go to $76                0733      0733      0733
go to $77                0737      0737      0737
go to $78                0743      0743      0743
go to $79                0747      0747      0747
go to $7A                0753      0753      0753
go to $7B                0757      0757      0757
go to $7C                0763      0763      0763
go to $7D                0767      0767      0767
go to $7E                0773      0773      0773
go to $7F                0777      0777      0777
go to $80                1003      1003      1003
go to $81                1007      1007      1007
go to $82                1013      1013      1013
go to $83                1017      1017      1017
go to $84                1023      1023      1023
go to $85                1027      1027      1027
go to $86                1033      1033      1033
go to $87                1037      1037      1037
go to $88                1043      1043      1043
go to $89                1047      1047      1047
go to $8A                1053      1053      1053
go to $8B                1057      1057      1057
go to $8C                1063      1063      1063
go to $8D                1067      1067      1067
go to $8E                1073      1073      1073
go to $8F                1077      1077      1077
go to $90                1103      1103      1103
go to $91                1107      1107      1107
go to $92                1113      1113      1113
go to $93                1117      1117      1117
go to $94                1123      1123      1123
go to $95                1127      1127      1127
go to $96                1133      1133      1133
go to $97                1137      1137      1137
go to $98                1143      1143      1143
go to $99                1147      1147      1147
go to $9A                1153      1153      1153
go to $9B                1157      1157      1157
go to $9C                1163      1163      1163
go to $9D                1167      1167      1167
go to $9E                1173      1173      1173
go to $9F                1177      1177      1177
go to $A0                1203      1203      1203
go to $A1                1207      1207      1207
go to $A2                1213      1213      1213
go to $A3                1217      1217      1217
go to $A4                1223      1223      1223
go to $A5                1227      1227      1227
go to $A6                1233      1233      1233
go to $A7                1237      1237      1237
go to $A8                1243      1243      1243
go to $A9                1247      1247      1247
go to $AA                1253      1253      1253
go to $AB                1257      1257      1257
go to $AC                1263      1263      1263
go to $AD                1267      1267      1267
go to $AE                1273      1273      1273
go to $AF                1277      1277      1277
go to $B0                1303      1303      1303
go to $B1                1307      1307      1307
go to $B2                1313      1313      1313
go to $B3                1317      1317      1317
go to $B4                1323      1323      1323
go to $B5                1327      1327      1327
go to $B6                1333      1333      1333
go to $B7                1337      1337      1337
go to $B8                1343      1343      1343
go to $B9                1347      1347      1347
go to $BA                1353      1353      1353
go to $BB                1357      1357      1357
go to $BC                1363      1363      1363
go to $BD                1367      1367      1367
go to $BE                1373      1373      1373
go to $BF                1377      1377      1377
go to $C0                1403      1403      1403
go to $C1                1407      1407      1407
go to $C2                1413      1413      1413
go to $C3                1417      1417      1417
go to $C4                1423      1423      1423
go to $C5                1427      1427      1427
go to $C6                1433      1433      1433
go to $C7                1437      1437      1437
go to $C8                1443      1443      1443
go to $C9                1447      1447      1447
go to $CA                1453      1453      1453
go to $CB                1457      1457      1457
go to $CC                1463      1463      1463
go to $CD                1467      1467      1467
go to $CE                1473      1473      1473
go to $CF                1477      1477      1477
go to $D0                1503      1503      1503
go to $D1                1507      1507      1507
go to $D2                1513      1513      1513
go to $D3                1517      1517      1517
go to $D4                1523      1523      1523
go to $D5                1527      1527      1527
go to $D6                1533      1533      1533
go to $D7                1537      1537      1537
go to $D8                1543      1543      1543
go to $D9                1547      1547      1547
go to $DA                1553      1553      1553
go to $DB                1557      1557      1557
go to $DC                1563      1563      1563
go to $DD                1567      1567      1567
go to $DE                1573      1573      1573
go to $DF                1577      1577      1577
go to $E0                1603      1603      1603
go to $E1                1607      1607      1607
go to $E2                1613      1613      1613
go to $E3                1617      1617      1617
go to $E4                1623      1623      1623
go to $E5                1627      1627      1627
go to $E6                1633      1633      1633
go to $E7                1637      1637      1637
go to $E8                1643      1643      1643
go to $E9                1647      1647      1647
go to $EA                1653      1653      1653
go to $EB                1657      1657      1657
go to $EC                1663      1663      1663
go to $ED                1667      1667      1667
go to $EE                1673      1673      1673
go to $EF                1677      1677      1677
go to $F0                1703      1703      1703
go to $F1                1707      1707      1707
go to $F2                1713      1713      1713
go to $F3                1717      1717      1717
go to $F4                1723      1723      1723
go to $F5                1727      1727      1727
go to $F6                1733      1733      1733
go to $F7                1737      1737      1737
go to $F8                1743      1743      1743
go to $F9                1747      1747      1747
go to $FA                1753      1753      1753
go to $FB                1757      1757      1757
go to $FC                1763      1763      1763
go to $FD                1767      1767      1767
go to $FE                1773      1773      1773
go to $FF                1777      1777      1777
gokeys                   0220      N/A       N/A
hi i'm woodstock         N/A       N/A       1760
i8 -> a                  N/A       0350      N/A
if a >= b[m]             1006      1006      1466
if a >= b[ms]            1026      1026      1476
if a >= b[p]             1002      1002      1442
if a >= b[s]             1036      1036      1462
if a >= b[w]             1016      1016      1472
if a >= b[wp]            1022      1022      1446
if a >= b[x]             1012      1012      1456
if a >= b[xs]            1032      1032      1452
if a >= c[m]             0606      0106      1426
if a >= c[ms]            0626      0126      1436
if a >= c[p]             0602      0102      1402
if a >= c[s]             0636      0136      1422
if a >= c[w]             0616      0116      1432
if a >= c[wp]            0622      0122      1406
if a >= c[x]             0612      0112      1416
if a >= c[xs]            0632      0132      1412
if a[m] >= 1             0646      1146      1526
if a[m] # 0              0646      1146      1526
if a[ms] >= 1            0666      1166      1536
if a[ms] # 0             0666      1166      1536
if a[p] >= 1             0642      1142      1502
if a[p] # 0              0642      1142      1502
if a[s] >= 1             0676      1176      1522
if a[s] # 0              0676      1176      1522
if a[w] >= 1             0656      1156      1532
if a[w] # 0              0656      1156      1532
if a[wp] >= 1            0662      1162      1506
if a[wp] # 0             0662      1162      1506
if a[x] >= 1             0652      1152      1516
if a[x] # 0              0652      1152      1516
if a[xs] >= 1            0672      1172      1512
if a[xs] # 0             0672      1172      1512
if b[m] = 0              1446      0006      1326
if b[ms] = 0             1466      0026      1336
if b[p] = 0              1442      0002      1302
if b[s] = 0              1476      0036      1322
if b[w] = 0              1456      0016      1332
if b[wp] = 0             1462      0022      1306
if b[x] = 0              1452      0012      1316
if b[xs] = 0             1472      0032      1312
if c[m] = 0              0006      0646      1366
if c[m] >= 1             0206      0146      1566
if c[m] # 0              0206      0146      1566
if c[ms] = 0             0026      0666      1376
if c[ms] >= 1            0226      0166      1576
if c[ms] # 0             0226      0166      1576
if c[p] = 0              0002      0642      1342
if c[p] >= 1             0202      0142      1542
if c[p] # 0              0202      0142      1542
if c[s] = 0              0036      0676      1362
if c[s] >= 1             0236      0176      1562
if c[s] # 0              0236      0176      1562
if c[w] = 0              0016      0656      1372
if c[w] >= 1             0216      0156      1572
if c[w] # 0              0216      0156      1572
if c[wp] = 0             0022      0662      1346
if c[wp] >= 1            0222      0162      1546
if c[wp] # 0             0222      0162      1546
if c[x] = 0              0012      0652      1356
if c[x] >= 1             0212      0152      1556
if c[x] # 0              0212      0152      1556
if c[xs] = 0             0032      0672      1352
if c[xs] >= 1            0232      0172      1552
if c[xs] # 0             0232      0172      1552
if p # 0                 0354      0054      1354
if p # 1                 0754      0154      0554
if p # 10                1654      1254      1554
if p # 11                0154      1354      1454
if p # 12                N/A       1454      0254
if p # 13                N/A       1554      1154
if p # 14                N/A       1654      N/A
if p # 15                N/A       1754      N/A
if p # 2                 1154      0254      0354
if p # 3                 1354      0354      0754
if p # 4                 1054      0454      0054
if p # 5                 0454      0554      1254
if p # 6                 1554      0654      0654
if p # 7                 1454      0754      1654
if p # 8                 0254      1054      0154
if p # 9                 0654      1154      0454
if p = 0                 N/A       N/A       1344
if p = 1                 N/A       N/A       0544
if p = 10                N/A       N/A       1544
if p = 11                N/A       N/A       1444
if p = 12                N/A       N/A       0244
if p = 13                N/A       N/A       1144
if p = 2                 N/A       N/A       0344
if p = 3                 N/A       N/A       0744
if p = 4                 N/A       N/A       0044
if p = 5                 N/A       N/A       1244
if p = 6                 N/A       N/A       0644
if p = 7                 N/A       N/A       1644
if p = 8                 N/A       N/A       0144
if p = 9                 N/A       N/A       0444
if s0 # 1                0024      0024      0034
if s0 = 0                0024      0024      0034
if s0 = 1                N/A       N/A       0024
if s1 # 1                0124      0124      0134
if s1 = 0                0124      0124      0134
if s1 = 1                N/A       N/A       0124
if s10 # 1               1224      1224      1234
if s10 = 0               1224      1224      1234
if s10 = 1               N/A       N/A       1224
if s11 # 1               1324      1324      1334
if s11 = 0               1324      1324      1334
if s11 = 1               N/A       N/A       1324
if s12 # 1               1424      1424      1434
if s12 = 0               1424      1424      1434
if s12 = 1               N/A       N/A       1424
if s13 # 1               1524      1524      1534
if s13 = 0               1524      1524      1534
if s13 = 1               N/A       N/A       1524
if s14 # 1               1624      1624      1634
if s14 = 0               1624      1624      1634
if s14 = 1               N/A       N/A       1624
if s15 # 1               1724      1724      1734
if s15 = 0               1724      1724      1734
if s15 = 1               N/A       N/A       1724
if s2 # 1                0224      0224      0234
if s2 = 0                0224      0224      0234
if s2 = 1                N/A       N/A       0224
if s3 # 1                0324      0324      0334
if s3 = 0                0324      0324      0334
if s3 = 1                N/A       N/A       0324
if s4 # 1                0424      0424      0434
if s4 = 0                0424      0424      0434
if s4 = 1                N/A       N/A       0424
if s5 # 1                0524      0524      0534
if s5 = 0                0524      0524      0534
if s5 = 1                N/A       N/A       0524
if s6 # 1                0624      0624      0634
if s6 = 0                0624      0624      0634
if s6 = 1                N/A       N/A       0624
if s7 # 1                0724      0724      0734
if s7 = 0                0724      0724      0734
if s7 = 1                N/A       N/A       0724
if s8 # 1                1024      1024      1034
if s8 = 0                1024      1024      1034
if s8 = 1                N/A       N/A       1024
if s9 # 1                1124      1124      1134
if s9 = 0                1124      1124      1134
if s9 = 1                N/A       N/A       1124
jsb $00                  0001      0001      0001
jsb $01                  0005      0005      0005
jsb $02                  0011      0011      0011
jsb $03                  0015      0015      0015
jsb $04                  0021      0021      0021
jsb $05                  0025      0025      0025
jsb $06                  0031      0031      0031
jsb $07                  0035      0035      0035
jsb $08                  0041      0041      0041
jsb $09                  0045      0045      0045
jsb $0A                  0051      0051      0051
jsb $0B                  0055      0055      0055
jsb $0C                  0061      0061      0061
jsb $0D                  0065      0065      0065
jsb $0E                  0071      0071      0071
jsb $0F                  0075      0075      0075
jsb $10                  0101      0101      0101
jsb $11                  0105      0105      0105
jsb $12                  0111      0111      0111
jsb $13                  0115      0115      0115
jsb $14                  0121      0121      0121
jsb $15                  0125      0125      0125
jsb $16                  0131      0131      0131
jsb $17                  0135      0135      0135
jsb $18                  0141      0141      0141
jsb $19                  0145      0145      0145
jsb $1A                  0151      0151      0151
jsb $1B                  0155      0155      0155
jsb $1C                  0161      0161      0161
jsb $1D                  0165      0165      0165
jsb $1E                  0171      0171      0171
jsb $1F                  0175      0175      0175
jsb $20                  0201      0201      0201
jsb $21                  0205      0205      0205
jsb $22                  0211      0211      0211
jsb $23                  0215      0215      0215
jsb $24                  0221      0221      0221
jsb $25                  0225      0225      0225
jsb $26                  0231      0231      0231
jsb $27                  0235      0235      0235
jsb $28                  0241      0241      0241
jsb $29                  0245      0245      0245
jsb $2A                  0251      0251      0251
jsb $2B                  0255      0255      0255
jsb $2C                  0261      0261      0261
jsb $2D                  0265      0265      0265
jsb $2E                  0271      0271      0271
jsb $2F                  0275      0275      0275
jsb $30                  0301      0301      0301
jsb $31                  0305      0305      0305
jsb $32                  0311      0311      0311
jsb $33                  0315      0315      0315
jsb $34                  0321      0321      0321
jsb $35                  0325      0325      0325
jsb $36                  0331      0331      0331
jsb $37                  0335      0335      0335
jsb $38                  0341      0341      0341
jsb $39                  0345      0345      0345
jsb $3A                  0351      0351      0351
jsb $3B                  0355      0355      0355
jsb $3C                  0361      0361      0361
jsb $3D                  0365      0365      0365
jsb $3E                  0371      0371      0371
jsb $3F                  0375      0375      0375
jsb $40                  0401      0401      0401
jsb $41                  0405      0405      0405
jsb $42                  0411      0411      0411
jsb $43                  0415      0415      0415
jsb $44                  0421      0421      0421
jsb $45                  0425      0425      0425
jsb $46                  0431      0431      0431
jsb $47                  0435      0435      0435
jsb $48                  0441      0441      0441
jsb $49                  0445      0445      0445
jsb $4A                  0451      0451      0451
jsb $4B                  0455      0455      0455
jsb $4C                  0461      0461      0461
jsb $4D                  0465      0465      0465
jsb $4E                  0471      0471      0471
jsb $4F                  0475      0475      0475
jsb $50                  0501      0501      0501
jsb $51                  0505      0505      0505
jsb $52                  0511      0511      0511
jsb $53                  0515      0515      0515
jsb $54                  0521      0521      0521
jsb $55                  0525      0525      0525
jsb $56                  0531      0531      0531
jsb $57                  0535      0535      0535
jsb $58                  0541      0541      0541
jsb $59                  0545      0545      0545
jsb $5A                  0551      0551      0551
jsb $5B                  0555      0555      0555
jsb $5C                  0561      0561      0561
jsb $5D                  0565      0565      0565
jsb $5E                  0571      0571      0571
jsb $5F                  0575      0575      0575
jsb $60                  0601      0601      0601
jsb $61                  0605      0605      0605
jsb $62                  0611      0611      0611
jsb $63                  0615      0615      0615
jsb $64                  0621      0621      0621
jsb $65                  0625      0625      0625
jsb $66                  0631      0631      0631
jsb $67                  0635      0635      0635
jsb $68                  0641      0641      0641
jsb $69                  0645      0645      0645
jsb $6A                  0651      0651      0651
jsb $6B                  0655      0655      0655
jsb $6C                  0661      0661      0661
jsb $6D                  0665      0665      0665
jsb $6E                  0671      0671      0671
jsb $6F                  0675      0675      0675
jsb $70                  0701      0701      0701
jsb $71                  0705      0705      0705
jsb $72                  0711      0711      0711
jsb $73                  0715      0715      0715
jsb $74                  0721      0721      0721
jsb $75                  0725      0725      0725
jsb $76                  0731      0731      0731
jsb $77                  0735      0735      0735
jsb $78                  0741      0741      0741
jsb $79                  0745      0745      0745
jsb $7A                  0751      0751      0751
jsb $7B                  0755      0755      0755
jsb $7C                  0761      0761      0761
jsb $7D                  0765      0765      0765
jsb $7E                  0771      0771      0771
jsb $7F                  0775      0775      0775
jsb $80                  1001      1001      1001
jsb $81                  1005      1005      1005
jsb $82                  1011      1011      1011
jsb $83                  1015      1015      1015
jsb $84                  1021      1021      1021
jsb $85                  1025      1025      1025
jsb $86                  1031      1031      1031
jsb $87                  1035      1035      1035
jsb $88                  1041      1041      1041
jsb $89                  1045      1045      1045
jsb $8A                  1051      1051      1051
jsb $8B                  1055      1055      1055
jsb $8C                  1061      1061      1061
jsb $8D                  1065      1065      1065
jsb $8E                  1071      1071      1071
jsb $8F                  1075      1075      1075
jsb $90                  1101      1101      1101
jsb $91                  1105      1105      1105
jsb $92                  1111      1111      1111
jsb $93                  1115      1115      1115
jsb $94                  1121      1121      1121
jsb $95                  1125      1125      1125
jsb $96                  1131      1131      1131
jsb $97                  1135      1135      1135
jsb $98                  1141      1141      1141
jsb $99                  1145      1145      1145
jsb $9A                  1151      1151      1151
jsb $9B                  1155      1155      1155
jsb $9C                  1161      1161      1161
jsb $9D                  1165      1165      1165
jsb $9E                  1171      1171      1171
jsb $9F                  1175      1175      1175
jsb $A0                  1201      1201      1201
jsb $A1                  1205      1205      1205
jsb $A2                  1211      1211      1211
jsb $A3                  1215      1215      1215
jsb $A4                  1221      1221      1221
jsb $A5                  1225      1225      1225
jsb $A6                  1231      1231      1231
jsb $A7                  1235      1235      1235
jsb $A8                  1241      1241      1241
jsb $A9                  1245      1245      1245
jsb $AA                  1251      1251      1251
jsb $AB                  1255      1255      1255
jsb $AC                  1261      1261      1261
jsb $AD                  1265      1265      1265
jsb $AE                  1271      1271      1271
jsb $AF                  1275      1275      1275
jsb $B0                  1301      1301      1301
jsb $B1                  1305      1305      1305
jsb $B2                  1311      1311      1311
jsb $B3                  1315      1315      1315
jsb $B4                  1321      1321      1321
jsb $B5                  1325      1325      1325
jsb $B6                  1331      1331      1331
jsb $B7                  1335      1335      1335
jsb $B8                  1341      1341      1341
jsb $B9                  1345      1345      1345
jsb $BA                  1351      1351      1351
jsb $BB                  1355      1355      1355
jsb $BC                  1361      1361      1361
jsb $BD                  1365      1365      1365
jsb $BE                  1371      1371      1371
jsb $BF                  1375      1375      1375
jsb $C0                  1401      1401      1401
jsb $C1                  1405      1405      1405
jsb $C2                  1411      1411      1411
jsb $C3                  1415      1415      1415
jsb $C4                  1421      1421      1421
jsb $C5                  1425      1425      1425
jsb $C6                  1431      1431      1431
jsb $C7                  1435      1435      1435
jsb $C8                  1441      1441      1441
jsb $C9                  1445      1445      1445
jsb $CA                  1451      1451      1451
jsb $CB                  1455      1455      1455
jsb $CC                  1461      1461      1461
jsb $CD                  1465      1465      1465
jsb $CE                  1471      1471      1471
jsb $CF                  1475      1475      1475
jsb $D0                  1501      1501      1501
jsb $D1                  1505      1505      1505
jsb $D2                  1511      1511      1511
jsb $D3                  1515      1515      1515
jsb $D4                  1521      1521      1521
jsb $D5                  1525      1525      1525
jsb $D6                  1531      1531      1531
jsb $D7                  1535      1535      1535
jsb $D8                  1541      1541      1541
jsb $D9                  1545      1545      1545
jsb $DA                  1551      1551      1551
jsb $DB                  1555      1555      1555
jsb $DC                  1561      1561      1561
jsb $DD                  1565      1565      1565
jsb $DE                  1571      1571      1571
jsb $DF                  1575      1575      1575
jsb $E0                  1601      1601      1601
jsb $E1                  1605      1605      1605
jsb $E2                  1611      1611      1611
jsb $E3                  1615      1615      1615
jsb $E4                  1621      1621      1621
jsb $E5                  1625      1625      1625
jsb $E6                  1631      1631      1631
jsb $E7                  1635      1635      1635
jsb $E8                  1641      1641      1641
jsb $E9                  1645      1645      1645
jsb $EA                  1651      1651      1651
jsb $EB                  1655      1655      1655
jsb $EC                  1661      1661      1661
jsb $ED                  1665      1665      1665
jsb $EE                  1671      1671      1671
jsb $EF                  1675      1675      1675
jsb $F0                  1701      1701      1701
jsb $F1                  1705      1705      1705
jsb $F2                  1711      1711      1711
jsb $F3                  1715      1715      1715
jsb $F4                  1721      1721      1721
jsb $F5                  1725      1725      1725
jsb $F6                  1731      1731      1731
jsb $F7                  1735      1735      1735
jsb $F8                  1741      1741      1741
jsb $F9                  1745      1745      1745
jsb $FA                  1751      1751      1751
jsb $FB                  1755      1755      1755
jsb $FC                  1761      1761      1761
jsb $FD                  1765      1765      1765
jsb $FE                  1771      1771      1771
jsb $FF                  1775      1775      1775
keys -> a                N/A       N/A       0120
keys -> rom address      N/A       0320      0020
load constant 0          N/A       0030      0030
load constant 1          N/A       0130      0130
load constant 10         N/A       1230      1230
load constant 11         N/A       1330      1330
load constant 12         N/A       N/A       1430
load constant 13         N/A       N/A       1530
load constant 14         N/A       N/A       1630
load constant 15         N/A       N/A       1730
load constant 2          N/A       0230      0230
load constant 3          N/A       0330      0330
load constant 4          N/A       0430      0430
load constant 5          N/A       0530      0530
load constant 6          N/A       0630      0630
load constant 7          N/A       0730      0730
load constant 8          N/A       1030      1030
load constant 9          N/A       1130      1130
m -> c                   0234      1250      N/A
m1 -> c                  N/A       N/A       0510
m1 exchange c            N/A       N/A       0410
m2 -> c                  N/A       N/A       0710
m2 exchange c            N/A       N/A       0610
mark and search          N/A       0400      N/A
memory delete            N/A       0600      N/A
memory full -> a         N/A       0745      N/A
memory initialize        N/A       1600      N/A
memory insert            N/A       0200      N/A
no operation             0000      0000      0000
p + 1 -> p               0320      1374      0720
p - 1 -> p               0420      1334      0620
pointer advance          N/A       1400      N/A
return                   0520      0060      1020
rotate a left            N/A       N/A       0520
search for label         N/A       1200      N/A
select rom 0             N/A       0020      0040
select rom 1             N/A       0220      0140
select rom 10            N/A       N/A       1240
select rom 11            N/A       N/A       1340
select rom 12            N/A       N/A       1440
select rom 13            N/A       N/A       1540
select rom 14            N/A       N/A       1640
select rom 15            N/A       N/A       1740
select rom 2             N/A       0420      0240
select rom 3             N/A       0620      0340
select rom 4             N/A       1020      0440
select rom 5             N/A       1220      0540
select rom 6             N/A       1420      0640
select rom 7             N/A       1620      0740
select rom 8             N/A       N/A       1040
select rom 9             N/A       N/A       1140
shift left a[m]          1646      0406      0726
shift left a[ms]         1666      0426      0736
shift left a[p]          1642      0402      0702
shift left a[s]          1676      0436      0722
shift left a[w]          1656      0416      0732
shift left a[wp]         1662      0422      0706
shift left a[x]          1652      0412      0716
shift left a[xs]         1672      0432      0712
shift right a[m]         1606      1306      1666
shift right a[ms]        1626      1326      1676
shift right a[p]         1602      1302      1642
shift right a[s]         1636      1336      1662
shift right a[w]         1616      1316      1672
shift right a[wp]        1622      1322      1646
shift right a[x]         1612      1312      1656
shift right a[xs]        1632      1332      1652
shift right b[m]         1706      1206      1726
shift right b[ms]        1726      1226      1736
shift right b[p]         1702      1202      1702
shift right b[s]         1736      1236      1722
shift right b[w]         1716      1216      1732
shift right b[wp]        1722      1222      1706
shift right b[x]         1712      1212      1716
shift right b[xs]        1732      1232      1712
shift right c[m]         1746      1106      1766
shift right c[ms]        1766      1126      1776
shift right c[p]         1742      1102      1742
shift right c[s]         1776      1136      1762
shift right c[w]         1756      1116      1772
shift right c[wp]        1762      1122      1746
shift right c[x]         1752      1112      1756
shift right c[xs]        1772      1132      1752
sleep                    0620      N/A       N/A
stack -> a               N/A       0650      1010
sw -> a                  0574      N/A       N/A
sw+                      1274      N/A       N/A
sw-                      1074      N/A       N/A
swstop                   1674      N/A       N/A
swstrt                   1574      N/A       N/A
y -> a                   N/A       N/A       1210
rom checksum             N/A       N/A       1460
pik1120                  N/A       N/A       1120
pik1220                  N/A       N/A       1220
pik1320                  N/A       N/A       1320
pik1660                  N/A       N/A       1660
pik1720                  N/A       N/A       1720
pik1420                  N/A       N/A       1660
pik1620                  N/A       N/A       1660
crc testf1               N/A       N/A       N/A
crc testf2               N/A       N/A       N/A
crc testf3               N/A       N/A       N/A
crc testf4               N/A       N/A       N/A
bcd -> c                 N/A       0750      N/A
