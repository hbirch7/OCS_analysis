#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <vector>
#include <TString.h>
#include <TChain.h>

class EventLoop {
 public: 
 /**
 * @brief Construct a new Event Loop object
 */
 EventLoop();

 /**
 * @brief Initialize the event loop
 */
 void initialize();

 /**
 * @brief calculates total width set from small and large widths sets
 */
  int calcWidthSetTotal(int widthsetLarge, int widthsetSmall);

  float calcWidthSetCount(float widthsetLarge, float widthsetSmall);

 /**
 * @brief Execute the event loop
 */
 void execute();

 /**
 * @brief list of input ROOT file names
 */
  std::vector<TString> inputFiles;

 /**
 * @brief Name of the TTree instance. Must be same in all files
 */
  TString treeName;

protected:
  
 /**
 * @brief Instance of the TChain class used to read the data 
 */
  TChain* m_chain = 0;
  
};

#endif
