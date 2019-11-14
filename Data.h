#ifndef DATA_H
#define DATA_H

#include "TTree.h"

class Data{
 public: 
  /**
   * @brief Construct a new Data object
   * 
   * @param tree - pointer to the TTree (or TChain) class
   */
  Data(TTree* tree);

 protected:

  /**
   * @brief pointer to the TTree (or TChain) class
   */
  TTree* m_tree = 0;

};

#endif
