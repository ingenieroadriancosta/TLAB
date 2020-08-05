struct colorRGB{ BYTE B,G,R; };
typedef struct { BYTE length; WORD value;} bitstring;
bitstring* compute_Huffman_table( BYTE*, BYTE*, bitstring* );

const BYTE scalefactor = 0;
const RGBQUAD BlackColor = { 0, 0, 0, 0 };
const RGBQUAD GreenColor = { 0, 255, 0, 0 };
const BYTE zigzag[64]={ 0, 1, 5, 6,14,15,27,28,
		  2, 4, 7,13,16,26,29,42,
		  3, 8,12,17,25,30,41,43,
		  9,11,18,24,31,40,44,53,
		 10,19,23,32,39,45,52,54,
		 20,22,33,38,46,51,55,60,
		 21,34,37,47,50,56,59,61,
		 35,36,48,49,57,58,62,63 };
const BYTE std_luminance_qt[64] = {
	16,  11,  10,  16,  24,  40,  51,  61,
	12,  12,  14,  19,  26,  58,  60,  55,
	14,  13,  16,  24,  40,  57,  69,  56,
	14,  17,  22,  29,  51,  87,  80,  62,
	18,  22,  37,  56,  68, 109, 103,  77,
	24,  35,  55,  64,  81, 104, 113,  92,
	49,  64,  78,  87, 103, 121, 120, 101,
	72,  92,  95,  98, 112, 100, 103,  99
  };
const BYTE std_chrominance_qt[64] = {
	17,  18,  24,  47,  99,  99,  99,  99,
	18,  21,  26,  66,  99,  99,  99,  99,
	24,  26,  56,  99,  99,  99,  99,  99,
	47,  66,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99
  };
const BYTE std_dc_luminance_nrcodes[17]={0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0};
const BYTE std_dc_luminance_values[12]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

const BYTE std_dc_chrominance_nrcodes[17]={0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0};
const BYTE std_dc_chrominance_values[12]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

const BYTE std_ac_luminance_nrcodes[17]={0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d };
const BYTE std_ac_luminance_values[162]= {
	  0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
	  0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
	  0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
	  0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
	  0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
	  0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
	  0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
	  0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
	  0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
	  0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
	  0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
	  0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	  0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	  0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
	  0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
	  0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
	  0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
	  0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
	  0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
	  0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	  0xf9, 0xfa };

const BYTE std_ac_chrominance_nrcodes[17]={0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77};
const BYTE std_ac_chrominance_values[162]={
	  0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
	  0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
	  0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
	  0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
	  0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
	  0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
	  0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
	  0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	  0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	  0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	  0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	  0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	  0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
	  0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
	  0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
	  0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
	  0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
	  0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
	  0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
	  0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	  0xf9, 0xfa };
/// ///////////////////////////////////////////////////////////
#define BYTE unsigned char
#define SBYTE signed char
#define SWORD signed short int
#define WORD unsigned short int
#define DWORD unsigned long int
#define SDWORD signed long int

struct APP0infotype {
    APP0infotype(){
        marker = 0xFFE0;
        length = 0x0010;
        JFIFsignature[0] = 'J';
        JFIFsignature[1] = 'F';
        JFIFsignature[2] = 'I';
        JFIFsignature[3] = 'F';
        JFIFsignature[4] = 0;
        versionhi = 1;
        versionlo = 1;
        xyunits = 0;
        xdensity = 1;
        ydensity = 1;
        thumbnwidth = 0;
        thumbnheight = 0;
    }
		WORD marker;// = 0xFFE0
		WORD length; // = 16 for usual JPEG, no thumbnail
		BYTE JFIFsignature[5]; // = "JFIF",'\0'
		BYTE versionhi; // 1
		BYTE versionlo; // 1
		BYTE xyunits;   // 0 = no units, normal density
		WORD xdensity;  // 1
		WORD ydensity;  // 1
		BYTE thumbnwidth; // 0
		BYTE thumbnheight; // 0
// } APP0info={0xFFE0,16,'J','F','I','F',0,1,1,0,1,1,0,0};
};
const APP0infotype APP0info;

struct  SOF0infotype {
		WORD marker; // = 0xFFC0
		WORD length; // = 17 for a truecolor YCbCr JPG
		BYTE precision ;// Should be 8: 8 bits/sample
		WORD height ;
		WORD width;
		BYTE nrofcomponents;//Should be 3: We encode a truecolor JPG
		BYTE IdY;  // = 1
		BYTE HVY; // sampling factors for Y (bit 0-3 vert., 4-7 hor.)
		BYTE QTY;  // Quantization Table number for Y = 0
		BYTE IdCb; // = 2
		BYTE HVCb;
		BYTE QTCb; // 1
		BYTE IdCr; // = 3
		BYTE HVCr;
		BYTE QTCr; // Normally equal to QTCb = 1
} SOF0info = { 0xFFC0,17,8,0,0,3,1,0x11,0,2,0x11,1,3,0x11,1};
// Default sampling factors are 1,1 for every image component: No downsampling

struct DQTinfotype {
    DQTinfotype(){
        marker = 0xFFDB;
        length = 132;
        QTYinfo = 0;
        QTCbinfo = 1;
        long temp;
        for (int i=0; i<64; i++){
            temp = ((long) std_luminance_qt[i] * scalefactor + 50L ) / 100L;
            if (temp <= 0L)
                temp = 1L;
            if (temp > 255L)
                temp = 255L;
            Ytable[zigzag[i]] = (BYTE) temp;
            temp = ((long) std_chrominance_qt[i] * scalefactor + 50L ) / 100L;
            if (temp <= 0L)
                temp = 1L;
            if (temp > 255L)
                temp = 255L;
            Ytable[zigzag[i]] = (BYTE) temp;
            Cbtable[zigzag[i]] = (BYTE) temp;
        }
    }
		 WORD marker;  // = 0xFFDB
		 WORD length;  // = 132
		 BYTE QTYinfo;// = 0:  bit 0..3: number of QT = 0 (table for Y)
				  //       bit 4..7: precision of QT, 0 = 8 bit
		 BYTE Ytable[64];
		 BYTE QTCbinfo; // = 1 (quantization table for Cb,Cr}
		 BYTE Cbtable[64];
		   } ;
const DQTinfotype DQTinfo;


struct DHTinfotype {
    DHTinfotype(){
        marker = 0xFFC4;
        length = 0x01A2;
        HTYDCinfo = 0;
        HTYACinfo = 0x10;
        HTCbDCinfo = 1;
        HTCbACinfo = 0x11;
        for (int i=0; i<16; i++){
            CbAC_nrcodes[i] = std_ac_chrominance_nrcodes[i+1];
            YDC_nrcodes[i] = std_dc_luminance_nrcodes[i+1];
            YAC_nrcodes[i] = std_ac_luminance_nrcodes[i+1];
            CbDC_nrcodes[i] = std_dc_chrominance_nrcodes[i+1];
        }
        for ( int i=0; i<162; i++){
            YAC_values[i] = std_ac_luminance_values[i];
            CbAC_values[i] = std_ac_chrominance_values[i];
        }
        for(int i=0; i<12; i++ ){
            CbDC_values[i] = std_dc_chrominance_values[i];
            YDC_values[i] = std_dc_luminance_values[i];
        }
    }
		 WORD marker;  // = 0xFFC4
		 WORD length;  //0x01A2
		 BYTE HTYDCinfo; // bit 0..3: number of HT (0..3), for Y =0
			  //bit 4  :type of HT, 0 = DC table,1 = AC table
				//bit 5..7: not used, must be 0
		 BYTE YDC_nrcodes[16]; //at index i = nr of codes with length i
		 BYTE YDC_values[12];
		 BYTE HTYACinfo; // = 0x10
		 BYTE YAC_nrcodes[16];
		 BYTE YAC_values[162];//we'll use the standard Huffman tables
		 BYTE HTCbDCinfo; // = 1
		 BYTE CbDC_nrcodes[16];
		 BYTE CbDC_values[12];
		 BYTE HTCbACinfo; //  = 0x11
		 BYTE CbAC_nrcodes[16];
		 BYTE CbAC_values[162];
} ;
const DHTinfotype DHTinfo;

struct SOSinfotype {
		 WORD marker;  // = 0xFFDA
		 WORD length; // = 12
		 BYTE nrofcomponents; // Should be 3: truecolor JPG
		 BYTE IdY; //1
		 BYTE HTY; //0 // bits 0..3: AC table (0..3)
				   // bits 4..7: DC table (0..3)
		 BYTE IdCb; //2
		 BYTE HTCb; //0x11
		 BYTE IdCr; //3
		 BYTE HTCr; //0x11
		 BYTE Ss,Se,Bf; // not interesting, they should be 0,63,0
} SOSinfo={0xFFDA,12,3,1,0,2,0x11,3,0x11,0,0x3F,0};


#define  Y(R,G,B) ((BYTE)( (YRtab[(R)]+YGtab[(G)]+YBtab[(B)])>>16 ) - 128)
#define Cb(R,G,B) ((BYTE)( (CbRtab[(R)]+CbGtab[(G)]+CbBtab[(B)])>>16 ) )
#define Cr(R,G,B) ((BYTE)( (CrRtab[(R)]+CrGtab[(G)]+CrBtab[(B)])>>16 ) )

#define writebyte(b) fputc((b),fp_jpeg_stream)
#define writeword(w) writebyte((w)/256);writebyte((w)%256);

const WORD mask[16]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};

// The Huffman tables we'll use:
const bitstring* YDC_HT  = compute_Huffman_table( (BYTE *)&std_dc_luminance_nrcodes[0], (BYTE *)&std_dc_luminance_values[0], NULL );
const bitstring* YAC_HT  = compute_Huffman_table( (BYTE *)&std_ac_luminance_nrcodes[0], (BYTE *)&std_ac_luminance_values[0], NULL );
const bitstring* CbDC_HT = compute_Huffman_table( (BYTE *)&std_dc_chrominance_nrcodes[0], (BYTE *)&std_dc_chrominance_values[0], NULL );
const bitstring* CbAC_HT = compute_Huffman_table( (BYTE *)&std_ac_chrominance_nrcodes[0], (BYTE *)&std_ac_chrominance_values[0], NULL );






bitstring* Fillbitcode_alloc();
BYTE *category_alloc = (BYTE*)malloc(65535*sizeof(BYTE));
BYTE *category = category_alloc + 32767;
const bitstring *bitcode_alloc = (bitstring*)malloc(65535*sizeof(bitstring));
const bitstring *bitcode = Fillbitcode_alloc();
bitstring* Fillbitcode_alloc(){
    bitstring *bitcode = (bitstring *)bitcode_alloc + 32767;
    SDWORD nrlower, nrupper; BYTE cat; SDWORD nr; nrlower = 1; nrupper = 2;
	for (cat=1; cat<=15; cat++){
		for (nr=nrlower; nr<nrupper; nr++){
			category[nr] = cat;
			bitcode[nr].length = cat;
			bitcode[nr].value = (WORD)nr;
		}
		for (nr=-(nrupper-1); nr<=-nrlower; nr++){
			category[nr] = cat;
			bitcode[nr].length = cat;
			bitcode[nr].value = (WORD)(nrupper-1+nr);
		}
		nrlower <<= 1;
		nrupper <<= 1;
	}
    return bitcode;
 }
SDWORD YRtab[256],
YGtab[256],
YBtab[256];
SDWORD CbRtab[256],CbGtab[256],CbBtab[256];
SDWORD CrRtab[256],CrGtab[256],CrBtab[256];
float fdtbl_Y[64];
float fdtbl_Cb[64];
#define write_commentA(comment, fp_jpeg_stream ) write_comment((unsigned char*)comment, fp_jpeg_stream)
void write_comment(BYTE *comment, FILE* fp_jpeg_stream )
{
	WORD i, length;

	writeword(0xFFFE); // The COM marker
	length = strlen((const char *)comment);
	writeword(length + 2);
	for (i=0; i<length; i++)
		writebyte(comment[i]);
}

void writebits(bitstring bs, BYTE* bytenew, SBYTE* bytepos, FILE* fp_jpeg_stream ){
    // fp_jpeg_stream
	WORD value;
	SBYTE posval;
	value = bs.value;
	posval = bs.length - 1;
	while (posval >= 0)
	{
		if (value & mask[posval])
			bytenew[0] |= mask[bytepos[0]];
		posval--;
		bytepos[0]--;
		if (bytepos[0] < 0)
		{
			// write it
			if (bytenew[0] == 0xFF)
			{
				// special case
				writebyte(0xFF);
				writebyte(0);
			}
			else
				writebyte(bytenew[0]);

			// reinit
			bytepos[0] = 7;
			bytenew[0] = 0;
		}
	}
}

bitstring* compute_Huffman_table(BYTE *nrcodes, BYTE *std_table, bitstring *HT){
	BYTE k,j;
	BYTE pos_in_table;
	WORD codevalue;
    bitstring* HTR = (bitstring*)malloc( 256 * sizeof(bitstring) );//AQUI
	codevalue = 0;
	pos_in_table = 0;
	for( k=1; k<=16; k++ ){
		for (j=1; j<=nrcodes[k]; j++){
            if( HT!=NULL ){
                HT[std_table[pos_in_table]].value = codevalue;
                HT[std_table[pos_in_table]].length = k;
            }
			//
			HTR[std_table[pos_in_table]].value = codevalue;
			HTR[std_table[pos_in_table]].length = k;
			pos_in_table++;
			codevalue++;
		}
		codevalue <<= 1;
	}
	return HTR;
}


void fdct_and_quantization(SBYTE *data, float *fdtbl, SWORD *outdata)
{
	float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	float tmp10, tmp11, tmp12, tmp13;
	float z1, z2, z3, z4, z5, z11, z13;
	float *dataptr;
	float datafloat[64];
	float temp;
	SBYTE ctr;
	BYTE i;

	for (i=0; i<64; i++)
		datafloat[i] = data[i];

	dataptr = datafloat;
	for (ctr = 7; ctr >= 0; ctr--)
	{
		tmp0 = dataptr[0] + dataptr[7];
		tmp7 = dataptr[0] - dataptr[7];
		tmp1 = dataptr[1] + dataptr[6];
		tmp6 = dataptr[1] - dataptr[6];
		tmp2 = dataptr[2] + dataptr[5];
		tmp5 = dataptr[2] - dataptr[5];
		tmp3 = dataptr[3] + dataptr[4];
		tmp4 = dataptr[3] - dataptr[4];

		/* Even part */

		tmp10 = tmp0 + tmp3;	/* phase 2 */
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;

		dataptr[0] = tmp10 + tmp11; /* phase 3 */
		dataptr[4] = tmp10 - tmp11;

		z1 = (tmp12 + tmp13) * ((float) 0.707106781); /* c4 */
		dataptr[2] = tmp13 + z1;	/* phase 5 */
		dataptr[6] = tmp13 - z1;

		/* Odd part */

		tmp10 = tmp4 + tmp5;	/* phase 2 */
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;

		/* The rotator is modified from fig 4-8 to avoid extra negations. */
		z5 = (tmp10 - tmp12) * ((float) 0.382683433); /* c6 */
		z2 = ((float) 0.541196100) * tmp10 + z5; /* c2-c6 */
		z4 = ((float) 1.306562965) * tmp12 + z5; /* c2+c6 */
		z3 = tmp11 * ((float) 0.707106781); /* c4 */

		z11 = tmp7 + z3;		/* phase 5 */
		z13 = tmp7 - z3;

		dataptr[5] = z13 + z2;	/* phase 6 */
		dataptr[3] = z13 - z2;
		dataptr[1] = z11 + z4;
		dataptr[7] = z11 - z4;

		dataptr += 8;		/* advance pointer to next row */
	}

  /* Pass 2: process columns. */

	dataptr = datafloat;
	for (ctr = 7; ctr >= 0; ctr--)
	{
		tmp0 = dataptr[0] + dataptr[56];
		tmp7 = dataptr[0] - dataptr[56];
		tmp1 = dataptr[8] + dataptr[48];
		tmp6 = dataptr[8] - dataptr[48];
		tmp2 = dataptr[16] + dataptr[40];
		tmp5 = dataptr[16] - dataptr[40];
		tmp3 = dataptr[24] + dataptr[32];
		tmp4 = dataptr[24] - dataptr[32];

		/* Even part */

		tmp10 = tmp0 + tmp3;	/* phase 2 */
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;

		dataptr[0] = tmp10 + tmp11; /* phase 3 */
		dataptr[32] = tmp10 - tmp11;

		z1 = (tmp12 + tmp13) * ((float) 0.707106781); /* c4 */
		dataptr[16] = tmp13 + z1; /* phase 5 */
		dataptr[48] = tmp13 - z1;

		/* Odd part */

		tmp10 = tmp4 + tmp5;	/* phase 2 */
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;

		/* The rotator is modified from fig 4-8 to avoid extra negations. */
		z5 = (tmp10 - tmp12) * ((float) 0.382683433); /* c6 */
		z2 = ((float) 0.541196100) * tmp10 + z5; /* c2-c6 */
		z4 = ((float) 1.306562965) * tmp12 + z5; /* c2+c6 */
		z3 = tmp11 * ((float) 0.707106781); /* c4 */

		z11 = tmp7 + z3;		/* phase 5 */
		z13 = tmp7 - z3;

		dataptr[40] = z13 + z2; /* phase 6 */
		dataptr[24] = z13 - z2;
		dataptr[8] = z11 + z4;
		dataptr[56] = z11 - z4;

		dataptr++;			/* advance pointer to next column */
	}

	/* Quantize/descale the coefficients, and store into output array */
	for (i = 0; i < 64; i++)
	{
		/* Apply the quantization and scaling factor */
		temp = datafloat[i] * fdtbl[i];

	/* Round to nearest integer.
	   Since C does not specify the direction of rounding for negative
	   quotients, we have to force the dividend positive for portability.
	   The maximum coefficient size is +-16K (for 12-bit data), so this
	   code should work for either 16-bit or 32-bit ints.
	*/
		outdata[i] = (SWORD) ((SWORD)(temp + 16384.5) - 16384);
	}
}

void process_DU(SBYTE *ComponentDU,float *fdtbl,SWORD *DC,
				bitstring *HTDC,bitstring *HTAC, BYTE* bytenew, SBYTE* bytepos, FILE* fp_jpeg_stream )
{
	bitstring EOB = HTAC[0x00];
	bitstring M16zeroes = HTAC[0xF0];
	BYTE i;
	BYTE startpos;
	BYTE end0pos;
	BYTE nrzeroes;
	BYTE nrmarker;
	SWORD Diff;
    SWORD DU[64];
    SWORD DU_DCT[64];
	fdct_and_quantization(ComponentDU, fdtbl, DU_DCT);
	// zigzag reorder
	for (i=0; i<64; i++)
		DU[zigzag[i]]=DU_DCT[i];

	// Encode DC
	Diff = DU[0] - *DC;
	*DC = DU[0];

	if (Diff == 0)
		writebits(HTDC[0], bytenew, bytepos, fp_jpeg_stream );
	else
	{
		writebits(HTDC[category[Diff]], bytenew, bytepos, fp_jpeg_stream );
		writebits(bitcode[Diff], bytenew, bytepos, fp_jpeg_stream );
	}

	// Encode ACs
	for (end0pos=63; (end0pos>0)&&(DU[end0pos]==0); end0pos--) ;
	//end0pos = first element in reverse order != 0

	i = 1;
	while (i <= end0pos)
	{
		startpos = i;
		for (; (DU[i]==0) && (i<=end0pos); i++) ;
		nrzeroes = i - startpos;
		if (nrzeroes >= 16)
		{
			for (nrmarker=1; nrmarker<=nrzeroes/16; nrmarker++)
				writebits(M16zeroes, bytenew, bytepos, fp_jpeg_stream );
			nrzeroes = nrzeroes%16;
		}
		writebits(HTAC[nrzeroes*16+category[DU[i]]], bytenew, bytepos, fp_jpeg_stream );
		writebits(bitcode[DU[i]], bytenew, bytepos, fp_jpeg_stream );
		i++;
	}

	if (end0pos != 63)
		writebits(EOB, bytenew, bytepos, fp_jpeg_stream );
}





colorRGB *RGB_buffer = NULL;
WORD width_original = 0;
WORD height_original = 0;
void SAVE_AS_JPG( LPCSTR JPG_filename,
                    int WidthBig,
                        int HeightBig,
                        RGBQUAD* RGBBIG ){
    FILE *fp_jpeg_stream;
    WORD width  = 0;WORD height = 0;
    bitstring fillbits;
////////////////////////////////////////////////////////////////////////////////
    width = WidthBig;
    while( width%8!=0 ){
        width++;
    }
    height = HeightBig;
    while( height%8!=0 ){
        height++;
    }

    if( RGB_buffer==NULL || width_original!=WidthBig ){
        if( RGB_buffer==NULL ){ free(RGB_buffer); }
        RGB_buffer = (colorRGB*)(malloc(3*width*height+1024));
        if( RGB_buffer==NULL ){
            exit( -123 );
        }
    }
    width_original = WidthBig;
    height_original = HeightBig;
    int JPGIndex = 0;
    int FromIndex = 0;
    for( int y=0; y<height; y++ ){
        FromIndex = y*WidthBig;
         for( int x=0; x<width; x++ ){
              if( y>=HeightBig || x>=WidthBig ){
                 RGB_buffer[JPGIndex].B = 0;
                 RGB_buffer[JPGIndex].G = 0;
                 RGB_buffer[JPGIndex].R = 0;
                 JPGIndex++;
              }else{
                 RGB_buffer[JPGIndex].B = RGBBIG[FromIndex].rgbBlue;
                 RGB_buffer[JPGIndex].G = RGBBIG[FromIndex].rgbGreen;
                 RGB_buffer[JPGIndex].R = RGBBIG[FromIndex].rgbRed;
                 FromIndex++;
                 JPGIndex++;
              }
        }
    }
////////////////////////////////////////////////////////////////////////////////
    fp_jpeg_stream = fopen( JPG_filename, "wb" );


    JPGIndex = 0;
	for ( int i=0; i<256; i++){
		YRtab[i] = (SDWORD)(65536*0.299+0.5)*i;
		CbRtab[i] = (SDWORD)(65536*-0.16874+0.5)*i;
		CrRtab[i] = (SDWORD)(32768)*i;
		//
		YGtab[i] = (SDWORD)(65536*0.587+0.5)*i;
		CbGtab[i] = (SDWORD)(65536*-0.33126+0.5)*i;
		CrGtab[i] = (SDWORD)(65536*-0.41869+0.5)*i;
		//
		YBtab[i] = (SDWORD)(65536*0.114+0.5)*i;
		CbBtab[i] = (SDWORD)(32768)*i;
		CrBtab[i] = (SDWORD)(65536*-0.08131+0.5)*i;
	}
	//
    double aanscalefactor[8] = {1.0, 1.387039845, 1.306562965, 1.175875602,
		1.0, 0.785694958, 0.541196100, 0.275899379};
	BYTE row, col;
	BYTE i = 0;
	for (row = 0; row < 8; row++){
		for (col = 0; col < 8; col++){
			fdtbl_Y[i] = (float) (1.0 / ((double) DQTinfo.Ytable[zigzag[i]] *
                                    aanscalefactor[row] * aanscalefactor[col] * 8.0));
			fdtbl_Cb[i] = (float) (1.0 / ((double) DQTinfo.Cbtable[zigzag[i]] *
                                    aanscalefactor[row] * aanscalefactor[col] * 8.0));
			i++;
		}
	}
    SOF0info.width = width_original;
    SOF0info.height = height_original;
    writeword(0xFFD8); // SOI
    writeword(APP0info.marker);
	writeword(APP0info.length);
	writebyte('J');
	writebyte('F');
	writebyte('I');
	writebyte('F');
	writebyte(0);
	writebyte(APP0info.versionhi);
	writebyte(APP0info.versionlo);
	writebyte(APP0info.xyunits);
	writeword(APP0info.xdensity);
	writeword(APP0info.ydensity);
	writebyte(APP0info.thumbnwidth);
	writebyte(APP0info.thumbnheight);

    write_commentA( "NONE\n", fp_jpeg_stream );
    writeword(DQTinfo.marker);
	writeword(DQTinfo.length);
	writebyte(DQTinfo.QTYinfo);
	for (int i=0; i<64; i++){
	    writebyte(DQTinfo.Ytable[i]);
	}
	writebyte(DQTinfo.QTCbinfo);
	for (int i=0; i<64; i++){
	    writebyte(DQTinfo.Cbtable[i]);
	}

    writeword(SOF0info.marker);
	writeword(SOF0info.length);
	writebyte(SOF0info.precision);
	writeword(SOF0info.height);
	writeword(SOF0info.width);
	writebyte(SOF0info.nrofcomponents);
	writebyte(SOF0info.IdY);
	writebyte(SOF0info.HVY);
	writebyte(SOF0info.QTY);
	writebyte(SOF0info.IdCb);
	writebyte(SOF0info.HVCb);
	writebyte(SOF0info.QTCb);
	writebyte(SOF0info.IdCr);
	writebyte(SOF0info.HVCr);
	writebyte(SOF0info.QTCr);

    writeword(DHTinfo.marker);
	writeword(DHTinfo.length);
	writebyte(DHTinfo.HTYDCinfo);
	for (int i=0; i<16; i++)
		writebyte(DHTinfo.YDC_nrcodes[i]);
	for (int i=0; i<12; i++)
		writebyte(DHTinfo.YDC_values[i]);
	writebyte(DHTinfo.HTYACinfo);
	for (int i=0; i<16; i++)
		writebyte(DHTinfo.YAC_nrcodes[i]);
	for (int i=0; i<162; i++)
		writebyte(DHTinfo.YAC_values[i]);
	writebyte(DHTinfo.HTCbDCinfo);
	for (int i=0; i<16; i++)
		writebyte(DHTinfo.CbDC_nrcodes[i]);
	for (int i=0; i<12; i++)
		writebyte(DHTinfo.CbDC_values[i]);
	writebyte(DHTinfo.HTCbACinfo);
	for (int i=0; i<16; i++)
		writebyte(DHTinfo.CbAC_nrcodes[i]);
	for (int i=0; i<162; i++)
		writebyte(DHTinfo.CbAC_values[i]);

    writeword(SOSinfo.marker);
	writeword(SOSinfo.length);
	writebyte(SOSinfo.nrofcomponents);
	writebyte(SOSinfo.IdY);
	writebyte(SOSinfo.HTY);
	writebyte(SOSinfo.IdCb);
	writebyte(SOSinfo.HTCb);
	writebyte(SOSinfo.IdCr);
	writebyte(SOSinfo.HTCr);
	writebyte(SOSinfo.Ss);
	writebyte(SOSinfo.Se);
	writebyte(SOSinfo.Bf);

    BYTE bytenew = 0;
    SBYTE bytepos = 7;
    SWORD DCY = 0, DCCb = 0, DCCr = 0; //DC coefficients used for differential encoding
	WORD xpos, ypos;

	for (ypos=0; ypos<height; ypos+=8){
		for (xpos=0; xpos<width; xpos+=8){
			BYTE x, y;
	BYTE pos = 0;
	DWORD location;
	BYTE R, G, B;
	SBYTE YDU[64];
	SBYTE CbDU[64];

 SBYTE CrDU[64];
  // Current DU (after DCT and quantization) which we'll zigzag
  //zigzag reordered DU which will be Huffman coded

//
	location = ypos * width + xpos;
	for (y=0; y<8; y++)
	{
		for (x=0; x<8; x++)
		{
			R = RGB_buffer[location].R;
			G = RGB_buffer[location].G;
			B = RGB_buffer[location].B;
			// convert to YCbCr
			YDU[pos] = Y(R,G,B);
			CbDU[pos] = Cb(R,G,B);
			CrDU[pos] = Cr(R,G,B);
			location++;
			pos++;
		}
		location += width - 8;
	}
	//
			process_DU(YDU, fdtbl_Y, &DCY, (bitstring*)&YDC_HT[0], (bitstring*)&YAC_HT[0], &bytenew, &bytepos, fp_jpeg_stream );
			process_DU(CbDU, fdtbl_Cb, &DCCb, (bitstring*)&CbDC_HT[0], (bitstring*)&CbAC_HT[0], &bytenew, &bytepos, fp_jpeg_stream );
			process_DU(CrDU, fdtbl_Cb, &DCCr, (bitstring*)&CbDC_HT[0], (bitstring*)&CbAC_HT[0], &bytenew, &bytepos, fp_jpeg_stream );
		}
	}

    if (bytepos >= 0){
        fillbits.length = bytepos + 1;
        fillbits.value = (1<<(bytepos+1)) - 1;
        writebits(fillbits, &bytenew, &bytepos, fp_jpeg_stream );
    }
    writeword(0xFFD9); // EOI
    free( RGB_buffer );
    RGB_buffer = NULL;
    fclose(fp_jpeg_stream);
}


