/*====================================================================

   filename:     gdsp_interpreter.cpp
   project:      GCemu
   created:      2004-6-18
   mail:		  duddie@walla.com

   Copyright (c) 2005 Duddie & Tratax

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   ====================================================================*/

#include <stdio.h>
#include <stdlib.h>

#include "DSPTables.h"
#include "DSPHost.h"
#include "DSPCore.h"
#include "DSPAnalyzer.h"

#include "DSPHWInterface.h"
#include "DSPIntUtil.h"

namespace DSPInterpreter {

volatile u32 gdsp_running;

// NOTE: These have nothing to do with g_dsp.r[DSP_REG_CR].

// Hm, should instructions that change CR use this? Probably not (but they
// should call UpdateCachedCR())
void WriteCR(u16 val)
{
	// reset
	if (val & 0x0001)
	{
		DSPCore_Reset();
	}

	val &= ~0x0001;

	// update cr
	g_dsp.cr = val;
}

// Hm, should instructions that read CR use this? (Probably not).
u16 ReadCR()
{
	if (g_dsp.pc & 0x8000)
	{
		g_dsp.cr |= 0x800;
	}
	else
	{
		g_dsp.cr &= ~0x800;
	}

	return g_dsp.cr;
}

void Step()
{
	DSPCore_CheckExceptions();

	g_dsp.step_counter++;

#if PROFILE
	g_dsp.err_pc = g_dsp.pc;

	ProfilerAddDelta(g_dsp.err_pc, 1);
	if (g_dsp.step_counter == 1)
	{
		ProfilerInit();
	}

	if ((g_dsp.step_counter & 0xFFFFF) == 0)
	{
		ProfilerDump(g_dsp.step_counter);
	}
#endif
/*
	//Pikmin GC (US)
	if (g_dsp.pc == 0x0506)
		NOTICE_LOG(DSPLLE,"-> FORMAT JUMPTABLE --> pc:=%04x,ac0.m:=%04x,ac1.m:=%04x", g_dsp.pc, dsp_get_acc_m(0), dsp_get_acc_m(1));
*/
	
	u16 opc = dsp_fetch_code();
	ExecuteInstruction(UDSPInstruction(opc));
	HandleLoop();
}

// Used by thread mode.
void Run()
{
	gdsp_running = true;
	while (!(g_dsp.cr & CR_HALT))
	{
		// Are we running?
		if (DSPHost_Running() && !DSPHost_OnThread())
			break;

		// This number (500) is completely arbitrary. TODO: tweak.
		RunCyclesDebug(500);

		if (!gdsp_running)
			break;
	}
	gdsp_running = false;
}

// This one has basic idle skipping, and checks breakpoints.
int RunCyclesDebug(int cycles)
{
	// First, let's run a few cycles with no idle skipping so that things can
	// progress a bit.
	for (int i = 0; i < 8; i++)
	{
		if (g_dsp.cr & CR_HALT)
			return 0; 
		if (dsp_breakpoints.IsAddressBreakPoint(g_dsp.pc))
		{
			DSPCore_SetState(DSPCORE_STEPPING);
			return cycles;
		}
		Step();
		cycles--;
		if (cycles < 0)
			return 0;
	}

	DSPCore_CheckExternalInterrupt();
	
	// Now, let's run a few cycles with idle skipping.
	for (int i = 0; i < 8; i++)
	{
		if (g_dsp.cr & CR_HALT)
			return 0;
		if (dsp_breakpoints.IsAddressBreakPoint(g_dsp.pc))
		{
			DSPCore_SetState(DSPCORE_STEPPING);
			return cycles;
		}
		// Idle skipping.
		if (DSPAnalyzer::code_flags[g_dsp.pc] & DSPAnalyzer::CODE_IDLE_SKIP)
			return 0;

		Step();
		cycles--;
		if (cycles < 0)
			return 0;
	}

	// Finally, run the rest of the block without.
	while (cycles > 0)
	{
		if (dsp_breakpoints.IsAddressBreakPoint(g_dsp.pc))
		{
			DSPCore_SetState(DSPCORE_STEPPING);
			return cycles;
		}
		Step();
		cycles--;
	}

	return cycles;
}

// Used by non-thread mode. Meant to be efficient.
int RunCycles(int cycles)
{
	DSPCore_CheckExternalInterrupt();

	if (cycles < 18)
	{
		for (int i = 0; i < cycles; i++)
		{
			if (g_dsp.cr & CR_HALT)
				return 0;
			if (DSPAnalyzer::code_flags[g_dsp.pc] & DSPAnalyzer::CODE_IDLE_SKIP)
				return 0;
			Step();
			cycles--;
		}
		return cycles;
	}

	DSPCore_CheckExternalInterrupt();

	// First, let's run a few cycles with no idle skipping so that things can progress a bit.
	for (int i = 0; i < 8; i++)
	{
		if (g_dsp.cr & CR_HALT)
			return 0; 
		Step();
		cycles--;
	}

	// Next, let's run a few cycles with idle skipping, so that we can skip
	// idle loops.
	for (int i = 0; i < 8; i++)
	{
		if (g_dsp.cr & CR_HALT)
			return 0;
		if (DSPAnalyzer::code_flags[g_dsp.pc] & DSPAnalyzer::CODE_IDLE_SKIP)
			return 0;
		Step();
		cycles--;
	}

	// Now, run the rest of the block without idle skipping. It might trip into
	// a idle loop and if so we waste some time here. Might be beneficial to
	// slice even further.
	while (cycles > 0)
	{
		Step();
		cycles--;
		// We don't bother directly supporting pause - if the main emu pauses,
		// it just won't call this function anymore.
	}

	return cycles;
}

void Stop()
{
	gdsp_running = false;
}

}  // namespace
