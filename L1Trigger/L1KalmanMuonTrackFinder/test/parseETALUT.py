import ROOT

f=ROOT.TFile("results_singleMuon140.root")
h = f.Get("etaLUT").ProfileX()


info={}
for i in range(0,4096):
    bini=h.GetXaxis().FindBin(i)
    c = h.GetBinContent(bini)
    if c !=0.0:
        info[i]=round(c)
f.Close()


a=[]
v=[]



for addr,val in info.iteritems():
    a.append(str(int(addr)))
    v.append(str(int(val)))

print "etaLUTAddr=cms.vint32("+",".join(a)+"),\n"
print "etaLUTValue=cms.vint32("+",".join(v)+"),"

