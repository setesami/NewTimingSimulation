// #include "SimTotem/RPDigiProducer/interface/RPDigiProducer.h"
#include "SimCTPPS/CTPPSPixelDigiProducer/interface/CTPPSPixelDigiProducer.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include <cstdlib> // I need it for random numbers
//needed for the geometry:
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

//#include "DataFormats/TotemRPDataTypes/interface/RPStripDigi.h"
//#include "DataFormats/TotemRPDataTypes/interface/RPDetTrigger.h"

#include "DataFormats/Common/interface/DetSetVector.h"
//#include "DataFormats/TotemRPDetId/interface/TotRPDetId.h"
//#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"

//Random Number
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "CLHEP/Random/RandomEngine.h"

//#include "TotemCondFormats/DAQInformation/interface/AnalysisMask.h"                           // needed??????????
//#include "TotemCondFormats/DataRecord/interface/TotemDAQMappingRecord.h"              // needed??????????

CTPPSPixelDigiProducer::CTPPSPixelDigiProducer(const edm::ParameterSet& conf) :
	conf_(conf) {
	//register your products
	/* Examples
	 produces<ExampleData2>();

	 //if do put with a label
	 produces<ExampleData2>("label");
	 */
	//now do what ever other initialization is needed
	//  std::string strip_digi_label ( conf.getParameter<std::string>("StripDigiLabel") );

	produces<edm::DetSetVector<CTPPSPixelDigi> > ();
//	produces<edm::DetSetVector<RPDetTrigger> > ();

	// register data to consume
	tokenCrossingFrameCTPPSPixel = consumes<CrossingFrame<PSimHit>>(edm::InputTag("mix","g4SimHitsCTPPSPixelHits"));

	RPix_hit_containers_.clear();
	RPix_hit_containers_ = conf.getParameter<std::vector<std::string> > ("ROUList");
	verbosity_ = conf.getParameter<int> ("RPixVerbosity");

//	simulateDeadChannels = false;
//	if (conf.exists("simulateDeadChannels")) { //check if "simulateDeadChannels" variable is defined in configuration file
//		simulateDeadChannels = conf.getParameter<bool> ("simulateDeadChannels");
//	}
}

CTPPSPixelDigiProducer::~CTPPSPixelDigiProducer() {

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called to produce the data  ------------
void CTPPSPixelDigiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
	using namespace edm;
	/* This is an event example
	 //Read 'PSimHit' from the Event
	 Handle<PSimHit> pIn;
	 iEvent.getByLabel("TotemHitsRP",pIn);

	 //Use the ExampleData to create an ExampleData2 which
	 // is put into the Event
	 std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
	 iEvent.put(pOut);
	 */

	/* this is an EventSetup example
	 //Read SetupData from the SetupRecord in the EventSetup
	 ESHandle<SetupData> pSetup;
	 iSetup.get<SetupRecord>().get(pSetup);
	 */

	// initialize random engine
	if(!rndEngine) {
		Service<RandomNumberGenerator> rng;
		if (!rng.isAvailable()) {
			throw cms::Exception("Configuration") << "This class requires the RandomNumberGeneratorService\n"
					"which is not present in the configuration file.  You must add the service\n"
					"in the configuration file or remove the modules that require it.";
		}
		rndEngine = &(rng->getEngine(iEvent.streamID()));
	}

	// Step A: Get Inputs
	edm::Handle<CrossingFrame<PSimHit> > cf;
//	iEvent.getByToken(tokenCrossingFrameTotemRP, cf);
	iEvent.getByToken(tokenCrossingFrameCTPPSPixel, cf);
//	iEvent.getByLabel("mix", "g4SimHitsCTPPSPixelHits", cf);

	if (verbosity_) {
		std::cout << "\n\n=================== Starting SimHit access" << "  ===================" << std::endl;

		std::auto_ptr<MixCollection<PSimHit> > col(
		        new MixCollection<PSimHit> (cf.product(), std::pair<int, int>(-0, 0)));
		std::cout << *(col.get()) << std::endl;
		MixCollection<PSimHit>::iterator cfi;
		int count = 0;
		for (cfi = col->begin(); cfi != col->end(); cfi++) {
			std::cout << " Hit " << count << " has tof " << cfi->timeOfFlight() << " trackid "
			        << cfi->trackId() << " bunchcr " << cfi.bunch() << " trigger " << cfi.getTrigger()
			        << ", from EncodedEventId: " << cfi->eventId().bunchCrossing() << " "
			        << cfi->eventId().event() << " bcr from MixCol " << cfi.bunch() << std::endl;
			std::cout << " Hit: " << (*cfi) << "  " << cfi->exitPoint()<<std::endl;
			count++;
		}
	}

	/*    if( verbosity_ ){
	 std::pair<int,int> range=cf->getBunchRange();
	 std::cout << "Nr of pileups " << cf->getNrPileups() << std::endl;
	 std::vector<PSimHit>::const_iterator it_begin;
	 std::vector<PSimHit>::const_iterator it_end;
	 cf->getPileups(it_begin,it_end);
	 std::vector<PSimHit>::iterator it;
	 for( std::vector<PSimHit>::const_iterator it = it_begin; it != it_end ; it++ )
	 std::cout<<" PSimHit "<<(*it)<<std::endl;
	 std::cout << "Crossing frame id for subdet "<<cf->getEventID()<<",  bunchrange = "<<range.first<<","<<range.second  <<", bunchSpace "<<cf->getBunchSpace() << std::endl;
	 } */

	//std::auto_ptr<MixCollection<PSimHit> > allRPHits(new MixCollection<PSimHit>(cf.product(),RP_hit_containers_));
	std::auto_ptr<MixCollection<PSimHit> > allRPixHits(
	        new MixCollection<PSimHit> (cf.product(), std::pair<int, int>(0, 0)));

	if (verbosity_)
		std::cout << "Input MixCollection size = " << allRPixHits->size() << std::endl;

	//Loop on PSimHit
	SimHitMap.clear();

	MixCollection<PSimHit>::iterator isim;
	for (isim = allRPixHits->begin(); isim != allRPixHits->end(); ++isim) {
		SimHitMap[(*isim).detUnitId()].push_back((*isim));
	}

	// Step B: LOOP on hits in event
	theDigiVector.reserve(400);
	theDigiVector.clear();


	for (simhit_map_iterator it = SimHitMap.begin(); it != SimHitMap.end(); ++it) { // secondo me qua il loop viene fatto sui rivelatori

		edm::DetSet<CTPPSPixelDigi> digi_collector(it->first);

		if (theAlgoMap.find(it->first) == theAlgoMap.end()) {
			theAlgoMap[it->first] = boost::shared_ptr<RPixDetDigitizer>(
			        new RPixDetDigitizer(conf_, *rndEngine, it->first, iSetup)); //per ogni rivelatore creo un digitizer
		}

		std::vector<int> input_links;
		std::vector<std::vector<std::pair<int, double> > >  output_digi_links;   // i links sono i links ai simhits che generano il digi


		(theAlgoMap.find(it->first)->second)->run(SimHitMap[it->first], input_links, digi_collector.data,  output_digi_links);  // runno il digitizer

		std::vector<CTPPSPixelDigi>::iterator pixelIterator = digi_collector.data.begin();


		/**
		 * Exclude all dead channels from vector of strips.
		 */

//		while(stripIterator != digi_collector.data.end()){
			//check if channel is dead
//			if (deadChannelsManager.isChannelDead(stripIterator->GetDetId(), stripIterator->GetStripNo())) {
				//remove channel from vector
//				stripIterator = digi_collector.data.erase(stripIterator);
//			}
//			else{
//				stripIterator++;
//			}
//		}

		if (digi_collector.data.size() > 0) {
		  theDigiVector.push_back(digi_collector);   // colleziono i digi di tutti i rivelatori
		}

	}
//----------------

	std::unique_ptr<edm::DetSetVector<CTPPSPixelDigi> > digi_output(
								      new edm::DetSetVector<CTPPSPixelDigi>(theDigiVector));    

	if (verbosity_) {
		std::cout << "digi_output->size()=" << digi_output->size() << std::endl;

	}
	// Step D: write output to file

	iEvent.put(std::move(digi_output));


//----------------------
	// Step C: create empty output collection
/*	std::auto_ptr<edm::DetSetVector<CTPPSPixelDigi> > digi_output(
								      new edm::DetSetVector<CTPPSPixelDigi>(theDigiVector));    

	if (verbosity_) {
		std::cout << "digi_output->size()=" << digi_output->size() << std::endl;

	}
*/	// Step D: write output to file

//	iEvent.put(std::make_unique<edm::DetSetVector<CTPPSPixelDigi>>(theDigiVector),"RPixDetDigitizer");

}

// ------------ method called once each job just before starting event loop  ------------
void CTPPSPixelDigiProducer::beginRun(edm::Run&, edm::EventSetup const& es){
	// get analysis mask to mask channels
//	if (simulateDeadChannels) {
//		edm::ESHandle<AnalysisMask> analysisMask;
//		es.get<TotemDAQMappingRecord> ().get(analysisMask);
//		deadChannelsManager = DeadChannelsManager(analysisMask); //set analysisMask in deadChannelsManager
//	}
}

// ------------ method called once each job just after ending the event loop  ------------
void CTPPSPixelDigiProducer::endJob() {
}

DEFINE_FWK_MODULE( CTPPSPixelDigiProducer);
