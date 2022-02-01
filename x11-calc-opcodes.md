## Op Codes

### Group 0 opcodes (n nnn nnn n00)

    Op Code               Classic   Woodstock
    0 -> f0                0140      N/A
    0 -> f1                0340      N/A
    0 -> f2                0540      N/A
    0 -> f3                0740      N/A
    0 -> f4                1140      N/A
    0 -> f5                1340      N/A
    0 -> f6                1540      N/A
    0 -> f7                1740      N/A
    1 -> f0                0040      N/A
    1 -> f1                0240      N/A
    1 -> f2                0440      N/A
    1 -> f3                0640      N/A
    1 -> f4                1040      N/A
    1 -> f5                1240      N/A
    1 -> f6                1440      N/A
    1 -> f7                1640      N/A
    0 -> p                 0014      1474
    1 -> p                 0114      1074
    2 -> p                 0214      0574
    3 -> p                 0314      1174
    4 -> p                 0414      0174
    5 -> p                 0514      1674
    6 -> p                 0614      1374
    7 -> p                 0714      0274
    8 -> p                 1014      0374
    9 -> p                 1114      1574
    10 -> p                1214      0674
    11 -> p                1314      0474
    12 -> p                1414      0774
    13 -> p                1514      1274
    14 -> p                1614      N/A
    15 -> p                1714      N/A
    0 -> s0                0044      0014
    0 -> s1                0144      0114
    0 -> s2                0244      0214
    0 -> s3                0344      0314
    0 -> s4                0444      0414
    0 -> s5                0544      0514
    0 -> s6                0644      0614
    0 -> s7                0744      0714
    0 -> s8                1044      1014
    0 -> s9                1144      1114
    0 -> s10               1244      1214
    0 -> s11               1344      1314
    0 -> s12               N/A       1414
    0 -> s13               N/A       1514
    0 -> s14               N/A       1614
    0 -> s15               N/A       1714
    1 -> s0                0004      0004
    1 -> s1                0104      0104
    1 -> s2                0204      0204
    1 -> s3                0304      0304
    1 -> s4                0404      0404
    1 -> s5                0504      0504
    1 -> s6                0604      0604
    1 -> s7                0704      0704
    1 -> s8                1004      1004
    1 -> s9                1104      1104
    1 -> s10               1204      1204
    1 -> s11               1304      1304
    1 -> s12               N/A       1404
    1 -> s13               N/A       1504
    1 -> s14               N/A       1604
    1 -> s15               N/A       1704
    a -> rom address       N/A       0220
    bank switch            N/A       1060
    bcd -> c               0750      N/A
    binary                 N/A       0420
    buffer -> rom address  0100      N/A
    c -> data              1360      1360
    c -> data address      1160      1160
    c -> data register 0   N/A       0050
    c -> data register 1   N/A       0150
    c -> data register 2   N/A       0250
    c -> data register 3   N/A       0350
    c -> data register 4   N/A       0450
    c -> data register 5   N/A       0550
    c -> data register 6   N/A       0650
    c -> data register 7   N/A       0750
    c -> data register 8   N/A       1050
    c -> data register 9   N/A       1150
    c -> data register 10  N/A       1250
    c -> data register 11  N/A       1350
    c -> data register 12  N/A       1450
    c -> data register 13  N/A       1550
    c -> data register 14  N/A       1650
    c -> data register 15  N/A       1750
    c exch m               0250      N/A
    clear data registers   N/A       1260
    clear registers        1650      0010
    clear status           0064      0110
    c -> stack             0450      1310
    data -> c              1370      N/A
    data register 0 -> c   N/A       0070
    data register 1 -> c   N/A       0170
    data register 2 -> c   N/A       0270
    data register 3 -> c   N/A       0370
    data register 4 -> c   N/A       0470
    data register 5 -> c   N/A       0570
    data register 6 -> c   N/A       0670
    data register 7 -> c   N/A       0770
    data register 8 -> c   N/A       1070
    data register 9 -> c   N/A       1170
    data register 10 -> c  N/A       1270
    data register 11 -> c  N/A       1370
    data register 12 -> c  N/A       1470
    data register 13 -> c  N/A       1570
    data register 14 -> c  N/A       1670
    data register 15 -> c  N/A       1770
    decimal                N/A       1410
    delayed select group 0 1064      N/A
    delayed select group 1 1264      N/A
    delayed select rom 0   0164      0064
    delayed select rom 1   0364      0164
    delayed select rom 2   0564      0264
    delayed select rom 3   0764      0364
    delayed select rom 4   1164      0464
    delayed select rom 5   1364      0564
    delayed select rom 6   1564      0664
    delayed select rom 7   1764      0764
    delayed select rom 8   N/A       1064
    delayed select rom 9   N/A       1164
    delayed select rom 10  N/A       1264
    delayed select rom 11  N/A       1364
    delayed select rom 12  N/A       1464
    delayed select rom 13  N/A       1564
    delayed select rom 14  N/A       1664
    delayed select rom 15  N/A       1764
    display off            1050      0310
    display reset twf      N/A       0320
    display toggle         0050      0210
    down rotate            1450      1110
    f -> a                 N/A       1610
    f exch a               N/A       1710
    hi i'm woodstock       N/A       1760
    i8 -> a                0350      N/A
    if p != 0              0054      1354
    if p = 0               N/A       1344
    if p != 1              0154      0554
    if p = 1               N/A       0544
    if p != 2              0254      0354
    if p = 2               N/A       0344
    if p != 3              0354      0754
    if p = 3               N/A       0744
    if p != 4              0454      0054
    if p = 4               N/A       0044
    if p != 5              0554      1254
    if p = 5               N/A       1244
    if p != 6              0654      0654
    if p = 6               N/A       0644
    if p != 7              0754      1654
    if p = 7               N/A       1644
    if p != 8              1054      0154
    if p = 8               N/A       0144
    if p != 9              1154      0454
    if p = 9               N/A       0444
    if p != 10             1254      1554
    if p = 10              N/A       1544
    if p != 11             1354      1454
    if p = 11              N/A       1444
    if p != 12             1454      0254
    if p = 12              N/A       0244
    if p != 13             1554      1154
    if p = 13              N/A       1144
    if p != 14             1654      N/A
    if p != 15             1754      N/A
    if s0 = 0              0024      0034
    if s0 != 1             0024      0034
    if s0 = 1              N/A       0024
    if s1 = 1              N/A       0124
    if s1 = 0              0124      0134
    if s1 != 1             0124      0134
    if s2 = 0              0224      0234
    if s2 != 1             0224      0234
    if s2 = 1              N/A       0224
    if s3 = 0              0324      0334
    if s3 != 1             0324      0334
    if s3 = 1              N/A       0324
    if s4 = 0              0424      0434
    if s4 != 1             0424      0434
    if s4 = 1              N/A       0424
    if s5 = 0              0524      0534
    if s5 != 1             0524      0534
    if s5 = 1              N/A       0524
    if s6 = 0              0624      0634
    if s6 != 1             0624      0634
    if s6 = 1              N/A       0624
    if s7 = 0              0724      0734
    if s7 != 1             0724      0734
    if s7 = 1              N/A       0724
    if s8 = 0              1024      1034
    if s8 != 1             1024      1034
    if s8 = 1              N/A       1024
    if s9 = 0              1124      1134
    if s9 != 1             1124      1134
    if s9 = 1              N/A       1124
    if s10 = 0             1224      1234
    if s10 != 1            1224      1234
    if s10 = 1             N/A       1224
    if s11 = 0             1324      1334
    if s11 != 1            1324      1334
    if s11 = 1             N/A       1324
    if s12 = 0             1424      1434
    if s12 != 1            1424      1434
    if s12 = 1             N/A       1424
    if s13 = 0             1524      1534
    if s13 != 1            1524      1534
    if s13 = 1             N/A       1524
    if s14 = 0             1624      1634
    if s14 != 1            1624      1634
    if s14 = 1             N/A       1624
    if s15 = 0             1724      1734
    if s15 != 1            1724      1734
    if s15 = 1             N/A       1724
    keys -> a              N/A       0120
    keys -> rom address    0320      0020
    load constant 0        0030      0030
    load constant 1        0130      0130
    load constant 2        0230      0230
    load constant 3        0330      0330
    load constant 4        0430      0430
    load constant 5        0530      0530
    load constant 6        0630      0630
    load constant 7        0730      0730
    load constant 8        1030      1030
    load constant 9        1130      1130
    load constant 10       1230      1230
    load constant 11       1330      1330
    load constant 12       N/A       1430
    load constant 13       N/A       1530
    load constant 14       N/A       1630
    load constant 15       N/A       1730
    m -> c                 N/A       0510
    m exch c               N/A       0410
    n -> c                 N/A       0710
    n exch c               N/A       0610
    mark and search        0400      N/A
    m -> c                 1250      N/A
    memory delete          0600      N/A
    memory full -> a       0745      N/A
    memory initialize      1600      N/A
    memory insert          0200      N/A
    no operation           0000      0000
    p - 1 -> p             0034      0620
    p + 1 -> p             0074      0720

    pointer advance        1400      N/A
    return                 0060      1020
    rom checksum           N/A       1460
    rotate a left          N/A       0520
    search for label       1200      N/A
    select rom 0           0020      0040
    select rom 1           0220      0140
    select rom 2           0420      0240
    select rom 3           0620      0340
    select rom 4           1020      0440
    select rom 5           1220      0540
    select rom 6           1420      0640
    select rom 7           1620      0740
    select rom 8           N/A       1040
    select rom 9           N/A       1140
    select rom 10          N/A       1240
    select rom 11          N/A       1340
    select rom 12          N/A       1440
    select rom 13          N/A       1540
    select rom 14          N/A       1640
    select rom 15          N/A       1740
    stack -> a             0650      1010
    y -> a                 N/A       1210

    crc 60                 N/A       0060
    crc 100                N/A       0100
    crc 160                N/A       0160
    crc 260                N/A       0260
    crc 300                N/A       0300
    crc 360                N/A       0360
    crc 400                N/A       0400
    crc 500                N/A       0500
    crc 560                N/A       0560
    crc 660                N/A       0660
    crc 760                N/A       0760
    crc 1000               N/A       1000
    crc 1100               N/A       1100
    crc 1200               N/A       1200
    crc 1300               N/A       1300
    crc 1400               N/A       1400
    crc 1500               N/A       1500
    crc 1700               N/A       1700
    crc testf1             N/A       N/A
    crc testf2             N/A       N/A
    crc testf3             N/A       N/A
    crc testf4             N/A       N/A
    pik 1120               N/A       1120
    pik 1220               N/A       1220
    pik 1320               N/A       1320
    pik 1660               N/A       1660
    pik 1720               N/A       1720
    pik 1420               N/A       1660
    pik 1620               N/A       1660

### Group 1 opcodes (n nnn nnn n10)

    Op Code               Classic   Woodstock
    jsb 0000               0001      0001
    jsb 0001               0005      0005
    jsb 0002               0011      0011
    jsb 0003               0015      0015
    jsb 0004               0021      0021
    jsb 0005               0025      0025
    jsb 0006               0031      0031
    jsb 0007               0035      0035
    jsb 0010               0041      0041
    jsb 0011               0045      0045
    jsb 0012               0051      0051
    jsb 0013               0055      0055
    jsb 0014               0061      0061
    jsb 0015               0065      0065
    jsb 0016               0071      0071
    jsb 0017               0075      0075
    jsb 0020               0101      0101
    jsb 0021               0105      0105
    jsb 0022               0111      0111
    jsb 0023               0115      0115
    jsb 0024               0121      0121
    jsb 0025               0125      0125
    jsb 0026               0131      0131
    jsb 0027               0135      0135
    jsb 0030               0141      0141
    jsb 0031               0145      0145
    jsb 0032               0151      0151
    jsb 0033               0155      0155
    jsb 0034               0161      0161
    jsb 0035               0165      0165
    jsb 0036               0171      0171
    jsb 0037               0175      0175
    jsb 0040               0201      0201
    jsb 0041               0205      0205
    jsb 0042               0211      0211
    jsb 0043               0215      0215
    jsb 0044               0221      0221
    jsb 0045               0225      0225
    jsb 0046               0231      0231
    jsb 0047               0235      0235
    jsb 0050               0241      0241
    jsb 0051               0245      0245
    jsb 0052               0251      0251
    jsb 0053               0255      0255
    jsb 0054               0261      0261
    jsb 0055               0265      0265
    jsb 0056               0271      0271
    jsb 0057               0275      0275
    jsb 0060               0301      0301
    jsb 0061               0305      0305
    jsb 0062               0311      0311
    jsb 0063               0315      0315
    jsb 0064               0321      0321
    jsb 0065               0325      0325
    jsb 0066               0331      0331
    jsb 0067               0335      0335
    jsb 0070               0341      0341
    jsb 0071               0345      0345
    jsb 0072               0351      0351
    jsb 0073               0355      0355
    jsb 0074               0361      0361
    jsb 0075               0365      0365
    jsb 0076               0371      0371
    jsb 0077               0375      0375
    jsb 0100               0401      0401
    jsb 0101               0405      0405
    jsb 0102               0411      0411
    jsb 0103               0415      0415
    jsb 0104               0421      0421
    jsb 0105               0425      0425
    jsb 0106               0431      0431
    jsb 0107               0435      0435
    jsb 0110               0441      0441
    jsb 0111               0445      0445
    jsb 0112               0451      0451
    jsb 0113               0455      0455
    jsb 0114               0461      0461
    jsb 0115               0465      0465
    jsb 0116               0471      0471
    jsb 0117               0475      0475
    jsb 0120               0501      0501
    jsb 1021               0505      0505
    jsb 0122               0511      0511
    jsb 0123               0515      0515
    jsb 0124               0521      0521
    jsb 0125               0525      0525
    jsb 0126               0531      0531
    jsb 0127               0535      0535
    jsb 0130               0541      0541
    jsb 0131               0545      0545
    jsb 0132               0551      0551
    jsb 0133               0555      0555
    jsb 0134               0561      0561
    jsb 0135               0565      0565
    jsb 0136               0571      0571
    jsb 0137               0575      0575
    jsb 0140               0601      0601
    jsb 0141               0605      0605
    jsb 0142               0611      0611
    jsb 0143               0615      0615
    jsb 0144               0621      0621
    jsb 1045               0625      0625
    jsb 0146               0631      0631
    jsb 0147               0635      0635
    jsb 0150               0641      0641
    jsb 0151               0645      0645
    jsb 0152               0651      0651
    jsb 0153               0655      0655
    jsb 0154               0661      0661
    jsb 0155               0665      0665
    jsb 0156               0671      0671
    jsb 0157               0675      0675
    jsb 0160               0701      0701
    jsb 0161               0705      0705
    jsb 0162               0711      0711
    jsb 0163               0715      0715
    jsb 0164               0721      0721
    jsb 0165               0725      0725
    jsb 0166               0731      0731
    jsb 0167               0735      0735
    jsb 0170               0741      0741
    jsb 0171               0745      0745
    jsb 0172               0751      0751
    jsb 0173               0755      0755
    jsb 0174               0761      0761
    jsb 0175               0765      0765
    jsb 0176               0771      0771
    jsb 0177               0775      0775
    jsb 0200               1001      1001
    jsb 0201               1005      1005
    jsb 0202               1011      1011
    jsb 0203               1015      1015
    jsb 0204               1021      1021
    jsb 0205               1025      1025
    jsb 0206               1031      1031
    jsb 0207               1035      1035
    jsb 0210               1041      1041
    jsb 0211               1045      1045
    jsb 0212               1051      1051
    jsb 0213               1055      1055
    jsb 0214               1061      1061
    jsb 0215               1065      1065
    jsb 0216               1071      1071
    jsb 0217               1075      1075
    jsb 0220               1101      1101
    jsb 0221               1105      1105
    jsb 0222               1111      1111
    jsb 0223               1115      1115
    jsb 0224               1121      1121
    jsb 0225               1125      1125
    jsb 0226               1131      1131
    jsb 0227               1135      1135
    jsb 0230               1141      1141
    jsb 0231               1145      1145
    jsb 0232               1151      1151
    jsb 0233               1155      1155
    jsb 0234               1161      1161
    jsb 0235               1165      1165
    jsb 0236               1171      1171
    jsb 0237               1175      1175
    jsb 0240               1201      1201
    jsb 0241               1205      1205
    jsb 0242               1211      1211
    jsb 0243               1215      1215
    jsb 0244               1221      1221
    jsb 0245               1225      1225
    jsb 0246               1231      1231
    jsb 0247               1235      1235
    jsb 0250               1241      1241
    jsb 0251               1245      1245
    jsb 0252               1251      1251
    jsb 0253               1255      1255
    jsb 0254               1261      1261
    jsb 0255               1265      1265
    jsb 0256               1271      1271
    jsb 0257               1275      1275
    jsb 0260               1301      1301
    jsb 0261               1305      1305
    jsb 0262               1311      1311
    jsb 0263               1315      1315
    jsb 0264               1321      1321
    jsb 0265               1325      1325
    jsb 0266               1331      1331
    jsb 0267               1335      1335
    jsb 0270               1341      1341
    jsb 0271               1345      1345
    jsb 0272               1351      1351
    jsb 0273               1355      1355
    jsb 0274               1361      1361
    jsb 0275               1365      1365
    jsb 0276               1371      1371
    jsb 0277               1375      1375
    jsb 0300               1401      1401
    jsb 0301               1405      1405
    jsb 0302               1411      1411
    jsb 0303               1415      1415
    jsb 0304               1421      1421
    jsb 0305               1425      1425
    jsb 0306               1431      1431
    jsb 0307               1435      1435
    jsb 0310               1441      1441
    jsb 0311               1445      1445
    jsb 0312               1451      1451
    jsb 0313               1455      1455
    jsb 0314               1461      1461
    jsb 0315               1465      1465
    jsb 0316               1471      1471
    jsb 0317               1475      1475
    jsb 0320               1501      1501
    jsb 0321               1505      1505
    jsb 0322               1511      1511
    jsb 0323               1515      1515
    jsb 0324               1521      1521
    jsb 0325               1525      1525
    jsb 0326               1531      1531
    jsb 0327               1535      1535
    jsb 0330               1541      1541
    jsb 0331               1545      1545
    jsb 0332               1551      1551
    jsb 0333               1555      1555
    jsb 0334               1561      1561
    jsb 0335               1565      1565
    jsb 0336               1571      1571
    jsb 0337               1575      1575
    jsb 0340               1601      1601
    jsb 0341               1605      1605
    jsb 0342               1611      1611
    jsb 0343               1615      1615
    jsb 0344               1621      1621
    jsb 0345               1625      1625
    jsb 0346               1631      1631
    jsb 0347               1635      1635
    jsb 0350               1641      1641
    jsb 0351               1645      1645
    jsb 0352               1651      1651
    jsb 0353               1655      1655
    jsb 0354               1661      1661
    jsb 0355               1665      1665
    jsb 0356               1671      1671
    jsb 0357               1675      1675
    jsb 0360               1701      1701
    jsb 0361               1705      1705
    jsb 0362               1711      1711
    jsb 0363               1715      1715
    jsb 0364               1721      1721
    jsb 0365               1725      1725
    jsb 0366               1731      1731
    jsb 0367               1735      1735
    jsb 0370               1741      1741
    jsb 0371               1745      1745
    jsb 0372               1751      1751
    jsb 0373               1755      1755
    jsb 0374               1761      1761
    jsb 0375               1765      1765
    jsb 0376               1771      1771
    jsb 0377               1775      1775

### Group 2 opcodes (n nnn nnn n10)

    Op Code               Classic   Woodstock
    0 -> a[m]              1346      0026
    0 -> a[ms]             1366      0036
    0 -> a[p]              1342      0002
    0 -> a[s]              1376      0022
    0 -> a[w]              1356      0032
    0 -> a[wp]             1362      0006
    0 -> a[x]              1352      0016
    0 -> a[xs]             1372      0012
    0 -> b[m]              0046      0066
    0 -> b[ms]             0066      0076
    0 -> b[p]              0042      0042
    0 -> b[s]              0076      0062
    0 -> b[w]              0056      0072
    0 -> b[wp]             0062      0046
    0 -> b[x]              0052      0056
    0 -> b[xs]             0072      0052
    0 - c - 1 -> c[m]      0346      1266
    0 - c - 1 -> c[ms]     0366      1276
    0 - c - 1 -> c[p]      0342      1242
    0 - c - 1 -> c[s]      0376      1262
    0 - c - 1 -> c[w]      0356      1272
    0 - c - 1 -> c[wp]     0362      1246
    0 - c - 1 -> c[x]      0352      1256
    0 - c - 1 -> c[xs]     0372      1252
    0 - c -> c[m]          0246      1226
    0 - c -> c[ms]         0266      1236
    0 - c -> c[p]          0242      1202
    0 - c -> c[s]          0276      1222
    0 - c -> c[w]          0256      1232
    0 - c -> c[wp]         0262      1206
    0 - c -> c[x]          0252      1216
    0 - c -> c[xs]         0272      1212
    0 -> c[m]              0306      0426
    0 -> c[ms]             0326      0436
    0 -> c[p]              0302      0402
    0 -> c[s]              0336      0422
    0 -> c[w]              0316      0432
    0 -> c[wp]             0322      0406
    0 -> c[x]              0312      0416
    0 -> c[xs]             0332      0412
    a - 1 -> a[m]          1546      1126
    a + 1 -> a[m]          1746      0666
    a - 1 -> a[ms]         1566      1136
    a + 1 -> a[ms]         1766      0676
    a - 1 -> a[p]          1542      1102
    a + 1 -> a[p]          1742      0642
    a - 1 -> a[s]          1576      1122
    a + 1 -> a[s]          1776      0662
    a - 1 -> a[w]          1556      1132
    a + 1 -> a[w]          1756      0672
    a - 1 -> a[wp]         1562      1106
    a + 1 -> a[wp]         1762      0646
    a - 1 -> a[x]          1552      1116
    a + 1 -> a[x]          1752      0656
    a - 1 -> a[xs]         1572      1112
    a + 1 -> a[xs]         1772      0652
    a - b -> a[m]          1406      1026
    a + b -> a[m]          1606      0466
    a - b -> a[ms]         1426      1036
    a + b -> a[ms]         1626      0476
    a - b -> a[p]          1402      1002
    a + b -> a[p]          1602      0442
    a - b -> a[s]          1436      1022
    a + b -> a[s]          1636      0462
    a - b -> a[w]          1416      1032
    a + b -> a[w]          1616      0472
    a - b -> a[wp]         1422      1006
    a + b -> a[wp]         1622      0446
    a - b -> a[x]          1412      1016
    a + b -> a[x]          1612      0456
    a - b -> a[xs]         1432      1012
    a + b -> a[xs]         1632      0452
    a -> b[m]              0446      0166
    a -> b[ms]             0466      0176
    a -> b[p]              0442      0142
    a -> b[s]              0476      0162
    a -> b[w]              0456      0172
    a -> b[wp]             0462      0146
    a -> b[x]              0452      0156
    a -> b[xs]             0472      0152
    a - c -> a[m]          1506      1626
    a + c -> a[m]          1706      0526
    a - c -> a[ms]         1526      1636
    a + c -> a[ms]         1726      0536
    a - c -> a[p]          1502      1602
    a + c -> a[p]          1702      0502
    a - c -> a[s]          1536      1622
    a + c -> a[s]          1736      0522
    a - c -> a[w]          1516      1632
    a + c -> a[w]          1716      0532
    a - c -> a[wp]         1522      1606
    a + c -> a[wp]         1722      0506
    a - c -> a[x]          1512      1616
    a + c -> a[x]          1712      0516
    a - c -> a[xs]         1532      1612
    a + c -> a[xs]         1732      0512
    a - c -> c[m]          0506      1066
    a + c -> c[m]          0706      0626
    a - c -> c[ms]         0526      1076
    a + c -> c[ms]         0726      0636
    a - c -> c[p]          0502      1042
    a + c -> c[p]          0702      0602
    a - c -> c[s]          0536      1062
    a + c -> c[s]          0736      0622
    a - c -> c[w]          0516      1072
    a + c -> c[w]          0716      0632
    a - c -> c[wp]         0522      1046
    a + c -> c[wp]         0722      0606
    a - c -> c[x]          0512      1056
    a + c -> c[x]          0712      0616
    a - c -> c[xs]         0532      1052
    a + c -> c[xs]         0732      0612
    a exch b[m]            1446      0126
    a exch b[ms]           1466      0136
    a exch b[p]            1442      0102
    a exch b[s]            1476      0122
    a exch b[w]            1456      0132
    a exch b[wp]           1462      0106
    a exch b[x]            1452      0116
    a exch b[xs]           1472      0112
    a exch c[m]            1646      0226
    a exch c[ms]           1666      0236
    a exch c[p]            1642      0202
    a exch c[s]            1676      0222
    a exch c[w]            1656      0232
    a exch c[wp]           1662      0206
    a exch c[x]            1652      0216
    a exch c[xs]           1672      0212
    b -> c[m]              0206      0326
    b -> c[ms]             0226      0336
    b -> c[p]              0202      0302
    b -> c[s]              0236      0322
    b -> c[w]              0216      0332
    b -> c[wp]             0222      0306
    b -> c[x]              0212      0316
    b -> c[xs]             0232      0312
    b exch c[m]            1046      0366
    b exch c[ms]           1066      0376
    b exch c[p]            1042      0342
    b exch c[s]            1076      0362
    b exch c[w]            1056      0372
    b exch c[wp]           1062      0346
    b exch c[x]            1052      0356
    b exch c[xs]           1072      0352
    c - 1 -> c[m]          0546      1166
    c + 1 -> c[m]          0746      0766
    c - 1 -> c[ms]         0566      1176
    c + 1 -> c[ms]         0766      0776
    c - 1 -> c[p]          0542      1142
    c + 1 -> c[p]          0742      0742
    c - 1 -> c[s]          0576      1162
    c + 1 -> c[s]          0776      0762
    c - 1 -> c[w]          0556      1172
    c + 1 -> c[w]          0756      0772
    c - 1 -> c[wp]         0562      1146
    c + 1 -> c[wp]         0762      0746
    c - 1 -> c[x]          0552      1156
    c + 1 -> c[x]          0752      0756
    c - 1 -> c[xs]         0572      1152
    c + 1 -> c[xs]         0772      0752
    c -> a[m]              0606      0266
    c -> a[ms]             0626      0276
    c -> a[p]              0602      0242
    c -> a[s]              0636      0262
    c -> a[w]              0616      0272
    c -> a[wp]             0622      0246
    c -> a[x]              0612      0256
    c -> a[xs]             0632      0252
    c + c -> c[m]          1246      0566
    c + c -> c[ms]         1266      0576
    c + c -> c[p]          1242      0542
    c + c -> c[s]          1276      0562
    c + c -> c[w]          1256      0572
    c + c -> c[wp]         1262      0546
    c + c -> c[x]          1252      0556
    c + c -> c[xs]         1272      0552
    if a >= b[m]           1006      1466
    if a >= b[ms]          1026      1476
    if a >= b[p]           1002      1442
    if a >= b[s]           1036      1462
    if a >= b[w]           1016      1472
    if a >= b[wp]          1022      1446
    if a >= b[x]           1012      1456
    if a >= b[xs]          1032      1452
    if a >= c[m]           0106      1426
    if a >= c[ms]          0126      1436
    if a >= c[p]           0102      1402
    if a >= c[s]           0136      1422
    if a >= c[w]           0116      1432
    if a >= c[wp]          0122      1406
    if a >= c[x]           0112      1416
    if a >= c[xs]          0132      1412
    if a[m] # 0            1146      1526
    if a[m] >= 1           1146      1526
    if a[ms] != 0          1166      1536
    if a[ms] >= 1          1166      1536
    if a[p] != 0           1142      1502
    if a[p] >= 1           1142      1502
    if a[s] != 0           1176      1522
    if a[s] >= 1           1176      1522
    if a[w] != 0           1156      1532
    if a[w] >= 1           1156      1532
    if a[wp] != 0          1162      1506
    if a[wp] >= 1          1162      1506
    if a[x] != 0           1152      1516
    if a[x] >= 1           1152      1516
    if a[xs] != 0          1172      1512
    if a[xs] >= 1          1172      1512
    if b[m] = 0            0006      1326
    if b[ms] = 0           0026      1336
    if b[p] = 0            0002      1302
    if b[s] = 0            0036      1322
    if b[w] = 0            0016      1332
    if b[wp] = 0           0022      1306
    if b[x] = 0            0012      1316
    if b[xs] = 0           0032      1312
    if c[m] != 0           0146      1566
    if c[m] = 0            0646      1366
    if c[m] >= 1           0146      1566
    if c[ms] != 0          0166      1576
    if c[ms] = 0           0666      1376
    if c[ms] >= 1          0166      1576
    if c[p] != 0           0142      1542
    if c[p] = 0            0642      1342
    if c[p] >= 1           0142      1542
    if c[s] != 0           0176      1562
    if c[s] = 0            0676      1362
    if c[s] >= 1           0176      1562
    if c[w] != 0           0156      1572
    if c[w] = 0            0656      1372
    if c[w] >= 1           0156      1572
    if c[wp] != 0          0162      1546
    if c[wp] = 0           0662      1346
    if c[wp] >= 1          0162      1546
    if c[x] != 0           0152      1556
    if c[x] = 0            0652      1356
    if c[x] >= 1           0152      1556
    if c[xs] != 0          0172      1552
    if c[xs] = 0           0672      1352
    if c[xs] >= 1          0172      1552
    shift left a[m]        0406      0726
    shift left a[ms]       0426      0736
    shift left a[p]        0402      0702
    shift left a[s]        0436      0722
    shift left a[w]        0416      0732
    shift left a[wp]       0422      0706
    shift left a[x]        0412      0716
    shift left a[xs]       0432      0712
    shift right a[m]       1306      1666
    shift right a[ms]      1326      1676
    shift right a[p]       1302      1642
    shift right a[s]       1336      1662
    shift right a[w]       1316      1672
    shift right a[wp]      1322      1646
    shift right a[x]       1312      1656
    shift right a[xs]      1332      1652
    shift right b[m]       1206      1726
    shift right b[ms]      1226      1736
    shift right b[p]       1202      1702
    shift right b[s]       1236      1722
    shift right b[w]       1216      1732
    shift right b[wp]      1222      1706
    shift right b[x]       1212      1716
    shift right b[xs]      1232      1712
    shift right c[m]       1106      1766
    shift right c[ms]      1126      1776
    shift right c[p]       1102      1742
    shift right c[s]       1136      1762
    shift right c[w]       1116      1772
    shift right c[wp]      1122      1746
    shift right c[x]       1112      1756
    shift right c[xs]      1132      1752

### Group 3 opcodes (n nnn nnn n11)

    Op Code               Classic   Woodstock
    go to 0000             0003      0003
    go to 0001             0007      0007
    go to 0002             0013      0013
    go to 0003             0017      0017
    go to 0004             0023      0023
    go to 0005             0027      0027
    go to 0006             0033      0033
    go to 0007             0037      0037
    go to 0010             0043      0043
    go to 0011             0047      0047
    go to 0012             0053      0053
    go to 0013             0057      0057
    go to 0014             0063      0063
    go to 0015             0067      0067
    go to 0016             0073      0073
    go to 0017             0077      0077
    go to 0020             0103      0103
    go to 0021             0107      0107
    go to 0022             0113      0113
    go to 0023             0117      0117
    go to 0024             0123      0123
    go to 0025             0127      0127
    go to 0026             0133      0133
    go to 0027             0137      0137
    go to 0030             0143      0143
    go to 0031             0147      0147
    go to 0032             0153      0153
    go to 0033             0157      0157
    go to 0034             0163      0163
    go to 0035             0167      0167
    go to 0036             0173      0173
    go to 0037             0177      0177
    go to 0040             0203      0203
    go to 0041             0207      0207
    go to 0042             0213      0213
    go to 0043             0217      0217
    go to 0044             0223      0223
    go to 0045             0227      0227
    go to 0046             0233      0233
    go to 0047             0237      0237
    go to 0050             0243      0243
    go to 0051             0247      0247
    go to 0052             0253      0253
    go to 0053             0257      0257
    go to 0054             0263      0263
    go to 0055             0267      0267
    go to 0056             0273      0273
    go to 0057             0277      0277
    go to 0060             0303      0303
    go to 0061             0307      0307
    go to 0062             0313      0313
    go to 0063             0317      0317
    go to 0064             0323      0323
    go to 0065             0327      0327
    go to 0066             0333      0333
    go to 0067             0337      0337
    go to 0070             0343      0343
    go to 0071             0347      0347
    go to 0072             0353      0353
    go to 0073             0357      0357
    go to 0074             0363      0363
    go to 0075             0367      0367
    go to 0076             0373      0373
    go to 0077             0377      0377
    go to 0100             0403      0403
    go to 0101             0407      0407
    go to 0102             0413      0413
    go to 0103             0417      0417
    go to 0104             0423      0423
    go to 0105             0427      0427
    go to 0106             0433      0433
    go to 0107             0437      0437
    go to 0110             0443      0443
    go to 0111             0447      0447
    go to 0112             0453      0453
    go to 0113             0457      0457
    go to 0114             0463      0463
    go to 0115             0467      0467
    go to 0116             0473      0473
    go to 0117             0477      0477
    go to 0120             0503      0503
    go to 1021             0507      0507
    go to 0122             0513      0513
    go to 0123             0517      0517
    go to 0124             0523      0523
    go to 0125             0527      0527
    go to 0126             0533      0533
    go to 0127             0537      0537
    go to 0130             0543      0543
    go to 0131             0547      0547
    go to 0132             0553      0553
    go to 0133             0557      0557
    go to 0134             0563      0563
    go to 0135             0567      0567
    go to 0136             0573      0573
    go to 0137             0577      0577
    go to 0140             0603      0603
    go to 0141             0607      0607
    go to 0142             0613      0613
    go to 0143             0617      0617
    go to 0144             0623      0623
    go to 1045             0627      0627
    go to 0146             0633      0633
    go to 0147             0637      0637
    go to 0150             0643      0643
    go to 0151             0647      0647
    go to 0152             0653      0653
    go to 0153             0657      0657
    go to 0154             0663      0663
    go to 0155             0667      0667
    go to 0156             0673      0673
    go to 0157             0677      0677
    go to 0160             0703      0703
    go to 0161             0707      0707
    go to 0162             0713      0713
    go to 0163             0717      0717
    go to 0164             0723      0723
    go to 0165             0727      0727
    go to 0166             0733      0733
    go to 0167             0737      0737
    go to 0170             0743      0743
    go to 0171             0747      0747
    go to 0172             0753      0753
    go to 0173             0757      0757
    go to 0174             0763      0763
    go to 0175             0767      0767
    go to 0176             0773      0773
    go to 0177             0777      0777
    go to 0200             1003      1003
    go to 0201             1007      1007
    go to 0202             1013      1013
    go to 0203             1017      1017
    go to 0204             1023      1023
    go to 0205             1027      1027
    go to 0206             1033      1033
    go to 0207             1037      1037
    go to 0210             1043      1043
    go to 0211             1047      1047
    go to 0212             1053      1053
    go to 0213             1057      1057
    go to 0214             1063      1063
    go to 0215             1067      1067
    go to 0216             1073      1073
    go to 0217             1077      1077
    go to 0220             1103      1103
    go to 0221             1107      1107
    go to 0222             1113      1113
    go to 0223             1117      1117
    go to 0224             1123      1123
    go to 0225             1127      1127
    go to 0226             1133      1133
    go to 0227             1137      1137
    go to 0230             1143      1143
    go to 0231             1147      1147
    go to 0232             1153      1153
    go to 0233             1157      1157
    go to 0234             1163      1163
    go to 0235             1167      1167
    go to 0236             1173      1173
    go to 0237             1177      1177
    go to 0240             1203      1203
    go to 0241             1207      1207
    go to 0242             1213      1213
    go to 0243             1217      1217
    go to 0244             1223      1223
    go to 0245             1227      1227
    go to 0246             1233      1233
    go to 0247             1237      1237
    go to 0250             1243      1243
    go to 0251             1247      1247
    go to 0252             1253      1253
    go to 0253             1257      1257
    go to 0254             1263      1263
    go to 0255             1267      1267
    go to 0256             1273      1273
    go to 0257             1277      1277
    go to 0260             1303      1303
    go to 0261             1307      1307
    go to 0262             1313      1313
    go to 0263             1317      1317
    go to 0264             1323      1323
    go to 0265             1327      1327
    go to 0266             1333      1333
    go to 0267             1337      1337
    go to 0270             1343      1343
    go to 0271             1347      1347
    go to 0272             1353      1353
    go to 0273             1357      1357
    go to 0274             1363      1363
    go to 0275             1367      1367
    go to 0276             1373      1373
    go to 0277             1377      1377
    go to 0300             1403      1403
    go to 0301             1407      1407
    go to 0302             1413      1413
    go to 0303             1417      1417
    go to 0304             1423      1423
    go to 0305             1427      1427
    go to 0306             1433      1433
    go to 0307             1437      1437
    go to 0310             1443      1443
    go to 0311             1447      1447
    go to 0312             1453      1453
    go to 0313             1457      1457
    go to 0314             1463      1463
    go to 0315             1467      1467
    go to 0316             1473      1473
    go to 0317             1477      1477
    go to 0320             1503      1503
    go to 0321             1507      1507
    go to 0322             1513      1513
    go to 0323             1517      1517
    go to 0324             1523      1523
    go to 0325             1527      1527
    go to 0326             1533      1533
    go to 0327             1537      1537
    go to 0330             1543      1543
    go to 0331             1547      1547
    go to 0332             1553      1553
    go to 0333             1557      1557
    go to 0334             1563      1563
    go to 0335             1567      1567
    go to 0336             1573      1573
    go to 0337             1577      1577
    go to 0340             1603      1603
    go to 0341             1607      1607
    go to 0342             1613      1613
    go to 0343             1617      1617
    go to 0344             1623      1623
    go to 0345             1627      1627
    go to 0346             1633      1633
    go to 0347             1637      1637
    go to 0350             1643      1643
    go to 0351             1647      1647
    go to 0352             1653      1653
    go to 0353             1657      1657
    go to 0354             1663      1663
    go to 0355             1667      1667
    go to 0356             1673      1673
    go to 0357             1677      1677
    go to 0360             1703      1703
    go to 0361             1707      1707
    go to 0362             1713      1713
    go to 0363             1717      1717
    go to 0364             1723      1723
    go to 0365             1727      1727
    go to 0366             1733      1733
    go to 0367             1737      1737
    go to 0370             1743      1743
    go to 0371             1747      1747
    go to 0372             1753      1753
    go to 0373             1757      1757
    go to 0374             1763      1763
    go to 0375             1767      1767
    go to 0376             1773      1773
    go to 0377             1777      1777
