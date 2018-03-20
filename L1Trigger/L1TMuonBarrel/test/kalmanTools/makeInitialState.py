import FWCore.ParameterSet.Config as cms


from L1Trigger.L1TMuonBarrel.simKBmtfDigis_cfi import bmtfKalmanTrackingSettings as settings

initialK = settings.initialK
initialK2 = settings.initialK2


def twosComp(number):
    if number>=0:
        return number
    else:
        return (~abs(number))+1



for i in range(0,4):
    arr = []
    for phiB in range(-511,512):
        addr=twosComp(phiB)
        if i in [2,3]:
            if abs(phiB)>63:
                addr = twosComp(63*phiB/abs(phiB))
        if i in [1,0]:
            if abs(addr)>127:
                addr = twosComp(127*phiB/abs(phiB))

        K = int(initialK[i]*8*phiB/(1+initialK2[i]*8*phiB))
        if K>8191:
            K=8191
        if K<-8191:
            K=-8191
        arr.append(str(K))
    print '\n\n\n'    
    if i in [0,1]:
        lut = 'ap_int<14> initialK_'+str(i+1)+'[1024] = {'+','.join(arr)+'};'
    if i in [2,3]:
        lut = 'ap_int<14> initialK_'+str(i+1)+'[1024] = {'+','.join(arr)+'};'
    print lut

        

    

