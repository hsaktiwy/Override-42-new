level09@override:~$ objdump -d -M intel -j .text ./a.out

./a.out:     file format elf64-x86-64


Disassembly of section .text:

0000000000001220 <_start>:
    1220:       f3 0f 1e fa             endbr64
    1224:       31 ed                   xor    ebp,ebp
    1226:       49 89 d1                mov    r9,rdx
    1229:       5e                      pop    rsi
    122a:       48 89 e2                mov    rdx,rsp
    122d:       48 83 e4 f0             and    rsp,0xfffffffffffffff0
    1231:       50                      push   rax
    1232:       54                      push   rsp
    1233:       45 31 c0                xor    r8d,r8d
    1236:       31 c9                   xor    ecx,ecx
    1238:       48 8d 3d 30 05 00 00    lea    rdi,[rip+0x530]        # 176f <main>
    123f:       ff 15 93 2d 00 00       call   QWORD PTR [rip+0x2d93]        # 3fd8 <__libc_start_main@GLIBC_2.34>
    1245:       f4                      hlt
    1246:       66 2e 0f 1f 84 00 00    cs nop WORD PTR [rax+rax*1+0x0]
    124d:       00 00 00 

0000000000001250 <deregister_tm_clones>:
    1250:       48 8d 3d b9 2d 00 00    lea    rdi,[rip+0x2db9]        # 4010 <__TMC_END__>
    1257:       48 8d 05 b2 2d 00 00    lea    rax,[rip+0x2db2]        # 4010 <__TMC_END__>
    125e:       48 39 f8                cmp    rax,rdi
    1261:       74 15                   je     1278 <deregister_tm_clones+0x28>
    1263:       48 8b 05 76 2d 00 00    mov    rax,QWORD PTR [rip+0x2d76]        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    126a:       48 85 c0                test   rax,rax
    126d:       74 09                   je     1278 <deregister_tm_clones+0x28>
    126f:       ff e0                   jmp    rax
    1271:       0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
    1278:       c3                      ret
    1279:       0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]

0000000000001280 <register_tm_clones>:
    1280:       48 8d 3d 89 2d 00 00    lea    rdi,[rip+0x2d89]        # 4010 <__TMC_END__>
    1287:       48 8d 35 82 2d 00 00    lea    rsi,[rip+0x2d82]        # 4010 <__TMC_END__>
    128e:       48 29 fe                sub    rsi,rdi
    1291:       48 89 f0                mov    rax,rsi
    1294:       48 c1 ee 3f             shr    rsi,0x3f
    1298:       48 c1 f8 03             sar    rax,0x3
    129c:       48 01 c6                add    rsi,rax
    129f:       48 d1 fe                sar    rsi,1
    12a2:       74 14                   je     12b8 <register_tm_clones+0x38>
    12a4:       48 8b 05 45 2d 00 00    mov    rax,QWORD PTR [rip+0x2d45]        # 3ff0 <_ITM_registerTMCloneTable@Base>
    12ab:       48 85 c0                test   rax,rax
    12ae:       74 08                   je     12b8 <register_tm_clones+0x38>
    12b0:       ff e0                   jmp    rax
    12b2:       66 0f 1f 44 00 00       nop    WORD PTR [rax+rax*1+0x0]
    12b8:       c3                      ret
    12b9:       0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]

00000000000012c0 <__do_global_dtors_aux>:
    12c0:       f3 0f 1e fa             endbr64
    12c4:       80 3d 5d 2d 00 00 00    cmp    BYTE PTR [rip+0x2d5d],0x0        # 4028 <completed.0>
    12cb:       75 2b                   jne    12f8 <__do_global_dtors_aux+0x38>
    12cd:       55                      push   rbp
    12ce:       48 83 3d 22 2d 00 00    cmp    QWORD PTR [rip+0x2d22],0x0        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    12d5:       00 
    12d6:       48 89 e5                mov    rbp,rsp
    12d9:       74 0c                   je     12e7 <__do_global_dtors_aux+0x27>
    12db:       48 8b 3d 26 2d 00 00    mov    rdi,QWORD PTR [rip+0x2d26]        # 4008 <__dso_handle>
    12e2:       e8 39 fe ff ff          call   1120 <__cxa_finalize@plt>
    12e7:       e8 64 ff ff ff          call   1250 <deregister_tm_clones>
    12ec:       c6 05 35 2d 00 00 01    mov    BYTE PTR [rip+0x2d35],0x1        # 4028 <completed.0>
    12f3:       5d                      pop    rbp
    12f4:       c3                      ret
    12f5:       0f 1f 00                nop    DWORD PTR [rax]
    12f8:       c3                      ret
    12f9:       0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]

0000000000001300 <frame_dummy>:
    1300:       f3 0f 1e fa             endbr64
    1304:       e9 77 ff ff ff          jmp    1280 <register_tm_clones>

0000000000001309 <win>:
    1309:       f3 0f 1e fa             endbr64
    130d:       55                      push   rbp
    130e:       48 89 e5                mov    rbp,rsp
    1311:       48 8d 05 f0 0c 00 00    lea    rax,[rip+0xcf0]        # 2008 <_IO_stdin_used+0x8>
    1318:       48 89 c7                mov    rdi,rax
    131b:       e8 50 fe ff ff          call   1170 <system@plt>
    1320:       90                      nop
    1321:       5d                      pop    rbp
    1322:       c3                      ret

0000000000001323 <default_finalize>:
    1323:       f3 0f 1e fa             endbr64
    1327:       55                      push   rbp
    1328:       48 89 e5                mov    rbp,rsp
    132b:       48 83 ec 10             sub    rsp,0x10
    132f:       48 89 7d f8             mov    QWORD PTR [rbp-0x8],rdi
    1333:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    1337:       48 8b 40 08             mov    rax,QWORD PTR [rax+0x8]
    133b:       48 89 c6                mov    rsi,rax
    133e:       48 8d 05 cb 0c 00 00    lea    rax,[rip+0xccb]        # 2010 <_IO_stdin_used+0x10>
    1345:       48 89 c7                mov    rdi,rax
    1348:       b8 00 00 00 00          mov    eax,0x0
    134d:       e8 2e fe ff ff          call   1180 <printf@plt>
    1352:       90                      nop
    1353:       c9                      leave
    1354:       c3                      ret

0000000000001355 <default_commit>:
    1355:       f3 0f 1e fa             endbr64
    1359:       55                      push   rbp
    135a:       48 89 e5                mov    rbp,rsp
    135d:       48 8d 05 cc 0c 00 00    lea    rax,[rip+0xccc]        # 2030 <_IO_stdin_used+0x30>
    1364:       48 89 c7                mov    rdi,rax
    1367:       e8 d4 fd ff ff          call   1140 <puts@plt>
    136c:       90                      nop
    136d:       5d                      pop    rbp
    136e:       c3                      ret

000000000000136f <rd_long>:
    136f:       f3 0f 1e fa             endbr64
    1373:       55                      push   rbp
    1374:       48 89 e5                mov    rbp,rsp
    1377:       48 83 ec 40             sub    rsp,0x40
    137b:       64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
    1382:       00 00 
    1384:       48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
    1388:       31 c0                   xor    eax,eax
    138a:       48 8d 45 d0             lea    rax,[rbp-0x30]
    138e:       ba 1f 00 00 00          mov    edx,0x1f
    1393:       48 89 c6                mov    rsi,rax
    1396:       bf 00 00 00 00          mov    edi,0x0
    139b:       e8 10 fe ff ff          call   11b0 <read@plt>
    13a0:       48 89 45 c8             mov    QWORD PTR [rbp-0x38],rax
    13a4:       48 83 7d c8 00          cmp    QWORD PTR [rbp-0x38],0x0
    13a9:       7f 0a                   jg     13b5 <rd_long+0x46>
    13ab:       bf 00 00 00 00          mov    edi,0x0
    13b0:       e8 5b fe ff ff          call   1210 <exit@plt>
    13b5:       48 8d 55 d0             lea    rdx,[rbp-0x30]
    13b9:       48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
    13bd:       48 01 d0                add    rax,rdx
    13c0:       c6 00 00                mov    BYTE PTR [rax],0x0
    13c3:       48 8d 45 d0             lea    rax,[rbp-0x30]
    13c7:       ba 00 00 00 00          mov    edx,0x0
    13cc:       be 00 00 00 00          mov    esi,0x0
    13d1:       48 89 c7                mov    rdi,rax
    13d4:       e8 e7 fd ff ff          call   11c0 <strtol@plt>
    13d9:       48 8b 55 f8             mov    rdx,QWORD PTR [rbp-0x8]
    13dd:       64 48 2b 14 25 28 00    sub    rdx,QWORD PTR fs:0x28
    13e4:       00 00 
    13e6:       74 05                   je     13ed <rd_long+0x7e>
    13e8:       e8 73 fd ff ff          call   1160 <__stack_chk_fail@plt>
    13ed:       c9                      leave
    13ee:       c3                      ret

00000000000013ef <obj_new>:
    13ef:       f3 0f 1e fa             endbr64
    13f3:       55                      push   rbp
    13f4:       48 89 e5                mov    rbp,rsp
    13f7:       48 83 ec 10             sub    rsp,0x10
    13fb:       48 8d 05 51 0c 00 00    lea    rax,[rip+0xc51]        # 2053 <_IO_stdin_used+0x53>
    1402:       48 89 c7                mov    rdi,rax
    1405:       b8 00 00 00 00          mov    eax,0x0
    140a:       e8 71 fd ff ff          call   1180 <printf@plt>
    140f:       48 8b 05 0a 2c 00 00    mov    rax,QWORD PTR [rip+0x2c0a]        # 4020 <stdout@GLIBC_2.2.5>
    1416:       48 89 c7                mov    rdi,rax
    1419:       e8 c2 fd ff ff          call   11e0 <fflush@plt>
    141e:       e8 4c ff ff ff          call   136f <rd_long>
    1423:       48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
    1427:       48 83 7d f8 00          cmp    QWORD PTR [rbp-0x8],0x0
    142c:       0f 88 f4 00 00 00       js     1526 <obj_new+0x137>
    1432:       48 83 7d f8 0f          cmp    QWORD PTR [rbp-0x8],0xf
    1437:       0f 8f e9 00 00 00       jg     1526 <obj_new+0x137>
    143d:       bf 30 00 00 00          mov    edi,0x30
    1442:       e8 89 fd ff ff          call   11d0 <malloc@plt>
    1447:       48 89 c1                mov    rcx,rax
    144a:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    144e:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    1455:       00 
    1456:       48 8d 05 e3 2b 00 00    lea    rax,[rip+0x2be3]        # 4040 <heap>
    145d:       48 89 0c 02             mov    QWORD PTR [rdx+rax*1],rcx
    1461:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    1465:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    146c:       00 
    146d:       48 8d 05 cc 2b 00 00    lea    rax,[rip+0x2bcc]        # 4040 <heap>
    1474:       48 8b 04 02             mov    rax,QWORD PTR [rdx+rax*1]
    1478:       48 89 c6                mov    rsi,rax
    147b:       48 8d 05 d8 0b 00 00    lea    rax,[rip+0xbd8]        # 205a <_IO_stdin_used+0x5a>
    1482:       48 89 c7                mov    rdi,rax
    1485:       b8 00 00 00 00          mov    eax,0x0
    148a:       e8 f1 fc ff ff          call   1180 <printf@plt>
    148f:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    1493:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    149a:       00 
    149b:       48 8d 05 9e 2b 00 00    lea    rax,[rip+0x2b9e]        # 4040 <heap>
    14a2:       48 8b 04 02             mov    rax,QWORD PTR [rdx+rax*1]
    14a6:       48 8d 15 76 fe ff ff    lea    rdx,[rip+0xfffffffffffffe76]        # 1323 <default_finalize>
    14ad:       48 89 10                mov    QWORD PTR [rax],rdx
    14b0:       8b 05 0a 2c 00 00       mov    eax,DWORD PTR [rip+0x2c0a]        # 40c0 <obj_seq>
    14b6:       8d 50 01                lea    edx,[rax+0x1]
    14b9:       89 15 01 2c 00 00       mov    DWORD PTR [rip+0x2c01],edx        # 40c0 <obj_seq>
    14bf:       48 8b 55 f8             mov    rdx,QWORD PTR [rbp-0x8]
    14c3:       48 8d 0c d5 00 00 00    lea    rcx,[rdx*8+0x0]
    14ca:       00 
    14cb:       48 8d 15 6e 2b 00 00    lea    rdx,[rip+0x2b6e]        # 4040 <heap>
    14d2:       48 8b 14 11             mov    rdx,QWORD PTR [rcx+rdx*1]
    14d6:       89 c0                   mov    eax,eax
    14d8:       48 89 42 08             mov    QWORD PTR [rdx+0x8],rax
    14dc:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    14e0:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    14e7:       00 
    14e8:       48 8d 05 51 2b 00 00    lea    rax,[rip+0x2b51]        # 4040 <heap>
    14ef:       48 8b 04 02             mov    rax,QWORD PTR [rdx+rax*1]
    14f3:       48 83 c0 10             add    rax,0x10
    14f7:       ba 20 00 00 00          mov    edx,0x20
    14fc:       be 00 00 00 00          mov    esi,0x0
    1501:       48 89 c7                mov    rdi,rax
    1504:       e8 87 fc ff ff          call   1190 <memset@plt>
    1509:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    150d:       48 89 c6                mov    rsi,rax
    1510:       48 8d 05 51 0b 00 00    lea    rax,[rip+0xb51]        # 2068 <_IO_stdin_used+0x68>
    1517:       48 89 c7                mov    rdi,rax
    151a:       b8 00 00 00 00          mov    eax,0x0
    151f:       e8 5c fc ff ff          call   1180 <printf@plt>
    1524:       eb 01                   jmp    1527 <obj_new+0x138>
    1526:       90                      nop
    1527:       c9                      leave
    1528:       c3                      ret

0000000000001529 <obj_release>:
    1529:       f3 0f 1e fa             endbr64
    152d:       55                      push   rbp
    152e:       48 89 e5                mov    rbp,rsp
    1531:       48 83 ec 10             sub    rsp,0x10
    1535:       48 8d 05 17 0b 00 00    lea    rax,[rip+0xb17]        # 2053 <_IO_stdin_used+0x53>
    153c:       48 89 c7                mov    rdi,rax
    153f:       b8 00 00 00 00          mov    eax,0x0
    1544:       e8 37 fc ff ff          call   1180 <printf@plt>
    1549:       48 8b 05 d0 2a 00 00    mov    rax,QWORD PTR [rip+0x2ad0]        # 4020 <stdout@GLIBC_2.2.5>
    1550:       48 89 c7                mov    rdi,rax
    1553:       e8 88 fc ff ff          call   11e0 <fflush@plt>
    1558:       e8 12 fe ff ff          call   136f <rd_long>
    155d:       48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
    1561:       48 83 7d f8 00          cmp    QWORD PTR [rbp-0x8],0x0
    1566:       78 44                   js     15ac <obj_release+0x83>
    1568:       48 83 7d f8 0f          cmp    QWORD PTR [rbp-0x8],0xf
    156d:       7f 3d                   jg     15ac <obj_release+0x83>
    156f:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    1573:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    157a:       00 
    157b:       48 8d 05 be 2a 00 00    lea    rax,[rip+0x2abe]        # 4040 <heap>
    1582:       48 8b 04 02             mov    rax,QWORD PTR [rdx+rax*1]
    1586:       48 85 c0                test   rax,rax
    1589:       74 21                   je     15ac <obj_release+0x83>
    158b:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    158f:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    1596:       00 
    1597:       48 8d 05 a2 2a 00 00    lea    rax,[rip+0x2aa2]        # 4040 <heap>
    159e:       48 8b 04 02             mov    rax,QWORD PTR [rdx+rax*1]
    15a2:       48 89 c7                mov    rdi,rax
    15a5:       e8 86 fb ff ff          call   1130 <free@plt>
    15aa:       eb 01                   jmp    15ad <obj_release+0x84>
    15ac:       90                      nop
    15ad:       c9                      leave
    15ae:       c3                      ret

00000000000015af <obj_edit>:
    15af:       f3 0f 1e fa             endbr64
    15b3:       55                      push   rbp
    15b4:       48 89 e5                mov    rbp,rsp
    15b7:       48 83 ec 10             sub    rsp,0x10
    15bb:       48 8d 05 91 0a 00 00    lea    rax,[rip+0xa91]        # 2053 <_IO_stdin_used+0x53>
    15c2:       48 89 c7                mov    rdi,rax
    15c5:       b8 00 00 00 00          mov    eax,0x0
    15ca:       e8 b1 fb ff ff          call   1180 <printf@plt>
    15cf:       48 8b 05 4a 2a 00 00    mov    rax,QWORD PTR [rip+0x2a4a]        # 4020 <stdout@GLIBC_2.2.5>
    15d6:       48 89 c7                mov    rdi,rax
    15d9:       e8 02 fc ff ff          call   11e0 <fflush@plt>
    15de:       e8 8c fd ff ff          call   136f <rd_long>
    15e3:       48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
    15e7:       48 83 7d f8 00          cmp    QWORD PTR [rbp-0x8],0x0
    15ec:       0f 88 b3 00 00 00       js     16a5 <obj_edit+0xf6>
    15f2:       48 83 7d f8 0f          cmp    QWORD PTR [rbp-0x8],0xf
    15f7:       0f 8f a8 00 00 00       jg     16a5 <obj_edit+0xf6>
    15fd:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    1601:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    1608:       00 
    1609:       48 8d 05 30 2a 00 00    lea    rax,[rip+0x2a30]        # 4040 <heap>
    1610:       48 8b 04 02             mov    rax,QWORD PTR [rdx+rax*1]
    1614:       48 85 c0                test   rax,rax
    1617:       0f 84 88 00 00 00       je     16a5 <obj_edit+0xf6>
    161d:       48 8d 05 6e 0a 00 00    lea    rax,[rip+0xa6e]        # 2092 <_IO_stdin_used+0x92>
    1624:       48 89 c7                mov    rdi,rax
    1627:       b8 00 00 00 00          mov    eax,0x0
    162c:       e8 4f fb ff ff          call   1180 <printf@plt>
    1631:       48 8b 05 e8 29 00 00    mov    rax,QWORD PTR [rip+0x29e8]        # 4020 <stdout@GLIBC_2.2.5>
    1638:       48 89 c7                mov    rdi,rax
    163b:       e8 a0 fb ff ff          call   11e0 <fflush@plt>
    1640:       e8 2a fd ff ff          call   136f <rd_long>
    1645:       48 89 45 f0             mov    QWORD PTR [rbp-0x10],rax
    1649:       48 83 7d f0 30          cmp    QWORD PTR [rbp-0x10],0x30
    164e:       76 08                   jbe    1658 <obj_edit+0xa9>
    1650:       48 c7 45 f0 30 00 00    mov    QWORD PTR [rbp-0x10],0x30
    1657:       00 
    1658:       48 8d 05 39 0a 00 00    lea    rax,[rip+0xa39]        # 2098 <_IO_stdin_used+0x98>
    165f:       48 89 c7                mov    rdi,rax
    1662:       b8 00 00 00 00          mov    eax,0x0
    1667:       e8 14 fb ff ff          call   1180 <printf@plt>
    166c:       48 8b 05 ad 29 00 00    mov    rax,QWORD PTR [rip+0x29ad]        # 4020 <stdout@GLIBC_2.2.5>
    1673:       48 89 c7                mov    rdi,rax
    1676:       e8 65 fb ff ff          call   11e0 <fflush@plt>
    167b:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    167f:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    1686:       00 
    1687:       48 8d 05 b2 29 00 00    lea    rax,[rip+0x29b2]        # 4040 <heap>
    168e:       48 8b 04 02             mov    rax,QWORD PTR [rdx+rax*1]
    1692:       48 8b 55 f0             mov    rdx,QWORD PTR [rbp-0x10]
    1696:       48 89 c6                mov    rsi,rax
    1699:       bf 00 00 00 00          mov    edi,0x0
    169e:       e8 0d fb ff ff          call   11b0 <read@plt>
    16a3:       eb 01                   jmp    16a6 <obj_edit+0xf7>
    16a5:       90                      nop
    16a6:       c9                      leave
    16a7:       c3                      ret

00000000000016a8 <obj_view>:
    16a8:       f3 0f 1e fa             endbr64
    16ac:       55                      push   rbp
    16ad:       48 89 e5                mov    rbp,rsp
    16b0:       48 83 ec 10             sub    rsp,0x10
    16b4:       48 8d 05 98 09 00 00    lea    rax,[rip+0x998]        # 2053 <_IO_stdin_used+0x53>
    16bb:       48 89 c7                mov    rdi,rax
    16be:       b8 00 00 00 00          mov    eax,0x0
    16c3:       e8 b8 fa ff ff          call   1180 <printf@plt>
    16c8:       48 8b 05 51 29 00 00    mov    rax,QWORD PTR [rip+0x2951]        # 4020 <stdout@GLIBC_2.2.5>
    16cf:       48 89 c7                mov    rdi,rax
    16d2:       e8 09 fb ff ff          call   11e0 <fflush@plt>
    16d7:       e8 93 fc ff ff          call   136f <rd_long>
    16dc:       48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
    16e0:       48 83 7d f8 00          cmp    QWORD PTR [rbp-0x8],0x0
    16e5:       78 71                   js     1758 <obj_view+0xb0>
    16e7:       48 83 7d f8 0f          cmp    QWORD PTR [rbp-0x8],0xf
    16ec:       7f 6a                   jg     1758 <obj_view+0xb0>
    16ee:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    16f2:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    16f9:       00 
    16fa:       48 8d 05 3f 29 00 00    lea    rax,[rip+0x293f]        # 4040 <heap>
    1701:       48 8b 04 02             mov    rax,QWORD PTR [rdx+rax*1]
    1705:       48 85 c0                test   rax,rax
    1708:       74 4e                   je     1758 <obj_view+0xb0>
    170a:       48 8d 05 8e 09 00 00    lea    rax,[rip+0x98e]        # 209f <_IO_stdin_used+0x9f>
    1711:       48 89 c7                mov    rdi,rax
    1714:       b8 00 00 00 00          mov    eax,0x0
    1719:       e8 62 fa ff ff          call   1180 <printf@plt>
    171e:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    1722:       48 8d 14 c5 00 00 00    lea    rdx,[rax*8+0x0]
    1729:       00 
    172a:       48 8d 05 0f 29 00 00    lea    rax,[rip+0x290f]        # 4040 <heap>
    1731:       48 8b 04 02             mov    rax,QWORD PTR [rdx+rax*1]
    1735:       ba 30 00 00 00          mov    edx,0x30
    173a:       48 89 c6                mov    rsi,rax
    173d:       bf 01 00 00 00          mov    edi,0x1
    1742:       e8 09 fa ff ff          call   1150 <write@plt>
    1747:       48 8d 05 61 09 00 00    lea    rax,[rip+0x961]        # 20af <_IO_stdin_used+0xaf>
    174e:       48 89 c7                mov    rdi,rax
    1751:       e8 ea f9 ff ff          call   1140 <puts@plt>
    1756:       eb 01                   jmp    1759 <obj_view+0xb1>
    1758:       90                      nop
    1759:       c9                      leave
    175a:       c3                      ret

000000000000175b <obj_commit>:
    175b:       f3 0f 1e fa             endbr64
    175f:       55                      push   rbp
    1760:       48 89 e5                mov    rbp,rsp
    1763:       48 8b 05 66 29 00 00    mov    rax,QWORD PTR [rip+0x2966]        # 40d0 <g_final>
    176a:       ff d0                   call   rax
    176c:       90                      nop
    176d:       5d                      pop    rbp
    176e:       c3                      ret

000000000000176f <main>:
    176f:       f3 0f 1e fa             endbr64
    1773:       55                      push   rbp
    1774:       48 89 e5                mov    rbp,rsp
    1777:       53                      push   rbx
    1778:       48 83 ec 08             sub    rsp,0x8
    177c:       e8 1f fa ff ff          call   11a0 <geteuid@plt>
    1781:       89 c3                   mov    ebx,eax
    1783:       e8 18 fa ff ff          call   11a0 <geteuid@plt>
    1788:       89 de                   mov    esi,ebx
    178a:       89 c7                   mov    edi,eax
    178c:       e8 5f fa ff ff          call   11f0 <setreuid@plt>
    1791:       48 8b 05 88 28 00 00    mov    rax,QWORD PTR [rip+0x2888]        # 4020 <stdout@GLIBC_2.2.5>
    1798:       b9 00 00 00 00          mov    ecx,0x0
    179d:       ba 02 00 00 00          mov    edx,0x2
    17a2:       be 00 00 00 00          mov    esi,0x0
    17a7:       48 89 c7                mov    rdi,rax
    17aa:       e8 51 fa ff ff          call   1200 <setvbuf@plt>
    17af:       48 8d 05 9f fb ff ff    lea    rax,[rip+0xfffffffffffffb9f]        # 1355 <default_commit>
    17b6:       48 89 05 13 29 00 00    mov    QWORD PTR [rip+0x2913],rax        # 40d0 <g_final>
    17bd:       48 8d 05 ec 08 00 00    lea    rax,[rip+0x8ec]        # 20b0 <_IO_stdin_used+0xb0>
    17c4:       48 89 c7                mov    rdi,rax
    17c7:       b8 00 00 00 00          mov    eax,0x0
    17cc:       e8 af f9 ff ff          call   1180 <printf@plt>
    17d1:       48 8d 05 e0 08 00 00    lea    rax,[rip+0x8e0]        # 20b8 <_IO_stdin_used+0xb8>
    17d8:       48 89 c7                mov    rdi,rax
    17db:       e8 60 f9 ff ff          call   1140 <puts@plt>
    17e0:       48 8d 05 f9 08 00 00    lea    rax,[rip+0x8f9]        # 20e0 <_IO_stdin_used+0xe0>
    17e7:       48 89 c7                mov    rdi,rax
    17ea:       e8 51 f9 ff ff          call   1140 <puts@plt>
    17ef:       48 8d 05 12 09 00 00    lea    rax,[rip+0x912]        # 2108 <_IO_stdin_used+0x108>
    17f6:       48 89 c7                mov    rdi,rax
    17f9:       b8 00 00 00 00          mov    eax,0x0
    17fe:       e8 7d f9 ff ff          call   1180 <printf@plt>
    1803:       48 8d 05 06 09 00 00    lea    rax,[rip+0x906]        # 2110 <_IO_stdin_used+0x110>
    180a:       48 89 c7                mov    rdi,rax
    180d:       b8 00 00 00 00          mov    eax,0x0
    1812:       e8 69 f9 ff ff          call   1180 <printf@plt>
    1817:       48 8b 05 02 28 00 00    mov    rax,QWORD PTR [rip+0x2802]        # 4020 <stdout@GLIBC_2.2.5>
    181e:       48 89 c7                mov    rdi,rax
    1821:       e8 ba f9 ff ff          call   11e0 <fflush@plt>
    1826:       e8 44 fb ff ff          call   136f <rd_long>
    182b:       48 83 f8 05             cmp    rax,0x5
    182f:       77 44                   ja     1875 <main+0x106>
    1831:       48 8d 14 85 00 00 00    lea    rdx,[rax*4+0x0]
    1838:       00 
    1839:       48 8d 05 0c 09 00 00    lea    rax,[rip+0x90c]        # 214c <_IO_stdin_used+0x14c>
    1840:       8b 04 02                mov    eax,DWORD PTR [rdx+rax*1]
    1843:       48 98                   cdqe
    1845:       48 8d 15 00 09 00 00    lea    rdx,[rip+0x900]        # 214c <_IO_stdin_used+0x14c>
    184c:       48 01 d0                add    rax,rdx
    184f:       3e ff e0                notrack jmp rax
    1852:       e8 98 fb ff ff          call   13ef <obj_new>
    1857:       eb 23                   jmp    187c <main+0x10d>
    1859:       e8 cb fc ff ff          call   1529 <obj_release>
    185e:       eb 1c                   jmp    187c <main+0x10d>
    1860:       e8 4a fd ff ff          call   15af <obj_edit>
    1865:       eb 15                   jmp    187c <main+0x10d>
    1867:       e8 3c fe ff ff          call   16a8 <obj_view>
    186c:       eb 0e                   jmp    187c <main+0x10d>
    186e:       e8 e8 fe ff ff          call   175b <obj_commit>
    1873:       eb 07                   jmp    187c <main+0x10d>
    1875:       b8 00 00 00 00          mov    eax,0x0
    187a:       eb 02                   jmp    187e <main+0x10f>
    187c:       eb 85                   jmp    1803 <main+0x94>
    187e:       48 8b 5d f8             mov    rbx,QWORD PTR [rbp-0x8]
    1882:       c9                      leave
    1883:       c3                      ret