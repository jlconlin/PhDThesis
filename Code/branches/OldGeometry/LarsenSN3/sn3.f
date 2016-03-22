CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C								       C
C								       C
C                                 SN3			               C
C    								       C
C								       C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C
C     This program computes the k-eigenvalue for the three-group, slab
C     geometry, discrete-ordinates equations, with fission but no 
C     upscattering.  The right boundary is reflecting and the left 
C     boundary is either reflecting or vacuum.  The Diamond Difference
C     spatial differencing scheme, with the Source Iteration method for 
C     the inner iterations and the Inverse Power Iteration method for
C     the outer iterations, are used. The Gauss-Legendre quadrature 
C     sets of orders 2,4, 8, and 16 are encoded.  The user specifies on
C     an input file INSN3 the quantities: 
C     
C          NREG = the number of spatial regions
C          NQ = the order of the quadrature set (2,4,8, or 16)
C          NB = 0 : left boundary vacuum
C               1 : left boundary reflecting,
C
C     and then, for each region,
C
C          NC = the number of cells
C          CDX = delta x
C          CTCS1 = total cross section for group 1
C          CTCS2 = total cross section for group 2
C          CTCS3 = total cross section for group 3
C          CSCS11 = scattering cross section from group 1 into 
C                   group 1
C          CSCS12 = scattering cross section from group 1 into
C                   group 2
C          CSCS13 = scattering cross section from group 1 into
C                   group 3
C          CSCS22 = scattering cross section from group 2 into
C                   group 2
C          CSCS23 = scattering cross section from group 2 into
C                   group 3
C          CSCS33 = scattering cross section from group 3 into 
C                   group 3
C          CFCS1 = nu times fission cross section for group 1
C          CFCS2 = nu times fission cross section for group 2
C          CFCS3 = nu times fission cross section for group 3.
C
C     In each region, CDX and the cross sections are constant.  The
C     user also specifies:
C
C          NMAXI = the maximum number of inner iterations per outer
C          EPSI = the "relative" inner iteration convergence
C                 criterion
C          NMAXO = the maximum number of outer iterations
C          EPSF = the fission source convergence criterion
C          EPSK = the k-eigenvalue convergence criterion.
C
C     SN3 prints out the above information on the output file
C     OUTSN3.  It also prints in OUTSN3 and on the computer screen,
C     for each M-th outer iteration,
C
C          N1 = the number of (inner) iterations in group 1
C          N2 = the number of (inner) iterations in group 2
C          N3 = the number of (inner) iterations in group 3
C          DF = the L2 norm of the difference between the new
C               estimate of the fission source and the previous
C               estimate
C          RDF = DF divided by its value for the previous iteration
C          DK = the absolute value of the difference between the 
C               new estimate of K and the previous estimate
C          RDK = DK divided by its value for the previous iteration
C          K = current estimate for the eigenvalue k.
C
C     Finally, SN3 prints in OUTSN3 the cell center values of x, the 
C     corresponding converged cell-average group scalar fluxes and
C     fission source, and the average group fluxes in each 
C     region.
C
C     Modifications:
C     4/20/1992---Original Code.  Ed Larsen
C     4/11/2007---Order 32 Gauss-Legendra Quadrature set added.  
C                 Jeremy Conlin
C
C     
      DIMENSION TCS1(200),TCS2(200),TCS3(200),SCS11(200),SCS12(200),
     1 SCS13(200),SCS22(200),SCS23(200),SCS33(200),FCS1(200),
     2 FCS2(200),FCS3(200),DX(200),FS(200),FSP(200),F1(200),
     3 F2(200),F3(200),F1P(200),F2P(200),F3P(200),
     4 AX(16),W(16),FIN1(16),FIN2(16),FIN3(16),
     5 F1AV(20),F2AV(20),F3AV(20),WC(20),MC(20)
C
C     READ IN DATA
C
      OPEN (2,FILE='OUTSN3',STATUS='NEW')
      WRITE (2,5)
    5 FORMAT (30X,'OUTSN3',/,/)
      WRITE (2,10)
   10 FORMAT (1X,'NREG',3X,'NQ',3X,'NB')
      OPEN (1,FILE='INSN3')
      READ (1,20) NREG,NQ,NB
   20 FORMAT (3I5)
      WRITE (2,20) NREG,NQ,NB
C
      WRITE (2,25) 
   25 FORMAT (/,2X,'REG',3X,'NC',8X,'DX',6X,'TCS1',6X,'TCS2',
     1 6X,'TCS3',5X,'SCS11',5X,'SCS12')
      J=0
      WS=0.0
      DO 45 K=1,NREG
      READ (1,30) NC,CDX,CTCS1,CTCS2,CTCS3,CSCS11,CSCS12
   30 FORMAT (I5,6F10.6)
      WRITE (2,35)K,NC,CDX,CTCS1,CTCS2,CTCS3,CSCS11,CSCS12
   35 FORMAT (2I5,6F10.6)
      MC(K)=NC
      WC(K)=CDX*FLOAT(NC)
      WS=WS+WC(K)      
      DO 40 I=1,NC
      J=J+1
      IF (J.GT.200) GO TO 570
      DX(J)=CDX
      TCS1(J)=CTCS1
      TCS2(J)=CTCS2
      TCS3(J)=CTCS3
      SCS11(J)=CSCS11
      SCS12(J)=CSCS12
   40 CONTINUE
   45 CONTINUE
      NC=J
C
      WRITE (2,50)
   50 FORMAT (/,5X,'SCS13',5X,'SCS22',5X,'SCS23',5X,'SCS33',6X,
     1 'FCS1',6X,'FCS2',6X,'FCS3')
      J=0
      DO 65 K=1,NREG
      READ (1,55) CSCS13,CSCS22,CSCS23,CSCS33,CFCS1,CFCS2,CFCS3
   55 FORMAT (7F10.6)
      WRITE (2,55) CSCS13,CSCS22,CSCS23,CSCS33,CFCS1,CFCS2,CFCS3
      DO 60 I=1,MC(K)
      J=J+1
      SCS13(J)=CSCS13
      SCS22(J)=CSCS22
      SCS23(J)=CSCS23
      SCS33(J)=CSCS33
      FCS1(J)=CFCS1
      FCS2(J)=CFCS2
      FCS3(J)=CFCS3
   60 CONTINUE
   65 CONTINUE
C     
      WRITE (2,70)
   70 FORMAT (/,'NMAXI',11X,'EPSI')
      READ (1,80) NMAXI,EPSI
   80 FORMAT (I5,F15.10)
      WRITE (2,80) NMAXI,EPSI
      WRITE (2,90)
   90 FORMAT (/,'NMAXO',11X,'EPSF',11X,'EPSK')
      READ (1,100) NMAXO,EPSF,EPSK
  100 FORMAT (I5,2F15.10)
      WRITE (2,100) NMAXO,EPSF,EPSK
C
      NQ=NQ/2
      IF (NQ.EQ.1) GO TO 110
      IF (NQ.EQ.2) GO TO 120
      IF (NQ.EQ.4) GO TO 130
      IF (NQ.EQ.8) GO TO 140
      IF (NQ.EQ.16) GO TO 145
      GO TO 580
  110 AX(1)=.5773502692
      W(1)=1.0
      GO TO 150
  120 AX(1)=.8611363116
      AX(2)=.3399810435
      W(1)=.3478548451
      W(2)=.6521451549
      GO TO 150
  130 AX(1)=.9602898565
      AX(2)=.7966664774
      AX(3)=.5255324099
      AX(4)=.1834346425
      W(1)=.1012285363
      W(2)=.2223810344
      W(3)=.3137066459
      W(4)=.3626837834
      GO TO 150
  140 AX(1)=.9894009350
      AX(2)=.9445750231
      AX(3)=.8656312024
      AX(4)=.7554044084
      AX(5)=.6178762444
      AX(6)=.4580167777
      AX(7)=.2816035508
      AX(8)=.0950125098
      W(1)=.0271524594
      W(2)=.0622535239
      W(3)=.0951585117
      W(4)=.1246289713
      W(5)=.1495959888
      W(6)=.1691565194
      W(7)=.1826034150
      W(8)=.1894506105
      GO TO 150
  145 AX(1)= 0.997263861849482
      AX(2)= 0.985611511545268
      AX(3)= 0.964762255587506
      AX(4)= 0.934906075937740
      AX(5)= 0.896321155766052
      AX(6)= 0.849367613732570
      AX(7)= 0.794483795967942
      AX(8)= 0.732182118740290
      AX(9)= 0.663044266930215
      AX(10)=0.587715757240762
      AX(11)=0.506899908932229
      AX(12)=0.421351276130635
      AX(13)=0.331868602282128
      AX(14)=0.239287362252137
      AX(15)=0.144471961582796
      AX(16)=0.048307665687738
      W(1)= 0.007018610009469
      W(2)= 0.016274394730904
      W(3)= 0.025392065309262
      W(4)= 0.034273862913021
      W(5)= 0.042835898022226
      W(6)= 0.050998059262376
      W(7)= 0.058684093478535
      W(8)= 0.065822222776361
      W(9)= 0.072345794108848
      W(10)=0.078193895787070
      W(11)=0.083311924226946
      W(12)=0.087652093004403
      W(13)=0.091173878695763
      W(14)=0.093844399080804
      W(15)=0.095638720079274
      W(16)=0.096540088514727
      GO TO 150
C
  150 WRITE (2,160)
  160 FORMAT (/,4X,'M',3X,'N1',3X,'N2',3X,'N3',8X,'DF',7X,'RDF',
     1 8X,'DK',7X,'RDK',9X,'K')
      WRITE (9,160)
C
C     INITIALIZE FLUXES
C
      DO 170 J=1,NC
      FSP(J)=1.0
      F1(J)=0.0
      F2(J)=0.0
      F3(J)=0.0
      F1P(J)=0.0
      F2P(J)=0.0
      F3P(J)=0.0
  170 CONTINUE
      DFP=1.0
      DKP=1.0
      NOUT=1
      EVP=1.0
      N1=1
      N2=1
      N3=1
      DO 180 M=1,NQ
      FIN1(M)=0.0
      FIN2(M)=0.0
      FIN3(M)=0.0
  180 CONTINUE
C
C     GROUP 1 : INNER ITERATIONS
C
  190 DO 200 M=1,NQ
      FIN1(M)=FIN1(M)*FLOAT(NB)
  200 CONTINUE
C
      DO 220 J=1,NC
      Q=0.5*(SCS11(J)*F1P(J)+EVP*FSP(J))
      DO 210 M=1,NQ
      C1=AX(M)-0.5*TCS1(J)*DX(J)
      C2=AX(M)+0.5*TCS1(J)*DX(J)
      FOUT=(C1*FIN1(M)+DX(J)*Q)/C2
      F1(J)=F1(J)+0.5*(FOUT+FIN1(M))*W(M)
      FIN1(M)=FOUT
  210 CONTINUE
  220 CONTINUE
C
      DO 240 I=1,NC
      J=NC+1-I
      Q=0.5*(SCS11(J)*F1P(J)+EVP*FSP(J))
      DO 230 M=1,NQ
      C1=AX(M)-0.5*TCS1(J)*DX(J)
      C2=AX(M)+0.5*TCS1(J)*DX(J)
      FOUT=(C1*FIN1(M)+DX(J)*Q)/C2
      F1(J)=F1(J)+0.5*(FOUT+FIN1(M))*W(M)
      FIN1(M)=FOUT
  230 CONTINUE
  240 CONTINUE
C
      Z=0.0
      DO 250 J=1,NC
      Z=Z+ABS(F1(J)-F1P(J))*DX(J)
  250 CONTINUE
      N1=N1+1
      DO 260 J=1,NC
      F1P(J)=F1(J)
      F1(J)=0.0
  260 CONTINUE
      IF (Z.LT.EPSI*DFP) GO TO 270
      IF (N1.GT.NMAXI) GO TO 270
      GO TO 190
C
C     GROUP 2 : INNER ITERATIONS
C
  270 DO 280 M=1,NQ
      FIN2(M)=FIN2(M)*FLOAT(NB)
  280 CONTINUE
C
      DO 300 J=1,NC
      Q=0.5*(SCS22(J)*F2P(J)+SCS12(J)*F1P(J))
      DO 290 M=1,NQ
      C1=AX(M)-0.5*TCS2(J)*DX(J)
      C2=AX(M)+0.5*TCS2(J)*DX(J)
      FOUT=(C1*FIN2(M)+DX(J)*Q)/C2
      F2(J)=F2(J)+0.5*(FOUT+FIN2(M))*W(M)
      FIN2(M)=FOUT
  290 CONTINUE
  300 CONTINUE
C
      DO 320 I=1,NC
      J=NC+1-I
      Q=0.5*(SCS22(J)*F2P(J)+SCS12(J)*F1P(J))
      DO 310 M=1,NQ
      C1=AX(M)-0.5*TCS2(J)*DX(J)
      C2=AX(M)+0.5*TCS2(J)*DX(J)
      FOUT=(C1*FIN2(M)+DX(J)*Q)/C2
      F2(J)=F2(J)+0.5*(FOUT+FIN2(M))*W(M)
      FIN2(M)=FOUT
  310 CONTINUE
  320 CONTINUE
C
      Z=0.0
      DO 330 J=1,NC
      Z=Z+ABS(F2(J)-F2P(J))*DX(J)
  330 CONTINUE
      N2=N2+1
      DO 340 J=1,NC
      F2P(J)=F2(J)
      F2(J)=0.0
  340 CONTINUE
      IF (Z.LT.EPSI*DFP) GO TO 350
      IF (N2.GT.NMAXI) GO TO 350
      GO TO 270
C
C     GROUP 3 : INNER ITERATIONS
C
  350 DO 360 M=1,NQ
      FIN3(M)=FIN3(M)*FLOAT(NB)
  360 CONTINUE
C
      DO 380 J=1,NC
      Q=0.5*(SCS33(J)*F3P(J)+SCS23(J)*F2P(J)+SCS13(J)*F1P(J))
      DO 370 M=1,NQ
      C1=AX(M)-0.5*TCS3(J)*DX(J)
      C2=AX(M)+0.5*TCS3(J)*DX(J)
      FOUT=(C1*FIN3(M)+DX(J)*Q)/C2
      F3(J)=F3(J)+0.5*(FOUT+FIN3(M))*W(M)
      FIN3(M)=FOUT
  370 CONTINUE
  380 CONTINUE
C
      DO 400 I=1,NC
      J=NC+1-I
      Q=0.5*(SCS33(J)*F3P(J)+SCS23(J)*F2P(J)+SCS13(J)*F1P(J))
      DO 390 M=1,NQ
      C1=AX(M)-0.5*TCS3(J)*DX(J)
      C2=AX(M)+0.5*TCS3(J)*DX(J)
      FOUT=(C1*FIN3(M)+DX(J)*Q)/C2
      F3(J)=F3(J)+0.5*(FOUT+FIN3(M))*W(M)
      FIN3(M)=FOUT
  390 CONTINUE
  400 CONTINUE
C
      Z=0.0
      DO 410 J=1,NC
      Z=Z+ABS(F3(J)-F3P(J))*DX(J)
  410 CONTINUE
      N3=N3+1
      DO 420 J=1,NC
      F3P(J)=F3(J)
      F3(J)=0.0
  420 CONTINUE
      IF (Z.LT.EPSI*DFP) GO TO 430
      IF (N3.GT.NMAXI) GO TO 430
      GO TO 350
C
C     COMPUTE NEW FISSION SOURCE AND EIGENVALUE, AND ITERATION ERRORS
C
  430 DO 440 J=1,NC
      FS(J)=FCS1(J)*F1P(J)+FCS2(J)*F2P(J)+FCS3(J)*F3P(J)
  440 CONTINUE
C
      DF=0.0
      ZN=0.0
      ZNP=0.0
      DO 450 J=1,NC
      DF=DF+ABS(FS(J)-FSP(J))*DX(J)
      ZNP=ZNP+FSP(J)*DX(J)
      ZN=ZN+FS(J)*DX(J)
  450 CONTINUE
C  
      IF (DFP.LT.0.0000001) DFP=1.0
      RDF=DF/DFP
      EV=EVP*ZNP/ZN
      IF (EV.LT.0.2*EVP) EV=0.2*EVP
      DK=ABS(1.0/EV-1.0/EVP)
      EVP=EV
      IF (DKP.LT.0.0000001) DKP=1.0
      RDK=DK/DKP
      EK=1.0/EV
      DO 460 J=1,NC
      FSP(J)=WS*FS(J)/ZN
  460 CONTINUE
      WRITE (2,470) NOUT,N1,N2,N3,DF,RDF,DK,RDK,EK
  470 FORMAT (4I5,5F10.6)
      WRITE (9,470) NOUT,N1,N2,N3,DF,RDF,DK,RDK,EK
      IF (NOUT.GE.NMAXO) GO TO 490
      IF (DF.GT.EPSF) GO TO 480
      IF (DK.GT.EPSK) GO TO 480
      GO TO 490
  480 NOUT=NOUT+1
      N1=1
      N2=1
      N3=1
      DFP=DF
      DKP=DK
      GO TO 190
C
C     WRITE OUT THE CONVERGED GROUP FLUXES AND FISSION SOURCE,
C     AND THE AVERAGE GROUP FLUXES FOR EACH REGION
C
  490 WRITE (2,500)
  500 FORMAT (/,7X,'X',7X,'F1(X)',7X,'F2(X)',7X,'F3(X)',7X,'FS(X)')
      X=0.5*DX(1)
      DO 520 J=1,NC
      WRITE (2,510) X,F1P(J),F2P(J),F3P(J),FSP(J)
  510 FORMAT (F8.4,4F12.7)
      IF (J.LT.NC) X=X+0.5*(DX(J)+DX(J+1))
  520 CONTINUE
C
      WRITE (2,530)
  530 FORMAT (/,2X,'REGION',8X,'F1AV',8X,'F2AV',8X,'F3AV')
      J1=1
      J2=MC(1)
      DO 560 K=1,NREG
      F1AV(K)=0.0
      F2AV(K)=0.0
      F3AV(K)=0.0
      DO 540 J=J1,J2
      F1AV(K)=F1AV(K)+F1P(J)*DX(J)/WC(K)
      F2AV(K)=F2AV(K)+F2P(J)*DX(J)/WC(K)
      F3AV(K)=F3AV(K)+F3P(J)*DX(J)/WC(K)
  540 CONTINUE
      WRITE (2,550) K,F1AV(K),F2AV(K),F3AV(K)
  550 FORMAT (I8,3F12.7)
      IF (K.EQ.NREG) GO TO 560
      J1=J2+1
      J2=J2+MC(K+1)
  560 CONTINUE
      GO TO 590
C 
  570 write(*,*) 'TOTAL NUMBER OF CELLS EXCEEDS ALLOWED DIMENSION'
      PAUSE
      GO TO 650
  580 write(*,*) 'A QUADRATURE SET OF THE ASSIGNED ORDER IS NOT ENCODED'
      PAUSE
      GO TO 650
C
C     CHECK NEUTRON BALANCE
C
  590 CUR1=0.0
      CUR2=0.0
      CUR3=0.0
      DO 600 M=1,NQ
      CUR1=CUR1+AX(M)*FIN1(M)*W(M)
      CUR2=CUR2+AX(M)*FIN2(M)*W(M)
      CUR3=CUR3+AX(M)*FIN3(M)*W(M)
  600 CONTINUE
C
      ABS1=0.0
      ABS2=0.0
      ABS3=0.0
      TFS=0.0
      DS2=0.0
      DS3=0.0
      DO 610 J=1,NC
      ABS1=ABS1+(TCS1(J)-SCS11(J))*F1P(J)*DX(J)
      ABS2=ABS2+(TCS2(J)-SCS22(J))*F2P(J)*DX(J)
      ABS3=ABS3+(TCS3(J)-SCS33(J))*F3P(J)*DX(J)
      TFS=TFS+FSP(J)*DX(J)/EK
      DS2=DS2+SCS12(J)*F1P(J)*DX(J)
      DS3=DS3+(SCS13(J)*F1P(J)+SCS23(J)*F2P(J))*DX(J)
  610 CONTINUE
C
      B1=CUR1*FLOAT(1-NB)+ABS1-TFS
      B2=CUR2*FLOAT(1-NB)+ABS2-DS2
      B3=CUR3*FLOAT(1-NB)+ABS3-DS3
      WRITE (2,620)
  620 FORMAT (/,8X,'B1',8X,'B2',8X,'B3')
      WRITE (2,630) B1,B2,B3
  630 FORMAT (3F10.6)
C
  640 write(*,*) 'CALCULATION IS DONE.PRESS CARRIAGE RETURN TO CONTINUE'
      PAUSE
  650 STOP
      END
