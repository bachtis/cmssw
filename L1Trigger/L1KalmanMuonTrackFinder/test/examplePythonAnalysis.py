import ROOT,itertools,math
from DataFormats.FWLite import Events, Handle
ROOT.FWLiteEnabler.enable()
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



def fetchGEN(event,etaMax=0.83):
    genH  = Handle  ('vector<reco::GenParticle>')
    event.getByLabel('genParticles',genH)
    genMuons=filter(lambda x: abs(x.pdgId())==13 and x.status()==1 and abs(x.eta())<etaMax,genH.product())
    return genMuons

def fetchBMTF(event):
    bmtfH  = Handle  ('BXVector<l1t::Muon>')
    event.getByLabel('hltGtStage2Digis','Muon','HLT',bmtfH)
    bmtf=bmtfH.product()
    bmtfMuons=[]
    for bx in range(bmtf.getFirstBX(),bmtf.getLastBX()+1):
        for j in range(0,bmtf.size(bx)):
            mu = bmtf.at(bx,j)
            if abs(mu.eta())<0.83:
                ##Calibrate the BMTF for now just by a factor to get the scale right
                p4 = ROOT.reco.Candidate.PolarLorentzVector(mu.pt()/1.162,mu.eta(),mu.phi(),0.105)
                mu.setP4(p4)
                bmtfMuons.append(mu)
    return sorted(bmtfMuons,key=lambda x: x.pt(),reverse=True)

def fetchKMTF(event,coll,etaMax=0.83):
    kmtfH  = Handle('vector<L1KalmanMuTrack>')
    event.getByLabel(coll,kmtfH)
    kmtf=filter(lambda x: abs(x.eta())<etaMax,kmtfH.product())
    return sorted(kmtf,key=lambda x: x.pt(),reverse=True)

def curvResidual(a,b):
    return (a.charge()/a.pt()-b.charge()/b.pt())*b.pt()/b.charge()


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


def log(stubs,gen,kmtfAll,kmtf,bmtf):   
    print "--------NEW EVENT------------"
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
        print "Preselected KMTF charge={q} pt={pt} eta={eta} phi={phi} curvature={K} sta curvature={STAK} chi2={chi} dxy={dxy} stubs={stubs} ".format(q=k.charge(),pt=k.pt(),eta=k.eta(),phi=k.phi(),K=k.curvatureAtVertex(),STAK=k.curvature(),chi=k.approxChi2(),dxy=k.dxy(),stubs=len(k.stubs()))
    print 'KMTF cleaned:'
    for k in kmtf :
        print "Selected Prompt  KMTF charge={q} pt={pt} eta={eta} phi={phi} curvature={K} sta curvature={STAK} chi2={chi} dxy={dxy} stubs={stubs} ".format(q=k.charge(),pt=k.pt(),eta=k.eta(),phi=k.phi(),K=k.curvatureAtVertex(),STAK=k.curvature(),chi=k.approxChi2(),dxy=k.dxy(),stubs=len(k.stubs()))
    print "-----------------------------"
    print "-----------------------------"
    print "c + enter to continue"
    import pdb;pdb.set_trace()

#########Histograms#############

#residual vs gen pt
resKMTF = ROOT.TH2D("resKMTF","resKF",10,8,100,60,-2,2)
resKMTFAll = ROOT.TH2D("resKMTFAll","resKF",10,8,100,60,-2,2)
resBMTF = ROOT.TH2D("resBMTF","resKF",10,8,100,60,-2,2)

genPt=ROOT.TH1F("genPt","genPt",40,8,100)
genPtKMTF=ROOT.TH1F("genPtKMTF","genPt",40,8,100)
genPtKMTFAll=ROOT.TH1F("genPtKMTFAll","genPt",40,8,100)
genPtBMTF=ROOT.TH1F("genPtBMTF","genPt",40,8,100)
genEta=ROOT.TH1F("genEta","genEta",40,-0.8,0.8)
genEtaKMTF=ROOT.TH1F("genEtaKMTF","genPt",40,-0.8,0.8)
genEtaKMTFAll=ROOT.TH1F("genEtaKMTFAll","genPt",40,-0.8,0.8)
genEtaBMTF=ROOT.TH1F("genEtaBMTF","genEta",40,-0.8,0.8)


chiBestKMTF = ROOT.TH1F("chiBestKMTF","chiBest",512,0,8192)
chiKMTF = ROOT.TH1F("chiKMTF","chiBest",512,0,8192)

dxyBestKMTF = ROOT.TH1F("dxyBestKMTF","chiBest",512,0,512)
dxyKMTF = ROOT.TH1F("dxyKMTF","chiBest",512,0,512)


rateBMTF = ROOT.TH1F("rateBMTF","rateBMTF",20,2.5,102.5)
rateKMTF = ROOT.TH1F("rateKMTF","rateKMTF",20,2.5,102.5)
rateBMTFp7 = ROOT.TH1F("rateBMTFp7","rateBMTF",20,2.5,102.5)
rateKMTFp7 = ROOT.TH1F("rateKMTFp7","rateKMTF",20,2.5,102.5)

##############################

verbose=False
tag='singleMuon140'


events=Events([tag+'.root'])

for event in events:
    #fetch stubs
    stubs = fetchStubs(event)
    #fetch gen
    gen = fetchGEN(event)
    #fetch kalman (fullcombinatorics)
    kmtfAll = fetchKMTF(event,'l1KalmanMuonTracks',0.83)

    #fetch kalman (prompt)
    kmtf = fetchKMTF(event,'l1PromptKalmanMuonTracks',0.83)

    #fetch BMTF
    bmtf = fetchBMTF(event)

    #printout
    if verbose:
        log(stubs,gen,kmtfAll,kmtf,bmtf)



    #Do not do anything if not at least 2 stubs anywhere 
    #runs faster and factors out DT inneficiency
    if len(stubs)<2:
        continue

    ##Fill histograms and rates
    for track in kmtfAll:
        chiKMTF.Fill(track.approxChi2())
        dxyKMTF.Fill(track.approxChi2())

    if len(kmtf)>0:
        if kmtf[0].pt()>102.4:
            rateKMTF.Fill(102.4)
            if abs(kmtf[0].eta())<0.7:
                rateKMTFp7.Fill(102.4)

        else:
            rateKMTF.Fill(kmtf[0].pt())
            if abs(kmtf[0].eta())<0.7:
                rateKMTFp7.Fill(kmtf[0].pt())

    if len(bmtf)>0:
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
        
        genPt.Fill(g.pt())
        ##the eta efficiency we want at the plateau to see strucuture
        if g.pt()>25:
            genEta.Fill(g.eta())

        #match *(loosely because we still use coarse eta)
        matchedBMTF = filter(lambda x: deltaR(g.eta(),g.phi(),x.eta(),x.phi())<0.8,bmtf) 
        matchedKMTFAll = filter(lambda x: deltaR(g.eta(),g.phi(),x.eta(),x.phi())<0.8,kmtfAll) 
        matchedKMTF = filter(lambda x: deltaR(g.eta(),g.phi(),x.eta(),x.phi())<0.8,kmtf)
        
        #find the best of them and fill
        if len(matchedBMTF)>0:
            bestBMTF = min(matchedBMTF,key = lambda x:  abs(curvResidual(x,g)))  
            resBMTF.Fill(g.pt(),curvResidual(bestBMTF,g))
            
            #the PT we want for 15 GeV to see turn on
            if matchedBMTF[0].pt()>15:
                genPtBMTF.Fill(g.pt())
                if g.pt()>25:
                    genEtaBMTF.Fill(g.eta())
                    

        if len(matchedKMTFAll)>0:
            bestKMTFAll = min(matchedKMTFAll,key = lambda x:  abs(curvResidual(x,g)))  
            resKMTFAll.Fill(g.pt(),curvResidual(bestKMTFAll,g))
            #the PT we want for 15 GeV to see turn on
            if matchedKMTFAll[0].pt()>15:
                genPtKMTFAll.Fill(g.pt())
                if g.pt()>25:
                    genEtaKMTFAll.Fill(g.eta())

            chiBestKMTF.Fill(bestKMTFAll.approxChi2())        
            dxyBestKMTF.Fill(bestKMTFAll.dxy())        

        if len(matchedKMTF)>0:
            bestKMTF = min(matchedKMTF,key = lambda x:  abs(curvResidual(x,g)))  
            resKMTF.Fill(g.pt(),curvResidual(bestKMTF,g))
            #the PT we want for 15 GeV to see turn on
            if matchedKMTF[0].pt()>15:
                genPtKMTF.Fill(g.pt())
                if g.pt()>25:
                    genEtaKMTF.Fill(g.eta())

        
        
f=ROOT.TFile("results_"+tag+".root","RECREATE")

resKMTF.Write()     
resKMTFAll.Write()   
resBMTF.Write()

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


chiBestKMTF.Write()  
c = chiBestKMTF.GetCumulative()
c.Scale(1.0/c.GetMaximum())
c.Write("efficiencyVsBestChi2")

c = chiKMTF.GetCumulative()
c.Scale(1.0/c.GetMaximum())
c.Write("efficiencyVsChi2")

dxyBestKMTF.Write()  
dxyKMTF.Write()
      
c = rateBMTF.GetCumulative(False)
c.Write("normRateBMTF")     

c = rateKMTF.GetCumulative(False)
c.Write("normRateKMTF")     

c = rateBMTFp7.GetCumulative(False)
c.Write("normRateBMTFEtaP7")     

c = rateKMTFp7.GetCumulative(False)
c.Write("normRateKMTFEtaP7")     

f.Close()







