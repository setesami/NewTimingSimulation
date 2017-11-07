import FWCore.ParameterSet.Config as cms

# this is a minimum configuration of the Mixing module,
# to run it in the zero-pileup mode
#

from SimGeneral.MixingModule.mixObjects_cfi import * 
from SimGeneral.MixingModule.pixelDigitizer_cfi import *
from SimGeneral.MixingModule.stripDigitizer_cfi import *
from SimGeneral.MixingModule.ecalDigitizer_cfi import *
from SimGeneral.MixingModule.hcalDigitizer_cfi import *
from SimGeneral.MixingModule.castorDigitizer_cfi import *
from SimGeneral.MixingModule.trackingTruthProducer_cfi import *

import FWCore.ParameterSet.Config as cms

# simCastorDigis = cms.EDAlias(
#     mix = cms.VPSet(
#       cms.PSet(type = cms.string('CastorDataFramesSorted'))
#     )
# )
# simEcalUnsuppressedDigis = cms.EDAlias(
#     mix = cms.VPSet(
#       cms.PSet(type = cms.string('EBDigiCollection')),
#       cms.PSet(type = cms.string('EEDigiCollection')),
#       cms.PSet(type = cms.string('ESDigiCollection'))
#     )
# )
# simHcalUnsuppressedDigis = cms.EDAlias(
#     mix = cms.VPSet(
#       cms.PSet(type = cms.string('HBHEDataFramesSorted')),
#       cms.PSet(type = cms.string('HcalUpgradeDataFramesSorted')),
#       cms.PSet(type = cms.string('HFDataFramesSorted')),
#       cms.PSet(type = cms.string('HODataFramesSorted')),
#       cms.PSet(type = cms.string('ZDCDataFramesSorted'))
#     )
# )
# simSiPixelDigis = cms.EDAlias(
#     mix = cms.VPSet(
#       cms.PSet(type = cms.string('PixelDigiedmDetSetVector')),
#       cms.PSet(type = cms.string('PixelDigiSimLinkedmDetSetVector'))
#     )
# )
# simSiStripDigis = cms.EDAlias(
#     mix = cms.VPSet(
#       cms.PSet(type = cms.string('SiStripDigiedmDetSetVector')),
#       cms.PSet(type = cms.string('SiStripRawDigiedmDetSetVector')),
#       cms.PSet(type = cms.string('StripDigiSimLinkedmDetSetVector'))
#     )
# )
# mergedtruth = cms.EDAlias(
#    mix = cms.VPSet(
#      cms.PSet(type = cms.string('TrackingParticles')),
#      cms.PSet(type = cms.string('TrackingVertexs'))
#    )
# )
#simCTPPSPixelDigis = cms.EDAlias(
#    mix = cms.VPSet(
#      cms.PSet(type = cms.string('CTPPSPixelDigiedmDetSetVector'))
#    )
#)



mix = cms.EDProducer("MixingModule",
    digitizers = cms.PSet(
#     pixel = cms.PSet(
#       pixelDigitizer
#     )
#                         ,
#       strip = cms.PSet(
#     stripDigitizer
#       )
#                           ,
#     ecal = cms.PSet(
#       ecalDigitizer
#     ),
# #     hcal = cms.PSet(
# #       hcalDigitizer
# #     ),
#     castor  = cms.PSet(
#       castorDigitizer
#     ),
#     mergedtruth = cms.PSet(
#         trackingParticles
#     )
    ),
    LabelPlayback = cms.string(''),
    maxBunch = cms.int32(3),
    minBunch = cms.int32(-5), ## in terms of 25 ns

    bunchspace = cms.int32(25),
    mixProdStep1 = cms.bool(False),
    mixProdStep2 = cms.bool(False),

    playback = cms.untracked.bool(False),
    useCurrentProcessOnly = cms.bool(False),
    mixObjects = cms.PSet(
        mixCH = cms.PSet(
            mixCaloHits
        ),
        mixTracks = cms.PSet(
            mixSimTracks
        )
                          ,
        mixVertices = cms.PSet(
            mixSimVertices
        ),
        mixSH = cms.PSet(
            mixSimHits
        ),
        mixHepMC = cms.PSet(
            mixHepMCProducts
        )
    )
)
mix.mixObjects.mixSH.crossingFrames = cms.untracked.vstring('MuonCSCHits',
'MuonDTHits',
'MuonRPCHits',
'TotemHitsRP',
'CTPPSPixelHits')


