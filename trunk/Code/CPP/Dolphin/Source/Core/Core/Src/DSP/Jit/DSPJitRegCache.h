// Copyright (C) 2011 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#ifndef _DSPJITREGCACHE_H
#define _DSPJITREGCACHE_H

#include "x64Emitter.h"

class DSPEmitter;

enum DSPJitRegSpecial {
	DSP_REG_ACC0_64  =32,
	DSP_REG_ACC1_64  =33,
	DSP_REG_AX0_32   =34,
	DSP_REG_AX1_32   =35,
	DSP_REG_PROD_64  =36,
	DSP_REG_MAX_MEM_BACKED = 36,

	DSP_REG_USED     =253,
	DSP_REG_STATIC   =254,
	DSP_REG_NONE     =255
};

enum DSPJitSignExtend {
    SIGN, ZERO, NONE
};

#ifdef _M_X64
#define NUMXREGS 16
#elif _M_IX86
#define NUMXREGS 8
#endif

class DSPJitRegCache {
private:
	struct X64CachedReg
	{
		int guest_reg; //including DSPJitRegSpecial
	};
	struct DynamicReg {
		Gen::OpArg loc;
		void *mem;
		size_t size;
		bool dirty;
	};

#ifdef _M_X64
	//when there is a way to do this efficiently in x86, uncondition
	struct {
		Gen::X64Reg host_reg;
		int shift;
		bool dirty;
		bool used;
		Gen::X64Reg tmp_reg;
	} acc[2];
#endif

	DynamicReg regs[DSP_REG_MAX_MEM_BACKED+1];
	X64CachedReg xregs[NUMXREGS];

	DSPEmitter &emitter;
	bool temporary;
	bool merged;
private:
	//find a free host reg
	Gen::X64Reg findFreeXReg();
	Gen::X64Reg spillXReg();
	void spillXReg(Gen::X64Reg reg);
public:
	DSPJitRegCache(DSPEmitter &_emitter);

	//for branching into multiple control flows
	DSPJitRegCache(const DSPJitRegCache &cache);
	DSPJitRegCache& operator=(const DSPJitRegCache &cache);

	~DSPJitRegCache();

	//merge must be done _before_ leaving the code branch, so we can fix
	//up any differences in state
	void flushRegs(DSPJitRegCache &cache, bool emit = true);
	/* since some use cases are non-trivial, some examples:

	   //this does not modify the final state of gpr
	   <code using gpr>
	   FixupBranch b = JCC();
	     DSPJitRegCache c = gpr;
	     <code using c>
	     gpr.flushRegs(c);
	   SetBranchTarget(b);
	   <code using gpr>

	   //this does not modify the final state of gpr
	   <code using gpr>
	   DSPJitRegCache c = gpr;
	   FixupBranch b1 = JCC();
	     <code using gpr>
	     gpr.flushRegs(c);
	     FixupBranch b2 = JMP();
	   SetBranchTarget(b1);
	     <code using gpr>
	     gpr.flushRegs(c);
	   SetBranchTarget(b2);
	   <code using gpr>

	   //this allows gpr to be modified in the second branch
	   //and fixes gpr according to the results form in the first branch
	   <code using gpr>
	   DSPJitRegCache c = gpr;
	   FixupBranch b1 = JCC();
	     <code using c>
	     FixupBranch b2 = JMP();
	   SetBranchTarget(b1);
	     <code using gpr>
	     gpr.flushRegs(c);
	   SetBranchTarget(b2);
	   <code using gpr>

	   //this does not modify the final state of gpr
	   <code using gpr>
	   u8* b = GetCodePtr();
	     DSPJitRegCache c = gpr;
	     <code using gpr>
	     gpr.flushRegs(c);
	     JCC(b);
	   <code using gpr>

	   this all is not needed when gpr would not be used at all in the
	   conditional branch
	 */
	//drop this copy without warning
	void drop();

	//prepare state so that another flushed DSPJitRegCache can take over
	void flushRegs();

	void loadStaticRegs();//load statically allocated regs from memory
	void saveStaticRegs();//save statically allocated regs to memory

	//gives no SCALE_RIP with abs(offset) >= 0x80000000
	void getReg(int reg, Gen::OpArg &oparg, bool load = true);
	//done with all usages of OpArg above
	void putReg(int reg, bool dirty = true);

	void readReg(int sreg, Gen::X64Reg host_dreg, DSPJitSignExtend extend);
	void writeReg(int dreg, Gen::OpArg arg);

	//find a free host reg, spill if used, reserve
	void getFreeXReg(Gen::X64Reg &reg);
	//spill a specific host reg if used, reserve
	void getXReg(Gen::X64Reg reg);
	//unreserve the given host reg
	void putXReg(Gen::X64Reg reg);
};

#endif /*_DSPJITREGCACHE_H*/
