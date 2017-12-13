import ROOT,itertools,math      # 
from DataFormats.FWLite import Events, Handle
ROOT.FWLiteEnabler.enable()
##A class to keep BMTF data
class BMTFMuon:
    def __init__(self,mu,pt,eta,phi):
        self.muon=mu
        self.p4 = ROOT.reco.Candidate.PolarLorentzVector(pt,eta,phi,0.105)
        
    def charge(self):
        if self.muon.hwSign()>0:
            return -1
        else:
            return +1 

    def __getattr__(self, name):
        return getattr(self.p4,name)



###Common methods############


def fetchStubs(event,ontime=True,twinMux=True):
    phiSeg    = Handle  ('L1MuDTChambPhContainer')
    if twinMux:
        event.getByLabel('simTwinMuxDigis',phiSeg)
    else:
        event.getByLabel('simDtTriggerPrimitiveDigis',phiSeg)
    if ontime:
        filtered=filter(lambda x: x.bxNum()==0, phiSeg.product().getContainer())
        return filtered
    else:
        return phiSeg.product().getContainer()



def fetchGEN(event,etaMax=1.2):
    genH  = Handle  ('vector<reco::GenParticle>')
    event.getByLabel('genParticles',genH)
    genMuons=filter(lambda x: abs(x.pdgId())==13 and x.status()==1 and abs(x.eta())<etaMax,genH.product())
    return genMuons


def globalBMTFPhi(muon):
    temp=muon.processor()*48+muon.hwPhi()

    temp=temp*2*math.pi/576.0-math.pi*15.0/180.0;
    if temp>math.pi:
        temp=temp-2*math.pi;
    return temp;

def fetchBMTF(event,etaMax=1.2):
    bmtfH  = Handle  ('BXVector<l1t::RegionalMuonCand>')
    event.getByLabel('simBmtfDigis:BMTF',bmtfH)
    bmtf=bmtfH.product()
    bmtfMuons=[]
    for bx in [0]:
        for j in range(0,bmtf.size(bx)):
            mu = bmtf.at(bx,j)
            pt = mu.hwPt()*0.5
            #calibration
            K=1.0/pt
            K = (1.09*K+1.23e-3-0.107*K*K)*1.03
#            K = 1.10*K+6.19e-4+5.81e-6/K
            pt=1.0/K
            ####
            
            phi=globalBMTFPhi(mu)
            eta = mu.hwEta()*0.010875           
            if abs(eta)<=etaMax:
                bmtfMuons.append(BMTFMuon(mu,pt,eta,phi))
    return sorted(bmtfMuons,key=lambda x: x.pt(),reverse=True)

def qPTInt(qPT,bits=14):
    lsb = lsBIT(bits)
    floatbinary = int(math.floor(abs(qPT)/lsb))
    return int((qPT/abs(qPT))*floatbinary)

def lsBIT(bits=14):
    maximum=1.25
    lsb = 1.25/pow(2,bits-1)
    return lsb

#import pdb;pdb.set_trace()

def fetchKMTF(event,coll,etaMax=0.83,chi2=1500):
    kmtfH  = Handle('vector<L1KalmanMuTrack>')
    event.getByLabel('l1KalmanMuonTracks:'+coll,kmtfH)
    kmtf=filter(lambda x: abs(x.eta())<etaMax and x.approxChi2()/x.stubs().size()<chi2,kmtfH.product())
    return sorted(kmtf,key=lambda x: x.pt(),reverse=True)

def curvResidual(a,b):
    return (a.charge()/a.pt()-b.charge()/b.pt())*b.pt()/b.charge()

def curvResidualSTA(a,b):
    charge=a.curvature()/abs(a.curvature()+0.5)
    if charge==0:
        charge=1
    else:
        charge=charge/abs(charge)
    return (charge/a.unconstrainedP4().pt()-b.charge()/b.pt())*b.pt()/b.charge()


def getBit(bitmask,pos):
    return (bitmask & ( 1 << pos )) >> pos


def deltaPhi( p1, p2):
    '''Computes delta phi, handling periodic limit conditions.'''
    res = p1 - p2
    while res > math.pi:
        res -= 2*math.pi
    while res < -math.pi:
        res += 2*math.pi
    return res

def deltaR( *args ):
    return math.sqrt( deltaR2(*args) )

def deltaR2( e1, p1, e2, p2):
    de = e1 - e2
    dp = deltaPhi(p1, p2)
    return de*de + dp*dp


def log(counter,stubs,gen,kmtfAll,kmtf,bmtf):   
    print "--------EVENT"+str(counter)+"------------"
    print "-----------------------------"
    print "-----------------------------"
    print 'Stubs:'
    for s in stubs:
        print 'wheel={w} sector={sc} station={st} high/low={ts} phi={phi} phiB={phiB}'.format(w=s.whNum(),sc=s.scNum(),st=s.stNum(),ts=s.Ts2Tag(),phi=s.phi(),phiB=s.phiB())
    print 'Gen muons:'
    for g in gen:
        print "Generated muon charge={q} pt={pt} eta={eta} phi={phi}".format(q=g.charge(),pt=g.pt(),eta=g.eta(),phi=g.phi())
    print 'BMTF:'
    for g in bmtf :
        print "BMTF charge={q} pt={pt} eta={eta} phi={phi}".format(q=g.charge(),pt=g.pt(),eta=g.eta(),phi=g.phi())
    print 'KMTF full combinatorics:'
    for k in kmtfAll :
        print "Preselected KMTF charge={q} pt={pt} eta={eta} phi={phi} stapt={stapt} curvature={K} sta curvature={STAK} chi2={chi} dxy={dxy} stubs={stubs} ".format(stapt = k.unconstrainedP4().pt(),q=k.charge(),pt=k.pt(),eta=k.eta(),phi=k.phi(),K=k.curvatureAtVertex(),STAK=k.curvature(),chi=k.approxChi2(),dxy=k.dxy(),stubs=len(k.stubs()))
    print 'KMTF cleaned:'
    for k in kmtf :
        print "Selected Prompt KMTF charge={q} pt={pt} eta={eta} phi={phi} stapt={stapt} curvature={K} sta curvature={STAK} chi2={chi} dxy={dxy} stubs={stubs} ".format(stapt = k.unconstrainedP4().pt(),q=k.charge(),pt=k.pt(),eta=k.eta(),phi=k.phi(),K=k.curvatureAtVertex(),STAK=k.curvature(),chi=k.approxChi2(),dxy=k.dxy(),stubs=len(k.stubs()))
    print "-----------------------------"
    print "-----------------------------"
    print "c + enter to continue"
    import pdb;pdb.set_trace()

#########Histograms#############
#residual vs gen pt
etaLUT = ROOT.TH2D("etaLUT","etaLUT",4096,0,4096,128,0,128)

bmtfCalib = ROOT.TH2D("bmtfCalib","resKF",50,1.0/120.,1.0/6.,100,0,10)
kfCalib = ROOT.TH2D("kfCalib","resKF",100,46,2700,100,0,5)


resKMTF = ROOT.TH2D("resKMTF","resKF",50,0,100,60,-2,2)
resKMTFAll = ROOT.TH2D("resKMTFAll","resKF",50,0,100,60,-2,2)
resSTAKMTFAll = ROOT.TH2D("resSTAKMTFAll","resKF",50,0,100,60,-6,6)
resBMTF = ROOT.TH2D("resBMTF","resKF",50,0,100,60,-2,2)

resEtaKMTF = ROOT.TH1D("resEtaKMTF","resKF",60,-0.8,0.8)
resEtaBMTF = ROOT.TH1D("resEtaBMTF","resKF",60,-0.8,0.8)
resSTAPhiKMTF = ROOT.TH1D("resSTAPhiKMTF","resKF",60,-0.5,0.5)

resPhiKMTF = ROOT.TH1D("resPhiKMTF","resKF",250,-0.5,0.5)
resPhiBMTF = ROOT.TH1D("resPhiBMTF","resKF",250,-0.5,0.5)

resRBMTF = ROOT.TH1D("resRBMTF","resKF",250,0,8)
resRKMTF = ROOT.TH1D("resRKMTF","resKF",250,0,8)

diffChiGood = ROOT.TH1D("diffChiGood","resKF",500,-7000,7000)
diffChiBad = ROOT.TH1D("diffChiBad","resKF",500,-7000,7000)



genPt=ROOT.TH1F("genPt","genPt",10,8,100)
genPtKMTF=ROOT.TH1F("genPtKMTF","genPt",10,8,100)
genPtKMTFAll=ROOT.TH1F("genPtKMTFAll","genPt",10,8,100)
genPtBMTF=ROOT.TH1F("genPtBMTF","genPt",10,8,100)

genEta=ROOT.TH1F("genEta","genEta",50,-1.0,1.0)
genEtaKMTF=ROOT.TH1F("genEtaKMTF","genPt",50,-1.0,1.0)
genEtaKMTFAll=ROOT.TH1F("genEtaKMTFAll","genPt",50,-1.0,1.0)
genEtaBMTF=ROOT.TH1F("genEtaBMTF","genEta",50,-1.0,1.0)


chiBestKMTF = ROOT.TH1F("chiBestKMTF","chiBest",512,0,8192)
chiKMTF = ROOT.TH1F("chiKMTF","chiBest",512,0,8192)

dxyBestKMTF = ROOT.TH1F("dxyBestKMTF","chiBest",512,0,2048)
dxyKMTF = ROOT.TH1F("dxyKMTF","chiBest",512,0,2048)

etaBMTF = ROOT.TH1F("etaBMTF","rateBMTF",24,-1.2,1.2)
etaKMTF = ROOT.TH1F("etaKMTF","rateKMTF",24,-1.2,1.2)


rateBMTF = ROOT.TH1F("rateBMTF","rateBMTF",20,2.5,102.5)
rateKMTF = ROOT.TH1F("rateKMTF","rateKMTF",20,2.5,102.5)
rateBMTFp7 = ROOT.TH1F("rateBMTFp7","rateBMTF",20,2.5,102.5)
rateKMTFp7 = ROOT.TH1F("rateKMTFp7","rateKMTF",20,2.5,102.5)

####Save the Kalman Gains for LUTs
kalmanGain={}
kalmanGain[3] = {}
kalmanGain[3][8]={}
kalmanGain[2] = {}
kalmanGain[2][8]={}
kalmanGain[2][12]={}
kalmanGain[2][4]={}
kalmanGain[1] = {}
kalmanGain[1][8]={}
kalmanGain[1][6]={}
kalmanGain[1][4]={}
kalmanGain[1][2]={}
kalmanGain[1][10]={}
kalmanGain[1][12]={}
kalmanGain[1][14]={}
kalmanGain[0] = {}
kalmanGain[0][15] = {}
kalmanGain[0][11] = {}
kalmanGain[0][13] = {}
kalmanGain[0][14] = {}
kalmanGain[0][12] = {}
kalmanGain[0][10] = {}
kalmanGain[0][9] = {}
kalmanGain[0][7] = {}
kalmanGain[0][5] = {}
kalmanGain[0][6] = {}
kalmanGain[0][3] = {}
for station,info1 in kalmanGain.iteritems():
    for bit,info2 in info1.iteritems():
        if station>0:
            kalmanGain[station][bit][0]=ROOT.TH2D("gain_{station}_{bit}_{element}".format(station=station,bit=bit,element=0),"h",128,0,512,512,-100,100)
            kalmanGain[station][bit][1]=ROOT.TH2D("gain_{station}_{bit}_{element}".format(station=station,bit=bit,element=1),"h",128,0,512,512,-3,3)
        else:
            kalmanGain[station][bit][0]=ROOT.TH2D("gain_{station}_{bit}_{element}".format(station=station,bit=bit,element=0),"h",128,0,512,512,-5,5)
            kalmanGain[station][bit][1]=ROOT.TH2D("gain_{station}_{bit}_{element}".format(station=station,bit=bit,element=1),"h",128,0,512,512,-5,5)

        kalmanGain[station][bit][2]=ROOT.TH2D("gain_{station}_{bit}_{element}".format(station=station,bit=bit,element=2),"h",128,0,512,512,-100,100)
        kalmanGain[station][bit][3]=ROOT.TH2D("gain_{station}_{bit}_{element}".format(station=station,bit=bit,element=3),"h",128,0,512,512,-100,100)
        kalmanGain[station][bit][4]=ROOT.TH2D("gain_{station}_{bit}_{element}".format(station=station,bit=bit,element=4),"h",128,0,512,512,-8,0)
        kalmanGain[station][bit][5]=ROOT.TH2D("gain_{station}_{bit}_{element}".format(station=station,bit=bit,element=5),"h",128,0,512,512,0,1)

        
    

##############################

verbose=False
tag='singleNeutrino140'


events=Events([tag+'.root'])
counter=-1
for event in events:
    counter=counter+1
    #fetch stubs
    stubs = fetchStubs(event)
    #fetch gen
    gen = fetchGEN(event,0.83)
    #fetch kalman (fullcombinatorics)
    kmtfAll = fetchKMTF(event,'All',1.2)

    #fetch kalman (prompt)
    kmtf = fetchKMTF(event,'Cleaned',1.2,600)


    #fetch BMTF
    bmtf = fetchBMTF(event,1.2)

    #printout
    if verbose and (len(kmtf)>0) and kmtf[0].pt()>35:
        log(counter,stubs,gen,kmtfAll,kmtf,bmtf)

    #Do not do anything if not at least 2 stubs anywhere 
    #runs faster and factors out DT inneficiency
    if len(stubs)<2:
        continue

    ##Fill histograms and rates
    for track in kmtfAll:
        mask = track.phiPattern();
        pattern=[]
        for i in range(0,4):
            pattern.append(getBit(mask,i))
        #start from station 3
        if pattern[2]==1:
            partialMask = pattern[3]*8
            if partialMask in kalmanGain[3]:
                gain = track.kalmanGain(3)
                for i in range(0,6):
                    kalmanGain[3][partialMask][i].Fill(gain[0]/4,gain[i+1])
        #start from station 2
        if pattern[1]==1:
            partialMask = pattern[3]*8+pattern[2]*4
            if partialMask in kalmanGain[2]:
                gain = track.kalmanGain(2)
                for i in range(0,6):
                    kalmanGain[2][partialMask][i].Fill(gain[0]/4,gain[i+1])
        #start from station 1
        if pattern[0]==1:
            partialMask = pattern[3]*8+pattern[2]*4+pattern[1]*2
            if partialMask in kalmanGain[1]:
                gain = track.kalmanGain(1)
                for i in range(0,6):
                    kalmanGain[1][partialMask][i].Fill(gain[0]/4,gain[i+1])
        #start from vertex
        partialMask = pattern[3]*8+pattern[2]*4+pattern[1]*2+pattern[0]
        if partialMask in kalmanGain[0]:
            gain = track.kalmanGain(0)
            for i in range(0,3):
                kalmanGain[0][partialMask][i].Fill(gain[0]/4,gain[i+1])

    
        chiKMTF.Fill(track.approxChi2()/track.stubs().size())
        dxyKMTF.Fill(abs(track.dxy()))

    if len(kmtf)>0:
        for k in kmtf:
            etaKMTF.Fill(k.eta())
        if kmtf[0].pt()>102.4:
            rateKMTF.Fill(102.4)
            if abs(kmtf[0].eta())<0.7:
                rateKMTFp7.Fill(102.4)

        else:
            rateKMTF.Fill(kmtf[0].pt())
            if abs(kmtf[0].eta())<0.7:
                rateKMTFp7.Fill(kmtf[0].pt())

    if len(bmtf)>0:
        for k in bmtf:
            etaBMTF.Fill(k.eta())
        if bmtf[0].pt()>102.4:
            if abs(bmtf[0].eta())<0.7:
                rateBMTFp7.Fill(102.4)
            rateBMTF.Fill(102.4)
        else:
            if abs(bmtf[0].eta())<0.7:
                rateBMTFp7.Fill(bmtf[0].pt())
            rateBMTF.Fill(bmtf[0].pt())

    ##loop on gen and fill resolutuion and efficiencies
    for g in gen:
        if abs(g.eta())<0.83:
            genPt.Fill(g.pt())
        ##the eta efficiency we want at the plateau to see strucuture
        if g.pt()>25:
            genEta.Fill(g.eta())

        #match *(loosely because we still use coarse eta)

        matchedBMTF = filter(lambda x: abs(deltaPhi(g.phi(),x.phi()))<0.5 and abs(g.eta()-x.eta())<0.5,bmtf) 
        matchedKMTFAll = filter(lambda x: abs(deltaPhi(g.phi(),x.phi()))<0.5 and abs(g.eta()-x.eta())<0.5,kmtfAll) 
        matchedKMTF = filter(lambda x: abs(deltaPhi(g.phi(),x.phi()))<0.5 and abs(g.eta()-x.eta())<0.5,kmtf)

#        if len(matchedBMTF)>len(matchedKMTF):
#            log(counter,stubs,gen,kmtfAll,kmtf,bmtf)
            

        #find the best of them and fill
        if len(matchedBMTF)>0:
            bestBMTF = min(matchedBMTF,key = lambda x:  abs(curvResidual(x,g)))  
            resBMTF.Fill(g.pt(),curvResidual(bestBMTF,g))
            resEtaBMTF.Fill(bestBMTF.eta()-g.eta())
            resPhiBMTF.Fill(bestBMTF.phi()-g.phi())
            resRBMTF.Fill(deltaR(g.eta(),g.phi(),bestBMTF.eta(),bestBMTF.phi()))
            bmtfCalib.Fill(1.0/bestBMTF.pt(),bestBMTF.pt()/g.pt())

            
            #the PT we want for 15 GeV to see turn on
            if bestBMTF.pt()>15:
                if abs(g.eta())<0.83:
                    genPtBMTF.Fill(g.pt())
                if g.pt()>25:
                    genEtaBMTF.Fill(g.eta())
                    

        if len(matchedKMTFAll)>0:
            bestKMTFAll = min(matchedKMTFAll,key = lambda x:  abs(curvResidual(x,g)))  
            resKMTFAll.Fill(g.pt(),curvResidual(bestKMTFAll,g))

            etaLUT.Fill(bestKMTFAll.etaCoarsePattern(),round(abs(g.eta())*100))

            bestSTAKMTFAll = min(matchedKMTFAll,key = lambda x:  abs(curvResidualSTA(x,g)))  
            resSTAKMTFAll.Fill(g.pt(),curvResidualSTA(bestSTAKMTFAll,g))




            #the PT we want for 15 GeV to see turn on
            if bestKMTFAll.pt()>15:
                if abs(g.eta())<0.83:
                    genPtKMTFAll.Fill(g.pt())
                if g.pt()>25:
                    genEtaKMTFAll.Fill(g.eta())

            chiBestKMTF.Fill(bestKMTFAll.approxChi2()/bestKMTFAll.stubs().size())        
            dxyBestKMTF.Fill(abs(bestKMTFAll.dxy()))        
                


        if len(matchedKMTF)>0:
            bestKMTF = min(matchedKMTF,key = lambda x:  abs(curvResidual(x,g)))  
            resKMTF.Fill(g.pt(),curvResidual(bestKMTF,g))
            resEtaKMTF.Fill(bestKMTF.eta()-g.eta())
            resPhiKMTF.Fill(bestKMTF.phi()-g.phi())
            resSTAPhiKMTF.Fill(bestKMTF.unconstrainedP4().phi()-g.phi())
            resRKMTF.Fill(deltaR(g.eta(),g.phi(),bestKMTF.eta(),bestKMTF.phi()))
            K = bestKMTF.curvatureAtVertex()
            if K==0:
                K=1;
            kfCalib.Fill(abs(K),float(qPTInt(1.0/g.pt()))/float(abs(K)))

#            if len(matchedBMTF)>0 and abs(bestKMTF.pt()-g.pt())-abs(bestBMTF.pt()-g.pt())>20.0:
#                log(counter,stubs,gen,kmtfAll,kmtf,bmtf)

#            if curvResidual(bestKMTF,g)<-1.3:
#                log(counter,stubs,gen,kmtfAll,kmtf,bmtf)

                

            #the PT we want for 15 GeV to see turn on
            if bestKMTF.pt()>15:
                if abs(g.eta())<0.83:
                    genPtKMTF.Fill(g.pt())
                if g.pt()>25:
                    genEtaKMTF.Fill(g.eta())

        
        
f=ROOT.TFile("results_"+tag+".root","RECREATE")

resKMTF.Write()     
resKMTFAll.Write()   
resBMTF.Write()
resSTAKMTFAll.Write()

resEtaKMTF.Write()     
resEtaBMTF.Write()     
resPhiKMTF.Write()     
resRKMTF.Write()     
resSTAPhiKMTF.Write()     
resPhiBMTF.Write()     
resRBMTF.Write()     
bmtfCalib.Write()
kfCalib.Write()
kmtfEff = ROOT.TGraphAsymmErrors(genPtKMTF,genPt)
kmtfEff.Write("efficiencyVsPtKMTF")
kmtfEffEta = ROOT.TGraphAsymmErrors(genEtaKMTF,genEta)
kmtfEffEta.Write("efficiencyVsEtaKMTF")

kmtfEffAll = ROOT.TGraphAsymmErrors(genPtKMTFAll,genPt)
kmtfEffAll.Write("efficiencyVsPtKMTFAll")
kmtfEffEtaAll = ROOT.TGraphAsymmErrors(genEtaKMTFAll,genEta)
kmtfEffEtaAll.Write("efficiencyVsEtaKMTFAll")

bmtfEff = ROOT.TGraphAsymmErrors(genPtBMTF,genPt)
bmtfEff.Write("efficiencyVsPtBMTF")
bmtfEffEta = ROOT.TGraphAsymmErrors(genEtaBMTF,genEta)
bmtfEffEta.Write("efficiencyVsEtaBMTF")


etaKMTF.Write()
etaBMTF.Write()

chiBestKMTF.Write()  
c = chiBestKMTF.GetCumulative()
if c.GetMaximum()!=0.0:
    c.Scale(1.0/c.GetMaximum())
    c.Write("efficiencyVsBestChi2")

chiKMTF.Write()
c = chiKMTF.GetCumulative()
if (c.GetMaximum())>0:
    c.Scale(1.0/c.GetMaximum())
    c.Write("efficiencyVsChi2")

dxyBestKMTF.Write()  
dxyKMTF.Write()


rateBMTF.Write()      
c = rateBMTF.GetCumulative(False)
c.Write("normRateBMTF")     

rateKMTF.Write()      
c = rateKMTF.GetCumulative(False)
c.Write("normRateKMTF")     

rateBMTFp7.Write()      
c = rateBMTFp7.GetCumulative(False)
c.Write("normRateBMTFEtaP7")     

rateKMTFp7.Write()      
c = rateKMTFp7.GetCumulative(False)
c.Write("normRateKMTFEtaP7")     

etaLUT.Write()


directory = f.mkdir("gains")
directory.cd()
for station,info1 in kalmanGain.iteritems():
    for bit,info2 in info1.iteritems():
        for i in range(0,6):
            kalmanGain[station][bit][i].Write()
f.Close()







