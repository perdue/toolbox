	PROGRAM RKIN
C
C  written by S.Manglos.  TUNL,Jan.1980.
C
C	MODIFIED BY B.C. KARP, APR 1982


c       Modified by R M Prior for Unix Fortran, May, 2000
C
C	THIS PROGRAM COMPUTES RELATIVISTIC KINEMATICS FOR TWO-BODY NUCLEAR REACTIONS
C
	IMPLICIT REAL*8 (A-H,O-Z)
	PARAMETER (AMU=931.5016, MAXANG=200)
	DIMENSION T1(20),AM(20,4),EX(20,20),NEX(20),ANGSTO(MAXANG),
	1 IA(20,4),IZ(20,4)
	INTEGER FILE
	DATA LIN/5/,LOUT/6/,FILE/3/
	CHARACTER*25 REACTN(20)
C
	COMMON AM1, AM2, AM3, AM40, AM1SQ, AM2SQ, AM3SQ, E1, EIN,
	1 P1, ELAB, P1CHK, P1CM, GAMMA, BETA, D, TCM
C
C  DATA READ IN SECTION
C
	WRITE(LOUT,1000)
	WRITE(LOUT,1002)
	WRITE(LOUT,1004)
	WRITE(LOUT,1006)
	READ(LIN,*)IFMT
	IF(IFMT.EQ.0) GO TO 10
	WRITE(LOUT,1008)
	READ(LIN,*)THICK1,THICK2
10	CONTINUE
	WRITE(LOUT,1010)
	READ(LIN,*)NANG
	IF(NANG.LE.0) THEN
	WRITE(LOUT,1012)
	READ(LIN,*) ANGSTO(1),XANGF,ANGINC
	NANG = (XANGF - ANGSTO(1))/ANGINC + 1.9
	IF(NANG.GT.MAXANG)NANG = MAXANG
	DO 20 I=2,NANG
	ANGSTO(I) = ANGSTO(I-1) + ANGINC
20	CONTINUE
	ELSE
	IF(NANG.GT.MAXANG) NANG = MAXANG
	WRITE(LOUT,1014)NANG
	READ(LIN,*)(ANGSTO(I),I=1,NANG)
	ENDIF
	WRITE(LOUT,1016)
	READ(LIN,*) NE
	WRITE(LOUT,1018)
	READ(LIN,*) (T1(I),I=1,NE)
	WRITE(LOUT,1020)
	READ(LIN,*) NR
	IF(NR.GT.20) NR = 20
	IF(NR.LE.0) STOP
	DO 40 I=1,NR
30	CALL DECIFR(IA,IZ,I,REACTN(I))
C	CHECK FOR CONSISTENCY
	IACHK = IA(I,1) + IA(I,2) - IA(I,3) - IA(I,4)
	IZCHK = IZ(I,1) + IZ(I,2) - IZ(I,3) - IZ(I,4)
	IF((IACHK.NE.0).OR.(IZCHK.NE.0)) THEN
	WRITE(LOUT,1022)
	GO TO 30
	ENDIF
	WRITE(LOUT,1024)
	READ(LIN,*) NEX(I)
	IF(NEX(I) .GT. 20) THEN
		NEX(I) = 20
		WRITE(LOUT,1025)
1025		FORMAT(' Maximum of 20 excitation energies per reaction')
	ENDIF
	WRITE(LOUT,1026)
	READ(LIN,*) (EX(I,J),J=1,NEX(I))
40	CONTINUE
	WRITE(LOUT,1028)
C  END DATA READ-IN
C
	OPEN(UNIT=FILE,TYPE='UNKNOWN',NAME='rkin.out')
C
C	GET MASSES FROM MASS TABLE
C
	DO 60 I=1,NR
	DO 60 J=1,4
	EXM = EMASS1(IA(I,J),IZ(I,J))

Ctest	write(*,*)exm


	AM(I,J) = FLOAT(IA(I,J)) + EXM/AMU

Ctest	write(*,*) am(i,j)

60	CONTINUE
C
	IF(IFMT.NE.0) GO TO 300
C
C	STANDARD LOOP
C
	DO 200 I=1,NE
	DO 200 J=1,NR
	Q0 = AMU*(AM(J,1) + AM(J,2) - AM(J,3) - AM(J,4))
	CALL MASSET(T1(I), AM(J,1), AM(J,2), AM(J,3), AM(J,4))
	DO 180 K=1,NEX(J)
	WRITE(FILE,1030) REACTN(J)
	WRITE(FILE,1032) (L,L=1,4)
	Q = Q0 - EX(J,K)
	WRITE(FILE,1034) (AM(J,L),L=1,4),EX(J,K),T1(I),Q,TCM
	WRITE(FILE,1036)
	DO 180 L=1,NANG
	ANGLAB = ANGSTO(L)
	CALL SRKIN(EX(J,K), ANGLAB,ANG3P,T3P,SHIFTP,
	1 ANGL4P,T4P,RATIOP,QP,ANG3M,T3M,SHIFTM,ANGL4M,
	2 T4M,RATIOM,QM)
	IF(ANG3P.EQ.-1.0) GO TO 151
	IF(ANG3P.EQ.-2.0) GO TO 152
	WRITE(FILE,1038) ANGLAB,ANG3P,T3P,ANGL4P,T4P,RATIOP
	GO TO 180
151	WRITE(FILE,1040)
	GO TO 200
152	WRITE(FILE,1042)ANGLAB
180	CONTINUE
200	CONTINUE
	STOP
C
C	ALTERNATE LOOP
C
300	DO 400 I=1,NE
	DO 400 J=1,NR
	RANGE = 0.0
	DELE1 = 0.0
	DELE2 = 0.0
	Q0 = AMU*(AM(J,1) + AM(J,2) - AM(J,3) - AM(J,4))
	CALL MASSET(T1(I), AM(J,1), AM(J,2), AM(J,3), AM(J,4))
	WRITE(FILE,1050) REACTN(J),T1(I)
	WRITE(FILE,1052) (L,L=1,4)
	WRITE(FILE,1054)(AM(J,L),L=1,4),T1(I),Q0,TCM
	IF((THICK1.GT.0.).OR.(THICK2.GT.0.)) WRITE(FILE,1056)THICK1,
	1 THICK2
	WRITE(FILE,1058)
	WRITE(FILE,1059)
	DO 400 K=1,NANG
	ANGLAB = ANGSTO(K)
	WRITE(FILE,1060)ANGLAB
	DO 380 L=1,NEX(J)
	Q = Q0 - EX(J,L)
	CALL SRKIN(EX(J,L), ANGLAB,ANG3P,T3P,SHIFTP
	1 ,ANGL4P,T4P,RATIOP,QP,ANG3M,T3M,SHIFTM,ANGL4M,
	2 T4M,RATIOM,QM)
	IF(ANG3P.EQ.-1.) GO TO 351
	IF(ANG3P.EQ.-2.) GO TO 352
C  CALCULATE RANGE AND ENERGY LOSS
	IF((IZ(J,3).EQ.0).OR.(T3P.LE.0.)) GO TO 340
	Z = IZ(J,3)
	CALL SILCON(T3P,AM(J,3),Z,0,RANGE,IERR)
	IF(THICK1.GT.0) THEN
	IF(RANGE.LT.THICK1) THEN
	DELE1 = T3P
	ELSE
	CALL SILCON(T3P,AM(J,3),Z,THICK1,DELE1,IERR)
	ENDIF
	ENDIF
	IF(THICK2.GT.0.) THEN
	IF(RANGE.LT.THICK2) THEN
	DELE2 = T3P
	ELSE
	CALL SILCON(T3P,AM(J,3),Z,THICK2,DELE2,IERR)
	ENDIF
	ENDIF
340	WRITE(FILE,1062)EX(J,L),Q,ANG3P,T3P,SHIFTP,ANGL4P,T4P,RATIOP,
	1 QP,RANGE,DELE1,DELE2
C  CHECK FOR SECOND SOLUTION
	IF(ANG3M.EQ.0.) GO TO 380
	IF(IZ(J,3).EQ.0) GO TO 345
	CALL SILCON(T3M,AM(J,3),Z,0,RANGE,IERR)
	IF(THICK1.GT.0.) THEN
	IF(RANGE.LT.THICK1) THEN
	DELE1 = T3M
	ELSE
	CALL SILCON(T3M,AM(J,3),Z,THICK1,DELE1,IERR)
	ENDIF
	ENDIF
	IF(THICK2.GT.0.) THEN
	IF(RANGE.LT.THICK2) THEN
	DELE2 = T3M
	ELSE
	CALL SILCON(T3M,AM(J,3),Z,THICK2,DELE2,IERR)
	ENDIF
	ENDIF
345	WRITE(FILE,1064)ANG3M,T3M,SHIFTM,ANGL4M,T4M,RATIOM,QM,RANGE,
	1 DELE1,DELE2
	GO TO 380
C
351	WRITE(FILE,1041) EX(J,L),Q
	GO TO 380
352	WRITE(FILE,1042)ANGLAB
	GO TO 400
380	CONTINUE
400	CONTINUE
	STOP
C
1000	FORMAT(/,'  Program RKIN')
1002	FORMAT(' Separate variables using "," or carriage return')
1004	FORMAT(' Output will appear in file rkin.out')
1006	FORMAT( ' Do you want the standard output',
	1 ' (type 0),',/,' or do you prefer the alternate ouput form',
	2 ' (type 1)? ',$)
1008	FORMAT(' Type in two delta-E thicknesses in microns.  Use 0.0 ',
	1 'if one or both',/,
	2 ' energy-deposition calculations are not wanted. ',$)
1010	FORMAT( ' Type 0(Zero) to input initial, final, and increment',
	1 ' lab angles OR',/,' type in the number of lab angles to be typed in ',
	2 '(200 max.): ',$)
1012	FORMAT(' Type in initial angle, final angle, angle increment: ',$)
1014	FORMAT(' Type in the ',I3, ' lab angles in degrees: ',$)
1016	FORMAT(' Type in number of energies (integer): ',$)
1018	FORMAT(' Type in energies in MeV: ',$)
1020	FORMAT(' Type in number of reactions (targets): ',$)
1022	FORMAT(' REACTION CODE IS INCONSISTENT.  TYPE IT AGAIN. ',$)
1024	FORMAT(' Type in number of final states for this reaction: ',$)
1026	FORMAT(' Type in excitation energies of residual nucleus in MeV: ',$)
1028	FORMAT(' END DATA INPUT')
1030	FORMAT(/,' Reaction:',4X,A25,/)
1032	FORMAT(4(5X,'AM(',I1,')'),5X,'EX',6X,'T1',7X,'Q',6X,'ECM')
1034	FORMAT(1X,4F10.6,4F8.3)
1036	FORMAT(/,3X,'LAB ANGLE  CM ANGLE    T3',5X,
	1 'LAB ANG4     T4    SIGCM/SIGLAB')
1038	FORMAT(2F10.3,3F10.3,f11.4)
1040	FORMAT('  NO REACTION AT THIS ENERGY FOR THIS EXCITATION')
1041	FORMAT(5X,2F10.3,
	1 '  NO REACTION AT THIS ENERGY FOR THIS EXCITATION')
1042	FORMAT(5X,1F10.4,'   NO REACTION FOR THIS ANGLE')
1050	FORMAT(1H1,5X,A25,2X,F5.1,' MeV',/)
1052	FORMAT(4(5X,'AM(',I1,')'),7X,'T1',8X,'Q0',7X,'ECM')
1054	FORMAT(1X,4F10.5,4F10.3)
1056	FORMAT(/,84X,'DELTA-E THICKNESSES:',2F10.1)
1058	FORMAT(/,7X,'EXCITATION    Q',9X,'CM',7X,'T3',7X,'dE/dAng',5X,
	1 'LAB',7X,'T4',5X,'SIGCM/',5X,'q CM',4X,'RANGE',6X,
	2 'DEL E1    DEL E2')
1059	FORMAT(10X,2('(MeV)',4X),2X,'ANGLE',4X,'(MeV)',
	2 4X,'(keV/deg)',4X,'ANG4',5X,
	3 '(MeV)',3X,'SIGLAB',3X,'(MeV/C)  (MICRONS)',4X,
	4 2('(MeV)',5X),/,2X,125('-'))
1060	FORMAT(1H0,4X,11HLAB ANGLE =,F6.1)
1062	FORMAT(5X,2F10.3,F10.2,F10.3,F10.1,F10.2,F10.3,F10.5,F10.3,F8.0,
	1 2X,2F10.3)
1064	FORMAT(5X,'SECOND SOLN.',8X,F10.2,F10.3,F10.1,F10.2,F10.3,F10.5,
	1 F10.3,F8.0,2X,2F10.3)
	END


	SUBROUTINE MASSET(T1,TM1,TM2,TM3,TM4)
C
C	THIS SUBROUTINE SETS UP THE MASS AND ENERGY VALUES THAT ARE
C	INDEPENDENT OF THE SCATTERING ANGLE AND THE EXCITATION OF
C	THE RESIDUAL NUCLEUS.
C
C	B.C. KARP, 8/82
C
	IMPLICIT REAL*8 (A-H, O-Z)
	COMMON AM1, AM2, AM3, AM40, AM1SQ, AM2SQ, AM3SQ, E1, EIN,
	1 P1, ELAB, P1CHK, P1CM, GAMMA, BETA, D, TCM
	PARAMETER (AMU=931.5016)
C
	AM1=TM1*AMU
	AM2=TM2*AMU
	AM3=TM3*AMU
	AM40=TM4*AMU
	AM1SQ = AM1*AM1
	AM2SQ = AM2*AM2
	AM3SQ = AM3*AM3
C
	E1=T1 + AM1
	EIN = DSQRT(AM1SQ + AM2SQ + 2.*AM2*E1)
	TCM = EIN - AM1 - AM2
	ELAB = E1 + AM2
	P1 = DSQRT(E1*E1 - AM1SQ)
	P1CHK = 1.E-4*P1
	P1CM = P1*AM2/EIN
	GAMMA=ELAB/EIN
	BETA=P1/ELAB
C
	D=(P1*AM3)
	D = D*D
	RETURN
	END


	SUBROUTINE SRKIN(EX,ANGLAB,ANG3P,T3P,SHIFTP, ANGL4P,
	1 T4P,RATIOP,QP,ANG3M,T3M, ANGL4M, T4M,SHIFTM,RATIOM,QM)
C
C	THIS IS A DOUBLE PRECISION SUBROUTINE TO DO RELATIVISTIC KINEMATICS
C	INPUT AND OUTPUT ENERGIES IN MEV, MASSES IN AMU, ANGLES IN DEGREES
C	OUTPUTS BOTH SOLUTIONS.  IF ONLY ONE SOLUTION IS ALLOWED, THE OTHER
C	IS SET TO 0.0
C
C	IF THE REACTION IS NOT ALLOWED BECAUSE ENERGY IS BELOW THE
C	THRESHOLD, SUBROUTINE SETS ANG3P=-1.0
C	IF THE REACTION IS NOT ALLOWED FOR A GIVEN ANGLE, ANG3P=-2.0
C	AM1=PROJECTILE MASS, AM2=TARGET MASS, AM3=SCATTERED PARTICLE
C	MASS, AM4=RESIDUAL NUCLEUS MASS
C
C	MODIFIED BY B.C. KARP     5/82
C
	IMPLICIT REAL*8 (A-H,O-Z)
	PARAMETER (DTR=0.01745329, DTRKEV=-17.45329, PI=3.1415927)
	COMMON AM1, AM2, AM3, AM40, AM1SQ, AM2SQ, AM3SQ, E1, EIN,
	1 P1, ELAB, P1CHK, P1CM, GAMMA, BETA, D, TCM
C
	AM4 = AM40 + EX
	AM4SQ = AM4*AM4
C
C	ENERGY THRESHOLD CHECK
C
	EOUT=AM3 + AM4
	IF (EIN.LT.EOUT) GO TO 20
C
	ANGLB=ANGLAB*DTR
C  CHECK FOR LAB ANGLE OF 0 OR 180 DEGREES
	IZERO=0
	IF(DABS(ANGLAB).LT.0.01) THEN
	ANGLB=0.0001D0
	IZERO=1
	ENDIF
	IF(DABS(ANGLB-PI).LT.0.00001) THEN
	ANGLB = 3.14159
	IZERO = -1
	ENDIF
C
	COSLAB = DCOS(ANGLB)
	SINLAB = DSIN(ANGLB)
	SIN2LB = DSIN(2.*ANGLB)
C
	X=(AM2*E1 + (AM1SQ + AM2SQ - AM3SQ -
	1 AM4SQ)/2.0)**2 - AM3SQ*AM4SQ -
	2 D*(SINLAB)**2
	IF(X.LT.0.0) GO TO 80
	X=DSQRT(X)
	E3A=ELAB*(AM2*E1 + (AM1SQ + AM2SQ + AM3SQ
	1 - AM4SQ)/2.0)
	E3B=P1*COSLAB*X
	E3C = ELAB**2 - (P1*COSLAB)**2
	SUMAB = E3A + E3B
	DIFAB = E3A - E3B
	E3P = SUMAB/E3C
	E3M = DIFAB/E3C
	T3P=E3P - AM3
	T3M=E3M - AM3
	A=EIN*SINLAB
C
C	CALCULATE THE KINEMATIC SHIFT
C
	DXDT = -0.5*D*SIN2LB/X
	DE3BDT = P1*(COSLAB*DXDT - SINLAB*X)
	DE3CDT = P1*P1*SIN2LB
C
	FACTOR1 = DE3BDT/E3C
	FACTOR2 = DE3CDT/(E3C*E3C)
	SHIFTP = FACTOR1 - FACTOR2*SUMAB
	SHIFTM = -FACTOR1 - FACT0R2*DIFAB
C
C	CONVERT TO KEV/DEGREE
C
	SHIFTP = SHIFTP*DTRKEV
	SHIFTM = SHIFTM*DTRKEV
C
	IF(E3P.LT.AM3) THEN
	IF(E3M.LT.AM3) GO TO 80
	T3P = T3M
	E3P = E3M
	SHIFTP = SHIFTM
	T3M = 0.
	E3M = 0.
	SHIFTM = 0.
	ENDIF
	P3P=DSQRT(E3P*E3P - AM3SQ)
	E4P=E1 + AM2 - E3P
	T4P=E4P - AM4
C  MOMENTUM CONSERVATION CHECK
	P4P=DSQRT(E4P*E4P - AM4SQ)
	DIFFA=(P1 - P3P*COSLAB)/P4P
	CHECKA=DABS(DIFFA)
	IF (CHECKA.GT.1.01) GO TO (80,40,40),IZERO+2
	IF(DIFFA.GE.1.0) THEN
	THETA1 = 0.
	ELSE
	IF(DIFFA.LE.-1.0) THEN
	THETA1 = PI
	ELSE
	THETA1 = DACOS(DIFFA)
	ENDIF
	ENDIF
	DIFFB=P3P*SINLAB/P4P
	CHECKB=DABS(DIFFB)
	IF(CHECKB.GT.1.01) GO TO 40
	IF(DIFFB.GE.1.0) THEN
	THETA2 = 0.5*PI
	ELSE
	THETA2 = DASIN(DIFFB)
	ENDIF
	CHECKC=DABS(THETA1-THETA2)
	CHECKD=DABS(THETA1-PI+THETA2)
	IF(CHECKC.LT.0.0001 .OR. CHECKD.LT.0.0001 .OR.
	1 IZERO.EQ.1) GO TO 100
	GO TO 40
100   CONTINUE
C
	BP=(E1 + AM2)*(COSLAB - P1*E3P/(E1 + AM2)/P3P)
	ANG3P = DATAN2(A,BP)
C
C	CALCULATE THE MOMENTUM TRANSFER
C
	E3CM = (EIN*EIN + AM3SQ - AM4SQ)/(2.*EIN)
	P3CM = DSQRT(E3CM*E3CM - AM3SQ)
	P3CMX = P3CM*DSIN(ANG3P)
	P3CMZ = P3CM*DCOS(ANG3P)
	DELDZ = P3CMZ - P1CM
	QP = DSQRT(P3CMX*P3CMX + DELPZ*DELPZ)
C
C	LAB ANGLE FOR FOURTH PARTICLE
C
	ANGL4P = SINLAB*(P3P/P4P)
	IF(ANGL4P.GE.1.0) THEN
	ANGL4P = 0.5*PI
	ELSE
	ANGL4P = DASIN(ANGL4P)
	ENDIF
C
C	DETERMINE QUADRANT OF RECOIL
C
	I = 0
	DIFFA = P1 - P3P*COSLAB 
	P4PZ = P4P*DCOS(ANGL4P)
110	CHECKA = DABS(DIFFA - P4PZ)
	IF(CHECKA.LT.P1CHK) GO TO 120
	IF(I.EQ.1) GO TO 40
	I = 1
	ANGL4P = PI - ANGL4P
	P4PZ = -P4PZ
	GO TO 110
120	CONTINUE
C
	Z=((DSIN(ANG3P))**3/SINLAB)*(GAMMA + 1.0/(DTAN(ANGLB)*
	1 DTAN(ANG3P)))
	R=BETA*GAMMA*(DSIN(ANG3P))**3/(SINLAB)**2
	Y=(1.0/P3P - E3P**2/P3P**3)*(-SINLAB/E3C)*(+P1*X*(1.0 +
	1 D*(COSLAB)**2/X**2) + 2.0*E3P*P1**2*COSLAB)
	RATIOP=1.0/(Z + R*Y)
	GO TO 50
40	ANG3P=0.0
	T3P=0.0
	SHIFTP = 0.
	T4P=0.0
	RATIOP=0.0
50	IF(E3M.LT.AM3) GO TO 60
	P3M=DSQRT(E3M*E3M - AM3SQ)
	E4M=E1 + AM2 - E3M
	T4M=E4M - AM4
C  MOMENTUM CONSERVATION CHECK
	P4M=DSQRT(E4M*E4M - AM4SQ)
	IF(P4M .EQ. 0.) GO TO 80
	DIFFA=(P1 - P3M*COSLAB)/P4M
	CHECKA=DABS(DIFFA)
	IF(CHECKA.GT.1.01) GO TO 60
	IF(DIFFA.GE.1.0) THEN
	THETA1 = 0.
	ELSE
	IF(DIFFA.LE.-1.0) THEN
	THETA1 = PI
	ELSE
	THETA1 = DACOS(DIFFA)
	ENDIF
	ENDIF
	DIFFB=P3M*SINLAB/P4M
	CHECKB=DABS(DIFFB)
	IF(CHECKB.GT.1.01) GO TO 60
	IF(DIFFB.GE.1.0) THEN
	THETA2 = 0.5*PI
	ELSE
	THEAT2 = DASIN(DIFFB)
	ENDIF
	CHECKC=DABS(THETA1-THETA2)
	CHECKD=DABS(THETA1-PI+THETA2)
	IF(CHECKC.LT.0.0001 .OR. CHECKD.LT.0.0001)  GO TO 101
	GO TO 60
101   CONTINUE
C
	BP=(E1 + AM2)*(COSLAB - P1*E3M/(E1 + AM2)/P3M)
	ANG3M=DATAN2(A,BP)
C
C	CALCULATE THE MOMENTUM TRANSFER
C
	P3CMX = P3CM*DSIN(ANG3M)
	P3CMZ = P3CM*DCOS(ANG3M)
	DELPZ = P3CMZ - P1CM
	QM = DSQRT(P3CMX*P3CMX + DELPZ*DELPZ)
C
C	LAB ANGLE FOR FOURTH PARTICLE
C
	ANGL4M = SINLAB * (P3M/P4M)
	IF(ANGL4M.GE.1.0) THEN
	ANGL4M = 0.5*PI
	ELSE
	ANGL4M = DASIN(ANGL4M)
	ENDIF
C
C	DETERMINE QUADRANT OF RECOIL
C
	I = 0
	DIFFA = P1 - P3M*COSLAB
	P4MZ = P4M*DCOS(ANGL4M)
130	CHECKA = DABS(DIFFA - P4MZ)
	IF(CHECKA.LT.P1CHK) GO TO 140
	IF(I.EQ.1) GO TO 60
	I = 1
	ANGL4P = PI - ANGL4P
	P4MZ = -P4MZ
	GO TO 130
140	CONTINUE
C
	CHECKE=DABS(ANG3P-ANG3M)
	IF(CHECKE.LT.0.0001) GO TO 60
	Z=((DSIN(ANG3M))**3/SINLAB)*(GAMMA + 1.0/(DTAN(ANGLB)
	1 *DTAN(ANG3M)))
	R=BETA*GAMMA*(DSIN(ANG3M))**3/(SINLAB)**2
	Y=(1.0/P3M - E3M**2/P3M**3)*(-SINLAB/E3C)*(-P1*X*(1.0 +
	1 D*(COSLAB)**2/X**2) + 2.0*E3M*P1**2*COSLAB)
	RATIOM=1.0/(Z + R*Y)
	GO TO 70
60	ANG3M=0.0
	ANGL4M = 0.
	T3M=0.0
	SHIFTM = 0.
	T4M=0.0
	RATIOM=0.0
	GO TO 70
80	ANG3P=-2.0
	GO TO 200
20	ANG3P=-1.0
	GO TO 200
C	
C	CONVERT RADIANS TO DEGREES
C
70    CONTINUE
	ANG3P=ANG3P/DTR
	ANG3M=ANG3M/DTR
	ANGL4P = ANGL4P/DTR
	ANGLM = ANGL4M/DTR
C
	IF(IZERO) 180,200,160
160	ANG3P=0.0D0
	IF(DABS(ANGL4P-180.).LT.0.5) THEN
		ANGL4P = 180.
	ELSE
		ANGL4P = 0.
	ENDIF
	ANG3M=0.0D0
	ANGL4M = 0.
	GO TO 200
180	ANG3P = 180.
	ANGL4P = 0.
	ANG3M = 0.
	ANGL4M = 0.
200	CONTINUE
	RETURN
	END


	SUBROUTINE DECIFR(IA,IZ,LN,REACTN)
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C
C                 Subroutine DECIFR
C
C         This routine takes a reaction of the form :
C
C                      27AL(P,P)27AL
C
C    and returns the A and Z of the target, incident particle,
C    scattered particle and residual nucleus.
C
C         The reaction must be in upper case. Acceptable abbreviations
C    are :
C
C                      P = proton
C                      N = neutron
C                      D = deuteron
C                      T = triton
C                      A = alpha
C                      G = gamma
C                      E = electron
C
C         DECIFR calls AZ which does the actual determination of A and Z.
C
C    Major Variables :
C
C                      REACT,RCTIN - Character form of the input reaction.
C                      TRGT - Stores reaction target in character form.
C                      PROJ -   "        "    projectile   "        "
C                      SCAT -   "        "    scattered particle    "
C                      RES  -   "        "    residual nucleus      "
C
C           Written by C. Torre ................3/82
C	    Modified by B.C. Karp ..............5/82
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C
	CHARACTER REACT(25),TRGT(5),PROJ(5),SCAT(5),RES(5),
	1  COMMA,BLANK,PRNTH1,PRNTH2
	INTEGER IA(20,4),IZ(20,4)
	PARAMETER (PRNTH1='(', PRNTH2=')', COMMA=',', BLANK=' ')
	CHARACTER*25 RCTIN,REACTN
	EQUIVALENCE(RCTIN,REACT)
	DATA LIN/5/,LOUT/6/
C
C       Prompt user for reaction.
C
	WRITE(LOUT,5)
5   	FORMAT (' Input reaction in form -  27Al(p,p)27Al  ',
	1 /,' (Residual nucleus may be omitted.) ',$)
10	  READ(LIN,15) RCTIN
15   	FORMAT (A25)
	REACTN=RCTIN
C
C       Extract the target. NTRGT is the number of characters.
C
1       N=0
	DO 100 I=1,25
	   N=N+1
	   IF (REACT(I) .NE. PRNTH1) THEN
	   IF(N.GT.5) GO TO 2000
	      TRGT(N)=REACT(I)
	   ELSE
	      NTRGT=N-1
C
C        Extract the projectile.
C
	      N=0
	      DO 80 J=I+1,25
	         N=N+1
	         IF (REACT(J) .NE. COMMA) THEN
		    IF(N.GT.5) GO TO 2000
	            PROJ(N)=REACT(J)
	         ELSE
	            NPROJ=N-1
C
C        Extract the scattered particle.
C
	            N=0
	            DO 60 K=J+1,25
	               N=N+1
	               IF (REACT(K) .NE. PRNTH2) THEN
			  IF(N.GT.5) GO TO 2000
	                  SCAT(N)=REACT(K)
	               ELSE
	                  NSCAT=N-1
C
C        Extract the residual nucleus.
C
	                  N=0
	                  DO 40 L=K+1,25
	                     N=N+1
	                     IF (REACT(L) .NE. BLANK) THEN
				IF(N.GT.5) GO TO 2000
	                        RES(N)=REACT(L)
	                     ELSE
	                        NRES=N-1
	                        GO TO 1000
	                     END IF
40                        CONTINUE
	               END IF
60                  CONTINUE
	         END IF
80            CONTINUE
	   END IF
100     CONTINUE
1000    CONTINUE
C
C        Call AZ to convert the character data to A and Z.
C
	CALL AZ(TRGT,NTRGT,IA(LN,2),IZ(LN,2),IERR)
	IF(IERR.NE.0) GO TO 2000
	CALL AZ(PROJ,NPROJ,IA(LN,1),IZ(LN,1),IERR)
	IF(IERR.NE.0) GO TO 2000
	CALL AZ(SCAT,NSCAT,IA(LN,3),IZ(LN,3),IERR)
	IF(IERR.NE.0) GO TO 2000
	IF(NRES.GT.0) THEN
	CALL AZ(RES,NRES,IA(LN,4),IZ(LN,4),IERR)
	IF(IERR.NE.0) GO TO 2000
	ELSE
	IA(LN,4) = IA(LN,1) + IA(LN,2) - IA(LN,3)
	IZ(LN,4) = IZ(LN,1) + IZ(LN,2) - IZ(LN,3)
	ENDIF
	RETURN
C
C	ERROR HANDLING
C
2000	WRITE(6,2001)
	GO TO 10
C
2001	FORMAT(' ERROR--Retype reaction code: ',$)
	 END
C
C
C


	SUBROUTINE AZ(NAME,NCHAR,A,Z,IERR)
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C                      Subroutine AZ
C
C
C	Subroutine to convert character representation of a nucleus or
C	particle into the appropriate A and Z.
C
C 
C       Major variables :
C
C                    NAME  -  Character representation of nucleus.
C                    EL    -  Character rep. of element
C                    ACHAR -     "       "   "     A.
C                    EL1   -  List of 1 character elements.
C                    EL2   -   "    " 2     "        "
C                    PRTCL -  List of particle abbreviations.
C                    ZEL1  -  Z corresponding to EL1.
C                    ZEL2  -  "       "       "  EL2.
C                    ZAP   -  A and Z for PRTCL.
C		ERROR CODES
C		0 - NO ERROR
C		1 - ELEMENT NOT FOUND
C		2 - Z > A
C
C            Notes :
C
C                AZ calls UPPER which converts character data to uppercase.
C
C                If no A is given via the calling program, the
C             routine assumes that a partical abbrev. is being
C             used
C
C                If the passed element is not identified, the routine
C             prints a message and assigns Z=0.
C
C                If Z > A the routine prints a message.
C
C
C       Written by.............C.Torre 3/82
C	Modified by............B.C. Karp 5/82
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
	CHARACTER*2 EL,PRTCL(7),EL2(89),UPEL
	CHARACTER*3 ACHAR
	CHARACTER*5 NAME
	CHARACTER INTGR(10),EL1(14)
	INTEGER A,Z,ZEL1(14),ZEL2(89),ZAP(7,2)
	LOGICAL MATCH
C
C		Data initialization 
C
	DATA INTGR /'1','2','3','4','5','6','7','8','9','0'/
	DATA LIN/5/,LOUT/6/
C
C		Initialize values of character reps of elements and
C		values of Z's.
C
	DATA PRTCL /'P ','N ','D ','T ','A ','G ','E '/
	DATA ZAP /1,0,1,1,2,0,-1,1,1,2,3,4,0,-1/
	DATA EL1/'H','B','C','N','O','F','P','S','K','V',
	1  'Y','I','W','U'/
	DATA EL2 /'HE','LI','BE','NE','NA','MG','AL','SI','CL'
	1  ,'AR','CA','SC','TI','CR','MN','FE','CO','NI','CU',
	2  'ZN','GA','GE','AS','SE','BR','KR','RB','SR','ZR',
	3  'NB','MO','TC','RU','RH','PD','AG','CD','IN','SN',
	4  'SB','TE','XE','CS','BA','LA','HF','PR','ND','PM',
	5  'SM','EU','GD','TB','DY','HO','ER','TM','YB','LU',
	6  'HF','TA','RF','OS','IR','PT','AU','HG','TL','PB',
	7  'BI','PO','AT','RN','FR','RA','AC','TH','PA','NP',
	8  'PU','AM','CM','BK','CF','ES','FM','MD','NO','LW'/
C
	DATA ZEL1 /1,5,6,7,8,9,15,16,19,23,39,53,74,92/
	DATA ZEL2 /2,3,4,10,11,12,13,14,17,18,20,21,22,24,25,26,
	1  27,28,29,30,31,32,33,34,35,36,37,38,40,41,42,43,44,45,
	2  46,47,48,49,50,51,52,54,55,56,57,58,59,60,61,62,63,64,
	3  65,66,67,68,69,70,71,72,73,75,76,77,78,79,80,81,82,83,
	4  84,85,86,87,88,89,90,91,93,94,95,96,97,98,99,100,101,
	5  102,103/
C
C       Extract A in character form.
C
	N=0
	IERR = 0
	DO 200 I=1,NCHAR
	   N=N+1
	   DO 180 J=1,10
	      IF (NAME(I:I) .EQ. INTGR(J)) THEN
	         GO TO 200
	      END IF
180        CONTINUE
	   GO TO 210
200     CONTINUE
210     NA=N-1
	NEL=NCHAR-NA
C
	ACHAR = NAME(:NA)
C
C       Extract element.
C
	EL = NAME(NA+1:NA+NEL)
C
C       Convert EL to upper case.
C
	CALL UPPER(EL,NEL,UPEL)
C
	IF (NA .NE. 0 ) READ (UNIT=ACHAR,FMT=220) A
220   	FORMAT (I3)
	MATCH=.FALSE.
3000    IF (NA .EQ. 0) THEN
C
C         If no A given, find element from list of abbreviations.
C
C
C          MATCH indicates the element has been identified.
C
	   DO 2200 I=1,7
	      IF (UPEL .EQ. PRTCL(I)) THEN
	         MATCH=.TRUE.
	         Z=ZAP(I,1)
	         A=ZAP(I,2)
	         GO TO 4000
	      END IF
2200       CONTINUE
	END IF
C
C      Search EL1.
C
	IF (NEL .EQ. 1) THEN
	   DO 2300 I=1,14
	      IF (UPEL .EQ. EL1(I)) THEN
	         MATCH=.TRUE.
	         Z=ZEL1(I)
	         GO TO 4000
	      END IF
2300       CONTINUE
	ELSE
C
C         Search EL2
C
	   DO 2400 I=1,89
	      IF (UPEL .EQ. EL2(I)) THEN
	         MATCH=.TRUE.
	         Z=ZEL2(I)
	         GO TO 4000
	      END IF
2400       CONTINUE
	END IF
4000    CONTINUE
C
C       Error messages.
C
	IF (.NOT. MATCH) THEN
	WRITE(LOUT,4500) NAME
	IERR = 1
	GO TO 5000
	ENDIF
4500  	FORMAT (/,' Element ',A5,' not found.',/)
C
	IF (Z .GT. A) THEN
	WRITE(LOUT,4600) Z,A,NAME
	IERR = 2
	ENDIF
5000	CONTINUE
4600  	FORMAT (/,' Z = ',I3,' is greater than  A = ',I3,
	1  /,' in element ',A5,'.')
	RETURN
	END
C
C


	SUBROUTINE UPPER(WORD,NCHAR,UPEL)
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C                         Subroutine UPPER
C
C       Upper changes WORD into upper case and returns the upper case
C    version via UPEL.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
	CHARACTER*2 UPEL,WORD
	CHARACTER UP(26),DN(26)
	DATA UP/'A','B','C','D','E','F','G','H','I','J','K','L',
	1  'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'/
	DATA DN/'a','b','c','d','e','f','g','h','i','j','k','l',
	1  'm','n','o','p','q','r','s','t','u','v','w','x','y','z'/
	UPEL=WORD
	DO 100 I=1,NCHAR
	   DO 90 J=1,26
	      IF (WORD(I:I) .EQ. DN(J)) THEN
	         UPEL(I:I)=UP(J)
	         GO TO 100
	      END IF
90         CONTINUE
100     CONTINUE
	RETURN
	END
	SUBROUTINE SILCON (E,AI,ZI,TT,ELOSS,IERROR)
C	                                                                *
C	THIS ROUTINE CALCULATES THE RANGE AND ENERGY LOSS FOR CHARGED
C	PARTICLES IN SILICON.  IT USES THE
C     RELATIVISTICALLY CORRECT BETHE FORMULA, DIVIDING THE TARGET UP
C     INTO REGIONS OF SMALL ENERGY LOSS.     REFERENCES FOR EQUATION -
C	  BETHE AND ASHKINS ARTICLE IN SEGRE S NUCL. PHYSICS BOOK.      *
C	  S. M. SELTZER AND M. J. BERGER, ENERGY-LOSS STRAGGLING OF     *
C	    PROTONS AND MESONS0 TABULATION OF THE VAVILOV DISTRIBUTION, *
C	    NAS-NRC PUBL. 1133 (NUCLEAR SCIENCE SERIES  REPORT NO. 39,  *
C	    STUDIES IN PENETRATION OF CHARGED PARTICLES IN MATTER),     *
C	    P. 187 (1964).                                              *
C	  J. F. JANNI, CALCULATIONS OF ENERGY LOSS, RANGE, PATHLENGTH,  *
C	    STRAGGLING, MULTIPLE SCATTERING,  (ETC.),TECHNICAL REPORT   *
C	    NO.1AFWL-TR-65-150 (1966).    AVAILABLE FROM U. S. FEDERAL  *
C	    CLEARINGHOUSE.                                              *
C	                                                                *
C	  INPUT PARAMETERS ARE  INCIDENT ENERGY (MEV), ATOMIC NO. OF
C	  INCIDENT PARTICLE (AMU), CHARGE OF INCIDENT PARTICLE, ATOMIC  *
C	  NO. OF TARGET (AMU), CHARGE OF TARGET, TARGET THICKNESS       *
C	  (MILLIGRAMS PER SQUARE CM.), TARGET IONIZATION POTL. (EV),    *
C	  SHELL CORRECTION FACTOR, AND DENSITY EFFECT CORRECTION TERM.  *
C
CT *	 USES FUNCTION EMASS1 TO RETURN CURRENT VALUE OF ELECTRON REST
CT *     MASS AND AMU IN MeV.
CT
CT *	 ERROR RETURN CODES.
CT *		IERROR=0 NORMAL CALCULATION
CT *		IERROR=1 BEAM STOPPED
CT *		IERROR=2 POSSIBLY NOT A VALID CALCULATION
CT *		IERROR=3 NEGATIVE STOPPING POWER
CT *
C	                                                                *
C	MODIFIED BY B.C. KARP;
C	BASED ON THE ENLOSS SUBROUTINE OF J.F. WILKERSON
C
	PARAMETER(DENSTY=2.33E-04, ZT=14., AT=28.086, POTL=1.70E-04,
	1 C=0.0, DELTA=0.0, AMU=931.5016)
	 DATA LOUT/6/
	IERROR=0
C
C  CHOOSE RANGE OR ENERGY LOSS CALCULATION
	IF(TT.GT.0.) THEN
	THICK = TT*DENSTY
	ELSE
C  CHOOSE dx FOR RANGE CALCULATION
	IF(E.GT.10.) THEN
	IF(ZI.EQ.1.) THEN
	INC = 10.
	ELSE
	INC = 5.0
	ENDIF
	ELSE
	IF(ZI.EQ.1.) THEN
	INC = 5.0
	ELSE
	INC = 1.
	ENDIF
	ENDIF
	THICK = INC*DENSTY
	ENDIF
C
	WI=AI*AMU
	INDEX=0
	 WE=EMASS1(0,-1)
C
C
C     SET ELOSS TO 0.  AND SET ENERGY EPRIME TO INCIDENT ENERGY.
	ELOSS=0.
	RANGE = 0.0
	EPRIM=E
C     CALCULATE PARTICLE VELOCITY  (C=1.)   AND PARAMETER EPSILON MAX.
10    BETASQ=(EPRIM**2+2.*WI*EPRIM)/((WI+EPRIM)**2)
	FACTOR=1.+2.*WE/(WI*SQRT(1.-BETASQ))+(WE/WI)**2
	EPSMAX=2.*WE*BETASQ/((1.-BETASQ)*FACTOR)
C	                                                                *
C     CALCULATE  DE/DX  (BETHE FORMULA) FOR ENERGY EPRIME.
	FACTOR=2.*WE*BETASQ*EPSMAX/((POTL**2)*(1.-BETASQ))
	X=0.30058*(ZI**2)*(WE/BETASQ)*(ZT/AT)*(ALOG(FACTOR)-2.*BETASQ-2.*C
	1/ZT-DELTA)
C
	IF(TT.GT.0.)GO TO 15
	RANGE = RANGE + INC
	EL = X*THICK
	EPRIM = EPRIM - EL
	IF(EPRIM.GT.EL) GO TO 10
	ELOSS = RANGE
	RETURN
15	CONTINUE
C
C     BRANCH OUT IF CALCULATED STOPPING POWER IS NEGATIVE.
	IF (X.LE.0.0) GO TO 35
C	                                                                *
C     BRANCH OUT IF PARTICLE IS STOPPED IN TARGET.
	IF (EPRIM.LT.X*THICK) GO TO 20
C     SET SMALL THICKNESS OF TARGET TO .5 PER CENT OF INCIDENT ENERGY
C     DIVIDED BY  DE/DX FROM BETHE FORMULA.
	TDELT=E/(200.*X)
C     BRANCH OUT IF THE TARGET THICKNESS REMAINING IS LESS THAN THE
C     SMALL THICKNESS JUST CALCULATED.
	IF (TDELT.GE.THICK) GO TO 30
C     LET PARTICLE PASS THROUGH SMALL THICKNESS, AND CALCULATE REMAINING
C     THICKNESS AND NEW PARTICLE ENERGY AFTER PASSING THROUGH.
	ELOSS=ELOSS+TDELT*X
	THICK=THICK-TDELT
	EPRIM=EPRIM-TDELT*X
C	                                                                *
C     SET PRINT INDEX IF ENERGY IS SUCH THAT THE BETHE FORMULA IS NOT
C     VALID, THEN GO BACK TO CALCULATE NEW NOS. FOR NEW ENERGY IF
C     PARTICLE IS NOT STOPPED.
	EMIN=EPRIM/(WI*ZI**(4./3.))
	IF (EMIN.LT.0.0005) INDEX=1
	IF (EPRIM.GT.0.0) GO TO 10
C	                                                                *
C     PARTICLE STOPPED IN TARGET. SET ENERGY LOSS TO INCIDENT ENERGY,
C     SET IERROR TO 1, AND RETURN.
   20  CONTINUE
	ELOSS=E
	IERROR=1
	RETURN
30	IF(INDEX.NE.1) RETURN
	IERROR = 2
	RETURN
35	IERROR = 3
	END


       FUNCTION EMASS1(A,Z)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C FUNCTION EMASS1(A,Z) - FINDS THE MASS DIFFERENCE IN MEV OF A NUCLEUS GIVEN A
C                       AND Z BY READING THE FILE MASS83.DAT USING RELATIVE
C			RECORD NUMBER A.
C
C VARIABLES USED
C       I       - AN INTEGER USED IN DO LOOPS.
C       J       - AN INTEGER USED IN DO LOOPS,
C       A       - THE A OF THE NUCLEUS.
C       Z       - THE Z OF THE NUCLEUS.
C       DATA    - A TWO DIMENSIONAL ARRAY WHERE
C                    DATA(I,1) = Z
C                    DATA(I,2) = MASS DIFFERENCE IN MEV
C                    DATA(I,3) = ERROR IN MASS DIFFERENCE IN MEV
C       LOLIM   - THE LOWEST VALUE OF A ALLOWED.
C       UPLIM   - THE LARGEST VALUE OF A ALLOWED.
C
C DATE - AUGUST 14, 1985
C
C WRITTEN BY MARK ROBERTS
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      INTEGER A,I,J,Z,iz
      INTEGER LOLIM,UPLIM
      REAL*8 DATA(13,3)
      REAL*8 EMASS1,AA
C
C CHECK TO SEE IF ELECTRON MASS IS REQUESTED
C
      IF ((A.EQ.0).AND.(Z.EQ.-1)) THEN
         EMASS = 0.5110034
         RETURN
      ENDIF
C
C CHECK FOR A VALID A AND Z
C
      LOLIM=1
      UPLIM=266
      IF (A.LT.LOLIM) GO TO 110
      IF (A.GT.UPLIM) GO TO 110
      IF ((Z.EQ.0).AND.(A.NE.1)) GO TO 110
C
C READ MASS83A.DAT
C
      OPEN (UNIT=9,FILE='/usr/local/lib/MASS83A.DAT'
     1  ,STATUS='OLD')

      do 10 k=1,A
          READ (9,*) AA,((DATA(I,J),J=1,3),I=1,13)
   10 continue

      CLOSE (UNIT=9)
C
C FIND THE DATA WITH THE CORRECT Z VALUE
C
      DO 100 I=1,13
        iz= data(i,1) + 0.5

Ctest	write(*,*) AA,iz,data(i,2)

        IF (iz.EQ.Z) GO TO 120
  100 CONTINUE
C
C SET THE MASS DIFFERENCE EQUAL TO ZERO IF THAT A AND Z COMBINATION IS NOT FOUND
C
  110 EMASS1 = 0.0
      RETURN
C
C SET EMASS1 EQUAL TO MASS DIFFERENCE
C
  120 EMASS1 = DATA(I,2)/1000.

Ctest	write(*,*) emass

      RETURN
      END
