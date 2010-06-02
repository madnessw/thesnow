// Copyright (C) 2003 Dolphin Project.

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

#include <stdio.h>
#include <cmath>
#include <assert.h>
#include <locale.h>

#include "Profiler.h"
#include "PixelShaderGen.h"
#include "XFMemory.h"  // for texture projection mode
#include "BPMemory.h"

PIXELSHADERUID last_pixel_shader_uid;

// Mash together all the inputs that contribute to the code of a generated pixel shader into
// a unique identifier, basically containing all the bits. Yup, it's a lot ....
// It would likely be a lot more efficient to build this incrementally as the attributes
// are set...
void GetPixelShaderId(PIXELSHADERUID *uid, u32 texturemask, u32 dstAlphaEnable)
{
	u32 projtexcoords = 0;
	for (u32 i = 0; i < (u32)bpmem.genMode.numtevstages + 1; i++) 
	{
		if (bpmem.tevorders[i/2].getEnable(i & 1)) 
		{
			int texcoord = bpmem.tevorders[i / 2].getTexCoord(i & 1);
			if (xfregs.texcoords[texcoord].texmtxinfo.projection)
				projtexcoords |= 1 << texcoord;
		}
	}
	uid->values[0] = (u32)bpmem.genMode.numtevstages |
				   ((u32)bpmem.genMode.numindstages << 4) |
				   ((u32)bpmem.genMode.numtexgens << 7) |
				   ((u32)dstAlphaEnable << 11) |
				   ((u32)((bpmem.alphaFunc.hex >> 16) & 0xff) << 12) |
				   (projtexcoords << 20) |
				   ((u32)bpmem.ztex2.op << 28);

	uid->values[0] = (uid->values[0] & ~0x0ff00000) | (projtexcoords << 20);
	// swap table
	for (int i = 0; i < 8; i += 2)
		((u8*)&uid->values[1])[i / 2] = (bpmem.tevksel[i].hex & 0xf) | ((bpmem.tevksel[i + 1].hex & 0xf) << 4);

	uid->values[2] = texturemask;

    u32 enableZTexture = (!bpmem.zcontrol.zcomploc && bpmem.zmode.testenable && bpmem.zmode.updateenable)?1:0;

    uid->values[3] = (u32)bpmem.fog.c_proj_fsel.fsel |
                   ((u32)bpmem.fog.c_proj_fsel.proj << 3) |
                   ((u32)enableZTexture << 4);

	int hdr = 4;
	u32 *pcurvalue = &uid->values[hdr];
	for (u32 i = 0; i < (u32)bpmem.genMode.numtevstages + 1; ++i) 
	{
		TevStageCombiner::ColorCombiner &cc = bpmem.combiners[i].colorC;
		TevStageCombiner::AlphaCombiner &ac = bpmem.combiners[i].alphaC;

		u32 val0 = cc.hex & 0xffffff;
		u32 val1 = ac.hex & 0xffffff;
		val0 |= bpmem.tevksel[i / 2].getKC(i & 1) << 24;
		val1 |= bpmem.tevksel[i / 2].getKA(i & 1) << 24;
		pcurvalue[0] = val0;
		pcurvalue[1] = val1;
		pcurvalue += 2;
	}

	for (u32 i = 0; i < ((u32)bpmem.genMode.numtevstages+1) / 2; ++i) 
	{
		u32 val0, val1;
		if (bpmem.tevorders[i].hex & 0x40)
			val0 = bpmem.tevorders[i].hex & 0x3ff;
		else
			val0 = bpmem.tevorders[i].hex & 0x380;
		if (bpmem.tevorders[i].hex & 0x40000)
			val1 = (bpmem.tevorders[i].hex & 0x3ff000) >> 12;
		else
			val1 = (bpmem.tevorders[i].hex & 0x380000) >> 12;

		switch (i % 3) {
			case 0: pcurvalue[0] = val0|(val1<<10); break;
			case 1: pcurvalue[0] |= val0<<20; pcurvalue[1] = val1; pcurvalue++; break;
			case 2: pcurvalue[1] |= (val0<<10)|(val1<<20); pcurvalue++; break;
			default: PanicAlert("Uknown case for Tev Stages / 2: %08x", (i % 3));
		}
	}

	if ((bpmem.genMode.numtevstages + 1) & 1) { // odd
		u32 val0;
		if (bpmem.tevorders[bpmem.genMode.numtevstages/2].hex & 0x40)
			val0 = bpmem.tevorders[bpmem.genMode.numtevstages/2].hex & 0x3ff;
		else
			val0 = bpmem.tevorders[bpmem.genMode.numtevstages/2].hex & 0x380;

		switch (bpmem.genMode.numtevstages % 3) 
		{
		case 0: pcurvalue[0] = val0; break;
		case 1: pcurvalue[0] |= val0 << 20; break;
		case 2: pcurvalue[1] |= val0 << 10; pcurvalue++; break;
		default: PanicAlert("Uknown case for Tev Stages: %08x", bpmem.genMode.numtevstages % 3);
		}
	}

	if ((bpmem.genMode.numtevstages % 3) != 2)
		++pcurvalue;

	uid->tevstages = (u32)(pcurvalue - &uid->values[0] - hdr);

	for (u32 i = 0; i < bpmem.genMode.numindstages; ++i) 
	{
		u32 val = bpmem.tevind[i].hex & 0x1fffff; // 21 bits
		switch (i % 3) 
		{
		case 0: pcurvalue[0] = val; break;
		case 1: pcurvalue[0] |= val << 21; pcurvalue[1] = val >> 11; ++pcurvalue; break;
		case 2: pcurvalue[0] |= val << 10; ++pcurvalue; break;
		default: PanicAlert("Uknown case for Ind Stages: %08x", (i % 3));
		}
	}

	// yeah, well ....
	uid->indstages = (u32)(pcurvalue - &uid->values[0] - (hdr - 1) - uid->tevstages);
}

//   old tev->pixelshader notes
//
//   color for this stage (alpha, color) is given by bpmem.tevorders[0].colorchan0
//   konstant for this stage (alpha, color) is given by bpmem.tevksel   
//   inputs are given by bpmem.combiners[0].colorC.a/b/c/d     << could be current chan color
//   according to GXTevColorArg table above
//   output is given by .outreg
//   tevtemp is set according to swapmodetables and 

static void WriteStage(char *&p, int n, u32 texture_mask, u32 HLSL);
static void SampleTexture(char *&p, const char *destination, const char *texcoords, const char *texswap, int texmap, u32 texture_mask, u32 HLSL);
// static void WriteAlphaCompare(char *&p, int num, int comp);
static bool WriteAlphaTest(char *&p, u32 HLSL);
static void WriteFog(char *&p);

const float epsilon8bit = 1.0f / 255.0f;

static const char *tevKSelTableC[] = // KCSEL
{
    "1.0f,1.0f,1.0f",    // 1   = 0x00
    "0.8745098f,0.8745098f,0.8745098f", // 7_8 = 0x01
    "0.7490196f,0.7490196f,0.7490196f",	 // 3_4 = 0x02
    "0.6235294f,0.6235294f,0.6235294f", // 5_8 = 0x03
    "0.4980392f,0.4980392f,0.4980392f",       // 1_2 = 0x04
    "0.372549f,0.372549f,0.372549f", // 3_8 = 0x05
    "0.2470588f,0.2470588f,0.2470588f",    // 1_4 = 0x06
    "0.1215686f,0.1215686f,0.1215686f", // 1_8 = 0x07
    "ERROR", // 0x08
    "ERROR", // 0x09
    "ERROR", // 0x0a
    "ERROR", // 0x0b
    I_KCOLORS"[0].rgb", // K0 = 0x0C
    I_KCOLORS"[1].rgb", // K1 = 0x0D
    I_KCOLORS"[2].rgb", // K2 = 0x0E
    I_KCOLORS"[3].rgb", // K3 = 0x0F
    I_KCOLORS"[0].rrr", // K0_R = 0x10
    I_KCOLORS"[1].rrr", // K1_R = 0x11
    I_KCOLORS"[2].rrr", // K2_R = 0x12
    I_KCOLORS"[3].rrr", // K3_R = 0x13
    I_KCOLORS"[0].ggg", // K0_G = 0x14 
    I_KCOLORS"[1].ggg", // K1_G = 0x15
    I_KCOLORS"[2].ggg", // K2_G = 0x16
    I_KCOLORS"[3].ggg", // K3_G = 0x17
    I_KCOLORS"[0].bbb", // K0_B = 0x18
    I_KCOLORS"[1].bbb", // K1_B = 0x19
    I_KCOLORS"[2].bbb", // K2_B = 0x1A
    I_KCOLORS"[3].bbb", // K3_B = 0x1B
    I_KCOLORS"[0].aaa", // K0_A = 0x1C
    I_KCOLORS"[1].aaa", // K1_A = 0x1D
    I_KCOLORS"[2].aaa", // K2_A = 0x1E
    I_KCOLORS"[3].aaa", // K3_A = 0x1F
};

static const char *tevKSelTableA[] = // KASEL
{
    "1.0f",  // 1   = 0x00
    "0.8745098f",// 7_8 = 0x01
    "0.7490196f", // 3_4 = 0x02
    "0.6235294f",// 5_8 = 0x03
    "0.4980392f",  // 1_2 = 0x04
    "0.372549f",// 3_8 = 0x05
    "0.2470588f", // 1_4 = 0x06
    "0.1215686f",// 1_8 = 0x07
    "ERROR", // 0x08
    "ERROR", // 0x09
    "ERROR", // 0x0a
    "ERROR", // 0x0b
    "ERROR", // 0x0c
    "ERROR", // 0x0d
    "ERROR", // 0x0e
    "ERROR", // 0x0f
    I_KCOLORS"[0].r", // K0_R = 0x10
    I_KCOLORS"[1].r", // K1_R = 0x11
    I_KCOLORS"[2].r", // K2_R = 0x12
    I_KCOLORS"[3].r", // K3_R = 0x13
    I_KCOLORS"[0].g", // K0_G = 0x14
    I_KCOLORS"[1].g", // K1_G = 0x15
    I_KCOLORS"[2].g", // K2_G = 0x16
    I_KCOLORS"[3].g", // K3_G = 0x17
    I_KCOLORS"[0].b", // K0_B = 0x18
    I_KCOLORS"[1].b", // K1_B = 0x19
    I_KCOLORS"[2].b", // K2_B = 0x1A
    I_KCOLORS"[3].b", // K3_B = 0x1B
    I_KCOLORS"[0].a", // K0_A = 0x1C
    I_KCOLORS"[1].a", // K1_A = 0x1D
    I_KCOLORS"[2].a", // K2_A = 0x1E
    I_KCOLORS"[3].a", // K3_A = 0x1F
};

static const char *tevScaleTable[] = // CS
{
    "1.0f",  // SCALE_1
    "2.0f",  // SCALE_2
    "4.0f",  // SCALE_4
    "0.5f",  // DIVIDE_2
};

static const char *tevBiasTable[] = // TB
{
    "",       // ZERO,
    "+0.4980392f",  // ADDHALF,
    "-0.4980392f",  // SUBHALF,
    "",
};

static const char *tevOpTable[] = { // TEV
    "+",      // TEVOP_ADD = 0,
    "-",      // TEVOP_SUB = 1,
};

//static const char *tevCompOpTable[] = { ">", "==" };

#define TEVCMP_R8    0
#define TEVCMP_GR16  1
#define TEVCMP_BGR24 2
#define TEVCMP_RGB8  3

static const char *tevCInputTable[] = // CC
{
    "(prev.rgb)",               // CPREV,
    "(prev.aaa)",         // APREV,
    "(c0.rgb)",                 // C0,
    "(c0.aaa)",           // A0,
    "(c1.rgb)",                 // C1,
    "(c1.aaa)",           // A1,
    "(c2.rgb)",                 // C2,
    "(c2.aaa)",           // A2,
    "(textemp.rgb)",            // TEXC,
    "(textemp.aaa)",      // TEXA,
    "(rastemp.rgb)",            // RASC,
    "(rastemp.aaa)",      // RASA,
    "float3(1.0f,1.0f,1.0f)",              // ONE
    "float3(0.4980392f,0.4980392f,0.4980392f)",                 // HALF
    "(konsttemp.rgb)", //"konsttemp.rgb",        // KONST
    "float3(0.0f,0.0f,0.0f)",              // ZERO
	///aded extra values to map clamped values
	"(cprev.rgb)",               // CPREV,
    "(cprev.aaa)",         // APREV,
    "(cc0.rgb)",                 // C0,
    "(cc0.aaa)",           // A0,
    "(cc1.rgb)",                 // C1,
    "(cc1.aaa)",           // A1,
    "(cc2.rgb)",                 // C2,
    "(cc2.aaa)",           // A2,
    "(textemp.rgb)",            // TEXC,
    "(textemp.aaa)",      // TEXA,
    "(rastemp.rgb)",            // RASC,
    "(rastemp.aaa)",      // RASA,
    "float3(1.0f,1.0f,1.0f)",              // ONE
    "float3(0.4980392f,0.4980392f,0.4980392f)",                 // HALF
    "(konsttemp.rgb)", //"konsttemp.rgb",        // KONST
    "float3(0.0f,0.0f,0.0f)",              // ZERO    
    "PADERROR",	"PADERROR",	"PADERROR",	"PADERROR",
};

static const char *tevAInputTable[] = // CA
{
    "prev",            // APREV,
    "c0",              // A0,
    "c1",              // A1,
    "c2",              // A2,
    "textemp",         // TEXA,
    "rastemp",         // RASA,
    "konsttemp",       // KONST,  (hw1 had quarter)
    "float4(0.0f,0.0f,0.0f,0.0f)", // ZERO
    ///aded extra values to map clamped values
	"cprev",            // APREV,
    "cc0",              // A0,
    "cc1",              // A1,
    "cc2",              // A2,
    "textemp",         // TEXA,
    "rastemp",         // RASA,
    "konsttemp",       // KONST,  (hw1 had quarter)
    "float4(0.0f,0.0f,0.0f,0.0f)", // ZERO
    "PADERROR", "PADERROR", "PADERROR", "PADERROR",
    "PADERROR", "PADERROR", "PADERROR", "PADERROR",
};	

static const char *tevRasTable[] =
{
    "colors_0",
    "colors_1",
    "ERROR", //2
    "ERROR", //3
    "ERROR", //4
    "alphabump", // use bump alpha
    "(alphabump*(255.0f/248.0f))", //normalized
    "float4(0.0f,0.0f,0.0f,0.0f)", // zero
};

static const char *alphaRef[2] = 
{
    I_ALPHA"[0].r",
    I_ALPHA"[0].g"
};

//static const char *tevTexFunc[] = { "tex2D", "texRECT" };

static const char *tevCOutputTable[]  = { "prev.rgb", "c0.rgb", "c1.rgb", "c2.rgb" };
static const char *tevAOutputTable[]  = { "prev.a", "c0.a", "c1.a", "c2.a" };
static const char *tevIndAlphaSel[]   = {"", "x", "y", "z"};
//static const char *tevIndAlphaScale[] = {"", "*32","*16","*8"};
static const char *tevIndAlphaScale[] = {"*(248.0f/255.0f)", "*(224.0f/255.0f)","*(240.0f/255.0f)","*(248.0f/255.0f)"};
static const char *tevIndBiasField[]  = {"", "x", "y", "xy", "z", "xz", "yz", "xyz"}; // indexed by bias
static const char *tevIndBiasAdd[]    = {"-128.0f", "1.0f", "1.0f", "1.0f" }; // indexed by fmt
static const char *tevIndWrapStart[]  = {"0.0f", "256.0f", "128.0f", "64.0f", "32.0f", "16.0f", "0.001f" };
static const char *tevIndFmtScale[]   = {"255.0f", "31.0f", "15.0f", "7.0f" };

#define WRITE p+=sprintf

static const char *swapColors = "rgba";
static char swapModeTable[4][5];

static char text[16384];

static void BuildSwapModeTable()
{
    for (int i = 0; i < 4; i++)
    {
        swapModeTable[i][0] = swapColors[bpmem.tevksel[i*2].swap1];
        swapModeTable[i][1] = swapColors[bpmem.tevksel[i*2].swap2];
        swapModeTable[i][2] = swapColors[bpmem.tevksel[i*2+1].swap1];
        swapModeTable[i][3] = swapColors[bpmem.tevksel[i*2+1].swap2];
        swapModeTable[i][4] = 0;
    }
}

const char *GeneratePixelShaderCode(u32 texture_mask, bool dstAlphaEnable, u32 HLSL)
{
	setlocale(LC_NUMERIC, "C"); // Reset locale for compilation
	text[sizeof(text) - 1] = 0x7C;  // canary
    DVSTARTPROFILE();

    BuildSwapModeTable();
    int numStages = bpmem.genMode.numtevstages + 1;
    int numTexgen = bpmem.genMode.numtexgens;

    char *p = text;
    WRITE(p, "//Pixel Shader for TEV stages\n");
    WRITE(p, "//%i TEV stages, %i texgens, %i IND stages\n",
		numStages, numTexgen, bpmem.genMode.numindstages);

    int nIndirectStagesUsed = 0;
    if (bpmem.genMode.numindstages > 0) 
	{
        for (int i = 0; i < numStages; ++i) 
		{
            if (bpmem.tevind[i].IsActive() && bpmem.tevind[i].bt < bpmem.genMode.numindstages) 
                nIndirectStagesUsed |= 1 << bpmem.tevind[i].bt;
        }
    }

    // Declare samplers
    if (texture_mask)
	{
		if (HLSL)
			WRITE(p, "uniform sampler ");
		else
			WRITE(p, "uniform samplerRECT ");
        bool bfirst = true;
		for (int i = 0; i < 8; ++i)
		{
            if (texture_mask & (1<<i))
			{
                WRITE(p, "%s samp%d : register(s%d)", bfirst?"":",", i, i);
                bfirst = false;
            }
		}
        WRITE(p, ";\n");
	}

    if (texture_mask != 0xff) {
        WRITE(p, "uniform sampler2D ");
        bool bfirst = true;
        for (int i = 0; i < 8; ++i) {
            if (!(texture_mask & (1<<i))) {
                WRITE(p, "%s samp%d : register(s%d)", bfirst?"":",",i, i);
                bfirst = false;
            }
        }
        WRITE(p, ";\n");
    }

    WRITE(p, "\n");

    WRITE(p, "uniform float4 "I_COLORS"[4] : register(c%d);\n", C_COLORS);
    WRITE(p, "uniform float4 "I_KCOLORS"[4] : register(c%d);\n", C_KCOLORS);
    WRITE(p, "uniform float4 "I_ALPHA"[1] : register(c%d);\n", C_ALPHA);
    WRITE(p, "uniform float4 "I_TEXDIMS"[8] : register(c%d);\n", C_TEXDIMS);
    WRITE(p, "uniform float4 "I_ZBIAS"[2] : register(c%d);\n", C_ZBIAS);
    WRITE(p, "uniform float4 "I_INDTEXSCALE"[2] : register(c%d);\n", C_INDTEXSCALE);
    WRITE(p, "uniform float4 "I_INDTEXMTX"[6] : register(c%d);\n", C_INDTEXMTX);
    WRITE(p, "uniform float4 "I_FOG"[2] : register(c%d);\n", C_FOG);

    WRITE(p, "void main(\n");
    WRITE(p, "  out float4 ocol0 : COLOR0,\n");	
    WRITE(p, "  out float depth : DEPTH,\n");

    // compute window position if needed because binding semantic WPOS is not widely supported
	if (numTexgen < 7) 
	{
		for (int i = 0; i < numTexgen; ++i)
			WRITE(p, "  in float3 uv%d : TEXCOORD%d, \n", i, i);

		WRITE(p, "  in float4 clipPos : TEXCOORD%d, \n", numTexgen);
	} 
	else 
	{
		// wpos is in w of first 4 texcoords
		for (int i = 0; i < numTexgen; ++i)
			WRITE(p, "  in float%d uv%d : TEXCOORD%d, \n", i<4?4:3, i, i);
	}

	WRITE(p, "  in float4 colors_0 : COLOR0,\n in float4 colors_1 : COLOR1){\n");

    char* pmainstart = p;

    WRITE(p, "  float4 c0="I_COLORS"[1],c1="I_COLORS"[2],c2="I_COLORS"[3],prev=float4(0.0f,0.0f,0.0f,0.0f),textemp,rastemp,konsttemp=float4(0.0f,0.0f,0.0f,0.0f);\n"
            "  float3 comp16 = float3(1.0f,255.0f,0.0f), comp24 = float3(1.0f,255.0f,255.0f*255.0f);\n"
            "  float4 alphabump=0;\n"
            "  float3 tevcoord;\n"
            "  float2 wrappedcoord, tempcoord;\n"
			"  float4 cc0,cc1,cc2,cprev;\n\n");

	// HACK to handle cases where the tex gen is not enabled
    if (numTexgen == 0)
	{
        WRITE(p, "float3 uv0 = float3(0.0f,0.0f,0.0f);\n");
	}
	else
	{
		for (int i = 0; i < numTexgen; ++i) 
		{
			// optional perspective divides
			if (xfregs.texcoords[i].texmtxinfo.projection == XF_TEXPROJ_STQ)
				WRITE(p, "uv%d.xy = uv%d.xy/uv%d.z;\n", i, i, i);

			// scale texture coordinates
			WRITE(p, "uv%d.xy = uv%d.xy * "I_TEXDIMS"[%d].zw;\n", i, i, i);        
		}
	}

    // indirect texture map lookup
    for(u32 i = 0; i < bpmem.genMode.numindstages; ++i) 
	{
        if (nIndirectStagesUsed & (1<<i)) 
		{
            int texcoord = bpmem.tevindref.getTexCoord(i);

            if (texcoord < numTexgen)
                WRITE(p, "tempcoord=uv%d.xy * "I_INDTEXSCALE"[%d].%s;\n", texcoord, i/2, (i&1)?"zw":"xy");
            else
                WRITE(p, "tempcoord=float2(0.0f,0.0f);\n");

            char buffer[32];
            sprintf(buffer, "float3 indtex%d", i);
            SampleTexture(p, buffer, "tempcoord", "abg", bpmem.tevindref.getTexMap(i), texture_mask,HLSL);
        }
    }

    

	for (int i = 0; i < numStages; i++)
		WriteStage(p, i, texture_mask,HLSL); //build the equation for this stage
	if(numStages)
	{
		// The results of the last texenv stage are put onto the screen,
		// regardless of the used destination register
		WRITE(p, "prev.rgb = %s;\n",tevCOutputTable[bpmem.combiners[numStages-1].colorC.dest]);
		WRITE(p, "prev.a = %s;\n",tevAOutputTable[bpmem.combiners[numStages-1].alphaC.dest]);
	}

	// emulation of unisgned 8 overflow when casting
	WRITE(p, "prev = frac(4.0f + prev * 0.99609375f) *  1.00392157f;\n");//fmod(fmod(prev * 255.0f,256.0f) + 256.0f,256.0f) * 0.0039215686f;\n");		
		
    if (!WriteAlphaTest(p, HLSL))
	{
        // alpha test will always fail, so restart the shader and just make it an empty function
        p = pmainstart;
        WRITE(p, "ocol0 = 0;\n");
		WRITE(p, "discard;return;\n");
    }
    else
	{
		if (numTexgen >= 7)
			WRITE(p, "float4 clipPos = float4(uv0.w, uv1.w, uv2.w, uv3.w);\n");

		// the screen space depth value = far z + (clip z / clip w) * z range
		WRITE(p, "float zCoord = "I_ZBIAS"[1].x + (clipPos.z / clipPos.w) * "I_ZBIAS"[1].y;\n");

		if (bpmem.ztex2.op != ZTEXTURE_DISABLE && !bpmem.zcontrol.zcomploc && bpmem.zmode.testenable && bpmem.zmode.updateenable)
		{
			// use the texture input of the last texture stage (textemp), hopefully this has been read and is in correct format...
			if (bpmem.ztex2.op == ZTEXTURE_ADD)
				WRITE(p, "zCoord = dot("I_ZBIAS"[0].xyzw, textemp.xyzw) + "I_ZBIAS"[1].w + zCoord;\n");
			else
				WRITE(p, "zCoord = dot("I_ZBIAS"[0].xyzw, textemp.xyzw) + "I_ZBIAS"[1].w;\n");

			// scale to make result from frac correct
			WRITE(p, "zCoord = zCoord * (16777215.0f/16777216.0f);\n");
			WRITE(p, "zCoord = frac(zCoord);\n");
			WRITE(p, "zCoord = zCoord * (16777216.0f/16777215.0f);\n");
		}
	    
		WRITE(p, "depth = zCoord;\n");
        
		if (dstAlphaEnable) 
            WRITE(p, "  ocol0 = float4(prev.rgb,"I_ALPHA"[0].a);\n");
		else
		{
            WriteFog(p);
            WRITE(p, "  ocol0 = prev;\n");
        }
    }
    
    WRITE(p, "}\n");

	if (text[sizeof(text) - 1] != 0x7C)
		PanicAlert("PixelShader generator - buffer too small, canary has been eaten!");

	setlocale(LC_NUMERIC, ""); // restore locale
    return text;
}



//table with the color compare operations
static const char *TEVCMPColorOPTable[16] =
{
	"float3(0.0f,0.0f,0.0f)",//0
	"float3(0.0f,0.0f,0.0f)",//1
	"float3(0.0f,0.0f,0.0f)",//2
	"float3(0.0f,0.0f,0.0f)",//3
	"float3(0.0f,0.0f,0.0f)",//4
	"float3(0.0f,0.0f,0.0f)",//5
	"float3(0.0f,0.0f,0.0f)",//6
	"float3(0.0f,0.0f,0.0f)",//7
	"   %s + ((%s.r >= %s.r + (0.25f/255.0f)) ? %s : float3(0.0f,0.0f,0.0f))",//#define TEVCMP_R8_GT 8
	"   %s + ((abs(%s.r - %s.r) < (0.5f/255.0f)) ? %s : float3(0.0f,0.0f,0.0f))",//#define TEVCMP_R8_EQ 9
	"   %s + (( dot(%s.rgb, comp16) >= (dot(%s.rgb, comp16) + (0.25f/255.0f))) ? %s : float3(0.0f,0.0f,0.0f))",//#define TEVCMP_GR16_GT 10
	"   %s + (abs(dot(%s.rgb, comp16) - dot(%s.rgb, comp16)) < (0.5f/255.0f) ? %s : float3(0.0f,0.0f,0.0f))",//#define TEVCMP_GR16_EQ 11
	"   %s + (( dot(%s.rgb, comp24) >= (dot(%s.rgb, comp24) + (0.25f/255.0f))) ? %s : float3(0.0f,0.0f,0.0f))",//#define TEVCMP_BGR24_GT 12
	"   %s + (abs(dot(%s.rgb, comp24) - dot(%s.rgb, comp24)) < (0.5f/255.0f) ? %s : float3(0.0f,0.0f,0.0f))",//#define TEVCMP_BGR24_EQ 13
	"   %s + (max(sign(%s.rgb - %s.rgb - (0.25f/255.0f)),float3(0.0f,0.0f,0.0f)) * %s)",//#define TEVCMP_RGB8_GT  14
	"   %s + ((float3(1.0f,1.0f,1.0f) - max(sign(abs(%s.rgb - %s.rgb) - (0.5f/255.0f)),float3(0.0f,0.0f,0.0f))) * %s)"//#define TEVCMP_RGB8_EQ  15
};

//table with the alpha compare operations
static const char *TEVCMPAlphaOPTable[16] =
{
	"0.0f",//0
	"0.0f",//1
	"0.0f",//2
	"0.0f",//3
	"0.0f",//4
	"0.0f",//5
	"0.0f",//6
	"0.0f",//7
	"   %s.a + ((%s.r >= (%s.r + (0.25f/255.0f))) ? %s.a : 0.0f)",//#define TEVCMP_R8_GT 8
	"   %s.a + (abs(%s.r - %s.r) < (0.5f/255.0f) ? %s.a : 0.0f)",//#define TEVCMP_R8_EQ 9
	"   %s.a + ((dot(%s.rgb, comp16) >= (dot(%s.rgb, comp16) + (0.25f/255.0f))) ? %s.a : 0.0f)",//#define TEVCMP_GR16_GT 10
	"   %s.a + (abs(dot(%s.rgb, comp16) - dot(%s.rgb, comp16)) < (0.5f/255.0f) ? %s.a : 0.0f)",//#define TEVCMP_GR16_EQ 11
	"   %s.a + ((dot(%s.rgb, comp24) >= (dot(%s.rgb, comp24) + (0.25f/255.0f))) ? %s.a : 0.0f)",//#define TEVCMP_BGR24_GT 12
	"   %s.a + (abs(dot(%s.rgb, comp24) - dot(%s.rgb, comp24)) < (0.5f/255.0f) ? %s.a : 0.0f)",//#define TEVCMP_BGR24_EQ 13	
	"   %s.a + ((%s.a >= (%s.a + (0.25f/255.0f))) ? %s.a : 0.0f)",//#define TEVCMP_A8_GT 14
	"   %s.a + (abs(%s.a - %s.a) < (0.5f/255.0f) ? %s.a : 0.0f)"//#define TEVCMP_A8_EQ 15

};


static void WriteStage(char *&p, int n, u32 texture_mask, u32 HLSL)
{
    char *rasswap = swapModeTable[bpmem.combiners[n].alphaC.rswap];
    char *texswap = swapModeTable[bpmem.combiners[n].alphaC.tswap];


    int texcoord = bpmem.tevorders[n/2].getTexCoord(n&1);
    bool bHasTexCoord = (u32)texcoord < bpmem.genMode.numtexgens;
    bool bHasIndStage = bpmem.tevind[n].IsActive() && bpmem.tevind[n].bt < bpmem.genMode.numindstages;

    // HACK to handle cases where the tex gen is not enabled
    if (!bHasTexCoord)
        texcoord = 0;

    if (bHasIndStage) 
	{
        // perform the indirect op on the incoming regular coordinates using indtex%d as the offset coords
		if (bpmem.tevind[n].bs != ITBA_OFF) 
		{
			WRITE(p, "alphabump = indtex%d.%s %s;\n", 
			bpmem.tevind[n].bt, 
			tevIndAlphaSel[bpmem.tevind[n].bs], 
			tevIndAlphaScale[bpmem.tevind[n].fmt]);			
		}		
        // format
        WRITE(p, "float3 indtevcrd%d = indtex%d * %s;\n", n, bpmem.tevind[n].bt, tevIndFmtScale[bpmem.tevind[n].fmt]);

        // bias
        if (bpmem.tevind[n].bias != ITB_NONE )
            WRITE(p, "indtevcrd%d.%s += %s;\n", n, tevIndBiasField[bpmem.tevind[n].bias], tevIndBiasAdd[bpmem.tevind[n].fmt]);

        // multiply by offset matrix and scale
        if (bpmem.tevind[n].mid != 0)
		{
            if (bpmem.tevind[n].mid <= 3) 
			{
                int mtxidx = 2*(bpmem.tevind[n].mid-1);
                WRITE(p, "float2 indtevtrans%d = float2(dot("I_INDTEXMTX"[%d].xyz, indtevcrd%d), dot("I_INDTEXMTX"[%d].xyz, indtevcrd%d));\n",
                    n, mtxidx, n, mtxidx+1, n);
            }
            else if (bpmem.tevind[n].mid <= 7 && bHasTexCoord)
			{ // s matrix
                int mtxidx = 2*(bpmem.tevind[n].mid-5);
                WRITE(p, "float2 indtevtrans%d = "I_INDTEXMTX"[%d].ww * uv%d.xy * indtevcrd%d.xx;\n", n, mtxidx, texcoord, n);
            }
            else if (bpmem.tevind[n].mid <= 11 && bHasTexCoord)
			{ // t matrix
                int mtxidx = 2*(bpmem.tevind[n].mid-9);
                WRITE(p, "float2 indtevtrans%d = "I_INDTEXMTX"[%d].ww * uv%d.xy * indtevcrd%d.yy;\n", n, mtxidx, texcoord, n);
            }
            else
                WRITE(p, "float2 indtevtrans%d = 0;\n", n);
        }
        else
            WRITE(p, "float2 indtevtrans%d = 0;\n", n);

        // ---------
		// Wrapping
		// ---------

        // wrap S
        if (bpmem.tevind[n].sw == ITW_OFF)
            WRITE(p, "wrappedcoord.x = uv%d.x;\n", texcoord);
        else if (bpmem.tevind[n].sw == ITW_0)
            WRITE(p, "wrappedcoord.x = 0.0f;\n");
        else 
            WRITE(p, "wrappedcoord.x = fmod( uv%d.x, %s );\n", texcoord, tevIndWrapStart[bpmem.tevind[n].sw]);

        // wrap T
        if (bpmem.tevind[n].tw == ITW_OFF)
            WRITE(p, "wrappedcoord.y = uv%d.y;\n", texcoord);
        else if (bpmem.tevind[n].tw == ITW_0)
            WRITE(p, "wrappedcoord.y = 0.0f;\n");
        else
            WRITE(p, "wrappedcoord.y = fmod( uv%d.y, %s );\n", texcoord, tevIndWrapStart[bpmem.tevind[n].tw]);

        if (bpmem.tevind[n].fb_addprev) // add previous tevcoord
            WRITE(p, "tevcoord.xy += wrappedcoord + indtevtrans%d;\n", n);
        else
            WRITE(p, "tevcoord.xy = wrappedcoord + indtevtrans%d;\n", n);
    }

    WRITE(p, "rastemp=%s.%s;\n", tevRasTable[bpmem.tevorders[n / 2].getColorChan(n & 1)],rasswap);

    if (bpmem.tevorders[n/2].getEnable(n&1)) 
	{
        int texmap = bpmem.tevorders[n/2].getTexMap(n&1);
        if(!bHasIndStage) 
		{
            // calc tevcord
            if(bHasTexCoord)
                WRITE(p, "tevcoord.xy = uv%d.xy;\n", texcoord);
            else
                WRITE(p, "tevcoord.xy = float2(0.0f,0.0f);\n");
        }

        SampleTexture(p, "textemp", "tevcoord", texswap, texmap, texture_mask, HLSL);
    }
    else
        WRITE(p, "textemp=float4(1.0f,1.0f,1.0f,1.0f);\n");

    int kc = bpmem.tevksel[n / 2].getKC(n & 1);
    int ka = bpmem.tevksel[n / 2].getKA(n & 1);

    TevStageCombiner::ColorCombiner &cc = bpmem.combiners[n].colorC;
    TevStageCombiner::AlphaCombiner &ac = bpmem.combiners[n].alphaC;

    bool bCKonst = cc.a == TEVCOLORARG_KONST || cc.b == TEVCOLORARG_KONST || cc.c == TEVCOLORARG_KONST || cc.d == TEVCOLORARG_KONST;
    bool bAKonst = ac.a == TEVALPHAARG_KONST || ac.b == TEVALPHAARG_KONST || ac.c == TEVALPHAARG_KONST || ac.d == TEVALPHAARG_KONST;
    if (bCKonst || bAKonst )
        WRITE(p, "konsttemp=float4(%s,%s);\n",tevKSelTableC[kc],tevKSelTableA[ka]);  

	if(cc.a == 0 || cc.a == 1 || cc.b == 0 || cc.b == 1 || cc.c == 0 || cc.c == 1 || ac.a == 0 || ac.b == 0 || ac.c == 0)
		WRITE(p, "cprev = frac(4.0f + prev * 0.99609375f) *  1.00392157f;\n");  
	if(cc.a == 2 || cc.a == 3 || cc.b == 2 || cc.b == 3 || cc.c == 2 || cc.c == 3 || ac.a == 1 || ac.b == 1 || ac.c == 1)
		WRITE(p, "cc0 = frac(4.0f + c0 * 0.99609375f) *  1.00392157f;\n");
	if(cc.a == 4 || cc.a == 5 || cc.b == 4 || cc.b == 5 || cc.c == 4 || cc.c == 5 || ac.a == 2 || ac.b == 2 || ac.c == 2)
		WRITE(p, "cc1 = frac(4.0f + c1 * 0.99609375f) *  1.00392157f;\n");  
	if(cc.a == 6 || cc.a == 7 || cc.b == 6 || cc.b == 7 || cc.c == 6 || cc.c == 7 || ac.a == 3 || ac.b == 3 || ac.c == 3)
		WRITE(p, "cc2 = frac(4.0f + c2 * 0.99609375f) *  1.00392157f;\n");  	

    if (cc.clamp)
		WRITE(p, "%s=saturate(", tevCOutputTable[cc.dest]);
	else
		WRITE(p, "%s=", tevCOutputTable[cc.dest]);

    // combine the color channel
    if (cc.bias != 3) // if not compare
	{
		//normal color combiner goes here        
		if (cc.shift>0)
			WRITE(p, "%s*(",tevScaleTable[cc.shift]);		

		if(!(cc.d == TEVCOLORARG_ZERO && cc.op == 0))
			WRITE(p, "%s%s",tevCInputTable[cc.d],tevOpTable[cc.op]);

		if (cc.a == cc.b)
			WRITE(p,"%s",tevCInputTable[cc.a + 16]);
		else if (cc.c == TEVCOLORARG_ZERO)
			WRITE(p,"%s",tevCInputTable[cc.a + 16]);
		else if (cc.c == TEVCOLORARG_ONE)
			WRITE(p,"%s",tevCInputTable[cc.b + 16]);
		else if (cc.a == TEVCOLORARG_ZERO)
			WRITE(p,"%s*(trunc(%s * 256.9921875f)*0.00390625f)",tevCInputTable[cc.b + 16],tevCInputTable[cc.c + 16]);
		else if (cc.b == TEVCOLORARG_ZERO)
			WRITE(p,"%s*(float3(1.0f,1.0f,1.0f)-(trunc(%s * 256.9921875f)*0.00390625f))",tevCInputTable[cc.a + 16],tevCInputTable[cc.c + 16]);
		else
			WRITE(p, "lerp(%s,%s,(trunc(%s * 256.9921875f)*0.00390625f))",tevCInputTable[cc.a + 16], tevCInputTable[cc.b + 16],tevCInputTable[cc.c + 16]);
		
		WRITE(p, "%s",tevBiasTable[cc.bias]);
		
		if(cc.shift>0)
			WRITE(p, ")");
    }
    else 
	{
        int cmp = (cc.shift<<1)|cc.op|8; // comparemode stored here
		WRITE(p, TEVCMPColorOPTable[cmp],//lookup the function from the op table
                tevCInputTable[cc.d], 
				tevCInputTable[cc.a + 16],
				tevCInputTable[cc.b + 16],
				tevCInputTable[cc.c + 16]);       
    }
	if (cc.clamp)
		WRITE(p,")");
	WRITE(p,";\n");	
    
    // combine the alpha channel
    if (ac.clamp)
	    WRITE(p, "%s=saturate(", tevAOutputTable[ac.dest]);
	else
		WRITE(p, "%s=", tevAOutputTable[ac.dest]);

    if (ac.bias != 3) // if not compare
	{
        //normal alpha combiner goes here
		if (ac.shift>0)
			WRITE(p, "%s*(",tevScaleTable[ac.shift]);		

		if(!(ac.d == TEVALPHAARG_ZERO && ac.op == 0))
			WRITE(p, "%s.a%s",tevAInputTable[ac.d],tevOpTable[ac.op]);		

		if (ac.a == ac.b)
			WRITE(p,"%s.a",tevAInputTable[ac.a + 8]);
		else if (ac.c == TEVALPHAARG_ZERO)
			WRITE(p,"%s.a",tevAInputTable[ac.a + 8]);
		else if (ac.a == TEVALPHAARG_ZERO)
			WRITE(p,"%s.a*(trunc(%s.a * 256.9921875f)*0.00390625f)",tevAInputTable[ac.b + 8],tevAInputTable[ac.c + 8]);
		else if (ac.b == TEVALPHAARG_ZERO)
			WRITE(p,"%s.a*(1.0f-(trunc(%s.a * 256.9921875f)*0.00390625f))",tevAInputTable[ac.a + 8],tevAInputTable[ac.c + 8]);
		else
	        WRITE(p, "lerp(%s.a,%s.a,(trunc(%s.a * 256.9921875f)*0.00390625f))",tevAInputTable[ac.a + 8],tevAInputTable[ac.b + 8],tevAInputTable[ac.c + 8]);
		
		WRITE(p, "%s",tevBiasTable[ac.bias]);
		
		if (ac.shift>0)
			WRITE(p, ")");

    }
    else 
	{
        //compare alpha combiner goes here
        int cmp = (ac.shift<<1)|ac.op|8; // comparemode stored here
		WRITE(p, TEVCMPAlphaOPTable[cmp],
                tevAInputTable[ac.d],
				tevAInputTable[ac.a + 8],
				tevAInputTable[ac.b + 8],
				tevAInputTable[ac.c + 8]);       		
    }
	if (ac.clamp)
		WRITE(p, ")");
	WRITE(p, ";\n\n");	
}

void SampleTexture(char *&p, const char *destination, const char *texcoords, const char *texswap, int texmap, u32 texture_mask, u32 HLSL)
{
    if (texture_mask & (1<<texmap)) {
        // non pow 2
         bool bwraps = (texture_mask & (0x100<<texmap)) ? true : false;
         bool bwrapt = (texture_mask & (0x10000<<texmap)) ? true : false;

         if (bwraps || bwrapt) {
             if (bwraps) {
                 WRITE(p, "tempcoord.x = fmod(%s.x, "I_TEXDIMS"[%d].x);\n", texcoords, texmap);
             }
             else {
                 WRITE(p, "tempcoord.x = %s.x;\n", texcoords);
             }

             if (bwrapt) {
                 WRITE(p, "tempcoord.y = fmod(%s.y, "I_TEXDIMS"[%d].y);\n", texcoords, texmap);
             }
             else {
                 WRITE(p, "tempcoord.y = %s.y;\n", texcoords);
             }

			 if (HLSL)
				 WRITE(p, "%s=tex2D(samp%d,tempcoord.xy).%s;\n", destination, texmap, texswap);
			 else
				 WRITE(p, "%s=texRECT(samp%d,tempcoord.xy).%s;\n", destination, texmap, texswap);
         }
         else {
			 if (HLSL)
				 WRITE(p, "%s=tex2D(samp%d,%s.xy).%s;\n", destination, texmap, texcoords, texswap);
			 else
				 WRITE(p, "%s=texRECT(samp%d,%s.xy).%s;\n", destination, texmap, texcoords, texswap);
         }
    }
    else {
        WRITE(p, "%s=tex2D(samp%d,%s.xy * "I_TEXDIMS"[%d].xy).%s;\n", destination, texmap, texcoords, texmap, texswap);
    }
}

static const char *tevAlphaFuncsTable[] = 
{
    "(false)",									//ALPHACMP_NEVER 0
	"(prev.a <= %s - (0.25f/255.0f))",			//ALPHACMP_LESS 1
	"(abs( prev.a - %s ) < (0.5f/255.0f))",		//ALPHACMP_EQUAL 2
	"(prev.a < %s + (0.25f/255.0f))",			//ALPHACMP_LEQUAL 3
	"(prev.a >= %s + (0.25f/255.0f))",			//ALPHACMP_GREATER 4
	"(abs( prev.a - %s ) >= (0.5f/255.0f))",	//ALPHACMP_NEQUAL 5
	"(prev.a > %s - (0.25f/255.0f))",			//ALPHACMP_GEQUAL 6
	"(true)"									//ALPHACMP_ALWAYS 7
};

static const char *tevAlphaFunclogicTable[] =
{
	" && ", // and
    " || ",	// or
    " != ",	// xor
    " == "	// xnor
};

static bool WriteAlphaTest(char *&p, u32 HLSL)
{
    u32 op = bpmem.alphaFunc.logic;
    u32 comp[2] = {bpmem.alphaFunc.comp0,bpmem.alphaFunc.comp1};

    //first kill all the simple cases
    switch(op) 
	{
    case 0: // AND
        if (comp[0] == ALPHACMP_ALWAYS && comp[1] == ALPHACMP_ALWAYS) return true;
        if (comp[0] == ALPHACMP_NEVER || comp[1] == ALPHACMP_NEVER) return false;
        break;
    case 1: // OR
        if (comp[0] == ALPHACMP_ALWAYS || comp[1] == ALPHACMP_ALWAYS) return true;
        if (comp[0] == ALPHACMP_NEVER && comp[1] == ALPHACMP_NEVER)return false;
        break;
    case 2: // XOR
        if ((comp[0] == ALPHACMP_ALWAYS && comp[1] == ALPHACMP_NEVER) || (comp[0] == ALPHACMP_NEVER && comp[1] == ALPHACMP_ALWAYS)) 
			return true;
        if ((comp[0] == ALPHACMP_ALWAYS && comp[1] == ALPHACMP_ALWAYS) || (comp[0] == ALPHACMP_NEVER && comp[1] == ALPHACMP_NEVER)) 
		    return false;        
        break;
    case 3: // XNOR
        if ((comp[0] == ALPHACMP_ALWAYS && comp[1] == ALPHACMP_NEVER) || (comp[0] == ALPHACMP_NEVER && comp[1] == ALPHACMP_ALWAYS)) 
			return false;        
        if ((comp[0] == ALPHACMP_ALWAYS && comp[1] == ALPHACMP_ALWAYS) || (comp[0] == ALPHACMP_NEVER && comp[1] == ALPHACMP_NEVER))
            return true;
        break;
	default: PanicAlert("bad logic for alpha test? %08x", op);
    }


	// using discard then return works the same in cg and hlsl
	WRITE(p, "if(!( ");

	int compindex = bpmem.alphaFunc.comp0 % 8;
	WRITE(p, tevAlphaFuncsTable[compindex],alphaRef[0]);//lookup the first component from the alpha function table
    
	WRITE(p, "%s", tevAlphaFunclogicTable[bpmem.alphaFunc.logic % 4]);//lookup the logic op
   
    compindex = bpmem.alphaFunc.comp1 % 8;
	WRITE(p, tevAlphaFuncsTable[compindex],alphaRef[1]);//lookup the second component from the alpha function table    

	WRITE(p, ")){ocol0 = 0;discard;return;}\n");
	
    return true;
}

static const char *tevFogFuncsTable[] =
{
	"",																//No Fog
	"",																//?
	"",																//Linear
	"",																//?
	"  fog = 1.0f - pow(2, -8.0f * fog);\n",						//exp
	"  fog = 1.0f - pow(2, -8.0f * fog * fog);\n",					//exp2
	"  fog = pow(2, -8.0f * (1.0f - fog));\n",						//backward exp
	"  fog = 1.0f - fog;\n   fog = pow(2, -8.0f * fog * fog);\n"	//backward exp2
};

static void WriteFog(char *&p)
{
	if(bpmem.fog.c_proj_fsel.fsel == 0)return;//no Fog

	if (bpmem.fog.c_proj_fsel.proj == 0) 
	{
        // perspective
        // ze = A/(B - Zs)
        WRITE (p, "  float ze = "I_FOG"[1].x / ("I_FOG"[1].y - depth);\n");
    } 
	else 
	{
        // orthographic
        // ze = a*Zs
        WRITE (p, "  float ze = "I_FOG"[1].x * depth;\n");
    }

	WRITE (p, "  float fog = saturate(ze - "I_FOG"[1].z);\n");

	if(bpmem.fog.c_proj_fsel.fsel > 3)
	{
		WRITE(p, "%s", tevFogFuncsTable[bpmem.fog.c_proj_fsel.fsel]);
	}
	else
	{
		if(bpmem.fog.c_proj_fsel.fsel != 2)
			WARN_LOG(VIDEO, "Unknown Fog Type! %08x", bpmem.fog.c_proj_fsel.fsel);
	}		

    WRITE(p, "  prev.rgb = lerp(prev.rgb,"I_FOG"[0].rgb,fog);\n");
    
}
