import FWCore.ParameterSet.Config as cms

from RecoLocalCalo.EcalRecAlgos.ecalCleaningAlgo import cleaningAlgoConfig 


process = cms.Process("Ana")


process.load("Configuration.Geometry.GeometryECALHCAL_cff")
process.load("FWCore.MessageService.MessageLogger_cfi")
from RecoEcal.Configuration.RecoEcal_cff import *

process.load("EventFilter.EcalRawToDigi.EcalUnpackerMapping_cfi");
process.load("EventFilter.EcalRawToDigi.EcalUnpackerData_cfi");


process.ecalEBunpacker.InputLabel = cms.InputTag('rawDataCollector');
#process.ecalEBunpacker.InputLabel = cms.InputTag('source');

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("RecoLocalCalo.EcalRecAlgos.EcalSeverityLevelESProducer_cfi")

process.EcalLaserCorrectionService = cms.ESProducer("EcalLaserCorrectionService")




# global tag for data
#process.GlobalTag.globaltag = 'GR_R_53_V21A::All'

# run 2
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v4'
#process.GlobalTag.globaltag = '74X_dataRun2_EcalCalib_v1'
#process.GlobalTag.globaltag = '76X_dataRun2_v15'
#process.GlobalTag.globaltag = '76X_dataRun2_v19'
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v9'# PATRIZIA



#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",


 fileNames=cms.untracked.vstring(

# Data

#'file:/afs/cern.ch/user/m/mquittna/public/exo/EXOMoriond16_v6_0T/pickeventsrunC.root'
#'file:/afs/cern.ch/work/p/pbarria/public/SingleEvent/CMSSW_8_3_0/src/JetMETCorrections/MCJet/pick1eventRUN274244.root'
  'file:/afs/cern.ch/user/m/mjoyce/WorkingArea/Ecal/CMSSW_8_3_0/src/JetMETCorrections/MCJet/pickevents_ECAL_cell_RECO.root'
 ))



#############   Geometry  ###############

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi");
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi");
process.load("Geometry.CaloEventSetup.CaloTopology_cfi");


#############    Analyser options   ############################
process.eventanalyser = cms.EDAnalyzer("EventAnalyser",
jets                 = cms.string('ak5PFJets'),
histogramFile        = cms.string('exomgg_0t_runC.root'),
tracks               = cms.string('generalTracks'),
vertex               = cms.string('offlinePrimaryVertices'),
JetCorrectionService = cms.string('L2L3JetCorrectorAK5PF'),
EBRecHitCollection   = cms.string('ReducedEcalRecHitsEB'),
EERecHitCollection   = cms.string('ReducedEcalRecHitsEE'),
IsMC                 = cms.bool(False),  # set to True if using MC
cleaningConfig       = cleaningAlgoConfig,
#run                  = cms.string('run'),  # Matt

# bunch structure, run 200473 - 50ns
bunchstartbx         = cms.vint32(66,146,226,306,413,493,573,653,773,853,960,1040,1120,1200,1307,1387,1467,1547,1667,1747,1854,1934,2014,2094,2201,2281,2361,2441,2549,2629,2736,2816,2896,2976,3083,3163,3243,3323)

# bunch structure, run 209146 - 25ns

#bunchstartbx         = cms.vint32(301,358,1195,1252,2089,2146,2977,3034)
# run 203708
#bunchstartbx         = cms.vint32(1786,2680)

                                           )
process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
                                  )


process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(24),
                                           maxd0 = cms.double(2)
                                           )


process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')


#process.load('RecoMET.METFilters.eeBadScFilter_cfi')

#############   Path       ###########################



# data

process.p = cms.Path( process.eventanalyser)



#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 1


