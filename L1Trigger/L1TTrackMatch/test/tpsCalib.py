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


histoData={}

for t in [0,1,2,3]:
    histoData[t]={}
    for depth in [1,2,3,4]:
        histoData[t][depth]={}
        histoData[t][depth]['phi'] = ROOT.TH3F("propPhi_type_"+str(t)+"_depth_"+str(depth),"propPhi_type_"+str(t)+"_depth_"+str(depth),105,0,420,512,-4096,4096,750,-1500,1500)
        if t==0:
            histoData[t][depth]['phiB'] = ROOT.TH3F("propPhiB_type_"+str(t)+"_depth_"+str(depth),"propPhiB_type_"+str(t)+"_depth_"+str(depth),105,0,420,512,-4096,4096,128,-512,512)

PHILSB  = 0.000340885
ETALSB  = 0.00585938
CURVLSB = 0.000122070

def deltaPhi(phi1,phi2):
    pi = int(3.14159/PHILSB)
    delta = phi1-phi2
    if delta>pi:
        return delta-2*pi
    if delta<-pi:
        return delta+2*pi
    return delta
    




counter=-1
for event in events:
    counter=counter+1
    gen=fetchGEN(event)
    stubs = fetchStubs(event,'l1TPSStubs')


    print ('NEW EVENT------------------')
    for s in stubs:
        print("BX={bx} type={type} eta Region={etaR} phiRegion={phiR} depthRegion={depthR}  id={tag} phi={phi} phiB={phiB} etas = {eta1},{eta2}".format(bx=s.bxNum(),type=s.type(),etaR = s.etaRegion(),phiR = s.phiRegion(),depthR=s.depthRegion(),tag=s.id(),phi=s.phi(),phiB=s.phiB(),eta1=s.eta(),eta2=s.alternateEta()))



    for g in gen:
        genphi = int((g.phi()/PHILSB))
        geneta = int(g.eta()/ETALSB)
        genk = int(g.charge()/g.pt()/CURVLSB)
        print( 'gen muon (phi,eta,k)',genphi,geneta,genk)

        for s in stubs:
             if abs(deltaPhi(genphi,s.phi()))>math.pi/2./PHILSB:
                 continue
             histoData[s.type()][s.depthRegion()]['phi'].Fill(abs(geneta),genk,deltaPhi(s.phi(),genphi))
             if (s.type()==0): #DT
                 histoData[s.type()][s.depthRegion()]['phiB'].Fill(abs(geneta),genk,s.phiB())
                 
               
f=ROOT.TFile("test2.root","RECREATE")
f.cd()
for t,dictio in histoData.iteritems():
    for depth,dictio2 in dictio.iteritems():
        if t==0:
            dictio2['phiB'].Write()
        dictio2['phi'].Write()

f.Close()


