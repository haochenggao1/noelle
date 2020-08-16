/*
 * Copyright 2016 - 2020  Yian Su
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include "Noelle.hpp"

using namespace llvm;

namespace llvm {
  
  struct LoopStats : public ModulePass {
    public:
      static char ID;

      LoopStats();
      virtual ~LoopStats();

      bool doInitialization(Module &M) override;
      void getAnalysisUsage(AnalysisUsage &AU) const override;
      bool runOnModule(Module &M) override;

    private:

      struct Stats {
        int64_t loopID = -1;
        int64_t numberOfIVs = 0;
        bool isGovernedByIV = false;
        int64_t numberOfComputingInstructionsForIVs = 0;
        int64_t numberOfInvariantsContainedWithinTheLoop = 0;
        int64_t numberOfNodesInSCCDAG = 0;
        int64_t numberOfSCCs = 0;
        int64_t numberOfSequentialSCCs = 0;
      };
      std::unordered_map<int, Stats *> statsByLoopAccordingToLLVM;
      std::unordered_map<int, Stats *> statsByLoopAccordingToNoelle;

      void collectStatsForLoop (int id, ScalarEvolution &SE, PDG *loopDG, Loop &llvmLoop);
      void collectStatsForLoop (LoopDependenceInfo &LDI);

      void collectStatsOnLLVMSCCs (PDG *loopDG, Stats *statsForLoop);
      void collectStatsOnLLVMIVs (ScalarEvolution &SE, Loop &llvmLoop, Stats *stats);
      void collectStatsOnLLVMInvariants (Loop &llvmLoop, Stats *stats);

      void collectStatsOnNoelleIVs (LoopDependenceInfo &LDI, Stats *stats);
      void collectStatsOnNoelleSCCs (LoopDependenceInfo &LDI, Stats *stats);
      void collectStatsOnNoelleInvariants (LoopDependenceInfo &LDI, Stats *stats);

      void collectStatsOnSCCDAG (SCCDAG *sccdag, SCCDAGAttrs *sccdagAttrs, Stats *statsForLoop);

      void printStats (Stats *stats);
      void printStatsHumanReadable (void);

  };

}
