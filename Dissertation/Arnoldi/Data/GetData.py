
import sys
import os
p=os.path.join(os.path.expanduser('~'), 'Code/Misc')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import gnuFile

A02 = 'ArnoldiBasic-w0.2.dat'
A2 = 'ArnoldiBasic-w2.dat'
A20 = 'ArnoldiBasic-w20.dat'
gA02 = gnuFile.gnuFile(A02)
gA2 = gnuFile.gnuFile(A2)
gA20 = gnuFile.gnuFile(A20)

P02 = 'PowerBasic-w0.2.dat'
P2 = 'PowerBasic-w2.dat'
P20 = 'PowerBasic-w20.dat'
gP02 = gnuFile.gnuFile(P02)
gP2 = gnuFile.gnuFile(P2)
gP20 = gnuFile.gnuFile(P20)

R02 = [0.33, 0.07919, 0.04499]
R2 = [2.09599, 0.8415,0.4823]
R20 = [4.8278, 4.3831, 3.8174]

VA02 = []
VA2 = []
VA20 = []
SDA02 = []
SDA2 = []
SDA20 = []

AFOM02 = float(gA02.Data['FOM-Arnoldi'][-1][1])
AFOM2 = float(gA2.Data['FOM-Arnoldi'][-1][1])
AFOM20 = float(gA20.Data['FOM-Arnoldi'][-1][1])

AT02 = float(gA02.Data['Arnoldi-Time'][-1][1])
AT2 = float(gA2.Data['Arnoldi-Time'][-1][1])
AT20 = float(gA20.Data['Arnoldi-Time'][-1][1])

VA02.append(float(gA02.Data['RAM eigenvalue-0-Real'][-1][1]) )
VA2.append( float(gA2.Data['RAM eigenvalue-0-Real'][-1][1]) )
VA20.append(float(gA20.Data['RAM eigenvalue-0-Real'][-1][1]) )
SDA02.append(float(gA02.Data['RAM eigenvalue-0-Real'][-1][2]) )
SDA2.append(  float(gA2.Data['RAM eigenvalue-0-Real'][-1][2]) )
SDA20.append(float(gA20.Data['RAM eigenvalue-0-Real'][-1][2]) )

VA02.append(float(gA02.Data['RAM eigenvalue-1-Real'][-1][1]) )
VA2.append(  float(gA2.Data['RAM eigenvalue-1-Real'][-1][1]) )
VA20.append(float(gA20.Data['RAM eigenvalue-1-Real'][-1][1]) )
SDA02.append(float(gA02.Data['RAM eigenvalue-1-Real'][-1][2]) )
SDA2.append(  float(gA2.Data['RAM eigenvalue-1-Real'][-1][2]) )
SDA20.append(float(gA20.Data['RAM eigenvalue-1-Real'][-1][2]) )

VA02.append(float(gA02.Data['RAM eigenvalue-2-Real'][-1][1]) )
VA2.append(  float(gA2.Data['RAM eigenvalue-2-Real'][-1][1]) )
VA20.append(float(gA20.Data['RAM eigenvalue-2-Real'][-1][1]) )
SDA02.append(float(gA02.Data['RAM eigenvalue-2-Real'][-1][2]) )
SDA2.append(  float(gA2.Data['RAM eigenvalue-2-Real'][-1][2]) )
SDA20.append(float(gA20.Data['RAM eigenvalue-2-Real'][-1][2]) )

VP02 = float(gP02.Data['Power values'][-1][1])
VP2 =  float( gP2.Data['Power values'][-1][1])
VP20 = float(gP20.Data['Power values'][-1][1])
SDP02 = float(gP02.Data['Power values'][-1][2])
SDP2 =  float( gP2.Data['Power values'][-1][2])
SDP20 = float(gP20.Data['Power values'][-1][2])

PFOM02 = float(gP02.Data['FOM-Power'][-1][1])
PFOM2 = float(gP2.Data['FOM-Power'][-1][1])
PFOM20 = float(gP20.Data['FOM-Power'][-1][1])

PT02 = float(gP02.Data['Power-Time'][-1][1])
PT2 = float(gP2.Data['Power-Time'][-1][1])
PT20 = float(gP20.Data['Power-Time'][-1][1])

diff = abs(R02[0]-VP02)
if diff > SDP02:
    sys.stdout.write("\x1b[31m")
print "%8.6f & %6.1E & %8.6f & %6.1E \\\\" %( VP02, SDP02, R02[0], diff )
if diff > SDP02:
    sys.stdout.write("\x1b[m")
print
for i in xrange(3):
    diff = abs(R02[i]-VA02[i])
    if diff > SDA02[i]:
        sys.stdout.write("\x1b[31m")
    print "%8.5f & %6.1E & %8.5f & %6.1E \\\\" %( 
            VA02[i], SDA02[i], R02[i], diff )
    if diff > SDA02[i]:
        sys.stdout.write("\x1b[m")
print

diff = abs(R2[0]-VP2)
if diff > SDP2:
    sys.stdout.write("\x1b[31m")
print "%8.5f & %6.1E & %8.5f & %6.1E \\\\" %( VP2, SDP2, R2[0], abs(R2[0]-VP2) )
if diff > SDP2:
    sys.stdout.write("\x1b[m")
print
for i in xrange(3):
    diff = abs(R2[i]-VA2[i])
    if diff > SDA2[i]:
        sys.stdout.write("\x1b[31m")
    print "%8.5f & %6.1E & %8.5f & %6.1E \\\\" %( 
            VA2[i], SDA2[i], R2[i], diff )
    if diff > SDA2[i]:
        sys.stdout.write("\x1b[m")
print

diff = abs(R20[0]-VP20)
if diff > SDP20:
    sys.stdout.write("\x1b[31m")
print "%8.5f & %6.1E & %8.5f & %6.1E \\\\" %( VP20, SDP20, R20[0], abs(R20[0]-VP20) )
if diff > SDP20:
    sys.stdout.write("\x1b[m")
print
for i in xrange(3):
    diff = abs(R20[i]-VA20[i])
    if diff > SDA20[i]:
        sys.stdout.write("\x1b[31m")
    print "%8.4f & %6.1E & %8.4f & %6.1E \\\\" %( 
            VA20[i], SDA20[i], R20[i], diff )
    if diff > SDA20[i]:
        sys.stdout.write("\x1b[m")
print


print "Power   & %8.6f & %6.1E & %6.1E & %6.1f \\\\" %(VP02, SDP02, PFOM02, PT02)
print "Arnoldi & %8.5f & %6.1E & %6.1E & %6.1f \\\\ \n" %(VA02[0], SDA02[0],
        AFOM02, AT02)
print "Power   & %8.5f & %6.1E & %6.1E & %6.1f \\\\" %(VP2, SDP2, PFOM2, PT2)
print "Arnoldi & %8.5f & %6.1E & %6.1E & %6.1f \\\\ \n" %(VA2[0], SDA2[0], AFOM2,
        AT2)
print "Power   & %8.5f & %6.1E & %6.1E & %6.1f \\\\" %(VP20, SDP20, PFOM20, PT20)
print "Arnoldi & %8.4f & %6.1E & %6.1E & %6.1f \\\\ \n" %(VA20[0], SDA20[0],
        AFOM20, AT20)


# Calculate Spread
ASpread = []
PSpread = []

dat = gA02.Data['raw value-00-Real'][:,1]
ASpread.append(dat[25:].std())
dat = gP02.Data['Power raw values'][:,1]
PSpread.append(dat[250:].std())

dat = gA2.Data['raw value-00-Real'][:,1]
ASpread.append(dat[25:].std())
dat = gP2.Data['Power raw values'][:,1]
PSpread.append(dat[250:].std())

dat = gA20.Data['raw value-00-Real'][:,1]
ASpread.append(dat[25:].std())
dat = gP20.Data['Power raw values'][:,1]
PSpread.append(dat[250:].std())

print "PSpread ",
for s in PSpread:
    print "& %6.4f" %s,
print "\\\\"

print "\nASpread ",
for s in ASpread:
    print "& %6.4f" %s,
print "\\\\"
