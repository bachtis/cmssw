from __future__ import print_function
import ROOT,itertools,math      #
from array import array         # 
from DataFormats.FWLite import Events, Handle
ROOT.FWLiteEnabler.enable()
# 



#verbose=False
#tag='singleMuonOfficial'
#isData=False
tag='test'

def fetchStubs(event,tag):
    phiSeg2    = Handle  ('std::vector<L1MuCorrelatorHit>')
    event.getByLabel(tag,phiSeg2)
    return phiSeg2.product()


def fetchGEN(event,etaMax=3.0):
    genH  = Handle  ('vector<reco::GenParticle>')
    event.getByLabel('genParticles',genH)
    genMuons=filter(lambda x: abs(x.pdgId())==13 and x.status()==1 and abs(x.eta())<etaMax,genH.product())
    return genMuons

def fetchGEANT(event):
    geantH  = Handle  ('vector<PSimHit>')
    event.getByLabel('g4SimHits:MuonDTHits',geantH)
    geant=filter(lambda x: x.pabs()>0.5 and abs(x.particleType())==13,geantH.product())
    return geant

def getTrueBarrelChambers(muon,geant):
    thisMuonGEANT = filter(lambda x: (muon.charge()>0 and x.particleType()==-13) or ((muon.charge()<0) and x.particleType()==13),geant)
    chambers=[]
    for p in thisMuonGEANT:        
        detid=ROOT.DTChamberId(p.detUnitId())
        info = {'wheel':detid.wheel(),'sector':detid.sector()-1,'station':detid.station()}
        isThereAlready=False
        for c in chambers:
            if info['wheel']==c['wheel'] and info['sector']==c['sector'] and info['station']==c['station']:
                isThereAlready=True
                break
        if not isThereAlready:    
            chambers.append(info)
    return chambers

def getMatchedBarrelStubs(muon,geant,stubs):
    matched=[]
    chambers = getTrueBarrelChambers(muon,geant)
    for s in stubs:
        for c in chambers:
            if s.stNum()==c['station'] and s.whNum()==c['wheel'] and s.scNum()==c['sector']:
                matched.append(s)
                break;
    return matched




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

events=Events([tag+'.root'])


phi11=ROOT.TH3F("phi11","phi1",270,140,410,512,-4096,4096,750,-1500,1500)
phi12=ROOT.TH3F("phi12","phi1",270,140,410,512,-4096,4096,750,-1500,1500)
phi13=ROOT.TH3F("phi13","phi1",270,140,410,512,-4096,4096,750,-1500,1500)
phi14=ROOT.TH3F("phi14","phi1",270,140,410,512,-4096,4096,750,-1500,1500)
phi21=ROOT.TH3F("phi21","phi1",270,140,410,512,-4096,4096,750,-1500,1500)
phi22=ROOT.TH3F("phi22","phi1",270,140,410,512,-4096,4096,750,-1500,1500)
phi23=ROOT.TH3F("phi23","phi1",270,140,410,512,-4096,4096,750,-1500,1500)
phi24=ROOT.TH3F("phi24","phi1",270,140,410,512,-4096,4096,750,-1500,1500)



counter=-1
for event in events:
    counter=counter+1

    gen=fetchGEN(event)
    stubs = fetchStubs(event,'l1NewStubs')

    print ('NEW EVENT------------------')
    for s in stubs:
        print("BX={bx} type={type} eta Region={etaR} phiRegion={phiR} depthRegion={depthR}  id={tag} phi={phi} phiB={phiB} etas = {eta1},{eta2}".format(bx=s.bxNum(),type=s.type(),etaR = s.etaRegion(),phiR = s.phiRegion(),depthR=s.depthRegion(),tag=s.id(),phi=s.phi(),phiB=s.phiB(),eta1=s.eta(),eta2=s.alternateEta()))



    for g in gen:
        genphi = int((g.phi()*180/3.14)*2048/30.)
        geneta = g.eta()*512/3.0
        genk = int(g.charge()*8192/g.pt())
        print( 'gen muon',genphi,geneta,genk)

        for s in stubs:
             if abs(genphi-s.phi())>5000 or (not s.isCSC()):
                 continue
             if s.etaRegion()>0:
                 if s.depthRegion()==1:
                     phi11.Fill(abs(s.eta()),genk,s.phi()-genphi)
                 if s.depthRegion()==2:
                     phi12.Fill(abs(s.eta()),genk,s.phi()-genphi)
                 if s.depthRegion()==3:
                     phi13.Fill(abs(s.eta()),genk,s.phi()-genphi)
                 if s.depthRegion()==4:
                     phi14.Fill(abs(s.eta()),genk,s.phi()-genphi)
             else:
                 if s.depthRegion()==1:
                     phi21.Fill(abs(s.eta()),genk,s.phi()-genphi)
                 if s.depthRegion()==2:
                     phi22.Fill(abs(s.eta()),genk,s.phi()-genphi)
                 if s.depthRegion()==3:
                     phi23.Fill(abs(s.eta()),genk,s.phi()-genphi)
                 if s.depthRegion()==4:
                     phi24.Fill(abs(s.eta()),genk,s.phi()-genphi)
               
               
f=ROOT.TFile("test2.root","RECREATE")
f.cd()
phi11.Write()
phi12.Write()
phi13.Write()
phi14.Write()
phi21.Write()
phi22.Write()
phi23.Write()
phi24.Write()
f.Close()


