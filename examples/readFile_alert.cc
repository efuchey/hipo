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
#define radtodeg 57.295779513

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

   twig::h1d  hmcP(100, 0.0, 2.0); // declare a 1D histogram
   twig::h1d  hmcTheta(75, 50, 125.0); // declare a 1D histogram
   twig::h1d  hmcPhi(90,-180.0,180.0); // declare a 1D histogram
   
   twig::h1d  htrackP(100, 0.0, 2.0); // declare a 1D histogram
   twig::h1d  htrackTheta(75, 50, 125); // declare a 1D histogram
   twig::h1d  htrackPhi(90, -180.0, 180.0); // declare a 1D histogram

   hipo::bank  MCparticles(factory.getSchema("MC::Particle"));
   hipo::bank  hits(factory.getSchema("AHDC::Hits"));
   hipo::bank  tracks(factory.getSchema("AHDC::Track"));
   
   hipo::event      event;

   int counter = 0;

   reader.gotoEvent(561);

   while(reader.next()==true){
      reader.read(event);
      
      event.getStructure(MCparticles);
      event.getStructure(hits);
      event.getStructure(tracks);
      
      int nrows;
      float  px;
      float  py;
      float  pz;
      int pid;
      
      float theta;
      float phi;
      float p;
      
      nrows = MCparticles.getRows();
      for(int row = 0; row < nrows; row++){
	pid = MCparticles.getInt("pid",row);
	px = MCparticles.getFloat("px",row);
	py = MCparticles.getFloat("py",row);
	pz = MCparticles.getFloat("pz",row);
	
	p = sqrt(px*px+py*py+pz*pz);
	theta = acos(pz/p)*radtodeg;
	phi = atan2(px, py)*radtodeg;

	if(pid==2212){
	  hmcP.fill(p);
	  hmcTheta.fill(theta);
	  hmcPhi.fill(phi);
	}else{
	  printf("%6d : %6d %8.4f %8.4f %8.4f\n",row,pid,px,py,pz);
	}
      }
      // nrows = hits.getRows();
      // for(int row = 0; row < nrows; row++){
      // 	tdc = hits.getFloat("tdc",row);
	
      // 	hHitTDC.fill(tdc);
      // }
      
      nrows = tracks.getRows();
      for(int row = 0; row < nrows; row++){
	px = tracks.getFloat("px",row);
	py = tracks.getFloat("py",row);
	pz = tracks.getFloat("pz",row);
	
	p = sqrt(px*px+py*py+pz*pz);
	theta = acos(pz/p)*radtodeg;
	phi = atan2(px, py)*radtodeg;

	printf("%6d : %8.4f %8.4f %8.4f => P = %8.4f\n",row,px,py,pz, p);	
	
	htrackP.fill(p);
	htrackTheta.fill(theta);
	htrackPhi.fill(phi);
      }
      
      counter++;
   }
   printf("processed events = %d\n",counter);
   hmcP.print();
   htrackP.print();
   hmcTheta.print();
   htrackTheta.print();
   hmcPhi.print();
   htrackPhi.print();
   
   
}
//### END OF GENERATED CODE
