
./3jane:     file format elf64-x86-64

```sh
Disassembly of section .text:

0000000000001200 <.text>:
    1200:	f3 0f 1e fa          	endbr64
    1204:	31 ed                	xor    ebp,ebp
    1206:	49 89 d1             	mov    r9,rdx
    1209:	5e                   	pop    rsi
    120a:	48 89 e2             	mov    rdx,rsp
    120d:	48 83 e4 f0          	and    rsp,0xfffffffffffffff0
    1211:	50                   	push   rax
    1212:	54                   	push   rsp
    1213:	45 31 c0             	xor    r8d,r8d
    1216:	31 c9                	xor    ecx,ecx
    1218:	48 8d 3d ed 01 00 00 	lea    rdi,[rip+0x1ed]        # 140c <exit@plt+0x21c>
    121f:	ff 15 b3 2d 00 00    	call   QWORD PTR [rip+0x2db3]        # 3fd8 <exit@plt+0x2de8>
    1225:	f4                   	hlt
    1226:	66 2e 0f 1f 84 00 00 	cs nop WORD PTR [rax+rax*1+0x0]
    122d:	00 00 00 
    1230:	48 8d 3d d9 2d 00 00 	lea    rdi,[rip+0x2dd9]        # 4010 <exit@plt+0x2e20>
    1237:	48 8d 05 d2 2d 00 00 	lea    rax,[rip+0x2dd2]        # 4010 <exit@plt+0x2e20>
    123e:	48 39 f8             	cmp    rax,rdi
    1241:	74 15                	je     1258 <exit@plt+0x68>
    1243:	48 8b 05 96 2d 00 00 	mov    rax,QWORD PTR [rip+0x2d96]        # 3fe0 <exit@plt+0x2df0>
    124a:	48 85 c0             	test   rax,rax
    124d:	74 09                	je     1258 <exit@plt+0x68>
    124f:	ff e0                	jmp    rax
    1251:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]
    1258:	c3                   	ret
    1259:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]
    1260:	48 8d 3d a9 2d 00 00 	lea    rdi,[rip+0x2da9]        # 4010 <exit@plt+0x2e20>
    1267:	48 8d 35 a2 2d 00 00 	lea    rsi,[rip+0x2da2]        # 4010 <exit@plt+0x2e20>
    126e:	48 29 fe             	sub    rsi,rdi
    1271:	48 89 f0             	mov    rax,rsi
    1274:	48 c1 ee 3f          	shr    rsi,0x3f
    1278:	48 c1 f8 03          	sar    rax,0x3
    127c:	48 01 c6             	add    rsi,rax
    127f:	48 d1 fe             	sar    rsi,1
    1282:	74 14                	je     1298 <exit@plt+0xa8>
    1284:	48 8b 05 65 2d 00 00 	mov    rax,QWORD PTR [rip+0x2d65]        # 3ff0 <exit@plt+0x2e00>
    128b:	48 85 c0             	test   rax,rax
    128e:	74 08                	je     1298 <exit@plt+0xa8>
    1290:	ff e0                	jmp    rax
    1292:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
    1298:	c3                   	ret
    1299:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]
    12a0:	f3 0f 1e fa          	endbr64
    12a4:	80 3d 7d 2d 00 00 00 	cmp    BYTE PTR [rip+0x2d7d],0x0        # 4028 <stdout@GLIBC_2.2.5+0x8>
    12ab:	75 2b                	jne    12d8 <exit@plt+0xe8>
    12ad:	55                   	push   rbp
    12ae:	48 83 3d 42 2d 00 00 	cmp    QWORD PTR [rip+0x2d42],0x0        # 3ff8 <exit@plt+0x2e08>
    12b5:	00 
    12b6:	48 89 e5             	mov    rbp,rsp
    12b9:	74 0c                	je     12c7 <exit@plt+0xd7>
    12bb:	48 8b 3d 46 2d 00 00 	mov    rdi,QWORD PTR [rip+0x2d46]        # 4008 <exit@plt+0x2e18>
    12c2:	e8 49 fe ff ff       	call   1110 <__cxa_finalize@plt>
    12c7:	e8 64 ff ff ff       	call   1230 <exit@plt+0x40>
    12cc:	c6 05 55 2d 00 00 01 	mov    BYTE PTR [rip+0x2d55],0x1        # 4028 <stdout@GLIBC_2.2.5+0x8>
    12d3:	5d                   	pop    rbp
    12d4:	c3                   	ret
    12d5:	0f 1f 00             	nop    DWORD PTR [rax]
    12d8:	c3                   	ret
    12d9:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]
    12e0:	f3 0f 1e fa          	endbr64
    12e4:	e9 77 ff ff ff       	jmp    1260 <exit@plt+0x70>
    12e9:	f3 0f 1e fa          	endbr64
    12ed:	48 83 ec 18          	sub    rsp,0x18
    12f1:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    12f8:	00 00 
    12fa:	48 89 44 24 08       	mov    QWORD PTR [rsp+0x8],rax
    12ff:	31 c0                	xor    eax,eax
    1301:	48 8b 77 08          	mov    rsi,QWORD PTR [rdi+0x8]
    1305:	48 8d 3d fc 0c 00 00 	lea    rdi,[rip+0xcfc]        # 2008 <exit@plt+0xe18>
    130c:	e8 5f fe ff ff       	call   1170 <printf@plt>
    1311:	48 8b 44 24 08       	mov    rax,QWORD PTR [rsp+0x8]
    1316:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    131d:	00 00 
    131f:	75 05                	jne    1326 <exit@plt+0x136>
    1321:	48 83 c4 18          	add    rsp,0x18
    1325:	c3                   	ret
    1326:	e8 25 fe ff ff       	call   1150 <__stack_chk_fail@plt>
    132b:	f3 0f 1e fa          	endbr64
    132f:	48 83 ec 18          	sub    rsp,0x18
    1333:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    133a:	00 00 
    133c:	48 89 44 24 08       	mov    QWORD PTR [rsp+0x8],rax
    1341:	31 c0                	xor    eax,eax
    1343:	48 8d 3d e6 0c 00 00 	lea    rdi,[rip+0xce6]        # 2030 <exit@plt+0xe40>
    134a:	e8 e1 fd ff ff       	call   1130 <puts@plt>
    134f:	48 8b 44 24 08       	mov    rax,QWORD PTR [rsp+0x8]
    1354:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    135b:	00 00 
    135d:	75 05                	jne    1364 <exit@plt+0x174>
    135f:	48 83 c4 18          	add    rsp,0x18
    1363:	c3                   	ret
    1364:	e8 e7 fd ff ff       	call   1150 <__stack_chk_fail@plt>
    1369:	48 83 ec 38          	sub    rsp,0x38
    136d:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    1374:	00 00 
    1376:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
    137b:	31 c0                	xor    eax,eax
    137d:	48 89 e6             	mov    rsi,rsp
    1380:	ba 1f 00 00 00       	mov    edx,0x1f
    1385:	bf 00 00 00 00       	mov    edi,0x0
    138a:	e8 01 fe ff ff       	call   1190 <read@plt>
    138f:	48 85 c0             	test   rax,rax
    1392:	7e 2b                	jle    13bf <exit@plt+0x1cf>
    1394:	c6 04 04 00          	mov    BYTE PTR [rsp+rax*1],0x0
    1398:	48 89 e7             	mov    rdi,rsp
    139b:	ba 00 00 00 00       	mov    edx,0x0
    13a0:	be 00 00 00 00       	mov    esi,0x0
    13a5:	e8 f6 fd ff ff       	call   11a0 <strtol@plt>
    13aa:	48 8b 54 24 28       	mov    rdx,QWORD PTR [rsp+0x28]
    13af:	64 48 2b 14 25 28 00 	sub    rdx,QWORD PTR fs:0x28
    13b6:	00 00 
    13b8:	75 0f                	jne    13c9 <exit@plt+0x1d9>
    13ba:	48 83 c4 38          	add    rsp,0x38
    13be:	c3                   	ret
    13bf:	bf 00 00 00 00       	mov    edi,0x0
    13c4:	e8 27 fe ff ff       	call   11f0 <exit@plt>
    13c9:	e8 82 fd ff ff       	call   1150 <__stack_chk_fail@plt>
    13ce:	f3 0f 1e fa          	endbr64
    13d2:	48 83 ec 18          	sub    rsp,0x18
    13d6:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    13dd:	00 00 
    13df:	48 89 44 24 08       	mov    QWORD PTR [rsp+0x8],rax
    13e4:	31 c0                	xor    eax,eax
    13e6:	48 8d 3d 25 0d 00 00 	lea    rdi,[rip+0xd25]        # 2112 <exit@plt+0xf22>
    13ed:	e8 6e fd ff ff       	call   1160 <system@plt>
    13f2:	48 8b 44 24 08       	mov    rax,QWORD PTR [rsp+0x8]
    13f7:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    13fe:	00 00 
    1400:	75 05                	jne    1407 <exit@plt+0x217>
    1402:	48 83 c4 18          	add    rsp,0x18
    1406:	c3                   	ret
    1407:	e8 44 fd ff ff       	call   1150 <__stack_chk_fail@plt>
    140c:	f3 0f 1e fa          	endbr64
    1410:	41 55                	push   r13
    1412:	41 54                	push   r12
    1414:	55                   	push   rbp
    1415:	53                   	push   rbx
    1416:	48 83 ec 18          	sub    rsp,0x18
    141a:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    1421:	00 00 
    1423:	48 89 44 24 08       	mov    QWORD PTR [rsp+0x8],rax
    1428:	31 c0                	xor    eax,eax
    142a:	e8 51 fd ff ff       	call   1180 <geteuid@plt>
    142f:	89 c3                	mov    ebx,eax
    1431:	e8 4a fd ff ff       	call   1180 <geteuid@plt>
    1436:	89 c7                	mov    edi,eax
    1438:	89 de                	mov    esi,ebx
    143a:	e8 91 fd ff ff       	call   11d0 <setreuid@plt>
    143f:	b9 00 00 00 00       	mov    ecx,0x0
    1444:	ba 02 00 00 00       	mov    edx,0x2
    1449:	be 00 00 00 00       	mov    esi,0x0
    144e:	48 8b 3d cb 2b 00 00 	mov    rdi,QWORD PTR [rip+0x2bcb]        # 4020 <stdout@GLIBC_2.2.5>
    1455:	e8 86 fd ff ff       	call   11e0 <setvbuf@plt>
    145a:	48 8d 05 ca fe ff ff 	lea    rax,[rip+0xfffffffffffffeca]        # 132b <exit@plt+0x13b>
    1461:	48 89 05 d8 2b 00 00 	mov    QWORD PTR [rip+0x2bd8],rax        # 4040 <stdout@GLIBC_2.2.5+0x20> g_final address
    1468:	48 8d 3d ab 0c 00 00 	lea    rdi,[rip+0xcab]        # 211a <exit@plt+0xf2a>
    146f:	b8 00 00 00 00       	mov    eax,0x0
    1474:	e8 f7 fc ff ff       	call   1170 <printf@plt>
    1479:	48 8d 3d d8 0b 00 00 	lea    rdi,[rip+0xbd8]        # 2058 <exit@plt+0xe68>
    1480:	e8 ab fc ff ff       	call   1130 <puts@plt>
    1485:	48 8d 3d f4 0b 00 00 	lea    rdi,[rip+0xbf4]        # 2080 <exit@plt+0xe90>
    148c:	e8 9f fc ff ff       	call   1130 <puts@plt>
    1491:	48 8d 3d 8a 0c 00 00 	lea    rdi,[rip+0xc8a]        # 2122 <exit@plt+0xf32>
    1498:	b8 00 00 00 00       	mov    eax,0x0
    149d:	e8 ce fc ff ff       	call   1170 <printf@plt>
    14a2:	48 8d 2d ff 0b 00 00 	lea    rbp,[rip+0xbff]        # 20a8 <exit@plt+0xeb8>
    14a9:	48 8d 1d 9c 0c 00 00 	lea    rbx,[rip+0xc9c]        # 214c <exit@plt+0xf5c>
    14b0:	48 89 ef             	mov    rdi,rbp
    14b3:	b8 00 00 00 00       	mov    eax,0x0
    14b8:	e8 b3 fc ff ff       	call   1170 <printf@plt>
    14bd:	48 8b 3d 5c 2b 00 00 	mov    rdi,QWORD PTR [rip+0x2b5c]        # 4020 <stdout@GLIBC_2.2.5>
    14c4:	e8 f7 fc ff ff       	call   11c0 <fflush@plt>
    14c9:	e8 9b fe ff ff       	call   1369 <exit@plt+0x179>
    14ce:	48 83 f8 05          	cmp    rax,0x5
    14d2:	0f 87 0e 02 00 00    	ja     16e6 <exit@plt+0x4f6>
    14d8:	48 63 04 83          	movsxd rax,DWORD PTR [rbx+rax*4]
    14dc:	48 01 d8             	add    rax,rbx
    14df:	3e ff e0             	notrack jmp rax
    14e2:	48 8d 3d 3e 0c 00 00 	lea    rdi,[rip+0xc3e]        # 2127 <exit@plt+0xf37>
    14e9:	b8 00 00 00 00       	mov    eax,0x0
    14ee:	e8 7d fc ff ff       	call   1170 <printf@plt>
    14f3:	48 8b 3d 26 2b 00 00 	mov    rdi,QWORD PTR [rip+0x2b26]        # 4020 <stdout@GLIBC_2.2.5>
    14fa:	e8 c1 fc ff ff       	call   11c0 <fflush@plt>
    14ff:	e8 65 fe ff ff       	call   1369 <exit@plt+0x179>
    1504:	49 89 c4             	mov    r12,rax
    1507:	48 83 f8 0f          	cmp    rax,0xf
    150b:	77 a3                	ja     14b0 <exit@plt+0x2c0>
    150d:	bf 30 00 00 00       	mov    edi,0x30
    1512:	e8 99 fc ff ff       	call   11b0 <malloc@plt>
    1517:	48 8d 15 42 2b 00 00 	lea    rdx,[rip+0x2b42]        # 4060 <stdout@GLIBC_2.2.5+0x40>
    151e:	4a 89 04 e2          	mov    QWORD PTR [rdx+r12*8],rax
    1522:	48 8d 0d c0 fd ff ff 	lea    rcx,[rip+0xfffffffffffffdc0]        # 12e9 <exit@plt+0xf9>
    1529:	48 89 08             	mov    QWORD PTR [rax],rcx
    152c:	8b 15 16 2b 00 00    	mov    edx,DWORD PTR [rip+0x2b16]        # 4048 <stdout@GLIBC_2.2.5+0x28>
    1532:	8d 4a 01             	lea    ecx,[rdx+0x1]
    1535:	89 0d 0d 2b 00 00    	mov    DWORD PTR [rip+0x2b0d],ecx        # 4048 <stdout@GLIBC_2.2.5+0x28>
    153b:	89 d1                	mov    ecx,edx
    153d:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
    1541:	48 8d 50 10          	lea    rdx,[rax+0x10]
    1545:	b9 08 00 00 00       	mov    ecx,0x8
    154a:	b8 00 00 00 00       	mov    eax,0x0
    154f:	48 89 d7             	mov    rdi,rdx
    1552:	f3 ab                	rep stos DWORD PTR es:[rdi],eax
    1554:	4c 89 e6             	mov    rsi,r12
    1557:	48 8d 3d 8a 0b 00 00 	lea    rdi,[rip+0xb8a]        # 20e8 <exit@plt+0xef8>
    155e:	e8 0d fc ff ff       	call   1170 <printf@plt>
    1563:	e9 48 ff ff ff       	jmp    14b0 <exit@plt+0x2c0>
    1568:	48 8d 3d b8 0b 00 00 	lea    rdi,[rip+0xbb8]        # 2127 <exit@plt+0xf37>
    156f:	b8 00 00 00 00       	mov    eax,0x0
    1574:	e8 f7 fb ff ff       	call   1170 <printf@plt>
    1579:	48 8b 3d a0 2a 00 00 	mov    rdi,QWORD PTR [rip+0x2aa0]        # 4020 <stdout@GLIBC_2.2.5>
    1580:	e8 3b fc ff ff       	call   11c0 <fflush@plt>
    1585:	e8 df fd ff ff       	call   1369 <exit@plt+0x179>
    158a:	48 83 f8 0f          	cmp    rax,0xf
    158e:	0f 87 1c ff ff ff    	ja     14b0 <exit@plt+0x2c0>
    1594:	48 8d 15 c5 2a 00 00 	lea    rdx,[rip+0x2ac5]        # 4060 <stdout@GLIBC_2.2.5+0x40>
    159b:	48 8b 3c c2          	mov    rdi,QWORD PTR [rdx+rax*8]
    159f:	48 85 ff             	test   rdi,rdi
    15a2:	0f 84 08 ff ff ff    	je     14b0 <exit@plt+0x2c0>
    15a8:	e8 73 fb ff ff       	call   1120 <free@plt>
    15ad:	e9 fe fe ff ff       	jmp    14b0 <exit@plt+0x2c0>
    15b2:	48 8d 3d 6e 0b 00 00 	lea    rdi,[rip+0xb6e]        # 2127 <exit@plt+0xf37>
    15b9:	b8 00 00 00 00       	mov    eax,0x0
    15be:	e8 ad fb ff ff       	call   1170 <printf@plt>
    15c3:	48 8b 3d 56 2a 00 00 	mov    rdi,QWORD PTR [rip+0x2a56]        # 4020 <stdout@GLIBC_2.2.5>
    15ca:	e8 f1 fb ff ff       	call   11c0 <fflush@plt>
    15cf:	e8 95 fd ff ff       	call   1369 <exit@plt+0x179>
    15d4:	49 89 c4             	mov    r12,rax
    15d7:	48 83 f8 0f          	cmp    rax,0xf
    15db:	0f 87 cf fe ff ff    	ja     14b0 <exit@plt+0x2c0>
    15e1:	48 8d 05 78 2a 00 00 	lea    rax,[rip+0x2a78]        # 4060 <stdout@GLIBC_2.2.5+0x40>
    15e8:	4a 83 3c e0 00       	cmp    QWORD PTR [rax+r12*8],0x0
    15ed:	0f 84 bd fe ff ff    	je     14b0 <exit@plt+0x2c0>
    15f3:	48 8d 3d 34 0b 00 00 	lea    rdi,[rip+0xb34]        # 212e <exit@plt+0xf3e>
    15fa:	b8 00 00 00 00       	mov    eax,0x0
    15ff:	e8 6c fb ff ff       	call   1170 <printf@plt>
    1604:	48 8b 3d 15 2a 00 00 	mov    rdi,QWORD PTR [rip+0x2a15]        # 4020 <stdout@GLIBC_2.2.5>
    160b:	e8 b0 fb ff ff       	call   11c0 <fflush@plt>
    1610:	e8 54 fd ff ff       	call   1369 <exit@plt+0x179>
    1615:	49 89 c5             	mov    r13,rax
    1618:	48 8d 3d 15 0b 00 00 	lea    rdi,[rip+0xb15]        # 2134 <exit@plt+0xf44>
    161f:	b8 00 00 00 00       	mov    eax,0x0
    1624:	e8 47 fb ff ff       	call   1170 <printf@plt>
    1629:	48 8b 3d f0 29 00 00 	mov    rdi,QWORD PTR [rip+0x29f0]        # 4020 <stdout@GLIBC_2.2.5>
    1630:	e8 8b fb ff ff       	call   11c0 <fflush@plt>
    1635:	b8 30 00 00 00       	mov    eax,0x30
    163a:	49 39 c5             	cmp    r13,rax
    163d:	49 0f 46 c5          	cmovbe rax,r13
    1641:	48 89 c2             	mov    rdx,rax
    1644:	48 8d 05 15 2a 00 00 	lea    rax,[rip+0x2a15]        # 4060 <stdout@GLIBC_2.2.5+0x40>
    164b:	4a 8b 34 e0          	mov    rsi,QWORD PTR [rax+r12*8]
    164f:	bf 00 00 00 00       	mov    edi,0x0
    1654:	e8 37 fb ff ff       	call   1190 <read@plt>
    1659:	e9 52 fe ff ff       	jmp    14b0 <exit@plt+0x2c0>
    165e:	48 8d 3d c2 0a 00 00 	lea    rdi,[rip+0xac2]        # 2127 <exit@plt+0xf37>
    1665:	b8 00 00 00 00       	mov    eax,0x0
    166a:	e8 01 fb ff ff       	call   1170 <printf@plt>
    166f:	48 8b 3d aa 29 00 00 	mov    rdi,QWORD PTR [rip+0x29aa]        # 4020 <stdout@GLIBC_2.2.5>
    1676:	e8 45 fb ff ff       	call   11c0 <fflush@plt>
    167b:	e8 e9 fc ff ff       	call   1369 <exit@plt+0x179>
    1680:	49 89 c4             	mov    r12,rax
    1683:	48 83 f8 0f          	cmp    rax,0xf
    1687:	0f 87 23 fe ff ff    	ja     14b0 <exit@plt+0x2c0>
    168d:	48 8d 05 cc 29 00 00 	lea    rax,[rip+0x29cc]        # 4060 <stdout@GLIBC_2.2.5+0x40>
    1694:	4a 83 3c e0 00       	cmp    QWORD PTR [rax+r12*8],0x0
    1699:	0f 84 11 fe ff ff    	je     14b0 <exit@plt+0x2c0>
    169f:	48 8d 3d 95 0a 00 00 	lea    rdi,[rip+0xa95]        # 213b <exit@plt+0xf4b>
    16a6:	b8 00 00 00 00       	mov    eax,0x0
    16ab:	e8 c0 fa ff ff       	call   1170 <printf@plt>
    16b0:	48 8d 05 a9 29 00 00 	lea    rax,[rip+0x29a9]        # 4060 <stdout@GLIBC_2.2.5+0x40>
    16b7:	4a 8b 34 e0          	mov    rsi,QWORD PTR [rax+r12*8]
    16bb:	ba 30 00 00 00       	mov    edx,0x30
    16c0:	bf 01 00 00 00       	mov    edi,0x1
    16c5:	e8 76 fa ff ff       	call   1140 <write@plt>
    16ca:	48 8d 3d 69 0a 00 00 	lea    rdi,[rip+0xa69]        # 213a <exit@plt+0xf4a>
    16d1:	e8 5a fa ff ff       	call   1130 <puts@plt>
    16d6:	e9 d5 fd ff ff       	jmp    14b0 <exit@plt+0x2c0>
    16db:	ff 15 5f 29 00 00    	call   QWORD PTR [rip+0x295f]        # 4040 <stdout@GLIBC_2.2.5+0x20>
    16e1:	e9 ca fd ff ff       	jmp    14b0 <exit@plt+0x2c0>
    16e6:	48 8b 44 24 08       	mov    rax,QWORD PTR [rsp+0x8]
    16eb:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    16f2:	00 00 
    16f4:	75 10                	jne    1706 <exit@plt+0x516>
    16f6:	b8 00 00 00 00       	mov    eax,0x0
    16fb:	48 83 c4 18          	add    rsp,0x18
    16ff:	5b                   	pop    rbx
    1700:	5d                   	pop    rbp
    1701:	41 5c                	pop    r12
    1703:	41 5d                	pop    r13
    1705:	c3                   	ret
    1706:	e8 45 fa ff ff       	call   1150 <__stack_chk_fail@plt>
```