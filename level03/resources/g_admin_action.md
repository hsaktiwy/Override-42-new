## 1. Objective

Identify the static memory offset (RVA) of the global structure member `g_admin.action` inside the stripped 64-bit ELF executable `virek` using static analysis with `objdump`.

---

## 2. Analysis & Execution Flow

Getting the Assembly:
```sh
level03@override:~$ objdump -d -M intel -j .text virek

virek:     file format elf64-x86-64


Disassembly of section .text:

0000000000001200 <.text>:
    1200:       f3 0f 1e fa             endbr64
    1204:       31 ed                   xor    ebp,ebp
    1206:       49 89 d1                mov    r9,rdx
    1209:       5e                      pop    rsi
    120a:       48 89 e2                mov    rdx,rsp
    120d:       48 83 e4 f0             and    rsp,0xfffffffffffffff0
    1211:       50                      push   rax
    1212:       54                      push   rsp
    1213:       45 31 c0                xor    r8d,r8d
    1216:       31 c9                   xor    ecx,ecx
    1218:       48 8d 3d ab 01 00 00    lea    rdi,[rip+0x1ab]        # 13ca <exit@plt+0x1da> [main function address ]
    121f:       ff 15 b3 2d 00 00       call   QWORD PTR [rip+0x2db3]        # 3fd8 <exit@plt+0x2de8> [__libc_start_main]
    1225:       f4                      hlt
    1226:       66 2e 0f 1f 84 00 00    cs nop WORD PTR [rax+rax*1+0x0]
    122d:       00 00 00 
    1230:       48 8d 3d d9 2d 00 00    lea    rdi,[rip+0x2dd9]        # 4010 <exit@plt+0x2e20>
    1237:       48 8d 05 d2 2d 00 00    lea    rax,[rip+0x2dd2]        # 4010 <exit@plt+0x2e20>
    123e:       48 39 f8                cmp    rax,rdi
    1241:       74 15                   je     1258 <exit@plt+0x68>
    1243:       48 8b 05 96 2d 00 00    mov    rax,QWORD PTR [rip+0x2d96]        # 3fe0 <exit@plt+0x2df0>
    124a:       48 85 c0                test   rax,rax
    124d:       74 09                   je     1258 <exit@plt+0x68>
    124f:       ff e0                   jmp    rax
    1251:       0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
    1258:       c3                      ret
    1259:       0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
    1260:       48 8d 3d a9 2d 00 00    lea    rdi,[rip+0x2da9]        # 4010 <exit@plt+0x2e20>
    1267:       48 8d 35 a2 2d 00 00    lea    rsi,[rip+0x2da2]        # 4010 <exit@plt+0x2e20>
    126e:       48 29 fe                sub    rsi,rdi
    1271:       48 89 f0                mov    rax,rsi
    1274:       48 c1 ee 3f             shr    rsi,0x3f
    1278:       48 c1 f8 03             sar    rax,0x3
    127c:       48 01 c6                add    rsi,rax
    127f:       48 d1 fe                sar    rsi,1
    1282:       74 14                   je     1298 <exit@plt+0xa8>
    1284:       48 8b 05 65 2d 00 00    mov    rax,QWORD PTR [rip+0x2d65]        # 3ff0 <exit@plt+0x2e00>
    128b:       48 85 c0                test   rax,rax
    128e:       74 08                   je     1298 <exit@plt+0xa8>
    1290:       ff e0                   jmp    rax
    1292:       66 0f 1f 44 00 00       nop    WORD PTR [rax+rax*1+0x0]
    1298:       c3                      ret
    1299:       0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
    12a0:       f3 0f 1e fa             endbr64
    12a4:       80 3d 7d 2d 00 00 00    cmp    BYTE PTR [rip+0x2d7d],0x0        # 4028 <stdout@GLIBC_2.2.5+0x8>
    12ab:       75 2b                   jne    12d8 <exit@plt+0xe8>
    12ad:       55                      push   rbp
    12ae:       48 83 3d 42 2d 00 00    cmp    QWORD PTR [rip+0x2d42],0x0        # 3ff8 <exit@plt+0x2e08>
    12b5:       00 
    12b6:       48 89 e5                mov    rbp,rsp
    12b9:       74 0c                   je     12c7 <exit@plt+0xd7>
    12bb:       48 8b 3d 46 2d 00 00    mov    rdi,QWORD PTR [rip+0x2d46]        # 4008 <exit@plt+0x2e18>
    12c2:       e8 49 fe ff ff          call   1110 <__cxa_finalize@plt>
    12c7:       e8 64 ff ff ff          call   1230 <exit@plt+0x40>
    12cc:       c6 05 55 2d 00 00 01    mov    BYTE PTR [rip+0x2d55],0x1        # 4028 <stdout@GLIBC_2.2.5+0x8>
    12d3:       5d                      pop    rbp
    12d4:       c3                      ret
    12d5:       0f 1f 00                nop    DWORD PTR [rax]
    12d8:       c3                      ret
    12d9:       0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
    12e0:       f3 0f 1e fa             endbr64
    12e4:       e9 77 ff ff ff          jmp    1260 <exit@plt+0x70>
    12e9:       f3 0f 1e fa             endbr64
    12ed:       48 83 ec 18             sub    rsp,0x18
    12f1:       64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
    12f8:       00 00 
    12fa:       48 89 44 24 08          mov    QWORD PTR [rsp+0x8],rax
    12ff:       31 c0                   xor    eax,eax
    1301:       48 8d 3d fc 0c 00 00    lea    rdi,[rip+0xcfc]        # 2004 <exit@plt+0xe14>
    1308:       e8 53 fe ff ff          call   1160 <system@plt>
    130d:       48 8b 44 24 08          mov    rax,QWORD PTR [rsp+0x8]
    1312:       64 48 2b 04 25 28 00    sub    rax,QWORD PTR fs:0x28
    1319:       00 00 
    131b:       75 05                   jne    1322 <exit@plt+0x132>
    131d:       48 83 c4 18             add    rsp,0x18
    1321:       c3                      ret
    1322:       e8 29 fe ff ff          call   1150 <__stack_chk_fail@plt>
    1327:       f3 0f 1e fa             endbr64
    132b:       48 83 ec 18             sub    rsp,0x18
    132f:       64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
    1336:       00 00 
    1338:       48 89 44 24 08          mov    QWORD PTR [rsp+0x8],rax
    133d:       31 c0                   xor    eax,eax
    133f:       48 8d 3d 3a 0d 00 00    lea    rdi,[rip+0xd3a]        # 2080 <exit@plt+0xe90>
    1346:       e8 e5 fd ff ff          call   1130 <puts@plt>
    134b:       48 8b 44 24 08          mov    rax,QWORD PTR [rsp+0x8]
    1350:       64 48 2b 04 25 28 00    sub    rax,QWORD PTR fs:0x28
    1357:       00 00 
    1359:       75 05                   jne    1360 <exit@plt+0x170>
    135b:       48 83 c4 18             add    rsp,0x18
    135f:       c3                      ret
    1360:       e8 eb fd ff ff          call   1150 <__stack_chk_fail@plt>
    1365:       48 83 ec 38             sub    rsp,0x38
    1369:       64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
    1370:       00 00 
    1372:       48 89 44 24 28          mov    QWORD PTR [rsp+0x28],rax
    1377:       31 c0                   xor    eax,eax
    1379:       48 89 e6                mov    rsi,rsp
    137c:       ba 1f 00 00 00          mov    edx,0x1f
    1381:       bf 00 00 00 00          mov    edi,0x0
    1386:       e8 05 fe ff ff          call   1190 <read@plt>
    138b:       48 85 c0                test   rax,rax
    138e:       7e 2b                   jle    13bb <exit@plt+0x1cb>
    1390:       c6 04 04 00             mov    BYTE PTR [rsp+rax*1],0x0
    1394:       48 89 e7                mov    rdi,rsp
    1397:       ba 00 00 00 00          mov    edx,0x0
    139c:       be 00 00 00 00          mov    esi,0x0
    13a1:       e8 fa fd ff ff          call   11a0 <strtol@plt>
    13a6:       48 8b 54 24 28          mov    rdx,QWORD PTR [rsp+0x28]
    13ab:       64 48 2b 14 25 28 00    sub    rdx,QWORD PTR fs:0x28
    13b2:       00 00 
    13b4:       75 0f                   jne    13c5 <exit@plt+0x1d5>
    13b6:       48 83 c4 38             add    rsp,0x38
    13ba:       c3                      ret
    13bb:       bf 00 00 00 00          mov    edi,0x0
    13c0:       e8 2b fe ff ff          call   11f0 <exit@plt>
    13c5:       e8 86 fd ff ff          call   1150 <__stack_chk_fail@plt>
    13ca:       f3 0f 1e fa             endbr64
    13ce:       41 55                   push   r13
    13d0:       41 54                   push   r12
    13d2:       55                      push   rbp
    13d3:       53                      push   rbx
    13d4:       48 83 ec 18             sub    rsp,0x18
    13d8:       64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
    13df:       00 00 
    13e1:       48 89 44 24 08          mov    QWORD PTR [rsp+0x8],rax
    13e6:       31 c0                   xor    eax,eax
    13e8:       e8 93 fd ff ff          call   1180 <geteuid@plt>
    13ed:       89 c3                   mov    ebx,eax
    13ef:       e8 8c fd ff ff          call   1180 <geteuid@plt>
    13f4:       89 c7                   mov    edi,eax
    13f6:       89 de                   mov    esi,ebx
    13f8:       e8 d3 fd ff ff          call   11d0 <setreuid@plt>
    13fd:       b9 00 00 00 00          mov    ecx,0x0
    1402:       ba 02 00 00 00          mov    edx,0x2
    1407:       be 00 00 00 00          mov    esi,0x0
    140c:       48 8b 3d 0d 2c 00 00    mov    rdi,QWORD PTR [rip+0x2c0d]        # 4020 <stdout@GLIBC_2.2.5>
    1413:       e8 c8 fd ff ff          call   11e0 <setvbuf@plt>
    1418:       48 8d 05 08 ff ff ff    lea    rax,[rip+0xffffffffffffff08]        # 1327 <exit@plt+0x137> 
    141f:       48 89 05 32 2c 00 00    mov    QWORD PTR [rip+0x2c32],rax        # 4058 <stdout@GLIBC_2.2.5+0x38>
    1426:       48 b8 54 45 53 53 49    movabs rax,0x2d52454953534554 # "TESSIER-"
    142d:       45 52 2d 
    1430:       48 ba 41 53 48 50 4f    movabs rdx,0x4c4f4f50485341 # "ASHPOOL\0"
    1437:       4f 4c 00 
    143a:       48 89 05 ff 2b 00 00    mov    QWORD PTR [rip+0x2bff],rax        # 4040 <stdout@GLIBC_2.2.5+0x20>
    1441:       48 89 15 00 2c 00 00    mov    QWORD PTR [rip+0x2c00],rdx        # 4048 <stdout@GLIBC_2.2.5+0x28>
    1448:       48 8d 3d bd 0b 00 00    lea    rdi,[rip+0xbbd]        # 200c <exit@plt+0xe1c>
    144f:       b8 00 00 00 00          mov    eax,0x0
    1454:       e8 17 fd ff ff          call   1170 <printf@plt>
    1459:       48 8d 3d 48 0c 00 00    lea    rdi,[rip+0xc48]        # 20a8 <exit@plt+0xeb8>
    1460:       e8 cb fc ff ff          call   1130 <puts@plt>
    1465:       48 8d 3d 84 0c 00 00    lea    rdi,[rip+0xc84]        # 20f0 <exit@plt+0xf00>
    146c:       e8 bf fc ff ff          call   1130 <puts@plt>
    1471:       48 8d 3d 9c 0b 00 00    lea    rdi,[rip+0xb9c]        # 2014 <exit@plt+0xe24>
    1478:       b8 00 00 00 00          mov    eax,0x0
    147d:       e8 ee fc ff ff          call   1170 <printf@plt>
    1482:       48 8d 35 60 fe ff ff    lea    rsi,[rip+0xfffffffffffffe60]        # 12e9 <exit@plt+0xf9>
    1489:       48 8d 3d 89 0b 00 00    lea    rdi,[rip+0xb89]        # 2019 <exit@plt+0xe29>
    1490:       b8 00 00 00 00          mov    eax,0x0
    1495:       e8 d6 fc ff ff          call   1170 <printf@plt>
    149a:       48 8d 2d 77 0c 00 00    lea    rbp,[rip+0xc77]        # 2118 <exit@plt+0xf28>
    14a1:       48 8d 1d d0 0c 00 00    lea    rbx,[rip+0xcd0]        # 2178 <exit@plt+0xf88>
    14a8:       48 89 ef                mov    rdi,rbp
    14ab:       b8 00 00 00 00          mov    eax,0x0
    14b0:       e8 bb fc ff ff          call   1170 <printf@plt>
    14b5:       48 8b 3d 64 2b 00 00    mov    rdi,QWORD PTR [rip+0x2b64]        # 4020 <stdout@GLIBC_2.2.5>
    14bc:       e8 ff fc ff ff          call   11c0 <fflush@plt>
    14c1:       e8 9f fe ff ff          call   1365 <exit@plt+0x175>
    14c6:       48 83 f8 05             cmp    rax,0x5
    14ca:       0f 87 39 02 00 00       ja     1709 <exit@plt+0x519>
    14d0:       48 63 04 83             movsxd rax,DWORD PTR [rbx+rax*4]
    14d4:       48 01 d8                add    rax,rbx
    14d7:       3e ff e0                notrack jmp rax
    14da:       48 8d 3d 4b 0b 00 00    lea    rdi,[rip+0xb4b]        # 202c <exit@plt+0xe3c>
    14e1:       b8 00 00 00 00          mov    eax,0x0
    14e6:       e8 85 fc ff ff          call   1170 <printf@plt>
    14eb:       48 8b 3d 2e 2b 00 00    mov    rdi,QWORD PTR [rip+0x2b2e]        # 4020 <stdout@GLIBC_2.2.5>
    14f2:       e8 c9 fc ff ff          call   11c0 <fflush@plt>
    14f7:       e8 69 fe ff ff          call   1365 <exit@plt+0x175>
    14fc:       49 89 c5                mov    r13,rax
    14ff:       48 8d 40 ff             lea    rax,[rax-0x1]
    1503:       48 3d ff 07 00 00       cmp    rax,0x7ff
    1509:       77 69                   ja     1574 <exit@plt+0x384>
    150b:       48 8d 3d 33 0b 00 00    lea    rdi,[rip+0xb33]        # 2045 <exit@plt+0xe55>
    1512:       b8 00 00 00 00          mov    eax,0x0
    1517:       e8 54 fc ff ff          call   1170 <printf@plt>
    151c:       48 8b 3d fd 2a 00 00    mov    rdi,QWORD PTR [rip+0x2afd]        # 4020 <stdout@GLIBC_2.2.5>
    1523:       e8 98 fc ff ff          call   11c0 <fflush@plt>
    1528:       e8 38 fe ff ff          call   1365 <exit@plt+0x175>
    152d:       49 89 c4                mov    r12,rax
    1530:       48 83 f8 0f             cmp    rax,0xf
    1534:       0f 87 6e ff ff ff       ja     14a8 <exit@plt+0x2b8>
    153a:       4c 89 ef                mov    rdi,r13
    153d:       e8 6e fc ff ff          call   11b0 <malloc@plt>
    1542:       48 89 c2                mov    rdx,rax
    1545:       48 8d 05 94 2b 00 00    lea    rax,[rip+0x2b94]        # 40e0 <stdout@GLIBC_2.2.5+0xc0>
    154c:       4a 89 14 e0             mov    QWORD PTR [rax+r12*8],rdx
    1550:       48 8d 05 09 2b 00 00    lea    rax,[rip+0x2b09]        # 4060 <stdout@GLIBC_2.2.5+0x40>
    1557:       4e 89 2c e0             mov    QWORD PTR [rax+r12*8],r13
    155b:       4c 89 e6                mov    rsi,r12
    155e:       48 8d 3d eb 0b 00 00    lea    rdi,[rip+0xbeb]        # 2150 <exit@plt+0xf60>
    1565:       b8 00 00 00 00          mov    eax,0x0
    156a:       e8 01 fc ff ff          call   1170 <printf@plt>
    156f:       e9 34 ff ff ff          jmp    14a8 <exit@plt+0x2b8>
    1574:       48 8d 3d b8 0a 00 00    lea    rdi,[rip+0xab8]        # 2033 <exit@plt+0xe43>
    157b:       e8 b0 fb ff ff          call   1130 <puts@plt>
    1580:       e9 23 ff ff ff          jmp    14a8 <exit@plt+0x2b8>
    1585:       48 8d 3d b9 0a 00 00    lea    rdi,[rip+0xab9]        # 2045 <exit@plt+0xe55>
    158c:       b8 00 00 00 00          mov    eax,0x0
    1591:       e8 da fb ff ff          call   1170 <printf@plt>
    1596:       48 8b 3d 83 2a 00 00    mov    rdi,QWORD PTR [rip+0x2a83]        # 4020 <stdout@GLIBC_2.2.5>
    159d:       e8 1e fc ff ff          call   11c0 <fflush@plt>
    15a2:       e8 be fd ff ff          call   1365 <exit@plt+0x175>
    15a7:       48 83 f8 0f             cmp    rax,0xf
    15ab:       0f 87 f7 fe ff ff       ja     14a8 <exit@plt+0x2b8>
    15b1:       48 8d 15 28 2b 00 00    lea    rdx,[rip+0x2b28]        # 40e0 <stdout@GLIBC_2.2.5+0xc0>
    15b8:       48 8b 3c c2             mov    rdi,QWORD PTR [rdx+rax*8]
    15bc:       48 85 ff                test   rdi,rdi
    15bf:       0f 84 e3 fe ff ff       je     14a8 <exit@plt+0x2b8>
    15c5:       e8 56 fb ff ff          call   1120 <free@plt>
    15ca:       48 8d 3d 7b 0a 00 00    lea    rdi,[rip+0xa7b]        # 204c <exit@plt+0xe5c>
    15d1:       e8 5a fb ff ff          call   1130 <puts@plt>
    15d6:       e9 cd fe ff ff          jmp    14a8 <exit@plt+0x2b8>
    15db:       48 8d 3d 63 0a 00 00    lea    rdi,[rip+0xa63]        # 2045 <exit@plt+0xe55>
    15e2:       b8 00 00 00 00          mov    eax,0x0
    15e7:       e8 84 fb ff ff          call   1170 <printf@plt>
    15ec:       48 8b 3d 2d 2a 00 00    mov    rdi,QWORD PTR [rip+0x2a2d]        # 4020 <stdout@GLIBC_2.2.5>
    15f3:       e8 c8 fb ff ff          call   11c0 <fflush@plt>
    15f8:       e8 68 fd ff ff          call   1365 <exit@plt+0x175>
    15fd:       49 89 c4                mov    r12,rax
    1600:       48 83 f8 0f             cmp    rax,0xf
    1604:       0f 87 9e fe ff ff       ja     14a8 <exit@plt+0x2b8>
    160a:       48 8d 05 cf 2a 00 00    lea    rax,[rip+0x2acf]        # 40e0 <stdout@GLIBC_2.2.5+0xc0>
    1611:       4a 83 3c e0 00          cmp    QWORD PTR [rax+r12*8],0x0
    1616:       0f 84 8c fe ff ff       je     14a8 <exit@plt+0x2b8>
    161c:       48 8d 3d 40 0a 00 00    lea    rdi,[rip+0xa40]        # 2063 <exit@plt+0xe73>
    1623:       b8 00 00 00 00          mov    eax,0x0
    1628:       e8 43 fb ff ff          call   1170 <printf@plt>
    162d:       48 8b 3d ec 29 00 00    mov    rdi,QWORD PTR [rip+0x29ec]        # 4020 <stdout@GLIBC_2.2.5>
    1634:       e8 87 fb ff ff          call   11c0 <fflush@plt>
    1639:       e8 27 fd ff ff          call   1365 <exit@plt+0x175>
    163e:       49 89 c5                mov    r13,rax
    1641:       48 8d 3d 2e 0a 00 00    lea    rdi,[rip+0xa2e]        # 2076 <exit@plt+0xe86>
    1648:       b8 00 00 00 00          mov    eax,0x0
    164d:       e8 1e fb ff ff          call   1170 <printf@plt>
    1652:       48 8b 3d c7 29 00 00    mov    rdi,QWORD PTR [rip+0x29c7]        # 4020 <stdout@GLIBC_2.2.5>
    1659:       e8 62 fb ff ff          call   11c0 <fflush@plt>
    165e:       48 8d 05 7b 2a 00 00    lea    rax,[rip+0x2a7b]        # 40e0 <stdout@GLIBC_2.2.5+0xc0>
    1665:       4a 8b 34 e0             mov    rsi,QWORD PTR [rax+r12*8]
    1669:       4c 89 ea                mov    rdx,r13
    166c:       bf 00 00 00 00          mov    edi,0x0
    1671:       e8 1a fb ff ff          call   1190 <read@plt>
    1676:       e9 2d fe ff ff          jmp    14a8 <exit@plt+0x2b8>
    167b:       48 8d 3d c3 09 00 00    lea    rdi,[rip+0x9c3]        # 2045 <exit@plt+0xe55>
    1682:       b8 00 00 00 00          mov    eax,0x0
    1687:       e8 e4 fa ff ff          call   1170 <printf@plt>
    168c:       48 8b 3d 8d 29 00 00    mov    rdi,QWORD PTR [rip+0x298d]        # 4020 <stdout@GLIBC_2.2.5>
    1693:       e8 28 fb ff ff          call   11c0 <fflush@plt>
    1698:       e8 c8 fc ff ff          call   1365 <exit@plt+0x175>
    169d:       49 89 c4                mov    r12,rax
    16a0:       48 83 f8 0f             cmp    rax,0xf
    16a4:       0f 87 fe fd ff ff       ja     14a8 <exit@plt+0x2b8>
    16aa:       48 8d 05 2f 2a 00 00    lea    rax,[rip+0x2a2f]        # 40e0 <stdout@GLIBC_2.2.5+0xc0>
    16b1:       4a 83 3c e0 00          cmp    QWORD PTR [rax+r12*8],0x0
    16b6:       0f 84 ec fd ff ff       je     14a8 <exit@plt+0x2b8>
    16bc:       48 8d 3d a6 09 00 00    lea    rdi,[rip+0x9a6]        # 2069 <exit@plt+0xe79>
    16c3:       b8 00 00 00 00          mov    eax,0x0
    16c8:       e8 a3 fa ff ff          call   1170 <printf@plt>
    16cd:       48 8d 05 8c 29 00 00    lea    rax,[rip+0x298c]        # 4060 <stdout@GLIBC_2.2.5+0x40>
    16d4:       4a 8b 14 e0             mov    rdx,QWORD PTR [rax+r12*8]
    16d8:       48 8d 05 01 2a 00 00    lea    rax,[rip+0x2a01]        # 40e0 <stdout@GLIBC_2.2.5+0xc0>
    16df:       4a 8b 34 e0             mov    rsi,QWORD PTR [rax+r12*8]
    16e3:       bf 01 00 00 00          mov    edi,0x1
    16e8:       e8 53 fa ff ff          call   1140 <write@plt>
    16ed:       48 8d 3d 37 09 00 00    lea    rdi,[rip+0x937]        # 202b <exit@plt+0xe3b>
    16f4:       e8 37 fa ff ff          call   1130 <puts@plt>
    16f9:       e9 aa fd ff ff          jmp    14a8 <exit@plt+0x2b8>
    16fe:       ff 15 54 29 00 00       call   QWORD PTR [rip+0x2954]        # 4058 <stdout@GLIBC_2.2.5+0x38>
    1704:       e9 9f fd ff ff          jmp    14a8 <exit@plt+0x2b8>
    1709:       48 8b 44 24 08          mov    rax,QWORD PTR [rsp+0x8]
    170e:       64 48 2b 04 25 28 00    sub    rax,QWORD PTR fs:0x28
    1715:       00 00 
    1717:       75 10                   jne    1729 <exit@plt+0x539>
    1719:       b8 00 00 00 00          mov    eax,0x0
    171e:       48 83 c4 18             add    rsp,0x18
    1722:       5b                      pop    rbx
    1723:       5d                      pop    rbp
    1724:       41 5c                   pop    r12
    1726:       41 5d                   pop    r13
    1728:       c3                      ret
    1729:       e8 22 fa ff ff          call   1150 <__stack_chk_fail@plt>
```
### Step 1: Locating `main` via `_start`

In x86_64 ELF executables, execution begins at `_start` (`0x1200`). The System V AMD64 ABI passes the entry point of `main()` to `__libc_start_main` as its first parameter in the `rdi` register.

```assembly
0000000000001200 <.text>:
    1200:   f3 0f 1e fa             endbr64
    ...
    1218:   48 8d 3d ab 01 00 00    lea    rdi,[rip+0x1ab]        # 0x13ca
    121f:   ff 15 b3 2d 00 00       call   QWORD PTR [rip+0x2db3] # __libc_start_main@plt

```

**Calculation:**


$$\text{Target Address} = \text{Next RIP} + \text{Offset}$$

$$0x121f + 0x01ab = 0x13ca$$

* **Finding:** `main()` starts at offset **`0x13ca`**.

---

### Step 2: Tracing `g_admin.action` Assignment in `main`

According to the C source snippet, `main()` initializes the global struct:

```c
g_admin.action = default_action;

```

Examining disassembly starting at `main` (`0x13ca`):

```assembly
    13f8:   e8 d3 fd ff ff          call   11d0 <setreuid@plt>
    ...
    1413:   e8 c8 fd ff ff          call   11e0 <setvbuf@plt>
    1418:   48 8d 05 08 ff ff ff    lea    rax,[rip-0xf8]       # Load default_action (0x1327)
    141f:   48 89 05 32 2c 00 00    mov    QWORD PTR [rip+0x2c32],rax # g_admin.action

```

**Calculation:**


$$\text{Address of } \text{g\_admin.action} = \text{Next RIP} + \text{Offset}$$

$$0x1426 + 0x2c32 = 0x4058$$

* **Finding:** `default_action` is located at **`0x1327`** (`0x141f - 0xf8`).
* **Finding:** `g_admin.action` is located at offset **`0x4058`**.

---

### Step 3: Cross-Verification

#### A. Struct Layout via `strcpy(g_admin.tag, "TESSIER-ASHPOOL")`

Immediately following the function pointer assignment, two 64-bit quadwords representing the string `"TESSIER-ASHPOOL\0"` are copied into memory:

```assembly
    1426:   48 b8 54 45 53 53 49 45 52 2d  movabs rax,0x2d52454953534554 # "TESSIER-"
    143a:   48 89 05 ff 2b 00 00          mov    QWORD PTR [rip+0x2bff],rax # 0x1441 + 0x2bff = 0x4040
    1441:   48 89 15 00 2c 00 00          mov    QWORD PTR [rip+0x2c00],rdx # 0x1448 + 0x2c00 = 0x4048

```

* `g_admin.tag` starts at **`0x4040`**.
* `g_admin.action` lies at **`0x4058`** (Offset: $+0x18$ bytes relative to `g_admin.tag`).

#### B. Execution Call Site

In the dispatch/menu loop, choice `5` (`node_sync()`) triggers an indirect call:

```assembly
    16fe:   ff 15 54 29 00 00       call   QWORD PTR [rip+0x2954] # 0x1704 + 0x2954 = 0x4058

```

Dereferencing `0x4058` fetches the target pointer (`g_admin.action`) for execution.

---

## 3. Summary of Offsets

| Symbol / Field | Offset (RVA) | Description |
| --- | --- | --- |
| `_start` | `0x1200` | ELF Entry Point |
| `default_action` | `0x1327` | Default function assigned to struct |
| `main` | `0x13ca` | Main routine |
| `g_admin.tag` | `0x4040` | Tag buffer within `g_admin` struct |
| **`g_admin.action`** | **`0x4058`** | **Target function pointer location** |

---