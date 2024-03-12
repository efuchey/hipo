//******************************************************************
//*  ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗
//*  ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗
//*  ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║
//*  ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║
//*  ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝
//*  ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝
//************************ Jefferson National Lab (2017) ***********
//******************************************************************
//* Example program for reading HIPO-4 Files..
//* Reads the file created by writeFile program
//*--
//* Author: E. Fuchey, based on a script by G.Gavalian
//*

#include <cstdlib>
#include <iostream>
#include <math.h>
#include "reader.h"
#include "twig.h"


int main(int argc, char** argv) {

   std::cout << " reading file example program (HIPO) "  << __cplusplus << std::endl;

   char inputFile[256];

   if(argc>1) {
     snprintf(inputFile,256,"%s",argv[1]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(1);
   }

   hipo::reader  reader;
   reader.open(inputFile);
   hipo::dictionary  factory;
   reader.readDictionary(factory);

   factory.show();

   twig::h1d  hMCPx(100,-2.0,2.0); // declare a 1D histogram
   twig::h1d  hMCPy(100,-2.0,2.0); // declare a 1D histogram
   twig::h1d  hMCPz(100, 0.5, 4.5); // declare a 1D histogram

   twig::h1d  hMCtheta(90,0,90.0); // declare a 1D histogram
   twig::h1d  hMCphi(90,-180.0,180.0); // declare a 1D histogram
   twig::h1d  hMCp(100, 0.5, 4.5); // declare a 1D histogram
   
   twig::h1d  hMCtheta_p(90,0,90.0); // declare a 1D histogram
   twig::h1d  hMCphi_p(90,-180.0,180.0); // declare a 1D histogram
   twig::h1d  hMCp_p(100, 0.5, 4.5); // declare a 1D histogram
   
   twig::h1d  hHitTDC(100, -10, 10);
   
   twig::h1d  hTrackPx(100,-0.5,0.5); // declare a 1D histogram
   twig::h1d  hTrackPy(100,-0.5,0.5); // declare a 1D histogram
   twig::h1d  hTrackPz(100, 0.0,1.0); // declare a 1D histogram

   // hipo::bank  particles(factory.getSchema("event::particle"));
   // hipo::bank  detectors(factory.getSchema("event::detector"));
   hipo::bank  MCparticles(factory.getSchema("MC::Particle"));
   hipo::bank  hits(factory.getSchema("AHDC::Hits"));
   hipo::bank  tracks(factory.getSchema("AHDC::KFTrack"));
   
   hipo::event      event;

   int counter = 0;

   reader.gotoEvent(561);

   while(reader.next()==true){
      reader.read(event);
      
      // event.getStructure(particles);
      // event.getStructure(detectors);

      // particles.show();
      // detectors.show();

      // int nrows = particles.getRows();
      // printf("---------- PARTICLE BANK CONTENT -------\n");
      // for(int row = 0; row < nrows; row++){
      //    int   pid = particles.getInt("pid",row);
      //    float  px = particles.getFloat("px",row);
      //    float  py = particles.getFloat("py",row);
      //    float  pz = particles.getFloat("pz",row);
      //    printf("%6d : %6d %8.4f %8.4f %8.4f\n",row,pid,px,py,pz);
      // }
      // printf("---------- END OF PARTICLE BANK -------\n");
      
      event.getStructure(MCparticles);
      event.getStructure(hits);
      event.getStructure(tracks);
      
      int nrows;
      float  px;
      float  py;
      float  pz;
      float tdc;
      
      float theta;
      float phi;
      float p;
      
      nrows = MCparticles.getRows();
      for(int row = 0; row < nrows; row++){
	px = MCparticles.getFloat("px",row);
	py = MCparticles.getFloat("py",row);
	pz = MCparticles.getFloat("pz",row);
	
	p = sqrt(px*px+py*py+pz*pz);
	theta = acos(pz/p);
	phi = atan2(px, py);

	hMCPx.fill(px);
	hMCPy.fill(py);
	hMCPz.fill(pz);
      }
      
      nrows = hits.getRows();
      for(int row = 0; row < nrows; row++){
	tdc = hits.getFloat("tdc",row);
	
	hHitTDC.fill(tdc);
      }
      
      nrows = tracks.getRows();
      for(int row = 0; row < nrows; row++){
	px = tracks.getFloat("px",row);
	py = tracks.getFloat("py",row);
	pz = tracks.getFloat("pz",row);
	
	hTrackPx.fill(px);
	hTrackPy.fill(py);
	hTrackPz.fill(pz);
      }
      
      counter++;
   }
   printf("processed events = %d\n",counter);
   hMCPx.print();
   hMCPy.print();
   hMCPz.print();
   hHitTDC.print();
   hTrackPx.print();
   hTrackPy.print();
   hTrackPz.print();
   
}
//### END OF GENERATED CODE
